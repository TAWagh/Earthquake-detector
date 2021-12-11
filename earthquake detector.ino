#include<Wire.h>
#define buzzer 7
const int mpu_addr=0x68;//I2C address of the MPU b1101000
int16_t raw_ax, raw_ay, raw_az;
double accX, accY, accZ,sx,sy,sz,x,y,z;
int samples = 50;
int itensity;
int d;// counter used in place of delay

void setup(){
  
  Wire.begin();
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);
  
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x1A); //  LPF
  Wire.write(0b00000011); 
  Wire.endTransmission(); 


  //Power Management Reg settings
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x6B); // sleep reg
  Wire.write(0b00000000); 
  Wire.endTransmission(); 

  //Configuring Acccelerometer
  Wire.beginTransmission(mpu_addr); 
  Wire.write(0x1C); 
  Wire.write(0b00001000); // +- 4g full scale reading
  Wire.endTransmission(); 


  delay(50);// beecause there is a sharp negative peak in the x value after restart for some reason

/// initial Values

  //sum
 for(int i=0;i<samples;i++){
    acc_data();
    sx= sx + accX;
    sy= sy + accY;
    sz= sz + accZ;
  }

  //avg
  sx=sx/samples;   
  sy=sy/samples;   
  sz=sz/samples;   

  Serial.print(sx);
  Serial.print(" ");
  Serial.print(sy);
  Serial.print(" ");
  Serial.println(sz);
  delay(1000);
  
}

void loop(){
  acc_data();


  x=accX-sx;
  y=accY-sy;
  z=accZ-sz;

  
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(z);

 if (accX>(sx+0.2)||accX<(sx-0.2)||accY>(sy+0.2)||accY<(sy-0.2)||accZ>(sz+0.2)||accZ<(sz-0.2) ){
      digitalWrite(buzzer, HIGH);               
      d=0;  
  }
   if (d==5)
   digitalWrite(buzzer, LOW);                     
   d++;


    x=abs(x);
    y=abs(y);
    z=abs(z);
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.println(z);
    
      if (x>0.014&&x<0.039||y>0.014&&y<0.039||z>0.014&&z<0.039)
      {
         Serial.println("............LIGHT............");
      }
      else if (x>0.039&&x<0.092||y>0.039&&y<0.092||z>0.039&&z<0.092)
      {
         Serial.println("............MODERATE............");
      }
      else if (x>0.092&&x<0.34||y>0.092&&y<0.34||z>0.092&&z<0.34)
      {
         Serial.println("............STRONG............");
      }
      else if (x>0.34&&x<1.24||y>0.34&&y<1.24||z>0.34&&z<1.24)
      {
         Serial.println("............SEVERE............");
      }
      else if (x>1.24||y>1.||z>1.24)
      {
         Serial.println("............EXTREME............");
      }
      else
      {
         Serial.println("............SAFE............");
      }
  
 delay(1);
}


void acc_data(){
  
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x3B); //Starting reg for Acclerometer
  Wire.endTransmission();
  Wire.requestFrom(mpu_addr,6); ////Requesting 3*2 registers
  while(Wire.available() < 6);
  raw_ax=Wire.read()<<8 | Wire.read();
  raw_ay=Wire.read()<<8 | Wire.read();
  raw_az=Wire.read()<<8 | Wire.read();


  
  // processed values (in gf)
  accX = raw_ax / 8192.0;
  accY = raw_ay / 8192.0; 
  accZ = raw_az / 8192.0;
}
