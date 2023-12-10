#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#define OLED_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA_PIN 4    // nebo jiný pin, který jste si vybrali
#define OLED_SCL_PIN 5    // nebo jiný pin, který jste si vybrali
#define OLED_RESET    -1 // nebo jiný pin, který chcete použít pro reset

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  
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

    display.print("MAC:");
    display.println(WiFi.BSSIDstr(i));

    display.print("dB: ");
    display.println(WiFi.RSSI(i));

    display.print("Sec: ");
    display.println(getEncryptionTypeText(WiFi.encryptionType(i)));

    display.print("CH: ");
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
