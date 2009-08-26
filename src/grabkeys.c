#include "conffile.h"
#include "grabkeys.h"

#include <stdlib.h>
#include <stdbool.h>
#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_event.h>

typedef struct
{
    config_t* config;
    key_handler_t key_handler;
} context_t;

static int handle_keypress(void* param, xcb_connection_t* c,
                           xcb_key_press_event_t* event)
{
    context_t* context = param;
    (*context->key_handler)(context->config, event->detail,
                            event->state & XCB_MOD_MASK_1);
    return 1;
}

int run(config_t* config, key_handler_t key_handler)
{
    int default_screen;
    xcb_connection_t* c = xcb_connect(NULL, &default_screen);

    if(xcb_connection_has_error(c))
    {
		fprintf(stderr, "Cannot open display %s\n",
                getenv("DISPLAY") ? getenv("DISPLAY") : "<NULL>");
        return -1;
    }
    xcb_screen_t* screen;
    if(!(screen = xcb_aux_get_screen(c, default_screen)))
    {
        fprintf(stderr, "Cannot obtain default screen.\n");
        return -1;
    }

    context_t context = { config, key_handler };

    xcb_event_handlers_t eh;
    memset(&eh, 0, sizeof(xcb_event_handlers_t));
    xcb_event_handlers_init(c, &eh);
    xcb_event_set_key_press_handler(&eh, handle_keypress, &context);

    xcb_void_cookie_t* cookies = alloca(sizeof(xcb_void_cookie_t)
                                             * config->count);
    if(!cookies)
        return -1;

    int i;
    for(i = 0; i < config->count; ++i)
        cookies[i] = xcb_grab_key(c, true, screen->root,
                                  config->keys[i].is_alt ? XCB_MOD_MASK_1 : 0,
                                  config->keys[i].keysym, XCB_GRAB_MODE_ASYNC,
                                  XCB_GRAB_MODE_ASYNC);

    for(i = 0; i < config->count; ++i)
    {
        xcb_generic_error_t* e = xcb_request_check(c, cookies[i]);
        if(e)
        {
            fprintf(stderr, "WARNING: unable to grab key: %d\n", e->error_code);
            free(e);
        }
    }

    xcb_generic_event_t* e;
    while((e = xcb_wait_for_event(c)))
    {
        xcb_event_handle(&eh, e);
        free(e);
    }

    return 0;
}
