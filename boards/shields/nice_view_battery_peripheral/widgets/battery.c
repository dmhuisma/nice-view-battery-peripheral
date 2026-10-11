#include <zephyr/kernel.h>
#include "battery.h"
#include "../assets/font.h"

#define OFFSET_X 29

LV_IMG_DECLARE(bolt);
LV_IMG_DECLARE(battery);
LV_IMG_DECLARE(battery_mask);

#define CHARGING_SLOT0_OFFSET_Y 2
#define BATT_SLOT0_OFFSET_Y (CHARGING_SLOT0_OFFSET_Y + 16)
#define BATT_SLOT1_OFFSET_Y (BATT_SLOT0_OFFSET_Y + 36)
#define CHARGING_SLOT1_OFFSET_Y (BATT_SLOT1_OFFSET_Y + 35)

// Draw a single battery (icon, fill, percentage text and charging bolt)
static void draw_battery(lv_obj_t *canvas, int batt_offset_y, int charging_offset_y,
                         bool connected, uint8_t level, bool charging) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
    lv_draw_label_dsc_t outline_dsc;
    init_label_dsc(&outline_dsc, LVGL_BACKGROUND, &font, LV_TEXT_ALIGN_CENTER);
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &font, LV_TEXT_ALIGN_CENTER);

    canvas_draw_img(canvas, 0, batt_offset_y, &battery, &img_dsc);
    canvas_draw_rect(canvas, 4, 4 + batt_offset_y, connected ? 54 * level / 100 : 0, 23,
                     &rect_dsc);
    canvas_draw_img(canvas, 2, 2 + batt_offset_y, &battery_mask, &img_dsc);

    char text[10] = {};
    if (connected) {
        sprintf(text, "%i%%", level);
    } else {
        strcpy(text, "?");
    }

    const int y = 9 + batt_offset_y;
    const int w = 62;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                canvas_draw_text(canvas, dx, y + dy, w, &outline_dsc, text);
            }
        }
    }

    canvas_draw_text(canvas, 0, y, w, &label_dsc, text);

    if (charging && connected) {
        canvas_draw_img(canvas, OFFSET_X, charging_offset_y, &bolt, &img_dsc);
    }
}

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
#if CONFIG_NICE_VIEW_BATTERY_PERIPHERAL_SLOT == 0
    // Only show the battery of this half (slot 0)
    draw_battery(canvas, BATT_SLOT0_OFFSET_Y, CHARGING_SLOT0_OFFSET_Y, state->connected_slot0,
                 state->battery_slot0, state->charging_slot0);
#elif CONFIG_NICE_VIEW_BATTERY_PERIPHERAL_SLOT == 1
    // Only show the battery of this half (slot 1), drawn in the first position
    draw_battery(canvas, BATT_SLOT0_OFFSET_Y, CHARGING_SLOT0_OFFSET_Y, state->connected_slot1,
                 state->battery_slot1, state->charging_slot1);
#else
    draw_battery(canvas, BATT_SLOT0_OFFSET_Y, CHARGING_SLOT0_OFFSET_Y, state->connected_slot0,
                 state->battery_slot0, state->charging_slot0);
    draw_battery(canvas, BATT_SLOT1_OFFSET_Y, CHARGING_SLOT1_OFFSET_Y, state->connected_slot1,
                 state->battery_slot1, state->charging_slot1);
#endif
}
