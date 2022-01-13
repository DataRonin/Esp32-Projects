// the number of the LED pin
const int rPin = 32;
const int gPin = 33;
const int bPin = 25;

// setting PWM properties
const int freq = 10000;
const int rChannel = 0;
const int gChannel = 1;
const int bChannel = 2;
const int resolution = 8;

//variables and constants for blinking
unsigned long previousMillis = 0; 
const long interval = 1000;
int ledColor = 0; 
//int ledSelect = 0;
int blinkMode =0;

void setup() {
  // configure LED PWM functionalitites
  ledcSetup(rChannel, freq, resolution);
  ledcSetup(gChannel, freq, resolution);
  ledcSetup(bChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(rPin, rChannel);
  ledcAttachPin(gPin, gChannel);
  ledcAttachPin(bPin, bChannel);
  Serial.begin(115200);
}

void loop() {
  
  ledMode(0,1); //Still Leaftech Color
  delay(4000);

  ledMode(1,2); //Blinking Yellow color;
  delay(4000);
  
  ledMode(0,2);
  delay(4000);
  
  ledMode(1,3);
  delay(4000);
  
  ledMode(1,1);
  delay(4000);
  
  ledMode(0,3);
  delay(4000);
  //setColor("Red");
  //delay(1000);
}

void ledMode(int blinkMode, int ledSelect){
  Serial.println("blinkMode");
  Serial.println(blinkMode);

  Serial.println("ledSelect");
  Serial.println(ledSelect);
  
  unsigned long currentMillis = millis();
  if (blinkMode == 1){
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      // if the LED is off turn it on and vice-versa:
      if (ledColor == 0) {
        ledColor = ledSelect;
        }else {
          ledColor = 0;
          }
       setColor(ledColor);
     }
     else if (blinkMode == 0)
     {
      ledColor = ledSelect;
      setColor(ledColor);
     }
      }
}
// Close to Leaftech colors: 17,108,197
//ledModes: 0 is Off, 1 is Leaftech, 2 is Yellow, 3 is Red

void setColor(int ledColor){
  if (ledColor == 1) {
    ledcWrite(rChannel, 17);
    ledcWrite(gChannel, 108);
    ledcWrite(bChannel, 197);
    }
  // For manual, let us go with Yellow:  245, 238, 11 
  if (ledColor == 2) {
    ledcWrite(rChannel, 245);
    ledcWrite(gChannel, 238);
    ledcWrite(bChannel, 11);
    }
  // For when the motors are moving: let us go with RED:   237, 50, 0

  if (ledColor == 3) {
    ledcWrite(rChannel, 255);
    ledcWrite(gChannel, 0);
    ledcWrite(bChannel, 0);
    }
  if (ledColor == 0) {
    ledcWrite(rChannel, 0);
    ledcWrite(gChannel, 0);
    ledcWrite(bChannel, 0);
    }
}
