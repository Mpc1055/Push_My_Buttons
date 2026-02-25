#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "PMB_26_Ver2.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RGB statusLED(statRed, statGreen, statBlue);
RGB aLED(aRed, aGreen, aBlue);
RGB bLED(bRed, bGreen, bBlue);


// Actual definitions of globals (only here!)
unsigned long countA = 0;
unsigned long countB = 0;

// const char* ssid = "MpcS21";
// const char* password = "tmec321$";

const char* ssid = "tuiot";
const char* password = "bruc3l0w3";

// const char* ssid = "Vandaley Industries";
// const char* password = "15Tattergrace48";

// -------------------- Firebase settings --------------------
const char* firebaseHost = "https://push-my-buttons-default-rtdb.firebaseio.com";
const char* firebasePath = "/counts.json";  // we'll PUT here

// -------------------- Button settings --------------------
// Change these pins to match how your buttons are wired

unsigned long lastSendTime = 0;
const unsigned long sendIntervalMs = 5000;  // send every 5 seconds

int lastA = HIGH;
int lastB = HIGH;

const unsigned long debounceMs = 30;
unsigned long lastPressA = 0;
unsigned long lastPressB = 0;

void setup() {
  statusLED.Red(HIGH);
  aLED.Red(HIGH);
  bLED.Red(HIGH);
  
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(1000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  OpenSplash();

  // Setup buttons — these should almost certainly be INPUTs
  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonBPin, INPUT_PULLUP);

 

  delay(50);  // let the pullups settle
  lastA = digitalRead(buttonAPin);
  lastB = digitalRead(buttonBPin);

  // Connect to Wi-Fi
  display.clearDisplay();
  display.setTextSize(2);  // 1 = small, 2 = bigger, etc.
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Connecting");
  display.setCursor(0, 16);
  display.print(ssid);
  display.display();

  Serial.println();
  Serial.println("Connecting to WiFi...");
  Serial.print("SSID: ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);


  // Check if WiFi is connected
  //display.setCursor(0, 24);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    statusLED.Red(HIGH);
    display.print(".");
    Serial.print(".");
    delay(100);
    statusLED.Red(LOW);
    display.display();
  }


Temple_48();
  // // Show WiFi is connected

  drawStatusBar();
  statusLED.Blue(HIGH);

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  loadCountsFromFirebase();

  aLED.Green(HIGH);
  bLED.Green(HIGH);


}

void loop() {


  drawStatusBar();

  int currentA = digitalRead(buttonAPin);
  int currentB = digitalRead(buttonBPin);
  unsigned long now = millis();



  if (lastA == HIGH && currentA == LOW && (now - lastPressA) > debounceMs) {
    countA++;
    lastPressA = now;

    
    sendCountsToFirebase(countA, countB);


  }

  if (lastB == HIGH && currentB == LOW && (now - lastPressB) > debounceMs) {
    countB++;
    lastPressB = now;

    sendCountsToFirebase(countA, countB);


  }
  
  lastA = currentA;
  lastB = currentB;
}
