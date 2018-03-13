objs=test.o
test : $(objs)
	g++ -o test $(objs) ./protocol/protoctest.pb.h ./protocol/protoctest.pb.cc -I/usr/local/protobuf/include -L/usr/local/protobuf/lib -g -levent -levent_core -lprotobuf -pthread
.PHONY:clean
clean:
	-rm test $(objs)
