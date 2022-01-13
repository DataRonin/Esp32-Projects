
#define SIM800L_RX     27
#define SIM800L_TX     26
#define SIM800L_PWRKEY 4
#define SIM800L_RST    5
#define SIM800L_POWER  23

void setup()
{
  pinMode(SIM800L_POWER, OUTPUT);
  digitalWrite(SIM800L_POWER, HIGH);

  Serial.begin(115200);
  Serial.println("ESP32+SIM800L AT CMD Test");
  delay(10000);
  
  Serial2.begin(9600, SERIAL_8N1, SIM800L_TX, SIM800L_RX);
}


void loop() {
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  while (Serial.available()) {
     Serial2.print(char(Serial.read()));
  }
}
