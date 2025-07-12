
# ESP32 Web-Controlled GPIO Interface

This project demonstrates how to use an ESP32 board as a simple web server to control GPIO pins through HTTP requests. It can be sets up in two different modalities:

1. As a Access Point to permit clients to connect via WiFi (currently disabled)
2. As a WiFi client, connected to an existing network

It exposes an HTTP interface (described below) to change the status of specific digital pins.

NOTE! ESP32 works with 2,4GHz band only.

## Features

- Hosts a simple HTML web page at the root (`/`) with example content.
- Provides an HTTP endpoint (`/pin`) to control GPIO pin states via query parameters.
- Supports both WiFi Station and Access Point modes.
- Includes error handling for invalid or missing parameters.
- CORS headers enabled for the `/pin` endpoint.

## Requirements

- ESP32 board
- Arduino IDE with ESP32 board support installed
- WiFi credentials (SSID and password)

## Installation

1. **Clone or copy the code** to your local machine.
2. **Open the sketch** in the Arduino IDE.
3. **Install the required libraries** (typically included with ESP32 core):
   - `WiFi.h`
   - `WebServer.h`
4. Depending on your preferred usage:
   - **Update WiFi credentials** in `setupWiFiStation()`:
      ```cpp
      const char* ssid = "change-me";
      const char* password = "change-me";
      ```
   - **Update WiFi credentials** in `setupAccessPoint()`:
     ```cpp
      WiFi.softAP("ESP32-AP", "asdrubale123")
      ```
6. **Upload the sketch** to your ESP32.

## Usage

### Web Page

Once the ESP32 connects to your network, open a browser and go to the IP address shown in the Serial Monitor. You'll see a simple HTML page at the root path `/`.

### Control GPIO Pins

You can control pins via the `/pin` endpoint.  
**Allowed pins:** `2, 4, 5, 12, 13, 14, 15, 16, 17`  
**Allowed statuses:** `ON`, `OFF`

#### Example URL:
```
http://<ESP32_IP>/pin?number=13&status=ON
```

This sets pin 13 to `HIGH`.

### Response

The ESP32 responds with an HTML confirmation showing the pin number and the new status.

## Error Handling

- Missing parameters → 400 Bad Request
- Invalid pins → 404 Not Found
- Unknown routes → 404 page with redirect to `/`

## Customization

- You can easily modify the HTML content in the `rootHtmlContent` variable.
- To enable Access Point mode, uncomment the `setupAccessPoint()` call in `setup()` and comment out the WiFi Station mode.

## License

This project is open-source and free to use for educational and personal purposes.
