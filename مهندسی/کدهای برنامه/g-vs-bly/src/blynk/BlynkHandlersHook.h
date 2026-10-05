#pragma once

/*
------------------------------------------------------------
GlassGarden

File : BlynkHandlers.h

وظیفه:
اعلان تابع registerBlynkHandlers که هیچ کاری انجام نمی‌دهد،
اما فراخوانی آن از main.cpp باعث می‌شود لینکر فایل
BlynkHandlers.cpp را به‌عنوان "کد استفاده‌نشده" حذف نکند.
بدون این فراخوانی، توابع BLYNK_WRITE داخل این فایل توسط
Dead Code Elimination لینکر حذف می‌شوند و فرمان‌های دستی
از اپلیکیشن هرگز به ESP32 نمی‌رسند.

Version : 1.0.0
------------------------------------------------------------
*/

void registerBlynkHandlers();