#include <SPI.h>


char buff [50];
volatile byte indx;
volatile boolean process;
int m1 = 5;  
int m2 = 4;    
int multitrun = A1;
int last_error=0; 
int error=0;
int max_error=100;
int tmbl_1=A7; //pin tombol 1
float I=0;
float D=0;
float ki=0.01; //intragral masukan
float kd=4; //deferent masukan
float kp=8; //proposa masukan
float output=0;
int sensor;
int input=512;

void setup (void) {
   Serial.begin (115200);
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   indx = 0; // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt

   Serial.begin(9600);
   pinMode(tmbl_1,INPUT_PULLUP);
   pinMode(6, OUTPUT);
   pinMode(4, OUTPUT);
   pinMode(5, OUTPUT);
   digitalWrite(tmbl_1,HIGH);
}
byte c;

int  data;
ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   c = SPDR; // read byte from SPI Data Register
   data=int (c);
   
   if (indx < sizeof buff) {
      buff [indx++] = c; // save data in the next index in the array buff
      if (c == '\r')  process = true;
      
   }
   
}

void loop (void) {
 //Serial.println(data);
   if (process) {
     process = false; //reset the process
      //Serial.println(buff);
      if (buff[0]=='1'){                      //1
         Serial.println("buang");
         input = 512+75;
      }
      if (buff[0]=='2'){
         input = 512+1;       
         Serial.println("Kembali");
      }
      if (buff[0]=='3'){                      //2
         input = 512-70*2;
         Serial.println("buang");
      }
      if (buff[0]=='4'){
         input = 512;          
         Serial.println("Kembali");
      }
      if (buff[0]=='5'){                      //3
         input = 512+70*2;
         Serial.println("buang");
      }
      if (buff[0]=='6'){
         input = 512+3*2;    
         Serial.println("Kembali");
      }
      if (buff[0]=='7'){                      //4
         input = 512-80*2;
         Serial.println("buang");
      }
      if (buff[0]=='8'){
         input = 512;          
         Serial.println("Kembali");
      }
      if (buff[0]=='9'){                      //5
         input = 512+70*3;
         Serial.println("buang");
      }
      if (buff[0]=='A'){                 
         input = 512+3*2;     
         Serial.println("Kembali");
      }
      if (buff[0]=='B'){                      //6
         input = 512-80*3;                
         Serial.println("buang");
      }
      if (buff[0]=='C'){
         input = 512;                     
         Serial.println("Kembali");
      }
      if (buff[0]=='D'){                      //7
         input = 512+70*4;                
         Serial.println("buang");
      }
      if (buff[0]=='E'){
         input = 512;
         Serial.println("Kembali");
      }
         indx=0;
   }

//Serial.println(input);
sensor = analogRead(A1);
Serial.println(sensor);
  last_error=error;
  error= input-sensor;
  if (error>max_error)error=max_error;
  if (error<-max_error)error=-max_error;
  if (abs(error) < max_error){
    I= I+error;
  }
  else {
    I=0;
  }

  D = error - last_error;
  output= (error * kp  + D*kd ); //rumus pid 

  if (sensor<=100)input=512;
  if (sensor>=1100)input=512;
  if (output>255)output=255;
  if (output<-255)output=-255;
  
  analogWrite(6,abs(output));

if (output<=0){
     analogWrite(4,0 );           // Write the output to the output pin
     analogWrite(5,abs( output));
  }
  else{
     analogWrite(4,abs( output)); // Write the output to the output pin
     analogWrite(5,0);
  }
}
