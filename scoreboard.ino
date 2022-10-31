#include <string.h>
#include <YK04_Module.h>

#include "RGBmatrixPanel.h"
#include "Team.h"

// RGB Matrix Panel 64x64
#define CLK 11
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3
#define E A4

// YK04 Remote Control
#define A_PIN 6
#define B_PIN 4
#define C_PIN 7
#define D_PIN 5

const int16_t matrixFirstRowX = 16;
const int16_t matrixFirstRowY = 4;

const int16_t matrixSecondRowX = 14;
const int16_t matrixSecondRowY = 18;

const int16_t matrixThirdRowX = 0;
const int16_t matrixThirdRowY = 40;

const long secondInMillis = 1000;
const long updateInterval = 1000;
const long remoteDelayInterval = 998;
unsigned long currentMillis;
unsigned long previousMillis = 0;

Timer *timer;
Team *leftTeam;
Team *rightTeam;

YK04_Module *remoteModule;
RGBmatrixPanel matrix(A, B, C, D, E, CLK, LAT, OE, false, 64);

void printTimerOnRGBMatrix() {
  const int8_t *timerMinutesList = timer->getMinutesList();
  const int8_t *timerSecondsList = timer->getSecondsList();

  const int16_t charWidth = 5;
  const int16_t charHeight = 8;
  const int16_t charPadding = 1;
  const int16_t maxCharsMinutes = 3;

  int16_t charsMinutes = maxCharsMinutes - 1;
  int16_t minutesX = matrixFirstRowX;

  if (timerMinutesList[0] > 0) {
    minutesX -= 6;
    charsMinutes++;
  }

  const int16_t dotSeparatorX = minutesX + (charWidth + charPadding) * charsMinutes;
  const int16_t secondsX = minutesX + ((charWidth + charPadding) * (charsMinutes + 1));

  static int8_t minutesList[3] = { 0, 0, 0 };
  static int8_t secondsList[2] = { 0, 0 };

  matrix.setTextColor(matrix.Color333(1, 1, 1));
  matrix.setTextSize(1);
  matrix.setCursor(minutesX, matrixFirstRowY);

  if (minutesList[0] != timerMinutesList[0]) {
    clearCoordinates(minutesX, matrixFirstRowY, charWidth + charPadding, charHeight);
  }

  if (minutesList[1] != timerMinutesList[1]) {
    clearCoordinates(minutesX + ((charWidth + charPadding) * (charsMinutes / maxCharsMinutes)), matrixFirstRowY, charWidth + charPadding, charHeight);
  }

  if (minutesList[2] != timerMinutesList[2]) {
    clearCoordinates(minutesX + ((charWidth + charPadding) * (charsMinutes / maxCharsMinutes + 1)), matrixFirstRowY, charWidth + charPadding, charHeight);
  }

  if (timerMinutesList[0] > 0) {
    matrix.print(timerMinutesList[0]);
  }

  matrix.print(timerMinutesList[1]);
  matrix.print(timerMinutesList[2]);

  clearCoordinates(dotSeparatorX, matrixFirstRowY, charWidth + charPadding, charHeight);
  matrix.print(":");

  if (secondsList[0] != timerSecondsList[0]) {
    clearCoordinates(secondsX, matrixFirstRowY, charWidth + charPadding, charHeight);
  }

  matrix.print(timerSecondsList[0]);

  clearCoordinates(secondsX + charWidth, matrixFirstRowY, charWidth + charPadding, charHeight);
  matrix.print(timerSecondsList[1]);


  minutesList[0] = timerMinutesList[0];
  minutesList[1] = timerMinutesList[1];
  minutesList[2] = timerMinutesList[2];

  secondsList[0] = timerSecondsList[0];
  secondsList[1] = timerSecondsList[1];
}

void printScoresOnRGBMatrix() {
  const int16_t dotsX = 26;
  const int8_t charHeight = 14;

  static int8_t leftTeamScore = 0;
  static int8_t rightTeamScore = 0;

  int16_t leftTeamX = matrixSecondRowX;
  int16_t rightTeamX = dotsX + 12;

  int8_t teamLeftTeamScore = leftTeam->getScore();
  int8_t teamRightTeamScore = rightTeam->getScore();

  int8_t charLeftWidth = 12;
  int8_t charRightWidth = 12;


  if (teamLeftTeamScore > 9) {
    leftTeamX -= 10;
    charLeftWidth *= 2;
  }

  if (teamRightTeamScore > 9) {
    rightTeamX -= 2;
    charRightWidth *= 2;
  }

  if (leftTeamScore != teamLeftTeamScore) {
    leftTeamScore = teamLeftTeamScore;
    clearCoordinates(leftTeamX, matrixSecondRowY, charLeftWidth, charHeight);
  }

  if (rightTeamScore != teamRightTeamScore) {
    rightTeamScore = teamRightTeamScore;
    clearCoordinates(rightTeamX, matrixSecondRowY, charRightWidth, charHeight);
  }

  matrix.setTextColor(matrix.Color333(1, 1, 1));
  matrix.setTextSize(2);

  matrix.setCursor(leftTeamX, matrixSecondRowY);
  matrix.print(leftTeamScore);

  matrix.setCursor(dotsX, matrixSecondRowY);
  matrix.print(":");

  matrix.setCursor(rightTeamX, matrixSecondRowY);
  matrix.print(rightTeamScore);
}

void printTeamsOnRGBMatrix() {
  const int16_t dotsX = 26;
  matrix.setTextColor(matrix.Color333(1, 1, 1));
  matrix.setTextSize(1);
  matrix.setTextWrap(true);

  matrix.setCursor(matrixThirdRowX, matrixThirdRowY);
  matrix.print(String(leftTeam->getName()));

  matrix.setCursor(dotsX, matrixThirdRowY - 3);
  matrix.setTextSize(2);
  matrix.print(":");

  matrix.setTextSize(1);
  matrix.setCursor(dotsX + 8, matrixThirdRowY);
  matrix.print(String(rightTeam->getName()));
}

void clearCoordinates(int16_t x, int16_t y, int16_t w, int16_t h) {
  matrix.fillRect(x, y, w, h, matrix.Color333(0, 0, 0));
}

void screenClear() {
  clearCoordinates(0, 0, matrix.width(), matrix.height());
}

void readRemoteModule() {
  remoteModule->multipleRead();

  if (currentMillis - previousMillis < remoteDelayInterval) {
    return;
  }

  if (remoteModule->isA() && remoteModule->isB()) {
    timer->pause();
  } else if (remoteModule->isC() && remoteModule->isD()) {
    timer->reset();
    leftTeam->resetScore();
    rightTeam->resetScore();
  } else {
    //scores
    if (remoteModule->isA()) {
      leftTeam->addScore();
    } else if (remoteModule->isB()) {
      rightTeam->addScore();
    } else if (remoteModule->isC()) {
      leftTeam->subtractScore();
    } else if (remoteModule->isD()) {
      rightTeam->subtractScore();
    }
  }
}

void setup() {
  timer = new Timer();

  char *leftTeamName = new char;
  leftTeamName = "Black";

  char *rightTeamName = new char;
  rightTeamName = "White";

  leftTeam = new Team(leftTeamName, timer);
  rightTeam = new Team(rightTeamName, timer);

  remoteModule = new YK04_Module(A_PIN, B_PIN, C_PIN, D_PIN);

  matrix.begin();
  matrix.setTextWrap(false);
  screenClear();

  delay(500); //wait for panel to be ready, avoids LED flashing
}

void loop() {
  currentMillis = millis();

  readRemoteModule();
  timer->advance(currentMillis, previousMillis);

  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    printTimerOnRGBMatrix();
  }

  printScoresOnRGBMatrix();
  printTeamsOnRGBMatrix();
}