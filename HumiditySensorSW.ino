/*
 * Board: NodeMCU 1.0 (Index: https://arduino.esp8266.com/stable/package_esp8266com_index.json)
 * Libraries: Adafruit_SSD1306 1.3.0, DHT_sensor_library_for_ESPx 1.0.13, Adafruit_NeoPixel 1.2.4
 */

#define DEBUG(cmd) cmd
//#define DEBUG(cmd)

#include "DHTesp.h" //DHT Bibliothek laden

/** Initialize DHT sensor */
DHTesp dht;
/** Pin number for DHT11 data pin */
const static int DHT_PIN = D6;
const static float SMOOTHING_FACTOR = 0.25;
float smoothedHumidity;

void setup() {
  Serial.begin(115200); //Serielle Verbindung starten

  // Initialize temperature sensor
  dht.setup(DHT_PIN, DHTesp::DHT11);

  setup_led();
  setup_display();
  delay(2000); //Zwei Sekunden Vorlaufzeit bis zur Messung (der Sensor ist etwas träge)
  
  TempAndHumidity lastValues = dht.getTempAndHumidity();
  smoothedHumidity = dht.getHumidity();
}

void loop() {
  TempAndHumidity lastValues = dht.getTempAndHumidity();
  
  DEBUG(Serial.println("Temperature: " + String(lastValues.temperature, 0));)
  DEBUG(Serial.println("Humidity: " + String(lastValues.humidity, 0));)
  smoothedHumidity = lastValues.humidity * SMOOTHING_FACTOR + smoothedHumidity * (1 - SMOOTHING_FACTOR);
  Serial.println("Smoothed humidity: " + String(smoothedHumidity, 1));
  
  update_led(smoothedHumidity);
  update_display(lastValues.temperature, lastValues.humidity);
  delay(2000); //Zwei Sekunden Vorlaufzeit bis zur Messung (der Sensor ist etwas träge)
}
