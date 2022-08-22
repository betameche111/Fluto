#include <AccelStepper.h>
#include <Servo.h>

// OTA
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
const char *ssid = "Chaco WiFi";
const char *password = "Je suis une fourmis";

#define pinEnable 14 // Activation du driver/pilote
#define pinStep 33   // Signal de PAS (avancement)
#define pinDir 32    // Direction
#define pinServo 13
#define pinESC 12
#define pinM0 27
#define pinM1 26
#define pinM2 25
#define pinButtonUp 23
#define pinButtonDown 18
#define pinButtonEnter 17
#define pinLedButtonUp 19
#define pinLedButtonDown 5
#define pinLedButtonEnter 16
#define pinEndButton 2

#define SERVO_UP_POSITION 0
#define SERVO_DOWN_POSITION 30
#define ESC_SPEED 9
#define STEP_BY_SECOND 10000
#define ACCELERATION_IN_STEP_PER_SECOND_2 100000

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
int16_t pos = 0;

AccelStepper stepper(AccelStepper::DRIVER, pinStep, pinDir);

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname("Flute");

  ArduinoOTA.begin();

  Serial.begin(115200);
  //  Brushless Motor
  esc.attach(pinESC);
  esc.write(0);

  // Servo
  servo.attach(pinServo, 1000, 2000);
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

void play_star_wars()
{
  start_song();
  play_note(LA4, 1200);
  stepper.setMaxSpeed(STEP_BY_SECOND);
  play_note(RE5, 1200);
  stepper.setMaxSpeed(STEP_BY_SECOND);
  play_note(MI5, 1200);
  play_note(FA5, 300);
  play_note(SOL5, 300);
  play_note(FA5, 1200);
  play_note(LA4, 1000);
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
int i = 0;
void loop()
{
  Serial.print("ok");
  Serial.println(i);
  i = i + 1;
  delay(1000);
  ArduinoOTA.handle();

  if (digitalRead(pinButtonUp) == LOW)
  {
    Serial.println("UP");
    play_imperial_march();
  }
}
