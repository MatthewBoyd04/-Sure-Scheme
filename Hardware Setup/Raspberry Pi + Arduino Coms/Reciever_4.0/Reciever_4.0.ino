#define PULSE_X 3
#define DIR_X 8

#define DLY 2500 //1RPS // 60RPM
#define NUMPULSES 200

#define DIR1 0 // 1 = cw / 0 = ccw
float x;
float speed = 0;

void setup() {
  Serial.begin(115200); //High Speed Baud rate for fast communication
  Serial.setTimeout(1);

  //Pin Setup  
  pinMode(PULSE_X,OUTPUT);
  pinMode(DIR_X,OUTPUT);
  digitalWrite(PULSE_X, 0);
  digitalWrite(DIR_X, DIR1);


  // 2 second delay to ensure no ill startup movement from motors
  delay(2000);
}

void loop() { 
  static uint32_t last_conversion_time = micros();
  if (micros() - last_conversion_time >= 1000) 
  {
        last_conversion_time += 100;
        float rps = changeDir(speed);
        float dly = rps_to_dly(rps);
        if (dly != -999999)
        {
          step(dly);
        }
  } 

  // Check for serial input every 100ms (or any desired interval)
  static uint32_t last_serial_check_time = micros();
  if (micros() - last_serial_check_time >= 100000) 
  {
    last_serial_check_time += 100000;
    // Read serial input if available
    if (Serial.available() > 0) 
    {
      float newSpeed = Serial.parseFloat(); // Parse integer from serial input
      if (Serial.read() == '\n') 
      { 
        // Ensure it reads until newline character
        speed = newSpeed; // Update the speed variable
        Serial.print("New speed set to: ");
        Serial.println(speed);
      }
    }
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
