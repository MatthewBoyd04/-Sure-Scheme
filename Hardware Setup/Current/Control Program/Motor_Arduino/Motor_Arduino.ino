#define Data_X_Len 8000

bool ready = false;
String ID = "Motor_Arduino: ";
int Data_X[Data_X_Len];

void send_message(String msg)
{
  Serial.print(ID);
  Serial.println(msg);
}

void controller_checks(){
  ready = false;
  while (ready == false)
  {
    if (Serial.available() > 0) 
    {
      // Read the incoming byte
      String receivedString = Serial.readStringUntil('\n');
    
      // Remove any trailing whitespace characters
      receivedString.trim();
    
      // Check if the received string matches "READY"
      if (receivedString.equals("READY")) 
      {
        send_message("Code READY received");
        ready = true;
      }
    }
  }
  delay(1000);
  send_message("START");
  
  ready = false;
  while (ready == false)
  {
    if (Serial.available() > 0) 
    {
      // Read the incoming byte
      String receivedString = Serial.readStringUntil('\n');
    
      // Remove any trailing whitespace characters
      receivedString.trim();
    
      // Check if the received string matches "READY"
      if (receivedString.equals("START_MOTOR_DATA_TRANSMISSION")) 
      {
        send_message("Code START_MOTOR_DATA_TRANSMISSION received");
        ready = true;
      }
    }
  }
  delay(1000);
  send_message("READY_FOR_DATA"); 
  ready = false;
  int i = 0; 
  while (ready == false)
  {
    if (Serial.available() > 0) 
    {
      // Read the incoming byte
      String receivedString = Serial.readStringUntil('\n');
    
      // Remove any trailing whitespace characters
      receivedString.trim();
    
      // Check if the received string matches "READY"
      if (receivedString.equals("END_OF_DATA_TRANSMISSION")) 
      {
        send_message("Code END_OF_DATA_TRANSMISSION received");
        ready = true;
      }
      else
      {
        
        if (i < Data_X_Len)
        {
          Data_X[i] = receivedString.toInt();
        }
        i = i + 1;
      }
    }
  }
  delay(1000);
  send_message("Data Recieved, awaiting next command");
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:
  controller_checks();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
