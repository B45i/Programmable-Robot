int buttonStart   = 2;
int buttonStop    = 3;
int buttonLeft    = 4;
int buttonRight   = 5;
int buttonFwd     = 6;
int buttonBwd     = 7;
int buttonReset   = 8;

int leftM1  = 9;
int leftM2  = 10;
int rightM1 = 11;
int rightM2 = 12;

int delayTime = 5000; // Change this according to motor

int debounceDelay   = 250;

unsigned long lastStartTime = 0;
unsigned long lastStopTime  = 0;
unsigned long lastLeftTime  = 0;
unsigned long lastRightTime = 0;
unsigned long lastFwdTime   = 0;
unsigned long lastBwdTime   = 0;
unsigned long lastRestTime  = 0;

int stack[100];
int stackTop = 0;
int stopFlag = 0;


void motorStop() {
  //Serial.println("Stopping...");
  digitalWrite(leftM1, LOW);
  digitalWrite(leftM2, LOW);
  digitalWrite(rightM1, LOW);
  digitalWrite(rightM2, LOW);
}

void goForward() {
  motorStop();
  Serial.println("Going FWD");
  digitalWrite(leftM1, HIGH);
  digitalWrite(leftM2, LOW);
  digitalWrite(rightM1, HIGH);
  digitalWrite(rightM2, LOW);
  delay(delayTime);
}

void goLeft() {
  motorStop();
  Serial.println("Going Left");
  digitalWrite(rightM1, HIGH);
  digitalWrite(rightM2, LOW);
  delay(delayTime);
}

void goRight() {
  motorStop();
  Serial.println("Going Right");
  digitalWrite(leftM1, HIGH);
  digitalWrite(leftM2, LOW);
  delay(delayTime);
}

void goBackward() {
  motorStop();
  Serial.println("Going Backwards");
  digitalWrite(leftM1, LOW);
  digitalWrite(leftM2, HIGH);
  digitalWrite(rightM1, LOW);
  digitalWrite(rightM2, HIGH);
  delay(delayTime);
}

void resetBot() {

  for(int i=0;i<stackTop;i++) {
    stack[stackTop] = 0;
  }
  stackTop = 0;
  Serial.println("Sequence reseted...");
}

void startRobot() {
  Serial.println("Bot stated playing Sequence...");

  for(int i=0;(i<stackTop) && (stopFlag != 1);i++){
    switch(stack[i]) {
      case 1:
        goForward();
        break;
      case 2:
        goBackward();
        break;
      case 3:
        goLeft();
        break;
      case 4:
        goRight();
        break;
    }
    resetBot();
    motorStop();
  }
  Serial.println("Sequence Finished..");
}

void handleStopButton() {
  if(millis() - lastStopTime > debounceDelay) {
    lastStopTime = millis();
    Serial.println("Stopping execution...");
    stopFlag = 1;
  }
}

void startSerial(int baud) {

  Serial.begin(baud);

  Serial.println(" ");
  Serial.println(" ");
  Serial.println("   _____             __             __       ");
  Serial.println("  / ___/____ _____  / /_____  _____/ /_      ");
  Serial.println("  \\__ \\/ __ `/ __ \\/ __/ __ \\/ ___/ __ \\     ");
  Serial.println(" ___/ / /_/ / / / / /_/ /_/ (__  ) / / /     ");
  Serial.println("/____/\\__,_/_/ /_/\\__/\\____/____/_/ /_/      ");
  Serial.println("                                            ");
  Serial.println(" ");
  Serial.println(" ");
}


void setup() {

  pinMode(leftM1, OUTPUT);
  pinMode(leftM2, OUTPUT);
  pinMode(rightM1, OUTPUT);
  pinMode(rightM2, OUTPUT);

  pinMode(buttonStart,  INPUT_PULLUP);
  pinMode(buttonStop,   INPUT_PULLUP);
  pinMode(buttonLeft,   INPUT_PULLUP);
  pinMode(buttonRight,  INPUT_PULLUP);
  pinMode(buttonFwd,    INPUT_PULLUP);
  pinMode(buttonBwd,    INPUT_PULLUP);
  pinMode(buttonReset,  INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(buttonStop), handleStopButton, LOW);

  startSerial(9600);

}

void loop() {
  //Program the bot

  int reading;

  reading = digitalRead(buttonFwd);
  if(reading == LOW && (millis() - lastFwdTime) > debounceDelay) {
    lastFwdTime = millis();
    stack[stackTop++] = 1;
    Serial.println("FWD movement set");
  }

  reading = digitalRead(buttonBwd);
  if(reading == LOW && (millis() - lastBwdTime) > debounceDelay) {
    lastBwdTime = millis();
    stack[stackTop++] = 2;
    Serial.println("BWD movement set");
  }

  reading = digitalRead(buttonLeft);
  if(reading == LOW && (millis() - lastLeftTime) > debounceDelay) {
    lastLeftTime = millis();
    stack[stackTop++] = 3;
    Serial.println("Left movement set");
  }

  reading = digitalRead(buttonRight);
  if(reading == LOW && (millis() - lastRightTime) > debounceDelay) {
    lastRightTime = millis();
    stack[stackTop++] = 4;
    Serial.println("Right movement set");
  }

  //Start robot
  reading = digitalRead(buttonStart);
  if(reading == LOW && (millis() - lastStartTime) > debounceDelay) {
    lastStartTime = millis();
    stopFlag = 0;
    startRobot();
  }

  //Clear stack
  reading = digitalRead(buttonReset);
  if(reading == LOW && (millis() - lastRestTime) > debounceDelay) {
    lastRestTime = millis();
    resetBot();
  }
}

