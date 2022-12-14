#include <stdio.h>
#include <stdlib.h>

#include "Timer.h"

void Timer::reset() {
  paused = true;

  minutesList[0] = 0;
  minutesList[1] = 0;
  minutesList[2] = 0;

  secondsList[0] = 0;
  secondsList[1] = 0;
}

void Timer::advance(long currentMillis, long previousMillis) {
  bool shouldEnd = false;

  if (paused) {
    return;
  }

  if (currentMillis - previousMillis >= secondInMillis) {
    int seconds = getSeconds();
    int minutes = getMinutes();

    if (seconds == 59) {
      seconds = 0;
      minutes++;
    } else {
      seconds++;
    }

    int secondsIndex = sizeof(secondsList) / sizeof(int8_t) - 1;
    int minutesIndex = sizeof(minutesList) / sizeof(int8_t) - 1;

    if (seconds == 0) {
      secondsList[0] = 0;
      secondsList[1] = 0;
    } else {
      while (seconds) {
        secondsList[secondsIndex--] = seconds % 10;
        seconds /= 10;
      }
    }

    if (minutes >= maxMinutes) {
      minutes = maxMinutes;
      shouldEnd = true;
    }

    while (minutes) {
      minutesList[minutesIndex--] = minutes % 10;
      minutes /= 10;
    }

    if (shouldEnd) {
      secondsList[0] = 5;
      secondsList[1] = 9;

      //we've reached the end.
      pause();
    }
  }
}

void Timer::resume() {
  paused = false;
}

void Timer::pause() {
  paused = true;
}

bool Timer::isPaused() {
  return paused;
}

uint8_t *Timer::getSecondsList() {
  return secondsList;
}

uint8_t *Timer::getMinutesList() {
  return minutesList;
}

int Timer::getMinutes() {
  char minutes[4];

  sprintf(&minutes[0], "%d", minutesList[0]);
  sprintf(&minutes[1], "%d", minutesList[1]);
  sprintf(&minutes[2], "%d", minutesList[2]);

  return atoi(minutes);
}

int Timer::getSeconds() {
  char seconds[3];

  sprintf(&seconds[0], "%d", secondsList[0]);
  sprintf(&seconds[1], "%d", secondsList[1]);

  return atoi(seconds);
}