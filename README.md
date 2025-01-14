# Room Sensor Modul

## Description

The ESP32 in this project is an ESP32C3. This is a small module with USB connector.
The Sensors is a BME280 for Temperatur, pressure and humidity.
It's Webinterface and Modbus available. 
The Modbus-server has 7 Register:
- Register 0, read, temperatur
- Register 1, read, pressure
- Register 2, read, humidity
- Register 3, read, altitude
- Register 4, read, voltage (batterie)
- Register 10, read/write, Set "True" from client: enable Deep-Sleep-Modus for xx sec
- Register 11, read/write, Deepsleep-Time in sec
- Register 15, read/write, calibraton value temperature
- Register 16, read/write, calibraton value pressure
- Register 17, read/write, calibraton value humidity

The 12 Volt is reduced 10-26 Volt to 5 Volt with a DC Step-Down_Converter for connect to wired power. 
Also is a Batterie inserted in the housing and measurement for this voltage.
After configuration with the Webinterface can you with modbus switch to Deepsleep-Modus.

The Website use LittleFS Filesystem. You must use Partition Schemes "Minimal SPIFFS with APPS and OTA".
The HTML Data upload separately.

Modbus is tested with PLC S7-1214 TCP Client and KTP400

## Partlist:

- Espressiv ESP32-C3-DevkitM1 [Link](https://www.reichelt.de/entwicklungsboard-esp32-c3-mini-1-esp32c3devkitm-p311730.html?&nbc=1)
- BME 280 [Link](https://www.reichelt.de/entwicklerboards-temperatur-feuchtigkeits-und-drucksensor--debo-bme280-p253982.html?&nbc=1)
- Traco-Power TSR 1-2450 for 12V / 5V [Link](https://www.reichelt.de/dc-dc-wandler-tsr-1-1-w-5-v-1000-ma-sil-to-220-tsr-1-2450-p116850.html?search=tsr+1-24)
- Housing [Link](https://www.reichelt.de/sensorgehaeuse-74x74x25-5-mm-belueftet-weiss-cb-rs02vwh-p317699.html?&nbc=1)
- Batterieholder [Link](https://www.reichelt.de/batteriehalter-fuer-1-18350-keystone-1095p-p213366.html?&nbc=1)
- Akku [Link](https://www.reichelt.de/li-ion-akku-cr123-3-6-v-850-mah-usb-c-xcell-148203-p366682.html?&nbc=1)

## Wiring diagram

![grafik](https://github.com/user-attachments/assets/d2bd1a8f-84b5-4373-a100-b24541ba1129)

## PCB and Housing assembly
![image](https://github.com/user-attachments/assets/1f1e8f95-4026-45f8-b86d-f63d5b3542ee)

PCB by Aisler [Link](https://aisler.net/p/JVQKQVUK)

## Webinterface

![image](https://github.com/user-attachments/assets/b9d54b22-bd9d-4a3b-9983-1489b16eb7a9)
![image](https://github.com/user-attachments/assets/f0e8f621-3172-453b-bf13-41479121e3d0)
![image](https://github.com/user-attachments/assets/5a74517a-ba20-4394-a14d-4751606eade1)
![image](https://github.com/user-attachments/assets/439c5168-e7c4-43c5-b4e7-7ae0b0f7ac12)
![image](https://github.com/user-attachments/assets/0ed3708e-595e-4d6e-a7cb-f49c0f9cf3de)


## S7 Connection

![grafik](https://github.com/user-attachments/assets/8d18e7bc-5492-497c-9e7f-ebbfbf8c76eb)


## Versions

- 1.4 add calibration Register
- 1.3 Update Settings (failure BMP Type)
- 1.2 add Modbusregister to switch DeepSleepModus on and WiFiScan off
- 1.1 add batterie voltage measurement
- 1.0 working Version

