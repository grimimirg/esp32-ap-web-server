#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

// ----------------------------------------------------------------
// CONSTANTS

// HTTP responses
enum class PinStatus {
  ON = HIGH,
  OFF = LOW
};

const int validPins[] = {2, 4, 5, 12, 13, 14, 15, 16, 17};

// HTTP responses
enum class HttpStatus {
  OK = 200,
  BAD_REQUEST = 400,
  NOT_FOUND = 404
};

// Other
const char *contentType = "text/html";


// ----------------------------------------------------------------
// ROUTES

const char *rootPath = "/";
const char *rootHtmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
  <head><title>ESP32 Node</title></head>
  <body>
    <h1>Hi, I am a node!</h1>
    <p>I can show you normal text</p>
    <b>some bold text</b>
    <h1 style="font-family:verdana;">This is a heading</h1>
    <p style="font-family:courier;">This is a paragraph.</p> 
    <p style="color:red;">I am red</p>
    <p style="color:blue;">I am blue</p>
    <p style="font-size:50px;">I am big</p>
  </body>
</html>
)rawliteral";

const char *pinPath = "/pin";

const char *rootNotFound = R"rawliteral(
    <!DOCTYPE html>
    <html>
      <head><title>404 Not Found</title></head>
      <body>
        <h1>404 - Page Not Found</h1>
        <p>The page you're looking for doesn't exist.</p>
        <a href="/">Go back to home</a>
      </body>
    </html>
  )rawliteral";

// ----------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println();

  //setupAccessPoint();
  setupWiFiStation();
  setupHttpServer();
}

void loop() {
  server.handleClient();
}

// ----------------------------------------------------------------

// Access Point
void setupAccessPoint() {
  Serial.println("Starting Access Point...");

  if (!WiFi.softAP("ESP32-AP", "asdrubale123")) {
    Serial.println("Access Point creation failed!");
    while (true);
  }

  Serial.println("Access Point created successfully.");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void setupWiFiStation() {
  const char* ssid = "change-me";
  const char* password = "change-me";

  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 50) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi");
    Serial.print(WiFi.status());
    while (true);
  }

  Serial.println("\nConnected to WiFi.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Web Server
void setupHttpServer() {
  // root path
  server.on(rootPath, []() {
    server.send(static_cast<int>(HttpStatus::OK), contentType, rootHtmlContent);
  });

  // pin path
  server.on(pinPath, []() {
    if (!server.hasArg("number") || !server.hasArg("status")) {
      server.send(static_cast<int>(HttpStatus::BAD_REQUEST), contentType, "<p>Missing parameters</p>");
      Serial.println("Missing parameter.");
      return;
    }

    int pinNumber = server.arg("number").toInt();

    if (!isValidPin(pinNumber)) {
      server.send(static_cast<int>(HttpStatus::NOT_FOUND), contentType, "<p>Invalid pin</p>");
      Serial.println("Invalid pin.");
      return;
    }

    pinMode(pinNumber, OUTPUT);

    String statusStr = server.arg("status");
    PinStatus status = parsePinStatus(statusStr);
    
    digitalWrite(pinNumber, static_cast<int>(status));

    String pinHtmlContent = "<html><body>"
                        "<h2>Pin update</h2>"
                        "<p>Set pin " + String(pinNumber) + " to " + statusStr + "</p>"
                        "</body></html>";

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
    server.send(static_cast<int>(HttpStatus::OK), contentType, pinHtmlContent);
    Serial.println("Pin " + String(pinNumber) + " to " + statusStr);
  });

  // Define other routes here below...

  // fallback path
  server.onNotFound([]() {
    server.sendHeader("Location", "/", true);
    server.send(static_cast<int>(HttpStatus::NOT_FOUND), contentType, rootNotFound);
    Serial.println("Route not found!");
  });

  server.begin();
  Serial.println("Web Server started.");
}

PinStatus parsePinStatus(const String& pinStatusStr) {
  if (pinStatusStr == "ON") return PinStatus::ON;
  if (pinStatusStr == "OFF") return PinStatus::OFF;

  return PinStatus::OFF;
}

bool isValidPin(int pin) {
  for (int i = 0; i < sizeof(validPins) / sizeof(validPins[0]); i++) {
    if (pin == validPins[i]) return true;
  }
  return false;
}
