int Check_Sum_Error;
int X_Signal_Status;
int FAIL = 0;
int Y_Signal_Status;
int X_Deflection_Status;
int NOFAIL = 1;
int Y_Deflection_Status;
int Power_Status;
int EHT_CONTROL;
int V6_3_CONTROL;
int AUTO_NORM_SELECT;
int MSW;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned char Cycle_Count = 0;
  unsigned char tempMSW;
  boolean Temp_Auto_Normal;

  tempMSW = 0x3f; //Mode is 0, TIP = 0
  Temp_Auto_Normal = 0; //Normal Mode
  //If HUD checksum error, Declare TIP = 1 and BIT Result Fail.
  if (Check_Sum_Error)
  {
    tempMSW |= 0x40;//If HUD checksum error, Declare TIP = 1
    tempMSW &= ~0x01;//Declare BIT Result Fail
  }
  else
  {
  }
  //If X Singal or Y Signal is not present,
  //Declare Video Signal Status fail
  if ((X_Signal_Status == FAIL) && (Y_Signal_Status == FAIL))
  {
    tempMSW &= ~0x02;//Declare Video Signal Status Fail
    Temp_Auto_Normal = 1; //Switch to Autonomous Mode
  }
  else
  {
    //no fail
  }
  //If X signal is present but x deflection is absent,
  //Declare X Deflection fail and BIT Result Fail.
  if ((X_Deflection_Status == FAIL) && (X_Signal_Status == NOFAIL))
  {
    tempMSW &= ~0x04;//Declare X deflection fail
    tempMSW &= ~0x01;//Declare BIT Result Fail
  }
  else
  {
    //no fail
  }
  //If Y signal is present but deflection is absent,
  //Declare Y Deflection fail and BIT Result Fail.
  if ((Y_Deflection_Status == FAIL) && (Y_Signal_Status == NOFAIL))
  {
    tempMSW &= ~0x08;//Declare Y deflection fail
    tempMSW &= ~0x01;//Declare BIT Result Fail
  }
  else
  {
    //no fail
  }

  //If EHT Supply OK is not present,
  //Declare EHT Status fail and BIT Result Fail.
  if ((Power_Status & 0x04) != 0x04)
  {
    tempMSW &= ~0x10;//EHT is bad
    tempMSW &= ~0x01;//Declare BIT Result Fail
  }
  else
  {
    //no fail
  }
  //If 6.3 Volt filament PS OK is not present,
  //Declare "6.3V/60V BOTH GOOD" Bit as FAIL and BIT Result Fail.
  //since 6.3 and 60 good signals are inverted 1->BAD 0->GOOD
  if ((Power_Status & 0x03) != 0x00)
  {
    tempMSW &= ~0x20;//Decalre "6.3V/60V BOTH GOOD" bit FAIL
    if ((Power_Status & 0x01) == 0x01)
    {
      //if deflection power fails immidiately switch off EHT
      //this is to protect the CRT phosphor
      EHT_CONTROL = 0;
      V6_3_CONTROL = 0;
      tempMSW &= ~0x10;
      //  HUD_Restart_Request=TRUE;//earlier it was unmasked as 60v is nt behaving properly.
    }
    else
    {
    }
    tempMSW &= ~0x01;//Declare BIT Result Fail
  }
  else
  {
    //no fail
  }
  AUTO_NORM_SELECT = Temp_Auto_Normal;
  MSW = tempMSW;
  //If any critical error, Restart HUD after completion of 10 cycles
  //after power ON or WDT Reset to see if it can recover
  //This is to wait for app 800 milliseconds after initial power ON or WDT reset
  //before restarting. This achieved by waiting for a Cycle_Count to reach
  //count of 10, corresponding to 10*80ms = 800ms after reset or power ON.
  //This will ensure enough recovery time for power supply and
  //time for OAC to detect and log the failure through serial comm channel.
  if ((Cycle_Count++) >= 10)
  {
    Cycle_Count = 10;
    //if any of the supplies is not ok and both the deflections fail (here i/p signal is present/ remember in auto mode one of the i/p is present)
    if (((Power_Status & 0x07) != 0x04) || ((MSW & 0x0C) == 0x00))
    {
      HUD_Restart();
    }
    else
    {
    }
  }
  else
  {
  }
}

void HUD_Restart(void)
{
  return;
}


