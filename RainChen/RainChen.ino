//红外声明
#include <IRremote.h>
IRrecv irrecv(11);
decode_results results;

//舵机声明
#include <Servo.h>
Servo myservo;

//雨水传感器声明
#define rainfall A0
int value;
int set=110;


void setup()
{
  //关联端口
  myservo.attach(9);
  irrecv.enableIRIn();
  pinMode(rainfall,INPUT);

  //测试专用
  Serial.begin(9600);
}

void loop() {
  
  static int a =0;
  value = map(analogRead(rainfall),0,1023,0,255);

  Serial.println(value);

  if (irrecv.decode(&results)) {
    irrecv.resume();

     //开窗户
    if(results.value==16718055 && value>=set)
    {
      a = 60;
    }

    //关窗户
    if(results.value==16730805)
    {
      a = 0;
    }
  }

    //自动关窗
    if(value<set)
    {
      Serial.println("rain detected");
      a = 0;
    }
  
  
  myservo.write(a);
  delay(200);
}
