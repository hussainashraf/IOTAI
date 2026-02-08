#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------- WiFi --------
const char* ssid = "Hussain";
const char* password = "RedmiIphone44@";

// -------- Pins --------
int obstaclePin = 18;
int ledPin = 4;

// -------- OLED --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -------- Server --------
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

bool lastState = HIGH;

// -------- HTML UI --------
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Intrusion Monitor</title>
  <style>
    body {
      background:#0f172a;
      color:white;
      font-family:Arial;
      text-align:center;
      margin-top:50px;
    }
    .status {
      width:220px;
      height:220px;
      margin:auto;
      border-radius:50%;
      background:green;
      box-shadow:0 0 40px green;
      transition:0.2s;
    }
    .alert {
      background:red !important;
      box-shadow:0 0 60px red !important;
      animation:pulse 0.5s infinite alternate;
    }
    @keyframes pulse {
      from { transform:scale(1); }
      to { transform:scale(1.1); }
    }
  </style>
</head>
<body>
  <h1>Real-Time Intrusion Monitor</h1>
  <div id="circle" class="status"></div>
  <h2 id="text">No Obstacle</h2>

<script>
  const ws = new WebSocket("ws://" + location.hostname + ":81/");
  ws.onmessage = (e) => {
    const circle = document.getElementById("circle");
    const text = document.getElementById("text");

    if(e.data === "DETECTED"){
      circle.classList.add("alert");
      text.innerText = "🚨 OBSTACLE DETECTED";
    } else {
      circle.classList.remove("alert");
      text.innerText = "✅ No Obstacle";
    }
  };
</script>
</body>
</html>
)rawliteral";

// -------- WebSocket Event --------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.println("Web client connected");
  }
}

// -------- OLED Helpers --------
void showNoObstacle() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("System Status:");
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println("CLEAR");
  display.display();
}

void showObstacle() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("System Status:");
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println("ALERT!");
  display.display();
}

// -------- Setup --------
void setup() {
  Serial.begin(115200);

  pinMode(obstaclePin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // I2C init
  Wire.begin(21, 22);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }

  showNoObstacle();

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  // WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

// -------- Loop --------
void loop() {
  webSocket.loop();

  bool currentState = digitalRead(obstaclePin);

  if (currentState != lastState) {
    if (currentState == LOW) {
      digitalWrite(ledPin, HIGH);
      showObstacle();
      webSocket.broadcastTXT("DETECTED");
    } else {
      digitalWrite(ledPin, LOW);
      showNoObstacle();
      webSocket.broadcastTXT("CLEAR");
    }
    lastState = currentState;
  }

  delay(30);
}