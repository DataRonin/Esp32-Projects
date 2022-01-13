const int PotPin = 32;

int PotInp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(4000);
}

void loop() {
  // put your main code here, to run repeatedly:
  PotInp = analogRead(PotPin);
  Serial.println(PotInp);
  delay(4000);
}
