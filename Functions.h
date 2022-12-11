#include"Initilize.h"

//Blinds the Built in LED pin 13
void Blink()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); 
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

//Stop Command to computer
void sendSuspendCmd() {
  // send the suspend-true command
  Serial.println("<S1>");
  // lcd.setCursor(0, 1);
  // lcd.print("<S1>");
}

//Enable command to computer
void sendEnableCmd() {
  // send the suspend-false command
  Serial.println("<S0>");
  // lcd.setCursor(0, 1);
  // lcd.print("<S0>");
}

//Send the coordinates recived back to the PC
void sendCoordinatesToPC() {
  // send the point data to the PC
  Serial.print("<P");
  Serial.print("X ");
  Serial.print(X_Coordinate);
  Serial.print(",");
  Serial.print("Y ");
  Serial.print(Y_Coordinate);
  Serial.print(",");
  Serial.print("BaseAngle ");
  Serial.print(BaseAngle);
  Serial.print(",");
  Serial.print("Theta1 ");
  Serial.print(Theta1);
  Serial.print(",");
  Serial.print("Theta2 ");
  Serial.print(Theta2);
  Serial.print(",");
  Serial.print("Theta3 ");
  Serial.print(Theta3);
  Serial.println(">");

  // lcd.setCursor(0, 1);
  // lcd.print("<P");
  // lcd.print(coordinates[0]);
  // lcd.print(",");
  // lcd.print(coordinates[1]);
  // lcd.println(">");
}

// alternative to the readBytes function:
void getDataFromPC() {
  // receive data from PC and save it into inputBuffer
  while (Serial.available() > 0) {
    char x = Serial.read();
    // the order of these IF clauses is significant
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      coordinates[0] = inputBuffer[0];
      coordinates[1] = inputBuffer[1];
      coordinates[2] = inputBuffer[2];
      
    }

    if (readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) {
      bytesRecvd = 0;
      readInProgress = true;
    }

    if(x == "X")
    {
      done = true;
    }
  }
}

//Change coordinates vs base of robot
void UpdateCoodrdinates()
{
  X_Coordinate = coordinates[0]/4 - PaperWidth;
  Y_Coordinate = -1*coordinates[1]/4 + DistanceToRobotCenter;
  ShapeType = coordinates[2];
  
  // X_Coordinate = 0;
  // Y_Coordinate = 12;
  // ShapeType = 0;
}

//Moves the Arm to the coordinates of shape recieved
void MoveArmToShape() 
{
  // myservo2.write(90, SPEED);
  // myservo3.write(135, SPEED);
  // BaseMotor.runToNewPosition(BaseAngle);
  
  // myservo2.wait();
  // myservo3.wait();
  // BaseMotor.run();

  // delay(5000);
  // myservo1.write(45, SPEED);
  // myservo1.wait();

  BaseMotor.runToNewPosition((int)BaseAngle);
  BaseMotor.run();

  delay(LongDelay);

  myservo2.write(Theta2, SPEED);
  myservo3.write(Theta3, SPEED);

  myservo2.wait();
  myservo3.wait();

  delay(LongDelay);

  //get ready to pick up shape
  digitalWrite(Magnet, HIGH);
  
  delay(2000);
 
  myservo1.write(Theta1, SPEED/4);
  myservo1.wait();

  delay(LongDelay);

  //Blink();
}

//Moves arm back to neutral 
void MoveArmToNeutral()
{
  myservo1.write(0, SPEED);     
  myservo2.write(0, SPEED);
  myservo3.write(60, SPEED);
  BaseMotor.runToNewPosition(0);  
  myservo1.wait();
  myservo2.wait();
  myservo3.wait();
  BaseMotor.run();

  //Blink();
}

// void Whack___F()
// {
//   myservo1.write(0, SPEED);
//   myservo1.wait();
//   delay(1000);
//   myservo1.write(70, 75);
//   myservo1.wait();
// }

//Moved given shape to correct Pile
void MoveShapeToPile()
{
  if(ShapeType == 0)
  {
    BaseAngle = 50;
  }
  else if (ShapeType == 1)
  {
    BaseAngle = -50;
  }

  myservo1.write(0, SPEED);
  myservo1.wait();

  delay(LongDelay);


  myservo2.write(60, SPEED);
  myservo3.write(71, SPEED);
  BaseMotor.runToNewPosition(BaseAngle);
  myservo2.wait();
  myservo3.wait();
  BaseMotor.run();

  delay(LongDelay);

  myservo1.write(78, SPEED/4);
  myservo1.wait();
  
  delay(LongDelay);

  //drop shape
  digitalWrite(Magnet, LOW);
  delay(5000);

  //Whack___F();

  //Blink();
}



//Moves Arm
void MoveArm()
{
  MoveArmToShape();

  delay(LongDelay);

  MoveShapeToPile();
  
  delay(LongDelay);

  MoveArmToNeutral();

  //delay(LongDelay);

  //CalibrateCenter();
}

void ChangeAnglesForMotors()
{
  //change angles to servo angles 
  BaseAngle = -1*BaseAngle/1.8;
  Theta1 = 180 - Theta1;
  Theta2 = Theta2;
  Theta3 = (Theta3 - 90)*2/3;

  // if(BaseAngle > 0)
  // {
  //   BaseAngle = (int)BaseAngle + 2;
  // }
  // else if (BaseAngle < 0)
  // {
  //   BaseAngle = (int)BaseAngle - 2;
  // }
  
  //StepperPos += BaseAngle

}

void DoMath()
{
  BaseAngle = atan(X_Coordinate/Y_Coordinate) * 180 / PI;
  distance = hypot(X_Coordinate, Y_Coordinate);
  height = L0 - L3; 
  hypotenuse = hypot(distance, height);
  int beta = atan(distance / height) * 180 / PI;
  int gama = acos((-1*(L2*L2-L1*L1-hypotenuse*hypotenuse))/(2*L1*hypotenuse))* 180 / PI;
  Theta1 = beta + gama;
  Theta2 = acos((-1*(hypotenuse*hypotenuse - L1*L1 - L2*L2))/(2*L1*L2))* 180 / PI;
  int delta = 180 - gama - Theta2;
  int alfa = 90 - beta;
  Theta3 = alfa + delta + 90; 

  ChangeAnglesForMotors();
}


void TestMotors()
{
  myservo1.write(0, SPEED); 
  myservo1.wait();
  myservo1.write(90, SPEED); 
  myservo1.wait();
  delay(ShortDelay);
  myservo1.write(0, SPEED); 
  myservo1.wait();

  delay(ShortDelay);

  myservo2.write(0, SPEED); 
  myservo2.wait();
  myservo2.write(90, SPEED); 
  myservo2.wait();
  delay(ShortDelay);
  myservo2.write(0, SPEED); 
  myservo2.wait();

  delay(ShortDelay);
  
  myservo3.write(60, SPEED); 
  myservo3.wait();
  myservo3.write(90, SPEED); 
  myservo3.wait();
  delay(ShortDelay);
  myservo3.write(60, SPEED); 
  myservo3.wait();

  delay(ShortDelay);

  BaseMotor.runToNewPosition(0);  
  BaseMotor.run();
  BaseMotor.runToNewPosition(100);  
  BaseMotor.run();
  delay(ShortDelay);
  BaseMotor.runToNewPosition(0);  
  BaseMotor.run();

  //CalibrateCenter();
}


// void CalibrateCenter()
// {
//   int pos = -20;
//   StepperPos += pos;
//   BaseMotor.runToNewPosition(pos);  
//   BaseMotor.run();

//   delay(LongDelay);

//   myservo1.write(95, SPEED); 
//   myservo2.write(5, SPEED); 
//   myservo1.wait();
//   myservo2.wait();

//   delay(LongDelay);

//   int limit = digitalRead(LimitSwitch);
//   while(!limit)
//   {
//     StepperPos++;
//     pos++;
//     BaseMotor.runToNewPosition(pos);
//     limit = digitalRead(LimitSwitch);     
//   }
    
// }


