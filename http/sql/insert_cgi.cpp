#include "sql.h"
#include <sys/types.h>
#include <sys/socket.h>

void insert_data(char* buf)
{
	char* start = buf;
	char* argv[5];
	int i = 0;
	while(*start)
	{
		if(*start == '=')
		{
			argv[i++] = ++start;
			continue;
		}
		if(*start == '&')
		{
			*start = 0;
		}
		start++;
	}
	argv[i] = NULL;

	sql obj("root", "127.0.0.1", "", "student_tlb",3306);
	obj.connect();
	obj.insert(argv[0], argv[1], argv[2], argv[3]);
}

int main()
{
	char* method = NULL;
	char* query_string = NULL;
	char* content_length = NULL;
	char info[SIZE];
	int i = 0;
	method = getenv("METHOD");
	if(method == NULL)
	{
		printf("get method error\n");
		return -1;
	}

	if(strcasecmp(method, "GET") == 0)
	{
		query_string = getenv("QUERY_STRING");
		if(query_string == NULL)
		{
			printf("get method GET errno\n");
			return -2;
		}
		strcpy(info, query_string);
	}
	else if(strcasecmp(method, "POST") == 0)
	{
		content_length = getenv("CONTENT_LENGTH");
		if(content_length == NULL)
		{
			printf("get CONTENT_LENGTH errno\n");
			return -3;
		}
		char c = 0;
		for(; i < atoi(content_length); i++)
		{
			recv(0, &c, 1, 0);
			info[i] = c;
		}
	}
	else
	{
		printf("method error\n");
		return -4;
	}
	insert_data(info);
	return 0;
}
