
//--Accelerometer--//
#include <Wire.h>  // Wire library - used for I2C communication

int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs

static uint32_t startTime = 0;

//--Motor Control--//
#define PULSE_X 3
#define DIR_X 7
#define DLY 2500 //1RPS // 60RPM
#define NUMPULSES 200
#define DIR1 0 // 1 = cw / 0 = ccw

float x;
float speed = 0;

void writeRegister(int deviceAddress, byte address, byte val) {
  Wire.beginTransmission(deviceAddress); // start transmission to device 
  Wire.write(address);       // send register address
  Wire.write(val);           // send value to write
  Wire.endTransmission();    // end transmission
}


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
  


  //--Acellerometer Setup--//
  Wire.begin(); // Initiate the Wire library
  // Set ADXL345 in measuring mode
  writeRegister(ADXL345, 0x2D, 8); // Power_ctl register - measurement mode
  // Set data rate to 1600Hz
  writeRegister(ADXL345, 0x2C, 0x0F); // BW_RATE register - 1600Hz
  // Set range to ±4g
  writeRegister(ADXL345, 0x31, 0x01); // DATA_FORMAT register - ±4g range

  // 2 second delay to ensure no ill startup movement from motors
  delay(2000);
  Serial.println("Ready");
}

void loop() { 

  // Check for serial input every 1ms
  static uint32_t last_serial_check_time = micros();
  if (micros() - last_serial_check_time >= 1000) 
  {
    last_serial_check_time += 1000;
    // Read serial input if available
    if (speed == 0) { //Only allow speed to be changed once on start of program

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
    }
  }

  //Move the motor ever 1ms
  static uint32_t last_motor_time = micros();
  if (micros() - last_motor_time >= 1000) 
  {
        last_motor_time += 1000;
        float rps = changeDir(speed);
        float dly = rps_to_dly(rps);
        if (dly != -999999)
        {
          step(dly);
        }
  }
  
  static uint32_t last_conversion_time = micros();
  if (micros() - last_conversion_time >= 1000) {
      last_conversion_time += 1000;
      if (speed != 0)
      {
        if (startTime == 0)
        {
          startTime = last_conversion_time;
        }

        // === Read acceleromter data === //
        Wire.beginTransmission(ADXL345);
        Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
        X_out = ( Wire.read()| Wire.read() << 8); // X-axis value
        X_out = X_out/128; //For a range of +-4g, we need to divide the raw values by 128, according to the datasheet
        Y_out = ( Wire.read()| Wire.read() << 8); // Y-axis value
        Y_out = Y_out/128;
        Z_out = ( Wire.read()| Wire.read() << 8); // Z-axis value
        Z_out = Z_out/128;

        unsigned long x = micros();
        unsigned long sendTime = x - startTime;
        unsigned long limit = 4000000;
        
        if (sendTime > limit)
        {
          speed = 0;
        }

        //Stop serial, change to array and batch send
        Serial.print(X_out); 
        Serial.print(",");
        Serial.print(Y_out); 
        Serial.print(",");
        Serial.print(Z_out); 
        Serial.print(",");
        Serial.println(sendTime);  
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
  // set to high
  digitalWrite(PULSE_X, 1);
  // wait 5 microseconds
  delayMicroseconds(dly);
  // set to low
  digitalWrite(PULSE_X, 0);
  // wait 5 us
  delayMicroseconds(dly);
}
