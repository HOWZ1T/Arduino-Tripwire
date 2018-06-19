#define MAX4446OUT A0

#define MAX4424IN 3

#define MAX7219DIN 4
#define MAX7219CS 5
#define MAX7219CLK 6

void setup() {
  MAX7219init();
  MAX7219brightness(15);

  MAX4446init();
  MAX4424init();
  
  Serial.begin(9600);
}

void loop() {
 int lux = measureLight();
 Serial.println(lux);

 if(lux <= 918)
 {
  sendnum(1);
  makenoise(true);
  delay(2000);
 }
 else
 {
  sendnum(0);
  makenoise(false);
 }
 delay(300);
}

void MAX4424init()
{
  pinMode(MAX4424IN, OUTPUT);
}

void makenoise(bool onoff)
{
  if(onoff == true)
  {
    tone(MAX4424IN, 1000);
  }
  else
  {
   noTone(MAX4424IN); 
  }
}

void MAX4446init()
{
  pinMode(MAX4446OUT, INPUT);
}

int sampleLight()
{
  int sum = 0;
  int t_delay = 10; // delay in ms
  int sample_size = 10; // 10 * 10 = 100 ms
  int sample_count = 0;
  
  while(sample_count < sample_size)
  {
    sum += analogRead(MAX4446OUT);
    sample_count++;
    delay(t_delay);
  }
  
  return sum;
}

int measureLight()
{
  return analogRead(MAX4446OUT);
}

void MAX7219brightness(byte b){  //0-15 is range high nybble is ignored
  MAX7219senddata(10,b);        //intensity  
}

void MAX7219init(){
  pinMode(MAX7219DIN,OUTPUT);
  pinMode(MAX7219CS,OUTPUT);
  pinMode(MAX7219CLK,OUTPUT);
  digitalWrite(MAX7219CS,HIGH);   //CS off
  digitalWrite(MAX7219CLK,LOW);   //CLK low
  MAX7219senddata(15,0);        //test mode off
  MAX7219senddata(12,1);        //display on
  MAX7219senddata(9,255);       //decode all digits
  MAX7219senddata(11,7);        //scan all
  for(int i=1;i<9;i++){
    MAX7219senddata(i,0);       //blank all
  }
}

void MAX7219senddata(byte reg, byte data){
  digitalWrite(MAX7219CS,LOW);   //CS on
  for(int i=128;i>0;i=i>>1){
    if(i&reg){
      digitalWrite(MAX7219DIN,HIGH);
    }else{
      digitalWrite(MAX7219DIN,LOW);      
    }
  digitalWrite(MAX7219CLK,HIGH);   
  digitalWrite(MAX7219CLK,LOW);   //CLK toggle    
  }
  for(int i=128;i>0;i=i>>1){
    if(i&data){
      digitalWrite(MAX7219DIN,HIGH);
    }else{
      digitalWrite(MAX7219DIN,LOW);      
    }
  digitalWrite(MAX7219CLK,HIGH);   
  digitalWrite(MAX7219CLK,LOW);   //CLK toggle    
  }
  digitalWrite(MAX7219CS,HIGH);   //CS off
}

void blank()
{
  for(int i=1;i<9;i++){
    MAX7219senddata(i,0);       //blank all
  }
}

void sendnum(long data)
{
  blank();
  int digitCount = 1;
  for(long i = 1; i <= 10000000; i = i*10)
  {
    if(data >= i)
    {
      long digit = getmod(i, data);
      MAX7219senddata(digitCount++, digit);
    }
    else
    {
      break;
    }
  }
}

long getmod(long mod, long data)
{
  long val = (data/mod)%10;
  return val;
}

