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
int clickBeep = 100; //Hz
int lowBeep = 500; //Hz
int highBeep = 1000;//Hz
int ultaShortBeep = 50; //ms
int shortBeep = 50; //ms
int longBeep = 100; //ms
int delayBeep = 200; //ms

//delys
int errDelay = 3000;
int creditsDelay = 10000;
int mainDelay = 20000;
int menuDelay = 5000;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousMenuMillis = 0;
unsigned long previousMainMillis = 0;
unsigned long previousTestMillis = 0;

//other
byte screen = 10;
byte prevScreen = 0;
int incomingScreen = 0;
int serialIncomingScreen = 0;;
String user = "FabLab robot";
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
byte deg[8] = {
  0b01100,
  0b10010,
  0b10010,
  0b01100,
  0b00000,
  0b00000,
  0b00000,
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
 lcd.createChar(7, deg);
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

 lcd.clear();
 Serial.println("*** Setup complete ***");


}

void loop() {
  currentMillis = millis();
  readEspSerial();
  readButton();
  showScreen();
  autoOut();
  //Serial.println(newStateBtn);
}



void readEspSerial(){
  // send data only when you receive data:
  //if (espSerial.available() > 0) {
  if (Serial.available() > 0) {
    // read the incoming byte:
    //incomingScreen = espSerial.parseInt();
    serialIncomingScreen = Serial.parseInt();
    // say what you got:
    Serial.print("I received on serial: ");
    Serial.println(serialIncomingScreen);//, DEC
  }
  if ((serialIncomingScreen > 0) && (serialIncomingScreen <= 9)) {
    serialIncomingScreen = incomingScreen;
    Serial.println("Next screen is" + String(incomingScreen));
    serialIncomingScreen = 0;
  }
}

void readButton() {
  newStateBtn = digitalRead(btnPrs);
  if(newStateBtn == LOW && oldStateBtn == HIGH){
    delay(20);
    newStateBtn = digitalRead(btnPrs);
    if(newStateBtn == LOW){
      if (screen == 4) {
        tone(beeper, clickBeep, ultaShortBeep);
        incomingScreen = 6;
      }
      else if (screen == 6){
        tone(beeper, clickBeep, ultaShortBeep);
        incomingScreen = 4;
      }
      else{
        incomingScreen = 0;
      }
    }
  }
  oldStateBtn = newStateBtn;
}

void showScreen(){
  if (screen != incomingScreen) {
    screen = incomingScreen;

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
        code
        */
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
  int y;
  y = 1;//line 1
  lcd.setCursor(2,y);
  lcd.print("Prusa I3 Fabman");
  y = 2;  //line 2
  lcd.setCursor(3,y);
  lcd.print("Swipe to login");
}

void noOpenHours() {
  logInBeep();
  lcd.clear();
  int y;
  y = 0;//line 0
  lcd.setCursor(0,y);
  lcd.print("You are not allowed");
  y = 1;//line 1
  lcd.setCursor(0,y);
  lcd.print("to use this printer");
  y = 2;//line 2
  lcd.setCursor(0,y);
  lcd.print("outside opening");
  y = 3;//line 3
  lcd.setCursor(0,y);
  lcd.print("hours.");
  delay(errDelay);
  logOutBeep();
  incomingScreen = 0;
}

void noCourse() {
  logInBeep();
  lcd.clear();
  int y;
  y = 0;//line 0
  lcd.setCursor(0,y);
  lcd.print("You do not have");
  y = 1;//line 1
  lcd.setCursor(0,y);
  lcd.print("required training");
  y = 2;//line 2
  lcd.setCursor(0,y);
  lcd.print("course!!");
  delay(errDelay);
  logOutBeep();
  incomingScreen = 0;
}

void noPackage() {
  logInBeep();
  lcd.clear();
  int y;
  y = 0;//line 0
  lcd.setCursor(0,y);
  lcd.print("You do not have");
  y = 1;//line
  lcd.setCursor(0,y);
  lcd.print("active package.");
  y = 2;//line 2
  lcd.setCursor(0,y);
  lcd.print("Please buy one.");
  delay(errDelay);
  logOutBeep();
  incomingScreen = 0;
}

void mainScreenStatic() {
  previousMainMillis = currentMillis;
  if (prevScreen == 0){
    logInBeep();
  }
  lcd.clear();
  int y;
  y = 0;//line 0
  lcd.setCursor(0,y);
  lcd.write(3);
  lcd.setCursor(2,y);
  lcd.print("28/0");
  lcd.write(7);
  lcd.setCursor(12,y);
  lcd.print("Z");
  lcd.setCursor(15,y);
  lcd.print("0.00");
  y = 1;//line 1
  lcd.setCursor(0,y);
  lcd.write(4);
  lcd.setCursor(2,y);
  lcd.print("28/0");
  lcd.write(7);
  lcd.setCursor(12,y);
  lcd.write(6);
  lcd.setCursor(13,y);
  lcd.print("100%");
  y = 2;//line 2
  lcd.setCursor(3,y);
  lcd.print("---%");
  lcd.setCursor(12,y);
  lcd.write(5);
  lcd.setCursor(13,y);
  lcd.print("--:--");
  y = 3;//line 3
  lcd.setCursor(0,y);
  lcd.print("User:");
  lcd.setCursor(6,y);
  lcd.print(user);

}

void mainScreenSimulate() {
  incomingScreen = 0;
  /* code */
}

void mainMenu() {
  previousMenuMillis = currentMillis;
  lcd.clear();
  int y;
  y = 0;//line 0
  lcd.setCursor(0,y);
  lcd.print(">" + user);
  lcd.setCursor(19,y);
  lcd.write(1);
  y = 1;//line 1
  lcd.setCursor(0,y);
  lcd.print(" Info screen");
  lcd.setCursor(19,y);
  lcd.write(1);
  y = 2;//line 2
  lcd.setCursor(0,y);
  lcd.print(" Preheat");
  lcd.setCursor(19,y);
  lcd.write(2);
  y = 3;//line 3
  lcd.setCursor(0,y);
  lcd.print(" No SD card");
  lcd.setCursor(19,y);
  lcd.write(2);
}

void credits() {
  logInBeep();
  lcd.clear();
  int y;
  y = 0;//line 0
  lcd.setCursor(4,y);
  lcd.print("FabLab Brno");
  y = 1;//line 1
  lcd.setCursor(2,y);
  lcd.print("fablabbrno.cz/en");
  y = 2;//line 2
  lcd.setCursor(7,y);
  lcd.print("Fabman");
  y = 3;//line 3
  lcd.setCursor(5,y);
  lcd.print("fabman.io");
  delay(creditsDelay);
  logOutBeep();
  incomingScreen = 0;
}

void unknowKey() {
  logInBeep();
  lcd.clear();
  int y;
  y = 1;//line 1
  lcd.setCursor(5,y);
  lcd.print("Unknown key");
  delay(errDelay);
  logOutBeep();
  incomingScreen = 0;
}


void autoOut() {
  if (screen == 6){
    previousMainMillis = currentMillis+mainDelay;
  }
  if(currentMillis >= (previousMenuMillis+menuDelay)){
    if (screen == 6){
      incomingScreen = 4;//main screen
    }
  }
  if(currentMillis >= (previousMainMillis+mainDelay)){
    if (screen == 4){
      logOutBeep();
    }
    incomingScreen = 0;

  }
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
