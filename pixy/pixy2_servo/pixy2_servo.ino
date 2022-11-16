//www.elegoo.com
#include <Servo.h>
#include <Pixy2.h>

Pixy2 pixy;
Servo myservo1;
Servo myservo2;
int minAngle = 700;//the pulse width, in microseconds, corresponding to the minimum (0-degree) angle on the servo (defaults to 700)
int maxAngle = 2400;//the pulse width, in microseconds, corresponding to the maximum (180-degree) angle on the servo (defaults to 2400)
int x, y;
float setx, sety;
void setup(){
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pixy.init();
  myservo1.attach(3,minAngle,maxAngle);//setting the servo IO pin and the steering range.
  myservo2.attach(4,minAngle,maxAngle); //x
} 
void loop()
{  
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks)
  {
    x = pixy.ccc.blocks[0].m_x;  
    y = pixy.ccc.blocks[0].m_y;
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    for (int i=0; i<pixy.ccc.numBlocks; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();
    }
    setx = (x/-2) + 180;
    Serial.println(setx);
    myservo1.write(setx);

    sety = (y * -0.869) + 180;
    Serial.println(sety);
    myservo2.write(180-sety);
    
  } 
  else
  {
    myservo2.write(90);
    myservo1.write(90);
  }
  delay(1);
}
