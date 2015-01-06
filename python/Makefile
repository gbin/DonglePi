# Build rule for the protocol
donglepi.pb2.py: ../protocol/donglepi.proto
	protoc --python_out=DPi -I../protocol ../protocol/donglepi.proto

clean:
	rm -f DPi/donglepi_pb2.py
	rm -f DPi/*.pyc
