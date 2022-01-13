#define SIM800L_RX     27
#define SIM800L_TX     26
#define SIM800L_PWRKEY 4
#define SIM800L_RST    5
#define SIM800L_POWER  23

String apn = "internet.eplus.de";                    //APN
String apn_u = "eplus";                     //APN-Username
String apn_p = "gprs";     //APN-Password

//Write URLs
String statusUrl = "http://api.thingspeak.com/update?api_key=V6HQCO58JJO1IQSL&field1=1";  //URL of the Rolladen Channel, Status Field (i.e.field1)
String manualUrl = "http://api.thingspeak.com/update?api_key=V6HQCO58JJO1IQSL&field2=0.5"; //URL of the Rolladen Channel, Mode Field (i.e.field2)
String autoUrl = "http://api.thingspeak.com/update?api_key=V6HQCO58JJO1IQSL&field2=1"; //URL of the Rolladen Channel, Mode Field (i.e.field2)

//Read Url
String posReadUrl = "http://api.thingspeak.com/channels/1552232/fields/3/last?api_key=D1HJDBAM6BAY107H&results=2"; // Read field3, for Ideal Position from Leaftech servers
//String idealPos = "";
// char serialBuffer[32]="";
String readResponse="";


void setup()
{
  pinMode(SIM800L_POWER, OUTPUT);
  digitalWrite(SIM800L_POWER, HIGH);

  Serial.begin(115200);
  Serial.println("ESP32+SIM800L AT CMD Test");
  Serial2.begin(9600, SERIAL_8N1, SIM800L_TX, SIM800L_RX);
  delay(15000);
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  delay(2000);
  gsm_send_serial("AT+CPIN=8569");
  gsm_config_gprs();
  statusUpdate(statusUrl);
}


void loop() {
  //modeUpdate();
  Serial.print("ideal position value is:");
  //Serial.print(readResponse);
  delay(2000);
}

void statusUpdate(String url) {
  Serial.println(" --- Start GPRS & HTTP --- ");
  gsm_send_serial("AT+SAPBR=1,1");
  gsm_send_serial("AT+SAPBR=2,1");
  gsm_send_serial("AT+HTTPINIT");
  gsm_send_serial("AT+HTTPPARA=CID,1");
  gsm_send_serial("AT+HTTPPARA=URL," + url);
  //gsm_send_serial("AT+HTTPPARA=CONTENT,application/json");
  //gsm_send_serial("AT+HTTPDATA=192,5000");

  gsm_send_serial("AT+HTTPACTION=0");
  gsm_send_serial("AT+HTTPREAD");
  gsm_send_serial("AT+HTTPTERM");
  gsm_send_serial("AT+SAPBR=0,1");
}

void modeUpdate() {
  Serial.println(" --- Start GPRS & HTTP --- ");
  gsm_send_serial("AT+SAPBR=1,1");
  gsm_send_serial("AT+SAPBR=2,1");
  gsm_send_serial("AT+HTTPINIT");
  gsm_send_serial("AT+HTTPPARA=CID,1");
  gsm_send_serial("AT+HTTPPARA=URL," + posReadUrl);
  //gsm_send_serial("AT+HTTPPARA=CONTENT,application/json");
  //gsm_send_serial("AT+HTTPDATA=192,5000");

  gsm_send_serial("AT+HTTPACTION=0");
  //gsm_send_serial("AT+HTTPREAD");
  Serial.println("Send ->: AT+HTTPREAD");
  Serial2.println("AT+HTTPREAD");
  long wtimer = millis();
  while (wtimer + 3000 > millis()) {
    while (Serial2.available()) {
      Serial.write(Serial2.read());
      //int i=0;
      String inchar = Serial.readString();
      readResponse = readResponse + inchar; 
      //Serial.println();
      //Serial.print("I am reading this dude:");
      //Serial.println(inchar);
      
      //i++;
      //static int bufpos = 0;
      //Serial.print("inchar is");
      //Serial.println(idealPos);
      }
    }
  //char* idealPos = strtok(readResponse, "+,:,\r\n");
  //while (idealPos!=NULL){
    //Serial.print("idealPos stuff is:");
    //Serial.println(idealPos);
  //}

  //Serial.println();
  gsm_send_serial("AT+HTTPTERM");
  gsm_send_serial("AT+SAPBR=0,1");
}

void gsm_config_gprs() {
  Serial.println(" --- CONFIG GPRS --- ");
  gsm_send_serial("AT+SAPBR=3,1,Contype,GPRS");
  gsm_send_serial("AT+SAPBR=3,1,APN," + apn);
  if (apn_u != "") {
    gsm_send_serial("AT+SAPBR=3,1,USER," + apn_u);
  }
  if (apn_p != "") {
    gsm_send_serial("AT+SAPBR=3,1,PWD," + apn_p);
  }
}

void gsm_send_serial(String command) {
  Serial.println("Send ->: " + command);
  Serial2.println(command);
  long wtimer = millis();
  while (wtimer + 3000 > millis()) {
    while (Serial2.available()) {
      Serial.write(Serial2.read());
    }
  }
  Serial.println();
}
