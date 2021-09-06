//Defines
#define LOGGING_INTERVAL 5000 //10min
#define LED_BLINK_INTERVAL 1000
#define CLOCK_COUNT_INTERVAL 1000
#define lmillis() ((long)millis())

//HW Const
const int humiditySensor0 = A0;
const int pump0 = 2;
const int waterButton = 3;
const int statusLed = 13;

//Sensor values
int rawSoilHumidity0 = 0;
int rawAirHumidity0 = 0;
int rawAirTemp0 = 0;
long watering0ElapsedTime = 0; //seconds

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


//------------------------------------------------------------------------
//    Setup
//------------------------------------------------------------------------
void setup () {

  lastTimeCheckedStatusLed = lmillis() + LED_BLINK_INTERVAL;
  lastTimeCheckedClock = lmillis() + CLOCK_COUNT_INTERVAL;
  lastTimeCheckedLogger = lmillis() + LOGGING_INTERVAL;

  //Set initial I/O state
  pinMode(pump0, OUTPUT);
  pinMode(statusLed, OUTPUT);
  pinMode(waterButton, INPUT);

  //Serial port
  Serial.begin(9600);

  //Set initial HW state
  digitalWrite(pump0, HIGH);
  digitalWrite(statusLed, LOW);
}



//------------------------------------------------------------------------
//    Loop
//------------------------------------------------------------------------

void loop() {

  rawSoilHumidity0 = analogRead(humiditySensor0);

  CheckWaterButtonPressed();
  CheckTimerLed();
  UpdateClock();  
}



//------------------------------------------------------------------------
//  Private methods
//------------------------------------------------------------------------
void CheckWaterButtonPressed(){
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

      if(waterButtonState == HIGH){
        watering0ElapsedTime = lmillis();
      } 
      else {
        watering0ElapsedTime = lmillis() - watering0ElapsedTime;
        LogData();
      }
    }
  }


  // set the pump:
  digitalWrite(pump0, pumpState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastWaterButtonState = reading;  
}


void CheckTimerLed (){
  if (lmillis() - lastTimeCheckedStatusLed >= 0) {
    lastTimeCheckedStatusLed = lmillis() + LED_BLINK_INTERVAL;
    digitalWrite(statusLed, !digitalRead(statusLed));

  }  
}


void UpdateClock(){
  if (lmillis() - lastTimeCheckedClock >= 0) {

    lastTimeCheckedClock = lmillis() + CLOCK_COUNT_INTERVAL;
    seconds++;

    if(seconds == 60){
      seconds = 0;
      minutes++;
    }
    if(minutes == 60){
      minutes = 0;
      hours++;
    }
    if(hours == 24){
      hours = 0;
      days++;
    }
  }   
}

void CheckTimerLogData(){
  if (lmillis() - lastTimeCheckedLogger >= 0) {

    lastTimeCheckedLogger = lmillis() + LOGGING_INTERVAL;
    LogData();
  }
}

void LogData(){

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
  Serial.println(String(watering0ElapsedTime));

}

