//HW Const
const int humiditySensor0 = A0;
const int pump0 = 2;
const int waterButton = 3;

//Variables
int rawHumiditySensor0 = 0;
int waterButtonState = 0;

void setup () {
  
  pinMode(pump0, OUTPUT);
  pinMode(waterButton, INPUT);
  Serial.begin(9600);
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
  
  delay(50);
  
  Serial.println(rawHumiditySensor0);
  Serial.println(waterButtonState);
  Serial.println();
  
}
