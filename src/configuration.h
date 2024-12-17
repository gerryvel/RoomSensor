#ifndef __configuration__H__
#define __configuration__H__

#include <Arduino.h>

// Versionierung
#define Version "V1.2 vom 17.12.2024"  // Version

//Configuration Web Page 
#define PAGE_REFRESH 10 // x Sec.
#define WEB_TITEL "Sensor TPH"

//Configuration mit Webinterface
struct Web_Config
{
	char wAP_SSID[64];
	char wAP_Password[12];
	char wBMP_Sensortype[1];
	char wClient_IP[16];
	char wClient_NMask[16];
	char wClient_Gateway[16];
	char wClient_DNS[16];
};
Web_Config tWeb_Config;

//Configuration AP 
#define HostName        "SensorTPH"
#define AP_SSID         "SensorTPH"  // SSID Name
#define AP_PASSWORD     "12345678"    // SSID Password - Set to NULL to have an open AP
const int   channel        = 10;                // WiFi Channel number between 1 and 13
const bool  hide_SSID      = false;             // To disable SSID broadcast -> SSID will not appear in a basic WiFi scan
const int   max_connection = 4;                 // Maximum simultaneous connected clients on the AP

// Variables for WIFI-AP
IPAddress IP = IPAddress(192, 168, 0, 180);
IPAddress Gateway = IPAddress(192, 168, 0, 254);
IPAddress NMask = IPAddress(255, 255, 255, 0);
IPAddress AP_IP;
IPAddress CL_IP = IPAddress();
IPAddress CL_NMask = IPAddress();
IPAddress CL_Gateway = IPAddress();
IPAddress CL_DNS = IPAddress();

//Configuration Client (Network Data Windsensor)
//#define 
String CL_SSID = "";					// Standard NoWa 			
String CL_PASSWORD = "";				// Standard 12345678

int iSTA_on = 0;                            // Status STA-Mode
int bConnect_CL = 0;
bool bClientConnected = 0;

//Confuration I2C
#define BMP_SDA 6                      //Standard 21
#define BMP_SCL 7                      //Standard 22

// Configuratin Sensors
#define SEALEVELPRESSURE_HPA (1013.25)  //1013.25
bool Sensortyp = 0;             // BMP280 = 0, BME280 = 1, BME3xx = 2
float fbmx_temperature = 0;
float fbmx_pressure = 0;
float fbmx_altitude = 0;
float fbmx_humidity = 0;

String sBMP_Status = "";
String sI2C_Status = "";
String sBMP = "";

//Modbus
const int Sensor_HREG = 100;  // Modbus Registers Offsets
int Reg0 = Sensor_HREG + 0;
int Reg1 = Sensor_HREG + 1;
int Reg2 = Sensor_HREG + 2;
int Reg3 = Sensor_HREG + 3;
int Reg4 = Sensor_HREG + 4;
int Reg5 = Sensor_HREG + 5; 	// config	1 = Deepsleep on

int RegVal0 = 0;	// Temperatur
int RegVal1 = 0;	// Pressure
int RegVal2 = 0;	// Hum
int RegVal3 = 0;	// Altitude
int RegVal4 = 0;	// Spannung


// RGB LED buildin
#define LED_PIN     8 //Pin 46 on Thing Plus C S3 is connected to WS2812 LED
#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define NUM_LEDS    1
#define BRIGHTNESS  25

// ADC
const int ADCpin1 = 1; // Voltage measure is connected GPIO 1 (Analog ADC1_1)
float BoardSpannung = 0;
#define ADC_Calibration_Value1 27.0 // The real value depends on the true resistor values for the ADC input (110K / 11 K)

// Deepsleep
int Sleeptime = 60000000;   // 60.000.000 Mikrosekunden = 1 Minute

#endif  