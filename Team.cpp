#include "Team.h"

Team::Team(char* aName, Timer* aTimer) {
  name = aName;
  timer = aTimer;
  score = 0;
}

char* Team::getName() {
  return name;
}

int8_t Team::getScore() {
  return score;
}

void Team::addScore() {
  if (timer->isPaused()) {
    timer->resume();

    return;
  }

  score += 1;

  if(score > maxScore){
    score = maxScore;
  }
}

void Team::subtractScore() {
  if (timer->isPaused()) {
    timer->resume();

    return;
  }

  if (score > 0) {
    score -= 1;
  }
}

void Team::resetScore() {
  score = 0;
}