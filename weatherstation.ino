#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <DHT.h>
#include <uTimerLib.h>

DHT dht(5, DHT11);
WiFiClient wiFiClient;
Adafruit_MQTT_Client mqttClient(&wiFiClient, "192.168.0.103", 1883);
Adafruit_MQTT_Publish temperaturePublisher(&mqttClient, "/temperature");
Adafruit_MQTT_Publish humiditePublisher(&mqttClient, "/humidite");


void sendData() {
  int temperature = dht.readTemperature();
  int humidity = dht.readHumidity();
  temperaturePublisher.publish(temperature);
  humiditePublisher.publish(humidity);
}


void setup() {

  dht.begin();
  
  Serial.begin(115200);

  WiFi.begin("createch2019", "createch2019");
  delay(5000);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  TimerLib.setInterval_s(sendData, 2);
}

void loop() {

  if (mqttClient.connected()) {
    mqttClient.processPackets(10000);
    mqttClient.ping();
  } else {
    mqttClient.disconnect();
    mqttClient.connect();
  }


 

}
