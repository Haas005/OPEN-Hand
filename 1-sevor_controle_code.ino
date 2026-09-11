#include <Servo.h>  // add servo library

Servo myservo;  // create servo object to control a servo

int xpin = A0;  // analog pin used to connect the Accelerometer
int pos;    // variable to read the value from the analog pin

// Threshold values for stopping the motor completely
int thresholdLow = 380;  // Lower bound for minimal rotation
int thresholdHigh = 555; // Upper bound for maximal rotation

void setup() {
  myservo.attach(2);  // attaches the servo on pin D2 to the servo
}

void loop() {
  pos = analogRead(xpin);  // reads the value of the accelerometer (value between 0 and 1023)

  // Check if the accelerometer is rotated past a threshold
  if (pos < thresholdLow || pos > thresholdHigh) {
    // If the accelerometer is rotated past the threshold, stop the motor (servo)
    myservo.write(90);  // Stop the motor by setting the servo to a neutral position
  } else {
    // If the accelerometer is within the normal range, map and move the motor (servo)
    pos = map(pos, thresholdLow, thresholdHigh, 0, 180);
    myservo.write(pos);  // sets the servo position according to the scaled value
  }
  
  delay(15);  // waits for the servo to get there
}

