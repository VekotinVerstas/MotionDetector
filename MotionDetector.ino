#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "settings.h"

#define Sens1 D8
#define LED_PIN D1

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
uint8_t sens_status = 0;

void connectWifi() {
  int count = 0;
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    count++;
    Serial.print(".");
    if ( count > 20 ) {
      ESP.restart();
    }
  }
  Serial.println("Connected to wifi");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Liiketunnistin");

  pinMode(Sens1, INPUT_PULLUP);        // sets the digital pin 0 as input
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  connectWifi();
}

static void mqtt_send(const char *topic, const char *message)
{
  // Make sure we have wifi and if not try to get some wifi. If we do not have saved wifi settings create accespoint with esp_id and wifi_pw ( at first run login to ap and save wifi settings ).
  if (!mqttClient.connected()) {
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.connect("Liikesensori", MQTT_USER, MQTT_PASSWORD);
  }
  if (mqttClient.connected()) {
    Serial.print("Publishing ");
    Serial.print(message);
    Serial.print(" to ");
    Serial.print(topic);
    Serial.print("...");
    int result = mqttClient.publish(topic, message, true);
    Serial.println(result ? "OK" : "FAIL");
  }
}

void loop() {
  if ( WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }
  else {
    uint8_t sens_status_new = digitalRead(Sens1);
    // Jos liikettä sytytetään espin led
    if (digitalRead(Sens1) == LOW) {
      digitalWrite(LED_PIN, HIGH);
    }
    else {
      digitalWrite(LED_PIN, LOW);
    }
    // Jos liikettä lähetetään mqtt viesti
    if ( sens_status_new and (sens_status != sens_status_new)) { // Jos liikettä ja edellinen tila on ollut ei liikettä ( ei lähetä samasta liikestää useaa viestiä)
      mqtt_send(MQTT_TOPIC, "aani");
    }
    sens_status=sens_status_new;
  }
}

