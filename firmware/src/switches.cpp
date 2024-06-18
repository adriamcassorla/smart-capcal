#include "switches.h"
#include "avdweb_Switch.h"
#include "lights.h"

/////
// MultiSwitch Implementation
/////
MultiSwitch::MultiSwitch(
    uint8_t *pinNumbers, ReadingLight *light, DemoLights::Mode mode
)
    : readingLight(light), demoMode(mode) {
  for (uint8_t i = 0; i < NUM_SWITCHES; ++i) {
    toggleSwitches[i] = new Switch(pinNumbers[i]);
  }
}

int MultiSwitch::pinIds[NUM_SWITCHES] = {0, 1, 2};
void MultiSwitch::setup() {
  for (uint8_t i = 0; i < NUM_SWITCHES; ++i) {
    MultiSwitchCallbackData *callbackData =
        new MultiSwitchCallbackData{this, pinIds[i]};
    toggleSwitches[i]->setPushedCallback(&callback, (void *)callbackData);
    toggleSwitches[i]->setReleasedCallback(&callback, (void *)callbackData);
  }
}

void MultiSwitch::poll() {
  for (uint8_t i = 0; i < NUM_SWITCHES; ++i) {
    toggleSwitches[i]->poll();
  }
}

void MultiSwitch::callback(void *callbackData) {
  // Cast the callbackData to MultiSwitchCallbackData
  MultiSwitchCallbackData *data =
      static_cast<MultiSwitchCallbackData *>(callbackData);

  // Access own instance of MultiSwitch and the pinId
  MultiSwitch *multiSwitch = data->instance;
  int pinId = data->pinId;

  // Check if multiSwitch is valid before using it
  if (!multiSwitch) { return; }

  // Stop demo if isOn and another switch has been selected
  if (demoLights.isOn && pinId != 2) {
    demoLights.stop();
  }
  // Reset other lights when demo mode is selected
  else if (pinId == 2) {
    ambientLight.reset();
    readingRight.reset();
    readingLeft.reset();
  }

  switch (pinId) {
  case 0:
    multiSwitch->readingLight->toggle();
    break;
  case 1:
    ambientLight.toggle();
    break;
  case 2:
    demoLights.setMode(multiSwitch->demoMode);
    demoLights.toggle();
    break;
  default:
    break;
  }
}

/////////
// SETUP
/////////

uint8_t leftPins[] = {27, 31, 25};
uint8_t rightPins[] = {28, 29, 30};
MultiSwitch leftSwitches(leftPins, &readingLeft, DemoLights::Mode::Rainbow);
MultiSwitch
    rightSwitches(rightPins, &readingRight, DemoLights::Mode::Chromotherapy);

void switchesSetup() {
  leftSwitches.setup();
  rightSwitches.setup();
}

////////
// LOOP
////////
void switchesLoop() {
  leftSwitches.poll();
  rightSwitches.poll();
}
