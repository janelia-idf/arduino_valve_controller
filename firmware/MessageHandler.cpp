#include <util/atomic.h>
#include "MessageHandler.h"
#include "Streaming.h"
#include "SystemState.h"

enum {
  cmdGetDevInfo,                    // Done
  cmdGetCmds,                       // Done
  cmdGetRspCodes,                   // Done
  cmdSetSerialNumber,               //

  cmdSetRelays,                 //
  cmdSetRelayOn,                //
  cmdSetRelayOff,               //
  cmdSetAllRelaysOn,            //
  cmdSetAllRelaysOff,           //

  cmdSetMfcFlowRate,                //
  cmdGetMfcFlowRateSetting,         //
  cmdGetMfcFlowRateMeasure,         //
  cmdGetMfcSerialNumber,            //
  cmdGetMfcCapacity,                //
  cmdGetMfcCapacityUnits,           //
  cmdGetMfcFluidName,               //

  cmdStartRelayBlink,           //
  cmdStopRelayBlink,            //
  cmdStopAllRelaysBlink,        //

  cmdAddPulseCentered,        //

  cmdSetDigitalOutputHigh,        //
  cmdSetDigitalOutputLow,        //
  cmdToggleDigitalOutput,        //
  cmdPulseDigitalOutput,        //

  cmdGetDigitalInput,        //
  cmdGetAnalogInput,        //

  cmdRemoveAllTasks,            //

  cmdGetTime,           //
  cmdSetTime,           //

  cmdGetTaskDetails,           //

  cmdStartAnalogInputsMapping,
  cmdStopAnalogInputsMapping,

  // DEVELOPMENT
  cmdDebug,
};


const int rspSuccess = 1;
const int rspError = 0;

void MessageHandler::processMsg() {
  while (Serial.available() > 0) {
    process(Serial.read());
    if (messageReady()) {
      msgSwitchYard();
      reset();
    }
  }
  return;
}

void MessageHandler::msgSwitchYard() {
  int cmd = readInt(0);
  dprint.start();
  dprint.addIntItem("cmd_id", cmd);

  switch (cmd) {

  case cmdGetDevInfo:
    handleGetDevInfo();
    break;

  case cmdGetCmds:
    handleGetCmds();
    break;

  case cmdGetRspCodes:
    handleGetRspCodes();
    break;

  case cmdSetSerialNumber:
    handleSetSerialNumber();
    break;

  case cmdSetRelays:
    handleSetRelays();
    break;

  case cmdSetRelayOn:
    handleSetRelayOn();
    break;

  case cmdSetRelayOff:
    handleSetRelayOff();
    break;

  case cmdSetAllRelaysOn:
    handleSetAllRelaysOn();
    break;

  case cmdSetAllRelaysOff:
    handleSetAllRelaysOff();
    break;

  case cmdSetMfcFlowRate:
    handleSetMfcFlowRate();
    break;

  case cmdGetMfcFlowRateSetting:
    handleGetMfcFlowRateSetting();
    break;

  case cmdGetMfcFlowRateMeasure:
    handleGetMfcFlowRateMeasure();
    break;

  case cmdGetMfcSerialNumber:
    handleGetMfcSerialNumber();
    break;

  case cmdGetMfcCapacity:
    handleGetMfcCapacity();
    break;

  case cmdGetMfcCapacityUnits:
    handleGetMfcCapacityUnits();
    break;

  case cmdGetMfcFluidName:
    handleGetMfcFluidName();
    break;

  case cmdStartRelayBlink:
    handleStartRelayBlink();
    break;

  case cmdStopRelayBlink:
    handleStopRelayBlink();
    break;

  case cmdStopAllRelaysBlink:
    handleStopAllRelaysBlink();
    break;

  case cmdAddPulseCentered:
    handleAddPulseCentered();
    break;

  case cmdSetDigitalOutputHigh:
    handleSetDigitalOutputHigh();
    break;

  case cmdSetDigitalOutputLow:
    handleSetDigitalOutputLow();
    break;

  case cmdToggleDigitalOutput:
    handleToggleDigitalOutput();
    break;

  case cmdPulseDigitalOutput:
    handlePulseDigitalOutput();
    break;

  case cmdGetDigitalInput:
    handleGetDigitalInput();
    break;

  case cmdGetAnalogInput:
    handleGetAnalogInput();
    break;

  case cmdRemoveAllTasks:
    handleRemoveAllTasks();
    break;

  case cmdGetTime:
    handleGetTime();
    break;

  case cmdSetTime:
    handleSetTime();
    break;

  case cmdGetTaskDetails:
    handleGetTaskDetails();
    break;

  case cmdStartAnalogInputsMapping:
    handleStartAnalogInputsMapping();
    break;

  case cmdStopAnalogInputsMapping:
    handleStopAnalogInputsMapping();
    break;

  // DEVELOPMENT
  case cmdDebug:
    handleDebug();
    break;

  default:
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "unknown command");
    break;
  }
  dprint.stop();
}

void MessageHandler::handleGetCmds() {
  dprint.addIntItem("status", rspSuccess);
  dprint.addIntItem("getDevInfo", cmdGetDevInfo);
  dprint.addIntItem("getCmds", cmdGetCmds);
  dprint.addIntItem("getRspCodes", cmdGetRspCodes);
  dprint.addIntItem("setArduinoSerialNumber", cmdSetSerialNumber);

  dprint.addIntItem("setRelays", cmdSetRelays);
  dprint.addIntItem("setRelayOn", cmdSetRelayOn);
  dprint.addIntItem("setRelayOff", cmdSetRelayOff);
  dprint.addIntItem("setAllRelaysOn", cmdSetAllRelaysOn);
  dprint.addIntItem("setAllRelaysOff", cmdSetAllRelaysOff);

  dprint.addIntItem("setMfcFlowRate", cmdSetMfcFlowRate);
  dprint.addIntItem("getMfcFlowRateSetting", cmdGetMfcFlowRateSetting);
  dprint.addIntItem("getMfcFlowRateMeasure", cmdGetMfcFlowRateMeasure);
  dprint.addIntItem("getMfcSerialNumber", cmdGetMfcSerialNumber);
  dprint.addIntItem("getMfcCapacity", cmdGetMfcCapacity);
  dprint.addIntItem("getMfcCapacityUnits", cmdGetMfcCapacityUnits);
  dprint.addIntItem("getMfcFluidName", cmdGetMfcFluidName);

  dprint.addIntItem("startRelayBlink", cmdStartRelayBlink);
  dprint.addIntItem("stopRelayBlink", cmdStopRelayBlink);
  dprint.addIntItem("stopAllRelaysBlink", cmdStopAllRelaysBlink);

  dprint.addIntItem("addPulseCentered", cmdAddPulseCentered);

  dprint.addIntItem("setDigitalOutputHigh", cmdSetDigitalOutputHigh);
  dprint.addIntItem("setDigitalOutputLow", cmdSetDigitalOutputLow);
  dprint.addIntItem("toggleDigitalOutput", cmdToggleDigitalOutput);
  dprint.addIntItem("pulseDigitalOutput", cmdPulseDigitalOutput);

  dprint.addIntItem("getDigitalInput", cmdGetDigitalInput);
  dprint.addIntItem("getAnalogInput", cmdGetAnalogInput);

  dprint.addIntItem("removeAllTasks", cmdRemoveAllTasks);

  dprint.addIntItem("getTime", cmdGetTime);
  dprint.addIntItem("setTime", cmdSetTime);

  dprint.addIntItem("getTaskDetails", cmdGetTaskDetails);

  dprint.addIntItem("startAnalogInputsMapping", cmdStartAnalogInputsMapping);
  dprint.addIntItem("stopAnalogInputsMapping", cmdStopAnalogInputsMapping);

  // DEVELOPMENT
  dprint.addIntItem("cmdDebug", cmdDebug);
}

void MessageHandler::handleGetDevInfo() {
  dprint.addIntItem("status", rspSuccess);
  dprint.addIntItem("model_number",  constants::modelNumber);
  dprint.addIntItem("serial_number", savedVariables.getSerialNumber());
  dprint.addIntItem("firmware_number", constants::firmwareNumber);
  dprint.addIntItem("relay_count",  constants::relayCount);
  dprint.addIntItem("mfc_count",  constants::mfcCount);
}

bool MessageHandler::checkNumberOfArgs(int num) {
  bool flag = true;
  if (numberOfItems() != num) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "incorrect number of arguments");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkSerialNumberArg(int serial_number) {
  bool flag = true;
  if ((serial_number<constants::serialNumberMin) || (constants::serialNumberMax<serial_number)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "serial number argument out of range");
    flag = false;
  }
  return flag;
}

void MessageHandler::systemCmdRsp(bool flag) {
  if (flag) {
    dprint.addIntItem("status", rspSuccess);
  }
  else {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", systemState.errMsg);
  }
}

void MessageHandler::handleGetRspCodes() {
  dprint.addIntItem("status", rspSuccess);
  dprint.addIntItem("rsp_success",rspSuccess);
  dprint.addIntItem("rsp_error", rspError);
}

void MessageHandler::handleSetSerialNumber() {
  if (!checkNumberOfArgs(2)) {return;}
  int serialNumber = readInt(1);
  if (checkSerialNumberArg(serialNumber)) {
    systemCmdRsp(savedVariables.setSerialNumber(serialNumber));
  }
}

bool MessageHandler::checkRelayArg(int relay) {
  bool flag = true;
  if ((relay<0) || (constants::relayCount<=relay)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "relay argument out of range");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkPeriodArg(uint32_t period) {
  bool flag = true;
  // if ((relay<0) || (constants::relayCount<=relay)) {
  //   dprint.addIntItem("status", rspError);
  //   dprint.addStrItem("err_msg", "period argument out of range");
  //   flag = false;
  // }
  return flag;
}

bool MessageHandler::checkDutyCycleArg(int duty_cycle) {
  bool flag = true;
  if ((duty_cycle<constants::dutyCycleMin) || (constants::dutyCycleMax<duty_cycle)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "period argument out of range");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkDigitalOutputArg(int digital_output) {
  bool flag = true;
  if ((digital_output<0) || (constants::digitalOutputCount<=digital_output)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "digital_output argument out of range");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkDigitalInputArg(int digital_input) {
  bool flag = true;
  if ((digital_input<0) || (constants::digitalInputCount<=digital_input)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "digital_input argument out of range");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkAnalogInputArg(int analog_input) {
  bool flag = true;
  if ((analog_input<0) || (constants::analogInputCount<=analog_input)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "analog_input argument out of range");
    flag = false;
  }
  return flag;
}

void MessageHandler::handleSetRelays() {
  if (!checkNumberOfArgs(2)) {return;}
  uint32_t relays = (uint32_t)readLong(1);
  systemCmdRsp(systemState.setRelays(relays));
}

void MessageHandler::handleSetRelayOn() {
  if (!checkNumberOfArgs(2)) {return;}
  int relay = readInt(1);
  if (checkRelayArg(relay)) {
    systemCmdRsp(systemState.setRelayOn(relay));
  }
}

void MessageHandler::handleSetRelayOff() {
  if (!checkNumberOfArgs(2)) {return;}
  int relay = readInt(1);
  if (checkRelayArg(relay)) {
    systemCmdRsp(systemState.setRelayOff(relay));
  }
}

void MessageHandler::handleSetAllRelaysOn() {
  systemCmdRsp(systemState.setAllRelaysOn());
}

void MessageHandler::handleSetAllRelaysOff() {
  systemCmdRsp(systemState.setAllRelaysOff());
}

bool MessageHandler::checkMfcArg(int mfc) {
  bool flag = true;
  if ((mfc<0) || (constants::mfcCount<=mfc)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "mfc argument out of range");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkPercentCapacityArg(int percent_capacity) {
  bool flag = true;
  if ((percent_capacity<constants::percentCapacityMin) || (constants::percentCapacityMax<percent_capacity)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "percent_capacity argument out of range");
    flag = false;
  }
  return flag;
}

void MessageHandler::handleSetMfcFlowRate() {
  if (!checkNumberOfArgs(3)) {return;}
  int mfc = readInt(1);
  int percent_capacity = readInt(2);
  if (checkMfcArg(mfc) && checkPercentCapacityArg(percent_capacity)) {
    systemCmdRsp(systemState.setMfcFlowRate(mfc,percent_capacity));
  }
}

void MessageHandler::handleGetMfcFlowRateSetting() {
  if (!checkNumberOfArgs(2)) {return;}
  int mfc = readInt(1);
  if (checkMfcArg(mfc)) {
    int percent_capacity;
    if (systemState.getMfcFlowRateSetting(mfc,percent_capacity)) {
      dprint.addIntItem("status", rspSuccess);
      dprint.addIntItem("percent_capacity", percent_capacity);
    } else {
      dprint.addIntItem("status", rspError);
      dprint.addStrItem("err_msg", "unable to get mfc flow rate setting");
    }
  }
}

void MessageHandler::handleGetMfcFlowRateMeasure() {
  if (!checkNumberOfArgs(2)) {return;}
  int mfc = readInt(1);
  if (checkMfcArg(mfc)) {
    int percent_capacity;
    if (systemState.getMfcFlowRateMeasure(mfc,percent_capacity)) {
      dprint.addIntItem("status", rspSuccess);
      dprint.addIntItem("percent_capacity", percent_capacity);
    } else {
      dprint.addIntItem("status", rspError);
      dprint.addStrItem("err_msg", "unable to get mfc flow rate measure");
    }
  }
}

void MessageHandler::handleGetMfcSerialNumber() {
  if (!checkNumberOfArgs(2)) {return;}
  int mfc = readInt(1);
  if (checkMfcArg(mfc)) {
    String mfc_serial_number;
    if (systemState.getMfcSerialNumber(mfc,mfc_serial_number)) {
      dprint.addIntItem("status", rspSuccess);
      dprint.addStrItem("mfc_serial_number", mfc_serial_number);
    } else {
      dprint.addIntItem("status", rspError);
      dprint.addStrItem("err_msg", "unable to get mfc serial number");
    }
  }
}

void MessageHandler::handleGetMfcCapacity() {
  if (!checkNumberOfArgs(2)) {return;}
  int mfc = readInt(1);
  if (checkMfcArg(mfc)) {
    int capacity;
    if (systemState.getMfcCapacity(mfc,capacity)) {
      dprint.addIntItem("status", rspSuccess);
      dprint.addIntItem("capacity", capacity);
    } else {
      dprint.addIntItem("status", rspError);
      dprint.addStrItem("err_msg", "unable to get mfc capacity");
    }
  }
}

void MessageHandler::handleGetMfcCapacityUnits() {
  if (!checkNumberOfArgs(2)) {return;}
  int mfc = readInt(1);
  if (checkMfcArg(mfc)) {
    String capacity_units;
    if (systemState.getMfcCapacityUnits(mfc,capacity_units)) {
      dprint.addIntItem("status", rspSuccess);
      dprint.addStrItem("capacity_units", capacity_units);
    } else {
      dprint.addIntItem("status", rspError);
      dprint.addStrItem("err_msg", "unable to get mfc capacity units");
    }
  }
}

void MessageHandler::handleGetMfcFluidName() {
  if (!checkNumberOfArgs(2)) {return;}
  int mfc = readInt(1);
  if (checkMfcArg(mfc)) {
    String fluid_name;
    if (systemState.getMfcFluidName(mfc,fluid_name)) {
      dprint.addIntItem("status", rspSuccess);
      dprint.addStrItem("fluid_name", fluid_name);
    } else {
      dprint.addIntItem("status", rspError);
      dprint.addStrItem("err_msg", "unable to get mfc fluid name");
    }
  }
}

void MessageHandler::handleStartRelayBlink() {
  if (!checkNumberOfArgs(5)) {return;}
  int relay = readInt(1);
  long period = readLong(2);
  int duty_cycle = readInt(3);
  long count = readLong(4);
  if (checkRelayArg(relay) && checkPeriodArg(period) && checkDutyCycleArg(duty_cycle)) {
    systemCmdRsp(systemState.startRelayBlink(relay,period,duty_cycle,count));
  }
}

void MessageHandler::handleStopRelayBlink() {
  if (!checkNumberOfArgs(2)) {return;}
  int relay = readInt(1);
  if (checkRelayArg(relay)) {
    systemCmdRsp(systemState.stopRelayBlink(relay));
  }
}

void MessageHandler::handleStopAllRelaysBlink() {
  systemCmdRsp(systemState.stopAllRelaysBlink());
}

void MessageHandler::handleAddPulseCentered() {
  if (!checkNumberOfArgs(4)) {return;}
  int relay = readInt(1);
  long delay = readLong(2);
  int duration = readInt(3);
  if (checkRelayArg(relay)) {
    systemCmdRsp(systemState.addPulseCentered(relay,delay,duration));
  }
}

void MessageHandler::handleSetDigitalOutputHigh() {
  if (!checkNumberOfArgs(2)) {return;}
  int digital_output = readInt(1);
  if (checkDigitalOutputArg(digital_output)) {
    systemCmdRsp(systemState.setDigitalOutputHigh(digital_output));
  }
}

void MessageHandler::handleSetDigitalOutputLow() {
  if (!checkNumberOfArgs(2)) {return;}
  int digital_output = readInt(1);
  if (checkDigitalOutputArg(digital_output)) {
    systemCmdRsp(systemState.setDigitalOutputLow(digital_output));
  }
}

void MessageHandler::handleToggleDigitalOutput() {
  if (!checkNumberOfArgs(2)) {return;}
  int digital_output = readInt(1);
  if (checkDigitalOutputArg(digital_output)) {
    systemCmdRsp(systemState.toggleDigitalOutput(digital_output));
  }
}

void MessageHandler::handlePulseDigitalOutput() {
  if (!checkNumberOfArgs(3)) {return;}
  int digital_output = readInt(1);
  int duration = readInt(2);
  if (checkDigitalOutputArg(digital_output)) {
    systemCmdRsp(systemState.pulseDigitalOutput(digital_output,duration));
  }
}

void MessageHandler::handleGetDigitalInput() {
  if (!checkNumberOfArgs(2)) {return;}
  int digital_input = readInt(1);
  if (checkDigitalInputArg(digital_input)) {
    int val = systemState.getDigitalInput(digital_input);
    dprint.addIntItem("status", rspSuccess);
    dprint.addIntItem("digital_input_value", val);
  }
}

void MessageHandler::handleGetAnalogInput() {
  if (!checkNumberOfArgs(2)) {return;}
  int analog_input = readInt(1);
  if (checkAnalogInputArg(analog_input)) {
    int val = systemState.getAnalogInput(analog_input);
    dprint.addIntItem("status", rspSuccess);
    dprint.addIntItem("analog_input_value", val);
  }
}

void MessageHandler::handleRemoveAllTasks() {
  systemCmdRsp(systemState.removeAllTasks());
}

void MessageHandler::handleGetTime() {
  dprint.addIntItem("status", rspSuccess);
  dprint.addLongItem("time", systemState.getTime());
}

void MessageHandler::handleSetTime() {
  if (!checkNumberOfArgs(2)) {return;}
  uint32_t time = readLong(1);
  systemCmdRsp(systemState.setTime(time));
}

void MessageHandler::handleGetTaskDetails() {
  if (!checkNumberOfArgs(2)) {return;}
  uint8_t taskId = (uint8_t)readInt(1);
  uint32_t time;
  int arg;
  long period;
  long count;
  bool free;
  bool enabled;
  systemState.getTaskDetails(taskId,time,arg,period,count,free,enabled);
  dprint.addIntItem("status", rspSuccess);
  dprint.addLongItem("time", time);
  dprint.addIntItem("arg", arg);
  dprint.addLongItem("period", period);
  dprint.addLongItem("count", count);
  dprint.addIntItem("free", free);
  dprint.addIntItem("enabled", enabled);
}

void MessageHandler::handleStartAnalogInputsMapping() {
  systemState.startAnalogInputsMapping();
  dprint.addIntItem("status", rspSuccess);
}

void MessageHandler::handleStopAnalogInputsMapping() {
  systemState.stopAnalogInputsMapping();
  dprint.addIntItem("status", rspSuccess);
}

// -------------------------------------------------


void MessageHandler::handleDebug() {
  char name[20];
  dprint.addIntItem("status", rspSuccess);
}


MessageHandler messageHandler;
