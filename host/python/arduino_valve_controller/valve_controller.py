#!python
from __future__ import print_function, division
import atexit
import argparse
import copy
import time

from arduino_device import ArduinoDevice, ArduinoDevices, findArduinoDevicePorts


DEBUG = False
BAUDRATE = 'default'
DEVICE_MODEL_NUMBER = 2117

class ArduinoValveController(ArduinoDevice):

    def __init__(self,*args,**kwargs):
        kwargs.update({'model_number': DEVICE_MODEL_NUMBER})
        serial_number = None
        if 'debug' not in kwargs:
            kwargs.update({'debug': DEBUG})
        if 'baudrate' not in kwargs:
            kwargs.update({'baudrate': BAUDRATE})
        elif (kwargs['baudrate'] is None) or (kwargs['baudrate'].lower() == 'default'):
            kwargs.update({'baudrate': BAUDRATE})
        if 'serial_number' in kwargs:
            serial_number = kwargs.pop('serial_number')
        if ('port' not in kwargs) or (kwargs['port'] is None):
            port =  findArduinoValveControllerPort(baudrate=kwargs['baudrate'],
                                                      serial_number=serial_number)
            kwargs.update({'port': port})
        super(ArduinoValveController,self).__init__(*args,**kwargs)
        atexit.register(self._exitArduinoValveController)
        dev_info = self.getDevInfo()
        self.relay_count = dev_info['relay_count']
        self.mfc_count = dev_info['mfc_count']

    def _exitArduinoValveController(self):
        try:
            self.stopAllRelaysBlink()
        except IOError:
            pass

    def getArduinoValveControllerInfo(self):
        arduino_relay_mfc_controller_info = self.getArduinoDeviceInfo()
        arduino_relay_mfc_controller_info.update({'relay_count': self.relay_count})
        arduino_relay_mfc_controller_info.update({'mfc_count': self.mfc_count})
        mfc_info_list = []
        for mfc_index in range(self.mfc_count):
            mfc_device_info = {}
            try:
                flow_rate_setting = self.getMfcFlowRateSetting(mfc_index)
                flow_rate_measure = self.getMfcFlowRateMeasure(mfc_index)
                serial_number = self.getMfcSerialNumber(mfc_index)
                capacity = self.getMfcCapacity(mfc_index)
                capacity_units = self.getMfcCapacityUnits(mfc_index)
                fluid_name = self.getMfcFluidName(mfc_index)

                mfc_device_info.update({'flow_rate_measure': flow_rate_measure,
                                        'flow_rate_setting': flow_rate_setting,
                                        'serial_number': serial_number,
                                        'capacity': capacity,
                                        'capacity_units': capacity_units,
                                        'fluid_name': fluid_name,
                                        })
            except IOError:
                pass
            if 0 < len(mfc_device_info):
                mfc_info_list.append(mfc_device_info)
        if 0 < len(mfc_info_list):
            arduino_relay_mfc_controller_info.update({'mfc_info': mfc_info_list})

        return arduino_relay_mfc_controller_info

# device_names example:
# [{'port':'/dev/ttyACM0',
#   'device_name':'relay_controller0'},
#  {'serial_number':3,
#   'device_name':'relay_controller1'}]
class ArduinoValveControllers(ArduinoDevices):

    def __init__(self,*args,**kwargs):
        if ('use_ports' not in kwargs) or (kwargs['use_ports'] is None):
            kwargs['use_ports'] = findArduinoValveControllerPorts(*args,**kwargs)
        super(ArduinoValveControllers,self).__init__(*args,**kwargs)

    def appendDevice(self,*args,**kwargs):
        self.append(ArduinoValveController(*args,**kwargs))

    def getArduinoValveControllerInfo(self):
        arduino_relay_mfc_controller_info = []
        for device_index in range(len(self)):
            dev = self[device_index]
            arduino_relay_mfc_controller_info.append(dev.getArduinoValveControllerInfo())
        return arduino_relay_mfc_controller_info


def findArduinoValveControllerPorts(baudrate=None, serial_number=None, try_ports=None, debug=DEBUG):
    arduino_device_ports = findArduinoDevicePorts(baudrate=baudrate,
                                                  model_number=DEVICE_MODEL_NUMBER,
                                                  serial_number=serial_number,
                                                  try_ports=try_ports,
                                                  debug=debug)

    if type(serial_number) is int:
        serial_number = [serial_number]

    arduino_relay_mfc_controller_ports = {}
    for port in arduino_device_ports:
        try:
            dev_serial_number = arduino_device_ports[port]['serial_number']
        except KeyError:
            break
        if (serial_number is None) or (dev_serial_number in serial_number):
            arduino_relay_mfc_controller_ports[port] = {'serial_number': dev_serial_number}
    return arduino_relay_mfc_controller_ports

def findArduinoValveControllerPort(baudrate=None, serial_number=None, try_ports=None, debug=DEBUG):
    arduino_relay_mfc_controller_ports = findArduinoValveControllerPorts(baudrate=baudrate,
                                                                            serial_number=serial_number,
                                                                            try_ports=try_ports,
                                                                            debug=debug)
    if len(arduino_relay_mfc_controller_ports) == 1:
        return arduino_relay_mfc_controller_ports.keys()[0]
    elif len(arduino_relay_mfc_controller_ports) == 0:
        arduino_device_ports = findArduinoDevicePorts(baudrate=baudrate,
                                                      model_number=DEVICE_MODEL_NUMBER,
                                                      serial_number=serial_number,
                                                      try_ports=try_ports,
                                                      debug=debug)
        err_str = 'Could not find Arduino relay mfc controllers. Check connections and permissions.\n'
        err_str += 'Tried ports: ' + str(arduino_device_ports)
        raise RuntimeError(err_str)
    else:
        err_str = 'Found more than one Arduino relay mfc controller. Specify port or serial_number.\n'
        err_str += 'Matching ports: ' + str(arduino_relay_mfc_controller_ports)
        raise RuntimeError(err_str)

def isArduinoValveControllerPortInfo(port_info):
    return port_info['model_number'] == DEVICE_MODEL_NUMBER

def arduinoValveControllerCli():
    parser = argparse.ArgumentParser(description='Arduino Relay Mfc Controller')
    # parser.add_argument('--debug',
    #                     help='Use the simulated software relay controller instead of the real hardware relay controller',
    #                     action='store_true')
    # parser.add_argument('-d','--device',nargs=1,type=int,default=[0],
    #                     help='device index')
    subparsers = parser.add_subparsers(dest='subparser_name',help='sub-command help')

    # create the parser for the "info" command
    parser_info = subparsers.add_parser('info', help='info help')

    # create the parser for the "relay" command
    parser_relay = subparsers.add_parser('relay', help='relay help')
    parser_relay.add_argument('-r','--relay',nargs=1,type=int,default=[0],
                              help='relay index')
    parser_relay.add_argument('--on',action="store_true",
                              help='turn on relay')
    parser_relay.add_argument('--off',action="store_true",
                              help='turn off relay')

    # create the parser for the "mfc" command
    parser_mfc = subparsers.add_parser('mfc', help='mfc help')
    parser_mfc.add_argument('-m','--mfc',nargs=1,type=int,required=True,
                            help='mfc index')
    parser_mfc.add_argument('-p','--percent-capacity',nargs=1,type=float,required=True,
                            help='sets flow rate as a percentage of mfc capacity, range 0-100')

    args = parser.parse_args()

    o = ArduinoValveController(debug=DEBUG)
    # if not args.debug:
    #     o = ArduinoValveController()
    # else:
    #     o = ArduinoValveControllerDebug()
    # device = args.device[0]
    if args.subparser_name == 'relay':
        relay = args.relay[0]
        if args.on:
            o.setRelayOn(relay)
        elif args.off:
            o.setRelayOff(relay)
        else:
            print("Add --on or --off")
    elif args.subparser_name == 'mfc':
        mfc = args.mfc[0]
        o.setMfcFlowRate(mfc,args.percent_capacity[0])
    elif args.subparser_name == 'info':
        print(o.getArduinoValveControllerInfoDict())

# -----------------------------------------------------------------------------------------
if __name__ == '__main__':
    arduinoValveControllerCli()
