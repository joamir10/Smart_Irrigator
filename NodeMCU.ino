//libraries 
#include <ESP8266WiFi.h>
#include <espnow.h>

//ESPNOW start
// Structure to receive data, match the sender structure
typedef struct struct_message {
    int b;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");

  Serial.print("Int: ");
  Serial.println(myData.b);

}
//ESPNOW end
//relay start
int relayInput = 2; // the input to the relay pin
//relay end

void setup() {

  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  pinMode(relayInput, OUTPUT); // initialize pin as OUTPUT
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {



  if (myData.b == 10)
    {
      digitalWrite(relayInput, HIGH); // turn relay on
      Serial.print(10);
    }
  if (myData.b == 11)
    {
      digitalWrite(relayInput, LOW); // turn relay off
      Serial.print(11);
    }  


}

