#include "../../include/central/render.h"

#include <ctype.h>
#include <lvgl.h>
#include <stdlib.h>
#include <string.h>
#include <zephyr/sys/util.h>
#include "../../include/colors.h"
#include "../../include/central/initialize_listeners.h"
#include "../../include/fonts/custom_font_22.h"
#include "../../include/fonts/pixel_operator_36.h"
#include "../../include/fonts/custom_font_shadow.h"
#include "../../include/fonts/custom_font_outline.h"
#include "../../include/main.h"
#include "../../include/utils/draw_battery.h"
#include "../../include/utils/draw_background.h"
#include "../../include/utils/draw_bluetooth_searching.h"
#include "../../include/utils/draw_bluetooth_logo_outlined.h"
#include "../../include/utils/draw_bluetooth_logo.h"
#include "../../include/utils/draw_usb_logo.h"
#include "../../include/utils/rotate_connectivity_canvas.h"
#include "../../include/images/background_alt_layer.h"
#include "../../include/images/background_temp_layer.h"
#include "../../include/images/background_main_layer.h"

void render_battery() {
    lv_canvas_fill_bg(battery_canvas, BACKGROUND_COLOR, LV_OPA_COVER);

    draw_battery(battery_canvas, 5, 0, states.battery);
}

static void render_bluetooth_logo() {
    static const unsigned x = CONNECTIVITY_CANVAS_WIDTH - 12;
    if (states.connectivity.active_profile_bonded) {
        if (states.connectivity.active_profile_connected) {
            draw_bluetooth_logo(connectivity_canvas, x, 0);
        } else {
            draw_bluetooth_logo_outlined(connectivity_canvas, x, 0);
        }
    } else {
        draw_bluetooth_searching(connectivity_canvas, x, 0);
    }
}

static void render_bluetooth_profile_index() {
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = FOREGROUND_COLOR;
    label_dsc.font = &custom_font_22;
    label_dsc.align = LV_TEXT_ALIGN_RIGHT;

    static const unsigned custom_font_22_height = 19;
    static const unsigned padding_y = -1;
    static const unsigned width = CONNECTIVITY_CANVAS_WIDTH - 18;
    static const char bluetooth_profile_label[5][2] = {"1", "2", "3", "4", "5"};
    const char* label = bluetooth_profile_label[states.connectivity.active_profile_index];
   
    lv_canvas_draw_text(connectivity_canvas, 4, padding_y, width, &label_dsc, label); 
}

static void render_bluetooth_connectivity() {
    render_bluetooth_logo();
    render_bluetooth_profile_index();
}

static void render_usb_connectivity() {
    draw_usb_logo(connectivity_canvas, 15, 4);
}

void render_connectivity() {
    lv_canvas_fill_bg(connectivity_canvas, BACKGROUND_COLOR, LV_OPA_COVER);

    switch (states.connectivity.selected_endpoint.transport) {
        case ZMK_TRANSPORT_BLE: {
            render_bluetooth_connectivity();
            break;
        }
        case ZMK_TRANSPORT_USB: {
            render_usb_connectivity();
            break;
        }
    }

    rotate_connectivity_canvas();
}

void render_main() {
#if IS_ENABLED(CONFIG_NICE_VIEW_ELEMENTAL_BACKGROUND)
    // Unfortunately, text transparency does not seem to work in LVGL 8.3. This
    // forces us to redraw the background on every render instead of having it
    // on a layer underneath.

    // skip rendering background on cental
    // draw_background(main_canvas, states.background_index);
#endif

    lv_canvas_fill_bg(main_canvas, BACKGROUND_COLOR, LV_OPA_COVER);

    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    switch (states.layer.index) {
        case 0:
            lv_canvas_draw_img(main_canvas, 0, 0, &background_main_layer, &img_dsc);
            break;
        case 7:
        case 11:
        // layers you use for a long time. ex: app specific, video, gaming
            lv_canvas_draw_img(main_canvas, 0, 0, &background_alt_layer, &img_dsc);
            break;
        default:
        // layers that you don't stick in for long. ex: symbols, portals, board management
            lv_canvas_draw_img(main_canvas, 0, 0, &background_temp_layer, &img_dsc);

    }

    // Capitalize the layer name if given or use the layer number otherwise.
    char* text = NULL;
    if (states.layer.name == NULL) {
        text = malloc(10 * sizeof(char));
        sprintf(text, "LAYER %i", states.layer.index);
    }
    else {
        text = malloc((strlen(states.layer.name) + 1) * sizeof(char));
        for (unsigned i = 0; states.layer.name[i] != '\0'; i++) {
            text[i] = toupper(states.layer.name[i]);
        }
        text[strlen(states.layer.name)] = '\0';
    }

    // // Magic number. The height of the font from the baseline to the ascender
    // // height is 34px, but halving the space remaining of the full height gives
    // // us another value ((68px - 34px) / 2 = 17px). 
    // static const unsigned text_y_offset = 15;

    // screen width
    // |      line height
    // |      |      some adjustment
    // V      V      V
    // 68px - 39px + 6px = 29px
    static const unsigned text_y_offset = 35;



    lv_draw_label_dsc_t layer_name_dsc;
    lv_draw_label_dsc_init(&layer_name_dsc);
    layer_name_dsc.color = FOREGROUND_COLOR;
    layer_name_dsc.font = &pixel_operator_36;
    layer_name_dsc.align = LV_TEXT_ALIGN_LEFT;

    lv_canvas_draw_text(
        main_canvas,
        -3,
        text_y_offset,
        MAIN_CANVAS_WIDTH,
        &layer_name_dsc,
        text
    );

    free(text);
    text = NULL;
}
