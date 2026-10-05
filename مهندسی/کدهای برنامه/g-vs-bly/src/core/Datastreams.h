#pragma once

/*
------------------------------------------------------------
GlassGarden

File : Datastreams.h

وظیفه:
تعریف تمام Virtual Pin های پروژه Blynk

تمام شماره‌های Datastream فقط در این فایل
تعریف می‌شوند.

Version : 1.0.0
------------------------------------------------------------
*/

#include <Arduino.h>

//============================================================
// Output Devices
//============================================================

// روشنایی
constexpr uint8_t VPIN_LIGHT = V0;

// مه‌ساز
constexpr uint8_t VPIN_FOGGER = V3;

// فن
constexpr uint8_t VPIN_FAN = V4;

// پمپ
constexpr uint8_t VPIN_PUMP = V5;

//============================================================
// Sensors
//============================================================

// دما
constexpr uint8_t VPIN_TEMPERATURE = V1;

// رطوبت
constexpr uint8_t VPIN_HUMIDITY = V2;

//============================================================
// Mode
//============================================================

// سوییچ حالت AUTO / MANUAL
constexpr uint8_t VPIN_AUTO_MODE = V6;

// سطح آب مخزن (٪)
constexpr uint8_t VPIN_WATER_LEVEL = V7;

//============================================================
// Future
//============================================================

// constexpr uint8_t VPIN_WATER_TEMP  = V8;
// constexpr uint8_t VPIN_LIGHT_LEVEL = V9;
// constexpr uint8_t VPIN_WIFI        = V10;
// constexpr uint8_t VPIN_SYSTEM      = V11;