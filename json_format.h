#include <ArduinoJson.h>

class JSON_format {
  
  public:
 char* format_to_JSON  (float t, float h , float hic, String t_stamp ){
  
//  const char* tiempo = "";
//  int t_stamp_len = t_stamp.length()+1;
//  char tempo [t_stamp_len];
//  t_stamp.toCharArray(tempo,t_stamp_len);
//  tiempo = tempo;
//  Serial.println(tiempo);

DynamicJsonDocument doc(384);

doc["guid"] = "61687b4d-0efb-41a1-bf13-0fdea43b4907";
doc["deviceid"] = "SN123456-A";
doc["devicename"] = "ESP82";
doc["deviceip"] = "192.168.56.1";
doc["msgtype"] = "dht11_data";
doc["msgversion"] = "1.0";
doc["msgtime"] = "UTC-6 Mexico";

JsonObject payload_0 = doc["payload"].createNestedObject();
//payload_0["t_stamp"] = t_stamp.c_str(); ;
payload_0["temp"] = t;
payload_0["humd"] = hic;
payload_0["senst"] = h;


 char message [400]= "";
 //serializeJson(doc, message, messageCapacity);
 serializeJson(doc, message);

//Serial.println(buffer);
return message ;
}





};
