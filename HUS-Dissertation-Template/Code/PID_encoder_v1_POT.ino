/*
Code này sử dụng chiết áp để thay đổi Setpoint(Target) của hệ.
Từ đó có thể quan sát chi tiết hơn về sự đáp ứng của hệ thống đối với yêu cầu đặt ra(setpoint)


Test: target = 250*sin(prevT/1.e6); Kp = 4.18, Ki= 0.0(khong), Kd = 0.56
Cho ket qua tot
*/



#define ENC_A 2
#define ENC_B 3
#define PWM 5
#define IN3 7
#define IN4 6

int pos = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0; //tich phan
float sp;


//---PID---

float kp = 4.18;
float ki = 0;
float kd = 0.56;

//---------

void setup(){
  Serial.begin(9600);
  Serial.println("var1:,var2:,var3:");
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  pinMode(4, INPUT_PULLUP);
  attachInterrupt(0, readEncoder, RISING);
}

void loop(){


 
  //set target position (setpoint)
  int target = map(analogRead(0), 0, 1023,0, 360); //500*sin(prevT/1.e6); //
  //float sp = 11*99.3*target;

  sp = (11*99.3*target)/360;
  
  //time difference
  long currT = micros();

  float deltaT = ((float)(currT-prevT))/1.0e6;
  prevT = currT;

  //error
  int e = sp - pos;

  //derivative (đạo hàm)
  float dedt = (e - eprev)/(deltaT);

  //intergral (tích phân)
  eintegral = eintegral + (e+eprev)*deltaT/2;
  //eintegral = e*deltaT;

  float u = kp*e + ki*eintegral + kd*dedt;

  //motor power
  float pwr = fabs(u);
  if(pwr > 255){
    pwr = 255;
  }

  // motor direction
  int dir = 1;
  if (u<0){
    dir = -1;
  }

  //signal the motor
  setMotor(dir, pwr, PWM, IN3, IN4);

  //store previous error
  eprev = e;
  Serial.println("Min:0,Max:300");
  Serial.print("target:");
  Serial.print(target);
  Serial.print(",");

  Serial.print("pos:");
  Serial.print(pos);
  Serial.print(",");

  Serial.print("sp:");
  Serial.print(sp);
  Serial.print(",");





}

void setMotor(int dir, int pwmVal, int pwm, int in3, int in4){ // huong, gia tri, pwm, in3, in4
  analogWrite(pwm, pwmVal);
  if(dir == 1){
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else if(dir == -1){
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  else{
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
}



void readEncoder(){
  int b = digitalRead(ENC_B);
  if(b>0){
    pos++;
  }
  else{
    pos--;
  }
}




