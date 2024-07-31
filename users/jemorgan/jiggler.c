#include <stdbool.h>
#include "action.h"
#include "keycodes.h"
bool is_mouse_jiggler_active = false;

void jiggle_mouse_if_active() {
    if (is_mouse_jiggler_active) {
        tap_code(KC_MS_UP);
        tap_code(KC_MS_DOWN);
    }
}
