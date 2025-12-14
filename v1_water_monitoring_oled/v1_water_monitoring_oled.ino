#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// -------- Pin Definitions (UPDATED) --------
// SDA = D2, SCL = D1
#define TDS_PIN A0
#define ONE_WIRE_BUS 2
#define FLOW_SENSOR_PIN 14
#define BUZZER_PIN 12

// -------- OLED Setup --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------- DS18B20 Temperature Setup --------
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// -------- Global Variables --------
volatile int flowPulseCount = 0;
float flowRate = 0.0;
unsigned long oldTime = 0;
float tdsValue = 0.0;
float temperatureC = 0.0;

// -------- Thresholds --------
const float TDS_THRESHOLD = 500.0;   // ppm
const float TEMP_MIN = 10.0;         // °C
const float TEMP_MAX = 45.0;         // °C
const float MIN_FLOW = 0.2;          // L/min

// -------- Flow Interrupt --------
void ICACHE_RAM_ATTR flowISR() {
  flowPulseCount++;
}

void setup() {
  Serial.begin(115200);
  sensors.begin();

  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), flowISR, RISING);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
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

void loop() {
  unsigned long currentTime = millis();

  // Calculate flow rate every 1 second
  if (currentTime - oldTime > 1000) {
    detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN));
    flowRate = (flowPulseCount / 7.5);  // pulses/sec ÷ 7.5 = L/min
    flowPulseCount = 0;
    oldTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), flowISR, RISING);
  }

  // Read temperature
  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);

  // Read TDS
  int analogValue = analogRead(TDS_PIN);
  float voltage = analogValue * (3.3 / 1024.0);
  tdsValue = (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage) * 0.5;

  // Check water condition
  bool impure = (tdsValue > TDS_THRESHOLD || temperatureC < TEMP_MIN || temperatureC > TEMP_MAX);
  bool noFlow = (flowRate < MIN_FLOW);

  // Update OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("TDS: "); display.print(tdsValue, 1); display.println(" ppm");
  display.print("Temp: "); display.print(temperatureC, 1); display.println(" C");
  display.print("Flow: "); display.print(flowRate, 2); display.println(" L/min");
  display.println("--------------------");

  if (noFlow) {
    display.println("Status: NO FLOW!");
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (impure) {
    display.println("Status: WATER IMPURE!");
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else {
    display.println("Status: WATER SAFE");
    digitalWrite(BUZZER_PIN, LOW);
  }

  display.display();

  delay(1000);
}
