#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>


#define leftAChannel 0
#define leftBChannel 1
#define rightAChannel 2
#define rightBChannel 3
#define leftAPin 32
#define leftBPin 33
#define rightAPin 23
#define rightBPin 22
#define buzzerPin 25

enum State {
  Stop,
  Forward,
  Backward,
  Left,
  Right
};

void quickBlink();
void stop();
void forward();
void backward();
void left();
void right();
void setState(State newState);
void buzzer();

int frequency = 5000;
int resolution = 8;
int high = 255;
int mid = 192;
int low = 176;
enum State currentState = Stop;

const char* ssid     = "AwesomeCar_30Y";
const char* password = "123456789";

// Set web server port number to 80
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello World!");
  });
  
  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request) {
    setState(Forward);
    request->send(200);
  });
  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request) {
    setState(Backward);
    request->send(200);
  });  
  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request) {
    setState(Left);
    request->send(200);
  });  
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request) {
    setState(Right);
    request->send(200);
  });
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request) {
    setState(Stop);
    request->send(200);
  });
  server.on("/buzzer", HTTP_GET, [](AsyncWebServerRequest *request) {
    buzzer();
    request->send(200);
  });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.begin();

  // initialize digital pin LED_BUILTIN (predefined to point to correct PIN) as output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  quickBlink();  

  ledcSetup(leftAChannel, frequency, resolution);
  ledcAttachPin(leftAPin, leftAChannel);
  ledcSetup(leftBChannel, frequency, resolution);
  ledcAttachPin(leftBPin, leftBChannel);
  ledcSetup(rightAChannel, frequency, resolution);
  ledcAttachPin(rightAPin, rightAChannel);
  ledcSetup(rightBChannel, frequency, resolution);
  ledcAttachPin(rightBPin, rightBChannel);
}

void loop() {
  // digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  // setState(Forward);
  // delay(1000);
  // setState(Stop);
  // delay(1000);

  // setState(Backward);
  // delay(1000);
  // setState(Stop);
  // delay(1000);

  // setState(Left);
  // delay(1000);
  // setState(Stop);
  // delay(1000);

  // setState(Right);
  // delay(1000);
  // setState(Stop);
  // delay(1000);

  // digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  // Serial.println("Long pause");
  // delay(5000);
}

// Quick blink for 3 second
void quickBlink() {
  for (size_t i = 0; i < 6; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(250);
  }
}

void setState(State newState) {
  Serial.printf("setState()->%d\n", newState);
  if (newState == currentState) {
    return;
  }

  switch (newState) {
  case Forward:
    forward();
    break;
  case Backward:
    backward();
    break;
  case Left:
    left();
    break;
  case Right:
    right();
    break;
  default:
    stop();
    break;
  }

  currentState = newState;
}

void buzzer() {
    Serial.println("buzzer()->");
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
}

void stop() {
  Serial.println("stop()->");
  ledcWrite(leftAChannel, 0);
  ledcWrite(leftBChannel, 0);
  ledcWrite(rightAChannel, 0);
  ledcWrite(rightBChannel, 0);
}

void forward() {
  Serial.println("forward()->");
  ledcWrite(leftAChannel, high);
  ledcWrite(leftBChannel, 0);
  ledcWrite(rightAChannel, high);
  ledcWrite(rightBChannel, 0);
}

void backward() {
  Serial.println("backward()->");
  ledcWrite(leftAChannel, 0);
  ledcWrite(leftBChannel, mid);
  ledcWrite(rightAChannel, 0);
  ledcWrite(rightBChannel, mid);
}

void left() {
  Serial.println("left()->");
  ledcWrite(leftAChannel, 0);
  ledcWrite(leftBChannel, low);
  ledcWrite(rightAChannel, low);
  ledcWrite(rightBChannel, 0);
}

void right() {
  Serial.println("right");
  ledcWrite(leftAChannel, low);
  ledcWrite(leftBChannel, 0);
  ledcWrite(rightAChannel, 0);
  ledcWrite(rightBChannel, low);
}
