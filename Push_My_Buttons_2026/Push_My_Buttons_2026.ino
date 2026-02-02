#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>


//Status LED
#define statRed   19
#define statGreen 21
#define statBlue  22


// -------------------- Wi-Fi settings --------------------
const char* ssid     = "MpcS21";
const char* password = "tmec321$";

// -------------------- Firebase settings --------------------
const char* firebaseHost = "https://push-my-buttons-default-rtdb.firebaseio.com";
const char* firebasePath = "/counts.json";  // we'll PUT here

// -------------------- Button settings --------------------
// Change these pins to match how your buttons are wired
const int buttonAPin = 5;
const int buttonBPin = 4;

unsigned long lastSendTime = 0;
const unsigned long sendIntervalMs = 5000;  // send every 5 seconds


unsigned long countA = 0;
unsigned long countB = 0;

int lastA = HIGH;
int lastB = HIGH;

void sendCountsToFirebase(unsigned long a, unsigned long b) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, can't send to Firebase");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();  // NOTE: for simplicity; skips certificate check

  HTTPClient http;

  String url = String(firebaseHost) + String(firebasePath);
  Serial.print("Sending to URL: ");
  Serial.println(url);

  if (!http.begin(client, url)) {
    Serial.println("HTTP begin() failed");
    return;
  }

  http.addHeader("Content-Type", "application/json");

  // Build JSON like: {"a": 11, "b": 22}
  String payload = String("{\"a\":") + String(a) + ",\"b\":" + String(b) + "}";

  Serial.print("Payload: ");
  Serial.println(payload);

  int httpCode = http.PUT(payload);

  Serial.print("HTTP response code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.print("Firebase response: ");
    Serial.println(response);
  } else {
    Serial.println("HTTP request failed");
  }

  http.end();
}



void setup() {
  Serial.begin(115200);
  delay(1000);

  // Setup buttons
  pinMode(buttonAPin, OUTPUT);
  pinMode(buttonBPin, OUTPUT);

  // Status LED Setup
  pinMode(statBlue, OUTPUT);
  pinMode(statRed, OUTPUT);
  
  digitalWrite(statRed, HIGH);
  Serial.println("Starting up...");
  Serial.print("Button A pin: ");
  Serial.println(buttonAPin);
  Serial.print("Button B pin: ");
  Serial.println(buttonBPin);

  // Connect to Wi-Fi
  Serial.println();
  Serial.println("Connecting to WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(statBlue, LOW);
    digitalWrite(statRed, HIGH);
    Serial.print(".");
    delay(100);
    digitalWrite(statRed, LOW);
  }

  digitalWrite(statRed, LOW);
  digitalWrite(statBlue, HIGH);

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read current button states
  int currentA = digitalRead(buttonAPin);
  int currentB = digitalRead(buttonBPin);

  if (lastA == HIGH && currentA == LOW) {
    countA++;
    Serial.print("Button A pressed. CountA = ");
    Serial.println(countA);
  }

  if (lastB == HIGH && currentB == LOW) {
    countB++;
    Serial.print("Button B pressed. CountB = ");
    Serial.println(countB);
  }

  lastA = currentA;
  lastB = currentB;

  unsigned long now = millis();
  if (now - lastSendTime >= sendIntervalMs) {
    lastSendTime = now;

    // Send the REAL counts now
    Serial.print("Sending counts to Firebase: A = ");
    Serial.print(countA);
    Serial.print(", B = ");
    Serial.println(countB);

    sendCountsToFirebase(countA, countB);
  }

  delay(10);
}

