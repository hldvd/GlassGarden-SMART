#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//======================================================
// GlassGarden Configuration
// نسخه : 1.1.0
//======================================================

//-------------- Project -----------------
#define PROJECT_NAME "GlassGarden"
#define PROJECT_VERSION "1.1.0"

//-------------- Serial ------------------
constexpr uint32_t SERIAL_BAUDRATE = 115200;

//-------------- WiFi & Blynk (from ConfigLocal.h) --------------------
#include "ConfigLocal.h"

//-------------- GPIO --------------------
// رله‌ها (Active LOW)
constexpr uint8_t PUMP_PIN      = 26;  // 💧 رله ۱
constexpr uint8_t LIGHT_PIN     = 27;  // 💡 رله ۲
constexpr uint8_t FOGGER_PIN    = 32;  // 💨 رله ۳
constexpr uint8_t FAN_PIN       = 33;  // 🌀 رله ۴

//-------------- Sensors -----------------
constexpr uint8_t DHT_PIN       = 17;  // 🌡️ DHT22
constexpr uint8_t WATER_PIN     = 4;   // 📏 سنسور سطح آب P100

// نوع سنسور : 11 = DHT11 | 22 = DHT22/AM2302 | 21 = DHT21
constexpr uint8_t DHT_TYPE      = 22;

//-------------- Relay Logic -------------
// رله Active LOW: LOW = روشن، HIGH = خاموش
constexpr bool OUTPUT_ACTIVE_HIGH = false;

//-------------- Water Level -------------
constexpr uint16_t WATER_LEVEL_EMPTY = 500;
constexpr uint16_t WATER_LEVEL_FULL  = 3500;

// حاشیهٔ هیسترزیس (واحد خام ADC) برای خروج از حالت "خالی"
// جلوگیری از نوسان سریع هشدار وقتی مقدار خام نزدیک مرز است
constexpr uint16_t WATER_LEVEL_EMPTY_HYSTERESIS = 150;

//-------------- Automation --------------
constexpr bool AUTOMATION_ENABLED = true;

// فن بر اساس دما (سانتی‌گراد)
constexpr float FAN_TEMP_ON  = 28.0f;
constexpr float FAN_TEMP_OFF = 26.0f;

// فن بر اساس رطوبت (درصد)
constexpr float FAN_HUMIDITY_ON  = 75.0f;
constexpr float FAN_HUMIDITY_OFF = 70.0f;

// مه‌ساز بر اساس رطوبت (درصد)
constexpr float FOGGER_HUMIDITY_ON  = 60.0f;
constexpr float FOGGER_HUMIDITY_OFF = 70.0f;

// روشنایی بر اساس زمان‌بندی (ساعت 24 ساعته)
constexpr int LIGHT_ON_HOUR  = 8;
constexpr int LIGHT_OFF_HOUR = 20;

// فن بر اساس زمان‌بندی
constexpr int FAN_ON_HOUR  = 8;
constexpr int FAN_OFF_HOUR = 20;

// مه‌ساز بر اساس زمان‌بندی
constexpr int FOGGER_ON_HOUR  = 6;
constexpr int FOGGER_OFF_HOUR = 22;

// پمپ بر اساس زمان‌بندی
constexpr int PUMP_ON_HOUR  = 8;
constexpr int PUMP_OFF_HOUR = 20;

//-------------- NTP (Time Sync) ---------
constexpr char NTP_SERVER[] = "pool.ntp.org";
constexpr long NTP_GMT_OFFSET_SEC = 12600;
constexpr int NTP_DAYLIGHT_OFFSET_SEC = 0;

#endif
