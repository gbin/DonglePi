from DPi import GPIO
from DPi import smbus
import time

LED_DEV = 0x70
print("setup...")
GPIO.setmode(GPIO.BOARD)
i2c = smbus.SMBus(0)
i2c.write_byte(LED_DEV, 0x21) # oscillator on
i2c.write_byte(LED_DEV, 0x81) # swith on with no blink
i2c.write_byte(LED_DEV, 0xEF) # brightness
for i in range(8):
  i2c.write_byte_data(LED_DEV, i*2, 1<<i)
  i2c.write_byte_data(LED_DEV, i*2 +1 , 1<< (8-i))
print("write on display done.")

GPIO.setup(7, GPIO.OUT)
while True:
  GPIO.output(7, 1)
  time.sleep(.9)
  GPIO.output(7, 0)
  time.sleep(.9)
