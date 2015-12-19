// Alarm_Clock.ino
// Main project file


DS1307 clock; // clock object
void setup(){
    Serial.begin(9600);
    clock.begin();
    clock.fillByYMD(2015,12,19); // Dec 19, 2015
    clock.fillByHMS(15,28,30); // 15:28 30
    clock.fillDayOfWeek(SAT);
    clock.setTime();
}

void loop(){

  
}

