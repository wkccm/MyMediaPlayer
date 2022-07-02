#ifndef CONFILE_H
#define CONFILE_H

// 解析配置文件ini
#include "iniparser.h"

extern IniParser* g_confile;
extern char g_exec_path[256];
extern char g_exec_dir[256];
extern char g_run_dir[256];
extern char g_conf_file[256];
extern char g_log_file[256];

#endif // CONFILE_H
