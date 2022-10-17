#include <string.h>
#include <YK04_Module.h>

#define A_PIN 6
#define B_PIN 4
#define C_PIN 7
#define D_PIN 5

const long secondInMillis = 1000;
const long updateInterval = 1000;
const long remoteDelayInterval = 998;

unsigned long currentMillis;
unsigned long previousMillis = 0;
char timerText[7];
String scoreText;

YK04_Module *remoteModule;

class Timer {
  int minutes;
  int seconds;
  bool paused;

public:
  Timer() {
    minutes = 0;
    seconds = 0;
    paused = false;
  }

  void reset() {
    paused = true;
    minutes = 0;
    seconds = 0;
  }

  void advance() {
    if(paused){
      return;
    }

    if (currentMillis - previousMillis >= secondInMillis) {
      if (seconds == 59) {
        seconds = 0;
        minutes++;
      } else {
        seconds++;
      }
    }
  }

  void resume() {
    paused = false;
  }

  void pause() {
    paused = true;
  }

  bool isPaused() {
    return paused;
  }

  int getMinutes() {
    return minutes;
  }

  int getSeconds() {
    return seconds;
  }
};

Timer *timer;

class Team {
  String name;
  int score;

public:
  Team(String aName) {
    name = aName;
    score = 0;
  }

  String getName() {
    return name;
  }

  int getScore() {
    return score;
  }

  void addScore() {
    if(timer->isPaused()){
      timer->resume();

      return;
    }
    
    score += 1;
  }

  void subtractScore() {
    if(timer->isPaused()){
      timer->resume();

      return;
    }

    if (score > 0) {
      score -= 1;
    }
  }

  void resetScore() {
    score = 0;
  }
};

Team *leftTeam;
Team *rightTeam;

void setup() {
  Serial.begin(9600);

  leftTeam = new Team("Black");
  rightTeam = new Team("White");
  remoteModule = new YK04_Module(A_PIN, B_PIN, C_PIN, D_PIN);
  timer = new Timer();
}

void loop() {
  currentMillis = millis();

  readRemoteModule();
  timer->advance();

  sprintf(timerText, "%02d:%02d", timer->getMinutes(), timer->getSeconds());
  scoreText = "Score: Left(" + leftTeam->getName() + ") => " + leftTeam->getScore() + " :  Right(" + rightTeam->getName() + ") => " + rightTeam->getScore() + "";



  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    // debugging only
    Serial.println(timerText);
    Serial.println(scoreText);
  }
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