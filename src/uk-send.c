#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "service.h"
#include "lops.h"

void usage()
{
    fprintf(stderr,"Usage: uk-send <app> <cmd>\n\t('port' is always 0)\n");
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        usage();
        return 1;
    }

    int sock = connect_service(argv[1]);
    if(sock == -1)
    {
        perror("uk-send: connect_service");
        return 2;
    };

    if(0 != lwrite(sock, argv[2], strlen(argv[2])))
    {
        perror("uk-send: lwrite");
        return 1;
    }

    if(0 != close(sock))
    {
        perror("uk-send: close");
        return 1;
    }

    return 0;
}
