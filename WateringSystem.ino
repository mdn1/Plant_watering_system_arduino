#define LED_BLINK_RATE 1000
#define lmillis() ((long)millis())

//HW Const
const int humiditySensor0 = A0;
const int pump0 = 2;
const int waterButton = 3;
const int led = 13;

//Variables
int rawHumiditySensor0 = 0;
int waterButtonState = 0;

long lastTimeCheck;


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
  
  // read the state of the pushbutton value:
  waterButtonState = digitalRead(waterButton);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (waterButtonState == HIGH) {
  
  //Turn pump on
  digitalWrite(pump0, LOW);

  } else {

    // turn pump off
    digitalWrite(pump0, HIGH);

  }
  
  CheckTimerLed();
  
  
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
