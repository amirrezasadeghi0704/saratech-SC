#ifndef SARATECH_SC_H
#define SARATECH_SC_H

#include <TDAxis12.h>
#include <Wire.h>
#include <Adafruit_SH1106_STM32.h>

class SaratechSC {
public:
    // ===== نمونه‌های سخت‌افزاری =====
    Adafruit_SH1106 display;
    TwoWire i2c;
    TDAxis12 gyro;

    // ===== پارامترهای قابل تنظیم (مقادیر پیش‌فرض همان کد اصلی) =====
    // حداکثر PWM موتور
    int max_motor;
    // حالت دروازه‌بان
    bool is_goaller;
    // آستانه حساسیت سنسور نوری
    int ldr_sens;
    // نمایش OLED (بعد از این زمان میلی‌ثانیه غیرفعال می‌شود)
    bool oled_en;
    int oled_timeout_ms;

    // ===== شیفت‌های کالیبراسیون موتور =====
    int kf_s;
    int kr_s;
    int kb_s;
    int kl_s;

    // ===== سازنده =====
    SaratechSC();

    // ===== راه‌اندازی (معمق setup اصلی) =====
    void begin();

    // ===== خواندن داده‌های سنسور =====
    void sensor();

    // ===== نمایش OLED =====
    void printall();

    // ===== کنترل موتور =====
    void motor(int ml1, int ml2, int mr2, int mr1);

    // ===== حرکت با زاویه و سرعت =====
    void move(float angle);
    void moveSpeed(float angle, float s);
    void movexy(float vx, float vy);

    // ===== حرکت با زاویه به مدت n ثانیه =====
    void movesecond(int m, int n);

    // ===== توقف موتور =====
    void stop();

    // ===== تشخیص خروج از زمین =====
    void out();
    bool move_inside();

    // ===== شوت زدن =====
    void shoot();

    // ===== حالت حمله =====
    void forward();

    // ===== حالت دروازه‌بان =====
    void goaller();

    // ===== دریافت وضعیت =====
    bool get_isball() const;
    bool get_ball_in_kicker() const;
    float get_ballangle() const;
    float get_balldistance() const;
    float get_gy() const;
    int get_shb() const;
    int get_shl() const;
    int get_shr() const;
    int get_d() const;
    bool get_kf() const;
    bool get_kr() const;
    bool get_kb() const;
    bool get_kl() const;
    bool get_cheking_out() const;
    int get_ball_stop_cnt() const;

    // ===== تنظیم پارامترها =====
    void setMaxMotor(int val);
    void setIsGoaller(bool val);
    void setLdrSens(int val);
    void setOledEn(bool val);
    void setOledTimeout(int ms);
    void setKfS(int val);
    void setKrS(int val);
    void setKbS(int val);
    void setKlS(int val);

private:
    // ===== متغیرهای داخلی =====
    float ballangle;
    float balldistance;
    float sensorangle;
    float gy;
    float dgy;
    float v;
    float vx;
    float value;
    float shift;
    int shb;
    int shl;
    int shr;
    int d;
    int start_time;
    bool kf;
    bool kr;
    bool kb;
    bool kl;
    bool isball;
    bool ball_in_kicker;
    bool cheking_out;
    bool goalkeeper_come_forward;
    int ball_stop_cnt;
    int last_ball_angle;

    // ===== توابع کمکی داخلی =====
    float clampFloat(float val, float min_val, float max_val);
    int posInt(int val);
    bool boolCompare(float val, float shart);
    float convertAngle(float angle);
    int calcSpeed(float a);
    float calcShift(float a);
};

// شیء سراسری
extern SaratechSC saratech;

#endif
