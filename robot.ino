/*
  Object avoiding robot
  
  version 1.0
  
  
*/

#include <Servo.h>
#include <NewPing.h>

#define SERVO_PIN    2    // Arduino pin 2 tied to the servo
#define TRIGGER_PIN  A0   // Arduino pin A0 tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A1   // Arduino pin A1 tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 150  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define MIN_DISTANCE 15
#define MOTOR_LEFT_ENABLE_PIN     13
#define MOTOR_LEFT_FORWARD_PIN    11
#define MOTOR_LEFT_BACKWARD_PIN   12
#define MOTOR_RIGHT_ENABLE_PIN    5      
#define MOTOR_RIGHT_FORWARD_PIN   6
#define MOTOR_RIGHT_BACKWARD_PIN  7

#define TURN_TIMER      1000
#define BACKWARD_TIMER  200

Servo myservo;  // create servo object to control a servo
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // create sonar object

int pos = 0;    // keep track of the position
int mindist = 255; 
int dist = 0;
bool backward = false;

int getDistanceAndSend2Serial(int angle) {
  myservo.write(angle);             // tell servo to go to position in variable 'pos'
  delay(250);                     // delay 0.5 sec to let the servo move  
  
  int cm = sonar.ping_cm();       // send ping and get distance
  Serial.print(angle, DEC);
  Serial.print(",");
  Serial.println(cm, DEC);
  return cm;  
}

void motorStop() {
  digitalWrite(MOTOR_RIGHT_ENABLE_PIN, LOW);
  digitalWrite(MOTOR_LEFT_ENABLE_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_RIGHT_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_FORWARD_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_ENABLE_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_ENABLE_PIN, HIGH);

  delay(TURN_TIMER);
  motorStop();
}

void turnLeft() {
  digitalWrite(MOTOR_RIGHT_FORWARD_PIN, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD_PIN, HIGH);
  digitalWrite(MOTOR_RIGHT_ENABLE_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_ENABLE_PIN, HIGH);

  delay(TURN_TIMER);
  motorStop();
}

void goForward() {
  digitalWrite(MOTOR_RIGHT_FORWARD_PIN, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_ENABLE_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_ENABLE_PIN, HIGH);
}

void goBackward() {
  digitalWrite(MOTOR_RIGHT_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD_PIN, HIGH);
  digitalWrite(MOTOR_RIGHT_ENABLE_PIN, HIGH);
  digitalWrite(MOTOR_LEFT_ENABLE_PIN, HIGH);
  delay(BACKWARD_TIMER);
}


void setup() {
  Serial.begin(115200);
  myservo.attach(SERVO_PIN);  // attaches the servo on the pin defined in SERVO_PORT to the servo object
  
  // setup motor pins
  pinMode(MOTOR_LEFT_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_ENABLE_PIN, OUTPUT); 
  pinMode(MOTOR_RIGHT_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD_PIN, OUTPUT);
  
  digitalWrite(MOTOR_LEFT_ENABLE_PIN, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_ENABLE_PIN, LOW); 
  digitalWrite(MOTOR_RIGHT_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
  
  delay(5000);
}

void loop() {
  pos = 90;
  dist = getDistanceAndSend2Serial(pos);
    
  if (dist > 15 && !backward) {
    goForward();
  }
  else {
    backward = false;
    motorStop();
    int distLeft = getDistanceAndSend2Serial(45);
    int distRight = getDistanceAndSend2Serial(135);
    
    if (min(distLeft, distRight) > MIN_DISTANCE) { // 
      if (distLeft < distRight) {
        turnLeft();
      } else {
        turnRight();
      }
    } else {
      goBackward();
      backward = true;
    }  
    
  }
}



