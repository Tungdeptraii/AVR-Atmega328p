#include <Servo.h>
#include <SimpleKalmanFilter.h>

SimpleKalmanFilter kf_1(2, 2, 0.01);
SimpleKalmanFilter kf_2(2, 2, 0.01);
SimpleKalmanFilter kf_3(2, 2, 0.01);


long prevT = 0;
float eprev = 0;
double I = 0; //tich phan
double pos, setpoint;
double error, P, ServoOutput;
boolean Saturation = false;

const int trig1 = 6;
const int echo1 = 5; 
const int trig2 = 8;
const int echo2 = 7;

// float kp = 6.075;
// float ki = 5.112;
// float kd = 1.6107;

float kp = 4.8;
float ki = 5.3156;
float kd = 1.08;

Servo myServo;

float readPosition_1(void);
float readPosition_2(void);
void move_servo(int);

void setup() {
  Serial.begin(9600);                                             
  
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  myServo.attach(9);                                           
  myServo.write(100);
   

}

void loop() {

  pos = readPosition_1();
  setpoint = readPosition_2();


  long currT = micros();
  float deltaT = (float(currT - prevT))/1.0e6;
  prevT = currT;

  error =  setpoint - pos;

  P = error;
  if (! Saturation) I = I + (error+eprev)*deltaT/2;
  //I = e*deltaT;

  //derivative (đạo hàm)
  float D = (error - eprev)/(deltaT);


 
  float u = kp*P + ki*I + kd*D;
  u = 100 - u;
  //u = kf_1.updateEstimate(u);


  move_servo(u);

  Serial.print(pos);
  Serial.print(" ");
  Serial.println(setpoint);


  eprev = error; 
  
}

float readPosition_1(void){

long durata=0;
float distance=0; 

digitalWrite(trig1, LOW); 
delayMicroseconds(10); 

digitalWrite(trig1, HIGH);
delayMicroseconds(10);
 
digitalWrite(trig1, LOW);

durata = pulseIn(echo1, HIGH);
distance = (float)durata/58;

delay(30);

if (distance > 28) distance=28;
else if (distance < 2) distance=0;

return round(distance);  

}

float readPosition_2(void){
long durata=0;
float distance=0; 

digitalWrite(trig2, LOW); 
delayMicroseconds(10); 

digitalWrite(trig2, HIGH);
delayMicroseconds(10);
 
digitalWrite(trig2, LOW);

durata = pulseIn(echo2, HIGH);
distance = (float)durata/58;

delay(30);

if (distance > 30) distance=15;
else if (distance < 0) distance=0;

return round(distance-1);
}
void move_servo(int u){
  if (u>155){
    u = 155;
    Saturation = true;
  }
  else if(u<60){
    u = 60;
    Saturation = true;
  }
  else Saturation = false;
  myServo.write(u);
}