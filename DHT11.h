#include "DHT.h"
#define DHTPIN 2     // Pin al que se esta conectado fun_fact extrañamente solo lo lee en el pin d1 del nodemcu
#define DHTTYPE DHT11   // Tipo de sensor: DHT 11
DHT dht(DHTPIN, DHTTYPE);

class DTH_measurements {
  public:
  float t;
  float h;
  float hif;

void start_DHT (){dht.begin();}

  
float humedad (){
  h = dht.readHumidity();
  return h; 
  }
  
float temperatura_c(){
  t = dht.readTemperature();
  return t;
  }

  
float s_termica (){
  hif = dht.computeHeatIndex(temperatura_c(),humedad() , false);
  return hif;
}
  
  

  };
