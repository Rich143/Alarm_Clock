# Alarm_Clock
Alarm Clock Arduino Project

Uses a LCD display and a real time clock to create an alarm clock capable of displaying the current time, setting an alarm, and sounding alarms at the preset time.

The arduino interfaces with the real time clock over I2C. It first sets the clock to the correct time, then repeatedly reads the clock to get the current time. 

The LCD is used to display the current time, as well as to set the alarm time.
