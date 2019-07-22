

float temp_c;
float temp_f = 0;
float humidity = 0;
float prev_temp_f = 0;
float prev_humidity = 0;

const int statColumn = 15;
const int statRow = 0;

void UpdateSensor()
{
	float dewPoint;

//    digitalWrite(DRED, HIGH);
    lcd.setCursor(statColumn, statRow);
    lcd.print(dotdotdot);

    // save prev values
    prev_temp_f = temp_f;
    prev_humidity = humidity;

    // Read values from the sensor
    int err = Sensor.measure( &temp_c, &humidity, &dewPoint);
    temp_f = 32 + 9./5. * temp_c;

    // Print the values to the serial port
    Serial.print("Temperature: ");
    Serial.print(temp_f );
    Serial.print("F / ");
    //Serial.print(temp_f );
    Serial.print(" Humidity: ");
    Serial.print(humidity);
    Serial.println("%    ");

    UpdateDisplayHomePage();

//     digitalWrite(DRED, LOW);
}

void UpdateDisplayHomePage()
{
   lcd.setCursor(0, 0);
   lcd.print("Temp   Humidity");

    char tdirection = UpOrDown( temp_f, prev_temp_f );
    char hdirection = UpOrDown( humidity, prev_humidity );

    lcd.setCursor(0, 1);
    lcd.print(temp_f );
    lcd.print("F");
    lcd.print( tdirection );
    lcd.print(" ");

    lcd.setCursor(8, 1);
    lcd.print(humidity);
    lcd.print("%");
    lcd.print( hdirection );
    lcd.print(" ");

    lcd.setCursor(statColumn, statRow);
    lcd.print(" ");
}

char UpOrDown( float value, float prev_value ){
  if( prev_value == 0 )
    return ' ';
    if( value > prev_value )
      return uparrow1;
    else
      return downarrow1;
}

void UpdateDisplay(){
//    digitalWrite(DGRN, HIGH);

    lcd.setCursor(0, 1);
    //lcd.print(digitalRead(EncoderA));
    //lcd.print(" ");
    //lcd.print(digitalRead(EncoderB));
    //lcd.print(" ");
    lcd.print(" ");
    lcd.print(Counter); //millis()/1000);
    lcd.print("  ");

//    digitalWrite(DGRN, LOW);
}


