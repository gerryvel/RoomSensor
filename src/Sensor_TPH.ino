/*
  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  NMEA2000 Temperature and Barometric Pressure with BMP280 or 388.
  Reads messages from NMEA0183 WindSensor and forwards them to the N2k bus.

  V1.2 vom 16.12.2024, gerryvel Gerry Sebb
*/

#include <Arduino.h>
#include "BoardInfo.h"
#include "configuration.h"
#include "helper.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BME280.h>
#include <wire.h>
#include <Preferences.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "LED.h"
#include <LittleFS.h>
#include <arpa/inet.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h> 
#include "web.h"
#include "analog.h"
#include <ModbusIP_ESP8266.h>
#include "esp_sleep.h"

// BMP
Adafruit_BMP280 bmp280;
Adafruit_BMP3XX bmp3xx;
Adafruit_BME280 bme280;

//ModbusIP object
ModbusIP mb;

//================================== SETUP ==============================//
void setup()
{
  Serial.begin(115200);

  Serial.printf("Room Sensor setup %s start\n", Version);

// Set the CPU frequency to 80 MHz for consumption optimization
  setCpuFrequencyMhz(80);
  uint16_t Freq = getCpuFrequencyMhz();
  Serial.print("CPU Freq = ");
  Serial.print(Freq);
  Serial.println(" MHz");

//Filesystem
	if (!LittleFS.begin(true)) {
		Serial.println("An Error has occurred while mounting LittleFS");
		return;
	}
	Serial.println("Memory LittleFS used:");
	Serial.println(LittleFS.usedBytes());
	File root = LittleFS.open("/");
  listDir(LittleFS, "/", 3);
  readConfig("/config.json");
  // read connectdata
	CL_SSID = tWeb_Config.wAP_SSID;               
	CL_PASSWORD = tWeb_Config.wAP_Password;
  //read sensortype
  Sensortyp = tWeb_Config.wBMP_Sensortype; 
  // read client data
  CL_IP = inet_addr(tWeb_Config.wClient_IP);
  CL_Gateway = inet_addr(tWeb_Config.wClient_Gateway);
  CL_NMask = inet_addr(tWeb_Config.wClient_NMask);
  CL_DNS = inet_addr(tWeb_Config.wClient_DNS);
  Serial.println("Configdata:\n Client SSID: " + CL_SSID + ", Passwort: " + CL_PASSWORD + ", Sensortyp: " + Sensortyp);
  Serial.println("Clientdata:\n Client IP: " + CL_IP.toString() + ", Netmask: " + CL_NMask.toString() + ", Gateway: " + CL_Gateway.toString() + ", DNS: " + CL_DNS.toString());

  freeHeapSpace();

// I2C
  Wire.begin(BMP_SDA, BMP_SCL);  // BMP_SDA, BMP_SCL
  I2C_scan();

// Boardinfo	
  sBoardInfo = boardInfo.ShowChipIDtoString();

//Wifi
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPdisconnect();
  if(WiFi.softAP(AP_SSID, AP_PASSWORD, channel, hide_SSID, max_connection)){
    WiFi.softAPConfig(IP, Gateway, NMask);
    Serial.println("");
    Serial.println("\nAccessspoint " + String(AP_SSID) + " running");
    Serial.println("\nAccessspiont IP " + IP.toString() + ", GW: " + Gateway.toString() + ", Mask: " + NMask.toString() + " set");
    delay(1000);
  } else {
      Serial.println("Starting AP failed.");
      delay(1000); 
      ESP.restart();
  }
  
  WiFi.hostname(HostName);
  Serial.println("Set Hostname done");

  if (mb.onGetHreg(105) == 0){
        WiFiDiag();
  }

// Anmelden mit WiFi als Client 
Serial.println("Client connection");
  WiFi.disconnect(true);
  delay(1000);   

  int count = 0; // Init Counter WFIConnect  
  Serial.println("\nClient " + String(CL_SSID) + ", " + String(CL_PASSWORD) + " connect");

WiFi.begin((const char*)CL_SSID.c_str(), (const char*)CL_PASSWORD.c_str());
  while (WiFi.status() != WL_CONNECTED)
  {   
    LEDboard(Blue);
    delay(500);
    Serial.print(".");
    neopixelWrite(LED_BUILTIN,0,0,0);
   count++;
    if (count = 10) break;
  }
  if (WiFi.isConnected()) {
   bConnect_CL = 1;
    Serial.println("Client connected");
  }
  else
    Serial.println("Client connection failed");
    //LEDoff(LED(Blue));
    WiFi.reconnect(); 

  if (WiFi.config(CL_IP, CL_Gateway, CL_NMask, CL_DNS) == false) {
    Serial.println("Client configuration failed.");
  } else {
    Serial.println("Client configured with IP:" + CL_IP.toString() + ", Netmask:" + CL_NMask.toString() + ", Gateway:" + CL_Gateway.toString() + ", DNS:" + CL_DNS.toString());
  }
  delay(500);    
  
// Start OTA
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

// Start TCP (HTTP) server
	server.begin();
	Serial.println("TCP server started");


// BMP begin & setup
  if (Sensortyp == 0){
      sBMP = "BMP280";
    if (!bmp280.begin()) 
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  }
  if (Sensortyp == 1){
    sBMP = "BME280";
    if(!bme280.begin(0x76)){
    Serial.println("Could not find a valid BME280 sensor, check wiring!");  
    }
  }
  if (Sensortyp == 2){
      sBMP = "BMP388";
    if (!bmp3xx.begin_I2C()) 
  {
    Serial.println("Could not find a valid BMP3xx sensor, check wiring!");
  }
  bmp3xx.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp3xx.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp3xx.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp3xx.setOutputDataRate(BMP3_ODR_50_HZ); 
  }

  mb.server();
  mb.addHreg(Sensor_HREG, 0, 20);
  mb.addHreg(105, 0);
  mb.addHreg(106, 60);

// MDNS
  MDNSResponder mdns;
  MDNS.begin(HostName);
  MDNS.addService("http", "tcp", 80);

  website();
  
}

//=================================== Loop ==============================//
void loop()
{ 
  // Connect to wlan
  bConnect_CL = WiFi.status() == WL_CONNECTED ? 1 : 0;
  Serial.printf("WLAN Status: %s \n", sWifiStatus(WiFi.status()));
  Serial.printf("bConnect Client: %i \n", bConnect_CL);
  { // Listen DB-Bereich?
    if (bConnect_CL == 1){ // Connected an listen
      Serial.printf("Wifi %s connencted!\n", CL_SSID);
      CL_NMask = WiFi.subnetMask();
      CL_Gateway = WiFi.gatewayIP();
      CL_DNS = WiFi.dnsIP();
      delay(100);
    }
    else{
      Serial.println("Wifi connect failed!\n");
      bConnect_CL = 0;
      Serial.printf("Reconnecting to %s\n", CL_SSID);
      WiFi.reconnect();    // wifi down, reconnect here
      delay(500);
        int UpCount = 0;
        int WLCount = 0;
      while (WiFi.status() != WL_CONNECTED && WLCount < 50){
        delay(50);
        Serial.printf(".");
        flashLED(Red, 5);
        if (UpCount >= 20)  // just keep terminal from scrolling sideways
        {
          UpCount = 0;
          Serial.printf("\n");
        }
        ++UpCount;
        ++WLCount;
      }
    }
  }
  
ArduinoOTA.handle();

// Status AP 
  Serial.printf("Soft-AP IP address   : %s\n", WiFi.softAPIP().toString());
  Serial.printf("Client IP address    : %s\n", WiFi.localIP().toString());
  Serial.printf("Client Subnet Mask   : %s\n", WiFi.subnetMask().toString());
  Serial.printf("Client Gateway adress: %s\n", WiFi.gatewayIP().toString());
  sCL_Status = sWifiStatus(WiFi.status());
  

 delay(500);

 //Read BMP  
  if (Sensortyp == 0)
  {
    Serial.print("Read BMP 280\n");
    sBMP_Status = "BMP lesen erfolgreich";
    fbmx_temperature = bmp280.readTemperature();
    fbmx_pressure = bmp280.readPressure();
    fbmx_altitude = bmp280.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.printf("Temperatur: %3.1f °C\n", fbmx_temperature);
    Serial.printf("Luftdruck : %4.1f mbar\n", fbmx_pressure);
    Serial.printf("Höhe      : %4.1f m\n", fbmx_altitude);
  }
  if (Sensortyp == 1)
  {
    Serial.print("Read BME 280\n");
    sBMP_Status = "BME lesen erfolgreich";
    fbmx_temperature = bme280.readTemperature();
    fbmx_pressure = bme280.readPressure();
    fbmx_humidity = bme280.readHumidity();
    fbmx_altitude = bme280.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.printf("Temperatur  : %3.1f °C\n", fbmx_temperature);
    Serial.printf("Luftdruck   : %4.1f mbar\n", fbmx_pressure/100);
    Serial.printf("Luftfeuchte : %4.1f %%\n", fbmx_humidity);
    Serial.printf("Höhe        : %5.1f m\n", fbmx_altitude);
  }

  if (Sensortyp == 2)
  {
    if (!bmp3xx.performReading()) 
    {       
      Serial.println("BMP 3xx reading error");
      sBMP_Status = "BMP Lesefehler";
      flashLED(Red, 2);
    } else 
    {
      Serial.println("BMP 3xx read successful");
      sBMP_Status = "BMP lesen erfolgreich";
      fbmx_pressure = bmp3xx.readPressure();
      fbmx_temperature = bmp3xx.readTemperature();
      fbmx_altitude = bmp3xx.readAltitude(SEALEVELPRESSURE_HPA);
      delay(100);
    }
  }

WebReboot();  

BoardSpannung = ((BoardSpannung * 15) + (ReadVoltage(ADCpin1) * ADC_Calibration_Value1 / 4096)) / 16; // This implements a low pass filter to eliminate spike for ADC readings
Serial.printf("Spannung    : %3.2f V\n", BoardSpannung);

mb.task();
   delay(10);

  RegVal0 = fbmx_temperature*10;
  RegVal1 = fbmx_pressure/100;
  RegVal2 = fbmx_humidity*10;
  RegVal3 = fbmx_altitude;
  RegVal4 = BoardSpannung*100;

  mb.Hreg(100, RegVal0);   // Value in xx,x °C 
  mb.Hreg(101, RegVal1);   // Value in xxxx mbar
  mb.Hreg(102, RegVal2);   // Value in xx.x %
  mb.Hreg(103, RegVal3);   // Value in xxxx m
  mb.Hreg(104, RegVal4);   // Value in xx.xx V
  SleepOn = mb.Hreg(105);
  SleepT = mb.Hreg(106);

delay(100);

// Sleep start, mb.Reg5 must be true !
  Serial.println("\nRead Register 105 : " + String(mb.Hreg(105)));
  Serial.println("Read Register 106 : " + String(mb.Hreg(106)));
  Sleeptime = SleepT * 1000000;   // sec to millisec
  Serial.println("\nTimer: " + String(Sleeptime));
  Serial.printf("Modbus Signal for Deep-Sleep is %i \n", SleepOn);

// Time for read registers from modbus client befor deepsleep
if (UpCount >= 10 && SleepOn == 0){     
      UpCount = 0;
        Serial.println("\nGo to Deep-Sleep-Mode for " + String(Sleeptime/1000000) + " seconds\n");
        esp_sleep_enable_timer_wakeup(Sleeptime); 
        esp_deep_sleep_start();
    } else {
        ++UpCount;
  }
       
  Serial.println("\nCounter: " + String(UpCount));


}
