// =============================================
// مرجع کامل کتابخانه saratech_sc
// Complete saratech_sc Library Reference
// =============================================

#include <saratech_sc.h>

void setup() {
    // ========== 1. راه‌اندازی ==========
    // Initialization
    saratech.begin();

    // ========== 2. تنظیم پارامترهای قابل تنظیم ==========
    // Adjustable Parameter Settings

    // تنظیم حداکثر PWM موتور، پیش‌فرض 65000
    // Set maximum motor PWM, default 65000
    saratech.setMaxMotor(65000);

    // تغییر حالت دروازه‌بان، پیش‌ف� false (حالت حمله)
    // Toggle goalkeeper mode, default false (forward mode)
    saratech.setIsGoaller(false);

    // تنظیم آستانه حساسیت سنسور نوری، پیش‌فرض 300
    // Set photodiode sensitivity threshold, default 300
    saratech.setLdrSens(300);

    // تنظیم زمان خودکار خاموشی OLED (میلی‌ثانیه)، پیش‌فرض 5000ms
    // Set OLED auto-off timeout (ms), default 5000ms
    saratech.setOledTimeout(5000);

    // فعال/غیرفعال‌سازی دستی نمایش OLED
    // Manually enable/disable OLED display
    saratech.setOledEn(true);

    // کالیبراسیون دستی چهار جهت سنسور نوری
    // Manually calibrate four-direction photodiode baselines
    saratech.setKfS(0);   // جلو Forward
    saratech.setKrS(0);   // راست Right
    saratech.setKbS(0);   // عقب Back
    saratech.setKlS(0);   // چپ Left

    // ========== 3. کنترل حرکت پایه ==========
    // Basic Movement Control

    // حرکت با زاویه و سرعت v (0-65000)
    // Move by angle with speed v (0-65000)
    saratech.move(90.0);

    // حرکت با زاویه و سرعت مشخص
    // Move by angle with specified speed
    saratech.moveSpeed(45.0, 50000);

    // حرکت با سرعت x/y (محدوده -65535 تا 65535)
    // Move by x/y velocity (range -65535 to 65535)
    saratech.movexy(30000, 40000);

    // حرکت با زاویه به مدت n ثانیه
    // Move by angle for n seconds
    saratech.movesecond(90, 10);

    // توقف همه موتورها
    // Stop all motors
    saratech.stop();

    // ========== 4. خواندن سنسورها ==========
    // Sensor Reading

    // خواندن تمام داده‌های سنسور (نوری، ژیروسکوپ، TSOP و غیره)
    // Read all sensor data (photodiode, gyroscope, TSOP, etc.)
    saratech.sensor();

    // دریافت زاویه توپ (درجه -180 تا 180)
    // Get ball angle (degrees -180 to 180)
    float angle = saratech.get_ballangle();

    // دریافت فاصله توپ
    // Get ball distance
    float distance = saratech.get_balldistance();

    // دریافت زاویه ژیروسکوپ
    // Get gyroscope angle
    float gyroVal = saratech.get_gy();

    // دریافت مقدار سنسور پایین
    // Get bottom sensor value
    int shb = saratech.get_shb();

    // دریافت مقادیر سنسور چپ/راست
    // Get left/right sensor values
    int shl = saratech.get_shl();
    int shr = saratech.get_shr();

    // دریافت اختلاف جهت جلو/عقب
    // Get front-back direction difference
    int d = saratech.get_d();

    // دریافت وضعیت چهار جهت نوری
    // Get four-direction photodiode status
    bool kf = saratech.get_kf();
    bool kr = saratech.get_kr();
    bool kb = saratech.get_kb();
    bool kl = saratech.get_kl();

    // دریافت وضعیت تشخیص خروج از زمین
    // Get out-of-bounds checking status
    bool checking = saratech.get_cheking_out();

    // دریافت شمارنده توقف توپ
    // Get ball stop counter
    int stopCnt = saratech.get_ball_stop_cnt();

    // ========== 5. دریافت وضعیت ==========
    // Status Queries

    // آیا توپ تشخیص داده شده؟
    // Is ball detected?
    bool isBall = saratech.get_isball();

    // آیا توپ در مکانیزم شوت قرار دارد؟
    // Is ball in kicker?
    bool inKicker = saratech.get_ball_in_kicker();

    // ========== 6. تشخیص خروج از زمین ==========
    // Out of Bounds Detection

    // تشخیص خروج و اصلاح موقعیت
    // Detect out-of-bounds and correct position
    saratech.out();

    // بررسی داخلی: آیا در زمین هستیم؟
    // Internal check: inside bounds?
    bool inside = saratech.move_inside();

    // ========== 7. شوت زدن ==========
    // Shooting

    // فعال‌سازی مکانیزم شوت (راه‌اندازی شوتر و حلقه تشخیص خروج)
    // Activate shooter and out-detection loop
    saratech.shoot();

    // ========== 8. حالت‌های خودکار ==========
    // Autonomous Modes

    // حالت حمله: تعقیب توپ، تشخیص خروج، شوت
    // Forward mode: chase ball, out detection, shoot
    saratech.forward();

    // حالت دروازه‌بان: دفاع و شوت هنگام ورود توپ
    // Goalkeeper mode: defend and shoot on ball entry
    saratech.goaller();

    // ========== 9. نمایش OLED ==========
    // OLED Display

    // بروزرسانی نمایشگر با داده‌های سنسور و وضعیت
    // Refresh display with sensor data and status
    saratech.printall();
}

void loop() {
    // حلقه اصلی: خواندن سنسور -> نمایش OLED -> منطق خودکار
    // Main loop: read sensor -> OLED display -> autonomous logic
    saratech.sensor();
    saratech.printall();

    if (saratech.is_goaller)
        saratech.goaller();
    else
        saratech.forward();
}
