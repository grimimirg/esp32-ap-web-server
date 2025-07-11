#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

// ----------------------------------------------------------------
// CONSTANTS


// HTTP responses
enum class HttpStatus {
  OK = 200,
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

  setupAccessPoint();
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

// Web Server
void setupHttpServer() {
  server.on(rootPath, []() {
    server.send(static_cast<int>(HttpStatus::OK), contentType, rootHtmlContent);
  });

  // Define routes here below...

  server.onNotFound([]() {
    server.sendHeader("Location", "/", true);
    server.send(static_cast<int>(HttpStatus::NOT_FOUND), contentType, rootNotFound);
  });

  server.begin();
  Serial.println("Web Server started.");
}
