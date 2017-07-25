#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 1024

void math_cgi(char* info)
{
	char* start = info;
	char* argv[3];
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
	int data1 = atoi(argv[0]);
	int data2 = atoi(argv[1]);

	printf("<html>\n");
	printf("<body>");
	printf("<h1>");
	printf("%d + %d=%d\n", data1, data2, data1+data2);
	printf("</h1>");
	printf("<h1>");
	printf("%d - %d=%d\n", data1, data2, data1-data2);
	printf("</h1>");
	printf("<h1>");
	printf("%d * %d=%d\n", data1, data2, data1*data2);
	printf("</h1>");
	printf("<h1>");
	printf("%d / %d=%d\n", data1, data2, data2==0?0 : data1/data2);
	printf("</h1>");
	printf("<h1>");
	printf("%d %% %d=%d\n", data1, data2, data2==0?0 : data1*data2);
	printf("</h1>");
	printf("</body>");
	printf("</html>\n");
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
