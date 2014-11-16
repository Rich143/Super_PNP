//Insert title comments//Insert title comments

typedef struct
{
	int angleA;
	int angleB;
	int angleC;
	int color; //one letter per color
} Position;

typedef struct
{
	Position positions[4][4];
} Grid;

void assignPosition(int angleA, int angleB, int angleC, Position & pos);
void assignCurrentPosition(Position & pos);
int getAbsMax(int a, int b);
void getSpeeds(const int SPEED, int angleA, int angleB, int angleC, int & speedA, int & speedB, int & speedC);
bool reachedAngle(int encoder, int endAngle, int dir);
void move(int speed, int rotateCCW, int innerArmUp, int outerArmUp);
void calibrateMotor(tMotor motor_name);
void calibrate();
void pickUp();
void motorsOff();
void checkIfDone();
void displayAngles(Position pos);
void moveToLocation(int speed, Position startPosition, Position endPosition);
int roundSpeed(float val);
void setPositions(Grid & grid);

task main()
{
	SensorType[S1] = sensorTouch;
	Grid pattern;
	setPositions(pattern);

	calibrate();
	Position startPosition;
	startPosition.angleA = 0;
	startPosition.angleB = 90;
	startPosition.angleC = 0;
	startPosition.color = 0;
	displayAngles(startPosition);
	wait10Msec(200);

	moveToLocation(20, startPosition, pattern.positions[0][0]);
	wait10Msec(100);
	pickUp();

	moveToLocation(20, pattern.positions[0][0], startPosition);

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

	speedA = roundSpeed((float)angleA / highestChange * SPEED);
	speedB = roundSpeed((float)angleB / highestChange * SPEED);
	speedC = roundSpeed((float)angleC / highestChange * SPEED);
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
	angleChangeB = 5 * innerArmUp;
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

void moveToLocation(int speed, Position startPosition, Position endPosition)
{
	motorsOff();
	move(speed, endPosition.angleA - startPosition.angleA, endPosition.angleB - startPosition.angleB, endPosition.angleC - startPosition.angleC);
	displayAngles(endPosition);
}

void calibrateMotor(tMotor motor_name)
{
	bool doneCalibration = false;
	while(!doneCalibration)
	{
		checkIfDone();
		if (nNxtButtonPressed == 	2)
			motor[motor_name] = 10;
		if (nNxtButtonPressed == 1)
			motor[motor_name] = -10;
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
// and stop the program
void checkIfDone()
{
	if (SensorValue[S1])
	{
		motorsOff();
		nxtDisplayString(0, "Program Complete!");
		while (1);
	}
}

void displayAngles(Position pos)
{
	eraseDisplay();
	nxtDisplayString(2, "Angle A: %d", pos.angleA);
	nxtDisplayString(3, "Angle B: %d", pos.angleB);
	nxtDisplayString(4, "Angle C: %d", pos.angleC);
}

void motorsOff()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;
}

void pickUp()
{
	move(20, 0, -5, -5);
	move(20, 0, -15, 20);
	wait10Msec(100);
	move(20, 0, 20, 10);
	move(20, 0, 45, -5);
}

// rounds to the fastest speed
int roundSpeed(float val)
{
	if (val>0)
		return (int)ceil(val);

	else
		return (int)floor(val);
}

// Predetermined positions of each gridspace
void setPositions(Grid & grid)
{
	// FIRST ROW
	for (int i = 0; i < 4; i++)
	{
		grid.positions[i][0].angleA = 40 + i*25;
		grid.positions[i][0].angleB = 75;
		grid.positions[i][0].angleC = -135;
	}
	// SECOND ROW
	for (int i = 0; i < 4; i++)
	{
		grid.positions[i][1].angleA = 40 + i*25;
		grid.positions[i][1].angleB = 60;
		grid.positions[i][1].angleC = -120;
	}
	// THIRD ROW
	for (int i = 0; i < 4; i++)
	{
		grid.positions[i][2].angleA = 40 + i*25;
		grid.positions[i][2].angleB = 50;
		grid.positions[i][2].angleC = -95;
	}
	// FOURTH ROW
	for (int i = 0; i < 4; i++)
	{
		grid.positions[i][3].angleA = 40 + i*25;
		grid.positions[i][3].angleB = 40;
		grid.positions[i][3].angleC = -70;
	}

}


// Test code 1: Multiple position moves
/*

Position p1;
	p1.angleA = 90;
	p1.angleB = 45;
	p1.angleC = -45;
	p1.color = 1;
	Position p2;
	p2.angleA = 0;
	p2.angleB = 20;
	p2.angleC = 0;
	p2.color = 1;
	Position p3;
	p3.angleA = -45;
	p3.angleB = 90;
	p3.angleC = -70;
	p3.color = 1;
	moveToLocation(20, startPosition, p1);
	wait10msec(200);
	moveToLocation(20, p1, startPosition);
	wait10msec(200);
	moveToLocation(20, startPosition, p2);
	wait10msec(200);
	moveToLocation(20, p2, p3);
	wait10msec(200);
	moveToLocation(20, p3, startPosition);
*/


/*
int inLength = 21;
int outLength = 24;
float toRADIANS = PI / 180;
float increaseLength(int length, float angle){
	float currLength = inLength * inLength + outLength * outLength
						 				 - 2 * inLength * outLength * cos ( (180 - abs(angle) * toRADIANS ));

	float oldAngleBetweenArms = acos((inLength*inLength + outLength*outLength - currLength*currLength) /
									 				 (2*inLength*outLength));
	float oldInnerAngle = asin(outLength * sin(oldAngleBetweenArms * toRADIANS) / currLength);
	currLength += length;

	float angleBetweenArms = acos((inLength*inLength + outLength*outLength - currLength*currLength) /
									 				 (2*inLength*outLength));

	float innerAngle = asin(outLength * sin(angleBetweenArms * toRADIANS) / currLength);

	float outerAngle = -180 + angleBetweenArms;

	Position startPos, endPos;
	assignCurrentPosition(startPos);
	assignPosition(startPos.angleA, startPos.angleB - oldInnerAngle + innerAngle, outAngle, endPos);
	moveToLocation(startPos, endPos);
}

void assignCurrentPosition(Position & pos)
{
	pos.angleA = nMotorEncoder[motorA] / 7;
	pos.angleB = nMotorEncoder[motorB] / 5;
	pos.angleC = nMotorEncoder[motorC] / 3;
}

void assignPosition(int angleA, int angleB, int angleC, Position & pos)
{
	pos.angleA = angleA;
	pos.angleB = angleB;
	pos.angleC = angleC;
}

*/
