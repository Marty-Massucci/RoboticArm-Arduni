#include <VarSpeedServo.h>
#include <AccelStepper.h>
#include <math.h>

//Servo Setup
VarSpeedServo myservo1;
VarSpeedServo myservo2;
VarSpeedServo myservo3;
#define SPEED 40
#define PI 3.1415
#define LongDelay 500
#define ShortDelay 300

//Stepper Setup
int stepPin = 3;
int dirPin = 2;

//random pins
int Magnet = 11;
int LimitSwitch = 10;

#define motorInterfaceType 1
//1 means an external stepper driver with Step and Direction pins
AccelStepper BaseMotor(motorInterfaceType, stepPin, dirPin);
int StepperPos = 0;

//Variables
//Incomming Bytes
const byte buffSize = 40;
unsigned int inputBuffer[buffSize];

//Marker
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;

//Byte Data
boolean readInProgress = false;
boolean newDataFromPC = false;
byte coordinates[3];
int ShapeType = -1;

//Coordinates
const double PaperWidth = 5.5;
const double DistanceToRobotCenter = 16;

//Math
double X_Coordinate = 0;
double Y_Coordinate = 0;
double L0 = 7.8;
double L1 = 12;
double L2 = 7.7;
double L3 = 3;
double height = 0;
double distance = 0;
double hypotenuse = 0;
double BaseAngle = 0;
double Theta1 = 0;
double Theta2 = 0;
double Theta3 = 0;

//done
bool done = false;



