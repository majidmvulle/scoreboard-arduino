#include "Timer.h"

class Team {

private:
  Timer* timer;

  int8_t score;
  char* name;
  const int8_t maxScore = 99;

public:
  Team(char* aName, Timer* timer);

  char* getName();
  int8_t getScore();

  void addScore();
  void subtractScore();
  void resetScore();
};