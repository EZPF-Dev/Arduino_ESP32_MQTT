#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

class mqtt_connection {
  
  private :
  
  const char* ssid = "ssid";
  const char* password = "pass";
  const char* mqtt_server = "13.72.75.38";

  int port = 1883;
  
  public:
  mqtt_connection (){}
  
  unsigned long lastMsg = 0;
  #define MSG_BUFFER_SIZE  (50)
  char msg[MSG_BUFFER_SIZE];
  int value = 0;
  
  const char* get_ssid(){return ssid;}
  const char* get_password() {return password;}
  const char* get_mqtt_server() {return mqtt_server;}
  int get_port() {return port;}
  

  void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(get_ssid(), get_password());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  //return WiFi.localIP();
  
  }

  void reconnect() {
 
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
//    String clientId = "ESP8266Client-";
//    clientId += String(random(0xffff), HEX);
//    // Attempt to connect
//    if (client.connect(clientId.c_str())) {
//      Serial.println("connected");
//    }
if (client.connect("ESP8266Client-")) {
     Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

  void set_server () {
    client.setServer(get_mqtt_server(), get_port());
    }

void publish_into_topic ( char* message ){
  
    Serial.print("Publicando mensaje: ");
//    int length = strlen(message);
//client.publish(topic, payload, length);
    Serial.println(message);
    if (client.publish("outTopic", message)== true){
      Serial.println("mensaje enviado correctamente"); 
    }
    
  }

    
  
  };
