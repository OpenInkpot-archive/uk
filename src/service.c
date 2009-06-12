#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX 108
#endif

/*
 * /tmp/.ecore-$ENV{'USER'}/[name]/[port]
 */

char* service_path(const char* name, int port)
{
    char* user = getenv("USER");
    if(!user)
        user = getenv("LOGNAME");
    if(!user)
    {
        errno = EINVAL;
        return NULL;
    }

    char* ret;
    if(-1 == asprintf(&ret, "/tmp/.ecore_service-%s/%s/%d", user, name, port))
        return NULL;

    return ret;
}

int connect_service(const char* name)
{
   int s = socket(AF_UNIX, SOCK_STREAM, 0);
   if(s == -1)
       return -1;

   char* path = service_path(name, 0);
   if(!path)
       return -1;

   struct sockaddr_un addr = { AF_UNIX };
   strncpy(addr.sun_path, path, UNIX_PATH_MAX - 1);

   free(path);

   if(-1 == connect(s, (struct sockaddr*)&addr, sizeof(addr)))
   {
       int saved_errno = errno;
       close(s);
       errno = saved_errno;
       return -1;
   }

   return s;
}
