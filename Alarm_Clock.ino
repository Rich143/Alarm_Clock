// Alarm_Clock.ino
// Main project file
#include <Wire.h>
#include "DS1307.h"
#include <LiquidCrystal.h>

#define BUZZER_PIN 6
#define BUTTON_PIN 8

#define ALARM_DURATION 5

// define the machine states
#define BEEPING 1
#define NOT_BEEPING 0
#define SETTING_ALARM 2

#define BUZZ_PERIOD 100 // ms

bool alarmMode = true;
int alarmHour = 15;
int alarmMinute = 28;

DS1307 clock; // clock object
LiquidCrystal lcd(2, 3, 9, 10, 11, 12);

int state;

void setup(){
  
    state = NOT_BEEPING; // initially not beeping
    
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
  displayTime();// prints regardless of switch cases..
  printTime(); // prints time to Serial Monitor for debuggin purposes
  
  switch (state){
    case NOT_BEEPING: // if its not in alarm mode, just regular
      if (setAlarmButton){ // if pressed than go to that state
        state = SETTING_ALARM;
        break; ..
      }
      if (alarmMode){
        if (clock.hour == alarmHour && clock.minute == alarmMinute){
          secondStarted = clock.second;
          state = BEEPING;
        }
      }
      break;
    case BEEPING: // if alarm mode is enabled
      Alarm();
      if (clock.second > secondStarted + ALARM_DURATION || buttonPressed()){
        Alarm_Off();
        state = NOT_BEEPING;
        alarmMode = false;      
        break;
      }  
     case SETTING_ALARM:
      // test to make sure the set alarm button is still being held
        if (!alarmSetButton){
          state = NOT_BEEPING;
          break;
        }
        if (alarmHourSetButton){
          alarmHour ++;
        }
        if (alarmMinuteSetButton){
          alarmMinute ++;
        }
        displayAlarm();
        break;
    }
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

/*
void Alarm(unsigned long startTime, int duration)
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
*/

void Alarm() 
{
    static uint8_t buzzState = 1;
    static unsigned long lastBuzz = 0;
    
    if (millis() - lastBuzz >= BUZZ_PERIOD) {
        digitalWrite(BUZZER_PIN, buzzState);
        buzzState ^= 1; // toggle buzzer state
    }
}

void Alarm_Off() 
{
    digitalWrite(BUZZER_PIN, LOW);
}

bool buttonPressed() 
{
    if (!digitalRead(BUTTON_PIN)) {
        return true;
    } else {
        return false;
    }
}
