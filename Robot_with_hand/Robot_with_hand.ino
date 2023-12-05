#include <Robojax_L298N_DC_motor.h>
#include <BluetoothSerial.h>
#include <Servo.h>

// motor 1 settings
#define IN1 19
#define IN2 18
#define ENA 16 // this pin must be PWM enabled pin

// motor 2 settings
#define IN3 12
#define IN4 13
#define ENB 5 // this pin must be PWM enabled pin

const int CCW = 2; 
const int CW  = 1; 

#define motor1 1 
#define motor2 2 

Robojax_L298N_DC_motor motors(IN1, IN2, ENA, IN2, IN3, ENB, true);
BluetoothSerial SerialBT;

Servo base_servo;
Servo shoulder_servo;
Servo elbow_servo;
Servo grip_servo;

#define servo1Pin 26
#define servo2Pin 25
#define servo3Pin 2
#define servo4Pin 4
int base_position = 90;
int shoulder_position = 90;
int elbow_position = 90;
int grip_position = 90;    

struct MotorsState {
      int speed_motor1 = 0;
      int speed_motor2 = 0;
      char direction_motor1 = CCW;
      char direction_motor2 = CW;
} motors_state;

void Forward(){
  Stop();
  motors_state.direction_motor2 = CW;
  motors_state.direction_motor1 = CCW;
  motors.rotate(motor2, motors_state.speed_motor2, motors_state.direction_motor2);
  motors.rotate(motor1, motors_state.speed_motor1, motors_state.direction_motor1);
}

void Back(){
  Stop();
  motors_state.direction_motor2 = CCW;
  motors_state.direction_motor1 = CW;
  motors.rotate(motor2, motors_state.speed_motor2, motors_state.direction_motor2);
  motors.rotate(motor1, motors_state.speed_motor1, motors_state.direction_motor1);
}

void Left(){
  Stop();
  motors_state.direction_motor1 = CCW;
  motors.rotate(motor1, motors_state.speed_motor1, motors_state.direction_motor1);
}

void Right(){
  Stop();
  motors_state.direction_motor2 = CW;
  motors.rotate(motor2, motors_state.speed_motor2, motors_state.direction_motor2);
}

void Stop(){
  motors.brake(1);  
  motors.brake(2);
}
String message = "";

void setup() {
  Serial.begin(115200);
  motors.begin();
  SerialBT.begin("Robot with hand");
  base_servo.attach(servo1Pin);
  elbow_servo.attach(servo2Pin);
  shoulder_servo.attach(servo3Pin);
  grip_servo.attach(servo4Pin);
}

void loop() {
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
    }
    else{
      message = "";
    }
    if(incomingChar=='F'){
        Forward();
      }
    else if(incomingChar=='B'){
        Back();
      }
    else if(incomingChar=='L'){
        Left();
      }
    else if(incomingChar=='R'){
        Right();
     }
    else if(incomingChar=='S'){
        Stop();
      }
    else if(isDigit(incomingChar)){
        motors_state.speed_motor2 = (incomingChar-'0')*10;
        motors_state.speed_motor1 = (incomingChar-'0')*10;
        Serial.println(motors_state.speed_motor1);
      }
    else if(incomingChar=='l'){
        base_position =(base_position<250) ? base_position+5:base_position;
        base_servo.write(base_position);
      }
    else if(incomingChar=='r'){
        base_position =(base_position>0) ? base_position-5:base_position;
        base_servo.write(base_position);
      }
    else if(incomingChar=='p'){
        shoulder_position =(shoulder_position<180) ? shoulder_position+5:shoulder_position;
        shoulder_servo.write(shoulder_position);
      }
    else if(incomingChar=='b'){
        shoulder_position =(shoulder_position>0) ? shoulder_position-5:shoulder_position;
        shoulder_servo.write(shoulder_position);
      }
    else if(incomingChar=='u'){
        elbow_position =(elbow_position<180) ? elbow_position+5:elbow_position;
        elbow_servo.write(elbow_position);
      }
    else if(incomingChar=='d'){
        elbow_position =(elbow_position>0) ? elbow_position-5:elbow_position;
        elbow_servo.write(elbow_position);
      }
    else if(incomingChar=='n'){
        grip_position = 180;
        grip_servo.write(grip_position);
      }
    else if(incomingChar=='f'){
        grip_position =0;
        grip_servo.write(grip_position);
      }
    Serial.write(incomingChar);  
  }
}
