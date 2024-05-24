#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonSetTime = 2;   // Button to set time
const int buttonSetAlarm = 3;  // Button to set alarm
const int buzzerPin = 8;

bool alarmSet = false;
DateTime alarmTime;

void setup() {
  pinMode(buttonSetTime, INPUT_PULLUP);
  pinMode(buttonSetAlarm, INPUT_PULLUP);
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

  if (digitalRead(buttonSetAlarm) == LOW) {
    setAlarm();
  }

  if (alarmSet && now.hour() == alarmTime.hour() && now.minute() == alarmTime.minute()) {
    soundAlarm();
  }

  delay(1000);
}

void setTime() {
  // Add code to set the time using buttons
}

void setAlarm() {
  // Add code to set the alarm using buttons
}

void soundAlarm() {
  digitalWrite(buzzerPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
}
