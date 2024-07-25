//--Motor--//
#define ALM_X1 22
#define ALM_X2 23
#define PULSE_X 11
#define DIR_X 32
float speed = 0; //Speed in rotations per second
float X_Hz = 0;

//--Encoder--//
#define A 42
#define B 44
#define I 46
int countTick = 0;
int countIndex = 0;
char precTick = 0;
char precIndex = 0;
char tick = 0;
char tickB =0;
char index = 0;


void setup() {
  //--Generic--//
  Serial.begin(9600);
  Serial.println("Starting");
  
  //--Motor--//
  pinMode(PULSE_X, OUTPUT);
  pinMode(DIR_X, OUTPUT);
  pinMode(ALM_X1, INPUT);
  pinMode(ALM_X2, INPUT);

  //--Encoder--//
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(I, INPUT);
}

void loop() {
  //--Failsafe
  if (digitalRead(ALM_X1)>0)
  {
    noTone(PULSE_X);
    Serial.print("ALARM X1 TRIGGERED. PLEASE RESTART");
    while(true){}
  }
  if (digitalRead(ALM_X2)>0)
  {
    noTone(PULSE_X);
    Serial.print("ALARM X2 TRIGGERED. PLEASE RESTART");
    while(true){}
  }

  
  //--Read Serial TEMPORARY, WILL START WITH AN ARRAY IN NEXT UPDATE--//
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

  //--Speed Setting--//
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

  //--Read Encoder--//
  tick = digitalRead(A);
  tickB = digitalRead(B);
  index = digitalRead(I);
  
  if(tick != precTick)
  {
    if(tick != tickB)
    {
      countTick = countTick + tick;
      precTick = tick;
    }
    else
    {
      countTick = countTick - tick;
      precTick = tick;
    }
    Serial.print("tick :");
    Serial.println(countTick);
  }
  
  if(index != precIndex)
  {
    if(countTick > 0)
    {
      countIndex = countIndex + index;
      precIndex = index;
    }
    else
    {
      countIndex = countIndex - index;
      precIndex = index;
    }
    countTick = 0;
    Serial.print("turn :");
    Serial.println(countIndex);
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
