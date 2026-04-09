#include "ui.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "stdlib.h"
#include "string.h"
#include "board.h"

static QueueHandle_t ui_queue;
static TaskHandle_t ui_task_handle;
static void ui_task(void *pvParameters);

void ui_clear_line(uint16_t line, uint16_t font_height, uint16_t color)
{
    ui_message_t msg;
    msg.action = UI_ACTION_CLEAR_LINE;
    msg.clear_line.line = line;
    msg.clear_line.font_height = font_height;
    msg.clear_line.color = color;
    xQueueSend(ui_queue, &msg, portMAX_DELAY);
}

void ui_draw_image(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *data)
{
    ui_message_t msg;
    msg.action = UI_ACTION_DRAW_IMAGE;
    msg.draw_image.x = x;
    msg.draw_image.y = y;
    msg.draw_image.w = w;
    msg.draw_image.h = h;
    msg.draw_image.data = data;
    xQueueSend(ui_queue, &msg, portMAX_DELAY);
}

void ui_write_string(uint16_t x, uint16_t y, const char *str, st_fonts_t *font, uint16_t color, uint16_t bg_color)
{
    ui_message_t msg;
    msg.action = UI_ACTION_WRITE_STRING;
    msg.write_string.x = x;
    msg.write_string.y = y;
    msg.write_string.str = str;
    msg.write_string.font = font;
    msg.write_string.color = color;
    msg.write_string.bgcolor = bg_color;
    xQueueSend(ui_queue, &msg, portMAX_DELAY);
}



void ui_task_init()
{
    ui_queue = xQueueCreate(16, sizeof(ui_message_t));
    configASSERT(ui_queue);
    xTaskCreate(ui_task, "ui_task", 254, NULL, 9, NULL);
}

static void ui_task(void *pvParameters)
{
    ui_message_t msg;
    while (1)
    {
        xQueueReceive(ui_queue, &msg, portMAX_DELAY);
        switch (msg.action)
        {
        case UI_ACTION_CLEAR_LINE:
            st7735_clear_line(msg.clear_line.line, msg.clear_line.font_height, msg.clear_line.color);
            break;
        case UI_ACTION_DRAW_IMAGE:
            st7735_draw_image(msg.draw_image.x, msg.draw_image.y, msg.draw_image.w, msg.draw_image.h, msg.draw_image.data);
            break;
        case UI_ACTION_WRITE_STRING:
            st7735_write_string(msg.write_string.x, msg.write_string.y, msg.write_string.str, msg.write_string.font, msg.write_string.color, msg.write_string.bgcolor);
            break;
        }
    }
}
