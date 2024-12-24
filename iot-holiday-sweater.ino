#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WS2812FX.h>


// DEFINE ONBOARD LED
#define LED_READY 2

// DEFINE LED STRIP
#define LED_COUNT 44
#define LED_STRIP 14
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_STRIP, NEO_GRB + NEO_KHZ800);

// SET IP ADDRESSES
IPAddress ip(172, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// INSTANTIATE SERVERS
DNSServer dns;
WebServer server(80);

String form = R"(
<!DOCTYPE html>
<html lang='en'>
    <head>
        <meta charset='utf-8'>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <title>Joe's Holiday Sweater</title>
        <link rel="icon" href="data:image/svg+xml,<svg xmlns=%22http://www.w3.org/2000/svg%22 viewBox=%220 0 100 100%22><text y=%22.9em%22 font-size=%2290%22>🎄</text></svg>">
    </head>
    <style>
        html {
            color: #fffeff;
            background-color: #04012e;
            font-family: "Comic Sans", "Comic Sans MS", "Chalkboard", "ChalkboardSE-Regular", sans-serif;
            width: 100%;
        }
        body {
            margin: 0 auto;
            padding: 1em;
            text-align: center;
            max-width: 400px;
        }
        p {
            margin: 0px;
        }
        h1 {
            margin-top: 0px;
        }
        button {
            width: 100%;
            height: 75px;
            margin: 1em 0px 1em 0px;
            font-family: "Comic Sans", "Comic Sans MS", "Chalkboard", "ChalkboardSE-Regular", sans-serif;
            font-weight: 900;
            font-size: larger;
            border-radius: 1em;
        }
        button:hover {
            filter: contrast(50%);
        }
        .button-label {
            color: white;
            mix-blend-mode: difference;
        }
    </style>
    <body>
        <h1>Joe's Holiday Sweater</h1>
        <h1>🎄 🎁 ❄️ ☃️</h1>
        <p>It's nearly Christmas and Joe has not finished decorating his tree! Please help Joe light his Christmas Tree! What color should he choose?</p>
        <br>
        <button onclick='input("red")' style="background-color:red"><p class="button-label">RED</p></button>
        <button onclick='input("orange")' style="background-color:orange"><p class="button-label">ORANGE</p></button>
        <button onclick='input("yellow")' style="background-color:yellow"><p class="button-label">YELLOW</p></button>
        <button onclick='input("green")' style="background-color:green"><p class="button-label">GREEN</p></button>
        <button onclick='input("blue")' style="background-color:blue"><p class="button-label">BLUE</p></button>
        <button onclick='input("indigo")' style="background-color: #4b0082"><p class="button-label">INDIGO</p></button>
        <button onclick='input("violet")' style="background-color: #7F00FF"><p class="button-label">VIOLET</p></button>
        <button onclick='input("white")' style="background-color:white"><p class="button-label">WHITE</p></button>
        <button onclick='input("rainbow")' style="background: linear-gradient(in hsl longer hue 45deg, red 0 0);"><p class="button-label">RAINBOW</p></button>
        <button onclick='input("candle")' style="background: linear-gradient(black 50%, darkorange 100%);"><p class="button-label">CANDLE</p></button>
        <button onclick='input("sparkle")' style="background-color: black"><p class="button-label">SPARKLE ✨</p></button>
        <button onclick='input("christmas")' style="background: linear-gradient( green, red);"><p class="button-label">CHRISTMAS</p></button>
        <button onclick='input("random")'><p class="button-label">RANDOM</p></button>
        <h1>🎄 🎁 ❄️ ☃️</h1>
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

  // Initialize Onboard LED
  pinMode(LED_READY,OUTPUT);
  digitalWrite(LED_READY, HIGH);

  // Initialize LED Strip
  ws2812fx.init();
  ws2812fx.setBrightness(128);
  ws2812fx.setSpeed(2500);
  ws2812fx.setColor(255,255,255);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();

  // Define routes and serve web pages
  server.onNotFound([]() {
    server.send(200, "text/html", form);
  });

  server.on("/input", [](){
    String state = server.arg("state");

    if (state == "red") {
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.setColor(255, 0, 0);
    }
    else if (state == "orange") {
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.setColor(255, 165, 0);
    }
    else if (state == "yellow") {
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.setColor(255, 255, 0);
    }
    else if (state == "green") {
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.setColor(0, 255, 0);
    }
    else if (state == "blue") {
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.setColor(0, 0, 255);
    }
    else if (state == "indigo") {
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.setColor(75, 0, 130);
    }
    else if (state == "violet") {
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.setColor(127, 0, 255);
    }
    else if (state == "white" ){
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.setColor(255, 255, 255);
    }
    else if (state == "rainbow") {
      ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
    }
    else if (state == "candle") {
      ws2812fx.setColor(255,140,0);
      ws2812fx.setMode(FX_MODE_FIRE_FLICKER);
    }
    else if (state == "sparkle") {
      ws2812fx.setColor(255, 255, 0);
      ws2812fx.setMode(FX_MODE_HYPER_SPARKLE);
    }
    else if (state == "christmas") {
      ws2812fx.setColor(255, 255, 0);
      ws2812fx.setMode(FX_MODE_MERRY_CHRISTMAS);
    }
    else if (state == "random") {
      ws2812fx.setColor(255, 255, 0);
      ws2812fx.setMode(FX_MODE_RUNNING_RANDOM);
    }
 
    server.send(200, "text/html", "Success! Set state to " + state + ".");
  });

  // Start servers
  dns.start(53, "*", ip);
  server.begin();

  digitalWrite(LED_READY, LOW);
}

void loop() {
  ws2812fx.service();
  server.handleClient();
  dns.processNextRequest();
}
