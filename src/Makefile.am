bin_PROGRAMS = uk uk-send
uk_SOURCES = conffile.c grabkeys.c main.c service.c
uk_send_SOURCES = uk-send.c service.c

AM_CFLAGS = -Wall

INCLUDES = @XCB_CFLAGS@ @XCB_UTIL_CFLAGS@ @LOPS_CFLAGS@
LDFLAGS = @XCB_LIBS@ @XCB_UTIL_LIBS@ @LOPS_LIBS@

