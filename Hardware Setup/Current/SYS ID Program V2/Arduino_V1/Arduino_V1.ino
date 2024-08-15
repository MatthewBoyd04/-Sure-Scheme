#include <SPI.h>                                                          //--Motor--//
#define ALM_X1 22
#define ALM_X2 23
#define PULSE_X 44 //Actually Y, allow it
#define DIR_X 32 //Actually Y, allow it
float speed = 0; //Speed in rotations per second
float X_Hz = 0;

//--Encoder--//

#define Iterations 800 //Iterations of data collection
#define X_ENC_A 19
#define X_ENC_B 18
#define Y_ENC_A 17
#define Y_ENC_B 16
#define PPR 200
#define DPR 8 // 8 mm distance per rev
#define ENC_PPR 5120
#define MAXSPD 2 // rev per second

unsigned long prev_ms = 0, curr_ms = 0;
float tmp, x_spd, y_spd;
volatile long x_curr_pos = 0, x_enc_pos = 0, y_curr_pos = 0, y_enc_pos = 0, x_prev_pos = 0, y_prev_pos = 0, diff;
volatile float x_pos_mm = 0, y_pos_mm = 0;

float X_POS=0, Y_POS=0;

unsigned long timeCurr = 0, prevTime = 0;
unsigned long diffTime;
float spdBiasX = 0, spdBiasY = 0;

volatile uint32_t last_speed_check_time = micros();
unsigned long last_speed_write_time = micros();

float EncoderData[Iterations];
unsigned long UsTimestamp[Iterations];  
int TimesRun = 0;
unsigned long StartTime = 0; 

void setup() { 
  //--Generic--//
  Serial.begin(9600);
  Serial.println("Starting");
  
  //--Motor--//
  pinMode(PULSE_X, OUTPUT);
  pinMode(DIR_X, OUTPUT);
  pinMode(ALM_X1, INPUT);
  pinMode(ALM_X2, INPUT);

  pinMode(X_ENC_A,INPUT_PULLUP);
  pinMode(Y_ENC_A,INPUT_PULLUP);

  pinMode(X_ENC_B,INPUT);
  pinMode(Y_ENC_B,INPUT);
  
  // Enable pull-up resistors if needed
  //digitalWrite(X_ENC_A, LOW);
  //digitalWrite(X_ENC_B, LOW);
  //digitalWrite(Y_ENC_B, LOW);
  //digitalWrite(Y_ENC_A, LOW);
  
  attachInterrupt(digitalPinToInterrupt(X_ENC_A), handleEncX, RISING);
  attachInterrupt(digitalPinToInterrupt(Y_ENC_A), handleEncY, RISING);

}

void handleEncX() {
    cli();
    // Determine direction of rotation based on state of pinB
    int direction = digitalRead(X_ENC_B);
    if (direction) {
        x_enc_pos++; // Clockwise rotation
    } 
    else {
        x_enc_pos--; // Counterclockwise rotation
    }
    x_pos_mm = x_enc_pos * (float)DPR / (float)ENC_PPR;
    sei();
}

void handleEncY() {
  cli();
    // Determine direction of rotation based on state of pinB
    int direction = digitalRead(Y_ENC_B);
    if (direction) {
        y_enc_pos++; // Clockwise rotation
    } 
    else {
        y_enc_pos--; // Counterclockwise rotation
    }
    y_pos_mm = y_enc_pos * (float)DPR / (float)ENC_PPR;
    sei();
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
   
//  //--Speed Setting--//
//  if (millis() - last_speed_write_time >= 500)
//  {
//    Serial.println(x_pos_mm);
/////    Serial.println(y_enc_pos);
//    last_speed_write_time = millis();
//  }

  if (micros() - last_speed_check_time >= 1000) 
  {
    if (speed != 0)
    {
      if (TimesRun == 0){StartTime = micros();}
      
      EncoderData[TimesRun] = x_pos_mm;
      UsTimestamp[TimesRun] = micros() - StartTime;
      TimesRun++;

      if (TimesRun == Iterations){
        speed = 0;
        X_Hz = 0;
        noTone(PULSE_X);
        for(int j; j < Iterations; j++)
        {
          //Serial.print("[POS] ");
          Serial.print(EncoderData[j]);
          Serial.print(",");
          //Serial.print(" [Us] ");
          Serial.println(UsTimestamp[j]);
        }
        }
    }
    
//  Serial.println(y_pos_mm);/
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
    last_speed_check_time = micros();
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
