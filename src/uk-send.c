#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "service.h"
#include "lops.h"

void
usage() {
    fprintf(stderr,"Usage: uk-send <app> <cmd>\n\t('port' is always 0)\n");
}

int
main(int argc, char **argv){
    int sock, rc;
    if(argc == 3) {
        sock = connect_service(argv[1]);
        if(sock == -1) {
            fprintf(stderr,"Can't connect: %s\n", strerror(errno));
            return 2;
        };
        rc = lwrite(sock, argv[2], strlen(argv[2]));
        close(sock);
        if(!rc)
            return 0;
    } else 
      usage();
    return 1;
}
