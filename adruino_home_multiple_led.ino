#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "TrungTrang_2.4G";  // Replace with your WiFi name
const char* password = "BEN@2020";     // Replace with your WiFi password

WebServer server(80);  // Create a web server on port 80

// Define LED pins
const int ledPin1 = 2;
const int ledPin2 = 4;
const int ledPin3 = 16;

void handleRoot() {
  String html = "<h1>ESP32 Multi-LED Control</h1>";
  html += "<p>LED 1 (GPIO 2): <a href=\"/on1\"><button>Turn ON</button></a> <a href=\"/off1\"><button>Turn OFF</button></a></p>";
  html += "<p>LED 2 (GPIO 4): <a href=\"/on2\"><button>Turn ON</button></a> <a href=\"/off2\"><button>Turn OFF</button></a></p>";
  html += "<p>LED 3 (GPIO 16): <a href=\"/on3\"><button>Turn ON</button></a> <a href=\"/off3\"><button>Turn OFF</button></a></p>";
  
  server.send(200, "text/html", html);
}

// Handlers for each LED
void handleLED1On() {
  digitalWrite(ledPin1, HIGH);
  server.send(200, "text/html", "<h1>LED 1 is ON</h1><p><a href=\"/\"><button>Back</button></a></p>");
}

void handleLED1Off() {
  digitalWrite(ledPin1, LOW);
  server.send(200, "text/html", "<h1>LED 1 is OFF</h1><p><a href=\"/\"><button>Back</button></a></p>");
}

void handleLED2On() {
  digitalWrite(ledPin2, HIGH);
  server.send(200, "text/html", "<h1>LED 2 is ON</h1><p><a href=\"/\"><button>Back</button></a></p>");
}

void handleLED2Off() {
  digitalWrite(ledPin2, LOW);
  server.send(200, "text/html", "<h1>LED 2 is OFF</h1><p><a href=\"/\"><button>Back</button></a></p>");
}

void handleLED3On() {
  digitalWrite(ledPin3, HIGH);
  server.send(200, "text/html", "<h1>LED 3 is ON</h1><p><a href=\"/\"><button>Back</button></a></p>");
}

void handleLED3Off() {
  digitalWrite(ledPin3, LOW);
  server.send(200, "text/html", "<h1>LED 3 is OFF</h1><p><a href=\"/\"><button>Back</button></a></p>");
}

void setup() {
  Serial.begin(115200);

  // Set LED pins as outputs
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define routes for LEDs
  server.on("/", handleRoot);
  server.on("/on1", handleLED1On);
  server.on("/off1", handleLED1Off);
  server.on("/on2", handleLED2On);
  server.on("/off2", handleLED2Off);
  server.on("/on3", handleLED3On);
  server.on("/off3", handleLED3Off);

  server.begin();
}

void loop() {
  server.handleClient();
}
