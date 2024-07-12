#define PULSE_X 3
#define DIR_X 8

#define DLY 4000
#define NUMPULSES 200

#define DIR1 0 // 1 = cw / 0 = ccw
int x; 

void setup() {
  Serial.begin(115200); //High Speed Baud rate for fast communication
  Serial.setTimeout(1);
  Serial.println("BEGIN");
  pinMode(PULSE_X,OUTPUT);
  pinMode(DIR_X,OUTPUT);

  
  digitalWrite(PULSE_X, 0);
  digitalWrite(DIR_X, DIR1);


  // 2 second delay to ensure no ill startup movement from motors
  delay(2000);
}

void loop() { 
  while (!Serial.available()); 
  x = Serial.readString().toInt();
  String confirmationStatement = "Recieved signal " + String(x);
  Serial.print(confirmationStatement); 
  String output = Rotations(x);
  Serial.print(output); 
} 

String Rotations(int x){
  
  int i = 0;
  for (i = 0; i<(NUMPULSES * x); i++) {
    // set to high
    digitalWrite(PULSE_X, 1);
    // wait 5 microseconds
    delayMicroseconds(DLY);
    // set to low
    digitalWrite(PULSE_X, 0);
    // wait 5 us
    delayMicroseconds(DLY);
  }
  digitalWrite(PULSE_X, 0);
  delay(500);
  String output = "Rotated (" + String(x) + ") Times";
  return output;
}
