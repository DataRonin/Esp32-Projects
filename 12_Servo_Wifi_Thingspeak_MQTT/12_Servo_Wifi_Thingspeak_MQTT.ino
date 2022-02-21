// Include required libraries and define data fields:
#include <PubSubClient.h>
#include <WiFiClientSecure.h>                        // Needed only if using secure connection.
#include <Servo.h>
#include "mqtt_secrets.h"

#define ANGLE_FIELD 0   
#define DATA_FIELD 1                                  // Data field to post the signal strength to.


// Define and initialize the variables. Be sure to edit the wireless network information, channel ID, and API keys.
//  Find your channel ID on the top of the main page for your channel:
const char* ssid = "Vodafone-";
const char* password = "yyy";

const char* server = "mqtt3.thingspeak.com";
char mqttUserName[] = "MBQiIzMTOzALHwcDKTAXOhE";            
char mqttPass[] = "4Nvky7pmbAoA2Ej9Oz8SzL1g";               // Change to your MQTT API key from Account > MyProfile.
char clientID[] = "MBQiIzMTOzALHwcDKTAXOhE";
long readChannelID = 1571316;
long writeChannelID = 1571320;

// Here's how to get ThingSpeak server fingerprint: https://www.a2hosting.com/kb/security/ssl/a2-hostings-ssl-certificate-fingerprints
const char* thingspeak_server_fingerprint = "27:18:92:DD:A4:26:C3:07:09:B9:7A:E6:C5:21:B9:5B:48:F7:16:E1";

// WiFiClient client;                                 // Initialize the Wi-Fi client library. Uncomment for nonsecure connection.
WiFiClientSecure client;                              // Uncomment for secure connection.  
PubSubClient mqttClient( client );                    // Initialize the PuBSubClient library.
Servo myservo;  // Create servo object to control a servo .

int fieldsToPublish[8]={1,1,0,0,0,0,0,0};             // Change to allow multiple fields.
float dataToPublish[8];                               // Holds your field data.
int changeFlag=0;                                     // Let the main loop know there is new data to set.
int servo_pos=0;                                      // Servo position

// Define the function prototypes in this code:
// Prototypes
//

// Handle messages from MQTT subscription.
void mqttSubscriptionCallback(char* topic, byte* payload, unsigned int length);  

// Generate a unique client ID and connect to MQTT broker.
void mqttConnect();  

// Subscribe to a field or feed from a ThingSpeak channel.
int mqttSubscribe( long subChannelID,int field, int unSub);

// Publish messages to a channel feed.

// Connect to a given Wi-Fi SSID.
int connectWifi();

// Measure the Wi-Fi signal strength.
void updateRSSIValue();

//Initialize pins for input and output, start the serial monitor, and initialize the MQTT client in the setup routine.

void setup() {
Serial.begin( 115200 );
Serial.println( "Start" );
int status = WL_IDLE_STATUS; // Set temporary Wi-Fi status.
       
    connectWifi();                                        // Connect to Wi-Fi network.
    // mqttClient.setServer( server, 1883 );              // Set the MQTT broker details, nonsecure port. Uncomment for nonsecure connection.
    mqttClient.setServer( server, 8883 );                 // Set the MQTT broker details, secure port. Uncomment for secure connection.
    mqttClient.setCallback( mqttSubscriptionCallback );   // Set the MQTT message handler function.
    myservo.attach(14);                                   // Attach the servo on GIO2 to the servo object. 
    myservo.write(90);                                    // Start in the middle.
}
// Each time the main loop executes, check to see if data from the MQTT subscription is available for processing. 
// Then set the servo position to match the data. Make sure the wireless and MQTT clients are active and maintain a connection to the client server.

void loop() {
    
    if (WiFi.status() != WL_CONNECTED) {
        connectWifi();
    }
    
    if (!mqttClient.connected())
    {
       
       mqttConnect(); // Connect if MQTT client is not connected.
        
         if(mqttSubscribe( readChannelID,1,0 )==1 ){
                Serial.println( " Subscribed " );
            }
    }
    
    mqttClient.loop(); // Call the loop to maintain connection to the server.                         

    if ((servo_pos>175)||(servo_pos<0)){
    servo_pos=0;
    }
   
    if (changeFlag){
      
        changeFlag=0;
        myservo.write(servo_pos);
        dataToPublish[ANGLE_FIELD]=servo_pos;
        delay(1100);                       // Wait for ThingSpeak to publish.
        Serial.println( "Servo value " + String( servo_pos ) );
        mqttPublish( writeChannelID, dataToPublish, fieldsToPublish );
    }
    
    delay(1);
}

// Use the mqttSubscriptionCallback function to handle incoming MQTT messages.
// The program runs smoother if the main loop performs the processing steps instead of the callback. In this function, use flags to cause changes in the main loop.

/**
 * Process messages received from subscribed channel via MQTT broker.
 * topic - Subscription topic for message.
 * payload - Field to subscribe to. Value 0 means subscribe to all fields.
 * mesLength - Message length.
 */
 
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int mesLength ) {
    
    char p[mesLength + 1];
    memcpy( p, payload, mesLength );
    p[mesLength] = NULL;
    Serial.print( "Answer: " );
    Serial.println( String(p) );
    servo_pos=atoi( p );
    changeFlag=1;
}

// Use the MQTTConnect function to set up and maintain a connection to the MQTT.

void mqttConnect()
{
    // Loop until connected.
    while ( !mqttClient.connected() )
    {
      Serial.println(String( mqttUserName)+ " , " + mqttPass + " , " + clientID);
   
        // Connect to the MQTT broker.
        Serial.print( "Attempting MQTT connection..." );
        if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) )
        {
            Serial.println( "Connected with Client ID:  " + String( clientID ) + " User "+ String( mqttUserName ) + " Pwd "+String( mqttPass ) );
           
        } else
        {
            Serial.print( "failed, rc = " );
            // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
            Serial.print( mqttClient.state() );
            Serial.println( " Will try again in 5 seconds" );
            delay( 5000 );
        }
    }
}

// Use mqttSubscribe to receive updates from the LED control field. In this example, you subscribe to a field, but you can also use this function to subscribe to the whole channel feed.
// Call the function with field = 0 to subscribe to the whole feed.

/**
 * Subscribe to fields of a channel.
 *   subChannelID - Channel to subscribe to.
 *   field - Field to subscribe to. Value 0 means subscribe to all fields.
 *   readKey - Read API key for the subscribe channel.
 *   unSub - Set to 1 for unsubscribe.
 */
 
int mqttSubscribe( long subChannelID, int field, int unsubSub ){
    String myTopic;
    
    // There is no field zero, so if field 0 is sent to subscribe to, then subscribe to the whole channel feed.
    if (field==0){
        myTopic="channels/"+String( subChannelID )+"/subscribe";
    }
    else{
        myTopic="channels/"+String( subChannelID )+"/subscribe/fields/field"+String( field );
    }
    
    Serial.println( "Subscribing to " +myTopic );
    Serial.println( "State= " + String( mqttClient.state() ) );

    if ( unsubSub==1 ){
        return mqttClient.unsubscribe(myTopic.c_str());
    }
    return mqttClient.subscribe( myTopic.c_str() ,0 );
}

// The mqttUnsubscribe function is not used in the code, but you can use it to end a subscription.

/**
 * Unsubscribe channel
 *   subChannelID - Channel to unsubscribe from.
 *   field - Field to unsubscribe subscribe from. The value 0 means subscribe to all fields.
 *   readKey - Read API key for the subscribe channel.
 */

int mqttUnSubscribe(long subChannelID,int field,char* readKey){
    String myTopic;
    
    if (field==0){
         myTopic="channels/"+String( subChannelID )+"/subscribe";
    }
    else{
        myTopic="channels/"+String( subChannelID )+"/subscribe/fields/field"+String( field );
    }
    return mqttClient.unsubscribe( myTopic.c_str() );   
}

// Use the mqttPublish function to send your angle and Wi-Fi RSSI data to a ThingSpeak channel.

/**
 * Publish to a channel
 *   pubChannelID - Channel to publish to.
 *   pubWriteAPIKey - Write API key for the channel to publish to.
 *   dataArray - Binary array indicating which fields to publish to, starting with field 1.
 *   fieldArray - Array of values to publish, starting with field 1.
 */

void mqttPublish(long pubChannelID, float dataArray[], int fieldArray[]) {
    int index=0;
    String dataString="";
    
    updateRSSIValue();  // Make sure the stored value is updated.
    
    // 
    while (index<8){
        
        // Look at the field array to build the posting string to send to ThingSpeak.
        if (fieldArray[ index ]>0){
          
            dataString+="&field" + String( index+1 ) + "="+String( dataArray [ index ] );
        }
        index++;
    }
    
    Serial.println( dataString );
    
    // Create a topic string and publish data to ThingSpeak channel feed.
     String topicString ="channels/" + String( pubChannelID ) + "/publish";
    mqttClient.publish( topicString.c_str(), dataString.c_str() );
    Serial.println( "Published to channel " + String( pubChannelID ) );
}

//Connect your device to a wireless network using the connectWiFi function.

int connectWifi()
{
    while ( WiFi.status() != WL_CONNECTED ) {
        WiFi.begin( ssid, password);
        delay( 8500 );
        Serial.println( "Connecting to Wi-Fi" ); 
    }
    Serial.println( "Connected" );
    client.setFingerprint(thingspeak_server_fingerprint);  // Comment this line if using nonsecure connection.
}

// Use the updateRSSIValue function to read the signal strength for the network that you are presently connected to.

void updateRSSIValue(){

   long rssi = WiFi.RSSI();  
   Serial.print( "RSSI:" );
   Serial.println(rssi);
   dataToPublish[ DATA_FIELD ]=float( rssi );

}
