#include <LedControl.h>

int DIN = 11; 
int CS =  10;
int CLK = 13;
LedControl lc=LedControl(DIN,CLK,CS,0);
int mX = 0;
int mY = 0;
int fX = 0;
int fY = 0;
int eGen = 0;
int jocPornit = 0;
int vert = 0,oriz = 1;
int sarpe[255][2];
int lungimeSarpe;
byte afisaj[8] = {0,0,0,0,0,0,0,0};
int mat[8][8]={
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
};

void startDisplay(){
  lc.shutdown(0,false);
  lc.setIntensity(0,10);
  lc.clearDisplay(0);
}


void printByte(byte character[])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}

int joyInput(int pin){
  if(analogRead(pin) > 700){
    return -1;
  }else if(analogRead(pin) < 300){
    return 1;
  }else{
    return 0;
  }
}



void setup(){
  startDisplay();
  Serial.begin(9600);
  // Pin 3 este butonul de start
  pinMode(3, INPUT);
  sarpe[0][0] = 4; sarpe[0][1] = 4;
  sarpe[1][0] = 4; sarpe[1][1] = 5;
  lungimeSarpe = 2;
  eGen = 0;
  randomSeed(3);
}

void loop(){
  if(jocPornit == 1 || digitalRead(3)){
    jocPornit = 1;
    byte afisaj[8] = {0,0,0,0,0,0,0,0};
    //printByte(afisaj);
    mY = joyInput(A0);
    mX = -1 * joyInput(A1);
    // Generarea Fructului
    if(eGen == 0){
      fX = random(0,6);
      fY = random(0,7);
      while(mat[fY][fX] == 1){
        fX = random(0,7);
        fY = random(0,7);
      }
      mat[fY][fX] = 1;
      eGen = 1;
    }
    if(mat[fY][fX] == 0){
      mat[fY][fX] = 1;
    }
    Serial.println(fX);
    Serial.println(fY);

    // miscarea sarpelui
    for(int i = 0; i < lungimeSarpe-1; i++){
      sarpe[i][0] = sarpe[i+1][0];
      sarpe[i][1] = sarpe[i+1][1];
    }

    if((sarpe[lungimeSarpe-1][0] > 7 || sarpe[lungimeSarpe-1][0] < 0 ) || (sarpe[lungimeSarpe-1][1] > 7 || sarpe[lungimeSarpe-1][1] < 0)){
      jocPornit = 0;
      sarpe[0][0] = 4; sarpe[0][1] = 5;
      sarpe[1][0] = 4; sarpe[1][1] = 4;
      lungimeSarpe = 2;
      byte afisaj[8] = {0x18,0x24,0x20,0x18,0x04,0x24,0x18,0x00};
      printByte(afisaj);
    }else{
      if(vert != mY && vert != -1 * mY && mY != 0){
        vert = mY;
        oriz = 0;
      }
      if(oriz != mX && oriz != -1 * mX && mX != 0){
        oriz = mX;
        vert = 0;
      }
      sarpe[lungimeSarpe-1][0] += vert;
      sarpe[lungimeSarpe-1][1] += oriz;
    }
    // Cresterea Sarpelui
    if(sarpe[lungimeSarpe-1][0] == fY && sarpe[lungimeSarpe-1][1] == fX){
      sarpe[lungimeSarpe][0] =sarpe[lungimeSarpe-1][0] + vert;
      sarpe[lungimeSarpe][1] = sarpe[lungimeSarpe-1][1] + oriz;
      lungimeSarpe++;
      eGen = 0;
    }
    // Scrierea Sarpelui pe matrice 
    for(int i = 0; i < lungimeSarpe; i++){
      mat[sarpe[i][0]][sarpe[i][1]] = 1;
    }

    // Transforma matricea mat in lista pentru functia printByte
    for(int j = 0; j < 8; j++){
        for(int i = 0; i < 8; i++)
        {
            afisaj[j] += mat[j][map(i,0,7,7,0)] << i;
        }
    }
    if(jocPornit){
      printByte(afisaj);
    }
    for(int i = 0; i < lungimeSarpe; i++){
      mat[sarpe[i][0]][sarpe[i][1]] = 0;
    }

  }else{
    byte afisaj[8] = {0x18,0x24,0x20,0x18,0x04,0x24,0x18,0x00};
    printByte(afisaj);
  }
  // mat[a][b] = 1;
  // if(a == 8 && b != 8){
  //   a = 0;
  //   b++;
  // }else if(a != 8 && b != 8){
  //   a++;
  // }

  delay(350);
}