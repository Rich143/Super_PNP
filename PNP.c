//Insert title comments//Insert title comments
const int SCANSPEED = 20;
typedef struct
{
	int angleA;
	int angleB;
	int angleC;
	int color; //one letter per color
} Position;

typedef struct
{
	Position positions[3][3];
} Grid;

void putDown(Position & currentPosition, int row, int mSpeed);
void placeBlock(Position & startPosition, Position & endPosition, int mSpeed, int row, int placeRow, Position & currentPosition);
void assignPosition(int angleA, int angleB, int angleC, Position & pos);
void assignCurrentPosition(Position & pos);
int getAbsMax(int a, int b);
void getSpeeds(const int SPEED, int angleA, int angleB, int angleC, int & speedA, int & speedB, int & speedC);
bool reachedAngle(int encoder, int endAngle, int dir);
void move(int speed, int rotateCCW, int innerArmUp, int outerArmUp);
void calibrateMotor(tMotor motor_name);
void calibrate();
void pickUp(Position & currentPosition, int row, int mSpeed);
void motorsOff();
void checkIfDone();
void displayAngles(Position pos);
void moveToLocation(int speed, Position currentPosition, Position endPosition);
int roundSpeed(float val);
void setPositions(Grid & pattern, Grid & colorPattern, Grid & scanGrid);
void scanColors(Grid & pattern, Grid & colorPattern, Grid & scanGrid);
int getColor();
void returnToStart();

task main()
{
	SensorType[S4] = sensorTouch;
	SensorType[S1] = sensorCOLORFULL;

	/*while (true)
	{
		int color = getColor();
		nxtDisplayString(0, "Color: %d", color);
	}
	*/



	Grid pattern, colorPattern, scanGrid;
	setPositions(pattern, colorPattern, scanGrid);

	calibrate();
  Position startPosition;
	startPosition.angleA = 0;
	startPosition.angleB = 90;
	startPosition.angleC = 0;
	startPosition.color = 0;
	displayAngles(startPosition);

	Position currentPosition;

	currentPosition.angleA = 0;
	currentPosition.angleB = 90;
	currentPosition.angleC = 0;
	currentPosition.color = 0;
	
	scanColors(pattern, colorPattern, scanGrid);

	//placeBlock(colorPattern.positions[0][0], pattern.positions[0][0], 20, 0, currentPosition);


	calibrate();

	currentPosition.angleA = 0;
	currentPosition.angleB = 90;
	currentPosition.angleC = 0;
	currentPosition.color = 0;
	
	while (nNxtButtonPressed == -1);
	while (nNxtButtonPressed != -1);

	int colorNeeded, pickupColumn, pickupRow;
	for (int column = 2; column >= 0; column--)
	{
		for (int row = 2; row >= 0; row--)
		{
			colorNeeded = pattern.positions[column][row].color;
			// 1 - black, 2 - blue, 3 - green, 4 - yellow, 5 - red, 6 - white
			pickupColumn = colorNeeded - 3;
			pickupRow = 0;

			while (colorPattern.positions[pickupColumn][pickupRow].color == 0)
			{
				nxtDisplayString(0,"row: %d,col: %d",row, column);
				nxtDisplayString(1,"%d", colorPattern.positions[pickupColumn][pickupRow].color);
				wait10Msec(200);
				pickupRow++;
			}
			colorPattern.positions[pickupColumn][pickupRow].color = 0; // color = 0 means the block has been used

			placeBlock(colorPattern.positions[pickupColumn][pickupRow], pattern.positions[column][row], 20, pickupRow, row, currentPosition);
			returnToStart();
			/*
			moveToLocation(20, currentPosition, colorPattern.positions[pickupColumn][pickupRow]);
			moveToLocation(20, currentPosition, pattern.positions[column][row]);
			*/
		}
	}

	returnToStart();

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

void moveToLocation(int speed, Position currentPosition, Position endPosition)
{
	motorsOff();
	move(speed, endPosition.angleA - currentPosition.angleA, endPosition.angleB - currentPosition.angleB, endPosition.angleC - currentPosition.angleC);
	displayAngles(endPosition);
	assignCurrentPosition(currentPosition);
}

void placeBlock(Position startPosition, Position endPosition, int mSpeed, int row, int placeRow, Position & currentPosition)
{
	Position upPosition;
	upPosition.angleA = 0;
	upPosition.angleB = 90;
	upPosition.angleC = -90;
	upPosition.color = 0;

	moveToLocation(mSpeed, currentPosition, startPosition);
	wait10Msec(10);
	pickUp(currentPosition, row, mSpeed);
	wait10Msec(10);
	moveToLocation(mSpeed, currentPosition, upPosition);
	wait10Msec(10);
	moveToLocation(mSpeed, currentPosition, endPosition);
	wait10Msec(10);
	putDown(currentPosition, placeRow, mSpeed);
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
	if (SensorValue[S4])
	{
		motorsOff();
		nxtDisplayString(0, "MOTORS OFF");
		wait1Msec(1000);
		returnToStart();
		nxtDisplayString(0, "Program Complete!");
		while ( true );
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

// @position: pass in the current position
// @row: the row we're picking up from
// @mSpeed: the speed to move at
void pickUp(Position & currentPosition, int row, int mSpeed)
{
	if (row == 0)
	{
		move(mSpeed, 0, -5, -5);
		move(mSpeed, 0, -25, 15);
		wait10Msec(10);
		move(mSpeed, 0, 20, 0);
		move(mSpeed, 0, 20, 10);
	}

	else if (row == 1)
	{
		move(mSpeed,0, -20, 0);
		wait10Msec(10);
		move(mSpeed, 0, -5, 15);
		wait10Msec(10);
		move(mSpeed, 0, 20, 0);
		move(mSpeed, 0, 20, 10);
	}

	else if (row == 2)
	{
		move(mSpeed, 0, -10, 0);
		move(mSpeed, 0, -15, 15);
		wait10Msec(10);
		move(mSpeed, 0, 20, -5);
		move(mSpeed, 0, 20, 10);
	}

	assignCurrentPosition(currentPosition);
}

void putDown(Position & currentPosition, int row, int mSpeed)
{
	if (row == 0)
	{
		move(mSpeed, 0, -25, -5);
		move(mSpeed, 0, 5, -20);
		move(mSpeed, 0, 40, 10);
	}

	else if (row == 1)
	{
		move(mSpeed, 0, -20, -5);
		move(mSpeed, 0, 5, -20);
		move(mSpeed, 0, 40, -10);
	}

	else if (row == 2)
	{
		move(mSpeed, 0, -15, -10);
		move(mSpeed, 0, 5, -15);
		move(mSpeed, 0, 40, -10);
	}

	assignCurrentPosition(currentPosition);
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
void setPositions(Grid & pattern, Grid & colorPattern, Grid & scanGrid)
{
	// FIRST ROW
	for (int i = 0; i < 3; i++)
	{
		scanGrid.positions[i][0].angleA = 66 + i*25;
		scanGrid.positions[i][0].angleB = 77;
		scanGrid.positions[i][0].angleC = -110;

		pattern.positions[i][0].angleA = 66 + i*25;
		pattern.positions[i][0].angleB = 75;
		pattern.positions[i][0].angleC = -115;

		colorPattern.positions[i][0].angleA = -66 - i*25;
		colorPattern.positions[i][0].angleB = 75;
		colorPattern.positions[i][0].angleC = -135;
	}
	// SECOND ROW
	for (int i = 0; i < 3; i++)
	{
		scanGrid.positions[i][1].angleA = 66 + i*25;
		scanGrid.positions[i][1].angleB = 66;
		scanGrid.positions[i][1].angleC = -97;

		pattern.positions[i][1].angleA = 66 + i*25;
		pattern.positions[i][1].angleB = 65;
		pattern.positions[i][1].angleC = -85;

		colorPattern.positions[i][1].angleA = -66 - i*25;
		colorPattern.positions[i][1].angleB = 65;
		colorPattern.positions[i][1].angleC = -110;
	}
	// THIRD ROW
	for (int i = 0; i < 3; i++)
	{
		scanGrid.positions[i][2].angleA = 66 + i*25;
		scanGrid.positions[i][2].angleB = 53;
		scanGrid.positions[i][2].angleC = -73;

		pattern.positions[i][2].angleA = 66 + i*25;
		pattern.positions[i][2].angleB = 55;
		pattern.positions[i][2].angleC = -65;

		colorPattern.positions[i][2].angleA = -66 - i*25;
		colorPattern.positions[i][2].angleB = 55;
		colorPattern.positions[i][2].angleC = -85;
	}

}

void assignCurrentPosition(Position & pos)
{
	pos.angleA = nMotorEncoder[motorA] / 7;
	pos.angleB = 90 + nMotorEncoder[motorB] / 5; // 90 in front since arm starts at 90 degrees
	pos.angleC = nMotorEncoder[motorC] / 3;
}

void assignPosition(int angleA, int angleB, int angleC, Position & pos)
{
	pos.angleA = angleA;
	pos.angleB = angleB;
	pos.angleC = angleC;
}

int getColor()
{
	int colors[7] = {0,0,0,0,0,0,0};
	for (int i = 0; i < 100; i++)
	{
		int currColor = SensorValue[S1];
		colors[currColor]++;
		wait1Msec(5);
	}

	for (int i = 0; i < 6; i++){
		nxtDisplayString(i, "%d: %d", i, colors[i]);
	}


	int maxIndex = 0;
	for (int i = 1; i < 6; i++)
	{
		if (colors[i] > colors[maxIndex])
			maxIndex = i;
	}
	return maxIndex;
}

void scanColors(Grid & pattern, Grid & colorPattern, Grid & scanGrid)
{
	// For the color grid:
	for (int column = 0; column < 3; column++)
	{
		for (int row = 0; row < 3; row++)
			colorPattern.positions[column][row].color = column + 3; // uses blue, green, yellow, red (2,3,4,5 respectively)
	}

	for (int column = 0; column < 3; column++)
	{
		for (int row = 0; row < 3; row++)
		{
			Position currPos;
			assignCurrentPosition(currPos);

			moveToLocation(SCANSPEED, currPos, scanGrid.positions[column][row]);
			wait1Msec(1500);
			int color = getColor();
			nxtDisplayString(7, "Color: %d", color);
			pattern.positions[column][row].color = color;
		}
	}
	returnToStart();
}

void returnToStart()
{
	Position currPos, startPosition;
	assignCurrentPosition(currPos);
	assignPosition(0,90,0, startPosition);
	moveToLocation(20, currPos, startPosition);
}
