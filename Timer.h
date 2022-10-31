#include <stdint.h>

class Timer {
private:
  const long secondInMillis = 1000;
  const uint8_t maxMinutes = UINT8_MAX;

  uint8_t minutesPrev10th = 0;
  uint8_t secondsPrev10th = 0;

  uint8_t minutesList[3] = { 0, 0, 0 };
  uint8_t secondsList[2] = { 0, 0 };

  bool paused = false;

public:
  void reset();
  void advance(long currentMillis, long previousMillis);
  void resume();
  void pause();
  bool isPaused();

  uint8_t *getSecondsList();
  uint8_t *getMinutesList();
  
  int getMinutes();
  int getSeconds();
  
  int getPrevious10Seconds();
  int getPrevious10Minutes();
};