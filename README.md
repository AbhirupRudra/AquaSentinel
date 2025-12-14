# 🌊 AquaSentinel – Smart Water Quality Monitoring System (ESP8266 + Blynk)

AquaSentinel is an **IoT-based smart water quality monitoring system** built using **ESP8266** and **Blynk IoT**. It continuously monitors **water purity (TDS)** and **temperature**, classifies water safety in real time, and sends live updates and alerts to a mobile dashboard.

---

## 🚀 Features

- ✅ Real-time **TDS (Total Dissolved Solids) monitoring**
- 🌡 Accurate **DS18B20 temperature sensing**
- 📐 **Temperature-compensated TDS calculation** for reliable results
- 🔴🟢 **LED status indication**
  - Green → Safe Water  
  - Red → Unsafe Water
- 📱 **Blynk IoT integration**
  - Live TDS & temperature values
  - Water status label (Safe / Impure / Hot / Cold)
  - Instant cloud notifications
- ⚡ Lightweight, low-cost, and power efficient
- 🧩 Modular design (easy to add pH, turbidity, relay, etc.)

---

## 🧠 Working Principle

1. The TDS sensor measures the electrical conductivity of water.
2. The DS18B20 sensor reads water temperature.
3. Temperature compensation is applied to the TDS value.
4. The system compares values with predefined safety thresholds.
5. Water is classified as:
   - **Safe Water**
   - **Impure Water**
   - **Hot Water**
   - **Cold Water**
6. Status is shown using LEDs and sent to the **Blynk mobile app**.

---

## 🛠 Hardware Used

- ESP8266 (NodeMCU)
- TDS Sensor Module
- DS18B20 Temperature Sensor (with 4.7kΩ pull-up)
- Red & Green LEDs
- Resistors, jumper wires, breadboard

---

## 📲 Blynk Dashboard Setup

| Virtual Pin | Usage |
|------------|------|
| V0 | TDS Value (ppm) |
| V1 | Temperature (°C) |
| V2 | Water Status Label |

Use **Label widgets** for text display and enable **Events** in Blynk Cloud for alerts.

---

## 📌 Applications

- Household drinking water monitoring
- Hostel & apartment water lines
- Educational IoT & embedded projects
- Smart infrastructure & water safety systems
- Swachh Bharat Mission aligned projects

---

## 🌱 Future Enhancements

- pH sensor integration
- Turbidity measurement
- Automatic relay-based water cutoff
- Data logging (SD card / cloud)
- Web dashboard & analytics

---

## 🧑‍💻 Author

Developed by **Abhirup Rudra**  
Electronics & IoT Enthusiast | Embedded Systems | ESP8266 Projects

---

## ⭐ Support

If you find this project useful:
- 🌟 Star this repository
- 🍴 Fork and improve it
- 📢 Share with fellow engineers

---

## 🔎 GitHub Topics

`esp8266` `iot` `water-quality` `tds-sensor` `blynk` `smart-water` `embedded-systems` `swachh-bharat`
