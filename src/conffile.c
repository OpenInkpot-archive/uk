#include <stdio.h>
#include <stdlib.h>

#include "conffile.h"

/* FIXME */
#define BUFSIZE 4096

static int add_config_entry(config_t* config, char* config_line)
{
    int keysym;
    char* app_service = NULL;
    char* app_cmdline = NULL;
    int r = sscanf(config_line, " %d %ms %m[^\n]",
                   &keysym, &app_service, &app_cmdline);
    if(3 != r)
    {
        free(app_service);
        free(app_cmdline);
        return -1;
    }

    hotkey_t* keys = realloc(config->keys, sizeof(hotkey_t) * (config->count + 1));
    if(!keys)
    {
        free(app_service);
        free(app_cmdline);
        return -1;
    }

    keys[config->count].keysym = keysym;
    keys[config->count].app_service = app_service;
    keys[config->count].app_cmdline = app_cmdline;

    config->keys = keys;
    config->count++;

    return 0;
}

config_t* parse_config(const char* config_file)
{
    config_t* config = calloc(1, sizeof(config_t));
    if(!config)
        return NULL;

    FILE* f = fopen(config_file, "r");
    if(!f)
    {
        free_config(config);
        return NULL;
    }

    char buf[BUFSIZE];
    while(fgets(buf, BUFSIZE, f))
    {
        if(*buf == '#' || *buf == '\n' || !*buf) continue;
        if(-1 == add_config_entry(config, buf))
        {
            /* FIXME */
            fprintf(stderr, "Unable to parse config line: %s\n", buf);
        }
    }

    if(EOF == fclose(f))
    {
        free_config(config);
        return NULL;
    }

    return config;
}

hotkey_t* lookup_key(const config_t* config, int keysym)
{
    int i;
    for(i = 0; i < config->count; ++i)
        if(config->keys[i].keysym == keysym)
            return config->keys + i;
    return NULL;
}

void free_config(config_t* config)
{
    int i;
    for(i = 0; i < config->count; ++i)
    {
        free(config->keys[i].app_service);
        free(config->keys[i].app_cmdline);
    }
    free(config->keys);
    free(config);
}
