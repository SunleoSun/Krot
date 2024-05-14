#include <Arduino.h>
#define MIN_SILENCE_DURATION 100
#define MAX_SILENCE_DURATION 5000
#define MIN_SOUND_DURATION 1000
#define MAX_SOUND_DURATION 10000
#define ELAPSED(time, dur) ((millis() - time > dur) || (millis() < time))
#define GET_RANDOM_CHANNEL random(0, 9)
#define KROT_DISABLE_PIN D3
#define MOUSE_DISABLE_PIN D8

typedef uint8_t ui8;
typedef uint16_t ui16;
typedef unsigned long ul;

bool isKrotSilence = false;
bool isMouseSilence = false;

struct PinIndex
{
  ui8 pinBit;
  ui8 pin;
};
const PinIndex pinMatrixKrotFreq[] = {{1, D0}, {2, D1}, {4, D2}};
const PinIndex pinMatrixMouseFreq[] = {{1, D5}, {2, D6}, {4, D7}};

ul krotTimer = 0;
ul mouseTimer = 0;
ui16 krotDuration;
ui16 mouseDuration;

void OpenChannel(ui8 ch, const PinIndex pinMatrix[]);
void updateKrotDuration();
void updateMouseDuration();

void setup()
{
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);
  pinMode(KROT_DISABLE_PIN, OUTPUT);
  digitalWrite(KROT_DISABLE_PIN, LOW);
  pinMode(D5, OUTPUT);
  digitalWrite(D5, LOW);
  pinMode(D6, OUTPUT);
  digitalWrite(D6, LOW);
  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);
  pinMode(MOUSE_DISABLE_PIN, OUTPUT);
  digitalWrite(MOUSE_DISABLE_PIN, LOW);
  randomSeed(analogRead(A0));
  updateKrotDuration();
  updateMouseDuration();
  OpenChannel(GET_RANDOM_CHANNEL, pinMatrixKrotFreq);
  OpenChannel(GET_RANDOM_CHANNEL, pinMatrixMouseFreq);
}

void loop() {
  if (ELAPSED(krotTimer, krotDuration))
  {
    updateKrotDuration();
    if (isKrotSilence)
    {
      digitalWrite(KROT_DISABLE_PIN, HIGH);
    }
    else
    {
      digitalWrite(KROT_DISABLE_PIN, LOW);
      OpenChannel(GET_RANDOM_CHANNEL, pinMatrixKrotFreq);
    }
    isKrotSilence = !isKrotSilence;
  }
  if (ELAPSED(mouseTimer, mouseDuration))
  {
    updateMouseDuration();
    if (isMouseSilence)
    {
      digitalWrite(MOUSE_DISABLE_PIN, HIGH);
    }
    else
    {
      digitalWrite(MOUSE_DISABLE_PIN, LOW);
      OpenChannel(GET_RANDOM_CHANNEL, pinMatrixMouseFreq);
    }
    isMouseSilence = !isMouseSilence;
  }
}

void updateMouseDuration()
{
  mouseTimer = millis();
  mouseDuration = isMouseSilence ? random(MIN_SILENCE_DURATION, MAX_SILENCE_DURATION) : random(MIN_SOUND_DURATION, MAX_SOUND_DURATION);
}

void updateKrotDuration()
{
  krotTimer = millis();
  krotDuration = isKrotSilence ? random(MIN_SILENCE_DURATION, MAX_SILENCE_DURATION) : random(MIN_SOUND_DURATION, MAX_SOUND_DURATION);
}

void OpenChannel(ui8 ch, const PinIndex pinMatrix[])
{
  for (uint i = 0; i < 3; i++)
  {
    if ((ch & pinMatrix[i].pinBit) > 0)
    {
      // Serial.println("To HIGHT pin " + String(pinMatrix[i].pinBit));
      digitalWrite(pinMatrix[i].pin, HIGH);
    }
    else
    {
      // Serial.println("To LOW pin " + String(pinMatrix[i].pinBit));
      digitalWrite(pinMatrix[i].pin, LOW);
    }
  }
}
