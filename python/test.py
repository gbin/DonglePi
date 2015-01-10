from DPi import GPIO
import time

GPIO.setmode(GPIO.BOARD)
GPIO.setup(7, GPIO.OUT)
while True:
  GPIO.output(7, 1)
  time.sleep(.9)
  GPIO.output(7, 0)
  time.sleep(.9)
