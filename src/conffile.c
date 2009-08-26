#include <stdio.h>
#include <stdlib.h>

#include "conffile.h"

/* FIXME */
#define BUFSIZE 4096

static int add_config_entry(config_t* config, char* config_line)
{
    char* keysym_s = NULL;
    char* app_service = NULL;
    char* app_action = NULL;
    char* app_cmdline = NULL;
    int r = sscanf(config_line, " %ms %ms %ms %m[^\n]",
                   &keysym_s, &app_service, &app_action, &app_cmdline);
    if(4 != r)
        goto err;

    int keysym;
    bool is_alt = false;
    if(!strncasecmp(keysym_s, "alt-", 4))
    {
        is_alt = true;
        if(1 != sscanf(keysym_s + 4, "%d", &keysym))
            goto err;
    }
    else
        if(1 != sscanf(keysym_s, "%d", &keysym))
            goto err;

    hotkey_t* keys = realloc(config->keys, sizeof(hotkey_t) * (config->count + 1));
    if(!keys)
        goto err;

    keys[config->count].keysym = keysym;
    keys[config->count].is_alt = is_alt;
    keys[config->count].app_service = app_service;
    keys[config->count].app_action = app_action;
    keys[config->count].app_cmdline = app_cmdline;

    config->keys = keys;
    config->count++;

    return 0;

err:
    free(keysym_s);
    free(app_service);
    free(app_action);
    free(app_cmdline);
    return -1;
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

hotkey_t* lookup_key(const config_t* config, int keysym, bool is_alt)
{
    int i;
    for(i = 0; i < config->count; ++i)
        if(config->keys[i].keysym == keysym && config->keys[i].is_alt == is_alt)
            return config->keys + i;
    return NULL;
}

void free_config(config_t* config)
{
    int i;
    for(i = 0; i < config->count; ++i)
    {
        free(config->keys[i].app_service);
        free(config->keys[i].app_action);
        free(config->keys[i].app_cmdline);
    }
    free(config->keys);
    free(config);
}
