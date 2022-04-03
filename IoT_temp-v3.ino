
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "RTClib.h"
#include "DHT.h"


#define DHTPIN 2     // Pin al que se esta conectado
#define DHTTYPE DHT11   // Tipo de sensor: DHT 11

DHT dht(DHTPIN, DHTTYPE);
//RTC_DS1307 rtc;


// Update these with values suitable for your network.

const char* ssid = "INFINITUM92A0_2.4";
const char* password = "jKum6G4WZH";
const char* mqtt_server = "192.168.1.107";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),"Devices", "Iot_is_Amazing" )) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
Serial.begin(115200);
  Serial.println("device started");
  StaticJsonDocument<200> doc;
  
  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  
  setup_wifi();
  dht.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //DateTime time = rtc.now();

 //time.timestamp(DateTime::TIMESTAMP_FULL);
 
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println("Failed to read from DHT sensor!");
    //return;
  //}

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  delay (2000);

//const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(8);
//DynamicJsonDocument doc(capacity);


DynamicJsonDocument doc(384);

doc["guid"] = "61687b4d-0efb-41a1-bf13-0fdea43b4907";
doc["deviceid"] = "SN123456-A";
doc["devicename"] = "ESP82";
doc["deviceip"] = "192.168.56.1";
doc["msgtype"] = "dht11_data";
doc["msgversion"] = "1.0";
doc["msgtime"] = "UTC-6 Mexico";

JsonObject payload_0 = doc["payload"].createNestedObject();
//payload_0["t_stamp"] = time.timestamp(DateTime::TIMESTAMP_FULL);
payload_0["temp"] = t;
payload_0["humd"] = hic;
payload_0["senst"] = h;


char buffer[400];
serializeJson(doc, buffer);


  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //unsigned long now = millis();
  //if (now - lastMsg > 2000) {
    //lastMsg = now;
    //++value;
    //snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publicando mensaje: ");
    Serial.println(buffer);
    client.publish("outTopic", buffer);
    delay(100);
    ESP.deepSleep(30e6);
    delay(100);
  }

  


//#include <ArduinoJson.h>
//#include "DHT11.h"
//#include "mqtt.h"
//#include "json_format.h"
//#include "real_time_clock.h"
//
//mqtt_connection conn;
//DTH_measurements dht11;
//JSON_format json;
//rtc_time t_stamp;
//
//float h;
//float temp_c;
//float sens_t;
//String time_stamp;
//char* message= "";
//char ip_local;
//
//void setup() {
//  
//  Serial.begin(115200);
//  Serial.println("device started");
//  
//  dht11.start_DHT();
//  t_stamp.initialize();
//  conn.setup_wifi();
//  conn.set_server();
//  //client.setCallback(callback);
//  
//
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  delay(2000);
//   h = dht11.humedad ();
//   temp_c = dht11.temperatura_c();
//   sens_t = dht11.s_termica();
//  //time_stamp = t_stamp.TimeStamp();
//  //Serial.println(time_stamp);
//   
//   // verifica si el sensor esta conectado
////   if (isnan(h) || isnan(temp_c) || isnan(sens_t)) {
////    Serial.println("Failed to read from DHT sensor!");
////    return ;
////  }
//
// if (!client.connected()) {
//  conn.reconnect();
//  }
//  client.loop();
//  
//delay(10000);
//
//conn.publish_into_topic(json.format_to_JSON ( temp_c, sens_t, h, t_stamp.TimeStamp() ));
//// message = json.format_to_JSON ( temp_c, sens_t, h );
////  conn.publish_into_topic(message);
//
//}
