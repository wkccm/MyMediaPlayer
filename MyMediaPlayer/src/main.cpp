#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <QFile>


#include "appdef.h"
#include "confile.h"
#include "hv.h"
#include "qtstyles.h"
#include "qtfunctions.h"

IniParser* g_confile = NULL;
char g_exec_path[256] = {0};
char g_exec_dir[256] ={0};
char g_run_dir[256] = {0};
char g_conf_file[256] = {0};
char g_log_file[256] = {0};

#define LOG_LEVEL  0

    // 日志句柄
static void qLogHandler(QtMsgType type, const QMessageLogContext &ctx, const QString &msg)
{
    if(type < LOG_LEVEL)
    {
        return;
    }

    static char s_types[5][6] = {"DEBUG", "WARN", "ERROR", "FATAL", "INFO"};
    const char* szType = "DEBUG";
    if(type < 5)
    {
        szType = s_types[type];
    }

//调试模式和发行模式不同的日志输出
#ifdef QT_NO_DEBUG
    QString strLog = QString::asprintf("[%s][%s] %s\n",
                                       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").toLocal8Bit().data(),
                                       szType,
                                       msg.toLocal8Bit().data());
#else
    QString strLog = QString::asprintf("[%s][%s] %s [%s:%d-%s]\n",
                                       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").toLocal8Bit().data(),
                                       szType,
                                       msg.toLocal8Bit().data(),
                                       ctx.file,ctx.line,ctx.function);
#endif

    // 处理文件流，大于2M时重置
    static FILE* s_fp = NULL;
    if (s_fp) {
        fseek(s_fp, 0, SEEK_END);
        if (ftell(s_fp) > (2 << 20)) {
            fclose(s_fp);
            s_fp = NULL;
        }
    }
    // 若上一步被重置，则建立一个新的日志文件
    if (!s_fp) {
        char logfile[256];
        snprintf(logfile, sizeof(logfile), "%s/logs/qt.log", g_exec_dir);
        s_fp = fopen(logfile, "w");
    }
    // 将日志写入文件流
    if (s_fp) {
        fputs(strLog.toLocal8Bit().data(), s_fp);
    }
}

static int load_confile()
{
    get_executable_path(g_exec_path, sizeof(g_exec_path));
    get_executable_dir(g_exec_dir, sizeof(g_exec_dir));
    get_run_dir(g_run_dir, sizeof(g_run_dir));

    g_confile = new IniParser;
    snprintf(g_conf_file, sizeof(g_conf_file), "%s/conf/%s.conf", g_exec_dir, APP_NAME);
    if(access(g_conf_file, 0) != 0)
    {
        QFile::copy(QString(g_exec_dir) + "/conf/" APP_NAME ".conf.default", g_conf_file);
    }
    g_confile->LoadFromFile(g_conf_file);

    std::string str = g_confile->GetValue("logfile");
    if(str.empty())
    {
        snprintf(g_log_file, sizeof(g_log_file), "%s/logs", g_exec_dir);
        hv_mkdir_p(g_log_file);
        snprintf(g_log_file, sizeof(g_log_file), "%s/logs/%s.log", g_exec_dir, APP_NAME);
    }
    else {
        strncpy(g_log_file, str.c_str(), sizeof(g_log_file));
    }
    hlog_set_file(g_log_file);

    str = g_confile->GetValue("loglevel");
    if (!str.empty()) {
        hlog_set_level_by_str(str.c_str());
    }

    str = g_confile->GetValue("log_filesize");
    if (!str.empty()) {
        hlog_set_max_filesize_by_str(str.c_str());
    }

    str = g_confile->GetValue("log_remain_days");
    if (!str.empty()) {
        hlog_set_remain_days(atoi(str.c_str()));
    }
    // hv_getboolean在hbase.h中
    str = g_confile->GetValue("log_fsync");
    if (!str.empty()) {
        logger_enable_fsync(hlog, hv_getboolean(str.c_str()));
    }

    hlogi("%s version: %s", g_exec_path, hv_compile_version());
    hlog_fsync();
}

int main(int argc, char *argv[])
{
    load_confile();

    qInstallMessageHandler(qLogHandler);
    qInfo("----------------app start---------------");
    QApplication app(argc, argv);
    app.setApplicationName(APP_NAME);

    std::string str = g_confile->GetValue("skin", "ui");
    loadSkin(str.empty()?DEFAULT_SKIN : str.c_str());

    str = g_confile->GetValue("palette", "ui");
    setPalette(str.empty() ? DEFAULT_PALETTE_COLOR : strtoul(str.c_str(), NULL, 16));

    str = g_confile->GetValue("language", "ui");
    loadLang(str.empty() ? DEFAULT_LANGUAGE : str.c_str());

    setFont(g_confile->Get<int>("fontsize", "ui", DEFAULT_FONT_SIZE));

    MainWindow::instance();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
