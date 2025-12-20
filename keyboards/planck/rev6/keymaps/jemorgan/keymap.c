/* Copyright 2015-2021 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "./secrets.h"

#ifdef AUDIO_ENABLE
#    include "muse.h"
#endif

enum planck_layers { _QWERTY, _LOWER, _RAISE, _ADJUST };

enum planck_keycodes { SP1 = SAFE_RANGE, SP2, ARROW, FAT_ARROW };

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define LCURL LSFT(KC_LBRC)
#define RCURL LSFT(KC_RBRC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Qwerty
     * ,-----------------------------------------------------------------------------------.
     * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
     * |------+------+------+------+------+------+------+------+------+------+------+------|
     * | SP1  | Ctrl | Alt  | GUI  |Lower | Space|Shift |Raise | GUI  | Alt  | Ctrl | SP2  |
     * `-----------------------------------------------------------------------------------'
     */
    // clang-format off
[_QWERTY] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     KC_BSPC,
    KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,  KC_QUOT,
    KC_TILD, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_ENT ,
    QK_LEAD, KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_LSFT, RAISE,   KC_RGUI, KC_RALT, KC_RCTL, QK_LEAD
),

[_LOWER] = LAYOUT_planck_grid(
    _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR,   KC_LPRN, KC_RPRN, KC_DEL,
    _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN,   KC_UP,  KC_RIGHT, KC_PGUP, KC_BSLS,
    _______, _______, _______, _______, _______, _______, _______, _______,   ARROW, FAT_ARROW, KC_PGDN, _______,
    _______, _______, _______, _______, _______, _______, KC_BSPC, _______, _______,   _______, _______, _______
),

[_RAISE] = LAYOUT_planck_grid(
    _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_DEL,
    _______, KC_MINS,   LCURL, KC_LPRN, KC_RPRN,   RCURL, KC_PLUS, KC_ASTR,  KC_EQL,   KC_LT,   KC_GT, KC_BSLS,
    _______, KC_UNDS, _______, KC_LBRC, KC_RBRC, _______, KC_MINS, KC_SLSH, KC_CIRC, KC_AMPR, KC_PIPE, _______,
    _______, _______, _______, _______, _______,  KC_ENT, _______, _______, _______, _______, _______, _______
),

/* Adjust (Lower + Raise)
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|Debug | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|  Del |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_grid(
    _______, QK_BOOT, DB_TOGG, UG_TOGG, UG_NEXT, UG_HUEU, UG_HUED, UG_SATU, UG_SATD, UG_VALU, UG_VALD, KC_DEL ,
    _______, _______, MU_NEXT, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, _______, _______, _______, _______,  _______,
    _______, AU_PREV, AU_NEXT, MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)
    // clang-format on

};

#ifdef AUDIO_ENABLE
float plover_song[][2]    = SONG(PLOVER_SOUND);
float plover_gb_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 50;

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
        case RAISE:
        case LOWER:
            return false;
        default:
            return true;
    }
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
