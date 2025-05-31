#include <RobotixInnoBot.h>
#include <EEPROM.h>


const int EEPROM_START_ADDRESS = 0;

// Function to write a char array to EEPROM
void writeCharArrayToEEPROM(int startAddress, char* data) {
  int i = 0;
  while (data[i] != '\0') { // Write until null terminator
    EEPROM.write(startAddress + i, data[i]);
    i++;
  }
  EEPROM.write(startAddress + i, '\0'); // Write the null terminator
}

// Function to read a char array from EEPROM
void readCharArrayFromEEPROM(int startAddress, char* buffer, int bufferSize) {
  int i = 0;
  while (i < bufferSize - 1) { // Ensure there's space for null terminator
    char c = EEPROM.read(startAddress + i);
    buffer[i] = c;
    if (c == '\0') break; // Stop at null terminator
    i++;
  }
  buffer[i] = '\0'; // Add null terminator in case it wasn't read
}

#define PORT7 bot.port_7

RobotixInnoBot mega;
uint8_t irRegister[4];

int pathlength; //variable to record the total of path length
int  readpath; //variable to call the path record
char path[99]; //array for path  record
int state=0;
 
#define Speed 150

#define SR9  ((irRegister[0] >> 4) & 1)
#define SR8  ((irRegister[0] >> 3) & 1)
#define SR7  ((irRegister[0] >> 2) & 1)
#define SR6  ((irRegister[0] >> 1) & 1)
#define SR5  ((irRegister[0] >> 0) & 1)
#define SR4  ((irRegister[1] >> 6) & 1)
#define SR3  ((irRegister[1] >> 5) & 1)
#define SR2  ((irRegister[1] >> 4) & 1)
#define SR1  ((irRegister[1] >> 3) & 1)
#define SM   ((irRegister[1] >> 2) & 1)
#define SL1  ((irRegister[1] >> 1) & 1)
#define SL2  ((irRegister[1] >> 0) & 1)
#define SL3  ((irRegister[2] >> 6) & 1)
#define SL4  ((irRegister[2] >> 5) & 1)
#define SL5  ((irRegister[2] >> 4) & 1)
#define SL6  ((irRegister[2] >> 3) & 1)
#define SL7  ((irRegister[2] >> 2) & 1)
#define SL8  ((irRegister[2] >> 1) & 1)
#define SL9  ((irRegister[2] >> 0) & 1)

void readsensor(){
  bot.rjIrArrayRead(PORT7, irRegister);
  delay(10);
}

void lilmoveforward()
{
  bot.speed(Speed, Speed);
  delay(100);  // adjust based on your batteries power, new one would be stronger than the drained-out  one, of course
  
}

void followline()
{
  readsensor();
if(SM)  bot.speed(Speed, Speed);
  else if(SR1)  bot.speed(Speed, Speed/2);
  else if(SR2)  bot.speed(Speed, 0);
  else if(SR3)  bot.speed(Speed, -Speed/2);
  else if(SR4)  bot.speed(Speed, -Speed);
  else if(SR5)  bot.speed(Speed, -Speed);
  else if(SR6)  bot.speed(Speed, -Speed);
  else if(SR7)  bot.speed(Speed, -Speed);
  else if(SR8)  bot.speed(Speed, -Speed);
  else if(SR9)  bot.speed(Speed, -Speed);
  else if(SL1)  bot.speed(Speed/2, Speed);
  else if(SL2)  bot.speed(0, Speed);
  else if(SL3)  bot.speed(-Speed/2, Speed);
  else if(SL4)  bot.speed(-Speed, Speed);
  else if(SL5)  bot.speed(-Speed, Speed);
  else if(SL6)  bot.speed(-Speed, Speed);
  else if(SL7)  bot.speed(-Speed, Speed);
  else if(SL8)  bot.speed(-Speed, Speed);
  else if(SL9)  bot.speed(-Speed, Speed);

  else if(!SM && !SR1 && !SR2 && !SR3 && !SR4 && !SR5 && !SR6 && !SR7 && !SR8 && !SR9 && !SL1 && !SL2 && !SL3 && !SL4 && !SL5 && !SL6 && !SL7 && !SL8 && !SL9){
    bot.speed(0, 0);
    delay(200);
    readsensor();
     if(!SM && !SR1 && !SR2 && !SR3 && !SR4 && !SR5 && !SR6 && !SR7 && !SR8 && !SR9 && !SL1 && !SL2 && !SL3 && !SL4 && !SL5 && !SL6 && !SL7 && !SL8 && !SL9){
    bot.speed(-Speed, Speed);
    delay(600);
    bot.speed(0, 0);
    delay(300);
    path[pathlength]='U';pathlength++;//save  U
    // ONforYELLOWled();
    }
  }

  if(SM && SR1 && SR2 && SR3 && SL1 && SL2 && SL3){
    bot.speed(0, 0);
    delay(500);
    lilmoveforward();
    bot.speed(0, 0);
    delay(200);
    readsensor();
    if(SM && SR1 && SR2 && SR3 && SL1 && SL2 && SL3){
      bot.speed(0, 0);
      delay(200);
      path[pathlength]='F';pathlength++;//save  F

      writeCharArrayToEEPROM(0,path);
      state = 1;
      EEPROM.write(150, state);

       bot.drawString(25, 10, "Playing:", FONT_SMALL);


      bot.speed(0, 0);
      delay(3000);
      
      shortpath(); //calculate the shortest path

      
      bot.playSong(NOKIA);
      
      shortestpath();  //move through the shortest path
    }

    else //ada garis  ke arah lurus 00100 dll (+ intersection)
    {
      lilmoveforward();  //turning stabilizer
      bot.speed(Speed, -Speed);
      delay(300);
      readsensor();
      path[pathlength]='R';pathlength++;//save  R
    }
    
  }

  else if((SM || SL1 || SR1) && SR7 &&(!SR3 && !SR4 && !SR5 && !SR6 && !SR8 && !SR9)){
    bot.speed(0, 0);
    delay(200);
    // readsensor();
    // if((SM || SL1 || SR1) && SR7){
    bot.speed(Speed, -Speed);
    delay(300);
    readsensor();
    path[pathlength]='R';pathlength++;//save  R
    // }

  }

    else if((SM || SL1 || SR1) && SL7 &&(!SL3 && !SL4 && !SL5 && !SL6 && !SL8 && !SL9)){
      
    bot.speed(0, 0);
    delay(200);
    // readsensor();
    // if((SM || SL1 || SR1) && SL7){
    bot.speed(Speed, Speed);
    delay(200);
    readsensor();
    path[pathlength]='S';pathlength++;//save  S
    // }
  }


}


void  shortpath() //calculate the shortest path
{
  //because (..F) is the last  and there is no U recorderd before F 
  int x = (pathlength-2);

  while  (x > 0)  
  {
    if (path[x]=='U')
      {
        if (path[x-1]=='L'  && path[x+1]=='L')  
          {path[x-1]='S';path[x]='O';path[x+1]='O';}
        else  if (path[x-1]=='L' && path[x+1]=='S')
          {path[x-1]='R';path[x]='O';path[x+1]='O';}
        else if (path[x-1]=='R' && path[x+1]=='R')
          {path[x-1]='S';path[x]='O';path[x+1]='O';}
        else if (path[x-1]=='R' && path[x+1]=='S')
          {path[x-1]='L';path[x]='O';path[x+1]='O';}
        else if (path[x-1]=='S' && path[x+1]=='L')
          {path[x-1]='R';path[x]='O';path[x+1]='O';}
        else if (path[x-1]=='S' && path[x+1]=='R')
          {path[x-1]='L';path[x]='O';path[x+1]='O';}
        else if (path[x-1]=='L' && path[x+1]=='R')
          {
            path[x-1]='U';path[x]='O';path[x+1]='O';
            
          }
        else if (path[x-1]=='R' && path[x+1]=='L')
          {
            path[x-1]='U';path[x]='O';path[x+1]='O';
            
          }
        else if (path[x-1]=='S' && path[x+1]=='S')
          {
            path[x-1]='U';path[x]='O';path[x+1]='O';
            
          }
        //---------------------------------------
        x--;
      }
    else
    	{x--;}
  }
}







void shortestpath()
{
 readsensor();
  if(SM)  bot.speed(Speed, Speed);
  else if(SR1)  bot.speed(Speed, Speed/2);
  else if(SR2)  bot.speed(Speed, 0);
  else if(SR3)  bot.speed(Speed, -Speed/2);
  else if(SR4)  bot.speed(Speed, -Speed);
  else if(SR5)  bot.speed(Speed, -Speed);
  else if(SR6)  bot.speed(Speed, -Speed);
  else if(SR7)  bot.speed(Speed, -Speed);
  else if(SR8)  bot.speed(Speed, -Speed);
  else if(SR9)  bot.speed(Speed, -Speed);
  else if(SL1)  bot.speed(Speed/2, Speed);
  else if(SL2)  bot.speed(0, Speed);
  else if(SL3)  bot.speed(-Speed/2, Speed);
  else if(SL4)  bot.speed(-Speed, Speed);
  else if(SL5)  bot.speed(-Speed, Speed);
  else if(SL6)  bot.speed(-Speed, Speed);
  else if(SL7)  bot.speed(-Speed, Speed);
  else if(SL8)  bot.speed(-Speed, Speed);
  else if(SL9)  bot.speed(-Speed, Speed);

 
     //right or straight
    //  if ((data==0b0000111)||(data==0b0001111))
    //   {
    //     lilmoveforward();
    //     if (data==0b0000000)//right only
    //       {
    //         lilmoveforward();
    //         righttillstraight();
    //       }
    //     else //there is a straight path
    //       {
    //         choosepath();
    //       }
    //   }

   if((SM || SL1 || SR1) && SR7){
    bot.speed(0, 0);
    delay(500);
    readsensor();
    // if((SM || SL1 || SR1) && SR7){
    choosepath();
    // }
  }

    //left or straight
    // else if ((data==0b0011100)||(data==0b0011110))
    //   {
    //     lilmoveforward();
    //     if (data==0b0000000) //left only
    //       {
    //         lilmoveforward();
    //         lefttillstraight();
    //       }
    //     else //there is a straight path
    //       {
    //         choosepath();
    //       }
    //   }

  else if((SM || SL1 || SR1) && SL7){
    bot.speed(0, 0);
    delay(500);
    readsensor();
    // if((SM || SL1 || SR1) && SL7){
     choosepath();
    // }

  }

    //T, +, or END OF MAZE
    else if(SM && SR1 && SR2 && SR3 && SL1 && SL2 && SL3 && SR4 && SL4){
      bot.speed(0, 0);
    delay(500);
    readsensor();
        choosepath();
      }
  shortestpath();


}


void  choosepath()//to get rid of the effect of “path[]==0” in the record
{
  if  (path[readpath]=='F')
    { 
      bot.speed(0, 0);
      delay(3000);
      finish();
    }
  else if  (path[readpath]=='R')
    {
      // ONforGREENled();
      // righttillstraight();
      lilmoveforward();
      bot.speed(Speed, -Speed);
      delay(300);
    }
  else if (path[readpath]=='S')
    {
      // ONforGREENled();ONforBLUEled();
      // moveforward();delay(200);
      lilmoveforward();
      bot.speed(Speed, Speed);
      delay(200);
    }
  else if (path[readpath]=='L')
    {
      // ONforGREENled();ONforYELLOWled();
      // lefttillstraight();
      lilmoveforward();
      bot.speed(-Speed, Speed);
      delay(300);
    }
  else if (path[readpath]=='O')
    {
      readpath++;choosepath();
    } 
  readpath++;
  shortestpath();
}


void finish()
{
  // ONforBLUEled();delay(100);
  // ONforYELLOWled();delay(100);
  // ONforGREENled();delay(100);
  
  state=0;
  EEPROM.write(150, state);
  bot.playSong(NOKIA);
  
  finish();
  

}


int state_EE=0;

void setup() {
  // put your setup code here, to run once:
  bot.begin();
  bot.rjIrArrayInit(PORT7);
  Serial.begin(115200);
  Serial.println("Hello World");

  state_EE = EEPROM.read(150);
  if(state_EE!=1){state_EE=0;}

}

void loop() {
  // put your main code here, to run repeatedly:
  if(state_EE==0){
    Serial.println("follow line");
    followline();
  }
  else if(state_EE==1){
    Serial.println("find short path");
    shortpath(); //calculate the shortest path
    shortestpath();  //move through the shortest path
  }
  
  // delay(100);

}
