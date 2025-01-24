#include "RobotSystem.h";

Robot robot (
//  PINS:
//    Movement:
//      Wheels | Speed | Direction |
//      _______|_______|___________|
//      Left:  |       |           |
                 0,      0,
//      _______|_______|___________|
//      Right: |       |           |
                 0,      0,
//      _______|_______|___________|

//    Sensors:
//      IRs | Left | Right |
//      ____|______|_______|
              0,     0,
//      ____|______|_______|

//      Ultrasonics | Trigger | Echo |
//      ____________|_________|______|
//      Left:       |         |      |
                      0,        0,
//      ____________|_________|______|
//      Middle:     |         |      |
                      0,        0,
//      ____________|_________|______|
//      Right:      |         |      |
                      0,        0
//      ____________|_________|______|
);

void setup(){
}

void loop() {
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
