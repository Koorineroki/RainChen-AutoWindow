#include <IRremote.h>
#include <Servo.h>

//IR Setup
int IR_PIN = 11;
IRrecv irrecv(IR_PIN);
decode_results results;

//Servo Setup
Servo myservo_01;
Servo myservo_02;
int Servo_01_PIN = 9;
int Servo_02_PIN = 5;
int angle_01 = 0;
int angle_02 = 10;

//RainSensor Setup
#define rainfall A0
int rain_value;
int set=110;
bool rain = false;
bool keep = false;

//Stage keep LED Setup
int LED = 3;
void setup()
{
  //Port
  myservo_01.attach(Servo_01_PIN);
  myservo_02.attach(Servo_02_PIN);
  irrecv.enableIRIn();
  pinMode(rainfall,INPUT);

  //Test
  Serial.begin(9600);

  pinMode(LED,OUTPUT);
}


void loop()
{
  //Map the value
  rain_value = map(analogRead(rainfall),0,1023,0,255);
  Serial.println(rain_value);


  //When Get IR Signal
  if (irrecv.decode(&results)) 
  {
    irrecv.resume();

    int signol = results.value;
    switch(signol){
      //Keep rain stage
      case 16738455 :
        keep = true;
        rain = false;
        digitalWrite(LED,HIGH);
        Serial.println("rain stage is KEEP FASLE!");
        break;
        
      case 16756815 :
        keep = false;
        Serial.println("rain stage is NOT KEEP");
        digitalWrite(LED,LOW);
        break;


      //Open ALL windows
      case 16718055 :
        if (!rain)
        {
          angle_01 = 60;
          angle_02 = 70;
        }
        break;

      //Close ALL windows
      case 16730805 :
        angle_01 = 0;
        angle_02 = 10;
        break;

        
      //Open window 1
      case 16753245 :
        if (!rain)
        {
          angle_01 = 60;
        }
        break;
        
      //Open window 2
      case 16720605 :
        if (!rain)
        {
          angle_02 = 70;
        }
        break;


      //Close window 1
      case 16769565 :
        if (!rain)
        {
          angle_01 = 0;
        }
        break;

      //Close window 2
      case 16761405 :
        if (!rain)
        {
          angle_02 = 10;
        }
        break;
      }
      

    //Satge of rain
    if (!keep)
    {
      if (rain_value >= set)
      {
        rain = false;
      }
      else if (rain_value < set)
      {
        rain = true;
      }
    }

    //Auto close
    if(rain_value<set && rain)
    {
      Serial.println("rain detected");
      angle_01 = 0;
      angle_02 = 10;
    }
  
  myservo_01.write(angle_01);
  myservo_02.write(angle_02);
  delay(200);
  }
}
