#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>

// ===== WIFI =====
const char* ssid = "Nothing Phone (1)";
const char* password = "maheshphand";

// ===== MQTT =====
const char* mqtt_server = "dev.coppercloud.in";
const char* subTopic = "mahesh/dashboard";

// ===== OLED =====
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
// If not visible, try:
// U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

WiFiClient espClient;
PubSubClient client(espClient);

// ===== LED =====
#define LED 2

String state = "WAIT";
float risk = 0;
float person = 0;
int ir = 0;
int distance = 0;
int gas = 0;

// ===== MQTT Callback =====
void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<512> doc;
  deserializeJson(doc, payload, length);

  state = doc["state"].as<String>();
  risk = doc["risk"];
  person = doc["vision"]["person_confidence"];
  ir = doc["sensors"]["ir_presence"];
  distance = doc["sensors"]["distance"];
  gas = doc["sensors"]["gas"];

  // LED control
  if (state == "ALARM") digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW);
}

// ===== MQTT Connect =====
void reconnect() {
  while (!client.connected()) {
    client.connect("ESP32_OLED");
    client.subscribe(subTopic);
  }
}

// ===== Setup =====
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  u8g2.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ===== Loop =====
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);

  u8g2.drawStr(0, 12, "AI SECURITY DASH");
  u8g2.drawStr(0, 26, ("State: " + state).c_str());

  char line1[32];
  sprintf(line1, "Risk: %.2f", risk);
  u8g2.drawStr(0, 38, line1);

  char line2[32];
  sprintf(line2, "Person: %.2f", person);
  u8g2.drawStr(0, 50, line2);

  char line3[32];
  sprintf(line3, "IR:%d Dist:%d", ir, distance);
  u8g2.drawStr(0, 62, line3);

  u8g2.sendBuffer();
}
