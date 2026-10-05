#pragma once

/*
------------------------------------------------------------
GlassGarden

File : StateManager.h

وظیفه:
نگهداری وضعیت لحظه‌ای سیستم

تمام بخش‌های نرم‌افزار فقط از این کلاس
وضعیت سیستم را می‌خوانند یا تغییر می‌دهند.

Version : 1.1.0
------------------------------------------------------------
*/

class StateManager
{
public:

    // راه‌اندازی
    void begin();

    // بروزرسانی
    void update();

    // تجهیزات
    bool light = false;
    bool fan = false;
    bool fogger = false;
    bool pump = false;

    // سنسورها
    float temperature = 0.0f;
    float humidity = 0.0f;

    // سطح آب مخزن (٪) بر اساس سنسور آنالوگ P100
    int waterLevelPercent = 0;

    // true = مخزن آب خالی است (هشدار)
    // با هیسترزیس در SensorManager تعیین می‌شود تا
    // نوسان مقدار خام سنسور باعث تغییر مکرر نشود
    bool waterEmpty = false;

    // زمان آخرین خواندن معتبر سنسور DHT (میلی‌ثانیه از بوت)
    // توسط SensorManager بروزرسانی می‌شود؛ برای تشخیص
    // قطعی طولانی سنسور و ورود به Safe Mode استفاده می‌شود
    unsigned long lastValidSensorReadMs = 0;

    // حالت ایمن (Safe Mode)
    // true = سنسور DHT برای مدت طولانی قطع بوده و فن/پمپ/
    // مه‌ساز به‌صورت خودکار خاموش نگه داشته می‌شوند.
    // توسط AutomationManager بروزرسانی می‌شود.
    bool safeMode = false;

    // ارتباط
    bool wifiConnected = false;
    bool blynkConnected = false;

    // حالت اتوماسیون : true = AUTO | false = MANUAL
    bool autoMode = true;

};

extern StateManager state;