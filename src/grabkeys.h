#ifndef GRAB_KEYS_H
#define GRAB_KEYS_H

typedef void (*key_handler_t)(const config_t*, int, bool);

int run(config_t* config, key_handler_t handler);

#endif
