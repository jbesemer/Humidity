



void AnimateUI()
{
  // process accumulated counts, if any
 
   unsigned long now = millis();
   
  int count = GetCount(); 
  
  if( count != 0 ){
    PreviousInputEvent = now;
    
    switch( UIMode ){
     case DarkOps:
       ExitBlackOpsMode();  // and otherwise ignore input
       break;
    case Selecting:
      UpdateSelection( count );
      break;
    case Editing:
      EditSelection( count );
      break;
    }
  }
  
  // then process button-presses, if any
  
  bool Cancel = false;
  
  // button is active LOW!!
  
  int button = digitalRead(Button);
  if( button == 0 ){
    PreviousInputEvent = now;
    while( digitalRead(Button) == 0 ){
    }
    
    unsigned long elapsed = millis() - PreviousInputEvent;
    
    // Serial.print( "elapsed = ");
    // Serial.println( elapsed );

    Cancel = ( elapsed >= 500 );
       
    switch( UIMode ){
     case DarkOps:
       ExitBlackOpsMode();  // and ignore input
       break;
    case Selecting:
      if( Cancel ){
        //Serial.println( "cancel selecting");
       ReturnToNormal();
      }
      else
      {
        if( SelectedIndex < UITableEntryCount 
        && UITable[ SelectedIndex ].Variable != NULL )
        {
          UIMode = Editing;
          BeginEdit();
        }
      }
      break;
    case Editing:
      if( Cancel )
        CancelEdit();
       else
         AcceptEdit();
       UIMode = Selecting;
       UpdateSelection( 0 );
       
       // if exiting set dim intensity, restore to normal
       if( UITable[ SelectedIndex ].Variable == &DisplayPwmDim )
       {
         SetDisplayPwm( DisplayPwmNormal );
       }
       break;
    }
  }

  // here, now-PreviousInputEvent == 0 if there was a button or encoder event ahove
  
  // revert to normal mode after 30 sec of inactivity.
  if( SelectedIndex != 0
  && now - PreviousInputEvent > ReturnNormalTimeout * 1000 )
  {
    Serial.println( "returning to normal");
     ReturnToNormal();
  }

  // revert to normal mode after 30 sec of inactivity.
  if( UIMode != DarkOps 
  && now - PreviousInputEvent > (unsigned long)DimDisplayTimeout * 1000 )
  {
     EnterBlackOpsMode();
  }
  
  if( now - PreviousTickEvent > 500 )
  {
    static byte cnt = 0;
    PreviousTickEvent = now;
    SetShiftRegister( cnt++ );
  }
}

void EnterBlackOpsMode()
{
  ReturnToNormal();  // should already be normal
  UIMode = DarkOps;
  SetDisplayPwm( DisplayPwmDim );
  Serial.println( "Dim....." );
}

void ExitBlackOpsMode()
{
  UIMode = Selecting;
  SetDisplayPwm( DisplayPwmNormal );
  Serial.println( "Bright..." );
}

const int ValueColumn = 8;

void UpdateSelection( int count )
{
  // Serial.print( "Update selection ");Serial.println(count);
  
  SelectedIndex += count;
  
  if( SelectedIndex < 0 )
    SelectedIndex = 0;
  
  if( SelectedIndex >= UITableEntryCount)
      SelectedIndex = UITableEntryCount - 1;

    Serial.print("Index=");
    Serial.print(SelectedIndex);
    Serial.print(" ");
    Serial.println(UITable[ SelectedIndex ].Title);
    
   lcd.clear();
    
   if( SelectedIndex == 0 ){
    UpdateDisplayHomePage();
    return;
   }
    
  if( SelectedIndex == UITableEntryCount ){
    lcd.setCursor(0, 0);
    lcd.print("[scroll back for");
    lcd.setCursor(0, 1);
    lcd.print("  more \1\2\3\4\5\6 options]");
    return;
  }
  
    lcd.setCursor(0, 0);
    // lcd.print("Change ");
    lcd.print(UITable[ SelectedIndex ].Title);
    // lcd.print("? ");

    lcd.setCursor(ValueColumn, 1);
    lcd.print( *(UITable[ SelectedIndex ].Variable));
}

// int SavedValue;

void BeginEdit()
{
  Serial.println( "BeginEdit ");  //Serial.println(SelectedIndex);
  
//  SavedValue = *(UITable[ SelectedIndex ].Variable);
  
    lcd.setCursor(0, 0);
    lcd.print(UITable[ SelectedIndex ].Title);

    lcd.setCursor(0, 1);
    lcd.print( "CHANGE" );
    lcd.setCursor(ValueColumn, 1);
    lcd.print( *(UITable[ SelectedIndex ].Variable));

       // if changing dim intensity, set display to dim
       if( UITable[ SelectedIndex ].Variable == &DisplayPwmDim )
       {
         SetDisplayPwm( DisplayPwmDim );
       }
}

void EditSelection( int count )
{
  // Serial.print( "EditSelection ");Serial.println(count);
  
  int value = *(UITable[ SelectedIndex ].Variable);
  value += count;
  
  if( value > UITable[ SelectedIndex ].Max )
    value = UITable[ SelectedIndex ].Max;
  
  if( value < UITable[ SelectedIndex ].Min )
    value = UITable[ SelectedIndex ].Min;
    
  *(UITable[ SelectedIndex ].Variable) = value;

    lcd.setCursor(ValueColumn, 1);
    lcd.print( value );
    lcd.print( "   " );
    
    Serial.print("Value = ");
    Serial.println(value);
    
    UpdateFunction* fn = UITable[ SelectedIndex ].Func;
    
    if( fn != NULL )
      (*fn)( value );
}


void CancelEdit()
{
  Serial.println( "CancelEdit ");
  //*(UITable[ SelectedIndex ].Variable) = SavedValue;  
  UITable[ SelectedIndex ].LoadSettingFromEEPROM();
}

void AcceptEdit()
{
  Serial.println( "AcceptEdit: " );
  Serial.println( *(UITable[ SelectedIndex ].Variable) );
  
  // SaveAllSettingsToEEPROM();
  UITable[ SelectedIndex ].SaveSettingToEEPROM();
}

void ReturnToNormal()
{
    if( UIMode == Editing )
      CancelEdit();
    
    UIMode = Selecting;
    UpdateSelection( -SelectedIndex );  // back to 0 and update display
}

void NormalRunMode()
{
  unsigned long ms = millis();

  if( ms - prevUpdateSensor > UpdateSensorInterval * 1000 ){
    prevUpdateSensor = ms;
    UpdateSensor();
  }
}


