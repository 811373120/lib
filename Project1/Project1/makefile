objs=test.o
test : $(objs)
	g++ -o test $(objs) -R/usr/local/lib -g -levent -levent_core
.PHONY:clean
clean:
	-rm test $(objs)
