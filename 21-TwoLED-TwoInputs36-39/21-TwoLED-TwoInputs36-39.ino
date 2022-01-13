// Complete Instructions: https://RandomNerdTutorials.com/esp32-digital-inputs-outputs-arduino/
// code for two analog inputs- buttons or touch sensors for the relays
// set pin numbers
const int buttonPin1 = 36;  // the number of the pushbutton pin
const int buttonPin2 = 39;

const int ledPin1 =  2;    // the number of the LED pin
const int ledPin2 =  15;    // the number of the LED pin

// variable for storing the pushbutton status 
int buttonState1 = 0;
int buttonState2 = 0;

void setup() {
  Serial.begin(115200);  
  // initialize the pushbutton pin as an input
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  // initialize the LED pin as an output
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  // read the state of the pushbutton value
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  Serial.println(buttonState1);
  Serial.println(buttonState2);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH
  if (buttonState1 == HIGH) {
    // turn LED on
    digitalWrite(ledPin1, HIGH);
  } else {
    // turn LED off
    digitalWrite(ledPin1, LOW);
  }
    if (buttonState2 == HIGH) {
    // turn LED on
    digitalWrite(ledPin2, HIGH);
  } else {
    // turn LED off
    digitalWrite(ledPin2, LOW);
  }
}
