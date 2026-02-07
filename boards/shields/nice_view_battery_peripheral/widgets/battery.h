#pragma once

#include <lvgl.h>
#include "../lib/util.h"

struct battery_status_state {
    uint8_t level;
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    bool charging;
#endif
};

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state);
