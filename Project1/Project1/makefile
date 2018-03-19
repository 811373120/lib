objs=test.o 
test : test.cpp
	g++ -o test test.cpp ./protocol/protoctest.pb.h ./protocol/protoctest.pb.cc -I/usr/local/protobuf/include -L/usr/local/protobuf/lib -g -levent -levent_core -lprotobuf -pthread
.PHONY:clean
clean:
	-rm test $(objs)
