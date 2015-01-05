# Build rule for the protocol
donglepi.pb2.py: ../protocol/donglepi.proto
	protoc --python_out=. -I../protocol ../protocol/donglepi.proto

clean:
	rm -f donglepi_pb2.py
	rm -f *.pyc
