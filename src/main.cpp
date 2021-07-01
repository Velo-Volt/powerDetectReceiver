#include <esp_now.h>
#include <WiFi.h>
#define N 10
#define RETRY_INTERVAL 5000
// #define debug

// the following 3 settings must match transmitter's settings
//uint8_t mac[] = {0x24, 0x0A, 0xC4, 0x08, 0xA1, 0x10};

typedef struct message // définir une structure message
{
  int current = 0;
  int voltage=0;
  int power=0;
}message;
message myData; 

void setup() {
  WiFi.mode(WIFI_AP_STA);
  //wifi_set_macaddr(SOFTAP_IF, &mac[0]);
  //WiFi.disconnect();
  #ifdef debug
  Serial.begin(9600);
  Serial.println();
  Serial.println("ESP-Now Receiver");
  Serial.printf("Reveiver mac: %s\n", WiFi.macAddress().c_str());
  #endif
  
  if (esp_now_init() != 0) {
  #ifdef debug
    Serial.println("ESP_Now init failed...");
  #endif
    delay(RETRY_INTERVAL);
    ESP.restart();
  }

  esp_now_register_recv_cb(OnDataRecv);
  #ifdef debug
  Serial.println("Slave ready. Waiting for messages...");
  #endif
}

void loop() {

}
void OnDataRecv(const uint8_t * senderMac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, len);
  #ifdef debug
  Serial.printf("Transmitter MacAddr: %02x:%02x:%02x:%02x:%02x:%02x, \n", senderMac[0], senderMac[1], senderMac[2], senderMac[3], senderMac[4], senderMac[5]);
  Serial.print("Current = ");Serial.println(myData.current);
  Serial.print("Voltage = ");Serial.println(myData.voltage);
  Serial.print("Power = ");Serial.println(myData.power);
  #endif
}