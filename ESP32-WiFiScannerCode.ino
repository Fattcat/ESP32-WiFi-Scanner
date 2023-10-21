// Created By Fattcat
// YouTube : https://www.youtube.com/channel/UCKfToKJFq-uvI8svPX0WzYQ
// Oled Pin VCC --> ESP32 Pin 3.3 V
// Oled Pin GND --> ESP32 Pin GND
// Oled Pin SCL --> ESP32 Pin 21
// Oled Pin SDA --> ESP32 Pin 22

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#define OLED_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("Zobrazovač SSD1306 nenačítaný."));
    for (;;);
  }

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.display();
}

void loop() {
  int n = WiFi.scanNetworks();

  display.clearDisplay();
  display.setCursor(0, 0);

  for (int i = 0; i < n; i++) {
    display.print("SSID: ");
    display.println(WiFi.SSID(i));

    display.print("MAC: ");
    display.println(WiFi.BSSIDstr(i));

    display.print("RSSI: ");
    display.println(WiFi.RSSI(i));

    display.print("Sec: ");
    display.println(getEncryptionTypeText(WiFi.encryptionType(i)));

    display.print("Channel: ");
    display.println(WiFi.channel(i));

    display.display();

    delay(1000);
  }

  // Vyčistíme displej po každých 9 sekundách
  static unsigned long lastDisplayClearTime = 0;
  if (millis() - lastDisplayClearTime >= 14000) {
    display.clearDisplay();
    display.display();
    lastDisplayClearTime = millis();
  }
}

String getEncryptionTypeText(uint8_t encryptionType) {
  switch (encryptionType) {
    case WIFI_AUTH_OPEN:
      return "None";
    case WIFI_AUTH_WEP:
      return "WEP";
    case WIFI_AUTH_WPA_PSK:
      return "WPA";
    case WIFI_AUTH_WPA2_PSK:
      return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return "WPA2 Enterprise";
    default:
      return "Unknown";
  }
}

