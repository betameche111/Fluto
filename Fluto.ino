#include "FastAccelStepper.h"
#include "AVRStepperPins.h" // Only required for AVR controllers
#include <ServoTimer2.h>  

#define pinEnable  13 // Activation du driver/pilote
#define pinStep     9 // Signal de PAS (avancement)
#define pinDir      8 // Direction 
#define pinRelay    2
#define pinServo    10
#define rightButton 3
#define leftButton  4
#define motorInterfaceType 1

#define servoUpPosition 1100
#define servoDownPosition 1162

#define SOLD4    12
#define LA4      86
#define LA4D    152
#define SI4     215
#define DO5     272
#define DO5D    327
#define RE5     378
#define RE5D    426
#define MI5     473
#define FA5     519
#define FA5D    561
#define SOL5    612

#define FULL_RANGE 670

ServoTimer2 servo;
int16_t pos = 0;

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setup() {
  engine.init();
  stepper = engine.stepperConnectToPin(pinStep);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, HIGH);
  servo.attach(pinServo);
  if (stepper) {
    stepper->setDirectionPin(pinDir);
   // stepper->setEnablePin(pinEnable);
    //stepper->setAutoEnable(true);
    pinMode(pinEnable, OUTPUT);
    digitalWrite(pinEnable, HIGH);
    stepper->setSpeedInHz(3000);         // 500 steps/s
    stepper->setAcceleration(100000);    // 100 steps/s²
  }
  servo.write(servoDownPosition); 
}

void start_song() {
  servo.write(servoDownPosition); 
  digitalWrite(pinEnable, LOW);
  digitalWrite(pinRelay, LOW);
  delay(2000);
}

void stop_song() {
  servo.write(servoDownPosition); 
  digitalWrite(pinEnable, HIGH);
  digitalWrite(pinRelay, HIGH);
}

void play_note(uint32_t note, uint32_t duration)
{
  static uint32_t lastNote = 0;
  stepper->moveTo(note);
  servo.write(servoUpPosition);
  delay(duration);
  if (lastNote == note )
  {
    servo.write(servoDownPosition); 
    delay(50);
  }
  lastNote = note;
}

void play_sncf()
{
  servo.write(servoDownPosition); 
  stepper->moveTo(LA4);
  delay(500);
  stepper->moveTo(MI5);
  delay(600);
  stepper->moveTo(FA5);
  delay(350);
  stepper->moveTo(DO5);
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

void play_star_wars()
{
  start_song();
  delay(2000);
  stepper->moveTo(LA4);
  stepper->setSpeedInHz(1000);   
  delay(1200);
  stepper->moveTo(RE5);
  stepper->setSpeedInHz(3000); 
  delay(1200);
  stepper->moveTo(MI5);
  delay(1200);
  stepper->moveTo(FA5);
  delay(300);
  stepper->moveTo(SOL5);
  delay(300);
  stepper->moveTo(FA5);
  delay(1200);
  stepper->moveTo(LA4);  
  delay(1000);
  stop_song();
}

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


void loop() {
  if (digitalRead(leftButton) == 0)
  {
    play_imperial_march();
  }
  if (digitalRead(rightButton) == 0 && pos < FULL_RANGE)
  {
    stepper->moveTo(0);
  }
  //stepper->moveTo(pos);
  delay(100);
}
