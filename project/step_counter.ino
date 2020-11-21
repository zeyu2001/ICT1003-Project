// Adapted from https://tinycircuits.com/blogs/learn/custom-fitness-tracker

int isValidStepPattern(){
  return validStepPattern;
}

int getTotalSteps(){
  return totalSteps;
}

int updatePedometer() {
 // if (millis() < sampleInterval + lastSample)return 0; we don't need to check this as it takes longer than sampleInterval to updatePulse anyway

  int temp = ((accel_sensor.rawTemp * 0.5) + 24.0);
  
  SerialMonitorInterface.print("X = ");
  SerialMonitorInterface.print(accel_sensor.X);
  
  SerialMonitorInterface.print("  Y = ");
  SerialMonitorInterface.print(accel_sensor.Y);
  
  SerialMonitorInterface.print("  Z = ");
  SerialMonitorInterface.print(accel_sensor.Z);
  
  SerialMonitorInterface.print("  Temperature(C) = ");
  SerialMonitorInterface.println(temp);

  lastSample = millis();

  accel_sensor.read();//This function gets new data from the acccelerometer

  int sum = pow(accel_sensor.X, 2);
  sum += pow(accel_sensor.Y, 2);
  sum += pow(accel_sensor.Z, 2);
  int A = sqrt(sum);
  
  int difference = abs(A - sampleOld);

  if (difference < 10) {
    difference = 0;
  }
  if (difference > 30) {
    difference = 30;
  }

  if (A - sampleOld > 0) {
    difference = sampleOld + difference;
  } else {
    difference = sampleOld - difference;
  }


  aBuff[aBuffPos] = difference;

  int Amin = 1000;
  int Amax = 0;
  for (int i = 0; i < amtSamples; i++) {
    int sampleVal = aBuff[i];
    if (sampleVal < Amin)Amin = sampleVal;
    if (sampleVal > Amax)Amax = sampleVal;
  }

  int peakToPeak = Amax - Amin;
  int threshold = (peakToPeak / 2) + Amin;

  sampleOld = sampleNew;

  bool newStep = false;
  if (abs(sampleNew - difference) > precision) {
    sampleNew = difference;
    if (peakToPeak > 70 && sampleOld > threshold && sampleNew < threshold) {
      //Serial.println("Step!");
      //potential step signal
      if (validStepPattern) {
        if (millis() > lastStepTime + stepIntervalLow && millis() < lastStepTime + stepIntervalHigh) {
          newStep = true;
        } else {
          validStepPattern = false;
        }
      } else if (millis() > lastStepTime + stepIntervalLow && millis() < lastStepTime + stepIntervalHigh) {
        newStep = true;
        validStepPattern = true;
      }
      lastStepTime = millis();
      if (newStep) {
        if(false)
        {
          stepAlert = true;
        }
        totalSteps++;
        SerialMonitorInterface.print("totalSteps:  ");
        SerialMonitorInterface.println(totalSteps);
        stepTimestamps[firstMinusOne()] = millis(); // adds the most recent step to the array containing the timestamps of the most recent steps
      }
    }
  }

  aBuffPos++;
  if (aBuffPos >= amtSamples) {
    aBuffPos = 0;
  }
  if (newStep /*&& (totalSteps & 1) == 0*/)
    return 1;
  return 0;
}

void resetStepCounters()
{
  for (int i = 0; i < 4; ++i)
  {
    stepArr[i] = 0;
  }
  stepsTowardGoal = 0;
}

void clearStepCount()
{
  totalSteps=0;
  resetStepCounters();
}
/*
void calibrate()
{
  for (int i = 0; i < amtSamples; i++) {
    while(!IMURead());
    aBuff[aBuffPos] = A;
  }
}
*/

void dailyStepReset()
{
  if(rtc.getHours() == 0 && rtc.getMinutes() == 0)
  {
    clearStepCount();
  }
}

int firstMinusOne() // returns the index in the stepTimestamps array that the next recorded step timestamp will be stored in
{
  for(int i = 0; i<getStepTrigger(); ++i)
  {
    if(stepTimestamps[i] == 4294967295)
    {
      //SerialUSB.print("First branch: ");
      //SerialUSB.println(i);
      return i; // if all array values have not been previously used, then we want to place the most recent value in the first position containing 4294967295
    }
       else if(i == getStepTrigger()-1) // if all array values have been previously used, then we want to shift array to the left and add the most recent value in the final position
    {
      //SerialUSB.print("Second branch: ");
      //SerialUSB.println(i);
      shiftLeft();
      return i;
    }
  }
}

void initStepTimestamps()
{
  for(int i = 0; i<getStepTrigger(); ++i)
  {
    stepTimestamps[i]= 4294967295;
  }
}

int getStepTrigger()
{
  return STEP_TRIGGER;
}

void shiftLeft()
{
  for(int i = 1; i<getStepTrigger(); ++i)
  {
    stepTimestamps[i-1] = stepTimestamps[i];
  }
  stepTimestamps[getStepTrigger()-1] = 4294967295;
}

void serialStepTimestamps()
{
  for(int i = 0; i<250; ++i)
  {
    SerialUSB.print(i);
    SerialUSB.print(": ");
    SerialUSB.println(stepTimestamps[i]);
  }
}

bool getStepAlert()
{
  return stepAlert;
}

void resetStepAlert()
{
  stepAlert = false;
}
