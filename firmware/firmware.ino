#include <SPI.h>
#include <Streaming.h>
#include <EEPROM.h>
#include <util/atomic.h>

#include "SerialReceiver.h"
#include "DictPrinter.h"
#include "Tle7232g.h"
#include "Bronkhorst.h"
#include "TimeTriggeredScheduler.h"
#include "NewhavenDisplay.h"
#include "TimerThree.h"

#include "constants.h"
#include "SystemState.h"
#include "MessageHandler.h"

void setup() {
    Serial.begin(constants::baudrate);
    systemState.initialize();
}

void loop() {
    messageHandler.processMsg();
    systemState.updateDisplay();
}


