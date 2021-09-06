//------------------------------------------------------------------------
//    Imports and defines
//------------------------------------------------------------------------
//Imports
#include "DHT.h"

//Defines
#define DHTPIN 4          //Air Temp/Humidity Sensor port
#define DHTTYPE DHT11     //Air Temp/Humidity Sensor type
#define COUNTER_1SEC 1000
#define COUNTER_10MIN 10000 //10min
#define lmillis() ((long)millis())


//------------------------------------------------------------------------
//    Variables and consts
//------------------------------------------------------------------------
//HW Const
const int humiditySensor0 = A0;
const int pump0 = 2;
const int waterButton = 3;
const int statusLed = 13;

//Sensor values
int rawSoilHumidity0 = 0;
float rawAirHumidity0 = 0;
float rawAirTemp0 = 0;
long watering0ElapsedTime = 0; //seconds
long watering0ElapsedTimeInterval = 0; //seconds

//Variables
int waterButtonState = LOW;
int lastWaterButtonState = LOW;
int pumpState = HIGH;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
long lastTimeCheckedStatusLed;
long lastTimeCheckedClock;
long lastTimeCheckedLogger;

int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 0;

DHT dht(DHTPIN, DHTTYPE);

//------------------------------------------------------------------------
//    Setup
//------------------------------------------------------------------------
void setup () {

  lastTimeCheckedStatusLed = lmillis() + COUNTER_1SEC;
  lastTimeCheckedClock = lmillis() + COUNTER_1SEC;
  lastTimeCheckedLogger = lmillis() + COUNTER_10MIN;

  //Set initial I/O state
  pinMode(pump0, OUTPUT);
  pinMode(statusLed, OUTPUT);
  pinMode(waterButton, INPUT);

  //Serial port
  Serial.begin(9600);

  //Set initial HW state
  digitalWrite(pump0, HIGH);
  digitalWrite(statusLed, LOW);
  dht.begin();

}



//------------------------------------------------------------------------
//    Loop
//------------------------------------------------------------------------

void loop() {

  CheckWaterButtonPressed();
  Timer_1Sec();
  Timer_10Min();
  UpdateClock();
}



//------------------------------------------------------------------------
//  Private methods
//------------------------------------------------------------------------
void CheckWaterButtonPressed() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(waterButton);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:
  // If the switch changed, due to noise or pressing:

  if (reading != lastWaterButtonState) {
    // reset the debouncing timer
    lastDebounceTime = lmillis();
  }

  // whatever the reading is at, it's been there for longer than the debounce
  // delay, so take it as the actual current state:
  if ((millis() - lastDebounceTime) > debounceDelay) {

    // if the button state has changed:
    if (reading != waterButtonState) {
      waterButtonState = reading;

      pumpState = !pumpState;

      //If button pressed start timer
      if (waterButtonState == HIGH) {
        watering0ElapsedTimeInterval = lmillis();
      }
      else {
        watering0ElapsedTimeInterval = lmillis() - watering0ElapsedTimeInterval;
        watering0ElapsedTime += watering0ElapsedTimeInterval;
      }
    }
  }


  // set the pump:
  digitalWrite(pump0, pumpState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastWaterButtonState = reading;
}


void UpdateClock() {
  if (lmillis() - lastTimeCheckedClock >= 0) {

    lastTimeCheckedClock = lmillis() + COUNTER_1SEC;
    seconds++;

    if (seconds == 60) {
      seconds = 0;
      minutes++;
    }
    if (minutes == 60) {
      minutes = 0;
      hours++;
    }
    if (hours == 24) {
      hours = 0;
      days++;
    }
  }
}


void Timer_1Sec () {
  if (lmillis() - lastTimeCheckedStatusLed >= 0) {
    lastTimeCheckedStatusLed = lmillis() + COUNTER_1SEC;
    digitalWrite(statusLed, !digitalRead(statusLed));
  }
}


void Timer_10Min() {
  if (lmillis() - lastTimeCheckedLogger >= 0) {
    lastTimeCheckedLogger = lmillis() + COUNTER_10MIN;

    //Collect sensor information
    rawSoilHumidity0 = analogRead(humiditySensor0);
    rawAirHumidity0 = dht.readHumidity();
    rawAirTemp0 = dht.readTemperature();

    if (waterButtonState == LOW) {
      LogData();
    }
  }
}


void LogData() {
  Serial.print(String(days) + "D");
  Serial.print(String(hours) + ":");
  Serial.print(String(minutes) + ":");
  Serial.print(String(seconds));
  Serial.print(",");
  Serial.print(String(rawSoilHumidity0));
  Serial.print(",");
  Serial.print(String(rawAirHumidity0));
  Serial.print(",");
  Serial.print(String(rawAirTemp0));
  Serial.print(",");
  Serial.println(String(watering0ElapsedTime / 1000));
  watering0ElapsedTime = 0;
}
