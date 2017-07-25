bin=httpd
cc=gcc
obj=http.o main.o
CGI_PATH=sql wwwroot/cgi-bin
FLAGS=#-D_DEBUG_
LDFLAGS=-lpthread

$(bin):$(obj)
	@$(cc) -o $@ $^ $(LDFLAGS)
	@echo linking

%.o:%.c
	@$(cc) -c $< $(FLAGS)
	@echo compiling

.PHONY:cgi
cgi:
	@for i in `echo $(CGI_PATH)`;\
		do\
			cd $$i;\
			make;\
			cd -;\
		done

.PHONY:clean
clean:
	@rm -f $(bin) $(obj)
	@for i in `echo $(CGI_PATH)`;\
	do\
		cd $$i;\
		make clean;\
		cd -;\
	done
.PHONY:output
output:
	mkdir -p output/wwwroot/cgi-bin
	cp httpd output
	cp -rf log output
	cp -rf conf output
	cp wwwroot/index.html output/wwwroot
	cp wwwroot/cgi-bin/math_cgi output/wwwroot/cgi-bin
	cp sql/insert_cgi output/wwwroot/cgi-bin
	cp sql/select_cgi output/wwwroot/cgi-bin
