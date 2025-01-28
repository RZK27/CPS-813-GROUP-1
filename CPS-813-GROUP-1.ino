#include "RobotSystem.h";
#include <WiFiNINA.h>

char ssid[] = "";             //  your network SSID (name) between the " "
char pass[] = "";      // your network password between the " "
int status = WL_IDLE_STATUS;      //connection status
WiFiServer server(80);            //server socket

WiFiClient client = server.available();

Robot robot (
//  PINS:
//    Movement:
//      Wheels | Speed | Direction |
//      _______|_______|___________|
//      Left   |       |           |
                 0,      0,
//      _______|_______|___________|
//      Right  |       |           |
                 0,      0,
//      _______|_______|___________|

//    Sensors:
//      IRs | Left | Right |
//      ____|______|_______|
              0,     0,
//      ____|______|_______|

//      Ultrasonics | Trigger | Echo |
//      ____________|_________|______|
//      Left        |         |      |
                      0,        0,
//      ____________|_________|______|
//      Middle      |         |      |
                      0,        0,
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
  while (!Serial);
  
  enable_WiFi();
  connect_WiFi();

  server.begin();
  printWifiStatus();
}

void loop() {
  // testMovement();
  
  client = server.available();

  if (client) {
    printWEB();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void enable_WiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connect_WiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
}

void printWEB() {

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
           
            // Start of HTML content
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<title>Sensor Data</title>");

            client.println("<style>");
            client.println("body { font-family: Arial, sans-serif; text-align: center; }");
            client.println("input { font-size: 1.2em; padding: 5px; width: 80%; margin-bottom: 10px; }");
            client.println("</style>");

            client.println("<h1>Robot Sensor Data</h1>");

            // Left IR sensor data
            client.print("<label>Left IR:</label><br>");
            client.print("<input type='text' value='");
            client.print(robot.sensors.leftIR.toString());
            client.println("' readonly><br><br>");

            // Right IR sensor data
            client.print("<label>Right IR:</label><br>");
            client.print("<input type='text' value='");
            client.print(robot.sensors.rightIR.toString());
            client.println("' readonly><br><br>");

            // Left Ultrasonic sensor data
            client.print("<label>Left Ultrasonic:</label><br>");
            client.print("<input type='text' value='");
            client.print(robot.sensors.leftUltrasonic.toString());
            client.println("' readonly><br><br>");

            // Middle Ultrasonic sensor data
            client.print("<label>Middle Ultrasonic:</label><br>");
            client.print("<input type='text' value='");
            client.print(robot.sensors.middleUltrasonic.toString());
            client.println("' readonly><br><br>");

            // Right Ultrasonic sensor data
            client.print("<label>Right Ultrasonic:</label><br>");
            client.print("<input type='text' value='");
            client.print(robot.sensors.rightUltrasonic.toString());
            client.println("' readonly><br><br>");
            
            client.println("</body>");
            client.println("</html>");

            // End HTTP response
            client.println();
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // analyze currentLine here for data
        Serial.println(currentLine);

        int leftBracketIndex = currentLine.indexOf('[');
        int rightBracketIndex = currentLine.indexOf(']');

        if (leftBracketIndex != -1 && rightBracketIndex != -1) {
          currentLine = currentLine.substring(leftBracketIndex + 1, rightBracketIndex);        

          String tempNum = "";
          int dataIndex = 0;
          int data[20];

          for (int i = 0; i < currentLine.length(); i++) {

            char c = currentLine[i];

            if (c == ',') {
              data[dataIndex] = tempNum.toInt();
              tempNum = "";
              dataIndex++;
              continue;
            } if (c == ' ') {
              continue;
            } tempNum += c;
          }

          robot.movement.move(data[0], data[1]);

        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
