const int relay1 = 2;
const int relay2 = 15;
int movementMode; // 1 for going Up and 2 for going down;
int signalDuration;
int signalMillis;

unsigned long currentMillis;    // stores the value of millis() in each iteration of loop()
unsigned long prevMillis = 0;   // will store last time the LED was updated

void setup() 
{
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
}

void loop() 
{
  currentMillis = millis();   // capture the latest value of millis()
  
  selectMode();
  Serial.println("You selected the movementMode:");
  
  //while(Serial.available() ==0){
    //wait for user input
  //}
  //signalDuration = Serial.parseInt();
  //Serial.print("You entered:");
  Serial.println(movementMode);
  signalMillis = 15m 000;

  while(signalMillis != 0)
  {
    if (movementMode == 1)
    {
      Serial.println("I see you want to move up!");
      up();
      selectMode();
      
    }
    else if(movementMode == 2)
    {
      Serial.println("I see you want to move down!");
      down();
      selectMode();
    }
  }
}

void selectMode()
{
  Serial.println("Enter 1 to go up and 2 to go down!");
  while(Serial.available() ==0)
  {
    //wait for user input
  }
  movementMode = Serial.parseInt();
}
void up()
{
digitalWrite(relay1, HIGH);
Serial.print("Opening them blinds");
delay(signalMillis);
digitalWrite(relay1, LOW);
}

void down()
{
digitalWrite(relay1, HIGH);
digitalWrite(relay2, HIGH);
Serial.print("Opening them blinds");
delay(signalMillis);
digitalWrite(relay1, LOW);
digitalWrite(relay2, LOW);
}
