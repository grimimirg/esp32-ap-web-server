# ESP32 Access Point Web Server

This project sets up an ESP32 as a Wi-Fi Access Point and runs a lightweight web server.  
When a user connects to the ESP32's Wi-Fi network and opens a browser, they can access a simple HTML page served directly by the ESP32 via http://192.168.4.1/

## 📡 Features

- ESP32 creates its own Wi-Fi Access Point (`ESP32-AP`)
- Serves an HTML page at `/` with styled text
- Handles unknown routes with a custom 404 error page
- Uses modern `enum class` to define HTTP status codes
- Clean and modular code, easy to expand

## 🛠️ Requirements

- ESP32 board (e.g., ESP32 DevKit, ESP32-S3 WROOM)
- Arduino IDE or PlatformIO
- Libraries:
  - [`WiFi.h`](https://www.arduino.cc/en/Reference/WiFi)
  - [`WebServer.h`](https://github.com/espressif/arduino-esp32/tree/master/libraries/WebServer)

## 🚀 How It Works

1. The ESP32 boots and creates a Wi-Fi access point:
   ```
   SSID: ESP32-AP
   Password: asdrubale123
   ```
2. A user connects to this Wi-Fi network with a smartphone or laptop.
3. The user navigates to `http://192.168.4.1/` in a browser.
4. The ESP32 serves a simple, styled HTML page.
5. If the user tries an unknown route, they are redirected to a custom 404 page with a link to go back to `/`.

## 🧠 Code Structure

- **`setupAccessPoint()`** – Creates the ESP32 AP and prints the IP address.
- **`setupHttpServer()`** – Defines routes and starts the web server.
- **`enum class HttpStatus`** – Enumerates HTTP status codes.
- **`rootHtmlContent`** – Main HTML page shown at `/`.
- **`rootNotFound`** – 404 error page shown when a path doesn't exist.

## 🔧 Customization

You can easily add more routes like this:

```cpp
server.on("/status", []() {
  server.send(static_cast<int>(HttpStatus::OK), contentType, "<p>ESP32 is running.</p>");
});
```

## 📄 License

This project is open-source under the MIT License.  
Feel free to use, modify, and build upon it.

