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

  Movement(int leftSpeedPin, int leftDirectionPin, int rightSpeedPin, int rightDirectionPin)
      : leftWheels(leftSpeedPin, leftDirectionPin), rightWheels(rightSpeedPin, rightDirectionPin) {}

  void move(int x, int y) {
    leftWheels.setSpeed(x * abs(x) + y * abs(y));
    rightWheels.setSpeed(-x * abs(x) + y * abs(y));
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
  int triggerPin;
  int echoPin;

  int distance;
  
  Ultrasonic(int triggerPin, int echoPin) {
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
};

struct IR {
  int pin;

  bool onGround;

  IR(int pin) {
    this->pin = pin;

    pinMode(pin, INPUT_PULLUP);
  }

  bool isOnGround() {
    onGround = digitalRead(pin) == HIGH;
    return onGround;
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
    : leftIR(leftIRPin), rightIR(rightIRPin),
    leftUltrasonic(leftUltrasonicTriggerPin, leftUltrasonicEchoPin),
    middleUltrasonic(middleUltrasonicTriggerPin, middleUltrasonicEchoPin),
    rightUltrasonic(rightUltrasonicTriggerPin, rightUltrasonicEchoPin) {}
};

struct Robot {
  Movement movement;
  Sensors sensors;

  Robot(int leftSpeedPin, int leftDirectionPin, int rightSpeedPin, int rightDirectionPin,
    int leftIRPin, int rightIRPin, 
    int leftUltrasonicTriggerPin, int leftUltrasonicEchoPin,
    int middleUltrasonicTriggerPin, int middleUltrasonicEchoPin,
    int rightUltrasonicTriggerPin, int rightUltrasonicEchoPin)
    : movement(leftSpeedPin, leftDirectionPin, rightSpeedPin, rightDirectionPin),
    sensors(leftIRPin, rightIRPin,
    leftUltrasonicTriggerPin, leftUltrasonicEchoPin,
    middleUltrasonicTriggerPin, middleUltrasonicEchoPin,
    rightUltrasonicTriggerPin, rightUltrasonicEchoPin) {}
};

Robot robot(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

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
