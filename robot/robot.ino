#include "RobotSystem.h";
#include <WiFiS3.h>
#include <WebSocketServer.h>
#include "index.h"

char ssid[] = "Clark";         //  your network SSID (name) between the " "
char pass[] = "12345678";      // your network password between the " "

using namespace net;

#define CMD_STOP 0
#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_LEFT 4
#define CMD_RIGHT 8

WebSocketServer webSocket(81);
WiFiServer server(80);            //server socket
int status = WL_IDLE_STATUS;      //connection status

const int MotorsEnable = 3;

Robot robot ( MotorsEnable, 
//  PINS:
//    Movement:
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
                      13,        12,
//      ____________|_________|______|
//      Right       |         |      |
                      0,        0
//      ____________|_________|______|

);

void testMovement() {
// test movement
  robot.movement.forward();
  delay(3000);
  robot.movement.brake();
  delay(1000);
  robot.movement.backward();
  delay(3000);
  robot.movement.brake();
  delay(1000);
  robot.movement.rotateLeft();
  delay(1000);
  robot.movement.brake();
  delay(1000);
  robot.movement.rotateRight();
  delay(2000);
  robot.movement.brake();
  delay(1000);
  robot.movement.rotateLeft();
  delay(1000);
  robot.movement.brake();
  delay(1000);
}

void setup(){

  Serial.begin(9600);
  
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    Serial.println("Please upgrade the firmware");

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(pass);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 4 seconds for connection:
    delay(4000);
  }

    // print your board's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("hi");
  webSocket.onConnection([](WebSocket &ws) {
    Serial.println("hoo");

    const auto protocol = ws.getProtocol();
    if (protocol) {
      Serial.print(F("Client protocol: "));
      Serial.println(protocol);
    }

    ws.onMessage([](WebSocket &ws, const WebSocket::DataType dataType,
                    const char *message, uint16_t length) {
      String cmd_str = String((char *)message);
      int command = cmd_str.toInt();
      Serial.print("command: ");
      Serial.println(command);

      switch (dataType) {
        case WebSocket::DataType::TEXT:
          switch (command) {
            case CMD_STOP:
              Serial.println("Stop");
              CAR_stop();
              break;
            case CMD_FORWARD:
              Serial.println("Move Forward");
              CAR_moveForward();
              break;
            case CMD_BACKWARD:
              Serial.println("Move Backward");
              CAR_moveBackward();
              break;
            case CMD_LEFT:
              Serial.println("Turn Left");
              CAR_turnLeft();
              break;
            case CMD_RIGHT:
              Serial.println("Turn Right");
              CAR_turnRight();
              break;

            default:
              Serial.println("Unknown command");
          }

          break;
        case WebSocket::DataType::BINARY:
          Serial.println(F("Received binary data"));
          break;
      }
    });

    ws.onClose([](WebSocket &, const WebSocket::CloseCode, const char *,
                  uint16_t) {
      Serial.println(F("Disconnected"));
    });

    Serial.print(F("New WebSocket Connnection from client: "));
    Serial.println(ws.getRemoteIP());
  });

  webSocket.begin();

}



void loop() {
  webSocket.listen();

  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // read the HTTP request header line by line
    while (client.connected()) {
      if (client.available()) {
        String HTTP_header = client.readStringUntil('\n');  // read the header line of HTTP request

        if (HTTP_header.equals("\r"))  // the end of HTTP request
          break;
        
        Serial.println("yea");

        Serial.print("<< ");
        Serial.println(HTTP_header);  // print HTTP request to Serial Monitor
      }
    }

    // send the HTTP response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();                     // the separator between HTTP header and body

    String html = String(HTML_CONTENT);

    client.println(html);
    client.flush();

    // give the web browser time to receive the data
    delay(100);

    // close the connection:
    client.stop();
  }
}



void CAR_moveForward() {
  robot.movement.forward();
}

void CAR_moveBackward() {
  robot.movement.backward();
}

void CAR_turnLeft() {
  robot.movement.rotateLeft();
}

void CAR_turnRight() {
  robot.movement.rotateRight();
}

void CAR_stop() {
  robot.movement.brake();
}
