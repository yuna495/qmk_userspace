#include QMK_KEYBOARD_H
#include <string.h>

// --- OLED用ダミー変数 ---
#ifdef OLED_ENABLE
static char last_str[6] = "     ";
static void set_last(const char *s) {
  for (uint8_t i = 0; i < sizeof(last_str) - 1; i++) last_str[i] = ' ';
  last_str[sizeof(last_str) - 1] = '\0';
  uint8_t i = 0;
  while (s[i] != '\0' && i < sizeof(last_str) - 1) {
    last_str[i] = s[i];
    i++;
  }
}
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}
#else
static void set_last(const char *s) {}
#endif

// 英語配列の「Ctrl+Space」を1つのキーコードとして定義
#define KC_IME_SW LCTL(KC_SPC)
#define KC_ALT_PSCR LALT(KC_PSCR)

// --- カスタムキーコードの定義 ---
enum custom_keycodes {
  M_PASTE = SAFE_RANGE,
  M_CODE_BLOCK,
  M_GRAVE
};

// --- 1. タップダンスIDの定義 ---
enum tap_dance_enums {
  TD_LPRN_RPRN = 0, // ( -> )
  TD_LBRC_RBRC,     // [ -> ]
  TD_Z_UNDO,        // Z -> Ctrl+Z
  TD_X_CUT,         // X -> Ctrl+X
  TD_C_COPY,        // C -> Ctrl+C
  TD_V_PASTE,       // V -> Ctrl+V
  TD_MINUS_M2,      // - -> —— (Alt+0151 x2)
  TD_COMM_M3,       // , -> …… (Alt+0133 x2)
  TD_DOT_SLSH,      // . -> /
  TD_ORDER_LAYER,   // TO(0), TO(1), TO(2)
  TD_SCLN_QUOT,     // ; -> '
  TD_MPLY_ALT_PSCR  // MPLY -> ALT_PSCR
};

// --- 2. タップダンス関数の定義 ---
void td_layer_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { layer_move(0); set_last("L_Def"); }
    else if (state->count == 2) { layer_move(1); set_last("L_Num"); }
    else if (state->count >= 3) { layer_move(2); set_last("L_Fn"); }
}

void td_lprn_rprn_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_LPRN); set_last("("); }
    else if (state->count == 2) { tap_code16(KC_RPRN); set_last(")"); }
}

void td_lbcr_rbcr_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_LBRC); set_last("["); }
    else if (state->count == 2) { tap_code16(KC_RBRC); set_last("]"); }
}

void td_z_undo_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_Z); set_last("Z"); }
    else if (state->count == 2) { tap_code16(LCTL(KC_Z)); set_last("UNDO"); }
}

void td_x_cut_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_X); set_last("X"); }
    else if (state->count == 2) { tap_code16(LCTL(KC_X)); set_last("CUT"); }
}

void td_c_copy_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_C); set_last("C"); }
    else if (state->count == 2) { tap_code16(LCTL(KC_C)); set_last("COPY"); }
}

void td_v_paste_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_V); set_last("V"); }
    else if (state->count == 2) { tap_code16(LCTL(KC_V)); set_last("PSTE"); }
}

void td_minus_m2_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_MINS); set_last("-"); }
    else if (state->count == 2) {
        for (int i = 0; i < 2; i++) {
            register_code(KC_LALT);
            tap_code(KC_P0); tap_code(KC_P1); tap_code(KC_P5); tap_code(KC_P1);
            unregister_code(KC_LALT);
        }
        set_last("--");
    }
}

void td_comm_m3_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_COMM); set_last(","); }
    else if (state->count == 2) {
        for (int i = 0; i < 2; i++) {
            register_code(KC_LALT);
            tap_code(KC_P0); tap_code(KC_P1); tap_code(KC_P3); tap_code(KC_P3);
            unregister_code(KC_LALT);
        }
        set_last("...");
    }
}

void td_dot_slsh_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_DOT); set_last("."); }
    else if (state->count == 2) { tap_code16(KC_SLSH); set_last("/"); }
}

void td_scln_quot_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) { tap_code16(KC_SCLN); set_last(";"); }
    else if (state->count >= 2) { tap_code16(KC_QUOT); set_last("'"); }
}

void td_mply_alt_pscr_finished(tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) { tap_code16(KC_MPLY); set_last("MPLY"); }
  else if (state->count >= 2) { tap_code16(KC_ALT_PSCR); set_last("ALT_PSCR");}
}

// --- 3. タップダンス設定 ---
tap_dance_action_t tap_dance_actions[] = {
  [TD_LPRN_RPRN] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_lprn_rprn_finished, NULL),
  [TD_LBRC_RBRC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_lbcr_rbcr_finished, NULL),
  [TD_Z_UNDO]    = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_z_undo_finished, NULL),
  [TD_X_CUT]     = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_x_cut_finished, NULL),
  [TD_C_COPY]    = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_c_copy_finished, NULL),
  [TD_V_PASTE]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_v_paste_finished, NULL),
  [TD_MINUS_M2]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_minus_m2_finished, NULL),
  [TD_COMM_M3]   = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_comm_m3_finished, NULL),
  [TD_DOT_SLSH]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_dot_slsh_finished, NULL),
  [TD_ORDER_LAYER] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_layer_finished, NULL),
  [TD_SCLN_QUOT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_scln_quot_finished, NULL),
  [TD_MPLY_ALT_PSCR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_mply_alt_pscr_finished, NULL)
};

void send_backtick(void) {
    register_code(KC_LALT);
    tap_code(KC_P0); tap_code(KC_P9); tap_code(KC_P6);
    unregister_code(KC_LALT);
}

void send_space(void) {
    register_code(KC_LALT);
    tap_code(KC_P0); tap_code(KC_P3); tap_code(KC_P2);
    unregister_code(KC_LALT);
}

const key_override_t *key_overrides[] = { NULL };

// --- 5. プロセスレコード ---
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static bool bspc_substituted = false;
  if (keycode == KC_BSPC) {
    if (record->event.pressed) {
      if (get_mods() & MOD_MASK_SHIFT) {
        bspc_substituted = true;
        register_code(KC_DEL);
        set_last("DEL");
        return false;
      }
    } else {
      if (bspc_substituted) {
        unregister_code(KC_DEL);
        bspc_substituted = false;
        return false;
      }
    }
  }

  static bool i_substituted = false;
  if (keycode == KC_I) {
    if (record->event.pressed) {
      if (get_mods() & MOD_MASK_CTRL) {
        i_substituted = true;
        register_code(KC_S);
        set_last("SAVE");
        return false;
      }
    } else {
      if (i_substituted) {
        unregister_code(KC_S);
        i_substituted = false;
        return false;
      }
    }
  }

  switch (keycode) {
    case M_PASTE:
      if (record->event.pressed) {
        send_space(); send_backtick(); tap_code16(LCTL(KC_V)); wait_ms(10); send_backtick(); send_space(); set_last("PSTE");
      }
      return false;
    case M_CODE_BLOCK:
      if (record->event.pressed) {
        send_backtick(); send_backtick(); send_backtick(); tap_code16(LSFT(KC_ENT)); tap_code16(LCTL(KC_V)); wait_ms(20); tap_code16(LSFT(KC_ENT)); send_backtick(); send_backtick(); send_backtick(); set_last("CBLK");
      }
      return false;
    case M_GRAVE:
      if (record->event.pressed) { send_backtick(); set_last("`"); }
      return false;
  }

  if (!record->event.pressed) return true;

#ifdef OLED_ENABLE
  uint16_t kc = keycode;
  if (kc >= KC_A && kc <= KC_Z) {
    char s[2] = { (char)('A' + (kc - KC_A)), '\0' }; set_last(s); return true;
  }
  if (kc >= KC_1 && kc <= KC_0) {
    char s[2] = { (kc == KC_0 ? '0' : (char)('1' + (kc - KC_1))), '\0' }; set_last(s); return true;
  }
  if (kc >= KC_F1 && kc <= KC_F12) { set_last("Fn"); return true; }
  if (kc >= KC_P1 && kc <= KC_P0) { set_last("Pad"); return true; }

  switch (kc) {
    case KC_SPC:  set_last("SPC"); break;
    case KC_ENT:  set_last("ENT"); break;
    case KC_BSPC: set_last("BSP"); break;
    case KC_TAB:  set_last("TAB"); break;
    case KC_ESC:  set_last("ESC"); break;
    case KC_DEL:  set_last("DEL"); break;
    case KC_MINS: set_last("-"); break;
    case KC_EQL:  set_last("="); break;
    case KC_LBRC: set_last("["); break;
    case KC_RBRC: set_last("]"); break;
    case KC_BSLS: set_last("\\"); break;
    case KC_SCLN: set_last(";"); break;
    case KC_QUOT: set_last("'"); break;
    case KC_GRV:  set_last("`"); break;
    case KC_COMM: set_last(","); break;
    case KC_DOT:  set_last("."); break;
    case KC_SLSH: set_last("/"); break;
    case KC_LALT: case KC_RALT: set_last("ALT"); break;
    case KC_LCTL: case KC_RCTL: set_last("CTL"); break;
    case KC_LSFT: case KC_RSFT: set_last("SFT"); break;
    case KC_LGUI: case KC_RGUI: set_last("GUI"); break;
    default: break;
  }
#endif
  return true;
}

// --- 6. キーマップの設定 ---
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_ESC,    KC_1,          KC_2,         KC_3,          KC_4,           KC_5,
    KC_TAB,    KC_Q,          KC_L,         KC_E,          TD(TD_COMM_M3), TD(TD_DOT_SLSH),
    KC_LCTL,   KC_A,          KC_I,         KC_U,          KC_O,           TD(TD_LBRC_RBRC), KC_MPLY,
    KC_LSFT,   TD(TD_Z_UNDO), TD(TD_X_CUT), TD(TD_C_COPY), TD(TD_V_PASTE), TD(TD_SCLN_QUOT), KC_DEL,
                              KC_LGUI,      KC_LALT,       LT(1, KC_SPC),  LT(2, KC_ENT),    KC_BSPC,
                  KC_6,          KC_7,      KC_8,   KC_9,      KC_0,  TD(TD_MPLY_ALT_PSCR),
                  KC_W,          KC_P,      KC_R,   KC_Y,      KC_F,  M_GRAVE,
    KC_ALT_PSCR,  KC_K,          KC_T,      KC_N,   KC_S,      KC_H,  TD(TD_MINUS_M2),
    KC_EQL,       KC_G,          KC_D,      KC_M,   KC_J,      KC_B,  KC_IME_SW,
    KC_SPC,       LT(2, KC_ENT), KC_BSPC,   KC_DEL, TD(TD_ORDER_LAYER)
  ),
  [1] = LAYOUT(
    KC_ESC,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,
    KC_TRNS,   KC_AT,   KC_UP,   KC_UNDS, KC_HASH, KC_DLR,
    KC_LCTL,   KC_LEFT, KC_DOWN, KC_RGHT, KC_TILD, TD(TD_LPRN_RPRN), KC_HOME,
    KC_LSFT,   KC_PIPE, KC_AMPR, KC_EXLM, KC_QUES, KC_CIRC,          KC_EQL,
                        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_TRNS,
              KC_P7,   KC_P8,   KC_P9,   KC_PMNS, KC_PERC, M_GRAVE,
    KC_TRNS,  KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_PSLS, KC_BSLS,
    KC_P0,    KC_P1,   KC_P2,   KC_P3,   KC_PDOT, KC_PAST, KC_EQL,
    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [2] = LAYOUT(
    KC_ESC,        LCTL(LALT(KC_DEL)), LWIN(LCTL(KC_LEFT)), LWIN(LCTL(KC_RGHT)), LWIN(KC_TAB), LALT(KC_TAB),
    LWIN(KC_LEFT), LWIN(KC_DOWN),      LWIN(KC_UP),         LWIN(KC_RGHT),       LWIN(KC_UP),  LWIN(KC_DOWN),
    TO(0),  KC_PGUP,  KC_PGDN,  KC_HOME, KC_END,  KC_INS,   QK_BOOT,
    TO(1),  KC_MUTE,  KC_VOLD,  KC_VOLU, M_PASTE, M_CODE_BLOCK,  KC_TRNS,
                      KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,
              KC_F10,  KC_F11,  KC_F12,   KC_F13,  KC_F14,  TO(3),
              KC_F7,   KC_F8,   KC_F9,    KC_INS,  KC_END,  KC_HOME,
    QK_BOOT,  KC_F4,   KC_F5,   KC_F6,    KC_RSFT, KC_UP,   KC_RCTL,
    KC_PGUP,  KC_F1,   KC_F2,   KC_F3,    KC_LEFT, KC_DOWN, KC_RGHT,
    KC_PGDN,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [3] = LAYOUT(
    KC_ESC,    KC_TAB,  KC_1,    KC_2,    KC_3,    KC_4,
    KC_T,      KC_LCTL, KC_Q,    KC_W,    KC_E,    KC_R,
    KC_G,      KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,   KC_5,
    KC_N,      KC_M,    KC_Z,    KC_X,    KC_C,    KC_V,   KC_B,
                        KC_TRNS, KC_LALT, KC_SPC,  KC_SPC, KC_ENT,
              KC_F8,    KC_F9,   KC_F10,  KC_Y,    KC_U,    KC_I,
              KC_F5,    KC_F6,   KC_F7,   KC_H,    KC_J,    KC_K,
    KC_TRNS,  KC_F4,    KC_F5,   KC_F6,   KC_M,    KC_UP,   KC_RCTL,
    KC_TRNS,  KC_F1,    KC_F2,   KC_F3,   KC_DOWN, KC_LEFT, KC_RGHT,
    KC_TRNS,  KC_TRNS,  LAFT,    KC_TRNS, TO(0)
  )
};

void keyboard_post_init_user(void) {}

// --- 7. ロータリーエンコーダー設定 ---
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) {
    if (clockwise) tap_code(KC_VOLU); else tap_code(KC_VOLD);
  } else if (index == 1) {
    if (clockwise) tap_code(KC_RGHT); else tap_code(KC_LEFT);
  }
  return false;
}
#endif

// --- 8. OLED描画処理 (単体動作版) ---
#ifdef OLED_ENABLE
static void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color) {
  for (uint8_t i = 0; i < w; i++) {
    for (uint8_t j = 0; j < h; j++) { oled_write_pixel(x + i, y + j, color); }
  }
}
bool oled_task_user(void) {
  oled_set_cursor(0, 0);  oled_write_P(PSTR("Patra"), false);
  oled_set_cursor(0, 2);  oled_write_P(PSTR("Lay:"), false);
  uint8_t this_layer = get_highest_layer(layer_state);
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t row = 3 + i; oled_set_cursor(0, row);
    switch (i) {
      case 0: oled_write_P(PSTR("Def"), false); break;
      case 1: oled_write_P(PSTR("Num"), false); break;
      case 2: oled_write_P(PSTR("Fn "), false); break;
      case 3: oled_write_P(PSTR("Ply"), false); break;
    }
    if (i == this_layer) draw_rect(24, row * 8, 8, 8, true);
    else draw_rect(24, row * 8, 8, 8, false);
  }
  oled_set_cursor(0, 8);  oled_write_P(PSTR("Mod:"), false);
  oled_set_cursor(0, 9);
  uint8_t mods = get_mods() | get_oneshot_mods();
  oled_write_P(mods & MOD_MASK_SHIFT ? PSTR("S") : PSTR("_"), false);
  oled_write_P(mods & MOD_MASK_CTRL  ? PSTR("C") : PSTR("_"), false);
  oled_write_P(mods & MOD_MASK_ALT   ? PSTR("A") : PSTR("_"), false);
  oled_write_P(mods & MOD_MASK_GUI   ? PSTR("G") : PSTR("_"), false);
  oled_write_P(PSTR("\nLast:\n"), false); oled_write(last_str, false);
  return false;
}
#endif

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT(1, KC_SPC): return 110;
    case MT(KC_LSFT, KC_TAB): case LT(2, KC_ENT): return 150;
    default: return TAPPING_TERM;
  }
}
