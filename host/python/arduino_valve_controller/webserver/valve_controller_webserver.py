#!python
from __future__ import print_function
import os
import sys
import flask
import flask_sijax
import argparse

from arduino_device import findArduinoDevicePorts

from arduino_valve_controller import ArduinoValveController
from arduino_valve_controller import isArduinoValveControllerPortInfo


network_port = 5000

# Setup application w/ sijax
path = os.path.join('.', os.path.dirname(__file__), 'static/js/sijax/')
app = flask.Flask(__name__)
app.config["SIJAX_STATIC_PATH"] = path
app.config["SIJAX_JSON_URI"] = '/static/js/sijax/json2.js'
flask_sijax.Sijax(app)

class SijaxHandler(object):
    """A container class for all Sijax handlers.

    Grouping all Sijax handler functions in a class
    (or a Python module) allows them all to be registered with
    a single line of code.
    """

    # arduino_valve_controller
    @staticmethod
    def setRelayOn(obj_response,relay):
        if ARMC is not None:
            ARMC.setRelayOn(relay)

    @staticmethod
    def setAllRelaysOff(obj_response):
        if ARMC is not None:
            ARMC.setAllRelaysOff()

@flask_sijax.route(app, "/")
def index():
    if flask.g.sijax.is_sijax_request:
        # The request looks like a valid Sijax request
        # Let's register the handlers and tell Sijax to process it
        flask.g.sijax.register_object(SijaxHandler)
        return flask.g.sijax.process_request()

    return flask.render_template('valve_controller.html')

def arduinoValveControllerWebserver():
    global ARMC
    ARMC = None

    parser = argparse.ArgumentParser(description='Arduino Relay Mfc Controller Webserver')
    parser.add_argument('-d', '--debug',
                        help='start the server in local-only debug mode',
                        action='store_true')
    args = parser.parse_args()

    server = 'remote'
    debug = False
    if args.debug:
        server = 'local'
        debug = True

    # Open connection to device
    arduino_device_ports = findArduinoDevicePorts()
    arduino_valve_controller_port = None
    for port in arduino_device_ports:
        port_info = arduino_device_ports[port]
        if isArduinoValveControllerPortInfo(port_info):
            arduino_valve_controller_port = port

    if arduino_valve_controller_port is not None:
        ARMC = ArduinoValveController(port=arduino_valve_controller_port)

    if server == 'local':
        print(' * using debug server - localhost only')
        app.run(port=network_port,debug=debug)
    else:
        print(' * using builtin server - remote access possible')
        app.run(host='0.0.0.0',port=network_port)


# -----------------------------------------------------------------------------
if __name__ == '__main__':
    arduinoValveControllerWebserver()
