MCU = RP2040
BOOTLOADER = rp2040

SPLIT_KEYBOARD = yes
OLED_ENABLE = yes
LTO_ENABLE = yes

TAP_DANCE_ENABLE = yes
KEY_OVERRIDE_ENABLE = yes
ENCODER_ENABLE = yes

SERIAL_DRIVER = vendor
SRC += quantum/split_common/transactions.c
