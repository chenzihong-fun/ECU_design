#ifndef __KEY_H__
#define __KEY_H__

#include "key_desc.h"

void key_init(key_desc_t* key);
void key_wait_released(key_desc_t* key);
bool key_pressed(key_desc_t* key);
bool key_long_pressed(key_desc_t *key);

#endif /* __KEY_H__ */