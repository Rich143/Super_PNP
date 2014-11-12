int getAbsMax(int a, int b);
void getSpeeds(const int SPEED, int angleA, int angleB, int angleC, int & speedA, int & speedB, int & speedC);
bool reachedAngle(int encoder, int endAngle, int dir);
void move(int speed, int rotateLeft, int innerArmUp, int outerArmUp);
void calibrateMotor(tMotor motor_name);
void calibrate();
void pickUp();
void motorsOff();
void rotateLeft(int speed, int angle);
void rotateRight(int speed, int angle);
void armOneUp(int speed, int angle);
void armOneDown(int speed, int angle);
void armTwoUp(int speed, int angle);
void armTwoDown(int speed, int angle);


task main()
{

	//wait1Msec(500);
	//if (nNxtButtonPressed == -1)
	calibrate();
	//rotateLeft(30,90);

	//rotateRight(30,90);
	//armOneUp(20,90);

	//armOneDown(20,90);
	//armTwoUp(40,90);
	//armTwoDown(40,90);
	//move(10,0,0,-45);
	//move(10,0,0,45);
	//move(10,0,-45,0);
	//move(10,0,45,0);
	//move(10,-45,90,0);
	//move(10,45,-90,0);
	move(25,+45,-45,-45);
	move(25,-45,+45,+45);

	}

//returns the absolute maximum of two integers
int getAbsMax(int a, int b)
{
	if (abs(a) > abs(b))
		return abs(a);
	return abs(b);
}

//calculates speeds according to the change in angle of each component
//@angleA/B/C indicates the angle each component must rotate
//@speedA/B/C are the return values of the appropriate motor speeds for each component
//@SPEED is the speed of the fastest moving
void getSpeeds(const int SPEED, int angleA, int angleB, int angleC, int & speedA, int & speedB, int & speedC)
{
	int highestChange;
	highestChange = getAbsMax(angleA, angleB);
	highestChange = getAbsMax(highestChange, angleC);

	speedA = (int)((float)angleA / highestChange * SPEED);
	speedB = (int)((float)angleB / highestChange * SPEED);
	speedC = (int)((float)angleC / highestChange * SPEED);
}

//@Encoder: the current encoder value for the motor
//@endAngle: the desired end angle
//@dir: bool true is left/up, false is right/down
bool reachedAngle(int encoder, int endAngle, int dir)
{
	if (dir)
	{
		if (encoder > endAngle)
			return true;
	}
	else
	{
		if (encoder < endAngle)
			return true;
	}
	return false;
}

//@armOneUp: positive angles are up
//@armTwoUp: positive angles are up
//@angle: Angle to rotate motor A, cCW being positive
void move(int speed, int rotateLeft, int innerArmUp, int outerArmUp)
{
	motorsOff();

	int angleChangeA, angleChangeB, angleChangeC;
	//incorporates gear ratios
	angleChangeA = 7 * rotateLeft;
	angleChangeB = 6 * innerArmUp;
	angleChangeC = 3 * outerArmUp;

	int endAngleA, endAngleB, endAngleC;

	endAngleA = nMotorEncoder[motorA] + angleChangeA;
	endAngleB = nMotorEncoder[motorB] + angleChangeB;
	endAngleC = nMotorEncoder[motorC] + angleChangeC;

	int speedA, speedB, speedC;

	getSpeeds(speed, angleChangeA, angleChangeB, angleChangeC, speedA, speedB, speedC);

	motor[motorA] = speedA;
	motor[motorB] = speedB;
	motor[motorC] = speedC;

	bool leftA = false, upB = false, upC = false;

	if (rotateLeft > 0)
		leftA = true;
	if(innerArmUp > 0)
		upB = true;
	if (outerArmUp > 0)
		upC = true;


	//while at least one motor is still running
	while (motor[motorA] != 0 || motor[motorB] != 0 || motor[motorC] != 0 )
	{
		if (motor[motorA] != 0 && reachedAngle(nMotorEncoder[motorA], endAngleA, leftA))
			motor[motorA] = 0;
		if (motor[motorB] != 0 && reachedAngle(nMotorEncoder[motorB], endAngleB, upB))
			motor[motorB] = 0;
		if (motor[motorC] != 0 && reachedAngle(nMotorEncoder[motorC], endAngleC, upC))
			motor[motorC] = 0;
	}
}

void calibrateMotor(tMotor motor_name)
{
	bool doneCalibration = false;
	while(!doneCalibration)
	{
		if (nNxtButtonPressed == 	2)
			motor[motor_name] = 20;
		if (nNxtButtonPressed == 1)
			motor[motor_name] = -20;
		if (nNxtButtonPressed == -1)
			motor[motor_name] = 0;
		if (nNxtButtonPressed == 3)
		{
			motor[motor_name] = 0;
			doneCalibration = true;
		}
	}
	while(nNxtButtonPressed != -1);
}

// set the motors to the desired start position and resets encoders
void calibrate()
{
		calibrateMotor(motorA);
		calibrateMotor(motorB);
		calibrateMotor(motorC);
		nMotorEncoder[motorA] = 0;
		nMotorEncoder[motorB] = 0;
		nMotorEncoder[motorC] = 0;
}

void pickUp()
{
	armOneDown(20, 20);
	move(20, 20, -10, 0);
	move(20, 30, -10, 0);
}

void motorsOff()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;
}


void rotateLeft(int speed, int angle)
{
	motor[motorA] = speed;
	int endAngle = nMotorEncoder[motorA] + 7*angle;
	while (nMotorEncoder[motorA] < endAngle);
	motor[motorA] = 0;
}

void rotateRight(int speed, int angle)
{
	motor[motorA] = -speed;
	int endAngle = nMotorEncoder[motorA] - 7*angle;
	while (nMotorEncoder[motorA] > endAngle);
	motor[motorA] = 0;
}
void armOneUp(int speed, int angle)
{
	motor[motorB] = speed;
	int endAngle = nMotorEncoder[motorB] + 6*angle;
	while (nMotorEncoder[motorB] < endAngle);
	motor[motorB] = 0;
}
void armOneDown(int speed, int angle)
{
	motor[motorB] = -speed;
	int endAngle = nMotorEncoder[motorB] - 6*angle;
	while (nMotorEncoder[motorB] > endAngle);
	motor[motorB] = 0;
}
void armTwoUp(int speed, int angle)
{
	motor[motorC] = speed;
	int endAngle = nMotorEncoder[motorC] + 3 * angle;
	while (nMotorEncoder[motorC] < endAngle);
	motor[motorC] = 0;
}
void armTwoDown(int speed, int angle)
{
	motor[motorC] = -speed;
	int endAngle = nMotorEncoder[motorC] - 3 * angle;
	while (nMotorEncoder[motorC] > endAngle);
	motor[motorC] = 0;
}
