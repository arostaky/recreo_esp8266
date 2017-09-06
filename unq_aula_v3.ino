#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ThingerESP8266.h>

#define USERNAME "jpdibacco"
#define DEVICE_ID "ESP8266UNQ"
#define DEVICE_CREDENTIAL "WtB3Cn1wIQO"
const char* ssid = "Mr_Robot_2.4";
const char* password = "sayh3ll0tomylittlefriend";
/* Set these to your desired credentials for AP. */
const char* apssid = "ESPap";
const char* appassword = "thereisnospoon";
int buttonState = 0;
ESP8266WebServer server(80);

const short int BUILTIN_LED1 = 13; //GPI13
const short int BUTTON = 12; //GPI12
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUILTIN_LED1, OUTPUT); // Initialize the BUILTIN_LED1 pin as an output
  pinMode(BUTTON, INPUT);
  thing["led"] << digitalPin(BUILTIN_LED1);
  thing["millis"] >> outputValue(millis());
  //buttonState = digitalRead(BUTTON);
  thing["button"] >>[](pson& out){
    out = (bool) digitalRead(BUTTON);
    };
  //Serial.begin(115200);
  //Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  //Serial.println("Ready");
  //Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());
  //digitalWrite(BUILTIN_LED1, LOW);
   WiFi.softAP(apssid, appassword);

  //IPAddress myIP = WiFi.softAPIP();
  // Serial.print("AP IP address: ");
  //Serial.println(myIP);
   server.on("/", handleRoot);
   server.begin();
  //digitalWrite(BUILTIN_LED1, HIGH);

}

void loop() {
  thing.handle();
  ArduinoOTA.handle();
  server.handleClient();
  buttonState = digitalRead(BUTTON);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      // turn LED on:
      digitalWrite(BUILTIN_LED1, HIGH);
      thing.call_endpoint("alumnos");
    } else {
      // turn LED off:
      digitalWrite(BUILTIN_LED1, LOW);
    }

}
