#include <LiquidCrystal.h>
#include <stdio.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


int tx1 = 0;
int rx1 = 1;

int tx2 = 8;
int rx2 = 9;


int button1  = 10;
int button2  = 11;

int buzzer  = 22;

 
unsigned char rcv,count,gchr='x';
 
char pastnumber[11];
 
char gpsval[50];

int i=0,k=0,lop=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";

unsigned char gv=0,msg1[10],msg2[11];
float lati=0,longi=0;
unsigned int lati1=0,longi1=0;
unsigned char flat[5],flong[5];
unsigned char finallat[10]="17.4210\0",finallong[10]="078.6419\0";
int ii=0,rchkr=0;
//17.4210,078.6419 

int sti=0;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void okcheck1()
{
  unsigned char rcr;
  do{
      rcr = Serial1.read();
    }while(rcr != 'K');
}
//17.6050421,78.4834139
//https://www.google.co.in/search?client=opera&q=17.6050421%2C78.4834139
void send_link(char *chtm)
 {
    delay(5000);delay(4000);
    Serial1.write("AT+CMGS=\"");
    Serial1.write(pastnumber);
    Serial1.write("\"\r\n");  delay(3000);
    for(;*chtm != '\0';chtm++)
       {
        Serial1.write(*chtm);  
       }
    Serial1.write(" https://www.google.co.in/search?client=opera&q=");
    for(ii=0;ii<=6;ii++){Serial1.write(finallat[ii]);}
    Serial1.write("%2C");
    for(ii=0;ii<=7;ii++){Serial1.write(finallong[ii]);}
    Serial1.write(0x1A);      delay(5000);delay(4000);
 }


 void beep()
 {
  digitalWrite(buzzer,HIGH);delay(1000);delay(1000);digitalWrite(buzzer,LOW);
 }
 
void setup() 
{
  Serial1.setRX(rx1);
  Serial1.setTX(tx1);
  Serial1.begin(1200);

  Serial2.setRX(rx2);
  Serial2.setTX(tx2);
  Serial2.begin(9600);
   
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW);

   
  lcd.begin(16, 2);lcd.cursor();
  lcd.print("An Advanced Embedded");
  delay(2000);
  
  /*
  get_gps();
  gps_convert();

    lcd.clear();
    lcd.setCursor(0,0);
     for(ii=0;ii<=6;ii++) lcd.write(finallat[ii]);
     
     lcd.setCursor(0,1);
     for(ii=0;ii<=7;ii++) lcd.write(finallong[ii]);    
  */
  delay(1500);
    gsminit();
  delay(1500); 
  lcd.clear();
}

void loop() 
{
  if(digitalRead(button1) == LOW)
    {
      lcd.setCursor(0,0);lcd.print("Need Help     ");
      beep();
        
      delay(5000);delay(4000);
     Serial1.write("AT+CMGS=\"");
     Serial1.write(pastnumber);
     Serial1.write("\"\r\n");  delay(3000);  
     Serial1.write("Need_Help@  https://www.google.co.in/search?client=opera&q=");
     for(ii=0;ii<=6;ii++){Serial1.write(finallat[ii]);}
     Serial1.write("%2C");
     for(ii=0;ii<=7;ii++){Serial1.write(finallong[ii]);}
     Serial1.write(0x1A);      delay(5000);delay(4000);  
    }
  if(digitalRead(button1) == HIGH)
    {
      lcd.setCursor(0,0);lcd.print("             ");
    }    

  if(digitalRead(button2) == LOW)
    {
      lcd.setCursor(0,1);lcd.print("Need Emergency ");
      beep();
        
      delay(5000);delay(4000);
     Serial1.write("AT+CMGS=\"");
     Serial1.write(pastnumber);
     Serial1.write("\"\r\n");  delay(3000);  
     Serial1.write("Need_Emergency@  https://www.google.co.in/search?client=opera&q=");
     for(ii=0;ii<=6;ii++){Serial1.write(finallat[ii]);}
     Serial1.write("%2C");
     for(ii=0;ii<=7;ii++){Serial1.write(finallong[ii]);}
     Serial1.write(0x1A);      delay(5000);delay(4000);  
    }
  if(digitalRead(button2) == HIGH)
    {
      lcd.setCursor(0,1);lcd.print("              ");
    }    
    
 while(Serial1.available() > 0)
      {
        char chrt = (char)Serial1.read();
        if(chrt == '*')
          {
            sti=1;
          }
        if(sti == 1)
          {
            inputString += chrt;
          }
        if(chrt == '#')
          { 
            sti=0;
            stringComplete = true;  
          }
      }

  if(stringComplete)
    {
      if(inputString[1] == 's' || inputString[1] == 'S')
        {
           send_link("Location-By_Request");   
        }
      sti=0;
      inputString = "";
      stringComplete = true;    
    }
}


int readSerial(char result[]) 
{
  int i = 0;
  while (1) 
  {
    while(Serial1.available() > 0) 
    {
      char inChar = Serial1.read();
      if (inChar == '\n') 
         {
          result[i] = '\0';
          Serial1.flush();
          return 0;
         }
      if (inChar != '\r') 
         {
          result[i] = inChar;
          i++;
         }
    }
  }
}

void gpsEvent()
{
  gpsString="";
  while(1)
  {
   //while (gps.available()>0)            //Serial incoming data from GPS

    while (Serial2.available() > 0)
   {
    //char inChar = (char)gps.read();
    char inChar = (char)Serial2.read();
     gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>60)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}

void get_gps()
{
  
  lcd.clear();
  lcd.print("Getting GPS Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait.....");
 
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    coordinate2dec();
    i=0;x=0;
    str_lenth=0;
   }
}

void coordinate2dec()
{
  String lat_degree="";
    for(i=17;i<=18;i++)         
      lat_degree+=gpsString[i];
      
  String lat_minut="";
     for(i=18;i<=19;i++)         
      lat_minut+=gpsString[i];
     for(i=21;i<=22;i++)         
      lat_minut+=gpsString[i];


       
  String log_degree="";
    for(i=29;i<=31;i++)
      log_degree+=gpsString[i];
  String log_minut="";
    for(i=32;i<=33;i++)
      log_minut+=gpsString[i];
    for(i=35;i<=36;i++)
      log_minut+=gpsString[i];

      
    Speed="";
    for(i=42;i<45;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
}

void gps_convert()
{
  if(gps_status)
  {
//  Serial.println(gpsString);

  if(gpsString[0] == '$' && gpsString[1] == 'G' && gpsString[2] == 'P' && gpsString[3] == 'R' && gpsString[4] == 'M' && gpsString[5] == 'C')
    {
     // Serial.println("Don11111111111111111111111111111111111111111111111111111\r\n");      
    //  Serial.write(gpsString[18]);Serial.write(gpsString[19]);Serial.write(gpsString[20]);Serial.write(gpsString[21]);Serial.write(gpsString[22]);
     //lcd.setCursor(0,0);
     for(ii=0;ii<9;ii++)
       {
        //lcd.write(gpsString[19+ii]);    
        msg1[ii] = gpsString[19+ii];
        //Serial.write(msg1[ii]);
       }
       //Serial.println("\r\n");
     //lcd.setCursor(0,1);
      for(ii=0;ii<10;ii++)
       {
        //lcd.write(gpsString[32+ii]);
        msg2[ii] = gpsString[32+ii];    
       // Serial.write(msg2[ii]);
       }

// Serial.println(msg1);
// Serial.println(msg2);

       //lati = (((msg1[2]-48)*100000) +((msg1[3]-48)*10000) + ((msg1[5]-48)*1000) + ((msg1[6]-48)*100) + ((msg1[7]-48)*10) + (msg1[8]-48)); 
       //longi = (((msg2[3]-48)*100000) + ((msg2[4]-48)*10000) + ((msg2[6]-48)*1000) + ((msg2[7]-48)*100) + ((msg2[8]-48)*10) + (msg2[9]-48));

       lati = (((msg1[2]-48)*1000) + ((msg1[3]-48)*100) + ((msg1[5]-48)*10) + (msg1[6]-48)); 
       longi = (((msg2[3]-48)*1000) + ((msg2[4]-48)*100) + ((msg2[6]-48)*10) + (msg2[7]-48));

      // converts(lati);Serial.write("-");
      // converts(longi);Serial.write("\r\n");
       
       lati = (lati/60);  longi = (longi/60);

       lati = (lati*100); longi = (longi*100);
       lati1 = lati;      longi1 = longi;
       
// Serial.write("After ");
  //      converts(lati1);Serial.write("-");
    //   converts(longi1);Serial.write("\r\n");
 

              convlat(lati); convlong(longi);
        finallat[0] = msg1[0];
        finallat[1] = msg1[1];
        finallat[2] = '.';
        finallat[3] = flat[0]; finallat[4] = flat[1];finallat[5] = flat[2];finallat[6] = flat[3];finallat[7] = '\0';


        finallong[0] = msg2[0];
        finallong[1] = msg2[1];
        finallong[2] = msg2[2];
        finallong[3] = '.';
        finallong[4] = flong[0];finallong[5] = flong[1];finallong[6] = flong[2];finallong[7] = flong[3];finallong[8] = '\0'; 

   
    }
  }  
}

 void convlat(unsigned int value)  
      {
             unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;              
      c=c|0x30;
      e=e|0x30; 
    g=g|0x30;              
      h=h|0x30;
   
  // dlcd(a);
//   dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    
         
               flat[0] = c;
               flat[1] = e;
               flat[2] = g;
             flat[3] = h;


          }

 void convlong(unsigned int value)  
      {
            unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;              
      c=c|0x30;
      e=e|0x30; 
    g=g|0x30;              
      h=h|0x30;
   
  // dlcd(a);
//   dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    
         
               flong[0] = c;
               flong[1] = e;
               flong[2] = g;
             flong[3] = h;


          }

void gsminit()
{
  Serial1.write("AT\r\n");                   okcheck1();
  Serial1.write("ATE0\r\n");                 okcheck1();
  Serial1.write("AT+CMGF=1\r\n");            okcheck1();
  Serial1.write("AT+CNMI=1,2,0,0\r\n");      okcheck1();
  Serial1.write("AT+CSMP=17,167,0,0\r\n");   okcheck1();
    
  lcd.clear();
  lcd.print("SEND MSG STORE");
  lcd.setCursor(0,1);
  lcd.print("MOBILE NUMBER");  
  do{
     rcv = Serial1.read();
    }while(rcv != '*');
     readSerial(pastnumber);pastnumber[10] = '\0';
     
     /*
  pastnumber1[0] = pastnumber[0];pastnumber1[1] = pastnumber[1];pastnumber1[2] = pastnumber[2];pastnumber1[3] = pastnumber[3];pastnumber1[4] = pastnumber[4];pastnumber1[5] = pastnumber[5];
  pastnumber1[6] = pastnumber[6];pastnumber1[7] = pastnumber[7];pastnumber1[8] = pastnumber[8];pastnumber1[9] = pastnumber[9];pastnumber1[10] = '\0';
    */
/*
  pastnumber3[0] = pastnumber[20];pastnumber3[1] = pastnumber[21];pastnumber3[2] = pastnumber[22];pastnumber3[3] = pastnumber[23];pastnumber3[4] = pastnumber[24];pastnumber3[5] = pastnumber[25];
  pastnumber3[6] = pastnumber[26];pastnumber3[7] = pastnumber[27];pastnumber3[8] = pastnumber[28];pastnumber3[9] = pastnumber[29];pastnumber3[10] = '\0';
*/
  lcd.clear();
  lcd.print(pastnumber);
  
    delay(4000);  delay(5000);  
    Serial1.write("AT+CMGS=\"");
    Serial1.write(pastnumber);
    Serial1.write("\"\r\n"); delay(3000);
    Serial1.write("Reg\r\n");
    Serial1.write(0x1A);   
    delay(4000);  delay(5000);  
}

void converts(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   Serial1.write(a);
   Serial1.write(c);
   Serial1.write(e); 
   Serial1.write(g);
   Serial1.write(h);
}

void convertl(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
     
   //lcd.write(a);
   //lcd.write(c);
   lcd.write(e); 
   lcd.write(g);
   lcd.write(h);
}
