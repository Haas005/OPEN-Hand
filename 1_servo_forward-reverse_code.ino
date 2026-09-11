#include <Servo.h>

Servo myservo;

const int xpin = A0;  // Accelerometer X-axis pin
const int thresholdLow = 380;
const int thresholdHigh = 555;

int direction = 0;         // 1 = CW, -1 = CCW, 0 = stop
long moveUnits = 0;        // Net movement away from starting point

unsigned long lastTrackTime = 0;
const int trackingInterval = 100; // Time (ms) between movement unit updates

void setup() {
  myservo.attach(2);
  myservo.write(90);  // Stop motor
}

void loop() {
  int accelVal = analogRead(xpin);
  unsigned long now = millis();

  // Detect tilt and set movement direction
  if (accelVal < thresholdLow) {
    myservo.write(70);  // Rotate CCW
    direction = -1;
  } else if (accelVal > thresholdHigh) {
    myservo.write(110); // Rotate CW
    direction = 1;
  } else {
    // Neutral: return to origin based on movement history
    if (moveUnits > 0) {
      myservo.write(70);  // Reverse CCW
      direction = -1;
    } else if (moveUnits < 0) {
      myservo.write(110); // Reverse CW
      direction = 1;
    } else {
      myservo.write(90);  // Already back at start
      direction = 0;
    }
  }

  // Update movement tracking at intervals
  if (direction != 0 && (now - lastTrackTime >= trackingInterval)) {
    moveUnits += direction;
    lastTrackTime = now;
  }

  // Stop exactly when movement is back to 0
  if ((direction == 1 && moveUnits >= 0) ||
      (direction == -1 && moveUnits <= 0)) {
    if (accelVal >= thresholdLow && accelVal <= thresholdHigh) {
      myservo.write(90);  // Stop only in neutral zone
      direction = 0;
    }
  }

  delay(10);  // Loop smoothing
}
