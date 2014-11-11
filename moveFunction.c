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
