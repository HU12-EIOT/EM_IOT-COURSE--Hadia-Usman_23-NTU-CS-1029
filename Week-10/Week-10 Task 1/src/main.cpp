/*
  ESP32 Webserver with Static IP
  Control Built-in LED (GPIO 2)
*/

#include <WiFi.h>

// ---------- WiFi Credentials ----------
const char* ssid = "Redmi11";
const char* password = "tfgwzf8mqjqqq5e";

// ---------- STATIC IP CONFIG ----------
IPAddress local_IP(10, 257, 57, 19);   // ESP32 Static IP
IPAddress gateway(10, 257, 57, 1);      // WiFi Router gateway
IPAddress subnet(255, 255, 255, 0);     // Subnet mask

WiFiServer server(80);
const int LED_PIN = 2;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // ---------- APPLY STATIC IP ----------
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure Static IP");
  }

  // ---------- Connect to WiFi ----------
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("ESP32 Static IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("New Client Connected");
  String request = client.readStringUntil('\r');
  Serial.println(request);

  // ----- LED CONTROL -----
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(LED_PIN, HIGH);
  }
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(LED_PIN, LOW);
  }

  // ----- HTML RESPONSE -----
  String htmlPage =
    "<!DOCTYPE html><html>"
    "<h1>ESP32 LED Control (Static IP)</h1>"
    "<p><a href=\"/LED=ON\"><button>LED ON</button></a></p>"
    "<p><a href=\"/LED=OFF\"><button>LED OFF</button></a></p>"
    "</html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(htmlPage);

  delay(1);
  client.stop();
  Serial.println("Client disconnected");
}