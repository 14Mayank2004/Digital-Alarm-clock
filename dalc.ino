#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonSetTime = 2;   // Button to enter set time mode
const int buttonIncrementHour = 4; // Button to increment hour
const int buttonIncrementMinute = 5; // Button to increment minute
const int buttonConfirm = 6; // Button to confirm the time setting
const int buzzerPin = 8;

bool alarmSet = false;
DateTime alarmTime;

void setup() {
  pinMode(buttonSetTime, INPUT_PULLUP);
  pinMode(buttonIncrementHour, INPUT_PULLUP);
  pinMode(buttonIncrementMinute, INPUT_PULLUP);
  pinMode(buttonConfirm, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Wire.begin();
  rtc.begin();
  lcd.begin();
  lcd.backlight();

  if (!rtc.begin()) {
    lcd.print("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());

  if (alarmSet) {
    lcd.setCursor(0, 1);
    lcd.print("Alarm: ");
    lcd.print(alarmTime.hour());
    lcd.print(':');
    lcd.print(alarmTime.minute());
  }

  if (digitalRead(buttonSetTime) == LOW) {
    setTime();
  }

  // Placeholder for setting alarm
  // if (digitalRead(buttonSetAlarm) == LOW) {
  //   setAlarm();
  // }

  if (alarmSet && now.hour() == alarmTime.hour() && now.minute() == alarmTime.minute()) {
    soundAlarm();
  }

  delay(1000);
}

void setTime() {
  int hour = 0;
  int minute = 0;
  bool settingTime = true;

  lcd.clear();
  lcd.print("Set Time:");

  while (settingTime) {
    lcd.setCursor(0, 1);
    lcd.print(hour < 10 ? "0" : "");
    lcd.print(hour);
    lcd.print(':');
    lcd.print(minute < 10 ? "0" : "");
    lcd.print(minute);

    if (digitalRead(buttonIncrementHour) == LOW) {
      delay(200); // debounce
      hour++;
      if (hour > 23) hour = 0;
    }

    if (digitalRead(buttonIncrementMinute) == LOW) {
      delay(200); // debounce
      minute++;
      if (minute > 59) minute = 0;
    }

    if (digitalRead(buttonConfirm) == LOW) {
      delay(200); // debounce
      DateTime now = rtc.now();
      rtc.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, 0));
      settingTime = false;
    }

    delay(200);
  }

  lcd.clear();
}

void soundAlarm() {
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
}
