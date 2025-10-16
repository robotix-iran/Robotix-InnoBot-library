//  --------                 --------
// |  S1    |               |  S4    |
// | joint9 |               | joint7 |
//  ----- --------     --------- -----
//       |  S6    |   |  S7    |
//       | joint8 |   | joint6 |
//        --------     --------
//       |  S5    |  |   S8    |
//       | joint2 |  |  joint4 |
//  ----- --------    --------- -----
// |  S2    |               |  S3    |
// | joint3 |               | joint5 |
//  --------                 --------
//                Front

#include <RobotixInnoBot.h>

// Servo declarations
Servo joint2;  // Right front hip
Servo joint3;  // Right front leg
Servo joint4;  // Left front hip
Servo joint5;  // Left front leg
Servo joint6;  // Left rear hip
Servo joint7;  // Left rear leg
Servo joint8;  // Right rear hip
Servo joint9;  // Right rear leg


// Home positions
int home_joint2 = 120;
int home_joint3 = 15;
int home_joint4 = 60;
int home_joint5 = 165;
int home_joint6 = 100;
int home_joint7 = 15;
int home_joint8 = 70;
int home_joint9 = 165;

void setup(){
  joint2.attach(PIN_SERVO_5);
  joint3.attach(PIN_SERVO_2);
  joint4.attach(PIN_SERVO_8);
  joint5.attach(PIN_SERVO_3);
  joint6.attach(PIN_SERVO_7);
  joint7.attach(PIN_SERVO_4);
  joint8.attach(PIN_SERVO_6);
  joint9.attach(PIN_SERVO_1);

  joint2.write(home_joint2);
  joint3.write(home_joint3);
  joint4.write(home_joint4);
  joint5.write(home_joint5);
  joint6.write(home_joint6);
  joint7.write(home_joint7);
  joint8.write(home_joint8);
  joint9.write(home_joint9);

  delay(3000);
}

void loop(){
  sithome();
  delay(2000);

  stand1();
  delay(2000);

  forward(5);
  delay(2000);

  backward(5);
  delay(2000);

  rightturn(5);
  delay(2000);

  leftturn(5);
  delay(2000);

  twist();
  delay(2000);

  sithome();
  delay(2000);

  stand1();
  delay(2000);

  stand2();
  delay(2000);

  stand3();
  delay(2000);

  downaction(4);
  delay(2000);

  wink(4);
  delay(2000);
}

// Idle time
void idle(){
  delay(100);
}

// Home standing position
void standhome(){
  joint2.write(home_joint2);
  joint3.write(home_joint3);
  joint4.write(home_joint4);
  joint5.write(home_joint5);
  joint6.write(home_joint6);
  joint7.write(home_joint7);
  joint8.write(home_joint8);
  joint9.write(home_joint9);
}

// Extend arms and touch the ground
void sithome(){
    joint2.write(135);
    joint3.write(65);
    joint4.write(45);
    joint5.write(110);
    joint6.write(135);
    joint7.write(80);
    joint8.write(45);
    joint9.write(110);
}

// Standing up motion 1
void stand1(){
  sithome();
  joint2.write(170);
  delay(300);
  joint2.write(home_joint2);
  delay(300);
  joint4.write(10);
  delay(300);
  joint4.write(home_joint4);
  delay(300);
  joint6.write(170);
  delay(300);
  joint6.write(home_joint6);
  delay(300);
  joint8.write(10);
  delay(300);
  joint8.write(home_joint8);
  delay(300);
  
  joint3.write(home_joint3);
  joint5.write(home_joint5);
  joint7.write(home_joint7);
  joint9.write(home_joint9);
}

// Standing up motion 2
void stand2(){
  sithome();
  joint2.write(175);
  joint4.write(5);
  joint6.write(175);
  joint8.write(5);
  delay(600);
    
  joint2.write(home_joint2);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  joint8.write(home_joint8);
  delay(600);
  
  joint3.write(home_joint3);
  joint5.write(home_joint5);
  joint7.write(home_joint7);
  joint9.write(home_joint9);
}

// Standing up motion 3
void stand3()
{ 
  sithome();
  int i;
  int j = 90;
  int k = 90;
  joint2.write(home_joint2);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  joint8.write(home_joint8);
  for(i = 90; i < 165; i++)
  {
    joint5.write(i);
    j = j - 1;
    joint3.write(j);
    delay(20);
  }
  
  for(i = 115; i < 165; i++)
  {
    joint9.write(i);
    k = k - 1;
    joint7.write(k);
    delay(20);
  }
}

// Quick up-down motion (number of steps)
void downaction(unsigned int step){
  while (step-- > 0){
  sithome();
  delay(100);
  standhome();
  delay(100);
  }
}

// Wave hand
void wink(unsigned int step){
  standhome();
  joint9.write(home_joint9-30);
  
  while (step-- > 0){
  joint5.write(30);
  joint4.write(home_joint4 + 60);
  delay(300);
  joint4.write(home_joint4 - 60);
  delay(300);
  }
}

// Wiggle body
void twist(){
  joint3.write(home_joint3);
  joint5.write(home_joint5);
  joint7.write(home_joint7);
  joint9.write(home_joint9);
  
  for(int right=90;right<170;right++){
      joint2.write(right);
      joint6.write(right);
      joint4.write(right-90);
      joint8.write(right-90);
      delay(10);
  }

   for(int right=170;right>90;right--){
      joint2.write(right);
      joint6.write(right);
      joint4.write(right-90);
      joint8.write(right-90);
      delay(10);
  }
}

// Move forward (number of steps)
void forward(unsigned int step){
  while (step-- > 0){
  joint3.write(home_joint3+30);
  joint7.write(home_joint7+30);
  delay(100);
  joint2.write(home_joint2+30);
  joint8.write(home_joint8-30);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  delay(100);
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();
  
  joint5.write(home_joint5-30);
  joint9.write(home_joint9-30);
  delay(100);
  joint2.write(home_joint2);
  joint8.write(home_joint8);
  joint4.write(home_joint4-30);
  joint6.write(home_joint6+30);
  delay(100);
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();
  }
}

// Move backward (number of steps)
void backward(unsigned int step){
  while (step-- > 0){
  joint3.write(home_joint3+30);
  joint7.write(home_joint7+30);
  delay(100);
  joint2.write(home_joint2);
  joint8.write(home_joint8);
  joint4.write(home_joint4-30);
  joint6.write(home_joint6+30);
  delay(100);
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();
  
  joint5.write(home_joint5-30);
  joint9.write(home_joint9-30);
  delay(100);
  joint2.write(home_joint2+30);
  joint8.write(home_joint8-30);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  delay(100);
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();
  }
}

// Turn right (number of steps)
void rightturn(unsigned int step){
  while (step-- > 0){
  joint5.write(home_joint5-30);
  joint9.write(home_joint9-30);
  delay(100);
  joint2.write(home_joint2+30);
  joint8.write(home_joint8-30);
  joint4.write(home_joint4-30);
  joint6.write(home_joint6+30);
  delay(100);
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();
  
  joint3.write(home_joint3+30);
  joint7.write(home_joint7+30);
  delay(100);
  joint2.write(home_joint2);
  joint8.write(home_joint8);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  delay(100);
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();
  }
}

// Turn left (number of steps)
void leftturn(unsigned int step){
  while (step-- > 0){
  joint3.write(home_joint3+30);
  joint7.write(home_joint7+30);
  delay(100);
  joint2.write(home_joint2+30);
  joint8.write(home_joint8-30);
  joint4.write(home_joint4-30);
  joint6.write(home_joint6+30);
  delay(100);
  joint3.write(home_joint3);
  joint7.write(home_joint7);
  idle();
  
  joint5.write(home_joint5-30);
  joint9.write(home_joint9-30);
  delay(100);
  joint2.write(home_joint2);
  joint8.write(home_joint8);
  joint4.write(home_joint4);
  joint6.write(home_joint6);
  delay(100);
  joint5.write(home_joint5);
  joint9.write(home_joint9);
  idle();
  }
}
