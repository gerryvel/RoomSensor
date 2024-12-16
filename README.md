#Room Sensor Modul

The ESP32 in this project is an ESP32C3. This is a small module with USB connector.
The Sensors is a BME280 for Temperatur, pressure and humidity.
It's Webinterface and Modbus available. 
The Modbus-server has 5 Register:
- temperatur
- pressure
- humidity
- altitude
- voltage (batterie)

The 12 Volt is reduced 10-26 Volt to 5 Volt with a DC Step-Down_Converter for connect to wired power. 
Also is a Batterie inserted in the housing and measurement for this voltage.

The Website use LittleFS Filesystem. You must use Partition Schemes "Minimal SPIFFS with APPS and OTA".
The HTML Data upload separately.

# Partlist:

- Espressiv ESP32-C3-DevkitM1
- BME 280
- Traco-Power TSR 1-2450 for 12V / 5V [Link](https://www.reichelt.de/dc-dc-wandler-tsr-1-1-w-5-v-1000-ma-sil-to-220-tsr-1-2450-p116850.html?search=tsr+1-24)


# Wiring diagram



# PCB and Housing assembly


# Webinterface

<img width="1080" alt="image" src="https://github.com/user-attachments/assets/b9d54b22-bd9d-4a3b-9983-1489b16eb7a9" />
<img width="1080" alt="image" src="https://github.com/user-attachments/assets/76e7ed92-e2f1-4aca-aae2-e641b5249f7e" />
<img width="1080" alt="image" src="https://github.com/user-attachments/assets/a76b5d52-b510-472e-954c-a52bf90fcb06" />
<img width="1080" alt="image" src="https://github.com/user-attachments/assets/5fa71013-38e1-4a74-ab89-2a668cdcb6f8" />


# Versions

- 1.0 working Version
