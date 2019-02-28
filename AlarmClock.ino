// This Alarm Clock can only be set to a full hour (based on 24 hour clock), i.e. 7:00, 13:00, 20:00, etc
// Help with RTC from: https://create.arduino.cc/projecthub/Tittiamo/alarm-clock-f61bad
// Sean Malloy

#include <RTClib.h>

RTC_DS1307 RTC;

const int button = 2;
const int buzzer =  9;

uint8_t setalarm = -1;
uint8_t alarm = -1;
uint8_t state = 0;

void setup() {
  pinMode(button, INPUT);
  pinMode(buzzer, OUTPUT);

  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Set the date and time at compile time
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  // RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
    // The default display shows the date and time
}

void loop() {
  DateTime now = RTC.now();
  
  if (digitalRead(button) == HIGH) { // button pushed
    if (state == 0) { // ready to set alarm
      if (setalarm == -1) {
        setalarm = 0; //set alarm for midnight
      }
      else if (alarm == 23) {
        setalarm = 0; //set alarm back to midnight
      }
      else {
        setalarm = setalarm + 1; //increase alarm hour by 1
      }
    }
    else if (state == 1){ // buzzer is on: turn it off, keep alarm set
      state = 2;
    }
    else { // wipe current alarm setting
      state = 0;
      setalarm = -1;
    }
  }

  if (setalarm == now.hour() && now.minute() == 0 && now.second() == 0) { //don't let alarm be set to now
      alarm = -1;
    }
  else {
      alarm = setalarm;
    }
  
  if (now.hour() == alarm  && now.minute() == 0 && now.second() <= 2) { //set off alarm when hour starts
      state = 1;
  }

  if (state == 1) { // play buzzer until shut off
    tone(buzzer,500);
    delay(1000);
    noTone(buzzer);
    delay(1000);
  }
  else {
    noTone(buzzer);
  }
}

