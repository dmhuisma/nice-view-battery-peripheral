#include <zephyr/kernel.h>
#include "profile.h"

#define OFFSET_X 0
#define OFFSET_Y 15

LV_IMG_DECLARE(profile);
LV_IMG_DECLARE(profile_active);

void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    for (int i = 0; i < 5; i++) {
        lv_canvas_draw_img(canvas, OFFSET_X + (i * 14), OFFSET_Y, i == state->active_profile_index ? &profile_active : &profile, &img_dsc);
    }
}
