let radius;

let Min_Temperature=-30;
let temperatureBlue=0;
let temperatureGreen=25;
let temperatureRed=36;
let Max_Temperature=45;

var breatheradius;
var outerH=0; 
var outerS=100; 
var outerB=0; 
var oldcenterH; 
var oldcenterS=100; 
var oldcenterB; 
var centerH; 
var centerS=100; 
var centerB; 
var centerFadeH; 
var centerFadeS=100; 
var centerFadeB; 
var breatheH; 
var breatheS=100; 
var breatheB; 
var H=0;
var S=100;
var B=0;
var easeB;
var breatheSpeed=0.05;
var breatheLimit=1.3;
var flag=1;
var breatheFlag=1;

var colorFadeSpeed=0.0001;
var colorFadeH;
var colorFadeS=100;

newer_element=document.createElement("script");
newer_element.setAttribute("type","text/javascript");
newer_element.setAttribute("src","mqtt.js");// 在这里引入了a.js
 document.body.appendChild(newer_element);


function setup() {

  createCanvas(windowWidth+100, windowHeight+100);
  radius = windowWidth / 5;
  breatheRadius=radius;
  background(0);
  
  colorMode(HSB, 360, 100, 100);
  noStroke();
  ellipseMode(RADIUS);
  frameRate(200);

}


function draw() {

  if (!isStart) return 0;

  if (isUpdate) {
	
	oldcenterH=centerH;
	updateColor(); 

//---------- fade
	

  centerFadeB=centerB;

  if(colorFadeFlag) {

  if (centerH-oldcenterH>0) {
  if (centerFadeH+ colorFadeSpeed*(centerH-oldcenterH)>=centerH) {centerFadeH =centerH; colorFadeFlag=0;}
    else {centerFadeH+= colorFadeSpeed*(centerH-oldcenterH);}
  }
  else if (centerH-oldcenterH <0) {
   if (centerFadeH + colorFadeSpeed*(centerH-oldcenterH)<=centerH) {centerFadeH =centerH; colorFadeFlag=0;}
    else {centerFadeH+= colorFadeSpeed*(centerH-oldcenterH);}
  }
  else centerFadeH = centerH;

 }
    }

else {      	//first time receive message

updateColor(); 
oldcenterH=centerH; centerFadeH=oldcenterH;  
oldcenterB=centerB; centerFadeB=oldcenterB; 

breatheH=centerH; breatheB=centerB;

}

//------------
 
//-----------breathe

  breatheH = centerFadeH;	
  if (breatheFlag) {
		breatheRadius+= breatheSpeed*radius*0.2;
		if (breatheB*(1+breatheSpeed)>centerFadeB*breatheLimit) {breatheB=centerFadeB*breatheLimit; breatheFlag=0;}
		else {breatheB*=(1+breatheSpeed);}
	}
	else {
		breatheRadius-= breatheSpeed*radius*0.2;
		if (breatheB/(1+breatheSpeed)<centerFadeB/breatheLimit) {breatheB=centerFadeB/breatheLimit; breatheFlag=1;}
		else {breatheB/=(1+breatheSpeed);}
	}
	background(0);
	flag=1;
	drawGradient(width/2, height / 2);
}



function drawGradient(x, y) {
	
if (flag) {

  	easeB=(breatheB-outerB)/breatheRadius*0.5; 
 	B=0;
	H=breatheH;

for (let r = breatheRadius; r >0; r-=0.5) {

 	if (B+easeB>breatheB) {B=breatheB;}
	else {B+=easeB;}
 	fill(H,S,B); 
	ellipse(x, y, r, r);
}
	flag=0;
}
}



function updateColor(){ 

//264,100,50   <-30  Min_Temperature
//240,100,50     0     temperatureBlue
//120 100 50     25    temperatureGreen
//0,100,50     36    temperatureRed
//322,100,50   >45    Max_Temperature


 if( temperature<=Min_Temperature) centerH = 264;

 else if (temperature<=temperatureBlue) centerH= 264-24/(temperatureBlue-Min_Temperature)*(temperature-Min_Temperature);
 else if (temperature<=temperatureGreen) centerH= 240-120/(temperatureGreen-temperatureBlue)*(temperature-temperatureBlue);	  
 else if (temperature<=temperatureRed)	centerH= 120-120/(temperatureRed-temperatureGreen)*(temperature-temperatureGreen); 
 else if (temperature<=Max_Temperature)	centerH= 360-38/(Max_Temperature-temperatureRed)*(temperature-temperatureRed);
 else centerH=322;

 centerB=70;


}