#include <Servo.h>

Servo myServo;

const int emgPin = A0;
const int servoPin = 9;

int relaxedPos = 15;
int flexedPos  = 165;

// Tunable
int activationDelta = 60;   // how much above baseline = flex
int samplesFast = 8;
int samplesSlow = 60;

int fastBuf[8];
int slowBuf[60];
int fastIdx = 0, slowIdx = 0;
long fastSum = 0, slowSum = 0;

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin);
  myServo.write(relaxedPos);

  for (int i = 0; i < samplesFast; i++) fastBuf[i] = 0;
  for (int i = 0; i < samplesSlow; i++) slowBuf[i] = 0;
}

int fastRead() {
  fastSum -= fastBuf[fastIdx];
  fastBuf[fastIdx] = analogRead(emgPin);
  fastSum += fastBuf[fastIdx];
  fastIdx = (fastIdx + 1) % samplesFast;
  return fastSum / samplesFast;
}

int slowRead(int val) {
  slowSum -= slowBuf[slowIdx];
  slowBuf[slowIdx] = val;
  slowSum += slowBuf[slowIdx];
  slowIdx = (slowIdx + 1) % samplesSlow;
  return slowSum / samplesSlow;
}

void loop() {
  int emgFast = fastRead();
  int baseline = slowRead(emgFast);
  int delta = emgFast - baseline;

  Serial.print("Fast: ");
  Serial.print(emgFast);
  Serial.print("  Base: ");
  Serial.print(baseline);
  Serial.print("  Δ: ");
  Serial.println(delta);

  if (delta > activationDelta) {
    myServo.write(flexedPos);
  } else {
    myServo.write(relaxedPos);
  }

  delay(5);
}

