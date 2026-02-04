#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "PMB_26_Ver2.h"



// Actual definitions of globals (only here!)
unsigned long countA = 0;
unsigned long countB = 0;

const char* ssid = "tuiot";
const char* password = "bruc3l0w3";

// -------------------- Firebase settings --------------------
const char* firebaseHost = "https://push-my-buttons-default-rtdb.firebaseio.com";
const char* firebasePath = "/counts.json";  // we'll PUT here

// -------------------- Button settings --------------------
// Change these pins to match how your buttons are wired

unsigned long lastSendTime = 0;
const unsigned long sendIntervalMs = 5000;  // send every 5 seconds

int lastA = LOW;
int lastB = LOW;

const unsigned long debounceMs = 30;
unsigned long lastPressA = 0;
unsigned long lastPressB = 0;

void setup() {
  Serial.begin(115200);
   Wire.begin(21, 22);
  delay(1000);


  // Setup buttons — these should almost certainly be INPUTs
  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonBPin, INPUT_PULLUP);

  // Status LED Setup
  pinMode(statBlue, OUTPUT);
  pinMode(statRed, OUTPUT);
  pinMode(statGreen, OUTPUT);

  // A Button LED
  pinMode(aRed,    OUTPUT);
  pinMode(aGreen,  OUTPUT);
  pinMode(aBlue,   OUTPUT);

  //B Button LED
  pinMode(bRed,    OUTPUT);
  pinMode(bGreen,  OUTPUT);
  pinMode(bBlue,   OUTPUT);

  Status_RGB('r');

  // Connect to Wi-Fi
  Serial.println();
  Serial.println("Connecting to WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Check if WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Status_RGB('r');
    Serial.print(".");
    delay(100);
    Status_RGB('o');
  }

  // Show WiFi is connected
  Status_RGB('b');
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  loadCountsFromFirebase();

  A_RGB('g');
  B_RGB('g');

}

void loop() {
  int currentA = digitalRead(buttonAPin);
  int currentB = digitalRead(buttonBPin);
  unsigned long now = millis();

  if (lastA == LOW && currentA == HIGH && (now - lastPressA) > debounceMs) {
    countA++;
    lastPressA = now;

    A_RGB('r');
    sendCountsToFirebase(countA, countB);
    A_RGB('g');

  }

  if (lastB == LOW && currentB == HIGH && (now - lastPressB) > debounceMs) {
    countB++;
    lastPressB = now;

    B_RGB('r');
    sendCountsToFirebase(countA, countB);
    B_RGB('g');

  }

  lastA = currentA;
  lastB = currentB;
}
