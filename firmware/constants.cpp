#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "constants.h"
namespace constants {
  // Communications parameters
  const int baudrate = 9600;

  // Device parameters
  const int modelNumber = 2117;
  const int serialNumberMin = 0;
  const int serialNumberMax = 255;
  const int firmwareNumber = 1;

  // Pin assignment
  const int relayDriverCsPin = 49;
  const int relayDriverInPin = 48;
  const int digitalOutputPins[digitalOutputCount] = {};
  const int digitalInputPins[digitalInputCount] = {};
  const int analogInputPins[analogInputCount] = {0,1};
  const int analogInputPinAliases[analogInputCount] = {A0,A1};

  // Blink parameters
  const int dutyCycleMin = 0;
  const int dutyCycleMax = 100;

  // Mfc parameters
  const int percentCapacityMin = 0;
  const int percentCapacityMax = 100;
  const int setpointMin = 0;
  const int setpointMax = 32000;

  // Analog input parameters
  const int analogValueMin = 0;
  const int analogValueMax = 1023;

  // Analog input map
  const int mapRelayOnAnalogInputIndex = 0;
  const int mapMfcFlowRateAnalogInputIndex = 1;
  const int mapMfcFlowRateMfcIndex = 0;
  const int mappingEnabledDefault = false;
  const int mapPeriod = 1000;
  // map 0-5V to -1 to 9
  // no relays on : 0.25V
  // relay 0 on   : 0.75V
  // relay 1 on   : 1.25V
  // relay 2 on   : 1.75V
  // relay 3 on   : 2.25V
  // relay 4 on   : 2.75V
  // relay 5 on   : 3.25V
  // relay 6 on   : 3.75V
  // relay 7 on   : 4.25V
  const int mapRelayOnIndexMin = -1;
  const int mapRelayOnIndexMax = relayCount+1;

  // Display
  const int displayPeriod = 1000;
}
