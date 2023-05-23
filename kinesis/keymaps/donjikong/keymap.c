#include QMK_KEYBOARD_H
#include "features/custom_shift_keys.h"
#include "raw_hid.h"

void raw_hid_receive(uint8_t *data, uint8_t length) {
    raw_hid_send(data, length);
}

const key_override_t exlm_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_EXLM, KC_8);  // Shift . is ?

/* EZ_KEYS conflict with these
const key_override_t lbrc_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, KC_9); // Shift , is !
const key_override_t lcbr_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_LCBR, KC_0);  // Shift - is =
const key_override_t lprn_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_1); // Shift : is ;
const key_override_t pmns_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_PMNS, KC_2); // Shift : is ;
*/

const key_override_t coln_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_COLN, KC_3); // Shift : is ;
const key_override_t rprn_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_RPRN, KC_4); // Shift : is ;
const key_override_t rcbr_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_RCBR, KC_5); // Shift : is ;
const key_override_t rbrc_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_RBRC, KC_6); // Shift : is ;
const key_override_t astr_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_ASTR, KC_7); // Shift : is ;

const key_override_t slsh_key_override =
    ko_make_basic(MOD_MASK_SHIFT, KC_SLSH, KC_BSLS); // Shift : is ;

const key_override_t** key_overrides = (const key_override_t*[]){
    &exlm_key_override,
/* EZ_KEYS conflict with these
    &lbrc_key_override,
    &lcbr_key_override,
    &lprn_key_override,
    &pmns_key_override,
*/

    &coln_key_override,
    &rprn_key_override,
    &rcbr_key_override,
    &rbrc_key_override,
    &astr_key_override,

    &slsh_key_override,

    NULL
};

const custom_shift_key_t custom_shift_keys[] = {
  {KC_CIRC, KC_AMPR}, // Shift % is &
  {KC_DLR,  KC_PIPE}, // Shift # is |
  {KC_SCLN, KC_PLUS}, // Shift / is ~
};

uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);


enum custom_keycodes {
  SHIFT_CTRL_PRTSC = SAFE_RANGE,

  J_JUMP,
  K_JUMP,

  EZ_BRC,
  EZ_CBR,
  EZ_PRN,
  EZ_LT,
  EZ_QUOT,
  EZ_DQUO,
};

static uint16_t hold_timer = 0;
static uint16_t repeat_timer = 0;
static int hold_detected = 0;
static int repeat_started = 0;

enum Bracket {
    BRC,
    CBR,
    PRN,
    LT,
    QUOT,
    DQUO
};

static enum Bracket bracketType;

void outputBracket(enum Bracket bracketType) {
    switch (bracketType) {
        case BRC:
            SEND_STRING("[");
            break;
        case CBR:
            SEND_STRING("{");
            break;
        case PRN:
            SEND_STRING("(");
            break;
        case LT:
            tap_code(KC_PMNS);
            break;
        case QUOT:
            SEND_STRING("'");
            break;
        case DQUO:
            SEND_STRING("\"");
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t * record) {
    if (!process_custom_shift_keys(keycode, record)) {
        return false;
    }

    static int shift_detected;

    const uint8_t mods = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();

    switch (keycode) {
    case J_JUMP:
        if (record -> event.pressed) {
            if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);

                tap_code(KC_DOWN);

                register_mods(mods);
                shift_detected = 1;
            } else if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                tap_code(KC_LEFT);
                register_mods(mods);
                shift_detected = 1;
            } else {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);

                tap_code(KC_ESC);
                layer_on(1);

                register_mods(mods);
                shift_detected = 0;
            }
        } else {
            if (shift_detected == 1) {
                shift_detected = 0;
            } else {
                layer_off(1);
                unregister_code(KC_LSFT);
                tap_code(KC_J);
                shift_detected = 0;
            }

        }
        return false;

    case K_JUMP:
        if (record -> event.pressed) {
            if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);

                tap_code(KC_UP);

                register_mods(mods);
                shift_detected = 1;
            } else if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);
                tap_code(KC_RIGHT);
                register_mods(mods);
                shift_detected = 1;
            } else {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);

                tap_code(KC_ESC);
                layer_on(1);

                register_mods(mods);
                shift_detected = 0;
            }
        } else {
            if (shift_detected == 1) {
                shift_detected = 0;
            } else {
                layer_off(1);
                unregister_code(KC_LSFT);
                tap_code(KC_K);
                shift_detected = 0;
            }

        }
        return false; // Skip all further processing of this key

    case EZ_BRC:
        if (record -> event.pressed) {
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);

                SEND_STRING("[]");
                tap_code(KC_LEFT);

                register_mods(mods);
            } else if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
                tap_code(KC_9);
                register_mods(mods);
            } else {
                SEND_STRING("[");
                hold_timer = timer_read();
                hold_detected = 1;
                repeat_started = 0;
                bracketType = BRC;
            }
        } else {
            hold_detected = 0;
            repeat_started = 0;
        }
        return false;

    case EZ_CBR:
        if (record -> event.pressed) {
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);

                SEND_STRING("{}");
                tap_code(KC_LEFT);

                register_mods(mods);
            } else if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
                tap_code(KC_0);
                register_mods(mods);
            } else {
                SEND_STRING("{");
                hold_timer = timer_read();
                hold_detected = 1;
                repeat_started = 0;
                bracketType = CBR;
            }
        } else {
            hold_detected = 0;
            repeat_started = 0;
        }
        return false;

    case EZ_PRN:
        if (record -> event.pressed) {
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);

                SEND_STRING("()");
                tap_code(KC_LEFT);

                register_mods(mods);
            } else if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
                tap_code(KC_1);
                register_mods(mods);
            } else {
                SEND_STRING("(");
                hold_timer = timer_read();
                hold_detected = 1;
                repeat_started = 0;
                bracketType = PRN;
            }
        } else {
            hold_detected = 0;
            repeat_started = 0;
        }
        return false;

    case EZ_LT:
        if (record -> event.pressed) {
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);

                SEND_STRING("<>");
                tap_code(KC_LEFT);

                register_mods(mods);
            } else if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
                tap_code(KC_2);
                register_mods(mods);
            } else {
                tap_code(KC_PMNS);
                hold_timer = timer_read();
                hold_detected = 1;
                repeat_started = 0;
                bracketType = LT;
            }
        } else {
            hold_detected = 0;
            repeat_started = 0;
        }
        return false;

    case EZ_QUOT:
        if (record -> event.pressed) {
            if ((mods | oneshot_mods) & MOD_MASK_CTRL) {
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);

                SEND_STRING("''");
                tap_code(KC_LEFT);

                register_mods(mods);
            } else if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
                SEND_STRING("?");
                register_mods(mods);
            } else {
                SEND_STRING("'");
                hold_timer = timer_read();
                hold_detected = 1;
                repeat_started = 0;
                bracketType = QUOT;
            }
        } else {
            hold_detected = 0;
            repeat_started = 0;
        }
        return false;

    case EZ_DQUO:
        if (record -> event.pressed) {
            if (((mods | oneshot_mods) & MOD_MASK_SHIFT) || ((mods | oneshot_mods) & MOD_MASK_CTRL)) {
                del_oneshot_mods(MOD_MASK_SHIFT);
                unregister_mods(MOD_MASK_SHIFT);
                del_oneshot_mods(MOD_MASK_CTRL);
                unregister_mods(MOD_MASK_CTRL);

                SEND_STRING("\"\"");
                tap_code(KC_LEFT);

                register_mods(mods);
            } else {
                SEND_STRING("\"");
                hold_timer = timer_read();
                hold_detected = 1;
                repeat_started = 0;
                bracketType = DQUO;
            }
        } else {
            hold_detected = 0;
            repeat_started = 0;
        }
        return false;

    case SHIFT_CTRL_PRTSC:
        if (record -> event.pressed) {
            // Send Shift + Control + Print Screen
            register_code(KC_LSFT);
            register_code(KC_LCTL);
            register_code(KC_PSCR);
            unregister_code(KC_PSCR);
            unregister_code(KC_LCTL);
            unregister_code(KC_LSFT);
        }
        break;

        // Fix control C for insert mode
    case KC_C:
        if (record -> event.pressed && (get_mods() & MOD_BIT(KC_LCTL))) {
            // Send Control + C
            SEND_STRING(SS_LCTL("c"));
            return false; // Cancel the original key press event
        }
        break;
    }

    return true;
}



void matrix_scan_user(void) {
    if (hold_detected) {
        if (!repeat_started && timer_elapsed(hold_timer) > 400) {
            // The key has been held down for more than 200ms, start repeating
            outputBracket(bracketType);
            repeat_timer = timer_read();  // Reset the repeat timer
            repeat_started = 1;
        } else if (repeat_started && timer_elapsed(repeat_timer) > 30) {
            // 10ms has passed since the last repeat
            outputBracket(bracketType);
            repeat_timer = timer_read();  // Reset the repeat timer
        }
    }
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(
          KC_ESC,        KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,
           KC_HASH,       KC_EXLM,  EZ_BRC,   EZ_CBR,   EZ_PRN,   EZ_LT,
           KC_UNDS,       KC_Z,     KC_M,     KC_L,     KC_D,     KC_F,
           KC_X,          KC_R,     KC_N,     KC_T,     KC_S,     KC_G,
           KC_ESC,        MO(1),    KC_B,     KC_W,     KC_C,     KC_V,
                          KC_AT,    KC_PERC,  K_JUMP,   J_JUMP,

                                             KC_LCTL,  KC_NO,
                                                       KC_LCTL,
                                     KC_LSFT, KC_ENT,  KC_LALT,


          KC_F9,         KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_SCRL,  KC_PAUS,   KC_NO,    QK_BOOT,
           KC_COLN,       KC_RPRN,  KC_RCBR,  KC_RBRC,  KC_PEQL,  KC_ASTR,
           KC_COMM,       KC_U,     KC_O,     KC_Y,     EZ_QUOT,  EZ_DQUO,
           KC_P,          KC_H,     KC_E,     KC_I,     KC_A,     KC_TAB,
           KC_BSPC,       KC_K,     KC_J,     KC_DOT,   KC_Q,     KC_SCLN,
                          KC_CIRC,  KC_DLR,   KC_SLSH,  KC_GRV,

                     KC_NO,   KC_UP,
                     KC_DOWN,
                     KC_LGUI, KC_LCTL, KC_SPC),



        [1] = LAYOUT(
          KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,         KC_NO,    KC_LBRC,  KC_LCBR,  KC_LPRN,  KC_PERC,
           KC_NO,         KC_NO,    KC_CIRC,  KC_UP,    KC_DLR,   KC_NO,
           KC_BSPC,       KC_NO,    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_NO,
           KC_LSFT,       KC_TRNS,  KC_B,     KC_W,     KC_NO,    KC_NO,
                          KC_NO,    KC_NO,    KC_NO,    KC_PLUS,

                                               KC_NO, KC_NO,
                                                      KC_NO,
                                      KC_LSFT, MO(2), KC_NO,

          KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_COLN,       KC_RPRN,  KC_RCBR,  KC_RBRC,  KC_ASTR,  KC_PEQL,
           KC_COMM,       KC_7,     KC_8,     KC_9,     KC_NO,  KC_QUOT,
           KC_AMPR,       KC_4,     KC_5,     KC_6,     KC_NO,    KC_TAB,
           KC_BSPC,       KC_1,     KC_2,     KC_3,     KC_NO,    KC_PLUS,
                          KC_DOT,   KC_0,     KC_SLSH,  KC_NO,

                      KC_NO, KC_NO,
                      KC_NO,
                      KC_NO, KC_LCTL, KC_SPC),



        [2] = LAYOUT(
          KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,     KC_NO,     KC_NO,
           KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_TRNS,       KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
                          KC_NO,    KC_NO,    KC_NO,    KC_NO,

                                               KC_NO, KC_NO,
                                                      KC_NO,
                                      KC_ESC, KC_ENT, KC_NO,


          KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,     KC_NO,     KC_NO,
           KC_NO,         KC_F10,   KC_F11,   KC_F12,   KC_NO,    KC_NO,
           KC_NO,         KC_F7,    KC_F8,    KC_F9,    KC_NO,    KC_NO,
           KC_NO,         KC_F4,    KC_F5,    KC_F6,    KC_NO,    KC_NO,
           KC_NO,         KC_F1,    KC_F2,    KC_F3,    KC_NO,    KC_NO,
                          KC_NO,    KC_NO,    KC_NO,    KC_NO,

                      KC_NO, KC_NO,
                      KC_NO,
                      KC_NO, KC_NO, KC_NO)

};
