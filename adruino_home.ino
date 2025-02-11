#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR WIFI";  // Replace with your WiFi name
const char* password = "WIFI PASSWORD";     // Replace with your WiFi password

WebServer server(80);  // Create a web server on port 80
const int ledPin = 2;  // GPIO pin for LED

void handleRoot() {
  server.send(200, "text/html", "<h1>ESP32 LED Control</h1><p><a href=\"/on\"><button>Turn ON</button></a> <a href=\"/off\"><button>Turn OFF</button></a></p>");
}

void handleLEDOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/html", "<h1>LED is ON</h1><p><a href=\"/\"><button>Back</button></a></p>");
}

void handleLEDOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/html", "<h1>LED is OFF</h1><p><a href=\"/\"><button>Back</button></a></p>");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.begin(115200);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleLEDOn);
  server.on("/off", handleLEDOff);

  server.begin();
}

void loop() {
  server.handleClient();
}
