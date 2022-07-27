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

#define SERVO_UP_POSITION 0
#define SERVO_DOWN_POSITION 30
#define ESC_SPEED 9
#define STEP_BY_SECOND 22000
#define ACCELERATION_IN_STEP_PER_SECOND_2 1000000

#define LA4 150
#define LA4D 680
#define SI4 1240
#define DO5 1730
#define DO5D 2200
#define RE5 2630
#define RE5D 3030
#define MI5 3390
#define FA5 3705
#define FA5D 4025
#define SOL5 4300
#define SOL5D 4560
#define LA5 4790
#define LA5D 5000
#define SI5 5200

#define FULL_RANGE 670

Servo esc;
Servo servo;
int16_t pos = 0;

AccelStepper stepper(AccelStepper::DRIVER, pinStep, pinDir);

void setup()
{
  Serial.begin(115200);
  // Brushless Motor
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  esc.setPeriodHertz(50); // standard 50 hz servo
  esc.attach(pinESC, 1000, 2000);
  esc.write(0);

  // Servo
  servo.setPeriodHertz(50);
  servo.attach(pinServo, 1000, 2000);
  servo.write(SERVO_UP_POSITION);

  pinMode(pinM0, OUTPUT);
  pinMode(pinM1, OUTPUT);
  pinMode(pinM2, OUTPUT);
  digitalWrite(pinM0, HIGH);
  digitalWrite(pinM1, HIGH);
  digitalWrite(pinM2, LOW);

  pinMode(pinEnable, OUTPUT);
  digitalWrite(pinEnable, HIGH);
  stepper.setMaxSpeed(STEP_BY_SECOND);
  stepper.setAcceleration(ACCELERATION_IN_STEP_PER_SECOND_2); // 100 steps/s²

  servo.write(SERVO_DOWN_POSITION);
}

void start_song()
{
  servo.write(SERVO_DOWN_POSITION);
  digitalWrite(pinEnable, LOW);
  esc.write(ESC_SPEED);
  delay(2000);
}

void stop_song()
{
  servo.write(SERVO_DOWN_POSITION);
  digitalWrite(pinEnable, HIGH);
  esc.write(0);
}

void play_note(uint32_t note, uint32_t duration)
{
  static uint32_t lastNote = 0;
  stepper.runToNewPosition(note);
  servo.write(SERVO_UP_POSITION);
  delay(duration);
  if (lastNote == note)
  {
    servo.write(SERVO_DOWN_POSITION);
    delay(50);
  }
  lastNote = note;
}

void play_sncf()
{
  start_song();
  play_note(LA4, 500);
  play_note(MI5, 600);
  play_note(FA5, 350);
  play_note(DO5, 300);
  stop_song();
}

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

void loop()
{
  long value = Serial.parseInt();
  if (value == 1)
  {
    play_jurassik();
  }
  else if (value == 2)
  {
    play_sncf();
  }
}
