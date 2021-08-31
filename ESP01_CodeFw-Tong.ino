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

//### Auto WiFi Connect ##################
const char* ssidNm    = "";

const char* ssid1     = "Tha7-2.4G";        //<-- ใส่ชือ Wifi รอง ตรงนี ถ้าใช้ WiFi ตัวเดียวก็ให้ใส่เหมือนกันกับ ssid1
const char* password1 = "0819065291";         //<-- ใส่รหัส Wifi รอง ตรงนี

const char* ssid2     = "Tha8-2.4G";        //<-- ใส่ชือ Wifi หลัก
const char* password2 = "1234567890";         //<-- ใส่รหัส Wifi หลัก ตรงนี

//อันนี้ใช้ได้


//### Other ##################
int lastWiFi = 0;
//int ReCnctWiFi = 1;

String USERNAME       = "";
String RIG_IDENTIFIER = "";
String AcctName       = "Thanormsin";  //<-- ใส่ชือ User Name ตรงนี
String BrdNm          = "Tha-69"; //<-- ใส่ชือ RigName ตรงนี

String eeprom_acct;
String eeprom_brd;
String HostNm          = ""; 
int x;

// รายละเอียดของ OTI, ตำแหน่งเก็บไฟล์
const String FirmwareVer={"3.6"}; // Last of Version\
//Siwadol
//#define URL_fw_Version "/Thanormsin/FwESP01S/master/bin_version-Tha.txt"
//#define URL_fw_Bin "https://raw.githubusercontent.com/Thanormsin/FwESP01S/master/fw-Tha.bin"

//Thanormsin
#define URL_fw_Version "/Thanormsin/FwESP01/master/bin_version-Nop.txt"
#define URL_fw_Bin "https://raw.githubusercontent.com/Thanormsin/FwESP01/master/fw-Nop.bin"
                   
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
  setClock();
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

//void connect_wifi();
//void firmwareUpdate();

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
      Connect2WiFi();
      
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
//const char* SSID          = "MY_IOT";   // Change this to your WiFi name
//const char* PASSWORD      = "0819065291";    // Change this to your WiFi PASSWORD
//char* USERNAME      = "Test1";     // Change this to your Duino-Coin username
//char* RIG_IDENTIFIER = "Test2";       // Change this if you want a custom miner name

// Official Master Server: 51.15.127.80 port 2820
// Official Kolka Pool: 149.91.88.18 port 6000
int port;
const char* host;

//beyond-pool-2
//const char * host1 = "35.173.194.122"; // Static server IP
//const int port1 = 6000;
//const int port1 = 6002;

//pi-pool-1
//const char * host1 = "87.188.155.115"; // Static server IP
//const int port1 = 6000;

//Pulse Pool
const char * host1 = "149.91.88.18"; // Static server IP
const int port1 = 6000;

//StarPool
const char * host2 = "51.158.182.90"; // Static server IP
const int port2 = 6000;
//Master Wallet
const char * host3 = "193.164.7.180"; // Static server IP
const int port3 = 6000;

//Beyond Pool-1
const char * host4 = "50.112.145.154"; // Static server IP
const int port4 = 6000;

//const int port2 = 6001;
//Master Pool
const char * host5 = "51.15.127.80"; // Static server IP
const int port9 = 2810;
const int port8 = 2811;
const int port7 = 2812;
const int port6 = 2813;
const int port5 = 2820;

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

  ArduinoOTA.setHostname(RIG_IDENTIFIER.c_str()); // Give port a name not just address
  ArduinoOTA.begin();
}

void blink(uint8_t count, uint8_t pin = LED_BUILTIN) {
  uint8_t state = HIGH;

  for (int x = 0; x < (count << 1); ++x) {
    digitalWrite(pin, state ^= HIGH);
    delay(200);
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

void ConnectToServer() {
  if (client.connected())
    return;

int i;
  for (i = 1; i <= 30; i++) {
    if (!client.connected()) {
      client.connect(host1, port1);
//      host = host1;
      port = port1;
      Serial.print("Try Connect to : ");
      Serial.print("Pulse Pool/" + String(host1) );
      //Serial.print(host1 );
      Serial.print("/");
      Serial.println(port);
    }
    if (!client.connected()) {
      client.connect(host2, port2);
//      host = host2.c_str;
      port = port2;
      Serial.print("Try Connect to : ");
      Serial.print("Star Pool/" + String(host2) );
      //Serial.print(host2);
      Serial.print("/");
      Serial.println(port);
    }
    if (!client.connected()) {
      client.connect(host3, port3);
//      host = host3.c_str;
      port = port3;
      Serial.print("Try Connect to : ");
      Serial.print("Master Wallet/" + String(host3) );
      //Serial.print(host3);
      Serial.print("/");
      Serial.println(port);
    }
    if (!client.connected()) {
      client.connect(host4, port4);
//      host = host3.c_str;
      port = port4;
      Serial.print("Try Connect to : ");
      Serial.print("Beyond Pool-1/" + String(host4) );
      //Serial.print(host3);
      Serial.print("/");
      Serial.println(port);
    }    
    if (!client.connected()) {
      client.connect(host5, port5);
//      host = host4.c_str;
      port = port5;
      Serial.print("Try Connect to : ");
      Serial.print("Master Pool/" + String(host5) );
      //Serial.print(host5);
      Serial.print("/");
      Serial.println(port);
    }
    if (!client.connected()) {
      client.connect(host5, port6);
//      host = host4;
      port = port6;
      Serial.print("Try Connect to : ");
      Serial.print("Beyond Pool-1/" + String(host5) );
      //Serial.print(host6);
      Serial.print("/");
      Serial.println(port);
    }
    if (!client.connected()) {
      client.connect(host5, port7);
//      host = host4;
      port = port7;
      Serial.print("Try Connect to : ");
      Serial.print("Beyond Pool-1/" + String(host5) );
      //Serial.print(host7);
      Serial.print("/");
      Serial.println(port);
    }
    if (!client.connected()) {
      client.connect(host5, port8);
//      host = host4;
      port = port8;
      Serial.print("Try Connect to : ");
      Serial.print("Beyond Pool-1/" + String(host5) );
      //Serial.print(host8);
      Serial.print("/");
      Serial.println(port);
    }   
  }

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

void setup() {
  EEPROM.begin(512);
  
  // Start serial connection
  Serial.begin(115200);
 
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Blinking LED 
  digitalWrite(LED_BUILTIN,HIGH);
  
  delay(3000);

  //connect_wifi();
  Connect2WiFi();

  // EEPROM Setup ////////////////////////
  Serial.println("");
  Serial.println("------------------Get EEPROM Setting------------------");
  GetAcctName();
  GetBoardName();
  Serial.println("------------------Get EEPROM Setting------------------");

  Serial.println("");
  Serial.println("------------------Compair Setting------------------");

  Serial.print("AcctName :");
  Serial.print(eeprom_acct); 
  Serial.print(" <=> ");
  Serial.println(AcctName);
  Serial.println("------------------Compair Setting------------------");
   
  //ทำการตรวจสอบ Account
  if (String(eeprom_acct).equals(String(AcctName))){
    // อ่าน AcctNm
    GetAcctName();   
    // อ่าน BrdNm
    GetBoardName();

  }else{
    
    Serial.println("Writting config to New board.");
    Serial.print("With : ");
    Serial.print(AcctName);
    Serial.print("/");
    Serial.println(BrdNm);
    
    SaveName();
    Serial.println("------------ Done ------------");
  }
 
  Serial.println(" ");
  Serial.print("2-Hello my name is " );
  Serial.print(USERNAME);
  Serial.print("/");
  Serial.print(RIG_IDENTIFIER);
  Serial.println("");
 
  Serial.println("\nDuino-Coin ESP8266 Miner v2.55");

//  SetupWifi();
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

  //Check On start up
  Serial.println(" ");
  Serial.println("Firmware Check on Starting..");
  Serial.println("Current Firmware : " + FirmwareVer);
  
  Serial.println(" ");
  Serial.println("OTI. Check..");
  FirmwareUpdate();

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
  
  Serial.println("Asking for a new job for user: " + String(USERNAME)+ "/" + String(RIG_IDENTIFIER)+ "/" + String(port));
  //Serial.println("Asking for a new job for user: " + String(USERNAME)+ "/" + String(RIG_IDENTIFIER)+ "/" + (String(host)) + "/" + String(port));
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
  delay(200);

  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(200);

  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(200);

  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(200);

  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(200);

  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
  delay(200);
  
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
      delay(200);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(200);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(200);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(200);
      
      digitalWrite(LED_BUILTIN,HIGH); 

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

////// EEPROM Setup ////////
void GetAcctName(){
//  Serial.println("");
//  Serial.println("------------------Get Account Name ------------------"); 
  // อ่าน AcctName
  eeprom_acct = "";
  for(int i=0;i< 20;i++){ 
//    Serial.print(i);
//    Serial.print(" GAcctNm ");
//    Serial.println(char(EEPROM.read(i))); 
  
    if(EEPROM.read(i) > 20)eeprom_acct += char(EEPROM.read(i));
  }  

  USERNAME = eeprom_acct;
  Serial.println(USERNAME); 
 
}

void GetBoardName(){
  // อ่าน BrdNm
  eeprom_brd = "";
  for(int i=20;i< 40;i++){
   if(EEPROM.read(i) > 0){
//      Serial.print(i);
//      Serial.print(" GBrdNm ");
//      Serial.println(char(EEPROM.read(i)));
    
      eeprom_brd += char(EEPROM.read(i));
   }
  }

  RIG_IDENTIFIER = eeprom_brd;
  Serial.println(RIG_IDENTIFIER); 
}

void SaveName(){  
  Serial.println("");
  Serial.println("------------------Start Save ------------------");
  // CLEAR EEPROM /////////
  for(int i=0;i < 20;i++)
  {
     EEPROM.write(i, 0);
  }
 
  Serial.print("AcctName : ");
  Serial.println(AcctName);  

  // เขียน AcctName
  for(int i=0;i < AcctName.length();i++)
  {
//    Serial.print(i);
//    Serial.print(" SvAcctNm ");
//    Serial.println(AcctName[i]);
    
    EEPROM.write(i, AcctName[i]); 
  }
  USERNAME = AcctName;

  // เคลียร์พื้นที่ก่อน เขียน BrdNm
  for(int i=20;i < 40;i++)
  {
   EEPROM.write(i, 0);
  }

  // เขียน BrdNm
//  Serial.println("------------------Show : BrdNm ------------------"); 
  Serial.print("BoardName : ");
  Serial.println(BrdNm); 
  
  x = 0;
  for(int i=20;i < 40;i++)
  {
//    Serial.print(i);
//    Serial.print(" SvBrdNm ");
//    Serial.println(BrdNm[x]); 

    if(x < BrdNm.length())EEPROM.write(i, BrdNm[x]); //Write one by one with starting address of 0x0F
    x++;
  }
  RIG_IDENTIFIER = BrdNm;
  
  EEPROM.commit();
   
  Serial.println("------------------End Save ------------------");
}

////// ตั้งค่า 2 WiFi //////
void Connect2WiFi()
{
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.mode(WIFI_OFF);
      
      //Blinking
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(250);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(250);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(250);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(250);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(250);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(250);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(250);
    
      digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
      delay(250);

      digitalWrite(LED_BUILTIN,HIGH);
    
//      Serial.print("O");
      
      //delay(250);
      
      WiFi.mode(WIFI_STA);
      if (lastWiFi == 1) {
        Serial.print("Connecting WiFi#2 :");
        Serial.println(ssid2);
        WiFi.begin(ssid2, password2);
        
        lastWiFi = 2;
        ssidNm = ssid2;   
    
      } else {        
        Serial.print("Connecting WiFi#1 :");
        //WiFi.begin(ssid1, password1);
        
        Serial.println("Start Connect to WiFi"); 
        Serial.println(ssid1);
        WiFi.begin(ssid1, password1);
        
        lastWiFi = 1;
        ssidNm = ssid1;  
      } 
    
      unsigned long timeout = millis();
      while (WiFi.status() != WL_CONNECTED) {
         if (millis() - timeout > 20000) {
            Serial.println("");
            Serial.println(">>> Connecting Wait Timeout !");
            break;
         }
//         delay(500);
         Serial.print(".");
         digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN)));
         delay(500);
         
         digitalWrite(LED_BUILTIN,HIGH);
      }
    }
    Serial.println("");
    Serial.print("Connected to WIFi: ");
//    Serial.print("SSID: ");
    Serial.println(ssidNm);
    Serial.print("IP address: ");
    Serial.print(WiFi.localIP());     
    Serial.println(""); 

    delay(5000);
}
