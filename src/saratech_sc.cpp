#include "saratech_sc.h"

// شیء سراسری
SaratechSC saratech;

// ===== سازنده =====
SaratechSC::SaratechSC()
    : display(-1),
      i2c(2, I2C_FAST_MODE),
      gyro(&i2c, 0x10),
      max_motor(65000),
      is_goaller(false),
      ldr_sens(300),
      oled_en(true),
      oled_timeout_ms(5000),
      kf_s(0), kr_s(0), kb_s(0), kl_s(0),
      gy(0), dgy(0), v(0), vx(0), value(0), shift(0),
      shb(0), shl(0), shr(0), d(0),
      start_time(0),
      kf(false), kr(false), kb(false), kl(false),
      isball(false), ball_in_kicker(false),
      cheking_out(false), goalkeeper_come_forward(false),
      ball_stop_cnt(0), last_ball_angle(0) {
}

// ===== راه‌اندازی (معمق setup اصلی) =====
void SaratechSC::begin() {
    pinMode(PB12, OUTPUT);
    pinMode(PB13, OUTPUT);
    pinMode(PB14, OUTPUT);
    pinMode(PB15, OUTPUT);
    pinMode(PB6, PWM);
    pinMode(PB7, PWM);
    pinMode(PB8, PWM);
    pinMode(PB9, PWM);
    pinMode(PA8, OUTPUT);
    pinMode(PB1, OUTPUT);
    pinMode(PC14, OUTPUT);
    pinMode(PC15, OUTPUT);
    pinMode(PC13, OUTPUT);
    delay(250);

    display.begin(0x2, 0x3c);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("loading00");
    display.display();

    i2c.begin();

    // کالیبراسیون سنسورهای نوری
    int kf_s_sum = 0;
    int kr_s_sum = 0;
    int kb_s_sum = 0;
    int kl_s_sum = 0;
    for (int i = 0; i < 10; i++) {
        kf_s_sum += analogRead(PA5);
        kr_s_sum += analogRead(PA6);
        kb_s_sum += analogRead(PA7);
        kl_s_sum += analogRead(PB0);
    }
    kf_s = kf_s_sum / 10;
    kr_s = kr_s_sum / 10;
    kb_s = kb_s_sum / 10;
    kl_s = kl_s_sum / 10;

    start_time = millis();
}

// ===== توابع کمکی =====
float SaratechSC::clampFloat(float val, float min_val, float max_val) {
    if (val > max_val) return max_val;
    if (val < min_val) return min_val;
    return val;
}

int SaratechSC::posInt(int val) {
    if (val < 0) return 0;
    else return val;
}

bool SaratechSC::boolCompare(float val, float shart) {
    if (val > shart) return true;
    else return false;
}

float SaratechSC::convertAngle(float angle) {
    if (angle > 180) return angle - 360;
    if (angle < -180) return angle + 360;
    return angle;
}

int SaratechSC::calcSpeed(float a) {
    if (a > -15 && a < 15) return 60000;
    if (a > 15 && a < 100) return 40000;
    if (a < -15 && a > -100) return 34000;
    return 50000;
}

float SaratechSC::calcShift(float a) {
    if (shb > 900) return 0;
    if (a >  0 && a < 15) return 5;
    if (a > -15 && a < 0) return -7;
    if (a > 15 && a < 90) return 60;
    if (a < -15 && a > -90) return -60;
    if (a > 0) return 80;
    return -80;
}

// ===== کنترل موتور =====
void SaratechSC::motor(int ml1, int ml2, int mr2, int mr1) {
    float correction = clampFloat(gy - dgy, -90, 90);

    ml1 += (correction * 300);
    ml2 += (correction * 300);
    mr2 += (correction * 300);
    mr1 += (correction * 300);
    int maxVal = abs(ml1);

    if (abs(ml2) > maxVal) maxVal = abs(ml2);
    if (abs(mr2) > maxVal) maxVal = abs(mr2);
    if (abs(mr1) > maxVal) maxVal = abs(mr1);
    if (maxVal > max_motor) {
        float scale = (float)max_motor / maxVal;
        ml1 *= scale;
        ml2 *= scale;
        mr2 *= scale;
        mr1 *= scale;
    }
    if (ml1 > 65535) ml1 = 65535;
    if (ml2 > 65535) ml2 = 65535;
    if (mr2 > 65535) mr2 = 65535;
    if (mr1 > 65535) mr1 = 65535;

    if (ml1 < -65535) ml1 = -65535;
    if (ml2 < -65535) ml2 = -65535;
    if (mr2 < -65535) mr2 = -65535;
    if (mr1 < -65535) mr1 = -65535;

    // MR1
    if (mr1 > 0) {
        digitalWrite(PB12, 0);
        pwmWrite(PB6, mr1);
    } else {
        digitalWrite(PB12, 1);
        pwmWrite(PB6, mr1 + 65535);
    }
    // MR2
    if (mr2 > 0) {
        digitalWrite(PB13, 0);
        pwmWrite(PB7, mr2);
    } else {
        digitalWrite(PB13, 1);
        pwmWrite(PB7, mr2 + 65535);
    }
    // ML1
    if (ml1 > 0) {
        digitalWrite(PB15, 0);
        pwmWrite(PB9, ml1);
    } else {
        digitalWrite(PB15, 1);
        pwmWrite(PB9, ml1 + 65535);
    }
    // ML2
    if (ml2 > 0) {
        digitalWrite(PB14, 0);
        pwmWrite(PB8, ml2);
    } else {
        digitalWrite(PB14, 1);
        pwmWrite(PB8, ml2 + 65535);
    }
}

void SaratechSC::move(float angle) {
    vx = sin(radians(angle)) * v;
    float vy = cos(radians(angle)) * v;
    movexy(vx, vy);
}

void SaratechSC::moveSpeed(float angle, float s) {
    vx = sin(radians(angle)) * s;
    float vy = cos(radians(angle)) * s;
    movexy(vx, vy);
}

void SaratechSC::movexy(float vx, float vy) {
    motor(vy + vx, vy - vx, -vy - vx, -vy + vx);
}

void SaratechSC::movesecond(int m, int n) {
    sensor();
    for (int i = 0; i < n; i++) {
        moveSpeed(m, 50000);
        sensor();
        printall();
    }
}

void SaratechSC::stop() {
    motor(0, 0, 0, 0);
}

// ===== خواندن سنسور =====
void SaratechSC::sensor() {
    kf = boolCompare(posInt(analogRead(PA5) - kf_s), ldr_sens);
    kr = boolCompare(posInt(analogRead(PA6) - kr_s), ldr_sens);
    kb = boolCompare(posInt(analogRead(PA7) - kb_s), ldr_sens);
    kl = boolCompare(posInt(analogRead(PB0) - kl_s), ldr_sens);

    shb = analogRead(PA1);
    shr = analogRead(PA2);
    shl = analogRead(PA3);
    d = (shl - shr) * 30;

    // محاسبه زاویه TSOP
    float sumx = 0, sumy = 0;
    for (int i = 0; i < 16; i++) {
        digitalWrite(PA8, (i / 1) % 2);
        digitalWrite(PB1, (i / 2) % 2);
        digitalWrite(PC14, (i / 4) % 2);
        digitalWrite(PC15, (i / 8) % 2);
        sensorangle = radians(i * 22.5);
        value = 4095 - analogRead(PA0);
        sumx += value * cos(sensorangle);
        sumy += value * sin(sensorangle);
    }
    ballangle = degrees(atan2(sumy, sumx));
    balldistance = sqrt(pow(sumx, 2) + pow(sumy, 2));

    if (balldistance > 2000) isball = true;
    else isball = false;

    if (!digitalRead(PA4) && balldistance > 7500 && (-90 < ballangle && ballangle < 90))
        ball_in_kicker = true;
    else
        ball_in_kicker = false;

    // جبران‌سازی زاویه
    if (is_goaller && !goalkeeper_come_forward)
        dgy = 0;
    else {
        if (shr > 900 && isball) dgy = 21.80;
        else if (shl > 1100 && isball) dgy = -21.00;
        else if (!isball || abs(d) < 6000) dgy = 0;
    }

    gy = gyro.read();

    if (millis() - start_time > oled_timeout_ms) {
        oled_en = false;
        display.clearDisplay();
        display.display();
    }

    if (isball && abs(last_ball_angle - ballangle) < 40)
        ball_stop_cnt++;
    else
        ball_stop_cnt = 0;
    last_ball_angle = ballangle;
}

// ===== نمایش OLED =====
void SaratechSC::printall() {
    if (!oled_en) return;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("BA:");
    display.println(ballangle);
    display.print("BD:");
    display.println(balldistance);
    display.print("GY:");
    display.println(gy);
    display.print("SHR:");
    display.println(shr);
    display.print("SHL:");
    display.println(shl);
    display.print("SHB:");
    display.println(shb);
    display.print("d :");
    display.println(d);

    if (kf) display.fillRoundRect(74, 0, 40, 3, 0, WHITE);
    else display.drawRoundRect(74, 0, 40, 3, 0, WHITE);
    if (kr) display.fillRoundRect(123, 12, 3, 40, 0, WHITE);
    else display.drawRoundRect(123, 12, 3, 40, 0, WHITE);
    if (kb) display.fillRoundRect(74, 61, 40, 3, 0, WHITE);
    else display.drawRoundRect(74, 61, 40, 3, 0, WHITE);
    if (kl) display.fillRoundRect(62, 12, 3, 40, 0, WHITE);
    else display.drawRoundRect(62, 12, 3, 40, 0, WHITE);

    display.drawCircle(94, 32, 20, WHITE);
    display.fillTriangle(
        94 + 15 * sin(radians(gy)), 32 - 15 * cos(radians(gy)),
        94 + 22.5 * sin(radians(gy + 21)), 32 - 22.5 * cos(radians(gy + 21)),
        94 + 22.5 * sin(radians(gy - 21)), 32 - 22.5 * cos(radians(gy - 21)), BLACK);
    display.drawLine(
        94 + 20 * sin(radians(gy + 20)), 32 - 20 * cos(radians(gy + 20)),
        94 + 12 * sin(radians(gy + 30)), 32 - 12 * cos(radians(gy + 30)), WHITE);
    display.drawLine(
        94 + 20 * sin(radians(gy - 20)), 32 - 20 * cos(radians(gy - 20)),
        94 + 12 * sin(radians(gy - 30)), 32 - 12 * cos(radians(gy - 30)), WHITE);
    display.drawLine(
        94 + 12 * sin(radians(gy + 30)), 32 - 12 * cos(radians(gy + 30)),
        94 + 12 * sin(radians(gy - 30)), 32 - 12 * cos(radians(gy - 30)), WHITE);

    if (ball_in_kicker)
        display.fillCircle(93 + sin(radians(gy)) * 17, 32 - cos(radians(gy)) * 17, 2, WHITE);
    else if (isball)
        display.fillCircle(93 + sin(radians(ballangle)) * 25, 32 - cos(radians(ballangle)) * 25, 2, WHITE);

    display.display();
}

// ===== تشخیص خروج از زمین =====
void SaratechSC::out() {
    float out_angle;
    int out_cnt = 0;
    cheking_out = true;
    if (kf && kr) {
        movesecond(225, 10);
        out_angle = 45;
    } else if (kf && kl) {
        movesecond(135, 10);
        out_angle = -45;
    } else if (kf) {
        if (shr < shl) movesecond(140, 17);
        else movesecond(220, 17);
        out_angle = 0;
    } else if (kb) {
        movesecond(0, 15);
        out_angle = 180;
    } else if (kr) {
        movesecond(270, 10);
        out_angle = 90;
    } else if (kl) {
        movesecond(90, 10);
        out_angle = -90;
    } else if (kb && kr) {
        movesecond(315, 10);
        out_angle = 135;
    } else if (kb && kl) {
        movesecond(45, 10);
        out_angle = -135;
    } else {
        cheking_out = false;
        return;
    }
    while (isball && abs(out_angle - convertAngle(ballangle)) < 30 && out_cnt < 50) {
        out_cnt++;
        sensor();
        printall();
        if (move_inside()) stop();
    }
    cheking_out = false;
}

bool SaratechSC::move_inside() {
    if (kf && kr) moveSpeed(225, 50000);
    else if (kf && kl) moveSpeed(135, 50000);
    else if (kb && kr) moveSpeed(315, 50000);
    else if (kb && kl) moveSpeed(45, 50000);
    else if (kf) moveSpeed(180, 50000);
    else if (kb) moveSpeed(0, 50000);
    else if (kr) moveSpeed(270, 50000);
    else if (kl) moveSpeed(90, 50000);
    else return true;
    return false;
}

// ===== شوت زدن =====
void SaratechSC::shoot() {
    digitalWrite(PC13, 1);
    for (int i = 0; i < 4; i++) {
        sensor();
        printall();
        out();
    }
    digitalWrite(PC13, 0);
}

// ===== حالت حمله =====
void SaratechSC::forward() {
    if (ball_in_kicker) {
        v = 60000;
        shoot();
    } else if (isball) {
        out();
        shift = calcShift(ballangle);
        v = calcSpeed(ballangle);
        move(ballangle + shift);
    } else {
        movexy(d, (shb - 950) * 60);
    }
}

// ===== حالت دروازه‌بان =====
void SaratechSC::goaller() {
    int bd = (shb - 1500) * 60;
    float a = ballangle;
    if (a > 180) a -= 360;
    a *= 3000;
    if (shr > 1200 && a > 0) a = 0;
    if (shl > 1200 && a < 0) a = 0;
    if (!isball) goalkeeper_come_forward = false;
    if (ball_in_kicker && isball) {
        moveSpeed(0, 65000);
        shoot();
        goalkeeper_come_forward = false;
    } else if (ball_stop_cnt > 140 || goalkeeper_come_forward) {
        forward();
        goalkeeper_come_forward = true;
    } else if (isball) {
        movexy(constrain(a, -50000, 50000), bd);
    } else {
        movexy(d, bd);
    }
}

// ===== متدهای Getter =====
bool SaratechSC::get_isball() const { return isball; }
bool SaratechSC::get_ball_in_kicker() const { return ball_in_kicker; }
float SaratechSC::get_ballangle() const { return ballangle; }
float SaratechSC::get_balldistance() const { return balldistance; }
float SaratechSC::get_gy() const { return gy; }
int SaratechSC::get_shb() const { return shb; }
int SaratechSC::get_shl() const { return shl; }
int SaratechSC::get_shr() const { return shr; }
int SaratechSC::get_d() const { return d; }
bool SaratechSC::get_kf() const { return kf; }
bool SaratechSC::get_kr() const { return kr; }
bool SaratechSC::get_kb() const { return kb; }
bool SaratechSC::get_kl() const { return kl; }
bool SaratechSC::get_cheking_out() const { return cheking_out; }
int SaratechSC::get_ball_stop_cnt() const { return ball_stop_cnt; }

// ===== متدهای Setter =====
void SaratechSC::setMaxMotor(int val) { max_motor = val; }
void SaratechSC::setIsGoaller(bool val) { is_goaller = val; }
void SaratechSC::setLdrSens(int val) { ldr_sens = val; }
void SaratechSC::setOledEn(bool val) { oled_en = val; }
void SaratechSC::setOledTimeout(int ms) { oled_timeout_ms = ms; }
void SaratechSC::setKfS(int val) { kf_s = val; }
void SaratechSC::setKrS(int val) { kr_s = val; }
void SaratechSC::setKbS(int val) { kb_s = val; }
void SaratechSC::setKlS(int val) { kl_s = val; }
