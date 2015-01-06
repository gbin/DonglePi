import donglepi_pb2

def hexdump(s):
  return " ".join("{:02x}".format(ord(c)) for c in s)

def delimitedMessage(message):
  serializedMessage = message.SerializeToString()
  delimiter = donglepi_pb2.Transport()
  delimiter.message_length = len(serializedMessage)
  return delimiter.SerializeToString() + serializedMessage

req = donglepi_pb2.DonglePiRequest()
req.message_nb = 1
req.data.gpio.mask = 0xDEADBEEF
req.data.gpio.values = 0x13333331
print(hexdump(delimitedMessage(req)))

req2 = donglepi_pb2.DonglePiRequest()
req2.message_nb = 9
print(hexdump(delimitedMessage(req2)))

