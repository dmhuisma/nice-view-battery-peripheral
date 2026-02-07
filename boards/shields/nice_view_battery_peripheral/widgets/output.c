#include <zephyr/kernel.h>
#include "output.h"
#include "../assets/font.h"

#define OFFSET_X 0
#define OFFSET_X_PERIPHERAL 44
#define OFFSET_Y 0

LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(bt_disconnected);
LV_IMG_DECLARE(unbound);
LV_IMG_DECLARE(usb);
LV_IMG_DECLARE(link);
LV_IMG_DECLARE(link_disconnected);

static void draw_usb_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, OFFSET_X, OFFSET_Y, &usb, &img_dsc);
}

static void draw_ble_unbonded(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, OFFSET_X, OFFSET_Y, &unbound, &img_dsc);
}

static void draw_ble_disconnected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, OFFSET_X, OFFSET_Y,
                       &bt_disconnected,
                       &img_dsc);
}

static void draw_ble_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, OFFSET_X, OFFSET_Y,
                       &bt,
                       &img_dsc);
}

static void draw_peripheral_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, OFFSET_X_PERIPHERAL, OFFSET_Y,
                       &link,
                       &img_dsc);
}

static void draw_peripheral_disconnected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    lv_canvas_draw_img(canvas, OFFSET_X_PERIPHERAL, OFFSET_Y,
                       &link_disconnected,
                       &img_dsc);
}

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas);
        break;

    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                draw_ble_connected(canvas);
            } else {
                draw_ble_disconnected(canvas);
            }
        } else {
            draw_ble_unbonded(canvas);
        }
        break;
    }

    // if (state->connected_slot0) {
    //     draw_peripheral_connected(canvas);
    // } else {
    //     draw_peripheral_disconnected(canvas);
    // }
}
