// Alarm_Clock.ino
// Main project file
#include <Wire.h>
#include "DS1307.h"
#include <LiquidCrystal.h>

#define BUZZER_PIN 6
#define BUTTON_PIN 8
#define ALARM_DURATION 5
#define BEEPING 1
#define NOT_BEEPING 0

bool alarmMode = true;
int alarmHour = 15;
int alarmMinute = 28;

DS1307 clock; // clock object
LiquidCrystal lcd(2, 3, 9, 10, 11, 12);

int state = 0;
void setup(){

  
    Serial.begin(9600);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    clock.begin();
    clock.fillByYMD(2015,12,19); // Dec 19, 2015
    clock.fillByHMS(15,27,50); // 15:28 30
    clock.fillDayOfWeek(SAT);
    clock.setTime();
}

void loop(){

  static int secondStarted;
  
  printTime(); // prints time to Serial Monitor for debuggin purposes
  switch (state){
    case NOT_BEEPING: // if its not in alarm mode, just regular
      displayTime();
      if (clock.hour == alarmHour && clock.minute == alarmMinute){
        secondStarted = clock.second;
        state = BEEPING;
      }
      break;
    case BEEPING: // if alarm mode is enabled
    // rememeber the alarm goes for 5 seconds
      Alarm();
      if (clock.second > secondStarted + ALARM_DURATION){
        Alarm_Off();
        state = NOT_BEEPING;
        }
      break;
  }  
  delay(500);
}

void displayTime(){
  // outputs the current time to the LCD
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(clock.hour, DEC);
  lcd.print(":");
  lcd.print(clock.minute < 10 ? "0":"");
  lcd.print(clock.minute, DEC);
  lcd.print(":");
  lcd.print(clock.second < 10 ? "0":"");
  lcd.print(clock.second, DEC);
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
       if (!digitalRead(BUTTON_PIN)) {
            digitalWrite(BUZZER_PIN, LOW);
            break;
       }

       delay(100);
    }
}
