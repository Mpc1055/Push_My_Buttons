#include "esp32-hal-gpio.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "PMB_26_Ver2.h"

void loadCountsFromFirebase() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, can't read from Firebase");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();  // skip certificate validation

  HTTPClient http;

  String url = String(firebaseHost) + String(firebasePath);
  Serial.print("Reading from Firebase: ");
  Serial.println(url);

  if (!http.begin(client, url)) {
    Serial.println("HTTP begin() failed");
    return;
  }

  int httpCode = http.GET();

  Serial.print("HTTP response code: ");
  Serial.println(httpCode);

  if (httpCode == 200) {
    String payload = http.getString();
    Serial.print("Firebase payload: ");
    Serial.println(payload);

    // Very simple JSON parsing (expects {"a":number,"b":number})
    int aIndex = payload.indexOf("\"a\":");
    int bIndex = payload.indexOf("\"b\":");

    if (aIndex != -1 && bIndex != -1) {
      countA = payload.substring(aIndex + 4, payload.indexOf(",", aIndex)).toInt();
      countB = payload.substring(bIndex + 4, payload.indexOf("}", bIndex)).toInt();

      Serial.print("Loaded counts from Firebase -> A = ");
      Serial.print(countA);
      Serial.print(", B = ");
      Serial.println(countB);
    } else {
      Serial.println("Failed to parse counts from Firebase");
    }
  } else {
    Serial.println("Failed to read from Firebase");
  }

  http.end();
}

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

void Status_RGB(char led) {
  switch (led) {
    case 'r':
      digitalWrite(statRed, HIGH);
      digitalWrite(statBlue, LOW);
      digitalWrite(statGreen, LOW);
      break;

    case 'g':
      digitalWrite(statRed, LOW);
      digitalWrite(statBlue, LOW);
      digitalWrite(statGreen, HIGH);
      break;

    case 'b':
      digitalWrite(statRed, LOW);
      digitalWrite(statBlue, HIGH);
      digitalWrite(statGreen, LOW);
      break;

    case 'o':
      digitalWrite(aRed, LOW);
      digitalWrite(aBlue, LOW);
      digitalWrite(aGreen, LOW);
      break;
  }
}

void A_RGB(char led) {
  switch (led) {
    case 'r':
      digitalWrite(aRed, HIGH);
      digitalWrite(aBlue, LOW);
      digitalWrite(aGreen, LOW);
      break;

    case 'g':
      digitalWrite(aRed, LOW);
      digitalWrite(aBlue, LOW);
      digitalWrite(aGreen, HIGH);
      break;

    case 'b':
      digitalWrite(aRed, LOW);
      digitalWrite(aBlue, HIGH);
      digitalWrite(aGreen, LOW);
      break;

    case 'o':
      digitalWrite(aRed, LOW);
      digitalWrite(aBlue, LOW);
      digitalWrite(aGreen, LOW);
      break;
  }
}

void B_RGB(char led) {
  switch (led) {
    case 'r':
      digitalWrite(bRed, HIGH);
      digitalWrite(bBlue, LOW);
      digitalWrite(bGreen, LOW);
      break;

    case 'g':
      digitalWrite(bRed, LOW);
      digitalWrite(bBlue, LOW);
      digitalWrite(bGreen, HIGH);
      break;

    case 'b':
      digitalWrite(bRed, LOW);
      digitalWrite(bBlue, HIGH);
      digitalWrite(bGreen, LOW);
      break;
    
    case 'o':
      digitalWrite(aRed, LOW);
      digitalWrite(aBlue, LOW);
      digitalWrite(aGreen, LOW);
      break;
  }
}


