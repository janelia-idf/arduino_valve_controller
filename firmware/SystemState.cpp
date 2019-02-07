#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "string.h"
#include "SystemState.h"

SystemState::SystemState() {
  setErrMsg("");
}

void SystemState::setErrMsg(char *msg) {
  strncpy(errMsg,msg,SYS_ERR_BUF_SZ);
}

void SystemState::initialize() {
  relayDriver = TLE7232G(constants::relayDriverCsPin,constants::relayDriverInPin);

  for (int mfcIndex=0; mfcIndex<constants::mfcCount; mfcIndex++) {
    mfc[mfcIndex] = Bronkhorst(mfcIndex+1);
    mfc[mfcIndex].init();
  }

  // Initialize SPI:
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();

  relayDriver.init(1);
  setAllRelaysOff();

  tts.init();

  for (int relay; relay < constants::relayCount; relay++) {
    relayBlinkTasks[relay].onTaskId = -1;
    relayBlinkTasks[relay].offTaskId = -1;
  }

  for (int digital_output; digital_output < constants::digitalOutputCount; digital_output++) {
    pinMode(constants::digitalOutputPins[digital_output], OUTPUT);
    digitalWrite(constants::digitalOutputPins[digital_output], LOW);
    digitalOutputStates[digital_output] = LOW;
  }

  for (int digital_input; digital_input < constants::digitalInputCount; digital_input++) {
    pinMode(constants::digitalInputPins[digital_input], INPUT);
    digitalWrite(constants::digitalInputPins[digital_input], HIGH);
  }

  for (int analog_input; analog_input < constants::analogInputCount; analog_input++) {
    pinMode(constants::analogInputPinAliases[analog_input], INPUT);
    digitalWrite(constants::analogInputPinAliases[analog_input], LOW);
  }

  // Display
  display = NewhavenDisplay();
  initializeDisplay();

  if (constants::mappingEnabledDefault) {
    startAnalogInputsMapping();
  }
}

bool SystemState::setRelays(uint32_t relays) {
  relayDriver.setChannels(relays);
  return true;
}

bool SystemState::setRelayOn(int relay) {
  relayDriver.setChannelOn(relay);
  return true;
}

bool SystemState::setRelayOff(int relay) {
  relayDriver.setChannelOff(relay);
  return true;
}

bool SystemState::setAllRelaysOn() {
  uint32_t relays = 0;
  relays = ~relays;
  setRelays(relays);
  return true;
}

bool SystemState::setAllRelaysOff() {
  uint32_t relays = 0;
  setRelays(relays);
  return true;
}

bool SystemState::setMfcFlowRate(int mfcIndex, int percent_capacity) {
  int setpoint = map(percent_capacity,constants::percentCapacityMin,constants::percentCapacityMax,constants::setpointMin,constants::setpointMax);
  return mfc[mfcIndex].setSetpoint(setpoint);
}

bool SystemState::getMfcFlowRateSetting(int mfcIndex, int &percent_capacity) {
  int setpoint = mfc[mfcIndex].getSetpoint();
  percent_capacity = map(setpoint,constants::setpointMin,constants::setpointMax,constants::percentCapacityMin,constants::percentCapacityMax);
  return true;
}

bool SystemState::getMfcFlowRateMeasure(int mfcIndex, int &percent_capacity) {
  int measure = mfc[mfcIndex].getMeasure();
  percent_capacity = map(measure,constants::setpointMin,constants::setpointMax,constants::percentCapacityMin,constants::percentCapacityMax);
  return true;
}

bool SystemState::getMfcSerialNumber(int mfcIndex, String &serial_number) {
  return mfc[mfcIndex].getSerialNumber(serial_number);
}

bool SystemState::getMfcCapacity(int mfcIndex, int &capacity) {
  capacity = (int)mfc[mfcIndex].getCapacity();
  return true;
}

bool SystemState::getMfcCapacityUnits(int mfcIndex, String &capacity_units) {
  return mfc[mfcIndex].getCapacityUnits(capacity_units);
}

bool SystemState::getMfcFluidName(int mfcIndex, String &fluid_name) {
  return mfc[mfcIndex].getFluidName(fluid_name);
}

bool SystemState::startRelayBlink(int relay, long period, int duty_cycle, long count=-1) {
  stopRelayBlink(relay);
  if ((0 < duty_cycle) && (duty_cycle < 100)) {
    int onTaskId = (int)tts.addTaskUsingDelay(100,inlineSetRelayOn,relay,period,count,false);
    int offTaskId = (int)tts.addTaskUsingOffset((uint8_t)onTaskId,(period*duty_cycle)/100,inlineSetRelayOff,relay,period,count,false);

    // Only save continuous blink patterns. Let patterns with count
    // expire naturally. This means multiple blink patterns could
    // overlap until count expires and that finite blink patterns
    // cannot be stopped unless all tasks are removed.

    if (count < 0) {
      relayBlinkTasks[relay].onTaskId = onTaskId;
      relayBlinkTasks[relay].offTaskId = offTaskId;
    }
  } else if (100 <= duty_cycle) {
    setRelayOn(relay);
  }
  return true;
}

bool SystemState::stopRelayBlink(int relay) {
  int onTaskId = relayBlinkTasks[relay].onTaskId;
  int offTaskId = relayBlinkTasks[relay].offTaskId;
  if (0 <= onTaskId) {
    tts.removeTask((uint8_t)onTaskId);
    relayBlinkTasks[relay].onTaskId = -1;
  }
  if (0 <= offTaskId) {
    tts.removeTask((uint8_t)offTaskId);
    relayBlinkTasks[relay].offTaskId = -1;
  }
  setRelayOff(relay);
  return true;
}

bool SystemState::stopAllRelaysBlink() {
  for (uint8_t relay; relay < constants::relayCount; relay++) {
    stopRelayBlink(relay);
  }
  return true;
}

bool SystemState::addPulseCentered(int relay, long delay, long duration) {
  long rising = delay - duration/2;
  if (rising < 0) {
    rising = 0;
  }
  int onTaskId = (int)tts.addTaskUsingDelay(rising,inlineSetRelayOn,relay,-1,-1,false);
  tts.addTaskUsingOffset((uint8_t)onTaskId,duration,inlineSetRelayOff,relay,-1,-1,false);
  return true;
}

bool SystemState::setDigitalOutputHigh(int digital_output) {
  digitalWrite(constants::digitalOutputPins[digital_output], HIGH);
  digitalOutputStates[digital_output] = HIGH;
  return true;
}

bool SystemState::setDigitalOutputLow(int digital_output) {
  digitalWrite(constants::digitalOutputPins[digital_output], LOW);
  digitalOutputStates[digital_output] = LOW;
  return true;
}

bool SystemState::toggleDigitalOutput(int digital_output) {
  if (digitalOutputStates[digital_output] == LOW) {
    setDigitalOutputHigh(digital_output);
  } else {
    setDigitalOutputLow(digital_output);
  }
  return true;
}

bool SystemState::pulseDigitalOutput(int digital_output, long duration) {
  int onTaskId = (int)tts.addTaskUsingDelay(0,inlineSetDigitalOutputHigh,digital_output,-1,-1,false);
  tts.addTaskUsingOffset((uint8_t)onTaskId,duration,inlineSetDigitalOutputLow,digital_output,-1,-1,false);
  return true;
}

int SystemState::getDigitalInput(int digital_input) {
  return digitalRead(constants::digitalInputPins[digital_input]);
}

int SystemState::getAnalogInput(int analog_input) {
  return analogRead(constants::analogInputPins[analog_input]);
}

bool SystemState::removeAllTasks() {
  return tts.removeAllTasks();
}

uint32_t SystemState::getTime() {
  return tts.ms();
}

bool SystemState::setTime(uint32_t time=0) {
  tts.setTime(time);
  return true;
}

bool SystemState::getTaskDetails(uint8_t taskId, uint32_t& time, int& arg, long& period, long& count, bool& free, bool& enabled) {
  tts.getTaskDetails(taskId, time, arg, period, count, free, enabled);
  return true;
}

bool SystemState::mapAnalogInputToRelays() {
  int analogValue = getAnalogInput(constants::mapRelayOnAnalogInputIndex);
  int relayOn = map(analogValue,
                    constants::analogValueMin,
                    constants::analogValueMax,
                    constants::mapRelayOnIndexMin,
                    constants::mapRelayOnIndexMax);
  if (relayOn != this->relayOn) {
    this->relayOn = relayOn;
    if (relayOn < 0) {
      setAllRelaysOff();
      return true;
    } else if (relayOn > (constants::relayCount - 1)) {
      relayOn = constants::relayCount - 1;
    };
    uint32_t relays = 1 << relayOn;
    setRelays(relays);
  };
  return true;
}

bool SystemState::mapAnalogInputToMfcFlowRate() {
  int analogValue = getAnalogInput(constants::mapMfcFlowRateAnalogInputIndex);
  int percentCapacity = map(analogValue,
                            constants::analogValueMin,
                            constants::analogValueMax,
                            constants::percentCapacityMin,
                            constants::percentCapacityMax);
  if (percentCapacity != this->percentCapacity) {
    this->percentCapacity = percentCapacity;
    setMfcFlowRate(constants::mapMfcFlowRateMfcIndex,percentCapacity);
  };
  return true;
}

bool SystemState::mapAnalogInputs(int dummy_arg) {
  mapAnalogInputToRelays();
  mapAnalogInputToMfcFlowRate();
  return true;
}

bool SystemState::startAnalogInputsMapping() {
  stopAllRelaysBlink();
  this->relayOn = -1;
  this->percentCapacity = 0;
  this->mapAnalogInputsTaskId = (int)tts.addTaskUsingDelay(1000,
                                                           inlineMapAnalogInputs,
                                                           0,
                                                           constants::mapPeriod,
                                                           -1,
                                                           false);
  this->mappingEnabled = true;
  return true;
}

bool SystemState::stopAnalogInputsMapping() {
  if (mappingEnabled) {
    tts.removeTask((uint8_t)mapAnalogInputsTaskId);
    mappingEnabled = false;
  }
  return true;
}

void SystemState::updateDisplay() {
  if (displayEnabled) {
    unsigned long time_now = millis();
    if ((time_now - time_last_display_update) > constants::displayPeriod) {
      time_last_display_update = time_now;

      uint32_t relaysOn = relayDriver.getChannelsOn();
      for (int relay=0; relay < constants::relayCount; relay++) {
        display.setCursor(1,relay*2 + 5);
        if (relaysOn & 1<<relay) {
          display.print("1");
        } else {
          display.print("0");
        }
      }

      int percent_capacity;
      display.setCursor(2,14);
      display.print("   ");
      display.setCursor(2,14);
      getMfcFlowRateSetting(constants::mapRelayOnAnalogInputIndex,percent_capacity);
      display.print(String(percent_capacity));
      display.setCursor(3,14);
      display.print("   ");
      display.setCursor(3,14);
      getMfcFlowRateMeasure(constants::mapRelayOnAnalogInputIndex,percent_capacity);
      display.print(String(percent_capacity));
    }
  }
}

void SystemState::initializeDisplay() {
  display.init();
  display.clearScreen();
  delay(100);
  display.print("Relay0 1 2 3 4 5 6 7");
  display.setCursor(2,0);
  display.print("Mfc0 Setting:");
  display.setCursor(3,0);
  display.print("Mfc0 Measure:");
  time_last_display_update = millis();
  displayEnabled = true;
}

SystemState systemState;


