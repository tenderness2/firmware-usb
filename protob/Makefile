check: config.pb messages.pb storage.pb types.pb

%.pb: %.proto
	protoc  $< --cpp_out=.

clean:
	rm -f *pb.cc *.pb.h
