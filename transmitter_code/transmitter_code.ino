
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <LoRa.h>

// GPS setup
static const int RXPin = 5, TXPin = 4;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// LoRa setup
#define SS 10
#define RST 9
#define DIO0 2

void setup()
{
  // Serial Monitor for debugging
  Serial.begin(115200);

  // Initialize GPS
  ss.begin(GPSBaud);
  Serial.println(F("GPS and LoRa Integration"));

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed!");
    while (1);
  }
  Serial.println("LoRa initialized successfully!");
}

void loop()
{
  // Check for GPS data
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read())) // Process new GPS data
    {
      if (gps.location.isValid()) // If valid location data is available
      {
        String googleMapLink = "http://www.google.com/maps/place/";
        googleMapLink += String(gps.location.lat(), 6);
        googleMapLink += ",";
        googleMapLink += String(gps.location.lng(), 6);

        // Display the link on Serial Monitor
        Serial.println("Google Maps Link: ");
        Serial.println(googleMapLink);

        // Send the link via LoRa
        LoRa.beginPacket();
        LoRa.print(googleMapLink);
        LoRa.endPacket();

        // Optional: Delay before next transmission
        //delay(5000);
      }
      else
      {
        Serial.println("Waiting for valid GPS data...");
      }
    }
  }

  // Check for no GPS data condition
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected: check wiring.");
    while (true);
  }
}