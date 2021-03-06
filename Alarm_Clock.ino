// Alarm_Clock.ino
// Main project file
#include <Wire.h>
#include "DS1307.h"
#include <LiquidCrystal.h>

#define BUZZER_PIN 6
#define ALARM_OFF_BUTTON_PIN 8
#define ALARM_SET_BUTTON_PIN A0
#define ALARM_INC_HOUR_PIN A1
#define ALARM_INC_MIN_PIN A2

#define ALARM_DURATION 5 // in seconds

// define the machine states
#define BEEPING 1
#define NOT_BEEPING 0
#define SETTING_ALARM 2

#define BUZZ_PERIOD 100 // ms

bool alarmMode = true;
int alarmHour = 15;
int alarmMinute = 28;

int secondStarted;

DS1307 clock; // clock object
LiquidCrystal lcd(2, 3, 9, 10, 11, 12);

int state; // state of the machine

boolean minuteButtonPressed = false;
boolean hourButtonPressed = false;

void setup(){
  
    state = NOT_BEEPING; // initially not beeping
    
    Serial.begin(9600);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(ALARM_OFF_BUTTON_PIN, INPUT_PULLUP);
    pinMode(ALARM_SET_BUTTON_PIN, INPUT_PULLUP);
    pinMode(ALARM_INC_HOUR_PIN, INPUT_PULLUP);
    pinMode(ALARM_INC_MIN_PIN, INPUT_PULLUP);


    clock.begin();
    clock.fillByYMD(2015,12,19); // Dec 19, 2015
    clock.fillByHMS(15,27,50); // 15:28 30
    clock.fillDayOfWeek(SAT);
    clock.setTime();
}

void loop(){

  displayTime();// prints regardless of switch cases..
  printTime(); // prints time to Serial Monitor for debuggin purposes
  
  switch (state){
    case NOT_BEEPING: // if its not in alarm mode, just regular
      if (buttonPressed(ALARM_SET_BUTTON_PIN)){ // if pressed than go to that state
        state = SETTING_ALARM;
        break; 
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
      if (clock.second > secondStarted + ALARM_DURATION || buttonPressed(ALARM_OFF_BUTTON_PIN)){
        Alarm_Off();
        state = NOT_BEEPING;
        alarmMode = false;      
        break;
      }  
     case SETTING_ALARM:
      // test to make sure the set alarm button is still being held
        Serial.print("Setting Alarm Mode");
        if (!buttonPressed(ALARM_SET_BUTTON_PIN)){
          state = NOT_BEEPING;
          break;
        }
        if (buttonPressed(ALARM_INC_HOUR_PIN)){
          Serial.print("Increment Hour");
          alarmHour ++;
        }
        if (buttonPressed(ALARM_INC_MIN_PIN)){
          Serial.print("Increment Second");
          alarmMinute ++;
        }
        displayAlarm();
        break;
    }
}

void displayAlarm(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(alarmHour, DEC);
  lcd.print(":");
  lcd.print(alarmMinute < 10 ? "0":"");
  lcd.print(alarmMinute, DEC);
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
       if (!digitalRead(ALARM_OFF_BUTTON_PIN)) {
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

bool buttonPressed(int buttonPin) 
{
    if (!digitalRead(buttonPin)) {
        return true;
    } else {
        return false;
    }
}

bool buttonPressedDebounce(int buttonPin) 
{
    if (buttonPressed(buttonPin)) {
        delay(100); // delay to debounce
        if (buttonPressed(buttonPin)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool incrementCount(int buttonPin)
{
    if (hourButtonPressed && buttonPin == ALARM_INC_HOUR_PIN) {
        if (!buttonPressed(buttonPin)) {
            hourButtonPressed = false;
            return true;
        } else {
            return false;
        }
    }

    if (minuteButtonPressed && buttonPin == ALARM_INC_MIN_PIN) {
        if (!buttonPressed(buttonPin)) {
            minuteButtonPressed = false;
            return true;
        } else {
            return false;
        }
    }
    
    if (buttonPressedDebounce(buttonPin)) {
        if (buttonPin == ALARM_INC_HOUR_PIN) {
            hourButtonPressed = true;
            return false;
        }
        if (buttonPin == ALARM_INC_MIN_PIN) {
            minuteButtonPressed = true;
            return false;
        }
    }

    return false;
}
