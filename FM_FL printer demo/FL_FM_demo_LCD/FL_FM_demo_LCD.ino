/*
Autor: Chroja
Setup for show custom fabman on 3d printer

Board: Arduino nano
LCD: 2004 from 3d printer




*/

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
//2004
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//software serial for esp
SoftwareSerial espSerial(2, 3); // RX, TX
//pins
const int beeper = 10;
const int btnPrs = 11;
//state
bool newStateBtn = 0;
bool oldStateBtn = 0;
//beeps
int lowBeep = 500; //Hz
int highBeep = 1000;
int shortBeep = 50; //ms
int longBeep = 100;
int delayBeep = 200; //ms
//tone(beeper, lowBeep, lowBeep);
int errDelay = 3000;
int creditsDelay = 10000;
int mainDelay = 6000;
int menuDelay = 3000;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousMenuMillis = 0;
unsigned long previousMainMillis = 0;




//other
byte screen = 10;
byte prevScreen = 0;
int incomingScreen = 0;
//define custom char
byte up[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b11100,
  0b00000,
  0b00000,
  0b00000
};
byte right[8] = {
  0b00000,
  0b00100,
  0b00010,
  0b11111,
  0b00010,
  0b00100,
  0b00000,
  0b00000
};
byte temp[8] = {
  0b00100,
  0b01010,
  0b01010,
  0b01010,
  0b10001,
  0b10001,
  0b01110,
  0b00000
};
byte bed[8] = {
  0b00000,
  0b11111,
  0b10101,
  0b10001,
  0b10101,
  0b11111,
  0b00000,
  0b00000
};
byte time[8] = {
  0b00000,
  0b01110,
  0b10011,
  0b10101,
  0b10001,
  0b01110,
  0b00000,
  0b00000
};
byte speed[8] = {
  0b01000,
  0b00100,
  0b10010,
  0b01001,
  0b10010,
  0b00100,
  0b01000,
  0b00000
};






void setup() {
  //beeper
 pinMode(beeper, OUTPUT);
 digitalWrite(beeper, LOW);
 //bnt click
 pinMode(btnPrs, INPUT_PULLUP);
 newStateBtn = digitalRead(btnPrs);
 oldStateBtn = digitalRead(btnPrs);

 //create custom charr
 lcd.createChar(1, up);
 lcd.createChar(2, right);
 lcd.createChar(3, temp);
 lcd.createChar(4, bed);
 lcd.createChar(5, time);
 lcd.createChar(6, speed);
//serial comuniacation
 Serial.begin(115200);
 espSerial.begin(57600);
 // set up the LCD's number of columns and rows:
 lcd.begin(20, 4);
 lcd.setCursor(1, 1);                           //(znak, radek)
 // Print a message to the LCD.
 lcd.print("Original Prusa I3");
 lcd.setCursor(3, 2);
 lcd.print("Prusa Research");

 delay(1000);

 tone(beeper, highBeep, longBeep);
 delay(delayBeep+longBeep);
 //tone(beeper, lowBeep, shortBeep);
 delay(1000);
 lcd.clear();
 Serial.println("*** Setup complete ***");


}

void loop() {
  readEspSerial();
  readButton();
  showScreen();
  Serial.println(newStateBtn);
}

void readEspSerial(){
  // send data only when you receive data:
  //if (espSerial.available() > 0) {
  if (Serial.available() > 0) {
   // read the incoming byte:
   //incomingScreen = espSerial.parseInt();
   incomingScreen = Serial.parseInt();

   // say what you got:
   Serial.print("I received: ");
   Serial.println("btn state " + String(incomingScreen) + "  screen" + String(screen));//, DEC

   }
}

void readButton() {
  newStateBtn = digitalRead(btnPrs);
  if(newStateBtn == LOW && oldStateBtn == HIGH){
    delay(20);
    newStateBtn = digitalRead(btnPrs);
    if(newStateBtn == LOW){
      int a;
      if (screen == 4) {
        //a = 6;
        incomingScreen = 6;
      }
      else if (screen == 6){
        //a = 4;
        incomingScreen = 4;
      }
      else{
        //a = 0;
        logOutBeep();
        incomingScreen = 0;
      }
      //incomingScreen = a;
    }
  }
  oldStateBtn = newStateBtn;
}

void showScreen(){
  if (screen != incomingScreen) {
    currentMillis = millis();
    screen = incomingScreen;

    //Serial.println("screen: " + String(screen));
    //Serial.println("incoming screen: " + String(incomingScreen));

    switch (screen) {
      case 0:
        swipeCard();
        break;
      case 1:
        noOpenHours();
        break;
      case 2:
        noCourse();
        break;
      case 3:
        noPackage();
        break;
      case 4:
        mainScreenStatic();
        break;
      case 5:
        mainScreenSimulate();
        break;
      case 6:
        mainMenu();
        break;
      case 7:
                            /*
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Case 7");*/
        break;
      case 8:
        credits();
        break;
      case 9:
        unknowKey();
        break;
      default:
        screen = 0;
        break;

    }
    prevScreen = screen;
  }
}

void swipeCard(){
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Prusa I3 Fabman");
  lcd.setCursor(3,2);
  lcd.print("Swipe to login");
}

void noOpenHours() {
  logInBeep();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You are not allowed");
  lcd.setCursor(0,1);
  lcd.print("to use this printer");
  lcd.setCursor(0,2);
  lcd.print("outside opening");
  lcd.setCursor(0,3);
  lcd.print("hours.");
  delay(errDelay);
  logOutBeep();
  incomingScreen = 0;
}

void noCourse() {
  logInBeep();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You do not have");
  lcd.setCursor(0,1);
  lcd.print("required training");
  lcd.setCursor(0,2);
  lcd.print("course!!");
  delay(errDelay);
  logOutBeep();
  incomingScreen = 0;
}

void noPackage() {
  logInBeep();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("You do not have");
  lcd.setCursor(0,1);
  lcd.print("active package.");
  lcd.setCursor(0,2);
  lcd.print("Please buy one.");
  delay(errDelay);
  logOutBeep();
  incomingScreen = 0;
}

void mainScreenStatic() {
  previousMainMillis = millis();
  if (prevScreen == 0){
    logInBeep();
  }
  lcd.clear();
  //code
  lcd.setCursor(0,0);
  lcd.print("main screen");



  if(millis() >= (previousMainMillis+mainDelay)){
    incomingScreen = 0;
  }
}

void mainScreenSimulate() {
  incomingScreen = 0;
  /* code */
}

void mainMenu() {
  previousMenuMillis = millis();
  logInBeep();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">FabLab Robot");
  lcd.setCursor(0,1);
  lcd.print(" Info screen");
  lcd.setCursor(0,2);
  lcd.print(" Preheat");
  lcd.setCursor(0,3);
  lcd.print(" No SD card");
  lcd.setCursor(19,0);
  lcd.write(1);
  lcd.setCursor(19,1);
  lcd.write(1);
  lcd.setCursor(19,2);
  lcd.write(2);
  lcd.setCursor(19,3);
  lcd.write(2);
  //delay(creditsDelay);
  if(millis() >= (previousMenuMillis+menuDelay)){
    incomingScreen = 4;
  }
}

void credits() {
  logInBeep();
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("FabLab Brno");
  lcd.setCursor(2,1);
  lcd.print("fablabbrno.cz/en");
  lcd.setCursor(7,2);
  lcd.print("Fabman");
  lcd.setCursor(5,3);
  lcd.print("fabman.io");
  delay(creditsDelay);
  logOutBeep();
  incomingScreen = 0;

}

void unknowKey() {
  logInBeep();
  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print("Unknow key");
  delay(errDelay);
  logOutBeep();
  incomingScreen = 0;
}



void logInBeep(){
  tone(beeper, lowBeep, shortBeep);
  delay(5*delayBeep);
  tone(beeper, highBeep, longBeep);
  delay(longBeep);
}

void logOutBeep(){
  tone(beeper, highBeep, longBeep);
  delay(longBeep + delayBeep);
  tone(beeper, lowBeep, longBeep);
  delay(longBeep);
}
