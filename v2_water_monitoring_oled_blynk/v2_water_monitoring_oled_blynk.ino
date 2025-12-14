#define BLYNK_TEMPLATE_ID "TMPL34Wij8NP8"
#define BLYNK_TEMPLATE_NAME "SMART WATER MONITORING SYSTEM"
#define BLYNK_AUTH_TOKEN "exTA1LS9RRG0T4tB-VtLym15qkXef3Su"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "ABHIRUP_RUDRA";
char pass[] = "12122006";

#define TDS_PIN A0
#define ONE_WIRE_BUS 2
#define RLED_PIN 12
#define GLED_PIN 14
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(128, 64, &Wire, -1);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temperatureC = 0.0;
float tdsValue = 0.0;

const float TDS_THRESHOLD = 500.0;
const float TEMP_MIN = 10.0;
const float TEMP_MAX = 45.0;

void setup() {
  Serial.begin(115200);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Blynk.begin(auth, ssid, pass);

  sensors.begin();

  pinMode(RLED_PIN, OUTPUT);
  digitalWrite(RLED_PIN, LOW);
  pinMode(GLED_PIN, OUTPUT);
  digitalWrite(GLED_PIN, HIGH);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  display.println("Smart Water Monitor");
  display.display();
  delay(1500);
}

inline void ledStatus(const bool safe) {
  digitalWrite(RLED_PIN, safe ? LOW : HIGH);
  digitalWrite(GLED_PIN, !safe ? LOW : HIGH);
}

void loop() {
  Blynk.run();

  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);

  int analogValue = analogRead(TDS_PIN);
  float voltage = analogValue * (3.3 / 1024.0);

  float compensationCoeff = 1 + 0.02 * (temperatureC - 25.0);
  float compensatedVoltage = voltage / compensationCoeff;

  tdsValue = (133.42 * compensatedVoltage * compensatedVoltage * compensatedVoltage - 255.86 * compensatedVoltage * compensatedVoltage + 857.39 * compensatedVoltage) * 0.5;

  String waterStatus;

  bool impure = tdsValue > TDS_THRESHOLD;

  if (impure) {
    waterStatus = "IMPURE WATER";
    ledStatus(false);
    Blynk.logEvent("impure", "⚠ WATER IMPURE DETECTED!");
  }
  else if (temperatureC >= TEMP_MAX) {
    waterStatus = "HOT WATER";
    ledStatus(false);
    Blynk.logEvent("temp", "⚠ WATER TOO HOT TO DRINK!");
  }
  else if (temperatureC <= TEMP_MIN) {
    waterStatus = "COLD WATER";
    ledStatus(false);
    Blynk.logEvent("temp", "⚠ WATER TOO COLD TO DRINK!");
  }
  else {
    waterStatus = "SAFE WATER";
    ledStatus(true);
  }

  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("TDS: ");
  display.print(tdsValue, 1);
  display.println(" ppm");

  display.print("Temp: ");
  display.print(temperatureC, 1);
  display.println(" C");

  display.println("--------------------");
  display.println("Status:");
  display.println(waterStatus);

  display.display();

  Blynk.virtualWrite(V0, tdsValue);
  Blynk.virtualWrite(V1, temperatureC);
  Blynk.virtualWrite(V2, waterStatus);

  delay(300);
}
