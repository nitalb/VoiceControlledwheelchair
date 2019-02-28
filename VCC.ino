

  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"


VR myVR(2,3);    
 
byte buf[64];

int R1=6;
int R2=7;
int R3=8;
int R4=9;


#define LeftRecord    (0)
#define RightRecord   (1) 
#define StopRecord   (2) 
#define StartRecord   (3) 
#define BackRecord   (4) 

void printSignature(byte *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(byte *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

    Serial.print("NONE");

  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  myVR.begin(9600);
  
  Serial.begin(115200);
      pinMode(R1, OUTPUT);
     pinMode(R2, OUTPUT);
     pinMode(R3,OUTPUT);
     pinMode(R4,OUTPUT);
     pinMode(13,OUTPUT);

    digitalWrite(R1,LOW);
    digitalWrite(R2,LOW);
    digitalWrite(R3,LOW);
    digitalWrite(R4,LOW);
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("No VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((byte)LeftRecord) >= 0){
    Serial.println("LeftRecord loaded");
    digitalWrite(13,HIGH);
  }
  else
  {
    digitalWrite(13,LOW);
  }
  
  if(myVR.load((byte)RightRecord) >= 0){
    Serial.println("RightRecord loaded");
    digitalWrite(13,HIGH);
  }
  else
  {
    digitalWrite(13,LOW);
  }
  
  if(myVR.load((byte)StopRecord) >= 0){
    Serial.println("StopRecord loaded");
    digitalWrite(13,HIGH);
  }
  else
  {
    digitalWrite(13,LOW);
  }
  
  if(myVR.load((byte)StartRecord) >= 0){
    Serial.println("StartRecord loaded");
    digitalWrite(13,HIGH);
  }
  else
  {
    digitalWrite(13,LOW);
  }
  
  if(myVR.load((byte)BackRecord) >= 0){
    Serial.println("BackRecord loaded");
    digitalWrite(13,HIGH);
  }
  else
  {
    digitalWrite(13,LOW);
  }
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case LeftRecord:       
       
        digitalWrite(R2, HIGH);
        digitalWrite(R1, LOW);
      digitalWrite(R3,LOW);
        digitalWrite(R4,LOW);
        delay(1000);
        digitalWrite(R2, LOW);
           
        break;
      case RightRecord:       
        digitalWrite(R1, HIGH);
        digitalWrite(R2, LOW);
        digitalWrite(R3,LOW);
        digitalWrite(R4,LOW);
        delay(1000);
        digitalWrite(R1, LOW);
       
        break;
      case StopRecord:       
        digitalWrite(R1, LOW);
        digitalWrite(R2, LOW);
        digitalWrite(R3,LOW);
        digitalWrite(R4,LOW);
        break;
      case StartRecord:
        digitalWrite(R1, HIGH);
        digitalWrite(R2, HIGH);
        digitalWrite(R3,LOW);
       digitalWrite(R4,LOW);
        break;
      case BackRecord:
        digitalWrite(R1, HIGH);
       digitalWrite(R2, HIGH);
       digitalWrite(R3,HIGH);
       digitalWrite(R4,HIGH);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
    printVR(buf);
  }
}



