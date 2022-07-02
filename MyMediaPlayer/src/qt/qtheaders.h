#ifndef QTHEADERS_H
#define QTHEADERS_H



// 只在VS下生效，解决乱码
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#define DATE qPrintable(QDate::currentDate().toString("yyyy-MM-dd"))
#define TIME qPrintable(QTime::currentTime().toString("hh:mm:ss"))
#define DATETIME qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))

#define TIMEMS qPrintable(QTime::currentTime().toString("hh:mm:ss zzz"))
#define DATETIMENBSP qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"))
#define DATETIMEMSNBSP qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz"))

#define DESKTOP_WIDTH   qApp->primaryScreen()->availableGeometry().width()
#define DESKTOP_HEIGHT  qApp->primaryScreen()->availableGeometry().height()

#endif // QTHEADERS_H
