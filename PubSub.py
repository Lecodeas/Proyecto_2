# /* -----------------------------------------------
#  Universidad del Valle de Guatemala
#  IE2023: Programacion de Microcontroladores
#  PubSub.py
#  Autor: Ian Anleu Rivera
#  Proyecto: Proyecto 2
#  Hardware: ATMEGA328P
#  Creado: 21/04/2024
#  Ultima modificacion: 22/05/2024
#     -----------------------------------------------
# */

# Import standard python modules.
import sys
import time
import serial


# This example uses the MQTTClient instead of the REST client
from Adafruit_IO import MQTTClient

# holds the count for the feed
global run_count

ADAFRUIT_IO_USERNAME = "Lecodeas"
ADAFRUIT_IO_KEY = "aio_aSWG90R7vwiJxHjw3Bcw1ReOFDGX"

# Set to the ID of the feed to subscribe to for updates.
#feedContador = 'contador'
feedServo2 = 'hombro'
feedServo1 = 'base'
feedServo4 = 'garra'
feedServo3 = 'codo'
mensaje = ''

# Define callback functions which will be called when certain events happen.
def connected(client):
    """Connected function will be called when the client is connected to
    Adafruit IO.This is a good place to subscribe to feed changes.  The client
    parameter passed to this function is the Adafruit IO MQTT client so you
    can make calls against it easily.
    """
    # Subscribe to changes on a feed named Counter.
    #print('Subscribing to Feed {0} and {1}'.format(feedLed, feedContador))
    client.subscribe(feedServo1)
    client.subscribe(feedServo2)
    client.subscribe(feedServo3)
    client.subscribe(feedServo4)
    print('Waiting for feed data...')

def disconnected(client):
    """Disconnected function will be called when the client disconnects."""
    sys.exit(1)

def message(client, feed_id, payload):
    """Message function will be called when a subscribed feed has a new value.
    The feed_id parameter identifies the feed, and the payload parameter has
    the new value.
    """
    print('Feed {0} received new value: {1}'.format(feed_id, payload))
    
    if(feed_id == feedServo1):
        arduino.write(bytes('1', 'utf-8'))
        arduino.write(bytes((payload.rjust(3,'0')), 'utf-8'))
    if(feed_id == feedServo2):
        arduino.write(bytes('2', 'utf-8'))
        arduino.write(bytes((payload.rjust(3,'0')), 'utf-8'))
    if(feed_id == feedServo3):
        arduino.write(bytes('3', 'utf-8'))
        arduino.write(bytes((payload.rjust(3,'0')), 'utf-8'))
    if(feed_id == feedServo4):
        arduino.write(bytes('4', 'utf-8'))
        arduino.write(bytes((payload.rjust(3,'0')), 'utf-8'))

    
    


try:
    client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

    # Setup the callback functions defined above.
    client.on_connect = connected
    client.on_disconnect = disconnected
    client.on_message = message

    # Connect to the Adafruit IO server.
    client.connect()
    client.loop_background()
              
    arduino = serial.Serial(port='COM3', baudrate =9600, timeout = 0.1)
    
    while True:
        #mensaje = arduino.readline().decode('utf-8')
        #print(mensaje)
        time.sleep(0.1)
        
        
except KeyboardInterrupt:
    print("Salimos del programa")
    if arduino.is_open:
        arduino.close()
    sys.exit(1)