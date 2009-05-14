#ifndef CONFFILE_H
#define CONFFILE_H

typedef struct
{
    int keysym;
    char* app_service;
    char* app_action;
    char* app_cmdline;
} hotkey_t;

typedef struct
{
    hotkey_t* keys;
    int count;
} config_t;

config_t* parse_config(const char* config_file);

hotkey_t* lookup_key(const config_t* config, int keysym);

void free_config(config_t*);

#endif
