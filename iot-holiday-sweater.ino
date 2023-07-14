#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

// DEFINE GPIO
#define LED_READY 2

// SET IP ADDRESSES
IPAddress ip(172, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// INSTANTIATE SERVERS
DNSServer dns;
ESP8266WebServer server(80);

String form = R"(
<!DOCTYPE html>
<html lang='en'>
    <head>
        <meta charset='utf-8'>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <title>Joe's Holiday Sweater</title>
    </head>
    <body>
        <h1>Joe's Holiday Sweater</h1>
        <button onclick='input("on")'>Turn On</button>
        <button onclick='input("off")'>Turn Off</button>
    </body>
</html>
<script>
    function input(state) {
        console.log(state)
        var xhr = new XMLHttpRequest();
        xhr.open("POST", 'input');
        xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xhr.send("state="+state);
    }
</script>
)";

void setup() {
  // Initialize serial
  Serial.begin(115200);

  // Initialize wifi
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP("Joe's Holiday Sweater");

  // Initialize LED
  pinMode(LED_READY,OUTPUT);
  digitalWrite(LED_READY, HIGH);

  // Define routes and serve web pages
  server.onNotFound([]() {
    server.send(200, "text/html", form);
  });
  server.on("/input", [](){
    String state = server.arg("state");
    if (state == "on") {
        digitalWrite(LED_READY, LOW);
    } else if (state == "off") {
        digitalWrite(LED_READY, HIGH);
    }
    server.send(200, "text/html", "Success! Set state to " + state + ".");
  });

  // Start servers
  dns.start(53, "*", ip);
  server.begin();

  digitalWrite(LED_READY, HIGH);
}

void loop() {
  server.handleClient();
  dns.processNextRequest();
  delay(0.1);
}
