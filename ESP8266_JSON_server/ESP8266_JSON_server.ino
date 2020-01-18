#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiManager.h> 
#include <Wire.h> 
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> 
#include <TimeLib.h>

#define JSON_BUFFER_SIZE 300
#define PACKET_TIME_OUT_MS 2000
#define SERVER_PORT_NO 80
#define AP_SSID "ESP_07"
#define AP_PSK "abcd1234" // Must consist of 8 to 63 ASCII-encoded characters in the range of 32 to 126 (decimal)


#define BUZZER_FREQ 800

#define AP_ON_DEMAND_TRIGGER_PIN 13
#define GPIO_OUTPUT_1_PIN 12
#define GPIO_OUTPUT_2_PIN 14
#define GPIO_OUTPUT_BUZZER_PIN 16

#define GPIO_OUTOUT_1_ON_DURATION_MS 2000
#define GPIO_OUTOUT_2_ON_DURATION_MS 2000
#define GPIO_OUTOUT_BUZZER_ON_DURATION_MS 1000

#define GPIO_OUTPUT_1_TRIGGER_KEYVALUE 1
#define GPIO_OUTPUT_2_TRIGGER_KEYVALUE 2
#define GPIO_OUTPUT_BUZZER_TRIGGER_KEYVALUE 3

#define UTC_TIME_OFFSETS_HRS 8

// Declare lcd object: auto locate & config display for hd44780 chip
hd44780_I2Cexp lcd; 


// Set up JSON server
WiFiServer server(SERVER_PORT_NO);

// Variable to store receiving packet
char received_packet[JSON_BUFFER_SIZE];


// Time variables to keep track of receiving packet time out 
unsigned long current_time_ms = millis();
unsigned long previous_time_ms = 0; 


// Variables to keep track of gpio ON time
bool gpio_1_is_ON = false;
bool gpio_2_is_ON = false;
unsigned long gpio_1_end_time_ms = 0;
unsigned long gpio_2_end_time_ms = 0; 



// Prepare DynamicJsonDocument for storing JSON packet
DynamicJsonDocument doc(JSON_BUFFER_SIZE);
void setup() {
  Serial.begin(115200);

  // Initialize LCD with number of columns and rows:
  lcd.begin(20, 4);
  
  // Setup IO pins
  pinMode(AP_ON_DEMAND_TRIGGER_PIN, INPUT);
  pinMode(GPIO_OUTPUT_1_PIN, OUTPUT);
  pinMode(GPIO_OUTPUT_2_PIN, OUTPUT);
  pinMode(GPIO_OUTPUT_BUZZER_PIN, OUTPUT);

  // Handle connect to WIFI using wifiManager library
  if ( digitalRead(AP_ON_DEMAND_TRIGGER_PIN) == LOW ) {  //On Demand Configuration Portal
    // Start the configuration portal on demand.
    WiFiManager wifiManager;
    wifiManager.startConfigPortal(AP_SSID, AP_PSK);
  }
  else {
    // Start in Station mode and tries to connect to a previously saved Access Point
    // if unsuccessful (or no previous network saved) will moves the ESP into Access Point mode and 
    // spins up a DNS and WebServer (default ip 192.168.4.1)
    WiFiManager wifiManager;
    wifiManager.autoConnect(AP_SSID, AP_PSK);
  }
  print_string_to_serial("\nSuccessful connected to WIFI");


  // Start JSON server
  server.begin();
  print_string_to_serial("\nServer Started.");
}



void loop(){

  
  WiFiClient client = server.available();               // Listen for incoming clients.
  if (client) {                             
    print_string_to_serial("New client connected");     // Inform client connected.
  
    strcpy(received_packet, "");                        // Clear received_packet.
    current_time_ms = millis();
    previous_time_ms = current_time_ms;

    int no_of_char_received = 0;
    while (client.connected() && current_time_ms - previous_time_ms <= PACKET_TIME_OUT_MS && no_of_char_received < JSON_BUFFER_SIZE) { // loop while the client's connected
      current_time_ms = millis();         
      if (client.available()) {                       // if there's bytes to read from the client,
        char c = client.read();                       // read a byte, then 
        *(received_packet + no_of_char_received) = c; // store to received_packet
        no_of_char_received++;
      }
    }
    *(received_packet + no_of_char_received) = 0;     // add a null at the end

    // Convert received_packet to JSON doc
    DeserializationError err = deserializeJson(doc, received_packet); // using ArduinoJson library

    // Process received JSON packet
    if (err) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.c_str());
    }
    else{
      
        String device_id = doc["device_id"];
        int num_of_faces = doc["num_of_faces"];
        int key_value = doc["key_value"];
        int time_stamp = doc["time_stamp"];
        JsonArray faceID_list = doc["faceID_list"];
  

        Serial.println("");
        Serial.println("Device ID: " + device_id);
        Serial.println("Num of faces: " + String(num_of_faces));
        Serial.println("Key value: " + String(key_value));
        Serial.println("Time stamp: " + String(time_stamp));

        for(JsonVariant v : faceID_list) {
            Serial.println(v.as<char*>());
        }

        // Trigger GPIO_OUTPUT_1 ON
        if (key_value == GPIO_OUTPUT_1_TRIGGER_KEYVALUE){
          digitalWrite(GPIO_OUTPUT_1_PIN, HIGH);
          gpio_1_end_time_ms = millis() + GPIO_OUTOUT_1_ON_DURATION_MS;
          gpio_1_is_ON = true;
        }

        // Trigger GPIO_OUTPUT_2 ON
        if (key_value == GPIO_OUTPUT_2_TRIGGER_KEYVALUE){
          digitalWrite(GPIO_OUTPUT_2_PIN, HIGH);
          gpio_2_end_time_ms = millis() + GPIO_OUTOUT_2_ON_DURATION_MS;
          gpio_2_is_ON = true;
        }


        // Trigger GPIO_OUTPUT_BUZZER ON
        if (key_value == GPIO_OUTPUT_BUZZER_TRIGGER_KEYVALUE){
          tone(GPIO_OUTPUT_BUZZER_PIN, BUZZER_FREQ, GPIO_OUTOUT_BUZZER_ON_DURATION_MS);
        }

        // Trigger GPIO_OUTPUT_1 OFF
        if (gpio_1_is_ON){
          if (millis() > gpio_1_end_time_ms){
            digitalWrite(GPIO_OUTPUT_1_PIN, LOW);
            gpio_1_is_ON = false;
          }
        }


        // Trigger GPIO_OUTPUT_2 OFF
        if (gpio_2_is_ON){
          if (millis() > gpio_2_end_time_ms){
            digitalWrite(GPIO_OUTPUT_2_PIN, LOW);
            gpio_2_is_ON = false;
          }
        }

        // Clear lcd
        lcd.clear();
        
        // Display detected names on lcd (only the first 4 ids will be displayed)
        int lcd_row_index = 0;
        
        for(JsonVariant v : faceID_list) {
          
          lcd.setCursor(0, lcd_row_index);
          const char* this_row_name = v.as<char*>();
          if (lcd_row_index < 3){
            int no_of_char_printed = 0;
            while(* this_row_name && no_of_char_printed <20){ // Only print the first 20 characters
              lcd.print(*this_row_name);
              this_row_name++;
              no_of_char_printed++;
        
            }
          }
          else{
            int no_of_char_printed = 0;
            while(* this_row_name && no_of_char_printed <12){ // Only print the first 12 characters for row #4
              lcd.print(*this_row_name);
              this_row_name++;
              no_of_char_printed++;
            
            }
          }
         
          lcd_row_index++;
          if (lcd_row_index == 4){
            break;
          }
    
          
        }

        // Display datetime to lcd if number of faces is less than 4, else only the time
        int unix_time_epoch = time_stamp + (UTC_TIME_OFFSETS_HRS * 3600);
        if (num_of_faces < 4){
          char datetime_buffer[20];
          sprintf(
            datetime_buffer,
            "%4d-%02d-%02d  %02d:%02d:%02d", 
            year(unix_time_epoch),
            month(unix_time_epoch), 
            day(unix_time_epoch), 
            hour(unix_time_epoch), 
            minute(unix_time_epoch), 
            second(unix_time_epoch));
          Serial.println(datetime_buffer);
          lcd.setCursor(0, 3);
          lcd.print(datetime_buffer);
        }
        else{
          char time_buffer[8];
          sprintf(
            time_buffer,
            "%02d:%02d:%02d", 
            hour(unix_time_epoch), 
            minute(unix_time_epoch), 
            second(unix_time_epoch));
          Serial.println(time_buffer);
          lcd.setCursor(12, 3);
          lcd.print(time_buffer);
        }

    }

    
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void print_string_to_serial(const char *str){
  const char *p;
  p = str;
  while(*p){
      Serial.print(*p);
      p++;
    }
}


String ip2Str(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++) {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  }
  return s;
}
