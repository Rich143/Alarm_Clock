// Alarm_Clock.ino
// Main project file
#include <Wire.h>
#include "DS1307.h"
#include <LiquidCrystal.h>


void setup(){


}

void loop(){
  printTime(); // prints time to Serial Monitor for debuggin purposes
  lcd.setCursor(0,1);
  lcd.print(clock.hour, DEC);
  lcd.print(":");
  lcd.
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

