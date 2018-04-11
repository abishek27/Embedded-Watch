#include<avr/io.h>
#include<stdlib.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<LiquidCrystal.h>

LiquidCrystal lcd (13,12,7,6,5,4);

unsigned long timer=0; 
unsigned long sec=0;
unsigned long mint =0;
unsigned long hour = 0;
unsigned long snd = 0;
unsigned long stopwatchtimer=0;


 int mode = 0;
 int mincrement = 0; 
 int select = 0;
 int stopwatch =0;
 int ah=-2;
 int am = -2;
 int stopwatchmode =0 ;
 int timermode = 0;
 int alarmmode=0;
 int ssr=0;
 int start = 0;
 int alarmscroll = 0;
 int salarm = 0;
 int sah=0;
 int sam = 0;
 int tah=0;
 int tam=0;
 int talarm=0;
 int alm=0;

void timer_initialize()
{
  TCCR1B |= (1<<CS12)|(1<<CS10)|(1<<WGM12);
  TIMSK1 |= (1<<OCIE1A);
  OCR1A = 15625;
  TCNT1 = 0;
   mode = 0;
   mincrement = 0;
   select =0;
   stopwatch = 0;
}
void pwm()
{
  TCCR2A |=(1<<WGM20)|(1<<COM2A1)|(1<<WGM21);
  TCCR2B |=(1<<CS20);
  OCR2A = 0;
}



unsigned int a=0; 
void InitADC()
 { ADMUX=(1<<REFS0); 
 ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 
 Serial.begin(9600);
}
unsigned int ReadADC(byte ch) 
{ ch=ch&0b00000111; 
ADMUX|=ch; 
ADCSRA|=(1<<ADSC); 
while(!(ADCSRA & (1<<ADIF))); 
ADCSRA|=(1<<ADIF); return (ADC); 
}
void intr()
{
  EICRA |= (1<<ISC11)|(1<<ISC01);
  EIMSK |= (1<<INT0)|(1<<INT1);
  PCICR |= (1<<PCIE1);
  PCMSK1 |= (1<<PCINT13);
  
  sei();
}
ISR (TIMER1_COMPA_vect)
{
  timer++;
  sec=timer; 
   snd = sec%60;
   mint = sec/60;
   hour = mint/60;
   mint = mint%60;
   hour = hour%60;
   if(salarm==2)
   if(hour == sah)
   if(mint== sam)
   {TIMSK1 |= (0<<OCIE1A);
   DDRB|=(1<<PB3);
   lcd.clear();
   lcd.setCursor(5,1);
   lcd.print("Alarm!!!");
   for(int j = 0;j<6;j++)
   {for(int i =0; i<255;i = i+5) 
        
        {OCR2A = i;
        _delay_ms(100);
        }

        for(int i =255; i>=0;i = i-5) 
        
        {OCR2A = i;
        _delay_ms(100);
        }
   }
   timer+=60;
   TIMSK1 |= (1<<OCIE1A);
   DDRB &= ~(1<<PB3);
   PORTB &= ~(1<<PB3);
   }
   
  
   
   
 
    if(mode==0)
    {
      stopwatchmode =0;
      alarmmode= 0;
      start = 0;
      stopwatch = 0;
      timermode = 0;
      alarmscroll = 0;
      ah = -2;
      am = -2;
      lcd.clear();
   a= ReadADC(0);
   
   a=(5.0 * a * 100.0) / 1024;
   
   lcd.setCursor(0,0);
   lcd.print(a);
   lcd.setCursor(7,0);
   lcd.print("15/07/17");
   lcd.setCursor(5,1);
  
   if(hour<10)
   {
    lcd.print("0");
    lcd.print(hour);
   }
   else
   {
    lcd.print(hour);
   }
   lcd.print(":");
   if(mint<10)
   {
    lcd.print("0");
    lcd.print(mint);
   }
   else 
   {
    lcd.print(mint);
   }
   lcd.print(":");
   if(snd<10)
   {
    lcd.print("0");
    lcd.print(snd);
   }
   else
   {
    lcd.print(snd);
   }
  }
  if(mode == 1 & mincrement == 0 & stopwatchmode == 1 & stopwatch ==1)
  {
    stopwatchtimer++;
     lcd.clear();
   lcd.setCursor(5,1);
   snd = stopwatchtimer%60;
   mint = stopwatchtimer/60;
   hour = mint/60;
   if(hour<10)
   {
    lcd.print("0");
    lcd.print(hour);
   }
   else
   {
    lcd.print(hour);
   }
   lcd.print(":");
   if(mint<10)
   {
    lcd.print("0");
    lcd.print(mint);
   }
   else 
   {
    lcd.print(mint);
   }
   lcd.print(":");
   if(snd<10)
   {
    lcd.print("0");
    lcd.print(snd);
   }
   else
   {
    lcd.print(snd);
   }
  }
 
}

ISR(INT1_vect)
{
  mode++;
  Serial.println(mode);
  if(mode==1)
  {
    lcd.clear();
    lcd.setCursor(5,1);
    lcd.print("Mode");
    mincrement =0;
  }
  if(mode>=2)
  mode=0;
}
ISR(INT0_vect)
{
  if(mode == 1 & stopwatchmode==0 & alarmmode==0 & timermode ==0 )
  {
    mincrement++;
    if(mincrement==1)
    {
      lcd.clear();
    lcd.setCursor(5,1);
    lcd.print("Mode");
    }
    if(mincrement == 2)
    {
       lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Alarm & Reset");
    
    }
    if(mincrement == 3)
    {
       lcd.clear();
    lcd.setCursor(5,1);
    lcd.print("Stopwatch");
    mincrement =0;
    stopwatchmode =0;
    }
  }
  if(mode==1 & stopwatchmode ==1)
  {
    start++;
    if(start ==1)
    {
      stopwatch=1;
    }
    if(start==2)
    {
      stopwatch = 0;
      start =0;
    }
  }
  if(alarmmode == 1 & mode ==1)
  {
    alarmscroll++;
    if(alarmscroll==1)
    {
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Set Hour");
      
    }
    if(alarmscroll==2)
    {
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Set Min");
      
    }
    if(alarmscroll==3)
    {
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Alarm Mode");
      
    }
    if(alarmscroll==4)
    {
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Reset");
    }
    if(alarmscroll >= 5)
    alarmscroll =0;
  }
 
}


ISR(PCINT1_vect)

{ 
  if(mode==1 & mincrement ==0)
  {
    stopwatchmode =1;
    lcd.clear();
    lcd.setCursor(5,1);
    lcd.print("00:00:00");
    stopwatchtimer =0;
  }

  if(mode==1 & mincrement == 2 & alarmmode == 0)
  { alarmmode = 1;
   
  }
  if(mode == 1 & mincrement == 2 & alarmmode == 1 & alarmscroll == 1)
  { ah++;
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Set Hour");
    lcd.setCursor(5,1);
    ah = ah%48;
    lcd.print(ah/2);
  }
  if(mode == 1 & mincrement ==2 & alarmmode ==1 & alarmscroll ==2)
  {
    am++;
      lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Set Min");
    lcd.setCursor(5,1);
    am = am%120;
    lcd.print(am/2);
  }
  if(mode == 1 & mincrement == 2 & alarmmode ==1 & alarmscroll == 3)
  {
    salarm++;
    if(salarm == 2)
    {
     
      lcd.setCursor(5,1);
      lcd.print("On ");
      sah = ah/2;
      sam = am/2;
      Serial.println(sah);
      Serial.println(sam);
      Serial.println(salarm);
     
    }
    if(salarm >= 3)
    {
      lcd.setCursor(5,1);
      lcd.print("Off");
      salarm=0;
    }
  }
  if(mode==1 & mincrement == 2 & alarmmode == 1 & alarmscroll == 4)
  {
    talarm++;
    if(talarm == 2)
    {
      lcd.setCursor(5,1);
      lcd.print("Confirm");
      tah = ah/2;
      tam = am/2;
      timer = tah*3600 + tam*60;
    }
    if(talarm>=3)
    {
      talarm=0;
    }
  }
 
}
int main()
{
  intr();
  sei();
  lcd.begin(16,2);
  lcd.clear();
  pwm();
  InitADC();
  timer_initialize();
  Serial.begin(9600);
  while(1)
  {
    
  }
}

