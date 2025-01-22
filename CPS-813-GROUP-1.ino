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
  // put your main code here, to run repeatedly:
  move(0, 10);
}

// moves robot according to x (rotation) and y (forward or backward) arguments
// x and y are expected to be between -50 and 50 (inclusive)
void move(int x, int y){
    int left_speed = -x + y;
    int right_speed = x + y;

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