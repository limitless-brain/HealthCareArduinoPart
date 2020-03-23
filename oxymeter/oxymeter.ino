//  Created by Frederic Ajaltouni <fredericajaltouni@gmail.com> *
//  Modified by Mohamad Yazan Suliman <m.yazan.suliman@gmail.com> *
//  Line add/modified by modifiers are marked by '*' *


#include <PinChangeInt.h>
#include <eHealth.h>

//  Ucomment this if you're using HW serial (pins 0,1) *
#define USE_HW_SERIAL
#ifndef USE_HW_SERIAL   //  Check if we use Hardwear serial *
#define BT_SERIAL_TX 2  //  Change this pin to fit your setup *
#define BT_SERIAL_RX 3  //  Change this pin to fit your setup *
//  Due SoftwareSerial library conclusion with PinChangeInt library  *
//  This project can't use SoftwareSerial  *
//#include<SoftwareSerial.h>
  SoftwareSerial BTModule(BT_SERIAL_RX,BT_SERIAL_TX);
#endif

int cont = 0;

int Min =88 ; 
int Normal = 92 ;

//  last time code execution *
long lastTime = 0; // this start at 0 *
//  time between two loop *
long exeRate = 500; //  decrease this to make the loop go faster *
void setup() {
  //  Initilize serial port *
  Serial.begin(115200);
  //  Initilize E-Health sensor *
  eHealth.initPulsioximeter();
  //  Initilize BT module *
  #ifndef USE_HW_SERIAL
  Serial.print("Initilizing BT module...");
  BTModule.begin(115200); //  Change bundrate to fit your module speces *
  BTModule.println("done!");
  #endif

  //  Attach the inttruptions for using the pulsioximeter.   
  //PCintPort::attachInterrupt(6, readPulsioximeter, RISING);

  //  Everthing take amount of time, so we record the time we take *
  lastTime = millis();  //  Storing the time take to execute the above code *
  
}

void loop() {
 /* 
  int x;
  x=eHealth.getOxygenSaturation();
  if (x<Min){

      Serial.println("15L of oxygen");  
        delay(120000);
  }
  
  else if(x>Min && x<Normal){
    Serial.println("3L or 4L or 5L of oxygen");
  }
  else{
  Serial.println("2L of Oxygen ");
  }
  */
  //  We calaculate the amount of time between each two measurement *
  //  so we can delay the execution of our code without effecting *
  //  inttruptions. *
  if ( millis() >= lastTime + exeRate ){ // Check if we hit the execution time *
    //  Store data from E-Health module *
    float rate = eHealth.getBPM(); // *
    float oxRate = eHealth.getOxygenSaturation(); // *
    #ifndef USE_HW_SERIAL
    //  Printing sensor result into serial port for debug purposes *
    
    Serial.print("PRbpm : "); 
    Serial.print(rate); //  *

    Serial.print("    %SPo2 : ");
    Serial.print(oxRate); //  *
    Serial.print("\n");  
    Serial.println("=============================");

    
    //  Printing data to BTModule in Android format *
    
    BTModule.print("P");  //  *
    BTModule.print(rate); //  *
    BTModule.print("o");  //  *
    BTModule.println(oxRate); //  *
    
    #else
    //  We are not using HW serial for debug *
    //  printing data to bt in Android format *
    
    Serial.print("P");  //  *
    Serial.print(rate); //  *
    Serial.print("o");  //  *
    Serial.println(oxRate); //  *
    
    #endif

    //  Increase the time by execute rate *
    lastTime += exeRate;
  }
  //  delay(500); we don't use delay becuase it's stopping code execution *
}


//Include always this code when using the pulsioximeter sensor
//=========================================================================
void readPulsioximeter(){  

  cont ++;

  if (cont == 50) { //Get only of one 50 measures to reduce the latency
    eHealth.readPulsioximeter();  
    cont = 0;
  }
}
