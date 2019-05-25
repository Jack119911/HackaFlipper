enum PaddlePosition {
  UP,
  DOWN,
  MOVING
};

// servo pin
int servo1 = 8;
int servo2 = 6;

// variables for servo control
int Servo_pos_down = 500;
int Servo_pos_up = -500;
int Servo_pos1 = Servo_pos_down;
int Servo_pos2 = Servo_pos_down;
int microsToMove = 250000;
int microsAfterLastPosChange1 = 0;
int microsAfterLastPosChange2 = 0;
PaddlePosition currentPaddlePosition1 = MOVING;
PaddlePosition desiredPaddlePosition1 = UP;
PaddlePosition currentPaddlePosition2 = MOVING;
PaddlePosition desiredPaddlePosition2 = DOWN;

void servoMove(int servo, int Servo_pos){
  int pwm = (Servo_pos) + 1500;
  digitalWrite(servo, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(servo, LOW);
}

void setup() {
  //communications
    Serial.begin(9600);          // start serial for output
    delay(10);
  //servo1
    pinMode(servo1, OUTPUT);
  //servo2
    pinMode(servo2, OUTPUT);
  //button1
    pinMode(5, INPUT);
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);
  //button2
    pinMode(22, INPUT);
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

  delay(100);
  Serial.println("Setup Done");
}

void loop() {
  //move Servo
    servoMove(servo1, Servo_pos1); 
    servoMove(servo2, Servo_pos2); 
    delay(14);
    
  //servo1
  if (currentPaddlePosition1 == MOVING) {
    microsAfterLastPosChange1 += 50000;
    checkIfPaddleArrived1();
  } else {
    checkForPaddleChange1();
  }
  if (getPressed(5)) {
    desiredPaddlePosition1 = DOWN;
  } else {
    desiredPaddlePosition1 = UP;
  }
  //servo2
  if (currentPaddlePosition2 == MOVING) {
    microsAfterLastPosChange2 += 50000;
    checkIfPaddleArrived2();
  } else {
    checkForPaddleChange2();
  }
  if (getPressed(22)) {
    desiredPaddlePosition2 = UP;
  } else {
    desiredPaddlePosition2 = DOWN;
  }
}

void checkForPaddleChange1() {
  if (desiredPaddlePosition1 != currentPaddlePosition1) {
    changePaddlePosition1();
  }
}

void checkForPaddleChange2() {
  if (desiredPaddlePosition2 != currentPaddlePosition2) {
    changePaddlePosition2();
  }
}

void changePaddlePosition1() {
  if (desiredPaddlePosition1 == UP) {
    Servo_pos1 = Servo_pos_up;
  } else if (desiredPaddlePosition1 == DOWN) {
    Servo_pos1 = Servo_pos_down;
  }
  currentPaddlePosition1 = MOVING;
}

void changePaddlePosition2() {
  if (desiredPaddlePosition2 == UP) {
    Servo_pos2 = Servo_pos_up;
  } else if (desiredPaddlePosition2 == DOWN) {
    Servo_pos2 = Servo_pos_down;
  }
  currentPaddlePosition2 = MOVING;
}

void checkIfPaddleArrived1() {
  if (microsAfterLastPosChange1 >= microsToMove) {
    if (Servo_pos1 == Servo_pos_down) {
      currentPaddlePosition1 = DOWN;
    } else {
      currentPaddlePosition1 = UP;
    }
    microsAfterLastPosChange1 = 0;
  }
}

void checkIfPaddleArrived2() {
  if (microsAfterLastPosChange2 >= microsToMove) {
    if (Servo_pos2 == Servo_pos_down) {
      currentPaddlePosition2 = DOWN;
    } else {
      currentPaddlePosition2 = UP;
    }
    microsAfterLastPosChange2 = 0;
  }
}

bool getPressed(uint8_t pin){
  uint8_t highs = 0;
  for(int i = 0; i < 8; ++i){
    highs += digitalRead(pin);
    delay(2);
  }
  return (highs > 7);
}

