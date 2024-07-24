//--Encoder Control--//
 #define outputA 42
 #define outputB 44

 int counter = 0; 
 int aState;
 int aLastState;  


float encoder_data_X = 0; 
float encoder_data_Y = 0;

//--Motor Control--//
#define PULSE_X 30
#define DIR_X 32
#define DLY 2500 //1RPS // 60RPM
#define NUMPULSES 200
#define DIR1 0 // 1 = cw / 0 = ccw

float x;
float speed = 0;
float encoderData[8000];

void setup() {
  //--Motor Setup--//
  Serial.begin(115200); //High Speed Baud rate for fast communication
  Serial.setTimeout(1);
  //Pin Setup  
  pinMode(PULSE_X,OUTPUT);
  pinMode(DIR_X,OUTPUT);
  digitalWrite(PULSE_X, 0);
  digitalWrite(DIR_X, DIR1);
  digitalWrite(9,0); //only for machine testing
  digitalWrite(4,0); //for machine testing

  //--Encoder Setup--//
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);

  // Reads the initial state of the outputA
  aLastState = digitalRead(outputA);

  
  delay(2000);
  Serial.println("Ready");
}

void loop() {
  
      if (Serial.available() > 0) 
      {
        float newSpeed = Serial.parseFloat(); // Parse integer from serial input
        if (Serial.read() == '\n') 
        { 
          speed = newSpeed; // Update the speed variable
          Serial.print("New speed set to: ");
          Serial.println(speed);
        }
      }
      
   float rps = changeDir(speed);
   float dly = rps_to_dly(rps);
   if (dly != -999999)
   {
     step(dly);
   }

  static uint32_t last_serial_check_time = micros();
  if (micros() - last_serial_check_time >= 100000) 
  {
    last_serial_check_time += 100000;
    /*
    if (speed != 0)
    {
      int a = digitalRead(outputA); 
      int b = digitalRead(outputB);
      Serial.print("A = ");
      Serial.print(a); 
      Serial.print("B = ");
      Serial.print(b);
      if (a == b)
      {
        Serial.println("EQUAL");
      }
      else{
        Serial.println("!EQUAL"); 
      }
    }
    */

   
   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter ++;
     } else {
       counter --;
     }

   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
   Serial.println(counter);
   
  }
}


float changeDir(float x){
  if (x >= 0)
  {
    digitalWrite(DIR_X, 1);
  }
  else
  {
    x = -x;
    digitalWrite(DIR_X, 0);
  }
  return x;
}

float rps_to_dly(float speed)
{
  float dly = 0;
  if (speed == 0)
  {
    dly = -999999;
  }
  else
  {
     dly = DLY/speed;
  }
  return dly;
}

void step(float dly){
  int i; 
  for (i = 0; i<1; i++) {
    // set to high
    digitalWrite(PULSE_X, 1);
    // wait 5 microseconds
    delayMicroseconds(dly);
    // set to low
    digitalWrite(PULSE_X, 0);
    // wait 5 us
    delayMicroseconds(dly);
  }
  digitalWrite(PULSE_X, 0);
}
