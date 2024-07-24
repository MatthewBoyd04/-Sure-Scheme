//--Motor--//
#define PULSE_X 11
#define DIR_X 32
float speed = 0; //Speed in rotations per second
float X_Hz = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting");
  // put your setup code here, to run once:
  pinMode(PULSE_X, OUTPUT);
  pinMode(DIR_X, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) 
   {
     float newSpeed = Serial.parseFloat(); // Parse integer from serial input
     if (Serial.read() == '\n') 
     { 
       speed = newSpeed; // Update the speed variable
       Serial.print("New speed set to: ");
       Serial.println(speed);
       X_Hz = speed_to_Hz(speed);
     }
   }
   
  static uint32_t last_speed_check_time = micros();
  if (micros() - last_speed_check_time >= 1000) 
  {
    last_speed_check_time += 1000;
    if (X_Hz < 32)
    {
      //Serial.println("Set NO TONE");
      noTone(PULSE_X);
    }
    else
    {
      //Serial.print("Set Tone:");
      //Serial.println(X_Hz);
      tone(PULSE_X,X_Hz);
    }
  }
  
}

float speed_to_Hz(float newSpeed)
{
  if (newSpeed < 0)
  {
    Serial.println("Setting Dir as 0");
    digitalWrite(DIR_X, 0);
    newSpeed = -newSpeed;
  }
  else
  {
    Serial.println("Setting Dir as 1");
    digitalWrite(DIR_X, 1);
  }
  return newSpeed * 200; 
  
}
