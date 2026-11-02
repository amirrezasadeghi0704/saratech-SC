# saratech_sc

کتابخانه کنترل ربات STM32 شامل کنترل موتور، خواندن سنسور، نمایش OLED، تشخیص توپ و حرکت خودکار. از پروژه اصلی Secondry_ST_02 برده شده است.

STM32 robot control library with motor control, sensor reading, OLED display, ball detection, and autonomous movement. Ported from original Secondry_ST_02 project.

## نصب / Installation

پوشه `saratech_sc` را در دایرکتوری کتابخانه‌های Arduino (`Arduino/libraries/`) قرار دهید یا از طریق Arduino IDE **Sketch -> Include Library -> Add .ZIP Library** وارد کنید.

Place the `saratech_sc` folder into your Arduino libraries directory (`Arduino/libraries/`), or import via Arduino IDE **Sketch -> Include Library -> Add .ZIP Library**.

## شروع سریع / Quick Start

```cpp
#include <saratech_sc.h>

void setup() {
    saratech.begin();
}

void loop() {
    saratech.sensor();
    saratech.printall();
    if (saratech.is_goaller)
        saratech.goaller();
    else
        saratech.forward();
}
```

## پارامترهای قابل تنظیم / Adjustable Parameters

| پارامتر / Parameter | روش / Method | پیش‌فرض / Default | توضیح / Description |
|---|---|---|---|
| حداکثر PWM موتور / MaxMotor | `setMaxMotor(int)` | 65000 | حد خروجی موتور / Motor output limit |
| حالت دروازه‌بان / IsGoaller | `setIsGoaller(bool)` | false | true=دروازه‌بان، false=حمله / true=goalkeeper, false=forward |
| حساسیت نوری / LdrSens | `setLdrSens(int)` | 300 | آستانه فعال‌سازی سنسور نوری / Photodiode trigger threshold |
| زمان‌بندی OLED / OledTimeout | `setOledTimeout(int)` | 5000ms | غیرفعال‌سازی خودکار OLED پس از این زمان / Auto-off OLED after ms |
| شیفت جلو / KfS | `setKfS(int)` | 0 | مقدار کالیبراسیون سنسور جلو / Front photodiode calibration |
| شیفت راست / KrS | `setKrS(int)` | 0 | مقدار کالیبراسیون سنسور راست / Right photodiode calibration |
| شیفت عقب / KbS | `setKbS(int)` | 0 | مقدار کالیبراسیون سنسور عقب / Back photodiode calibration |
| شیفت چپ / KlS | `setKlS(int)` | 0 | مقدار کالیبراسیون سنسور چپ / Left photodiode calibration |

## مرجع API / API Reference

### راه‌اندازی / Initialization

- `saratech.begin()` — راه‌اندازی پین‌ها، OLED، ژیروسکوپ I2C و کالیبراسیون سنسورها / Initialize all pins, OLED, I2C gyro, and sensor calibration

### کنترل حرکت / Motion Control

- `saratech.move(angle)` — حرکت با زاویه و سرعت v فعلی / Move at current speed v by angle
- `saratech.moveSpeed(angle, speed)` — حرکت با زاویه و سرعت مشخص / Move by angle and specified speed
- `saratech.movexy(vx, vy)` — حرکت با سرعت x/y (محدوده -65535 تا 65535) / Move by x/y velocity (range -65535 to 65535)
- `saratech.movesecond(angle, seconds)` — حرکت با زاویه به مدت ثانیه / Move by angle for seconds
- `saratech.stop()` — توقف همه موتورها / Stop all motors

### سنسورها / Sensors

- `saratech.sensor()` — خواندن تمام داده‌های سنسور / Read all sensor data
- `saratech.printall()` — بروزرسانی نمایش OLED / Refresh OLED display

### دریافت وضعیت / Status Queries

- `saratech.get_isball()` — تشخیص توپ / Is ball detected
- `saratech.get_ball_in_kicker()` — توپ در شوتر / Is ball in kicker
- `saratech.get_ballangle()` — زاویه توپ (درجه) / Ball angle (degrees)
- `saratech.get_balldistance()` — فاصله توپ / Ball distance
- `saratech.get_gy()` — زاویه ژیروسکوپ / Gyroscope angle
- `saratech.get_shb/shl/shr/d()` — سنسور پایین/چپ/راست/اختلاف / Bottom/left/right/diff sensors
- `saratech.get_kf/kr/kb/kl()` — وضعیت نوری چهار جهت / Four-direction photodiode status
- `saratech.get_ball_stop_cnt()` — شمارنده توقف توپ / Ball stop count

### قابلیت‌های خودکار / Autonomous Functions

- `saratech.out()` — تشخیص خروج از زمین و اصلاح / Out-of-bounds detection and correction
- `saratech.move_inside()` — بررسی داخلی زمین / Internal inside-bounds check
- `saratech.shoot()` — فعال‌سازی شوت / Activate shooter
- `saratech.forward()` — حالت حمله و تعقیب توپ / Forward chase-ball mode
- `saratech.goaller()` — حالت دروازه‌بان / Goalkeeper mode

## پین‌های سخت‌افزار / Hardware Pins

| عملکرد / Function | پین / Pin |
|---|---|
| جهت موتور راست جلو / MR1 dir | PB12 |
| جهت موتور راست عقب / MR2 dir | PB13 |
| جهت موتور چپ عقب / ML2 dir | PB14 |
| جهت موتور چپ جلو / ML1 dir | PB15 |
| PWM موتور راست جلو / MR1 PWM | PB6 |
| PWM موتور راست عقب / MR2 PWM | PB7 |
| PWM موتور چپ عقب / ML2 PWM | PB8 |
| PWM موتور چپ جلو / ML1 PWM | PB9 |
| انتخاب ردیف TSOP / TSOP row select | PA8, PB1, PC14, PC15 |
| خواندن ستون TSOP / TSOP column read | PA0 |
| سنسور پایین / Bottom sensor | PA1 |
| سنسور راست / Right sensor | PA2 |
| سنسور چپ / Left sensor | PA3 |
| تشخیص شوتر / Kicker detect | PA4 |
| نوری جلو / Front LDR | PA5 |
| نوری راست / Right LDR | PA6 |
| نوری عقب / Back LDR | PA7 |
| نوری چپ / Left LDR | PB0 |
| فعال‌سازی شوتر / Kicker trigger | PC13 |
| I2C SDA/SCL | PB10/PB11 (Wire2) |
| آدرس I2C OLED / OLED I2C addr | 0x3C |
| آدرس I2C ژیروسکوپ / Gyro I2C addr | 0x10 |

## نمونه‌ها / Examples

- `Original_Code` — نسخه اصلی کد با کتابخانه، کاملاً مشابه عملکرد / Original code using the library, functionally identical
- `Complete_Reference` — مثال کامل تمام APIها / Complete usage example of all APIs

## نکات فنی / Technical Notes

- جبران‌سازی موتور: انحراف زاویه ژیروسکوپ به‌طور خودکار خروجی چهار موتور را اصلاح می‌کند (ضریب 300)
- Motor compensation: gyroscope angle deviation automatically corrects four motor outputs, coefficient 300
- آستانه تشخیص توپ: فاصله TSOP > 2000 = توپ موجود / TSOP distance > 2000 = ball present
- شرط فعال‌سازی شوتر: PA4 پایین + فاصله > 7500 + زاویه در محدوده 90-/+ / Kicker trigger: PA4 low + distance > 7500 + angle within ±90°
