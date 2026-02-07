
#include <string.h>
#define DT_DRV_COMPAT zmk_nice_view_battery_peripheral

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/ssr_central_battery_state_changed.h>
#include <zmk/events/ssr_peripheral_battery_state_changed.h>
#include <zmk/events/ssr_central_layer_state_changed.h>
#include <zmk/events/ssr_central_transport_changed.h>
#include <zmk/events/ssr_central_ble_profile_changed.h>
#include <zmk/events/ssr_central_usb_conn_state_changed.h>
#include <zmk/events/ssr_peripheral_connection_state_changed.h>
#include <zmk/events/ssr_peripheral_usb_conn_state_changed.h>

#include "peripheral.h"
#include "../widgets/output.h"
#include "../widgets/battery.h"
#include "../widgets/profile.h"
#include "../widgets/layer.h"

#define MAX_LAYER_LABEL_LENGTH 32

typedef struct {
    uint8_t battery_slot0;
    uint8_t battery_slot1;
    bool charging_slot0;
    bool charging_slot1;
    bool connected_slot0;
    bool connected_slot1;
    enum zmk_transport transport;
    uint8_t active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
    uint8_t layer_index;
    char layer_label[MAX_LAYER_LABEL_LENGTH];
} display_state_t;

display_state_t display_state = {
    .layer_label = "",
};

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

// Draw

static void draw_top(lv_obj_t *widget, lv_color_t cbuf[], const struct status_state *state) {
    lv_obj_t *canvas = lv_obj_get_child(widget, 0);
    draw_background(canvas, TOP_WIDTH);

    draw_output_status(canvas, state);

    rotate_canvas(canvas, cbuf, TOP_WIDTH);
}

static void draw_middle(lv_obj_t *widget, lv_color_t cbuf[], const struct status_state *state) {
    lv_obj_t *canvas = lv_obj_get_child(widget, 1);
    draw_background(canvas, CANVAS_SIZE);

    draw_battery_status(canvas, state);

    rotate_canvas(canvas, cbuf, CANVAS_SIZE);
}

static void draw_bottom(lv_obj_t *widget, lv_color_t cbuf[], const struct status_state *state) {
    lv_obj_t *canvas = lv_obj_get_child(widget, 2);
    draw_background(canvas, BOTTOM_SIZE);

    draw_layer_status(canvas, state);
    draw_profile_status(canvas, state);

    rotate_canvas(canvas, cbuf, BOTTOM_SIZE);
}

// Battery status

static void set_battery_status_slot0(struct zmk_widget_screen *widget,
                               struct battery_status_state state) {
    widget->state.charging_slot0 = state.charging;
    widget->state.battery_slot0 = state.level;
    draw_middle(widget->obj, widget->cbuf2, &widget->state);
}

static void set_battery_status_slot1(struct zmk_widget_screen *widget,
                               struct battery_status_state state) {
    widget->state.charging_slot1 = state.charging;
    widget->state.battery_slot1 = state.level;
    draw_middle(widget->obj, widget->cbuf2, &widget->state);
}

static void battery_status_slot0_update_cb(struct battery_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_status_slot0(widget, state); }
}

static struct battery_status_state battery_status_get_state_slot0(const zmk_event_t *eh) {
    const struct zmk_ssr_peripheral_battery_state_changed *battery_ev = as_zmk_ssr_peripheral_battery_state_changed(eh);
    if (battery_ev && battery_ev->slot == 0) {
        display_state.battery_slot0 = battery_ev->battery_level;
    }
    
    const struct zmk_ssr_peripheral_usb_conn_state_changed *usb_ev = as_zmk_ssr_peripheral_usb_conn_state_changed(eh);
    if (usb_ev && usb_ev->slot == 0) {
        display_state.charging_slot0 = usb_ev->connected;
    }
    
    const struct zmk_ssr_peripheral_connection_state_changed *conn_ev = as_zmk_ssr_peripheral_connection_state_changed(eh);
    if (conn_ev && conn_ev->slot == 0) {
        display_state.connected_slot0 = conn_ev->connected;
    }
    
    return (struct battery_status_state){
        .level = display_state.battery_slot0,
        .charging = display_state.charging_slot0,
    };
}

static void battery_status_slot1_update_cb(struct battery_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_status_slot1(widget, state); }
}

static struct battery_status_state battery_status_get_state_slot1(const zmk_event_t *eh) {
    const struct zmk_ssr_peripheral_battery_state_changed *battery_ev = as_zmk_ssr_peripheral_battery_state_changed(eh);
    if (battery_ev && battery_ev->slot == 1) {
        display_state.battery_slot1 = battery_ev->battery_level;
    }
    
    const struct zmk_ssr_peripheral_usb_conn_state_changed *usb_ev = as_zmk_ssr_peripheral_usb_conn_state_changed(eh);
    if (usb_ev && usb_ev->slot == 1) {
        display_state.charging_slot1 = usb_ev->connected;
    }
    
    const struct zmk_ssr_peripheral_connection_state_changed *conn_ev = as_zmk_ssr_peripheral_connection_state_changed(eh);
    if (conn_ev && conn_ev->slot == 1) {
        display_state.connected_slot1 = conn_ev->connected;
    }
    
    return (struct battery_status_state){
        .level = display_state.battery_slot1,
        .charging = display_state.charging_slot1,
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status_slot0, struct battery_status_state,
                            battery_status_slot0_update_cb, battery_status_get_state_slot0);
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status_slot1, struct battery_status_state,
                            battery_status_slot1_update_cb, battery_status_get_state_slot1);

ZMK_SUBSCRIPTION(widget_battery_status_slot0, zmk_ssr_peripheral_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status_slot0, zmk_ssr_peripheral_usb_conn_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status_slot0, zmk_ssr_peripheral_connection_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status_slot1, zmk_ssr_peripheral_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status_slot1, zmk_ssr_peripheral_usb_conn_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status_slot1, zmk_ssr_peripheral_connection_state_changed);

// Layer status

static void set_layer_status(struct zmk_widget_screen *widget, struct layer_status_state state) {
    widget->state.layer_label = state.label;
    draw_bottom(widget->obj, widget->cbuf3, &widget->state);
}

static void layer_status_update_cb(struct layer_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_layer_status(widget, state); }
}

static struct layer_status_state layer_status_get_state(const zmk_event_t *eh) {
    const struct zmk_ssr_central_layer_state_changed *ev = as_zmk_ssr_central_layer_state_changed(eh);
    if (ev != NULL) {
        display_state.layer_index = ev->layer;
        strncpy(display_state.layer_label, ev->layer_name, MAX_LAYER_LABEL_LENGTH - 1);
        display_state.layer_label[MAX_LAYER_LABEL_LENGTH - 1] = '\0';
    }
    return (struct layer_status_state){.index = display_state.layer_index, .label = display_state.layer_label};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_status, struct layer_status_state, layer_status_update_cb,
                            layer_status_get_state)
ZMK_SUBSCRIPTION(widget_layer_status, zmk_ssr_central_layer_state_changed);

// Output status

static void set_output_status(struct zmk_widget_screen *widget,
                              const struct output_status_state *state) {
    widget->state.selected_endpoint = state->selected_endpoint;
    widget->state.active_profile_index = state->active_profile_index;
    widget->state.active_profile_connected = state->active_profile_connected;
    widget->state.active_profile_bonded = state->active_profile_bonded;
    widget->state.connected_slot0 = state->connected_slot0;
    widget->state.connected_slot1 = state->connected_slot1;

    draw_top(widget->obj, widget->cbuf, &widget->state);
    draw_bottom(widget->obj, widget->cbuf3, &widget->state);
}

static void output_status_update_cb(struct output_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_output_status(widget, &state); }
}

static struct output_status_state output_status_get_state(const zmk_event_t *eh) {
    // Update display_state based on event type
    const struct zmk_ssr_peripheral_connection_state_changed *conn_ev = as_zmk_ssr_peripheral_connection_state_changed(eh);
    if (conn_ev) {
        if (conn_ev->slot == 0) {
            display_state.connected_slot0 = conn_ev->connected;
        } else if (conn_ev->slot == 1) {
            display_state.connected_slot1 = conn_ev->connected;
        }
    }
    
    const struct zmk_ssr_central_transport_changed *transport_ev = as_zmk_ssr_central_transport_changed(eh);
    if (transport_ev) {
        display_state.transport = (enum zmk_transport)transport_ev->transport;
    }
    
    const struct zmk_ssr_central_ble_profile_changed *ble_ev = as_zmk_ssr_central_ble_profile_changed(eh);
    if (ble_ev) {
        display_state.active_profile_index = ble_ev->active_profile_index;
        display_state.active_profile_connected = ble_ev->active_profile_connected;
        display_state.active_profile_bonded = ble_ev->active_profile_bonded;
    }
    
    return (struct output_status_state){
        .selected_endpoint = (struct zmk_endpoint_instance){ .transport = display_state.transport },
        .active_profile_index = display_state.active_profile_index,
        .active_profile_connected = display_state.active_profile_connected,
        .active_profile_bonded = display_state.active_profile_bonded,
        .connected_slot0 = display_state.connected_slot0,
        .connected_slot1 = display_state.connected_slot1,
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct output_status_state,
                            output_status_update_cb, output_status_get_state)
ZMK_SUBSCRIPTION(widget_output_status, zmk_ssr_peripheral_connection_state_changed);
ZMK_SUBSCRIPTION(widget_output_status, zmk_ssr_central_transport_changed);
ZMK_SUBSCRIPTION(widget_output_status, zmk_ssr_central_ble_profile_changed);

// Initialization

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 160, 68);

    lv_obj_t *top = lv_canvas_create(widget->obj);
    lv_obj_align(top, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_canvas_set_buffer(top, widget->cbuf, TOP_WIDTH, TOP_HEIGHT, LV_IMG_CF_TRUE_COLOR);

    lv_obj_t *middle = lv_canvas_create(widget->obj);
    lv_obj_align(middle, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_canvas_set_buffer(middle, widget->cbuf2, CANVAS_SIZE, CANVAS_SIZE, LV_IMG_CF_TRUE_COLOR);

    lv_obj_t *bottom = lv_canvas_create(widget->obj);
    lv_obj_align(bottom, LV_ALIGN_TOP_LEFT, -40, 0);
    lv_canvas_set_buffer(bottom, widget->cbuf3, BOTTOM_SIZE, BOTTOM_SIZE, LV_IMG_CF_TRUE_COLOR);

    sys_slist_append(&widgets, &widget->node);
    widget_battery_status_slot0_init();
    widget_battery_status_slot1_init();
    widget_output_status_init();
    widget_layer_status_init();

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
