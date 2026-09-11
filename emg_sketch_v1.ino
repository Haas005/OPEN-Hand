#include <Servo.h>

Servo myServo;

const int emgPin = A0;
const int servoPin = 9;

int relaxedPos = 15;
int flexedPos  = 165;

// Tune these after viewing serial plotter
int flexThreshold = 450;
int relaxThreshold = 350;

const int samples = 10;
int buffer[samples];
int index = 0;
long sum = 0;

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin);
  myServo.write(relaxedPos);

  for (int i = 0; i < samples; i++) buffer[i] = 0;
}

int filteredRead() {
  sum -= buffer[index];
  buffer[index] = analogRead(emgPin);
  sum += buffer[index];
  index = (index + 1) % samples;
  return sum / samples;
}

void loop() {
  int emg = filteredRead();

  Serial.println(emg);

  if (emg > flexThreshold) {
    myServo.write(flexedPos);
  }
  else if (emg < relaxThreshold) {
    myServo.write(relaxedPos);
  }

  delay(5);
}
