//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_WIFI_CLOUD
// Include required libraries
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <RemoteXY.h>



// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "AUTO"
#define REMOTEXY_WIFI_PASSWORD "123456789"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "82f29ba2c5d14f743a78533cf7a8c63e"
#define REMOTEXY_ACCESS_PASSWORD "ram1"


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 658 bytes
  { 255,4,0,46,0,139,2,16,16,5,129,0,13,21,34,5,4,24,82,65,
  77,32,73,114,114,105,103,97,116,105,111,110,0,129,0,8,77,45,3,4,
  1,66,121,32,58,32,89,111,117,115,101,102,32,65,109,105,114,32,83,104,
  97,109,115,32,65,108,100,101,101,110,0,131,0,16,91,29,8,1,16,24,
  73,114,114,105,103,97,116,105,111,110,0,129,0,4,12,18,6,1,24,73,
  114,114,105,103,97,116,105,111,110,32,83,116,97,116,101,32,58,0,129,0,
  32,44,28,4,1,24,84,121,112,101,32,79,102,32,80,108,97,110,116,32,
  58,0,129,0,1,44,24,4,1,24,84,121,112,101,32,79,102,32,83,111,
  105,108,58,0,131,0,42,2,20,7,3,16,24,77,111,110,105,116,111,114,
  0,130,1,11,64,39,24,1,24,70,16,32,70,15,15,1,36,135,0,68,
  51,1,12,61,49,3,31,24,203,1,77,111,105,115,116,117,114,101,0,65,
  105,114,32,84,101,109,112,0,72,117,109,105,100,105,116,121,0,67,0,5,
  68,20,6,3,203,64,11,67,0,38,68,20,6,3,203,64,11,67,0,23,
  82,20,6,3,203,64,11,129,0,14,69,8,4,1,36,82,101,100,32,0,
  129,0,13,78,8,3,1,136,71,114,101,101,110,0,129,0,15,82,13,2,
  1,31,73,114,114,105,103,97,116,105,111,110,32,79,78,0,129,0,15,74,
  13,2,1,31,73,114,114,105,103,97,116,105,111,110,32,79,70,70,0,129,
  0,33,66,13,2,1,31,73,114,114,105,103,97,116,105,111,110,32,76,101,
  100,0,131,1,3,2,20,7,4,16,24,72,111,109,101,32,0,129,0,5,
  35,53,4,4,12,87,101,108,99,111,109,101,32,116,111,32,82,65,77,73,
  82,82,32,105,110,32,116,104,105,115,0,129,0,6,41,49,4,4,12,97,
  112,112,32,121,111,117,32,99,97,110,32,109,111,110,105,116,111,114,32,97,
  110,100,32,0,129,0,6,47,47,4,4,12,99,111,110,116,114,111,108,32,
  105,110,32,121,111,117,114,32,105,114,114,105,103,97,116,105,111,110,0,10,
  49,7,26,14,13,1,135,36,31,79,78,0,31,79,70,70,0,10,49,29,
  27,25,10,1,39,208,24,65,117,116,111,109,97,116,105,99,0,24,77,97,
  110,117,97,108,0,129,0,7,22,14,3,1,200,73,110,32,77,97,110,117,
  97,108,32,0,129,0,1,63,22,5,3,200,77,111,105,115,116,117,114,101,
  58,0,129,0,34,63,23,5,3,200,72,117,109,105,100,105,116,121,58,0,
  129,0,20,77,19,5,3,200,65,46,84,101,109,112,58,0,2,1,33,51,
  27,8,1,66,13,24,24,67,111,116,116,111,110,0,82,105,115,101,0,2,
  1,2,51,26,8,1,207,165,24,24,67,108,97,121,0,83,97,110,100,121,
  0,129,0,21,53,16,4,4,12,32,115,121,115,116,101,109,46,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t irrstate_manual_switch; // =1 if state is ON, else =0 
  uint8_t irrstate_switch; // =1 if state is ON, else =0 
  uint8_t plant_switch; // =1 if switch ON and =0 if OFF 
  uint8_t soil_switch; // =1 if switch ON and =0 if OFF 

    // output variables
  uint8_t irrstate_led; // led state 0 .. 1 
  float Graph_var1;
  float Graph_var2;
  float Graph_var3;
  char Moisture[11];  // string UTF8 end zero 
  char Hum[11];  // string UTF8 end zero 
  char Air_Temp[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


// Define soil moisture sensor pin
const int soilMoisturePin = 34;

// DHT11 sensor
#define DHT11PIN 21
DHT dht(DHT11PIN, DHT11);




//ESPNOW Start
// Variables
int int_value;


// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x94, 0xB9, 0x7E, 0x02, 0x41, 0x13};

// Define a data structure
typedef struct struct_message {
    int b;
} struct_message;

// Create a structured object
struct_message myData;

// Peer info
esp_now_peer_info_t peerInfo;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
//ESPNOW END


void setup() 
{
  RemoteXY_Init (); 
  // Initialize Serial communication
  Serial.begin(115200);

  // Set soil moisture sensor pin as input
  pinMode(soilMoisturePin, INPUT);

  dht.begin();
//ESPNOW Start
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;

  }
  //ESPNOW End  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
    // Read soil moisture
  int soilMoisture = analogRead(soilMoisturePin);

  // Map the soil moisture value to a percentage (0-100%)
  int moisturePercentage = map(soilMoisture, 4095, 1600, 0, 100);

  // Print the soil moisture value
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println("%");

  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("ºC ");

  Serial.print("Humidity: ");
  Serial.print(humi);
  Serial.println("%");


  // Delay before reading again (adjust as needed)
  delay(500);

  RemoteXY.Graph_var1 = moisturePercentage;
  dtostrf(moisturePercentage, 0, 2, RemoteXY.Moisture);
  
  RemoteXY.Graph_var3 = humi;
  dtostrf(humi, 0, 2, RemoteXY.Hum);

  RemoteXY.Graph_var2 = temp;
  dtostrf(temp, 0, 2, RemoteXY.Air_Temp);


//IF Start
 int SM = 35;    
 int data = 10;  
if (RemoteXY.irrstate_switch == 0 && RemoteXY.irrstate_manual_switch == 1) {
    data = 10;
  } else if (RemoteXY.irrstate_switch == 0 && RemoteXY.irrstate_manual_switch == 0) {
    data = 11;
  } else if (RemoteXY.plant_switch == 0 && RemoteXY.soil_switch == 0 && RemoteXY.irrstate_switch == 1 && moisturePercentage < 50) {
    data = 11;
  } else if (RemoteXY.plant_switch == 0 && RemoteXY.soil_switch == 1 && RemoteXY.irrstate_switch == 1 && moisturePercentage < 40) {
    data = 11;
  } else if (RemoteXY.plant_switch == 1 && RemoteXY.soil_switch == 0 && RemoteXY.irrstate_switch == 1 && moisturePercentage < 30) {
    data = 11;
  } else if (RemoteXY.plant_switch == 1 && RemoteXY.soil_switch == 1 && RemoteXY.irrstate_switch == 1 && moisturePercentage < 20) {
    data = 11;
  } else {
    data = 10;
  }
//IF End  
// led Start

  if (data == 10) {
    RemoteXY.irrstate_led = 1;
  } else {
    RemoteXY.irrstate_led = 0;
  }
  // Print the value of data to the serial monitor
  Serial.print("Data: ");
  Serial.println(data);   
    delay(10);
// led End




//ESPNOW Start
  int_value = 11;
  // Format structured data
  myData.b = data;

  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  delay(2000);
//ESPNOW End
}