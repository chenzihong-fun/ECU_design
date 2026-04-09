#ifndef __UI_H__
#define __UI_H__

#include "st7735.h"
#include "stdint.h"

typedef enum
{
    UI_ACTION_WRITE_STRING,
    UI_ACTION_CLEAR_LINE,
    UI_ACTION_DRAW_IMAGE,
} ui_action_t;

typedef struct
{
    ui_action_t action;

    union
    {
        struct
        {
            uint16_t x;
            uint16_t y;
            const char *str;
            st_fonts_t *font;
            uint16_t color;
            uint16_t bgcolor;
        }write_string;

        struct
        {
            uint16_t x;
            uint16_t y;
            uint16_t w;
            uint16_t h;
            const uint8_t *data;
        }draw_image;

        struct {
            uint16_t line;
            uint16_t font_height;
            uint16_t color;
        } clear_line;

    };

}ui_message_t;

void ui_task_init();
void ui_write_string(uint16_t x, uint16_t y, const char *str, st_fonts_t *font, uint16_t color, uint16_t bg_color);
void ui_clear_line(uint16_t line, uint16_t font_height, uint16_t color);
void ui_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *data);

#endif /* __UI_H__ */
