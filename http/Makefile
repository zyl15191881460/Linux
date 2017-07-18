bin=httpd
cc=gcc
obj=http.o main.o
FLAGS=#-D_DEBUG_
LDFLAGS=-lpthread

$(bin):$(obj)
	$(cc) -o $@ $^ $(LDFLAGS)

%.o:%.c
	gcc -c $< $(FLAGS)

.PHONY:clean
clean:
	rm -f $(bin) *.o
