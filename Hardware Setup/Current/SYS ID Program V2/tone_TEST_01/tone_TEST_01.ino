//--Motor--//
#define PULSE_X 11
#define DIR_X 32
#define Dir_X_CCW 0 //1=cw, 1=ccw
float speed = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  // put your setup code here, to run once:
  pinMode(PULSE_X, OUTPUT);
  pinMode(DIR_X, OUTPUT);
  digitalWrite(DIR_X, 1);
  // Generate a x kHz square wave PULSE_X
  tone(PULSE_X, 100);;
  delay(1000);
  noTone(PULSE_X);
  Serial.println("Finished");
}

void loop() {
  // put your main code here, to run repeatedly:

  /*
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
   
  static uint32_t last_serial_check_time = micros();
  if (micros() - last_serial_check_time >= 1000000) 
  {
    last_serial_check_time += 1000000;
    noTone(PULSE_X);
  }
  */
   
}
