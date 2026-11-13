---
type: reference
tags: [code, github]
date: 2026-09-24
---

# ارجاع به ریپوی کد

کد اصلی، PCB، و مستندات فرمت‌شده اینجاست:
🔗 https://github.com/hldvd/GG-SMART

## ساختار مخزن

```
GlassGarden/
├── Archive/                          — نسخه‌های قدیمی
├── Docs/                             — اسناد رسمی (PDF/DOCX)
│   ├── 01-مدیریتی/                   — اسناد مدیریت پروژه
│   ├── 02-سخت‌افزار/                  — سند طراحی سخت‌افزار (HDD)
│   ├── 03-نرم‌افزار/                  — سند طراحی نرم‌افزار (SDD)
│   ├── 04-سازه/                      — سند مرجع سازه
│   ├── 05-طراحی-داخلی/               — طراحی داخلی و محوطه
│   └── 06-تست-و-اعتبارسنجی/           — تست و اعتبارسنجی
├── GlassGarden - local/              — کد فریم‌ورک (PlatformIO)
│   ├── platformio.ini
│   └── src/
│       ├── core/                     — Config.h, Datastreams.h
│       ├── hardware/                 — Hardware, Outputs
│       ├── sensors/                  — SensorManager
│       ├── devices/                  — DeviceManager + Light/Fan/Fogger/Pump
│       ├── automation/               — AutomationManager
│       ├── blynk/                    — BlynkManager + Handlers
│       ├── network/                  — NetworkManager
│       ├── state/                    — StateManager
│       ├── system/                   — SystemManager
│       └── webui/                    — WebUIManager + HTML/CSS/Images
└── README.md
```

→ برای فهرست اسناد رسمی: [[Docs-Index]]
→ برای معماری نرم‌افزار: [[Firmware-Architecture]]
