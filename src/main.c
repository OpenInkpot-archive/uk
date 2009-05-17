#include "conffile.h"
#include "service.h"
#include "lops.h"
#include "grabkeys.h"

#include <poll.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

void start_app(hotkey_t* key)
{
    /* Start the app */
    pid_t pid = fork();
    if(pid == -1)
    {
        /* FIXME */
        perror("ERROR: unable to fork");
        return;
    }

    if(pid == 0) /* child */
    {
        execl("/bin/sh", "/bin/sh", "-c", key->app_cmdline, NULL);
        fprintf(stderr, "ERROR: unable to spawn %s: %m\n",
                key->app_cmdline);
        exit(255);
    }
}

void key_pressed(const config_t* config, int keysym)
{
    hotkey_t* key = lookup_key(config, keysym);
    if(!key)
        return;

    if(!strcmp(key->app_service, "<none>"))
    {
        start_app(key);
        return;
    }

    /* FIXME: UGLY */
    int f = connect_service(key->app_service);
    if(f == -1 && (errno == ECONNREFUSED || errno == ENOENT))
    {
        start_app(key);

        /* Try to connect, waiting up to 10 seconds */
        struct pollfd fds[] = {};

        int i;
        for(i = 0; i < 1000; ++i)
        {
            f = connect_service(key->app_service);
            if(f != -1 || (errno != ECONNREFUSED && errno != ENOENT))
                break;

            poll(fds, 0, 10);
        }

        if(f == -1 && (errno == ECONNREFUSED || errno == ENOENT))
        {
            fprintf(stderr, "ERROR: spawned %s did not setup serivce %s\n",
                    key->app_cmdline, key->app_service);
            return;
        }
    }

    if(f == -1)
    {
        /* FIXME */
        fprintf(stderr, "Unable to connect to service %s: %m\n",
                key->app_service);
        return;
    }

    if(-1 == lwrite(f, key->app_action, strlen(key->app_action)))
    {
        close(f);
        return;
    }

    close(f);
    return;
}

void child_handler(int signum)
{
    wait(NULL);
}

#define DEFAULT_CONFIG "/etc/uk.conf"
#define USER_CONFIG "/.uk.conf"

/* FIXME */
#define BUFSIZE 4096

config_t* read_config()
{
    config_t* config = NULL;

    if(getenv("HOME"))
    {
        char buf[BUFSIZE];
        snprintf(buf, BUFSIZE, "%s" USER_CONFIG, getenv("HOME"));
        config = parse_config(buf);
    }

    if(!config)
        config = parse_config(DEFAULT_CONFIG);

    return config;
}

void usage()
{
    printf("Usage: uk [<config file>]\n");
}

int main(int argc, char** argv)
{
    signal(SIGCHLD, &child_handler);

    if(argc > 2)
    {
        usage();
        return 1;
    }

    config_t* config;

    if(argc == 2)
    {
        config = parse_config(argv[1]);
        if(!config)
        {
            fprintf(stderr, "ERROR: Unable to read config file '%s'\n", argv[1]);
            return 1;
        }
    }
    else
    {
        config = read_config();
        if(!config)
        {
            fprintf(stderr, "ERROR: Unable to read config file\n");
            return 1;
        }
    }

    run(config, &key_pressed);

    return 0;
}
