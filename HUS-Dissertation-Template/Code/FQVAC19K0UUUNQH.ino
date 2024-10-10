#include <I2Cdev.h>
#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"

//initialise object
MPU6050 mpu;

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
VectorFloat gravity;    // [x, y, z]            gravity vector
Quaternion q;           // [w, x, y, z]         quaternion container
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// Motor Control
// -- -- -- -- -- -- -- -- -- -- -- -- -- --
#define enA 3
#define in1 4
#define in2 5

#define enB 9
#define in3 6
#define in4 7


int pos = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0; //tich phan

// For PID Controller (Change these)
// -- -- -- -- -- -- -- -- -- -- -- -- -- --
float kp = 10.5;             // (P)roportional Tuning Parameter
float ki = 35;            // (I)ntegral Tuning Parameter        
float kd = 0.5;             // (D)erivative Tuning Parameter
float target = 170;  // Can be adjusted according to centre of gravity 
float u = 0;
float lastpitch;          // Keeps track of error over time
float iTerm;              // Used to accumulate error (integral)

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


// ================================================================
// ===              Set up connection to MPU6050                ===
// ================================================================
void MPU6050Connect(){
  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXAccelOffset(-2692);
  mpu.setYAccelOffset(20);
  mpu.setZAccelOffset(712);
  mpu.setXGyroOffset(137);
  mpu.setYGyroOffset(156);
  mpu.setZGyroOffset(-24);

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    
    // enable Arduino interrupt detection
    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
    
    mpu.resetFIFO(); // Clear fifo buffer
  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
      if(devStatus == 1) Serial.println("> Initial Memory Load Failed");
      else if (devStatus == 2) Serial.println("> DMP Configuration Updates Failed");
  }
}


// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
void setup() {
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

  // really up to you depending on your project)
  Serial.begin(115200);
  
  // put your setup code here, to run once:
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  MPU6050Connect();
}


// ================================================================
// ===                       GET IMU DATA                       ===
// ================================================================

void IMUData(){
  mpuIntStatus = mpu.getIntStatus();
  
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      fifoCount = mpu.getFIFOCount();
      Serial.println(F("FIFO overflow!"));
  
  // otherwise, check for DMP data ready interrupt (this should happen @ 100Hz)
  }else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
      // wait for correct available data length, should be a VERY short wait
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

      // read a packet from FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      
      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;
      
      // display Euler angles in degrees
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      mpu.resetFIFO();
  }
}


// ================================================================
// ===                     PID CONTROLLER                       ===
// ================================================================

// int PID(){
//   // Calculate pitch
//   float pitch = ypr[1] * 180/M_PI;
  
//   // Calculate Error
//   float error = -targetAngle + pitch;

//   // Calculate PID terms
//   float pTerm = Kp * error;
//   iTerm += Ki * error * 10;
//   float dTerm = Kd * (pitch - lastpitch) / 10;
//   lastpitch = pitch;

//   // Obtain PID output value
//   float PIDValue = pTerm + iTerm - dTerm;

//   // Cap values so be able to send the correct PWM signal to the motors
//   if (PIDValue > 255) PIDValue = 255;
//   else if (PIDValue < -255) PIDValue = -255;
  
//   return int(PIDValue);
// }


// ================================================================
// ===                   MOTOR CONTROLLER                       ===
// ================================================================




// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================




void loop() {
  // if programming failed, don't try to do anything
  if (!dmpReady) return;
  
  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    if (mpuInterrupt && fifoCount < packetSize) {
          // try to get out of the infinite loop 
          fifoCount = mpu.getFIFOCount();
        }
    }
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  
  IMUData();
  float pitch = ypr[1] * 180/M_PI+180;

  //time difference
  long currT = micros();

  float deltaT = ((float)(currT-prevT))/1.0e6;
  prevT = currT;

  //error

  int e =  target - pitch;

  //derivative (đạo hàm)
  float dedt = (e - eprev)/(deltaT);

  //intergral (tích phân)
  // eintegral = eintegral + (e+eprev)*deltaT/2;
  eintegral = e*deltaT;

  float u = kp*e + ki*eintegral + kd*dedt;

  u = abs(u);
  if (u>255) u =255;
  eprev = e;

  //motor power
  if (e > 0){
    Backwards(u);
  }
  else if(e < 0){
    
    Forwards(u);
  }
  else if (pitch == 170){
    Stop(u);
  }



  


  // Call PID() to send appropriate motor control signal
  // MotorDriver(PID());
  Serial.print("sp: ");
  Serial.print(target);
  // float pitch = ypr[1] * 180/M_PI;
  // Serial.print(" yaw: ");
  // Serial.print(yaw);
  Serial.print(" u: ");
  Serial.print(u);  
  Serial.print(" pitch: ");
  Serial.println(pitch);
  // // Serial.print(" roll: ");
  // // Serial.println(roll);

}


void Forwards(int u){

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, u);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, u);
}

void Backwards(int u){

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, u);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, u);
}

void Stop(int u){

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enA, 0);

    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 0);
}






  
  // if(u >= 0) {
  //   digitalWrite(in1, LOW);
  //   digitalWrite(in2, HIGH);
  //   analogWrite(enA, u);
    
  //   digitalWrite(in3, LOW);
  //   digitalWrite(in4, HIGH);
  //   analogWrite(enB, u);
  //   }
  //   // Backwards
  // else{
  //   digitalWrite(in1, HIGH);
  //   digitalWrite(in2, LOW);
  //   analogWrite(enA, u * -1);
    
  //   digitalWrite(in3, HIGH);
  //   digitalWrite(in4, LOW);
  //   analogWrite(enB, u * -1);
  //   }




// void Forwards()
// {
//   //Right Motor
//   digitalWrite(MA1, LOW);
//   digitalWrite(MA2, HIGH);
//   analogWrite (EA, Speed);
//   //Left Motor
//   digitalWrite(MB3, LOW);
//   digitalWrite(MB4, HIGH);
//   analogWrite (EB, Speed);
// }

// void Backwards()
// {
//   //Right Motor
//   digitalWrite(MA1, HIGH);
//   digitalWrite(MA2, LOW);
//   analogWrite (EA, Speed);
//   //Left Motor
//   digitalWrite(MB3, HIGH);
//   digitalWrite(MB4, LOW);
//   analogWrite (EB, Speed);
// }

// void Static()
// {
//   //Right Motor
//   digitalWrite(MA1, LOW);
//   digitalWrite(MA2, LOW);
//   analogWrite (EA, 0);
//   //Left Motor
//   digitalWrite(MB3, LOW);
//   digitalWrite(MB4, LOW);
//   analogWrite (EB, 0);
// }