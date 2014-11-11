void move(int armOneUp, int armTwoUp, int left, int speed)
{
	int genSpeed = 20;
	bool moving  = false, done = false;
	bool motorsOff[3] = {0, 0, 0};

	while (!done)
	{
		if (!moving)
		{
			if(armOneUp > 0)
				motor[motorA] = speed;
			else
				motor[motorA] = -speed;

			if (armTwoUp > 0)
				motor[motorA] = speed;
			else
				motor[motorB] = -speed;

			if (left > 0)
				motor[motorC] = speed;
			else
				motor[motorC] = -speed;

			moving = true;
		}
		if (abs(nMotorEncoder[motorA]) < abs(left))
		{
			motor[motorA] = 0;
			motorsOff[0] = true;
		}
		if (abs(nMotorEncoder[motorB]) < abs(armOneUp))
		{
			motor[motorB] = 0;
			motorsOff[1] = true;
		}
		if (abs(nMotorEncoder[motorC]) < abs(armOneUp))
		{
			motor[motorC] = 0;
			motorsOff[2] = true;
		}

		if (motorsOff[0] && motorsOff[1] && motorsOff[2])
		{
			done = true;
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
void calibrate()
{
		calibrateMotor(motorA);
		calibrateMotor(motorB);
		calibrateMotor(motorC);
}



//Gear ratio, motorB to arm: 1:5
void rotateLeft(int speed, int angle)
{
	motor[motorA] = speed;
	nMotorEncoder[motorA] = 0;
	while (nMotorEncoder[motorA] < angle);
	motor[motorA] = 0;
}

void rotateRight(int speed, int angle)
{
	motor[motorA] = -speed;
	nMotorEncoder[motorA] = 0;
	while (nMotorEncoder[motorA] > angle);
	motor[motorA] = 0;
}
void armOneUp(int speed, int angle)
{
	motor[motorB] = speed;
	nMotorEncoder[motorB] = 0;
	while (nMotorEncoder[motorB] < 5*angle);
	motor[motorB] = 0;
}
void armOneDown(int speed, int angle)
{
	motor[motorB] = -speed;
	nMotorEncoder[motorB] = 0;
	while (nMotorEncoder[motorB] > 5*angle);
	motor[motorB] = 0;
}
void armTwoUp(int speed, int angle)
{
	motor[motorC] = speed;
	nMotorEncoder[motorC] = 0;
	while (nMotorEncoder[motorC] < angle);
	motor[motorC] = 0;
}
void armTwoDown(int speed, int angle)
{
	motor[motorC] = -speed;
	nMotorEncoder[motorC] = 0;
	while (nMotorEncoder[motorC] > angle);
	motor[motorC] = 0;
}


void pickUp()
{
	armOneDown(20, 20);
	arm
void motorsOff()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;
}

task main()
{
	calibrate();


}
