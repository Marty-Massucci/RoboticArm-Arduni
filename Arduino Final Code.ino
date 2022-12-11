#include "Functions.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Magnet, OUTPUT);
  pinMode(LimitSwitch, INPUT);

  digitalWrite(Magnet, LOW);
  digitalWrite(LED_BUILTIN, LOW);


  //Servo Setup
  myservo1.attach(5);
  myservo2.attach(6);
  myservo3.attach(10);

  //Stepper Setup
  BaseMotor.setMaxSpeed(100);
  BaseMotor.setAcceleration(10);
  BaseMotor.setSpeed(25);
  BaseMotor.moveTo(0);

  //Put arm at Neutral to start the program
  MoveArmToNeutral();

  delay(1000);

  TestMotors();

  for(int i = 0; i < 5; i++)
  {
    Blink();
  }
  
}

void loop() {
  //checks for data from PC then saves it
  getDataFromPC();

  if (newDataFromPC) 
  {
    UpdateCoodrdinates();
    sendCoordinatesToPC();
    DoMath();
    
    digitalWrite(LED_BUILTIN, HIGH);
    MoveArm(); 
    // for(int i = 0; i < 5; i++)
    // {
    //   Blink();
    // }
    digitalWrite(LED_BUILTIN, LOW);
    sendEnableCmd();
    newDataFromPC = false;
  }

  // if(done)
  // {
  //   delay(10000);
  //   BaseMotor.setSpeed(75);
  //   BaseMotor.write(200*5);
  //   BaseMotor.wait(); 
  //   while(done)
  //   {

  //   }
  // }

}


