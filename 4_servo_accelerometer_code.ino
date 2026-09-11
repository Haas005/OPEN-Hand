#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

const int xpin = A0;  // Accelerometer X-axis pin
const int thresholdLow = 380;
const int thresholdHigh = 555;

int direction = 0;         // 1 = CW, -1 = CCW, 0 = stop
long moveUnits = 0;        // Net movement from origin

unsigned long lastTrackTime = 0;
const int trackingInterval = 100; // ms between movement updates

void setup() {
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);

  stopAllServos();  // Start idle
}

void loop() {
  int accelVal = analogRead(xpin);
  unsigned long now = millis();

  // If sensor is tilted → rotate accordingly
  if (accelVal > thresholdHigh) {
    rotateCW();
    direction = 1;

    if (now - lastTrackTime >= trackingInterval) {
      moveUnits += direction;
      lastTrackTime = now;
    }
  }
  else if (accelVal < thresholdLow) {
    rotateCCW();
    direction = -1;

    if (now - lastTrackTime >= trackingInterval) {
      moveUnits += direction;
      lastTrackTime = now;
    }
  }
  // Sensor is in neutral zone
  else {
    // If we're not at origin, return to origin
    if (moveUnits > 0) {
      rotateCCW();
      direction = -1;

      if (now - lastTrackTime >= trackingInterval) {
        moveUnits += direction;
        lastTrackTime = now;
      }
    }
    else if (moveUnits < 0) {
      rotateCW();
      direction = 1;

      if (now - lastTrackTime >= trackingInterval) {
        moveUnits += direction;
        lastTrackTime = now;
      }
    }
    else {
      // At origin, and sensor is neutral → STOP
      stopAllServos();
      direction = 0;
    }
  }

  delay(10);  // For smoothing
}

// === Helper functions ===
void rotateCW() {
  servo1.write(110);
  servo2.write(110);
  servo3.write(110);
  servo4.write(110);
}

void rotateCCW() {
  servo1.write(70);
  servo2.write(70);
  servo3.write(70);
  servo4.write(70);
}

void stopAllServos() {
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
}
