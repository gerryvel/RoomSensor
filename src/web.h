#ifndef __WEB__H__
#define __WEB__H__

#include <arduino.h>
#include "BoardInfo.h"
#include "configuration.h"
#include <ESPAsyncWebServer.h>
#include "helper.h"

// Set web server port number to 80
AsyncWebServer server(80);
AsyncEventSource events("/events");

// Info Board for HTML-Output
BoardInfo boardInfo;
String sBoardInfo;
bool IsRebootRequired = false;


String processor(const String& var)
{
	if (var == "CONFIGPLACEHOLDER")
	{
		String buttons = "";
		buttons += "<form onSubmit = \"event.preventDefault(); formToJson(this);\">";
		buttons += "<p class=\"CInput\"><label>SSID </label><input type = \"text\" name = \"SSID\" value=\"";
		buttons += tWeb_Config.wAP_SSID;
		buttons += "\"/></p>";
		buttons += "<p class=\"CInput\"><label>Password </label><input type = \"text\" name = \"Password\" value=\"";
		buttons += tWeb_Config.wAP_Password;
		buttons += "\"/></p>";
		buttons += "<p class=\"CInput\"><label>Sensortyp </label><input type = \"text\" name = \"BMP\" value=\"";
		buttons += tWeb_Config.wBMP_Sensortype;
		buttons += "\"/></p>";
		buttons += "<p class=\"CInput\"><label>Client IP </label><input type = \"text\" name = \"Client_IP\" value=\"";
		buttons += tWeb_Config.wClient_IP;
		buttons += "\"/></p>";
		buttons += "<p class=\"CInput\"><label>Client Gateway </label><input type = \"text\" name = \"Client_Gateway\" value=\"";
		buttons += tWeb_Config.wClient_Gateway;
		buttons += "\"/></p>";
		buttons += "<p class=\"CInput\"><label>Client Netmask </label><input type = \"text\" name = \"Client_NMask\" value=\"";
		buttons += tWeb_Config.wClient_NMask;
		buttons += "\"/></p>";
		buttons += "<p class=\"CInput\"><label>Client DNS </label><input type = \"text\" name = \"Client_DNS\" value=\"";
		buttons += tWeb_Config.wClient_DNS;
		buttons += "\"/></p>";
		buttons += "<p><input type=\"submit\" value=\"Speichern\"></p>";
		buttons += "</form>";
		return buttons;
	}
	return String();
}

//Variables for website
String sCL_Status = sWifiStatus(WiFi.status());
String replaceVariable(const String& var){
	if (var == "sHum")return String(fbmx_humidity, 1);
	if (var == "sAlt")return String(fbmx_altitude, 1);
	if (var == "sTemp")return String(fbmx_temperature, 1);
  	if (var == "sPress")return String(fbmx_pressure/100, 0);
	if (var == "sBoardInfo")return sBoardInfo;
  	if (var == "sFS_USpace")return String(LittleFS.usedBytes());
	if (var == "sFS_TSpace")return String(LittleFS.totalBytes());
	if (var == "sAP_IP")return WiFi.softAPIP().toString();
  	if (var == "sAP_Clients")return String(WiFi.softAPgetStationNum());
 	if (var == "sCL_Addr")return WiFi.localIP().toString();	
	if (var == "sCL_Subnet")return WiFi.subnetMask().toString();
	if (var == "sCL_Gateway")return WiFi.gatewayIP().toString();	
 	if (var == "sCL_Status")return String(sCL_Status);
  	if (var == "sI2C_Status")return String(sI2C_Status);
  	if (var == "sBMP_Status")return String(sBMP_Status);
  	if (var == "sCL_SSID")return String(CL_SSID);
  	if (var == "sCL_PASSWORD")return String(CL_PASSWORD);
	if (var == "sBMP")return String(sBMP);
	if (var == "sModbusOffset")return String(Sensor_HREG);
	if (var == "sReg0")return String(RegVal0);
	if (var == "sReg1")return String(RegVal1);
	if (var == "sReg2")return String(RegVal2);
	if (var == "sReg3")return String(RegVal3);
	if (var == "sReg4")return String(RegVal4);
	if (var == "sVersion")return Version;
  	if (var == "CONFIGPLACEHOLDER")return processor(var);
  return "NoVariable";
}

void website(){
server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/favicon.ico", "image/x-icon");
	});
server.on("/logo80.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/logo80.jpg", "image/jpg");
	});
server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(LittleFS, "/index.html", String(), false, replaceVariable);
	});
server.on("/system.html", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(LittleFS, "/system.html", String(), false, replaceVariable);
	});
server.on("/modbus.html", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(LittleFS, "/modbus.html", String(), false, replaceVariable);
	});
server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(LittleFS, "/settings.html", String(), false, replaceVariable);
	});
server.on("/ueber.html", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(LittleFS, "/ueber.html", String(), false, replaceVariable);
	});
server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest * request) {
    	request->send(LittleFS, "/reboot.html", String(), false, processor);
		IsRebootRequired = true;
	});
server.on("/gauge.min.js", HTTP_GET, [](AsyncWebServerRequest* request) {
		request->send(LittleFS, "/gauge.min.js");
	});
server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(LittleFS, "/style.css", "text/css");
	});
server.on("/settings.html", HTTP_POST, [](AsyncWebServerRequest *request)
	{
		int count = request->params();
		Serial.printf("Anzahl: %i\n", count);
		for (int i = 0;i < count;i++)
		{
			AsyncWebParameter* p = request->getParam(i);
			Serial.print("PWerte von der Internet - Seite: ");
			Serial.print("Param name: ");
			Serial.println(p->name());
			Serial.print("Param value: ");
			Serial.println(p->value());
			Serial.println("------");
			// p->value in die config schreiben
			writeConfig(p->value());
		}
		request->send(200, "text/plain", "Daten gespeichert");
	});
}

void WebReboot(){
	if (IsRebootRequired) {
		Serial.println("Rebooting ESP32: "); 
		delay(1000); // give time for reboot page to load
		ESP.restart();
		}
}

#endif