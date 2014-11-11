//@Encoder: the current encoder value for the motor
//@endAngle: the desired end angle
//@dir: bool true is left/up, false is right/down
bool reachedAngle(int encoder, int endAngle, int dir)
{
	if (dir)
	{
		if (encoder > endAngle)
		{
			nxtDisplayString(1, "1Cur: %d", encoder);
			nxtDisplayString(2, "1end: %d", endAngle);
			nxtDisplayString(3, "1Dir: %d", dir);
			return true;
		}
	}
	else
	{
		if (encoder > endAngle)
		{
			nxtDisplayString(1, "2Cur: %d", encoder);
			nxtDisplayString(2, "2end: %d", endAngle);
			nxtDisplayString(3, "2Dir: %d", dir);
			return true;
		}
	}
	return false;
}

//@armOneUp: positive angles are up
//@armTwoUp: positive angles are up
// @angle: Angle to rotate motor A, cCW being positive
void move(int speed, int armOneUp, int armTwoUp, int rotate)
{
	bool moving  = false, done = false;
	bool motorsOff[3] = {0, 0, 0};
	bool leftA = false, upB = false, upC = false;
	int endAngleA, endAngleB, endAngleC;

	motor[motorA]= 0;
	motor[motorB]= 0;
	motor[motorC]= 0;

	while (!done)
	{
		if (!moving)
		{
			if(armOneUp > 0)
			{
				motor[motorB] = speed;
				endAngleB = nMotorEncoder[motorB] + 6*armOneUp;
				upB = true;
			}
			else if(armOneUp < 0)
			{
				motor[motorB] = -speed;
				endAngleB = nMotorEncoder[motorB] + 6*armOneUp;
			}

			if (armTwoUp > 0)
			{
				motor[motorC] = speed;
				endAngleC = nMotorEncoder[motorC] + 3*armTwoUp;
				upC = true;
			}
			else if (armTwoUp < 0)
			{
				motor[motorC] = -speed;
				endAngleC = nMotorEncoder[motorC] - 3*armTwoUp;
			}

			if (rotate > 0)
			{
				motor[motorA] = speed;
				endAngleA = nMotorEncoder[motorA] + 7*rotate;
				//nxtDisplayString(0, "Turning");
				leftA = true;
			}
			else if (rotate < 0)
			{
				motor[motorA] = -speed;
				endAngleA = nMotorEncoder[motorA] - 7*rotate;
			}

			moving = true;
		}
		if (reachedAngle(nMotorEncoder[motorA], endAngleA, leftA))
		{
			motor[motorA] = 0;
			motorsOff[0] = true;
		}
		if (reachedAngle(nMotorEncoder[motorB], endAngleB, upB))
		{
			motor[motorB] = 0;
			motorsOff[1] = true;
		}
		if (reachedAngle(nMotorEncoder[motorC], endAngleC, upC))
		{
			motor[motorC] = 0;
			motorsOff[2] = true;
		}

		if (motorsOff[0] && motorsOff[1] && motorsOff[2])
		{
			done = true;
		}
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
	int endAngle = nMotorEncoder[motorB] + 5*angle;
	while (nMotorEncoder[motorB] < endAngle);
	motor[motorB] = 0;
}
void armOneDown(int speed, int angle)
{
	motor[motorB] = -speed;
	int endAngle = nMotorEncoder[motorB] - 5*angle;


	while (nMotorEncoder[motorB] > endAngle)
	{
		nxtDisplayString(0, "the angle is %d", nMotorEncoder[motorA]);
	}
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

task main()
{
	//wait1Msec(500);
	//if (nNxtButtonPressed == -1)
	//calibrate();
	//rotateLeft(30,90);

	//rotateRight(30,90);
	//armOneUp(20,90);

	//armOneDown(20,90);
	//armTwoUp(40,90);
	//armTwoDown(40,90);
	move(20,0,0,-90);
}
