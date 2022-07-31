

#include <WiFi.h>
#include <WebServer.h>
#include <string.h>

#define led 13

const char* ssid = "";
const char* pass = "";
const int serverPort = 6969;

bool ledState = false;
WebServer server(serverPort);

void setup() {
    pinMode(led, OUTPUT);

    Serial.begin(115200);
    Serial.print(">> connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.println(">> connection successful.");
    Serial.print(">> device ip is ");
    Serial.println(WiFi.localIP());

    server.on("/", handle_OnConnect);
    server.on("/ledon", handle_LedOn);
    server.on("/ledoff", handle_LedOff);
    server.onNotFound(handle_NotFound);
    
    Serial.println(">> starting http server.");
    server.begin();
    Serial.print(">> http server running on port ");
    Serial.println(serverPort);
}

void loop() {
    server.handleClient();
    if (ledState) {
        digitalWrite(led, HIGH);
    }
    else {
        digitalWrite(led, LOW);
    }
}

void handle_OnConnect() {
    server.send(200, "text/html", createPageHTML(ledState));
    Serial.println(">> request received ['/']");
}

void handle_LedOn() {
    ledState = true;
    server.send(200, "text/html", createPageHTML(ledState)); 
    Serial.println(">> request received ['/LedOn']");
}

void handle_LedOff() {
    ledState = false;
    server.send(200, "text/html", createPageHTML(ledState));
    Serial.println(">> request received ['/LedOff']");
}

void handle_NotFound() {
    server.send(404, "text/plain", "ERROR 404 page not found."); 
    Serial.println(">> request received [NOT FOUND]");
}

String createPageHTML(bool ledState) {
    String ptr = "<!DOCTYPE html> <html>\n";
    ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr +="<title>LED Control</title>\n";
    ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
    ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
    ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
    ptr +=".button-on {background-color: #3498db;}\n";
    ptr +=".button-on:active {background-color: #2980b9;}\n";
    ptr +=".button-off {background-color: #34495e;}\n";
    ptr +=".button-off:active {background-color: #2c3e50;}\n";
    ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
    ptr +="</style>\n";
    ptr +="</head>\n";
    ptr +="<body>\n";
    ptr +="<h1>ESP32 Web Server</h1>\n";
    ptr +="<h3>Using Station(STA) Mode</h3>\n";
  
    if (ledState)
    {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";}
    else
    {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";}

    ptr +="</body>\n";
    ptr +="</html>\n";
    return ptr;
}

