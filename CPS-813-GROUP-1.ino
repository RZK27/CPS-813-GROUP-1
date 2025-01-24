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

void setSpeed(int speed, int speed_pin, int direction_pin){
    if (speed > 0) {
      digitalWrite(direction_pin, HIGH);
  } else {
      digitalWrite(direction_pin, LOW);
  } analogWrite(speed_pin, abs(speed));
}

// --add optional argument for time spent doing action (do not use delay)
// moves robot according to x (rotation) and y (forward or backward) arguments
// x and y are expected to be between -10 and 10 (inclusive)
// the speeds of move will end up between -100 and 100
void move(int x, int y){
  setSpeed(x * abs(x) + y * abs(y), left_speed_pin, left_direction_pin);
  setSpeed(-x * abs(x) + y * abs(y), right_speed_pin, right_direction_pin);
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

void readIR(){

}

int readUltrasonic(){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  int distance = pulseIn(echo, HIGH, 5800) * 0.017;
  return distance;
}
