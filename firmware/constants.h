#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

namespace constants {
  enum {relayCount=8};
  enum {mfcCount=2};
  enum {digitalOutputCount=0};
  enum {digitalInputCount=0};
  enum {analogInputCount=2};

  // Communications parameters
  extern const int baudrate;

  // Device parameters
  extern const int modelNumber;
  extern const int serialNumberMin;
  extern const int serialNumberMax;
  extern const int firmwareNumber;

  // Pin assignment
  extern const int relayDriverCsPin;
  extern const int relayDriverInPin;
  extern const int digitalOutputPins[digitalOutputCount];
  extern const int digitalInputPins[digitalInputCount];
  extern const int analogInputPins[analogInputCount];
  extern const int analogInputPinAliases[analogInputCount];

  // Blink parameters
  extern const int dutyCycleMin;
  extern const int dutyCycleMax;

  // Mfc parameters
  extern const int percentCapacityMin;
  extern const int percentCapacityMax;
  extern const int setpointMin;
  extern const int setpointMax;

  // Analog input parameters
  extern const int analogValueMin;
  extern const int analogValueMax;

  // Analog input map
  extern const int mapRelayOnAnalogInputIndex;
  extern const int mapMfcFlowRateAnalogInputIndex;
  extern const int mapMfcFlowRateMfcIndex;
  extern const int mappingEnabledDefault;
  extern const int mapPeriod;
  extern const int mapRelayOnIndexMin;
  extern const int mapRelayOnIndexMax;

  // Display
  extern const int displayPeriod;
}
#endif
