import donglepi_pb2
import threading
import serial
import logging

def hexdump(s):
  return " ".join("{:02x}".format(ord(c)) for c in s)

def delimitedMessage(message):
  serializedMessage = message.SerializeToString()
  delimiter = donglepi_pb2.Transport()
  delimiter.message_length = len(serializedMessage)
  return delimiter.SerializeToString() + serializedMessage

pending_request = donglepi_pb2.DonglePiRequest()
request_lock = threading.Lock()

last_response = donglepi_pb2.DonglePiResponse()
response_lock = threading.Lock()


def main_loop(cb):
  global pending_request
  port = serial.Serial(port="/dev/ttyACA0", baudrate=115200, timeout=0.0002)
  port.open()
  rcv_buffer = ""
  while True:
    with request_lock:
      request_number = pending_request.message_nb
      msg = delimitedMessage(pending_request)
      pending_request = donglepi_pb2.DonglePiRequest()
      pending_request.message_nb = (request_number + 1) % 65536
    port.write(msg)

    delimiter = donglepi_pb2.Transport()
    offset = -1
    while True:
      rcv_buffer += port.read(1024)
      if len(rcv_buffer) < 2:
        continue # too short for a message
      if offset == -1:
        offset = delimiter.ParseFromString(rcv_buffer) # TODO check if error

      if len(rcv_buffer) - offset < delimiter.message_length:
        continue # not enough !

    response = donglepi_pb2.DonglePiResponse()
    response.ParseFromString(
        rcv_buffer[offset:offset + delimiter.message_length])
    rcv_buffer = rcv_buffer[delimiter.message_length:]
    if response.message_nb != pending_request.message_nb:
      logging.warn("Donglepi desynchronized req nb = %d but resp nb = %d" %
                   (response.message_nb, pending_request.message_nb))

    # assert response.message_nb == request_number
    with response_lock:
      last_response = response
    cb(response)



#req = donglepi_pb2.DonglePiRequest()
#req.message_nb = 1
#req.data.gpio.mask = 0xDEADBEEF
#req.data.gpio.values = 0x13333331
#print(hexdump(delimitedMessage(req)))

#req2 = donglepi_pb2.DonglePiRequest()
#req2.message_nb = 9
#print(hexdump(delimitedMessage(req2)))


