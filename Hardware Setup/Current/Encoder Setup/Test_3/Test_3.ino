//--Encoder Control--//
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


// Variables for encoder position and previous states
volatile long encoderPos = 0;
volatile bool lastA = LOW;
volatile bool lastB = LOW;
volatile bool indexDetected = false;


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
  Serial.begin(9600); //High Speed Baud rate for fast communication
  Serial.setTimeout(1);
  //Pin Setup  
  pinMode(PULSE_X,OUTPUT);
  pinMode(DIR_X,OUTPUT);
  digitalWrite(PULSE_X, 0);
  digitalWrite(DIR_X, DIR1);
  digitalWrite(9,0); //only for machine testing
  digitalWrite(4,0); //for machine testing

  //--Encoder Setup--//
  pinMode(A, INPUT);
  pinMode(B, INPUT);
  pinMode(I, INPUT);

  delay(2000);
  Serial.println("Ready"); 
}

void loop() {

   //--<Motor Control>--
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
   //--<End Of Motor Control>--

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

void doEncoderA() {
  // Test transition
  A_set = digitalRead(encoderPinA) == HIGH;

  // and adjust counter + if A leads B
  encoderPosition += (A_set != B_set) ? +1 : -1;
}

void doEncoderB() {
  // Test transition
  B_set = digitalRead(encoderPinB) == HIGH;

  // and adjust counter + if B follows A
  encoderPosition += (A_set == B_set) ? +1 : -1;
}

void doEncoderX() {
  // Use X channel as an absolute position reference if necessary
  Serial.println("Index pulse detected");
  // You can reset encoderPosition or perform other tasks here if needed
}
