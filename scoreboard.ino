#include <string.h>
#include <YK04_Module.h>

#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"

#include "Team.h"

#define R1_PIN 25
#define G1_PIN 26
#define B1_PIN 27
#define R2_PIN 14
#define G2_PIN 12
#define B2_PIN 13
#define A_PIN 23
#define B_PIN 22
#define C_PIN 5
#define D_PIN 17
#define E_PIN 32
#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 16

#define PANEL_RES_X 64
#define PANEL_RES_Y 64
#define PANEL_CHAIN 1

// YK04 Remote Control
// #define A_PIN 6
// #define B_PIN 4
// #define C_PIN 7
// #define D_PIN 5

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


HUB75_I2S_CFG::i2s_pins _pins = { R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN };
HUB75_I2S_CFG mxconfig(
  PANEL_RES_X,
  PANEL_RES_Y,
  PANEL_CHAIN,
  _pins,
  HUB75_I2S_CFG::FM6126A);

  MatrixPanel_I2S_DMA *matrix = nullptr;


void printTimerOnRGBMatrix() {
  const uint8_t *timerMinutesList = timer->getMinutesList();
  const uint8_t *timerSecondsList = timer->getSecondsList();

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

  matrix->setTextSize(1);
  matrix->setCursor(minutesX, matrixFirstRowY);

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
    matrix->print(timerMinutesList[0]);
  }

  matrix->print(timerMinutesList[1]);
  matrix->print(timerMinutesList[2]);

  if (!timer->isPaused()) {
    clearCoordinates(dotSeparatorX, matrixFirstRowY, charWidth + charPadding, charHeight);
  }

  matrix->print(":");

  if (secondsList[0] != timerSecondsList[0]) {
    clearCoordinates(secondsX, matrixFirstRowY, charWidth + charPadding, charHeight);
  }

  matrix->print(timerSecondsList[0]);

  if (secondsList[1] != timerSecondsList[1]) {
    clearCoordinates(secondsX + charWidth, matrixFirstRowY, charWidth + charPadding, charHeight);
  }

  matrix->print(timerSecondsList[1]);


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

  matrix->setTextSize(2);

  matrix->setCursor(leftTeamX, matrixSecondRowY);
  matrix->print(leftTeamScore);

  matrix->setCursor(dotsX, matrixSecondRowY);
  matrix->print(":");

  matrix->setCursor(rightTeamX, matrixSecondRowY);
  matrix->print(rightTeamScore);
}

void printTeamsOnRGBMatrix() {
  const int16_t dotsX = 26;
  matrix->setTextSize(1);
  matrix->setTextWrap(true);

  matrix->setCursor(matrixThirdRowX, matrixThirdRowY);
  matrix->print(String(leftTeam->getName()));

  matrix->setCursor(dotsX, matrixThirdRowY - 3);
  matrix->setTextSize(2);
  matrix->print(":");

  matrix->setTextSize(1);
  matrix->setCursor(dotsX + 8, matrixThirdRowY);
  matrix->print(String(rightTeam->getName()));
}

void clearCoordinates(int16_t x, int16_t y, int16_t w, int16_t h) {
  matrix->fillRect(x, y, w, h, matrix->color333(0, 0, 0));
}

void screenClear() {
  clearCoordinates(0, 0, matrix->width(), matrix->height());
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

  // remoteModule = new YK04_Module(A_PIN, B_PIN, C_PIN, D_PIN);

  matrix = new MatrixPanel_I2S_DMA(mxconfig);
  matrix->begin();
  //matrix->setTextColor(matrix->color333(1, 1, 1));
  matrix->setBrightness8(90);
  matrix->setTextWrap(false);
  screenClear();

  delay(500);  //wait for panel to be ready, avoids LED flashing
}

void loop() {
  currentMillis = millis();

  // readRemoteModule();
  timer->advance(currentMillis, previousMillis);

  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    printTimerOnRGBMatrix();
  }

  printScoresOnRGBMatrix();
  printTeamsOnRGBMatrix();
}