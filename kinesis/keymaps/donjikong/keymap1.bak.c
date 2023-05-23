#include QMK_KEYBOARD_H




#include "features/custom_shift_keys.h"

const custom_shift_key_t custom_shift_keys[] = {
  {KC_RPRN, KC_PIPE}, // Shift ) is |
  {KC_RCBR, KC_TILD}, // Shift } is ~
  {KC_PERC, KC_AMPR}, // Shift % is &
  {KC_HASH, KC_QUES}, // Shift # is ?  
};
uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);





enum custom_keycodes {
  SHIFT_CTRL_PRTSC = SAFE_RANGE,
};


bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_custom_shift_keys(keycode, record)) { return false; }

  switch (keycode) {
    case SHIFT_CTRL_PRTSC:
      if (record->event.pressed) {
        // Send Shift + Control + Print Screen
        register_code(KC_LSFT);
        register_code(KC_LCTL);
        register_code(KC_PSCR);
        unregister_code(KC_PSCR);
        unregister_code(KC_LCTL);
        unregister_code(KC_LSFT);
      }
      break;
    case KC_C:
      if (record->event.pressed && (get_mods() & MOD_BIT(KC_LCTRL))) {
        // Send Control + C
        SEND_STRING(SS_LCTL("c"));
        return false; // Cancel the original key press event
      }
      break;
  }
  
  return true;
}







const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(
          KC_ESC,        KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,
           KC_AT,         KC_EXLM,  KC_LBRC,  KC_LCBR,  KC_LPRN,  KC_UNDS,  
           KC_SCLN,       KC_Z,     KC_M,     KC_L,     KC_D,     KC_F, 
           KC_X,          KC_R,     KC_N,     KC_T,     KC_S,     KC_G, 
           KC_ESC,        KC_PMNS,  KC_B,     KC_W,     KC_C,     KC_V, 
                          KC_CIRC,  KC_GRV,   KC_PEQL,  KC_DLR,  
                                  
                                             KC_BSPC, KC_PGUP, 
                                                      KC_LALT, 
                                     KC_LSFT, MO(1), KC_LCTL, 

           
          KC_F9,         KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_SCRL,  KC_PAUS,   KC_NO,    QK_BOOT, 
           KC_COLN,       KC_RPRN,  KC_RCBR,  KC_RBRC,  KC_ASTR,  KC_PLUS,
           KC_COMM,       KC_U,     KC_O,     KC_Y,     KC_QUOT,  KC_DQUO, 
           KC_P,          KC_H,     KC_E,     KC_I,     KC_A,     KC_ENT, 
           KC_BSPC,       KC_K,     KC_J,     KC_DOT,   KC_Q,     KC_TAB,  
                          KC_PERC,  KC_HASH,  KC_SLSH,  KC_BSLS, 
                 
                     KC_PGUP, KC_PGDN, 
                     KC_TAB, 
                     KC_LGUI, KC_LCTL, KC_SPC),

         

        [1] = LAYOUT(
          KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
           KC_NO,         KC_NO,    KC_LBRC,  KC_LCBR,  KC_LPRN,  KC_PERC,
           KC_NO,         KC_NO,    KC_CIRC,  KC_UP,    KC_DLR,   KC_NO, 
           KC_NO,         KC_NO,    KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_NO, 
           KC_LSFT,       KC_PMNS,  KC_B,     KC_W,     KC_NO,    KC_NO, 
                          KC_NO,    KC_NO,    KC_NO,    SHIFT_CTRL_PRTSC,

                                               KC_NO, KC_NO,
                                                      KC_NO, 
                                       MO(2), KC_TRNS,KC_NO,
	
          KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, 
           KC_COLN,       KC_RPRN,  KC_RCBR,  KC_RBRC,  KC_ASTR,  KC_PLUS, 
           KC_COMM,       KC_7,     KC_8,     KC_9,     KC_QUOT,  KC_DQUO, 
           KC_AMPR,       KC_4,     KC_5,     KC_6,     KC_AT,    KC_ENT, 
           KC_BSPC,       KC_1,     KC_2,     KC_3,     KC_NO,    KC_TAB,
                          KC_DOT,   KC_0,     KC_SLSH,  KC_BSLS, 

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
