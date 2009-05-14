#ifndef LOPS_H
#define LOPS_H

#include <sys/types.h>

/*
 * This function is like read(2), but it reads until eof, size or error.
 */
ssize_t lread(int fd, void* buf, size_t count);

/*
 * This function is like write(2), but it writes until size or error.
 */
ssize_t lwrite(int fd, const void* buf, size_t count);

#endif
