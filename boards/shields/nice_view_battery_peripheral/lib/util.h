#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>

#define CANVAS_SIZE 134
#define BOTTOM_SIZE 68
#define TOP_WIDTH   25
#define TOP_HEIGHT  68
#define LVGL_BACKGROUND                                                                            \
    IS_ENABLED(CONFIG_NICE_VIEW_WIDGET_INVERTED) ? lv_color_black() : lv_color_white()
#define LVGL_FOREGROUND                                                                            \
    IS_ENABLED(CONFIG_NICE_VIEW_WIDGET_INVERTED) ? lv_color_white() : lv_color_black()

struct status_state {
    uint8_t battery_slot0;
    uint8_t battery_slot1;
    bool charging_slot0;
    bool charging_slot1;
    struct zmk_endpoint_instance selected_endpoint;
    int active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
    const char *layer_label;
    bool connected_slot0;
    bool connected_slot1;
};

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[], const int size);
void draw_background(lv_obj_t *canvas, const int size);
void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color);
void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width);
void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font,
                    lv_text_align_t align);
void to_uppercase(char *str);
