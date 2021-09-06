#define LED_BLINK_RATE 1000
#define lmillis() ((long)millis())

//HW Const
const int humiditySensor0 = A0;
const int pump0 = 2;
const int waterButton = 3;
const int led = 13;

//Variables
int rawHumiditySensor0 = 0;
int waterButtonState = LOW;
int lastWaterButtonState = LOW;
int pumpState = HIGH;

long lastTimeCheck;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers




void setup () {
  
  lastTimeCheck = lmillis() + LED_BLINK_RATE;

  pinMode(pump0, OUTPUT);
  pinMode(led, OUTPUT);
  
  pinMode(waterButton, INPUT);
  
  Serial.begin(9600);
  
  //Set initial pump and LED state
  digitalWrite(pump0, HIGH);
  digitalWrite(led, LOW);
}




void loop() {
  
  rawHumiditySensor0 = analogRead(humiditySensor0);
  
  CheckWaterButtonPressed();
  CheckTimerLed();  
}




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
    }
  }
  

  // set the pump:
  digitalWrite(pump0, pumpState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastWaterButtonState = reading;  
}


void CheckTimerLed (){
  if (lmillis() - lastTimeCheck >= 0) {

        lastTimeCheck = lmillis() + LED_BLINK_RATE;

        Serial.println(rawHumiditySensor0);
        Serial.println(waterButtonState);
        Serial.println();
        
        digitalWrite(led, !digitalRead(led));

    }  
}
