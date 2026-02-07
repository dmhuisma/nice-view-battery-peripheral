#include <zephyr/kernel.h>
#include "layer.h"
#include "../assets/font.h"

#define OFFSET_X 0
#define OFFSET_Y 0

LV_IMG_DECLARE(layer);

void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
    if (state->layer_label == NULL || state->layer_label[0] == '\0') {
        return;
    }

    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_BACKGROUND, &font, LV_TEXT_ALIGN_CENTER);

    char text[10] = {};
    strcpy(text, state->layer_label);
    to_uppercase(text);

    lv_canvas_draw_img(canvas, OFFSET_X, OFFSET_Y, &layer, &img_dsc);
    lv_canvas_draw_text(canvas, OFFSET_X + 2, OFFSET_Y, 64, &label_dsc, text);
}
