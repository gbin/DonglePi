import threading
import DPi

# Modes
BCM = 11
BOARD = 10

# Edges
RISING = 31
FALLING = 32
BOTH = 33

# Values
LOW = 0
HIGH = 1

# Pull
PUD_OFF = 20
PUD_DOWN = 21
PUD_UP = 22

# functions
OUT = 0    # also directiom
IN = 1     # also diraction
HARD_PWM = 43
I2C = 42
SERIAL = 40
SPI = 41
UNKNOWN = -1

# versioning
RPI_REVISION = 2
VERSION = '0.5.6'

# This is only for the last version of the numbering (it changed tons of times)
# gbin: can be optimized with a list.
BCM2PI = {
     2: 3,  # GPIO02 = pin 3
     3: 5,
     4: 7,
    14: 8,
    15:10,
    17:11,
    18:12,
    27:13,
    22:15,
    23:16,
    24:18,
    10:19,
     9:21,
    25:22,
    11:23,
     8:24,
     7:26
    }

current_mode = None # Runtime error if it is not set.
functions = [
    None,    # 0
    None,    # 1
    None,    # 2
    HARD_PWM,# 3 (I2C)
    None,    # 4
    HARD_PWM,# 5 (I2C)
    None,    # 6
    IN,      # 7
    SERIAL,  # 8
    None,    # 9
    SERIAL,  # 10
    IN,      # 11
    IN,      # 12
    IN,      # 13
    None,    # 14
    IN,      # 15
    IN,      # 16
    None,    # 17
    IN,      # 18
    SPI,     # 19
    None,    # 20
    SPI,     # 21
    IN,      # 22
    SPI,     # 23
    SPI,     # 24
    None,    # 25
    SPI,     # 26
    ]

def add_event_callback(channel, callback):
  raise NotImplementedError()

# bouncetime in ms
def add_event_detect(channel, edge, callback=None, bouncetime=None):
  raise NotImplementedError()

class PWM(object):

  def __init__(self, channel, frequency):
    raise NotImplementedError()

  def ChangeDutyCycle(self, dutycycle):
    raise NotImplementedError()

  def ChangeFrequency(self, frequency):
    raise NotImplementedError()

  def start(self, dutycycle):
    raise NotImplementedError()

  def stop(self):
    raise NotImplementedError()

# default, all channels
def cleanup(channel=None):
  raise NotImplementedError()

def event_detected(channel):
  # return true if an edge has been detected
  return False

#   Return the current GPIO function (IN, OUT, PWM, SERIAL, I2C, SPI)
#       channel - either board pin number or BCM number depending on which mode is set.
def gpio_function(channel):
  return functions[channel]

# Input from a GPIO channel.  Returns HIGH=1=True or LOW=0=False
# channel - either board pin number or BCM number depending on which mode is set.
def input(channel):
  raise NotImplementedError()


def output(channel, value):
  with DPi.request_lock:
    DPi.pending_request.data.gpio.mask |= 1 << 7
    if value:
      DPi.pending_request.data.gpio.values |= value << 7
    else:
      DPi.pending_request.data.gpio.values &= ~(value << 7)


def remove_event_detect(channel):
  raise NotImplementedError()

# BOARD - Use Raspberry Pi board numbers
# BCM   - Use Broadcom GPIO 00..nn numbers
def setmode(mode):
  if mode not in (BOARD, BCM):
    raise ValueError()
  global current_mode
  current_mode = mode
  if not comm_thread.is_alive():
    DPi.pending_request = DPi.donglepi_pb2.DonglePiRequest()
    comm_thread.start()

# setup(...)
# Set up the GPIO channel, direction and (optional) pull/up down control
# channel        - either board pin number or BCM number depending on which mode is set.
# direction      - IN or OUT
# [pull_up_down] - PUD_OFF (default), PUD_UP or PUD_DOWN
# [initial]      - Initial value for an output channel (gbin: LOW / HIGH for
# OUTPUT only)
def setup(channel, direction, pull_up_down = PUD_OFF, initial=None):
  with DPi.request_lock:
    new_pin = DPi.pending_request.config.gpio.pins.add()
    new_pin.number = channel
    if direction == IN:
      new_pin.direction = DPi.donglepi_pb2.Config.GPIO.Pin.IN
    else:
      new_pin.direction = DPi.donglepi_pb2.Config.GPIO.Pin.OUT

def setwarnings(on):
  raise NotImplementedError()

# Wait for an edge.
# channel - either board pin number or BCM number depending on which mode is set.
# edge    - RISING, FALLING or BOTH
def wait_for_edge(channel, edge):
  raise NotImplementedError()

def dp_callback(donglepi_response):
  # gbin: forward to the callbacks
  pass

comm_thread = threading.Thread(target=DPi.main_loop,
                               name="DonglePi comm thread",
                               args=(dp_callback,))
comm_thread.daemon = True

