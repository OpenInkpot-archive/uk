bin_PROGRAMS = uk uk-send
uk_SOURCES = conffile.c grabkeys.c main.c service.c
uk_send_SOURCES = uk-send.c service.c

AM_CFLAGS = -Wall

INCLUDES = @XCB_CFLAGS@ @XCB_UTIL_CFLAGS@ @LIBLOPS_CFLAGS@

uk_LDFLAGS = @XCB_LIBS@ @XCB_UTIL_LIBS@ @LIBLOPS_LIBS@
uk_send_LDFLAGS = @LIBLOPS_LIBS@

