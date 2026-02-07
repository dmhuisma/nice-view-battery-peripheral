#include <zephyr/kernel.h>
#include "battery.h"
#include "../assets/font.h"

#define OFFSET_X 29

LV_IMG_DECLARE(bolt);
LV_IMG_DECLARE(battery);
LV_IMG_DECLARE(battery_mask);

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
    lv_draw_label_dsc_t outline_dsc;
    init_label_dsc(&outline_dsc, LVGL_BACKGROUND, &font, LV_TEXT_ALIGN_CENTER);
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &font, LV_TEXT_ALIGN_CENTER);

    #define CHARGING_SLOT0_OFFSET_Y 2
    #define BATT_SLOT0_OFFSET_Y (CHARGING_SLOT0_OFFSET_Y + 16)
    #define BATT_SLOT1_OFFSET_Y (BATT_SLOT0_OFFSET_Y + 36)
    #define CHARGING_SLOT1_OFFSET_Y (BATT_SLOT1_OFFSET_Y + 35)

    lv_canvas_draw_img(canvas, 0, BATT_SLOT0_OFFSET_Y, &battery, &img_dsc);
    if (state->connected_slot0) {
        lv_canvas_draw_rect(canvas, 4, 4 + BATT_SLOT0_OFFSET_Y, 54 * state->battery_slot0 / 100, 23, &rect_dsc);
    } else {
        lv_canvas_draw_rect(canvas, 4, 4 + BATT_SLOT0_OFFSET_Y, 0, 23, &rect_dsc);
    }
    lv_canvas_draw_img(canvas, 2, 2 + BATT_SLOT0_OFFSET_Y, &battery_mask, &img_dsc);

    lv_canvas_draw_img(canvas, 0, BATT_SLOT1_OFFSET_Y, &battery, &img_dsc);
    if (state->connected_slot1) {
        lv_canvas_draw_rect(canvas, 4, 4 + BATT_SLOT1_OFFSET_Y, 54 * state->battery_slot1 / 100, 23, &rect_dsc);
    } else {
        lv_canvas_draw_rect(canvas, 4, 4 + BATT_SLOT1_OFFSET_Y, 0, 23, &rect_dsc);
    }
    lv_canvas_draw_img(canvas, 2, 2 + BATT_SLOT1_OFFSET_Y, &battery_mask, &img_dsc);

    char text_slot0[10] = {};
    if (state->connected_slot0) {
        sprintf(text_slot0, "%i%%", state->battery_slot0);
    } else {
        strcpy(text_slot0, "?");
    }
    char text_slot1[10] = {};
    if (state->connected_slot1) {
        sprintf(text_slot1, "%i%%", state->battery_slot1);
    } else {
        strcpy(text_slot1, "?");
    }

    const int y = 9 + BATT_SLOT0_OFFSET_Y;
    const int w = 62;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                lv_canvas_draw_text(canvas, dx, y + dy, w, &outline_dsc, text_slot0);
            }
        }
    }

    lv_canvas_draw_text(canvas, 0, y, w, &label_dsc, text_slot0);

    const int y_peripheral = 9 + BATT_SLOT1_OFFSET_Y;
    const int w_peripheral = 62;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                lv_canvas_draw_text(canvas, dx, y_peripheral + dy, w_peripheral, &outline_dsc, text_slot1);
            }
        }
    }

    lv_canvas_draw_text(canvas, 0, y_peripheral, w_peripheral, &label_dsc, text_slot1);

    if (state->charging_slot0 && state->connected_slot0) {
        lv_canvas_draw_img(canvas, OFFSET_X, CHARGING_SLOT0_OFFSET_Y, &bolt, &img_dsc);
    }
    if (state->charging_slot1 && state->connected_slot1) {
        lv_canvas_draw_img(canvas, OFFSET_X, CHARGING_SLOT1_OFFSET_Y, &bolt, &img_dsc);
    }
}
