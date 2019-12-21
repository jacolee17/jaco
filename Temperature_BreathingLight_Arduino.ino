/**
 * Example for reading temperature and humidity
 * using the DHT22 and ESP8266
 *
 * Copyright (c) 2016 Losant IoT. All rights reserved.
 * https://www.losant.com
 */

#include "DHT.h"

#define DHTPIN 14     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors

DHT dht(DHTPIN, DHTTYPE);

const int RED = 5;
const int GREEN = 4;
const int BLUE = 0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  // Wait for serial to initialize.
  while(!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running DHT!");
  Serial.println("-------------------------------------");

}

int timeSinceLastRead = 0;
const int delayInterval=20;

const double brightness=250.00;

const int Min_Temperature=-30;
const int temperatureBlue=0;
const int temperatureGreen=25;
const int temperatureRed=34;
const int Max_Temperature=45;

const int ColorSum=770;    // depends on the starting color and the ending color designer choose, represent the color for gradual changes
const int delayMultiple=4;

double R=0,G=0,B=0;
double breatheR,breatheG,breatheB;
double oldR,oldG,oldB;
double newR,newG,newB;
double breatheSpeed=0.01;

float temperature=0;
int delayTime=0;
int delayFlag=1;
int delaySlow=delayMultiple;
bool isStart=0;
bool isTwoTemperature=0;

void loop() {

  // Report every 2 seconds.
  if(timeSinceLastRead > 2000) {
    timeSinceLastRead = 0;
    temperature=getTemperature(temperature);
    isStart=1;
    if (!isTwoTemperature) 
    { UpdateColor(temperature,&R,&G,&B);
      isTwoTemperature=1;
    }
    else {
      UpdateColor(temperature,&newR,&newG,&newB);
      oldR=R; oldG=G; oldB=B;
    }
  }
  timeSinceLastRead += delayInterval;
  if (!isStart) return;

  if (newR-oldR>0) {
  if (R+breatheSpeed*(newR-oldR)>=newR) {R=newR;}
    else {R+=breatheSpeed*(newR-oldR);}
  }
  else if (newR-oldR<0) {
   if (R+breatheSpeed*(newR-oldR)<=newR) {R=newR;}
    else {R+=breatheSpeed*(newR-oldR);}
  }
  else R=newR;

  if (newG-oldG>0) {
  if (G+breatheSpeed*(newG-oldG)>=newG) {G=newG;}
    else {G+=breatheSpeed*(newG-oldG);}
  }
  else if (newG-oldG<0) {
   if (G+breatheSpeed*(newG-oldG)<=newG) {G=newG;}
    else {G+=breatheSpeed*(newG-oldG);}
  }
  else G=newG;

    if (newB-oldB>0) {
  if (B+breatheSpeed*(newB-oldB)>=newB) {B=newB;}
    else {B+=breatheSpeed*(newB-oldB);}
  }
  else if (newB-oldB<0) {
   if (B+breatheSpeed*(newB-oldB)<=newB) {B=newB;}
    else {B+=breatheSpeed*(newB-oldB);}
  }
  else B=newB;
  
  LED_lightup(&R,&G,&B);
  delay(delayTime);
  LED_godark(&R,&G,&B);
  delay(delayInterval-delayTime);
 
  if (delaySlow==0) {
  if (delayFlag) delayTime++;
  else delayTime--;
  delaySlow=delayMultiple;
  }
  else delaySlow--;
  
  if (delayTime==delayInterval) delayFlag=0;
  if (delayTime==0) delayFlag=1;
 
}


void UpdateColor( double temperature, double *R, double *G, double *B)
{ 

//100,0,255   <-30  Min_Temperature
//0,0,255     0     temperatureBlue
//0 255 0     25    temperatureGreen
//255,0,0     36    temperatureRed
//255,0,160   >45    Max_Temperature

 if( temperature<=Min_Temperature) 
  {
    *R= 100;
     *G=0; *B=255;
  // Serial.println("1");
       }
 if( temperature<=temperatureBlue) 
  {
  *R= 100-((double)(temperature - Min_Temperature)*100 / (double)(temperatureBlue-Min_Temperature));
   *G=0; *B=255;
  // Serial.println("1");
       }
  else if ( temperature<=temperatureGreen)
  { 
  *R=0;
  *G= ((double)(temperature - temperatureBlue)*255 / (double)(temperatureGreen-temperatureBlue));
  *B= 255-*G;
  //Serial.println("2");
      } 
  else if ( temperature<=temperatureRed)
  { 
  *R= ((double)(temperature - temperatureGreen)*255 / (double)(temperatureRed-temperatureGreen));
  *G= 255-*R;
  *B= 0;
 // Serial.println("3");
      } 
  else if (temperature<=Max_Temperature)
  {
  *R=255;
  *G=0;
  *B= ((double)(temperature - temperatureRed)* 160 / (double)(Max_Temperature-temperatureRed));
 // Serial.println("4");
      }
   else
   {  
    *R=255;
    *G=0;
    *B= 160;
   }

     // Serial.println(*R);Serial.println(*G);Serial.println(*B);
      Serial.println("----");

}

bool isRange(int a) {
  if (a>=0 and a<=255) return 1;
  return 0;
}

void LED_lightup(double *r, double *g, double *b)
{
  // Set LED’s brightness to a specific value with LED’s API

  if (!(isRange(*r) and isRange(*g) and isRange(*b))) return;
  
  analogWrite(RED, 1023-(int)(*r/255*brightness));
  analogWrite(GREEN, 1023-(int)(*g/255*brightness));
  analogWrite(BLUE, 1023-(int)(*b/255*brightness));  

  
}

void LED_godark(double *r, double *g, double *b)
{

  // Set LED’s brightness to zero with LED’s API

  if (!(isRange(*r) and isRange(*g) and isRange(*b))) return;

  analogWrite(RED, 1023-(int)((double)*r/255*brightness/6));
  analogWrite(GREEN, 1023-(int)((double)*g/255*brightness/6));
  analogWrite(BLUE, 1023-(int)((double)*b/255*brightness/6));  

}


float getTemperature(float ppp)
{
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return (ppp);
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    
  // Get temperature from sensor using sensor's API

    Serial.print(hic);
    Serial.println(" *C ");
    return hic;
    
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");

    
    
}
