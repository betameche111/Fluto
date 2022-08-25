
#include <AccelStepper.h>
#include <Servo.h>
#include <LcdMenu.h>

#define pinEnable 14 // Activation du driver/pilote
#define pinStep 33   // Signal de PAS (avancement)
#define pinDir 32    // Direction
#define pinServo 13
#define pinESC 12
#define pinM0 27
#define pinM1 26
#define pinM2 25
#define pinButtonUp 23
#define pinButtonDown 17
#define pinButtonEnter 18
#define pinLedButtonUp 19
#define pinLedButtonDown 16
#define pinLedButtonEnter 5
#define pinEndButton 2

#define SERVO_UP_POSITION 5
#define SERVO_DOWN_POSITION 30
#define ESC_SPEED 50
#define STEP_BY_SECOND 20000
#define ACCELERATION_IN_STEP_PER_SECOND_2 1000000

#define LCD_ROWS 2
#define LCD_COLS 16

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

#define FULL_RANGE 5346

Servo esc;
Servo servo;

void play_sncf();
void play_jurassik();
void play_star_wars();
void play_imperial_march();
void play_au_clair_de_la_lune();
void calibrate();

// Ecran LCD
extern MenuItem mainMenu[];
MenuItem mainMenu[] = {ItemHeader(),
                       ItemCommand("SNCF", play_sncf),
                       ItemCommand("Jurassik park", play_jurassik),
                       ItemCommand("Star Wars", play_star_wars),
                       ItemCommand("Imperial march", play_imperial_march),
                       ItemCommand("Au clair de la lune", play_au_clair_de_la_lune),
                       ItemCommand("Calibrate", calibrate),
                       ItemFooter()
                      };
LcdMenu menu(LCD_ROWS, LCD_COLS);


AccelStepper stepper(AccelStepper::DRIVER, pinStep, pinDir);

void setup()
{
  Serial.begin(115200);
  //  Brushless Motor
  esc.attach(pinESC);
  esc.write(0);

  // Servo
  servo.attach(pinServo);
  servo.write(SERVO_UP_POSITION);
  servo.write(SERVO_DOWN_POSITION);

  // // StepperMotor
  pinMode(pinM0, OUTPUT);
  pinMode(pinM1, OUTPUT);
  pinMode(pinM2, OUTPUT);
  pinMode(pinStep, OUTPUT);
  pinMode(pinDir, OUTPUT);
  digitalWrite(pinM0, HIGH);
  digitalWrite(pinM1, HIGH);
  digitalWrite(pinM2, LOW);
  pinMode(pinEnable, OUTPUT);
  digitalWrite(pinEnable, HIGH);
  stepper.setMaxSpeed(STEP_BY_SECOND);
  stepper.setAcceleration(ACCELERATION_IN_STEP_PER_SECOND_2); // 100 steps/s²

  // Button
  pinMode(pinButtonUp, INPUT_PULLUP);
  pinMode(pinButtonDown, INPUT_PULLUP);
  pinMode(pinButtonEnter, INPUT_PULLUP);
  pinMode(pinLedButtonUp, OUTPUT);
  pinMode(pinLedButtonDown, OUTPUT);
  pinMode(pinLedButtonEnter, OUTPUT);
  digitalWrite(pinLedButtonUp, HIGH);
  digitalWrite(pinLedButtonDown, HIGH);
  digitalWrite(pinLedButtonEnter, HIGH);

  pinMode(pinEndButton, INPUT_PULLUP);

  menu.setupLcdWithMenu(0x27, mainMenu);

  // Calibration
  calibrate();
}

void calibrate()
{
  int position = 0;
  digitalWrite(pinEnable, LOW);
  while (digitalRead(pinEndButton) == HIGH)
  {
    position = position + 1;
    stepper.runToNewPosition(position);
    delay(1);
  }
  stepper.setCurrentPosition(FULL_RANGE);
  digitalWrite(pinEnable, HIGH);
}

void start_song()
{
  digitalWrite(pinLedButtonUp, LOW);
  digitalWrite(pinLedButtonDown, LOW);
  digitalWrite(pinLedButtonEnter, LOW);
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
  digitalWrite(pinLedButtonUp, HIGH);
  digitalWrite(pinLedButtonDown, HIGH);
  digitalWrite(pinLedButtonEnter, HIGH);
}

void play_note(uint32_t note, uint32_t duration, bool cut)
{
  stepper.runToNewPosition(note);
  servo.write(SERVO_UP_POSITION);
  delay(duration);
  if (cut)
  {
    servo.write(SERVO_DOWN_POSITION);
    delay(60);
  }
}

void play_au_clair_de_la_lune()
{
  start_song();
  play_note(DO5, 300, true);
  play_note(DO5, 300, true);
  play_note(DO5, 300, true);
  play_note(RE5, 300, false);
  play_note(MI5, 600, false);
  play_note(RE5, 600, false);
  play_note(DO5, 300, false);
  play_note(MI5, 300, false);
  play_note(RE5, 300, true);
  play_note(RE5, 300, false);
  play_note(DO5, 600, true);
  stop_song();
}

void play_sncf()
{
  start_song();
  play_note(LA4, 500, true);
  play_note(MI5, 600, true);
  play_note(FA5, 350, false);
  play_note(DO5, 300, true);
  stop_song();
}

void play_jurassik()
{
  start_song();
  play_note(FA5, 400, false);
  play_note(MI5, 400, false);
  play_note(FA5, 600, true);
  play_note(DO5, 600, true);
  play_note(LA4D, 600, true);
  play_note(FA5, 400, false);
  play_note(MI5, 400, false);
  play_note(FA5, 600, true);
  play_note(DO5, 600, true);
  play_note(LA4D, 1000, true);
  stop_song();
}

void play_star_wars()
{
  start_song();
  play_note(LA4, 1200, false);
  stepper.setMaxSpeed(STEP_BY_SECOND/2);
  play_note(RE5, 1200, true);
  stepper.setMaxSpeed(STEP_BY_SECOND);
  play_note(MI5, 1200, false);
  play_note(FA5, 300, false);
  play_note(SOL5, 300, false);
  play_note(FA5, 1200, true);
  play_note(LA4, 1000, true);
  stop_song();
}

void play_imperial_march()
{
  start_song();
  play_note(RE5, 700, true);
  play_note(RE5, 700, true);
  play_note(RE5, 700, true);
  play_note(LA4D, 600, true);
  play_note(FA5, 200, true);
  play_note(RE5, 700, true);
  play_note(LA4D, 600, true);
  play_note(FA5, 200, true);
  play_note(RE5, 700, true);

  stop_song();
}

void loop()
{
  delay(100);

  if (digitalRead(pinButtonDown) == LOW)
  {
    menu.down();
  }
  else if (digitalRead(pinButtonUp) == LOW)
  {
    menu.up();
  }
  else if (digitalRead(pinButtonEnter) == LOW)
  {
    menu.enter();
  }
}
