#include "oled.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "action_layer.h"
#include "action_util.h"
#include "buffer.h"
#include "keycode_config.h"
#include "oled_driver.h"
#include "quantum.h"

void _render_select_menu(void);
void _render_keyboard_history(char*);
void _render_layer_modifier_state(void);
void _progress_cursor(const char*, uint8_t);

const uint16_t _GAME_MASK     = 0x01;
const uint16_t _SYMBOL_MASK   = 0x01 << 1;
const uint16_t _NUMBER_MASK   = 0x01 << 2;
const uint16_t _FUNCTION_MASK = 0x01 << 3;
const uint16_t _ADJUST_MASK   = 0x01 << 4;
const uint16_t _CTRL_MASK     = 0x01 << 5;
const uint16_t _ALT_MASK      = 0x01 << 6;
const uint16_t _GUI_MASK      = 0x01 << 7;
const uint16_t _SHIFT_MASK    = 0x01 << 8;
const uint16_t _OS_MASK       = 0x01 << 9;

struct oled_state* _state;
char*              previous_buffer_string;
char*              current_buffer_string;

void init(struct buffer* keypress_buffer) {
    _state                         = malloc(sizeof(struct oled_state));
    _state->active_layers_and_mods = 0xFFFF; // Initializing to impossible value so that the first try_render_oled succeeds
    _state->keypress_buffer        = keypress_buffer;
    current_buffer_string          = malloc(sizeof(char) * keypress_buffer->max_length + 1);
    current_buffer_string[0]       = '\0';
    previous_buffer_string         = malloc(sizeof(char) * keypress_buffer->max_length + 1);
    previous_buffer_string[0]      = '\0';
    get_buffer_as_string(keypress_buffer, previous_buffer_string);
}

void destroy() {
    free(_state);
}

// Checks to see if the state has changed since the last time the oled tried to render, and renders if it has.
void try_render_oled() {
    uint16_t current_active_layers_and_mods = gen_active_layers_and_mods();
    get_buffer_as_string(_state->keypress_buffer, current_buffer_string);

    // Check if the active layers/mods have changed, render either the select menu or the layer/modifier state if it has.
    if (_state->active_layers_and_mods != current_active_layers_and_mods) {
        _state->active_layers_and_mods = current_active_layers_and_mods;
        if (_state->active_layers_and_mods & _ADJUST_MASK) {
            _render_select_menu();
        } else {
            _render_layer_modifier_state();
        } } // Renders the keypress buffer if it's changed
    if (strcmp(current_buffer_string, previous_buffer_string)) {
        _render_keyboard_history(current_buffer_string);
        memcpy(previous_buffer_string, current_buffer_string, _state->keypress_buffer->max_length);
    }
}

void set_active_layers_and_mods(uint16_t active_layers_and_mods) {}

uint16_t gen_active_layers_and_mods() {
    uint16_t layers = 0x00 | (layer_state >> 1);

    if ((get_mods() | get_oneshot_mods()) & MOD_MASK_CTRL) layers |= _CTRL_MASK;
    if ((get_mods() | get_oneshot_mods()) & MOD_MASK_ALT) layers |= _ALT_MASK;
    if ((get_mods() | get_oneshot_mods()) & MOD_MASK_GUI) layers |= _GUI_MASK;
    if ((get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT) layers |= _SHIFT_MASK;
    if (keymap_config.oneshot_enable) layers |= _OS_MASK;

    return layers;
}

void _add_to_left_column(const char* text, int row) {
    oled_set_cursor(0, row + 2);
    oled_write(text, false);
}

void _add_to_right_column(const char* text, int row) {
    oled_set_cursor(oled_max_chars() - strlen(text), row + 2);
    oled_write(text, false);
}

bool _is_mask_active(uint16_t mask) {
    return _state->active_layers_and_mods & mask;
}

void _render_layer_modifier_state(void) {
    int left_column_count  = 0;
    int right_column_count = 0;

    oled_clear();
    oled_set_cursor(0, 0);
    oled_write("Layers", false);
    oled_set_cursor(oled_max_chars() - sizeof("Mods") + 1, 0);
    oled_write("Mods", false);

    for (uint8_t i = 0; i < oled_max_chars(); ++i)
        oled_write_char('-', false);

    if (_is_mask_active(_NUMBER_MASK)) {
        _add_to_left_column("Number", left_column_count++);
    }

    if (_is_mask_active(_SYMBOL_MASK)) {
        _add_to_left_column("Symbol", left_column_count++);
    }

    if (_is_mask_active(_FUNCTION_MASK)) {
        _add_to_left_column("Function", left_column_count++);
    }

    if (_is_mask_active(_GUI_MASK)) {
        _add_to_left_column("GUI", left_column_count++);
    }

    if (_is_mask_active(_ADJUST_MASK)) {
        _add_to_left_column("Adjust", left_column_count++);
    }

    if (_is_mask_active(_GAME_MASK)) {
        _add_to_left_column("Game", left_column_count++);
    }

    if (_is_mask_active(_SHIFT_MASK)) {
        _add_to_right_column("Shift", right_column_count++);
    }

    if (_is_mask_active(_CTRL_MASK)) {
        _add_to_right_column("Ctrl", right_column_count++);
    }

    if (_is_mask_active(_ALT_MASK)) {
        _add_to_right_column("Alt", right_column_count++);
    }

    if (_is_mask_active(_OS_MASK)) {
        _add_to_right_column("Oneshot", right_column_count++);
    }
}

void _render_keyboard_history(char* history) {
    oled_set_cursor(0, oled_max_lines() - 1);
    oled_write(history, false);
}

void _render_select_menu(void) {
    oled_write_ln("select", false);
}
