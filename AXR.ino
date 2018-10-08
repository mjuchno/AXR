//Arduino pins
const int PIN_BUTTON = 2; // buttond pin
const int LED_BUTTON = 13; // onboard LED (or external on pin 13) for button response
const int LED_SHIELD = 12; // LED for shield status

//button state and timer
int buttonStateNow = 0;
int buttonStateOld = 0;

unsigned long buttonTimeDel = 50; // check evety 50ms
unsigned long buttonTimeNow = 0;
unsigned long buttonTimeOld = 0;

//for serial communication
String inputString = "";  
boolean stringComplete = false; 

//LED status
boolean shieldLedState = false;
boolean shieldBlinkState = false;

//Duration and delays for a blinking LED
unsigned long blinkDel = 0;
unsigned long blinkDur = 0;
unsigned long del3 = 300;
unsigned long dur3 = 1000;
unsigned long del2 = 600;
unsigned long dur2 = 200;
unsigned long del1 = 100;
unsigned long dur1 = 100;

//blink timers
unsigned long blinkTimeNow = 0;
unsigned long blinkTimeOld = 0;

void setup() {
  pinMode(LED_SHIELD, OUTPUT);
  pinMode(LED_BUTTON, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  Serial.begin(9600);
}

void loop() {

  // read the state of the pushbutton value
  // change onboard led (optional, for testing)
  // send button state to python
  buttonTimeNow = millis();
  if (buttonTimeNow-buttonTimeOld > buttonTimeDel) {
    buttonTimeOld = buttonTimeNow;
    buttonStateNow = digitalRead(PIN_BUTTON);
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonStateNow == HIGH && buttonStateNow != buttonStateOld) {
      digitalWrite(LED_BUTTON, HIGH);
      buttonStateOld = buttonStateNow;
      Serial.println("BN ON");
    }
    if (buttonStateNow == LOW && buttonStateNow != buttonStateOld) {
      digitalWrite(LED_BUTTON, LOW);
      buttonStateOld = buttonStateNow;
      Serial.println("BN OFF");
    }
  }
    
  // LED control based on a Serial communication:
  if (stringComplete) {
    int bytesSent = Serial.print(inputString);
    if (inputString == "S4\n") {
      shieldBlinkState = false;
      shieldLedState = true;
      digitalWrite(LED_SHIELD, HIGH);
      }
    if (inputString == "S3\n") {
      shieldBlinkState = true;
      shieldLedState = false;
      digitalWrite(LED_SHIELD, LOW);
      blinkDel = del3;
      blinkDur = dur3;
      }
    if (inputString == "S2\n") {
      shieldBlinkState = true;
      shieldLedState = false;
      digitalWrite(LED_SHIELD, LOW);
      blinkDel = del2;
      blinkDur = dur2;
      }
    if (inputString == "S1\n") {
      shieldBlinkState = true;
      shieldLedState = false;
      digitalWrite(LED_SHIELD, LOW);
      blinkDel = del1;
      blinkDur = dur1;
      }
    if (inputString == "S0\n") {
      shieldBlinkState = false;
      shieldLedState = false;
      digitalWrite(LED_SHIELD, LOW);
      }
    // clear the string:
    inputString = "";
    stringComplete = false;
    }

    // blinking LED
    if (shieldBlinkState) {
      blinkTimeNow = millis();
      if ((shieldLedState == true) && (blinkTimeNow-blinkTimeOld > blinkDur)) {
        blinkTimeOld = blinkTimeNow;
        digitalWrite(LED_SHIELD, LOW);
        shieldLedState = false;
        }
      if ((shieldLedState == false) && (blinkTimeNow-blinkTimeOld > blinkDel)) {
        blinkTimeOld = blinkTimeNow;
        digitalWrite(LED_SHIELD, HIGH);
        shieldLedState = true;
        }
      }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
