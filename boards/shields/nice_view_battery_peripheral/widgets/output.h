#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>
#include "../lib/util.h"

struct output_status_state {
    struct zmk_endpoint_instance selected_endpoint;
    int active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
    bool connected_slot0;
    bool connected_slot1;
};

void draw_output_status(lv_obj_t *canvas, const struct status_state *state);
