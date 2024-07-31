#include "quantum_keycodes.h"
#include <stdint.h>
#include <stdlib.h>
#include "keyboard.h"
#include "keycodes.h"
#include "../../../../../users/jemorgan/oled.h"
#include "../../../../../users/jemorgan/buffer.h"
#include "oled_driver.h"
#include "quantum.h"
#include "./secrets.h"
#include QMK_KEYBOARD_H

#define RAISE OSL(_NUMBER)
#define LOWER OSL(_SYMBOL)
#define FUNCTION OSL(_FUNCTION)
#define CWT QK_CAPS_WORD_TOGGLE
#define CK_UNDRSC LSFT(KC_MINS)

#define LCURL LSFT(KC_LBRC)
#define RCURL LSFT(KC_RBRC)

enum layers { _QWERTY = 0, _GAME, _SYMBOL, _NUMBER, _FUNCTION, _ADJUST, _TRANS };

enum custom_keycodes { _ARROW = SAFE_RANGE, _JIGG };

struct buffer *keyboard_buffer         = NULL;
bool           is_mouse_jiggler_active = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
        KC_GRV,         KC_Q,         KC_W,         KC_E,         KC_R,         KC_T,                                                                 KC_Y,         KC_U,         KC_I,         KC_O,      KC_P,        KC_TAB,
        KC_ESC,         KC_A,         KC_S,         KC_D,         KC_F,         KC_G,                                                                 KC_H,         KC_J,         KC_K,         KC_L,      KC_SCLN,     KC_ENT,
        OSM(MOD_LSFT),       KC_Z,         KC_X,         KC_C,         KC_V,         KC_B,        LOWER,       FUNCTION,     FUNCTION,        RAISE,       KC_N,         KC_M,      KC_COMM,       KC_DOT,      KC_SLSH,     OSM(MOD_RSFT),
        MO(_ADJUST),  OSM(MOD_LALT),     OSM(MOD_LGUI),     KC_SPC,      OSM(MOD_LCTL),      OSM(MOD_RCTL),     KC_BSPC,     OSM(MOD_RGUI),     OSM(MOD_RALT),        QK_LEAD
    ),
    [_GAME] = LAYOUT(
        _______, _______, _______,     _______, _______, _______,                                                _______, _______, _______, _______, _______, _______,
        _______, _______, _______,     _______, _______, _______,                                                _______, _______, _______, _______, _______, _______,
        KC_LSFT, _______, _______,     _______, _______, _______, MO(_NUMBER), MO(_FUNCTION),  _______, _______, _______, _______, _______, _______, _______, _______,
        MO(_ADJUST), KC_LALT,   KC_F6,     _______,       KC_LCTL,  _______, _______, _______, _______, _______
    ),
    [_SYMBOL] = LAYOUT(
        _______, LSFT(KC_1),   LSFT(KC_2),   LSFT(KC_3),   LSFT(KC_4),   LSFT(KC_5),                                                         LSFT(KC_6),   LSFT(KC_7),   LSFT(KC_8),   LSFT(KC_9),   LSFT(KC_0),       LSFT(KC_EQL),
        CWT,     _______,      _______,      _______,      _______,      _______,                                                            KC_LEFT,      KC_DOWN,        KC_UP,     KC_RIGHT,      KC_QUOTE, KC_EQL,
        _______, _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______,    _______,    _______,    _______,      _______,       _ARROW,      KC_BSLS,      LSFT(KC_BSLS),
        _______,      _______,      _______,      _______,      _______,      _______,    _______,    _______,      _______,      _______
    ),
    [_NUMBER] = LAYOUT(
        KC_INS,      KC_1,    KC_2,    KC_3,    KC_4,   KC_5,                                             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_DEL,
        _______,   KC_MINS,   LCURL, KC_LPRN, KC_RPRN,  RCURL,                                          _______, _______, _______, _______, _______,  KC_EQL,
        CWT, CK_UNDRSC, KC_PLUS, KC_LBRC, KC_RBRC, KC_EQL,  _______, _______,     _______, _______, _______, _______, _______, _______, KC_BSLS, _______,
        _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
    ),
    [_FUNCTION] = LAYOUT(
        _______,        KC_F1,        KC_F2,        KC_F3,        KC_F4,        KC_F5,                                                                KC_F6,        KC_F7,        KC_F8,        KC_F9,       KC_F10,      _______,
        _______,       KC_F11,       KC_F12,       KC_F13,       KC_F14,       KC_F15,                                                               KC_F16,       KC_F17,       KC_F18,       KC_F19,       KC_F20,      _______,
        _______,       KC_F21,       KC_F22,       KC_F23,       KC_F24,      _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______,
        _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______,      _______
    ),
    [_ADJUST] = LAYOUT(
        _______, LCAG(KC_1), LCAG(KC_2), LCAG(KC_3), LCAG(KC_4), LCAG(KC_5),                                 _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                                                UG_TOGG, UG_HUEU, UG_SATU, UG_VALU, UG_SPDU, _______,
        KC_LSFT, _______, _______, _______, _______, _______, _______,        _______,    _______, _______,  _______, RGB_MODE_FORWARD, RGB_MODE_REVERSE, _______, _______, KC_LSFT,
        _______, TG(_GAME), DF(_QWERTY), OS_TOGG, _JIGG,                                      QK_LEAD, _______, _______, _______, _______
    ),
    [_TRANS] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______
    )
};
// clang-format on
//
uint16_t throttle = 0;
void     matrix_scan_user(void) {
    if (is_mouse_jiggler_active && ++throttle % 128 == 0) {
        int direction = rand() % 4;
        int keycode;
        switch (direction) {
            case 0:
                keycode = KC_MS_UP;
                break;
            case 1:
                keycode = KC_MS_DOWN;
                break;
            case 2:
                keycode = KC_MS_LEFT;
                break;
            case 3:
                keycode = KC_MS_RIGHT;
                break;
        }

        for (int i = 0; i < 8; i++) {
            tap_code(keycode);
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }

    if (is_mouse_jiggler_active) {
        is_mouse_jiggler_active = false;
    } else if (keycode == _JIGG) {
        is_mouse_jiggler_active = true;
    }

    if (keycode >= 0xe0 && keycode <= 0xe7) {
        add_keycode_to_buffer(keyboard_buffer, keycode);
    } else if (keycode >= KC_F1 && keycode <= KC_F12) {
        add_char_to_buffer(keyboard_buffer, 'F');
        add_char_to_buffer(keyboard_buffer, '1' + keycode - KC_F1);
    } else if (IS_QK_BASIC(keycode) && (keycode < 0xe0 || keycode > 0xe7)) {
        add_keycode_to_buffer(keyboard_buffer, keycode);
    }

    return true;
}

void keyboard_post_init_user(void) {
    keyboard_buffer = create_empty_buffer();
    init(keyboard_buffer);
}

void leader_end_user(void) {
    if (leader_sequence_two_keys(KC_P, KC_W)) {
        SEND_STRING(WORK_PASS);
    }
    if (leader_sequence_two_keys(KC_P, KC_F)) {
        SEND_STRING(L_PASS);
    }
    if (leader_sequence_two_keys(KC_P, KC_B)) {
        SEND_STRING(BITWARDEN_PASS);
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

bool oled_task_user(void) {
    try_render_oled();
    return false;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    return true;
}
