#include <SoftwareSerial.h>

SoftwareSerial Lcd(3,2);

// Turn on to output min/max X Y Z values
const bool CALIBRATE = true;
const bool LCD_ONLY = true;

// analog input pins
const int X_PIN = A0;
const int Y_PIN = A1;
const int Z_PIN = A2;

// ADXL334
// const int MIN = 401;
// const int MAX = 630;

// MMA7361
const int MIN = 224;
const int MAX = 758;


// MMA7361
// xMax:729 xMin:239 yMax:770 yMin:267 zMax:721 zMin:229

const int X_MIN = 239;
const int X_MAX = 729;
const int Y_MIN = 267;
const int Y_MAX = 770;
const int Z_MIN = 229;
const int Z_MAX = 721;

int xMax = 0;
int xMin = 1024;
int yMax = 0;
int yMin = 1024;
int zMax = 0;
int zMin = 1024;

void setup(){
  // set up the LCD's number of rows and columns:
  if (LCD_ONLY) {
    Lcd.begin(9600);
  }
  
  Serial.begin(9600);

  // set analogRead max to 3.3v instead of default 5v
  analogReference(EXTERNAL);

  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
}

void loop() {

  // sample the voltages
  // delay(10);
  // analogRead(X_PIN);
  // delay(10);
  int x = analogRead(X_PIN);
  // analogRead(Y_PIN);
  // delay(10);
  int y = analogRead(Y_PIN);
  // analogRead(Z_PIN);
  // delay(10);
  int z = analogRead(Z_PIN);

  if (CALIBRATE) {
    if (x > xMax) {
      xMax = x;
    }
    if (x < xMin) {
      xMin = x;
    }
    if (y > yMax) {
      yMax = y;
    }
    if (y < yMin) {
      yMin = y;
    }
    if (z > zMax) {
      zMax = z;
    }
    if (z < zMin) {
      zMin = z;
    }
 
    if (LCD_ONLY) {
      moveToFirstLine();
      Lcd.print("  ");
      Lcd.print(xMax);
      Lcd.print("  ");
      Lcd.print(yMax);
      Lcd.print("  ");
      Lcd.print(zMax);
      Lcd.print(" ");
      moveToSecondLine();
      Lcd.print("  ");
      Lcd.print(xMin);
      Lcd.print("  ");
      Lcd.print(yMin);
      Lcd.print("  ");
      Lcd.print(zMin);
      Lcd.print(" ");
    } else {
      Serial.print("xMax:");
      Serial.print(xMax);
      Serial.print(" xMin:");
      Serial.print(xMin);
      Serial.print(" yMax:");
      Serial.print(yMax);
      Serial.print(" yMin:");
      Serial.print(yMin);
      Serial.print(" zMax:");
      Serial.print(zMax);
      Serial.print(" zMin:");
      Serial.println(zMin);
    }
  }
  
  Serial.print("x:");
  Serial.print(x);
  Serial.print(" y:");
  Serial.print(y);
  Serial.print(" z:");
  Serial.println(z);

  // convert to range of -90 to +90 degrees
//  int xAng = map(x, MIN, MAX, -90, 90);
//  int yAng = map(y, MIN, MAX, -90, 90);
//  int zAng = map(z, MIN, MAX, -90, 90);
  int xAng = map(x, X_MIN, X_MAX, -90, 90);
  int yAng = map(y, Y_MIN, Y_MAX, -90, 90);
  int zAng = map(z, Z_MIN, Z_MAX, -90, 90);

  // print angles
//  Serial.print("xAng:");
//  Serial.print(xAng);
//  Serial.print(" yAng:");
//  Serial.print(yAng);
//  Serial.print(" zAng:");
//  Serial.println(zAng);

  // simulate mount on X axis
//  if (zAng > 0) {
//    if (xAng <= 0) {
//      xAng = xAng + 90;
//    } else {
//      xAng = 90 - xAng;
//    }
//  } else {
//    if (xAng <= 0) {
//      xAng = -90 - xAng;
//    } else {
//      xAng = xAng - 90;
//    }
//  }
//
//  if (xAng > 0) {
//    if (zAng <= 0) {
//      zAng = -90 - zAng;
//    } else {
//      zAng = 90 - zAng;
//    }
//  } else {
//    if (zAng <= 0) {
//      zAng = zAng + 90;
//    } else {
//      zAng = 90 - zAng;
//    }
//  }

  if (!LCD_ONLY) {
    Serial.print("xAng:");
    Serial.print(xAng);
    Serial.print(" yAng:");
    Serial.print(yAng);
    Serial.print(" zAng:");
    Serial.println(zAng);
  }

  // convert radians to degrees
  int pitch = -(RAD_TO_DEG * (atan2(-yAng, -zAng) + PI));
  int roll = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);

  // convert left roll and forward pitch to negative degrees
  if (pitch < -180) {
    pitch = pitch + 360;
  }
  if (roll > 180) {
    roll = roll - 360;
  }

  // write the pitch and roll to the second line
  if (!CALIBRATE) {
    updateDisplay(pitch, roll);
  }

  // Wait a quarter second so the numbers aren't flashing so fast
  delay(250);
}


void updateDisplay(int first, int second) {
  // convert int values to strings for output
  String firstString = String(first);
  String secondString = String(second);

  if (!LCD_ONLY) {
    Serial.print("Pitch:");
    Serial.print(secondString);
    Serial.print(" Roll:");
    Serial.println(firstString);
  }
}

void moveToFirstLine() {
  Lcd.write(254);
  Lcd.write(128); 
}

void moveToSecondLine() {
  Lcd.write(254);
  Lcd.write(192); 
}
