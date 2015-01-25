from DPi import GPIO
from DPi import smbus
import time

def rotl(num, bits):
    bit = num & (1 << (bits-1))
    num <<= 1
    if(bit):
        num |= 1
    num &= (2**bits-1)
 
    return num

LED_DEV = 0x70
print("setup...")
GPIO.setmode(GPIO.BOARD)
i2c = smbus.SMBus(0)
i2c.write_byte(LED_DEV, 0x21) # oscillator on
i2c.write_byte(LED_DEV, 0x81) # switch on with no blink
i2c.write_byte(LED_DEV, 0xEF) # brightness
print("write on display done.")

#GPIO.setup(25, GPIO.OUT)
#GPIO.setup(22, GPIO.IN, GPIO.PUD_UP)
pattern = [0] * 16
for i in range(8):
  pattern[i*2] = 1 << i
  pattern[i*2+1] = 1 << (7-i)

pattern[6] = 24
pattern[7] = 24
pattern[8] = 24
pattern[9] = 24

blink = 1
while True:
  #GPIO.output(25, blink)
  print("INPUT = " + str(GPIO.input(22)))
  blink = 0 if blink == 1 else 1
  for i, value in enumerate(pattern):
    i2c.write_byte_data(LED_DEV, i, value)
  pattern = [rotl(val,8) for val in pattern]
  time.sleep(.9)
