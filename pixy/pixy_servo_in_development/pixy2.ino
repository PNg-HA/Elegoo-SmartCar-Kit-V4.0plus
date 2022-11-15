#include <Servo.h>
#include <Pixy2.h>

// This is the main Pixy object 
Pixy2 pixy;
Servo myservo;
// khai báo cho động cơ
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 4
#define carspeed 150
float deadZone = 0.3125; //dead zone tha defines where the object will not move at all equals to 15%

////////////////////////////////////////////////////////

int cont = 0;
int signature, x, y, width, height;
float cx, cy, area;
void forward(int speed){ 
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("FORWARD");
}

void back(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("BACK");
}

void left(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("LEFT");
}

void right(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("RIGHT");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("STOP");
} 

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  myservo.attach(3);
  myservo.write(90);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pixy.init();
}

float pixyCheck() {
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[32];
  // grab blocks!
  blocks = pixy.ccc.getBlocks();

  // If there are detect blocks, print them!
  if (blocks)
  {
    signature = pixy.ccc.blocks[0].m_signature;
    height = pixy.ccc.blocks[0].m_height; //height of the object
    width = pixy.ccc.blocks[0].m_width; //width of the object
    x = pixy.ccc.blocks[0].m_x;//x value of the object
    y = pixy.ccc.blocks[0].m_y;//y value of the object
    cx = (x + (width / 2)); //center x position
    cy = (y + (height / 2)); //center y position
    cx = mapfloat(cx, 0, 320, -1, 1); // aplying normalization. If value is from 0-320 change from -1 to 1. This helps in the computation
    cy = mapfloat(cy, 0, 200, 1, -1);
    area = width * height;

      //   Serial.print("sig: ");
      //   Serial.print(signature);
      //   Serial.print(" x:");
      //   Serial.print(x);
      //   Serial.print(" y:");
      //   Serial.print(y);
      //   Serial.print(" width: ");
      //   Serial.print(width);
      //   Serial.print(" height: ");
      //   Serial.print(height);
      //  Serial.print(" cx: ");
      //   Serial.print(cx);
      //   Serial.print(" cy: ");
      //   Serial.println(cy);
  }
  else {
    cont += 1;
    if (cont == 100) {
    cont = 0;
    cx = 0;
    }
  }
  return cx; //sending back the x location to tell our robot to turn in a particular direction whether is positive or negative
}

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void loop()
{ myservo.write(90);
  int i; 
  // grab blocks!
  pixy.ccc.getBlocks();
  
  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks)
  {
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();
    }
    float turn = pixyCheck(); //checking the value of the turn. how much deviation we have from the center using the pixy check function
    if (turn > -deadZone && turn < deadZone) 
    { //applying deadzone
      turn = 0;
    }
    if (turn < 0)
    {
      left(carspeed); //deciding how much to move the robot
    }
    else if (turn > 0) { //looking if the turn is positive or negative and moving, otherwise keep going forward
      right(carspeed);
    }
    else {
      forward(carspeed); //takes the value of the left speed and the right speed
    }
  }
  else 
  {
    myservo.write(170);
    delay(200);
    pixy.ccc.getBlocks();
    delay(300);
    if (pixy.ccc.numBlocks){
      left(carspeed);
      delay(190);
      stop();
      delay(500);    
    }
    else      
    {
      myservo.write(15);
      delay(200);
      pixy.ccc.getBlocks();
      delay(300);
      if (pixy.ccc.numBlocks){             
        right(carspeed);
        delay(190);
        stop();
        delay(500);  
      }
      else
      { 
        myservo.write(90);
        delay(200);
        stop();
        delay(1);  
      }  
    }
    delay(1);
  }  
  delay(1);
}

