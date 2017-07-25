#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "./Date.cpp"
using namespace std;
#define SIZE 1024

void math_cgi(char* info)
{
	char* start = info;
	char* argv[SIZE];
	int i = 0;

	while(*start)
	{
		if(*start == '=')
		{
			start++;
			argv[i++] =start;
			continue;
		}

		if(*start == '&')
		{
			*start = '\0';
		}
		start++;
	}
	argv[i] = NULL;
	if(atoi(argv[0]) != 0 && atoi(argv[1]) != 0 && atoi(argv[2]) != 0)
	{
		Date date1(atoi(argv[0]), atoi(argv[1]), atoi(argv[2]));
		int day =atoi(argv[3]);
		date1 = date1 + day;
		printf("<html>\n");
		printf("<body>");
		printf("<h1>");
		cout<<date1._year<<"/"<<date1._month<<"/"<<date1._day<<endl;
		printf("</h1>");
		printf("</body>");
		printf("</html>\n");

	}


	if(atoi(argv[4]) != 0 && atoi(argv[5]) != 0 && atoi(argv[6]) != 0 &&\
	   atoi(argv[7]) != 0 && atoi(argv[8]) != 0 && atoi(argv[9]) != 0)
	{
		int tmp = 0;
		Date date2(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
		Date date3(atoi(argv[7]), atoi(argv[8]), atoi(argv[9]));
		tmp = date2 - date3;
		printf("<html>\n");
		printf("<body>");
		printf("<h1>");
		cout<<tmp<<endl;
		printf("</h1>");
		printf("</body>");
		printf("</html>\n");
	}
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
			printf("get method GET error\n");
			return -2;
		}
		strcpy(info, query_string);
	}
	else if(strcasecmp(method, "POST") == 0)
	{
		content_length = getenv("CONTENT_LENGTH");
		if(content_length == NULL)
		{
			printf("get VONTENT_LENGTH errno\n");
			return -3;
		}
		char c = 0;
		for(; i < atoi(content_length); i++)
		{
			recv(0, &c, 1, 0);
			info[i] = c;
		}
		info[i] = '\0';
	}
	else
	{
		printf("method error\n");
		return -4;
	}
	math_cgi(info);
	return 0;
}
