// Alarm_Clock.ino
// Main project file
#include <Wire.h>
#include "DS1307.h"
#include <LiquidCrystal.h>

#define BUZZER_PIN 7
#define BUTTON_PIN 8

DS1307 clock; // clock object
void setup(){
    Serial.begin(9600);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    clock.begin();
    clock.fillByYMD(2015,12,19); // Dec 19, 2015
    clock.fillByHMS(15,28,30); // 15:28 30
    clock.fillDayOfWeek(SAT);
    clock.setTime();
}

void loop(){
  printTime(); // prints time to Serial Monitor for debuggin purposes

  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(clock.hour, DEC);
  lcd.print(":");
  lcd.print(clock.min, DEC);
  
  delay(30000);
}

void displayTime(){
  // outputs the current time to the LCD
  lcd.setCursor(0,1);
  lcd.print(clock.hour, DEC); // returns military time
  lcd.print(":");
  lcd.print(clock.minute, DEC);
}

void printTime(){
  // prints to the serial monitor
  clock.getTime();
  Serial.print(clock.hour, DEC);
  Serial.print(":");
  Serial.print(clock.minute, DEC);
  Serial.print(":");
  Serial.print(clock.second, DEC);
  Serial.print("  ");
  Serial.print(clock.month, DEC);
  Serial.print("/");
  Serial.print(clock.dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(clock.year+2000, DEC);
  Serial.print(" ");
  switch (clock.dayOfWeek)// Friendly printout the weekday
  {
    case MON:
      Serial.print("MON");
      break;
    case TUE:
      Serial.print("TUE");
      break;
    case WED:
      Serial.print("WED");
      break;
    case THU:
      Serial.print("THU");
      break;
    case FRI:
      Serial.print("FRI");
      break;
    case SAT:
      Serial.print("SAT");
      break;
    case SUN:
      Serial.print("SUN");
      break;
  }
  Serial.println(" ");
}


void Alarm(int duration)
{
    unsigned long startTime = millis();
    while (millis()-startTime <= duration) {
       digitalWrite(BUZZER_PIN, HIGH);
       delay(100);
       digitalWrite(BUZZER_PIN, LOW);

       // turn off the alarm if button is pushed
       if (!digitRead(BUTTON_PIN)) {
            digitalWrite(BUZZER_PIN, LOW);
            break;
       }

       delay(100);
    }
}
