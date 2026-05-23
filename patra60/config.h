#pragma once

/* 分割キーボード通信設定（現行QMK準拠のPIO定義） */
#define SERIAL_DRIVER_PIO             // vendorドライバーの実体をPIOに指定
#define SERIAL_PIO_PIN GP14           // 通信に使用する物理ピン
#define SERIAL_PIO_HALF_DUPLEX        // 1本線（半二重）モードを強制

/* I2C設定 */
#define I2C_DRIVER I2CD1
#define OLED_DISPLAY_128X32
#define OLED_TIMEOUT 180000

#define I2C1_SDA_PIN GP26
#define I2C1_SCL_PIN GP27
#define I2C1_SPEED_400KHZ

/* 分割・自動判定定義 */
#define MASTER_LEFT
#define SPLIT_USB_DETECT
#define RPC_SYNC_OLED 1
