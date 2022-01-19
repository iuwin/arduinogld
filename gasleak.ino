#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(9, 10); //SIM900 Tx & Rx is connected to Arduino #7 & #8

int alarm = 8;
int smokeLevel = A5;
int threVal = 350;
int fan = 11;
int valve = 12;

void setup()
{
  //SIM900power();
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

  pinMode(alarm,OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(valve, OUTPUT);
  pinMode(smokeLevel,INPUT);
  digitalWrite(valve, HIGH); 
  Serial.println("Initializing..."); 
  delay(1000);
}

void loop()
{
  int sensorVal = analogRead(smokeLevel);

  Serial.println(sensorVal);
  if(sensorVal > threVal){
    sendSMS();
    tone(alarm,2000,1000);
    digitalWrite(fan, HIGH);
    digitalWrite(valve, LOW);
    }
  else{
    noTone(alarm);
    digitalWrite(fan, LOW);
    digitalWrite(valve, HIGH); 
    }  
}

void sendSMS()
{
  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+zzxxxxxxxxx\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("Gas leakage detected!! 😃"); //text content
  updateSerial();
  mySerial.write(26);  
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
