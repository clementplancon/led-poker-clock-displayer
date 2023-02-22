#include <Arduino.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// IO16
#define PIN_MATRIX 16

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    64,
    8,
    PIN_MATRIX,
    NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
    NEO_GRB + NEO_KHZ800);

int x = 0;
String smallBlindArray[] = {"50", "100", "150", "200", "250", "300", "400", "500", "600", "700", "800", "900", "1.000", "1.250", "1.500", "1.750", "2.000", "2.500", "3.000", "4.000", "5.000", "6.000", "7.000", "8.000", "9.000", "10.000"};
String bigBlindArray[] = {"100", "200", "300", "400", "500", "600", "800", "1.000", "1.200", "1.400", "1.600", "1.800", "2.000", "2.500", "3.000", "3.500", "4.000", "5.000", "6.000", "8.000", "10.000", "12.000", "14.000", "16.000", "18.000", "20.000"};

int roundLengthMilliseconds = 900000;
String displayTime = "15:00";
int currentRound = 0;
int screenLengthNeeded = -2;
String displayString = "15:00 50/100";

void formatDuration(int ms)
{
  string minStr;
  string secStr;
  int seconds = (int)(ms / 1000) % 60;
  int minutes = (int)((ms / (1000 * 60)));
  stringstream ssSec;
  stringstream ssMin;
  ssSec << seconds;
  ssMin << minutes;
  ssSec >> secStr;
  ssMin >> minStr;
  if (minStr.length() == 1)
  {
    minStr = "0" + minStr;
  }
  if (secStr.length() == 1)
  {
    secStr = "0" + secStr;
  }
  displayTime = (minStr + ":" + secStr).c_str();
}

void displayOnScreen(int round, String time, int cursor)
{
  matrix.fillScreen(0);
  matrix.setCursor(cursor, 0);
  if (round >= 25)
  {
    matrix.setTextColor(matrix.Color(0, 0, 255));
    matrix.print("00:00 ");
    matrix.setTextColor(matrix.Color(0, 255, 0));
    matrix.print("15000");
    matrix.setTextColor(matrix.Color(255, 255, 255));
    matrix.print("/");
    matrix.setTextColor(matrix.Color(255, 0, 0));
    matrix.print("30000");
    displayString = "00:00 " + smallBlindArray[24] + "/" + bigBlindArray[24];
  }
  else
  {
    matrix.setTextColor(matrix.Color(0, 0, 255));
    matrix.print(time + " ");
    matrix.setTextColor(matrix.Color(0, 255, 0));
    matrix.print(smallBlindArray[round]);
    matrix.setTextColor(matrix.Color(255, 255, 255));
    matrix.print("/");
    matrix.setTextColor(matrix.Color(255, 0, 0));
    matrix.print(bigBlindArray[round]);
    displayString = time + " " + smallBlindArray[round] + "/" + bigBlindArray[round];
  }
  screenLengthNeeded = 64 - ((displayString.length() * 5) + displayString.length());
  matrix.show();
}

void waitAndDisplay(int ms)
{
  roundLengthMilliseconds = roundLengthMilliseconds - ms;
  if (roundLengthMilliseconds <= 0)
  {
    roundLengthMilliseconds = 900000;
    currentRound = currentRound + 1;
  }
  formatDuration(roundLengthMilliseconds);
  displayOnScreen(currentRound, displayTime, x);
  delay(ms);
}

void setup()
{
  // put your setup code here, to run once:
  matrix.begin();
  matrix.setBrightness(10);
}

void loop()
{
  // put your main code here, to run repeatedly:
  x = x - 1;
  if (x < screenLengthNeeded)
  {
    x = 0;
  }
  if (x == 0)
  {
    waitAndDisplay(1000);
    waitAndDisplay(1000);
  }
  else if (x == screenLengthNeeded)
  {
    waitAndDisplay(1000);
    waitAndDisplay(1000);
  }
  else
  {
    waitAndDisplay(200);
  }
}