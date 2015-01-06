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

def add_event_callback(channel, callback):
  raise NotImplementedError()

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
  raise NotImplementedError()

# Input from a GPIO channel.  Returns HIGH=1=True or LOW=0=False
# channel - either board pin number or BCM number depending on which mode is set.
def input(channel):
  raise NotImplementedError()


def output(channel, value):
  raise NotImplementedError()

def remove_event_detect(channel):
  raise NotImplementedError()

# BOARD - Use Raspberry Pi board numbers
# BCM   - Use Broadcom GPIO 00..nn numbers
def setmode(mode):
  raise NotImplementedError()

# setup(...)
# Set up the GPIO channel, direction and (optional) pull/up down control
# channel        - either board pin number or BCM number depending on which mode is set.
# direction      - INPUT or OUTPUT
# [pull_up_down] - PUD_OFF (default), PUD_UP or PUD_DOWN
# [initial]      - Initial value for an output channel (gbin: LOW / HIGH for
# OUTPUT only)
def setup(channel, direction, pull_up_down = PUD_OFF, initial=None):
  raise NotImplementedError()

def setwarnings(on):
  raise NotImplementedError()

# Wait for an edge.
# channel - either board pin number or BCM number depending on which mode is set.
# edge    - RISING, FALLING or BOTH
def wait_for_edge(channel, edge):
  raise NotImplementedError()


