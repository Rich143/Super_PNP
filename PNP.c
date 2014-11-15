//Insert title comments
int getAbsMax(int a, int b);
void getSpeeds(const int SPEED, int angleA, int angleB, int angleC, int & speedA, int & speedB, int & speedC);
bool reachedAngle(int encoder, int endAngle, int dir);
void move(int speed, int rotateCCW, int innerArmUp, int outerArmUp);
void calibrateMotor(tMotor motor_name);
void calibrate();
void pickUp();
void motorsOff();
void checkIfDone();
int roundSpeed(float val);


typedef struct{
	int angleA;
	int angleB;
	int angleC;
	char color; //one letter per color
} Position;

typedef struct{
	Position positions[6];
	int size;
} Platform;

task main()
{
	SensorType[S1] = sensorTouch;
	calibrate();

	move(20,90,-20,-20);
	wait1Msec(1000);

	move(20,-90,20,20);
	wait1Msec(1000);
}

// Returns the absolute maximum of two integers
int getAbsMax(int a, int b)
{
	if (abs(a) > abs(b))
		return abs(a);
	return abs(b);
}

// Calculates speeds according to the change in angle of each component
// @angleA/B/C indicates the angle each component must rotate
// @speedA/B/C are the return values of the appropriate motor speeds for each component
// @SPEED is the speed of the fastest moving
void getSpeeds(const int SPEED, int angleA, int angleB, int angleC, int & speedA, int & speedB, int & speedC)
{
	int highestChange;
	highestChange = getAbsMax(angleA, angleB);
	highestChange = getAbsMax(highestChange, angleC);

	speedA = roundSpeed(angleA / highestChange) * SPEED; // / highestChange * SPEED);
	speedB = roundSpeed(angleB / highestChange) * SPEED; // / highestChange * SPEED);
	speedC = roundSpeed(angleC / highestChange) * SPEED; // / highestChange * SPEED);
}

// @Encoder: the current encoder value for the motor
// @endAngle: the desired end angle
// @dir: bool true is left/up, false is right/down
bool reachedAngle(int encoder, int endAngle, int dir)
{
	if (dir && encoder > endAngle)
	{
		return true;
	}
	else if (!dir && encoder <= endAngle)
	{
		return true;
	}
	return false;
}

// @outerArmUp: positive angles are up
// @innerArmUp: positive angles are up
// @angle: Angle to rotate motor A, cCW being positive
void move(int speed, int rotateCCW, int innerArmUp, int outerArmUp)
{
	motorsOff();

	int angleChangeA, angleChangeB, angleChangeC;
	//incorporates gear ratios
	angleChangeA = 7 * rotateCCW;
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

	if (rotateCCW > 0)
		leftA = true;
	if(innerArmUp > 0)
		upB = true;
	if (outerArmUp > 0)
		upC = true;


	//while at least one motor is still running
	while (motor[motorA] != 0 || motor[motorB] != 0 || motor[motorC] != 0)
	{
		checkIfDone();
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
		checkIfDone();
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



// If emergency stop pressed or pattern finished, display a message on the screen
// and
void checkIfDone()
{
	if (SensorValue[S1])
	{
		motorsOff();
		nxtDisplayString(0, "Program Complete!");
		while (1);
	}
}

void motorsOff()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;
}

void pickUp()
{
	//armOneDown(20, 20);
	move(20, 20, -10, 0);
	move(20, -30, -10, 0);
}

// rounds to the fastest speed
int roundSpeed(float val)
{
	if (val>0)
		return (int)ceil(val);
	else
		return (int)floor(val);
}
