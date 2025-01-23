const int right_speed_pin = 0;
const int left_speed_pin = 0;
const int right_direction_pin = 0;
const int left_direction_pin = 0;

void setup(){
    pinMode(right_speed_pin, OUTPUT);
    pinMode(left_speed_pin, OUTPUT);
    pinMode(right_direction_pin, OUTPUT);
    pinMode(left_direction_pin, OUTPUT);
}

void loop() {
  // test movement
  forward();
  delay(3000);
  brake();
  delay(1000);
  backward();
  delay(3000);
  brake();
  delay(1000);
  rotateLeft();
  delay(1000);
  brake();
  delay(1000);
  rotateRight();
  delay(2000);
  brake();
  delay(1000);
  rotateLeft();
  delay(1000);
  brake();
  delay(1000);
}

//--MOVEMENT----

// --add optional argument for time spent doing action (do not use delay)
// moves robot according to x (rotation) and y (forward or backward) arguments
// x and y are expected to be between -10 and 10 (inclusive)
// the speeds of move will end up between -100 and 100
void move(int x, int y){
    int left_speed = x * x + y * y;
    int right_speed = -x * x + y * y;

    if (left_speed > 0) {
        digitalWrite(left_direction_pin, HIGH);
        analogWrite(left_speed_pin, left_speed);
    } else {
        digitalWrite(left_direction_pin, LOW);
        analogWrite(left_speed_pin, -left_speed);
    }

    if (right_speed > 0) {
        digitalWrite(right_direction_pin, HIGH);
        analogWrite(right_speed_pin, right_speed);
    } else {
        digitalWrite(right_direction_pin, LOW);
        analogWrite(right_speed_pin, -right_speed);
    }
}

void brake(){
  move(0, 0);
}

void forward(){
  move(0, 7);
}

void backward(){
  move(0, -7);
}

void rotateLeft(){
  move(-7, 0);
}

void rotateRight(){
  move(7, 0);
}

//--SENSORS----

void readIR();

void readUltrasonic();
