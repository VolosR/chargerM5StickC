#include "M5StickCPlus.h"
#include "fonts.h"
TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

RTC_TimeTypeDef TimeStruct;

float vbat,vin,temp;
int charge;
String hr,mi,se;
int deb=0; int deb2=0;

#define color 0x01EA
#define color2 ORANGE

String cc[4]={"100 mAh","280 mAh","450 mAh","700 mAh"};
byte values[4]={0,2,4,7};
byte chosen=1;

/*
#define CURRENT_100MA (0b0000)
#define CURRENT_190MA (0b0001)
#define CURRENT_280MA (0b0010)
#define CURRENT_360MA (0b0011)
#define CURRENT_450MA (0b0100)
#define CURRENT_550MA (0b0101)
#define CURRENT_630MA (0b0110)
#define CURRENT_700MA (0b0111)
*/

byte brightness[5] ={20,40,60,80,90};
byte chosenB=1;

void setup(){
  pinMode(37,INPUT_PULLUP);
  pinMode(39,INPUT_PULLUP);
  M5.begin();
  spr.createSprite(135,240);
  TimeStruct.Hours   = 00;
  TimeStruct.Minutes = 00;
  TimeStruct.Seconds = 00;
  M5.Rtc.SetTime(&TimeStruct);

 M5.Axp.ScreenBreath(brightness[chosenB]);
 M5.Axp.SetChargeCurrent(values[chosen]);
}

void draw()
{
  spr.fillSprite(TFT_BLACK);
 
  for(int i=0;i<5;i++)
  spr.fillRect(100+(i*6),10,4,12,color);

  for(int i=0;i<chosenB+1;i++)
  spr.fillRect(100+(i*6),10,4,12,color2);

  spr.setTextDatum(0);
  spr.setTextColor(WHITE,0x0083);
  spr.drawString("BATTERY",10,8,2);
  spr.drawString("SET CURRENT     ",10,124,2);
  spr.setTextColor(WHITE,BLACK);
  spr.setFreeFont(&DSEG7_Classic_Bold_32);
  spr.drawString(String(vbat),10,30);
  spr.drawString("V",100,30,2);


  spr.setFreeFont(&DSEG7_Classic_Bold_17);
  spr.drawString(hr+":"+mi,10,216);
  spr.setFreeFont(&DSEG7_Classic_Bold_12);
  spr.drawString(se,76,216);
  
  for(int i=0;i<4;i++)
  spr.drawString(cc[i],70,142+(i*15),2);
  
  spr.fillCircle(62,150+(chosen*15),4,TFT_RED);
  
  spr.drawFastHLine(10, 144, 40, color2);
  spr.drawFastVLine(50, 144, 64, color2);
  spr.drawFastVLine(51, 144, 64, color2);
  spr.drawFastHLine(50, 208, 74, color2);

   spr.setTextFont(0);
   spr.setTextColor(0x4C77,BLACK);
   spr.drawString("TIME:",12,200);
   
   spr.drawString("TEMP:",12,150);
   spr.drawString(String(temp),12,160);
  
  spr.setTextDatum(4);
  spr.fillRoundRect(10, 72, 55,46,4,color);
  spr.fillRoundRect(70, 72, 55,46,4,color);
  
  spr.setTextColor(WHITE,color);
  spr.drawString("USB V",37,82,2);
  spr.drawString("CHR I",97,82,2);
  spr.setFreeFont(&DSEG7_Classic_Bold_17);
  spr.drawString(String(vin),37,102);
  spr.drawString(String(charge),97,102);

  spr.pushSprite(0,0); 
}

void loop() {

   if(digitalRead(37)==0)
   {
   if(deb==0) {deb=1; chosen++; if(chosen==4) chosen=0;  M5.Axp.SetChargeCurrent(values[chosen]);}
   }else deb=0;

   if(digitalRead(39)==0)
   {
   if(deb2==0) {deb2=1; chosenB++; if(chosenB==5) chosenB=0;  M5.Axp.ScreenBreath(brightness[chosenB]);}
   }else deb2=0;

   vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
   charge = M5.Axp.GetIchargeData() / 2;
   vin=M5.Axp.GetVBusVoltage();
   temp = M5.Axp.GetTempData()*0.1-144.7;

  M5.Rtc.GetTime(&TimeStruct);
 
  if(TimeStruct.Hours<10) hr="0"+String(TimeStruct.Hours); else hr=String(TimeStruct.Hours);
  if(TimeStruct.Minutes<10) mi="0"+String(TimeStruct.Minutes); else mi=String(TimeStruct.Minutes);
  if(TimeStruct.Seconds<10) se="0"+String(TimeStruct.Seconds); else se=String(TimeStruct.Seconds);

  draw();
  
}
