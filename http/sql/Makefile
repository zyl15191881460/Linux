cc=g++
LIB=-lmysqlclient

.PHONY:all
all:insert_cgi select_cgi

insert_cgi:insert_cgi.o sql.o
	@$(cc) -o $@ $^
	@echo linking insert_cgi
select_cgi:select_cgi.o sql.o
	@$(cc) -o $@ $^
	@echo linking insert_cgi

%.o:%.cpp
	@$(cc) -o $<
	@echo compiling

.PHONY:clean
clean:
	@rm -f *.o insert_cgi select_cgi
	@echo clean sql
