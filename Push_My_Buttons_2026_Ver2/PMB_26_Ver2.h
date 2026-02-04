#ifndef PMB_26_VER2_H
#define PMB_26_VER2_H

#include <Arduino.h>

#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 32
#define SCL_PIN 33

#define iconX 2
#define iconY 14


// Status LED pins
#define statRed   19
#define statGreen 21
#define statBlue  22

//Button A LED
#define aRed      13
#define aGreen    12
#define aBlue     14

//Button B Led
#define bRed    27
#define bGreen  26
#define bBlue   25

// Button pins
#define buttonAPin 5
#define buttonBPin 4

// OLED Variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Global variables (must match .ino types exactly)
extern unsigned long countA;
extern unsigned long countB;

extern const char* ssid;
extern const char* password;


// Firebase settings – also const char*
extern const char* firebaseHost;
extern const char* firebasePath;

// Function prototypes

void Status_RGB(char led);
void A_RGB(char led);
void B_RGB(char led);

void loadCountsFromFirebase();
void sendCountsToFirebase(unsigned long a, unsigned long b);

extern Adafruit_SSD1306 display;

int wifiBarsFromRSSI(int rssi);
void drawWifiIcon(int x, int y, int bars);



#endif
