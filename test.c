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

void motorsOff()
{
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;
}

task main()
{
	for (int i = 0; i < 5; i++)
	{
		rotateLeft(30, 90);
		armOneDown(30, 45);
		armTwoUp(5,90);

		motorsOff();
		wait1Msec(1000);

		rotateRight(30,90);
		armOneUp(30,45);
		armTwoDown(5,90);

		motorsOff();
		wait1Msec(1000);
	}
}
