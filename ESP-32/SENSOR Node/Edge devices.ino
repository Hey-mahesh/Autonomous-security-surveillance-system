#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// ======= WIFI =======
const char* ssid = "Nothing Phone (1)";
const char* password = "maheshphand";

// ======= MQTT =======
const char* mqtt_server = "dev.coppercloud.in";
const char* pubTopic = "mahesh/esp32/sensors";
const char* subTopic = "mahesh/esp32/command";

WiFiClient espClient;
PubSubClient client(espClient);

// ======= PINS =======
#define LDR 35
#define MQ2 34
#define IR 2
#define TRIG 5
#define ECHO 18
#define BUZZER 26
#define LED 25

// ======= Ultrasonic =======
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

// ======= MQTT Callback =======
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  if (msg.indexOf("ON") >= 0) {
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
  }
}

// ======= Connect MQTT =======
void reconnect() {
  while (!client.connected()) {
    client.connect("ESP32_Mahesh");
    client.subscribe(subTopic);
  }
}

// ======= SETUP =======
void setup() {
  Serial.begin(115200);

  pinMode(IR, INPUT_PULLUP);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// ======= LOOP =======
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  int ldr = analogRead(LDR);
  int gas = analogRead(MQ2);
  int ir_presence = !digitalRead(IR);   // inverted
  long distance = getDistance();

  bool isDark = ldr > 1500;

  StaticJsonDocument<256> doc;
  doc["ldr"] = ldr;
  doc["is_dark"] = isDark;
  doc["ir_presence"] = ir_presence;
  doc["distance"] = distance;
  doc["gas"] = gas;

  char buffer[256];
  serializeJson(doc, buffer);

  client.publish(pubTopic, buffer);

  Serial.println(buffer);
  delay(1000);
}
