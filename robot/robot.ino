#include "WiFiS3.h"
#include <WebSocketServer.h>
#include <ArduinoJson.h>
#include "RobotSystem.h"
#include <Servo.h>

Servo camServo;
// Servo camServo2;

char ssid[] = "Spark";
char pass[] = "12345678";

int status = WL_IDLE_STATUS;

Robot robot = Robot(
            //  PINS:
            //    Movement:
            //      Enable:
                      3,

            //      Wheels | Speed | Direction |
            //      _______|_______|___________|
            //      Left   |       |           |
                              6,      8,
            //      _______|_______|___________|
            //      Right  |       |           |
                              5,      7,
            //      _______|_______|___________|

            //    Sensors:
            //      IRs | Left | Right |
            //      ____|______|_______|
                           2,     4,
            //      ____|______|_______|

            //      Ultrasonics | Trigger | Echo |
            //      ____________|_________|______|
            //      Left        |         |      |
                                   0,        0,
            //      ____________|_________|______|
            //      Middle      |         |      |
                                   13,       12,
            //      ____________|_________|______|
            //      Right       |         |      |
                                   0,        0
            //      ____________|_________|______|
);

using namespace net;
WebSocketServer server{80};

void setup() {
  //Initialize serial and wait for port to open:
  // Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  camServo.attach(10);
  camServo.write(90);
  // camServo2.attach(11);
  // camServo.write(90);

  for (int pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees

    // in steps of 1 degree

    // camServo2.write(pos);              // tell servo to go to position in variable 'pos'

    // delay(15);                       // waits 15ms for the servo to reach the position

  }

  Serial.println("Access Point Web Socket");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  WiFi.config(IPAddress(192,48,56,2));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

  
  server.onConnection([](WebSocket &ws) {    // where the magic happens
    ws.onMessage([](WebSocket &ws, const WebSocket::DataType dataType, const char *message, uint16_t length) {
      handleWebSocketMessage(message, robot);
       // Assume the ultrasonic sensor reading is an integer
        int ultrasonicReading = robot.sensors.middleUltrasonic.getDistance();

        // Send the integer as binary data
        ws.send(WebSocket::DataType::BINARY, (const char*)&ultrasonicReading, sizeof(ultrasonicReading));
    });
  });

  server.begin();
}

void loop() {
  server.listen();
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

void handleWebSocketMessage(const char *message, Robot robot) {
  StaticJsonDocument<200> jsonDoc;

  // Attempt to parse the JSON string
  DeserializationError error = deserializeJson(jsonDoc, message);
  if (error) {
    Serial.print(F("JSON parsing failed: "));
    Serial.println(error.c_str());
    return;
  }

  // Extract values from the JSON document
  int x = jsonDoc["ls_x"];
  int y = jsonDoc["ls_y"];
  int cam = jsonDoc["rs_x"];
  // int cam2 = jsonDoc["rs_y"];
  y = -y;
  bool button_a = jsonDoc["button_a"];

  Serial.print("X: ");
  Serial.println(x);
  Serial.print("Y: ");
  Serial.println(y);
  Serial.print("A Pressed: ");
  Serial.println(button_a ? "Yes" : "No");

  robot.movement.move(x, y);
  camServo.write(cam + 90);
  // camServo2.write(cam2 + 90);

}
