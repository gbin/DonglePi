# Build rule for the protocol
donglepi.pb2.py: ../protocol/donglepi.proto
	protoc --python_out=DPi -I../protocol -I../firmware/nanopb/generator/proto ../protocol/donglepi.proto
	cp ../firmware/nanopb/generator/proto/nanopb_pb2.py DPi/

clean:
	rm -f DPi/donglepi_pb2.py
	rm -f DPi/nanopb_pb2.py
	rm -f DPi/*.pyc
