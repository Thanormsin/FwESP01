//////////////////////////////////////////////////////////
//  _____        _                    _____      _
// |  __ \      (_)                  / ____|    (_)
// | |  | |_   _ _ _ __   ___ ______| |     ___  _ _ __
// | |  | | | | | | '_ \ / _ \______| |    / _ \| | '_ \ 
// | |__| | |_| | | | | | (_) |     | |___| (_) | | | | |
// |_____/ \__,_|_|_| |_|\___/       \_____\___/|_|_| |_|
//  Code for ESP8266 boards - V2.55
//  © Duino-Coin Community 2019-2021
//  Distributed under MIT License
//////////////////////////////////////////////////////////
//  https://github.com/revoxhere/duino-coin - GitHub
//  https://duinocoin.com - Official Website
//  https://discord.gg/k48Ht5y - Discord
//  https://github.com/revoxhere - @revox
//  https://github.com/JoyBed - @JoyBed
//  https://github.com/kickshawprogrammer - @kickshawprogrammer
//////////////////////////////////////////////////////////
//  If you don't know what to do, visit official website
//  and navigate to Getting Started page. Happy mining!
//////////////////////////////////////////////////////////

// Credit to : Farrukh
// Link to : https://www.youtube.com/watch?v=qCmdUtguwPw&t=321s
// Link to : https://www.youtube.com/watch?v=FuY6BobS-1k&t=0s

// Modify by : Thanormsin.M
// Modify Date : 11-Jul-21
#include <EEPROM.h>
//#include <Wire.h>

#include <ESP8266WiFi.h> // Include WiFi library
#include <ESP8266mDNS.h> // OTA libraries
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>
#include <CertStoreBearSSL.h>
BearSSL::CertStore certStore;
#include <time.h>

char* USERNAME = "";
char* RIG_IDENTIFIER = "";
char* AcctName = "Thanormsin";  //User Name
char* BoardID = "ESP01-B0-U99"; //Rig Name

const String FirmwareVer={"2.1"}; // Last of Version
#define URL_fw_Version "/Thanormsin/FwESP01/master/bin_version.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/Thanormsin/FwESP01/master/fw.bin"
                   
const char* host = "raw.githubusercontent.com";
const int httpsPort = 443;

// DigiCert High Assurance EV Root CA
const char trustRoot[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j
ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL
MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3
LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug
RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm
+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW
PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM
xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB
Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3
hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg
EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF
MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA
FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec
nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z
eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF
hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2
Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe
vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep
+OkuE6N36B9K
-----END CERTIFICATE-----
)EOF";
X509List cert(trustRoot);

extern const unsigned char caCert[] PROGMEM;
extern const unsigned int caCertLen;

const char* ssid = "MY_IOT";
const char* password = "0819065291";

void setClock() {
   // Set time via NTP, as required for x.509 validation
   configTime(6 * 3600, 0, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  //configTime(6 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
    
    //Blinking LED 
    digitalWrite(LED_BUILTIN,HIGH);
  }
  
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void FirmwareUpdate()
{  
  //Add New Line
  Serial.println(" ");
  
  WiFiClientSecure client;
  client.setTrustAnchors(&cert);
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }
  client.print(String("GET ") + URL_fw_Version + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      //Serial.println("Headers received");
      break;
    }
  }
  String payload = client.readStringUntil('\n');

  payload.trim();
  if(payload.equals(FirmwareVer) )
  {   
     Serial.println("Device already on latest firmware version"); 
     Serial.println(URL_fw_Bin);
  }
  else
  {
    Serial.println("New firmware detected");
    Serial.println("Downloading and install new firmware");
    Serial.println(URL_fw_Bin);
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW); 
  //t_httpUpdate_return ret = httpUpdate.update(client, URL_fw_Bin);
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, URL_fw_Bin);
  //t_httpUpdate_return ret = ESPhttpUpdate.update("http://mydomain.com/arduino.bin");
        
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.println(URL_fw_Bin);
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    } 
  }
 }  

void connect_wifi();
void firmwareUpdate();

unsigned long previousMillis_2 = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 60000;              // Change Time Firmware ReUpdate
const long mini_interval = 1000;

 void repeatedCall(){
    unsigned long currentMillis = millis();
    if ((currentMillis - previousMillis) >= interval) 
    {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
//      setClock();
      FirmwareUpdate();
    }

    if ((currentMillis - previousMillis_2) >= mini_interval) {
      static int idle_counter=0;
      previousMillis_2 = currentMillis;    
      Serial.print("Active fw version:");
      Serial.println(FirmwareVer);
      Serial.print("Idle Loop....");
      Serial.println(idle_counter++);
      Serial.println(" ");
     if(idle_counter%2==0)
      digitalWrite(LED_BUILTIN, HIGH);
     else 
      digitalWrite(LED_BUILTIN, LOW);
     if(WiFi.status() == !WL_CONNECTED) 
      connect_wifi();
      
   }
 }

//////////////////////////////////////////////////////////
// NOTE: If during compilation, the below line causes a
// "fatal error: Crypto.h: No such file or directory"
// message to occur; it means that you do NOT have the
// latest version of the ESP8266/Arduino Core library.
//
// To install/upgrade it, go to the below link and
// follow the instructions of the readme file:
//
//       https://github.com/esp8266/Arduino
//////////////////////////////////////////////////////////
#include <Crypto.h>  // experimental SHA1 crypto library
using namespace experimental::crypto;

#include <Ticker.h>

namespace {
const char* SSID          = "MY_IOT";   // Change this to your WiFi name
const char* PASSWORD      = "0819065291";    // Change this to your WiFi password
//char* USERNAME      = "Test1";     // Change this to your Duino-Coin username
//char* RIG_IDENTIFIER = "Test2";       // Change this if you want a custom miner name

// Since 2.5.5 additional mining nodes available - you can change it manually to one of these:
// Official Master Server: 51.15.127.80 port 2820
// Official Kolka Pool: 149.91.88.18 port 6000
// This will be replaced with an automatic picker in the future version

//const char * host = "51.15.127.80";
//const int port = 2811;
//const int port = 2812;
//const int port = 2813;
//const int port = 2814;
//const int port = 2815;
//const int port = 2820;
//const int port = 5000;

const char * host = "149.91.88.18"; // Static server IP
const int port = 6000;

unsigned int share_count = 0; // Share variable

WiFiClient client;
String client_buffer = "";
String chipID = "";

// Loop WDT... please don't feed me...
// See lwdtcb() and lwdtFeed() below
Ticker lwdTimer;
#define LWD_TIMEOUT   60000

unsigned long lwdCurrentMillis = 0;
unsigned long lwdTimeOutMillis = LWD_TIMEOUT;

#define END_TOKEN  '\n'
#define SEP_TOKEN  ','

#define LED_BUILTIN 2

#define BLINK_SHARE_FOUND    1
#define BLINK_SETUP_COMPLETE 2
#define BLINK_CLIENT_CONNECT 3
#define BLINK_RESET_DEVICE   5

void SetupWifi() {
  Serial.println("Connecting to: " + String(SSID));
  WiFi.mode(WIFI_STA); // Setup ESP in client mode
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(SSID, PASSWORD);

  int wait_passes = 0;
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //Blinking LED 
    digitalWrite(LED_BUILTIN,HIGH);
    
    if (++wait_passes >= 10) {
      WiFi.begin(SSID, PASSWORD);
      wait_passes = 0;
    }
  }

  Serial.println("\nConnected to WiFi!");
  Serial.println("Local IP address: " + WiFi.localIP().toString());
}

void SetupOTA() {
  // Prepare OTA handler
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.setHostname(RIG_IDENTIFIER); // Give port a name not just address
  ArduinoOTA.begin();
}

void blink(uint8_t count, uint8_t pin = LED_BUILTIN) {
  uint8_t state = HIGH;

  for (int x = 0; x < (count << 1); ++x) {
    digitalWrite(pin, state ^= HIGH);
    delay(50);
  }
}

void RestartESP(String msg) {
  Serial.println(msg);
  Serial.println("Resetting ESP...");
  blink(BLINK_RESET_DEVICE);
  ESP.reset();
}

// Our new WDT to help prevent freezes
// code concept taken from https://sigmdel.ca/michel/program/esp8266/arduino/watchdogs2_en.html
void ICACHE_RAM_ATTR lwdtcb(void)
{
  if ((millis() - lwdCurrentMillis > LWD_TIMEOUT) || (lwdTimeOutMillis - lwdCurrentMillis != LWD_TIMEOUT))
    RestartESP("Loop WDT Failed!");
}

void lwdtFeed(void) {
  lwdCurrentMillis = millis();
  lwdTimeOutMillis = lwdCurrentMillis + LWD_TIMEOUT;
}

void VerifyWifi() {
  while (WiFi.status() != WL_CONNECTED || WiFi.localIP() == IPAddress(0, 0, 0, 0))
    WiFi.reconnect();
}

void handleSystemEvents(void) {
  VerifyWifi();
  ArduinoOTA.handle();
  yield();
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int max_index = data.length() - 1;

  for (int i = 0; i <= max_index && found <= index; i++) {
    if (data.charAt(i) == separator || i == max_index) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == max_index) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void waitForClientData(void) {
  client_buffer = "";

  while (client.connected()) {
    if (client.available()) {
      client_buffer = client.readStringUntil(END_TOKEN);
      if (client_buffer.length() == 1 && client_buffer[0] == END_TOKEN)
        client_buffer = "???\n"; // NOTE: Should never happen

      break;
    }
    handleSystemEvents();
  }
}

// LCD Disply Setup /////////////////////////////////////////////////////////////
void ConnectToServer() { 
  if (client.connected())
    return;

  Serial.println("\nConnecting to Duino-Coin server...");
  while (!client.connect(host, port));

  waitForClientData();
  Serial.println("Connected to the server. Server version: " + client_buffer );
  blink(BLINK_CLIENT_CONNECT); // Sucessfull connection with the server
}

bool max_micros_elapsed(unsigned long current, unsigned long max_elapsed) {
  static unsigned long _start = 0;

  if ((current - _start) > max_elapsed) {
    _start = current;
    return true;
  }
  return false;
}
} // namespace

struct MyConfig {
  char* USERNAME;
  char* RIG_IDENTIFIER;
//  int Value1;
//  double Value2;
};

void GetConfig() { 
  //int eeAddress = sizeof(float); //Move address to the next byte after float 'f'.
  byte eeAddress = 0;
  MyConfig customVar; //Variable to store custom object read from EEPROM.
  EEPROM.get( eeAddress, customVar );
  USERNAME = customVar.USERNAME;
  RIG_IDENTIFIER = customVar.RIG_IDENTIFIER;
}

void PreSetConfig() {
  MyConfig customVar = {
  AcctName,
  BoardID,
  };
  byte eeAddress = 0; //Move address to the next byte after float 'f'.

  EEPROM.put( eeAddress, customVar );
  EEPROM.commit();
  
  Serial.println("Update New Name");
  delay(2000);
}

void setup() {
  // Start serial connection
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //Blinking LED 
    digitalWrite(LED_BUILTIN,HIGH);
  }
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // EEPROM Setup /////////////////////////////////////////////////////////////
  EEPROM.begin(512);
  delay(1000);
  GetConfig();
  
  //AcctName
  Serial.println(" ");
  Serial.print("1-Hello my name is " );
  Serial.print(USERNAME);
  Serial.print("/");
  Serial.print(RIG_IDENTIFIER);
  
  if (USERNAME != AcctName) {
    Serial.println("New board. Write config to eeprom");
    PreSetConfig();
    GetConfig();
  }  
 
  Serial.println(" ");
  Serial.print("2-Hello my name is " );
  Serial.print(USERNAME);
  Serial.print("/");
  Serial.print(RIG_IDENTIFIER);

  //---------------จบคำสั่งเซ็ตจอ LCD มอนิเตอร์ โค้ดนี้เขียนสำหรับจอ 20x4 -------------------------//
  
  Serial.println("\nDuino-Coin ESP8266 Miner v2.55");

  // Prepare for blink() function
  pinMode(LED_BUILTIN, OUTPUT);

  SetupWifi();
  //Blinking LED 
  digitalWrite(LED_BUILTIN,HIGH);   

  SetupOTA();
  //Blinking LED 
  digitalWrite(LED_BUILTIN,HIGH);   

  lwdtFeed();
  lwdTimer.attach_ms(LWD_TIMEOUT, lwdtcb);

  // Sucessfull connection with wifi network
  blink(BLINK_SETUP_COMPLETE);

  chipID = String(ESP.getChipId(), HEX);

  connect_wifi(); 

  WiFi.mode(WIFI_STA);;

  //Check On start up
  Serial.println(" ");
  Serial.println("Firmware Check on Starting..");
  Serial.println("Current Firmware : " + FirmwareVer);
  
  setClock();
  
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // 1 minute watchdog
  lwdtFeed();

  // OTA handlers
  VerifyWifi();
  ArduinoOTA.handle();

  ConnectToServer();
  
  //Blinking LED 
  digitalWrite(LED_BUILTIN,HIGH);   
  
  Serial.println("Asking for a new job for user: " + String(USERNAME)+ "/" + String(RIG_IDENTIFIER));
  client.print("JOB," + String(USERNAME) + ",ESP8266");

  waitForClientData();
  String last_block_hash = getValue(client_buffer, SEP_TOKEN, 0);
  String expected_hash = getValue(client_buffer, SEP_TOKEN, 1);
  unsigned int difficulty = getValue(client_buffer, SEP_TOKEN, 2).toInt() * 100 + 1;

  //Blinking LED 
  digitalWrite(LED_BUILTIN,HIGH);   
  
  Serial.println("Job received: "
                 + last_block_hash
                 + " "
                 + expected_hash
                 + " "
                 + String(difficulty));
  expected_hash.toUpperCase();

  //Blinking LED
  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(50);

  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(50);

  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(50);

  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(50);
  
  digitalWrite(LED_BUILTIN,HIGH); 

  float start_time = micros();
  max_micros_elapsed(start_time, 0);

  for (unsigned int duco_numeric_result = 0; duco_numeric_result < difficulty; duco_numeric_result++) {
    // Difficulty loop
    String result = SHA1::hash(last_block_hash + String(duco_numeric_result));

    if (result == expected_hash) {
      // If result is found
      unsigned long elapsed_time = micros() - start_time;
      float elapsed_time_s = elapsed_time * .000001f;
      float hashrate = duco_numeric_result / elapsed_time_s;
      share_count++;

      client.print(String(duco_numeric_result)
                   + ","
                   + String(hashrate)
                   + ",ESP8266 Miner v2.55"
                   + ","
                   + String(RIG_IDENTIFIER)
                   + ","
                   + String(chipID));
  

  
      waitForClientData();
      Serial.println(client_buffer
                     + " share #"
                     + String(share_count)
                     + " (" + String(duco_numeric_result) + ")"
                     + " hashrate: "
                     + String(hashrate / 1000, 2)
                     + " kH/s ("
                     + String(elapsed_time_s)
                     + "s) Free RAM: "
                     + String(ESP.getFreeHeap()));
                     
      //Blinking LED
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(50);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(50);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(50);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(50);
      
      digitalWrite(LED_BUILTIN,HIGH); 

  //--------------ปิดชุดคำสั่งส่งข้อมูลออกจอ LCD มอนิเตอร์ โค้ดนี้เขียนสำหรับจอ 20x4 -------------------------// 

//      blink(BLINK_SHARE_FOUND);
      break;
    }
    if (max_micros_elapsed(micros(), 250000))
      handleSystemEvents();
  }

/*
//////////////////////////////////////////////////////////////////////////////////////////////////
  if (button_boot.pressed) { //to connect wifi via Android esp touch app 
    Serial.println("Manual Firmware update Starting..");
    FirmwareUpdate();
    button_boot.pressed = false;
  }
*/
  
  repeatedCall(); 

  //Blinking LED 
  digitalWrite(LED_BUILTIN,HIGH);   
  
}

void connect_wifi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
//  delay(200);
    //Blinking
    digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
    delay(100);
  
    digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
    delay(100);
  
    digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
    delay(100);
  
    digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
    delay(100);
  
    digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
    delay(100);
  
    digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
    delay(100);
  
    digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
    delay(100);
  
    digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
    delay(100);
  
    Serial.print("O");
  }                                   
  Serial.println("Connected to WiFi");
}
