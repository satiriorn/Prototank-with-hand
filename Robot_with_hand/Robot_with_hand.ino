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

Servo servo1;
Servo servo2;
#define servo1Pin 26
#define servo2Pin 4
int pos = 0;     

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
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
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
        pos =(pos<180) ? pos+5:pos;
        servo1.write(pos);
      }
    else if(incomingChar=='r'){
        pos =(pos>0) ? pos-5:pos;
        servo1.write(pos);
      }
    Serial.write(incomingChar);  
  }
}
