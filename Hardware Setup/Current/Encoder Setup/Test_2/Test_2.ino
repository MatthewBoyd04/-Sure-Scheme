//--Encoder Control--//
#define encoderPinA 30
#define encoderPinB 31
#define encoderPinX 28

volatile long encoderPosition = 0;
volatile boolean A_set = false;
volatile boolean B_set = false;

//--Motor Control--//
#define PULSE_X 3
#define DIR_X 7
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
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(encoderPinX, INPUT);
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  digitalWrite(encoderPinX, HIGH);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinX), doEncoderX, RISING);

  
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
    Serial.print("Encoder Position: ");
    Serial.println(encoderPosition);
   
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
