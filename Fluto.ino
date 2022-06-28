#include <AccelStepper.h>
#include <ESP32Servo.h>

#define pinEnable 14 // Activation du driver/pilote
#define pinStep 33   // Signal de PAS (avancement)
#define pinDir 32    // Direction
#define pinServo 13
#define pinESC 12
#define pinM0 27
#define pinM1 26
#define pinM2 25

#define servoUpPosition 10
#define servoDownPosition 30

#define LA4 120
#define LA4D 2500
#define SI4 4700
#define DO5 6800
#define DO5D 8500
#define RE5 10100
#define RE5D 11800
#define MI5 473
#define FA5 519
#define FA5D 561
#define SOL5 612

#define FULL_RANGE 670

Servo esc;
Servo servo;
int16_t pos = 0;

AccelStepper stepper(AccelStepper::DRIVER, pinStep, pinDir);

void setup()
{
  Serial.begin(115200);
  delay(5000);
  Serial.println("Start");

  // Brushless Motor
  // ESP32PWM::allocateTimer(0);
  // ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  esc.setPeriodHertz(50); // standard 50 hz servo
  esc.attach(pinESC, 1000, 2000);
  esc.write(0);

  // Servo
  servo.setPeriodHertz(50);
  servo.attach(pinServo, 1000, 2000);
  servo.write(servoUpPosition);

  pinMode(pinM0, OUTPUT);
  pinMode(pinM1, OUTPUT);
  pinMode(pinM2, OUTPUT);
  digitalWrite(pinM0, HIGH);
  digitalWrite(pinM1, HIGH);
  digitalWrite(pinM2, HIGH);

  pinMode(pinEnable, OUTPUT);
  digitalWrite(pinEnable, HIGH);
  stepper.setMaxSpeed(100000);
  stepper.setAcceleration(1000); // 100 steps/s²

  servo.write(servoDownPosition);
  Serial.println("End Setup");
}

void start_song()
{
  // servo.write(servoDownPosition);
  digitalWrite(pinEnable, LOW);
  // digitalWrite(pinRelay, LOW);
  delay(2000);
}

void stop_song()
{
  // servo.write(servoDownPosition);
  digitalWrite(pinEnable, HIGH);
  // digitalWrite(pinRelay, HIGH);
}

void play_note(uint32_t note, uint32_t duration)
{
  static uint32_t lastNote = 0;
  // stepper->moveTo(note);
  //  servo.write(servoUpPosition);
  delay(duration);
  if (lastNote == note)
  {
    // servo.write(servoDownPosition);
    delay(50);
  }
  lastNote = note;
}

// void play_sncf()
// {
//   // servo.write(servoDownPosition);
//   stepper->moveTo(LA4);
//   delay(500);
//   stepper->moveTo(MI5);
//   delay(600);
//   stepper->moveTo(FA5);
//   delay(350);
//   stepper->moveTo(DO5);
// }

void play_jurassik()
{
  start_song();
  play_note(FA5, 400);
  play_note(MI5, 400);
  play_note(FA5, 600);
  play_note(DO5, 600);
  play_note(LA4D, 600);
  play_note(FA5, 400);
  play_note(MI5, 400);
  play_note(FA5, 600);
  play_note(DO5, 600);
  play_note(LA4D, 1000);
  stop_song();
}

// void play_star_wars()
// {
//   start_song();
//   delay(2000);
//   stepper->moveTo(LA4);
//   stepper->setSpeedInHz(1000);
//   delay(1200);
//   stepper->moveTo(RE5);
//   stepper->setSpeedInHz(3000);
//   delay(1200);
//   stepper->moveTo(MI5);
//   delay(1200);
//   stepper->moveTo(FA5);
//   delay(300);
//   stepper->moveTo(SOL5);
//   delay(300);
//   stepper->moveTo(FA5);
//   delay(1200);
//   stepper->moveTo(LA4);
//   delay(1000);
//   stop_song();
// }

void play_imperial_march()
{
  start_song();
  play_note(RE5, 800);
  play_note(RE5, 800);
  play_note(RE5, 800);
  play_note(LA4D, 700);
  play_note(FA5, 250);
  play_note(RE5, 800);
  play_note(LA4D, 700);
  play_note(FA5, 250);
  play_note(RE5, 800);

  stop_song();
}

long motorValue = 9;

void loop()
{
  long value = Serial.parseInt();
  if (value < 0)
  {
    motorValue = -value;
  }
  else if (value != 0)
  {
    digitalWrite(pinEnable, LOW);
    Serial.print("Value : ");
    Serial.println(value);
    stepper.runToNewPosition(value);
    servo.write(servoDownPosition);
    esc.write(motorValue);
    delay(5000);
    servo.write(servoUpPosition);
    delay(5000);
    esc.write(0);
    servo.write(servoDownPosition);
    digitalWrite(pinEnable, HIGH);
  }
  // if (digitalRead(leftButton) == 0)
  // {
  //   play_imperial_march();
  // }
  // if (digitalRead(rightButton) == 0 && pos < FULL_RANGE)
  // {
  //   stepper->moveTo(0);
  // }
  // // stepper->moveTo(pos);
  // delay(100);
}
