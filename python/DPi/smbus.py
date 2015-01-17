import DPi
from struct import pack
from array import array

class SMBus(object):
  def __init__(self, bus = 0): # ignored for API compatibility.
    with DPi.request_lock:
      DPi.pending_request.config.i2c.enabled = True

  def _write(self, addr, buffer):
    with DPi.request_lock:
      write = DPi.pending_request.data.i2c.writes.add()
      write.addr = addr
      write.buffer = buffer

  # Quick transaction.  int addr  long
  def write_quick(self, addr):
    self._write(addr, "")
    return None

  # Read Byte transaction.  int addr  long
  def read_byte(self, addr):
    pass # return the read byte

  # Write Byte transaction. int addr,char val long
  def write_byte(self, addr, val):
    self._write(addr, pack('B', val))
    return None # or PyErr_SetFromErrno(PyExc_IOError);

  # Read Byte Data transaction. int addr,char cmd long
  def read_byte_data(self, addr, cmd):
    pass

  # Write Byte Data transaction.  int addr,char cmd,char val  long
  def write_byte_data(self, addr, cmd, val):
    print("write " + str(val))
    self._write(addr, pack('BB', cmd, val))
    return None

  # Read Word Data transaction. int addr,char cmd long
  def read_word_data(self, addr, cmd):
    pass

  # Write Word Data transaction.  int addr,char cmd,int val long
  def write_word_data(self, addr, cmd, val):
    self._write(addr, pack('BH', cmd, val))

  # Process Call transaction. int addr,char cmd,int val long
  def process_call(self, addr, cmd, val):
    pass

  # Read Block Data transaction.  int addr,char cmd long[]
  def read_block_data(self, addr, cmd):
    pass

  # Write Block Data transaction. int addr,char cmd,long[]  None
  def write_block_data(self, addr, cmd, vals):
    a = array('B')
    a.append(cmd)
    a.append(len(vals)) # apparently it prepends the size of the array
    a.fromlist(vals)
    self._write(addr, a.tostring())

  # Block Process Call transaction. int addr,char cmd,long[]  long[]
  def block_process_call(self, addr, cmd, vals):
    pass

  # Block Read transaction. int addr,char cmd long[]
  def read_i2c_block_data(self, addr, cmd):
    pass

  # Block Write transaction.  int addr,char cmd,long[]  None
  def write_i2c_block_data(self, addr, cmd, vals):
    a = array('B')
    a.append(cmd)
    a.fromlist(vals)
    self._write(addr, a.tostring())
