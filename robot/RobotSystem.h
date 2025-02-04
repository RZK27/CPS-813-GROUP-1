#ifndef _ROBOTSYSTEM_H
#define _ROBOTSYSTEM_H

struct Wheels {
  int speedPin;
  int directionPin;

  int speed;
  int direction;
  
  Wheels(int speedPin, int directionPin) {
    this->speedPin = speedPin;
    this->directionPin = directionPin;

    speed = 0;
    direction = HIGH;

    pinMode(speedPin, OUTPUT);
    pinMode(directionPin, OUTPUT);
  }

  void setSpeed(int speed) {
    if (speed > 0) {
      direction = HIGH;
    } else {
      direction = LOW;
    } speed = abs(speed);

    digitalWrite(directionPin, direction);
    analogWrite(speedPin, speed);
  }
};

struct Movement {
  Wheels leftWheels;
  Wheels rightWheels;

  Movement(int enableMovement, int leftSpeedPin, int leftDirectionPin, int rightSpeedPin, int rightDirectionPin) 
      : leftWheels(leftSpeedPin, leftDirectionPin), rightWheels(rightSpeedPin, rightDirectionPin) {
        pinMode(enableMovement, OUTPUT);

        digitalWrite(enableMovement, HIGH); }

  // --MAKE COMPATIBLE WITH CONTROLLER
  void move(int x, int y) {
    int leftSpeed = x * abs(x) + y * abs(y);
    int rightSpeed = -x * abs(x) + y * abs(y);

    leftSpeed = 15 * leftSpeed / abs(leftSpeed) * sqrt(abs(leftSpeed));
    rightSpeed = 15 * rightSpeed / abs(rightSpeed) * sqrt(abs(rightSpeed));

    leftWheels.setSpeed(leftSpeed);
    rightWheels.setSpeed(rightSpeed);
  }

  void brake() {
    move(0, 0);
  }

  void forward() {
    move(0, 7);
  }

  void backward() {
    move(0, -7);
  }

  void rotateLeft() {
    move(-7, 0);
  }

  void rotateRight() {
    move(7, 0);
  }
};

struct Ultrasonic {
  String name;
  int triggerPin;
  int echoPin;
  int distance;
  
  Ultrasonic(String name, int triggerPin, int echoPin) {
    this->name = name;
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  int getDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    distance = pulseIn(echoPin, HIGH, 5800) * 0.017;
    
    if (distance == 0) {
      distance = 100;
    }
    return distance;
  }

  String toString() {
    String toReturn = "";
    toReturn.concat(name);
    toReturn.concat(": ");
    toReturn.concat(String(distance));
    toReturn.concat(" cm -> ");

    if (distance > 10){
      toReturn.concat("🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩");
    } else {
      for (int i = 0; i < distance; i++){
        toReturn.concat("🟥");
      }
    }
    return toReturn;
  }
};

struct IR {
  String name;
  int pin;
  bool onGround;

  IR(String name, int pin) {
    this->name = name;
    this->pin = pin;

    pinMode(pin, INPUT_PULLUP);
  }

  // might be wrong
  bool isOnGround() {
    onGround = digitalRead(pin) == HIGH;
    return onGround;
  }

  String toString() {
    String toReturn = "";
    toReturn.concat(name);
    toReturn.concat(": ");

    if (isOnGround()) {
      toReturn.concat("Clear!");
    } else {
      toReturn.concat("DANGER!");
    }
    return toReturn;
  }
};

struct Sensors {
  IR leftIR;
  IR rightIR;

  Ultrasonic leftUltrasonic;
  Ultrasonic middleUltrasonic;
  Ultrasonic rightUltrasonic;

  Sensors(int leftIRPin, int rightIRPin, 
    int leftUltrasonicTriggerPin, int leftUltrasonicEchoPin,
    int middleUltrasonicTriggerPin, int middleUltrasonicEchoPin,
    int rightUltrasonicTriggerPin, int rightUltrasonicEchoPin)
    : leftIR("_LEFT IR", leftIRPin), rightIR("RIGHT IR: ", rightIRPin),
    leftUltrasonic("__LEFT ULTRASONIC", leftUltrasonicTriggerPin, leftUltrasonicEchoPin),
    middleUltrasonic("MIDDLE ULTRASONIC", middleUltrasonicTriggerPin, middleUltrasonicEchoPin),
    rightUltrasonic("_RIGHT ULTRASONIC", rightUltrasonicTriggerPin, rightUltrasonicEchoPin) {}
};

struct Robot {
  Movement movement;
  Sensors sensors;

  Robot(int enableMovement, int leftSpeedPin, int leftDirectionPin, int rightSpeedPin, int rightDirectionPin,
    int leftIRPin, int rightIRPin, 
    int leftUltrasonicTriggerPin, int leftUltrasonicEchoPin,
    int middleUltrasonicTriggerPin, int middleUltrasonicEchoPin,
    int rightUltrasonicTriggerPin, int rightUltrasonicEchoPin)
    : movement(enableMovement, leftSpeedPin, leftDirectionPin, rightSpeedPin, rightDirectionPin),
    sensors(leftIRPin, rightIRPin,
    leftUltrasonicTriggerPin, leftUltrasonicEchoPin,
    middleUltrasonicTriggerPin, middleUltrasonicEchoPin,
    rightUltrasonicTriggerPin, rightUltrasonicEchoPin) {}
};

#endif // _ROBOTSYSTEM_H