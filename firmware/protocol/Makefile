# Include the nanopb provided Makefile rules
include ../nanopb/extra/nanopb.mk

# Build rule for the protocol
donglepi.pb.c: ../../protocol/donglepi.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=. -I../../protocol ../../protocol/donglepi.proto

clean:
	rm -f donglepi.pb.c
	rm -f donglepi.pb.h
