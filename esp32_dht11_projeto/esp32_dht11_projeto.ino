#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>

#define DHTPIN 5       
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "CLARO_2G9F971E";
const char* password = "WrZHU48hzz";

WebServer server(80);

void handleRoot() {
  String umidade = String(dht.readHumidity());
  String temperatura = String(dht.readTemperature());

  server.send(200, "text/plain", umidade + "e" + temperatura);  // 70.0e23.0
}

void handleNotFound() {
  String message = "File Not Found\n";
  server.send(404, "text/plain", message);
}

void setup() {
  dht.begin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(1500);
  float u = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Umidade (%): ");
  Serial.print(u, 1);
  Serial.print("\t");
  Serial.print("Temperatura (C): ");
  Serial.print(t, 1);
  Serial.print("\t");
  Serial.println();
}
