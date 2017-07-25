#include "http.h"


int startup(const char* _ip, int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("sock");
		return -2;
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(_ip);
	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
	{
		perror("bind");
		return -3;
	}

	if(listen(sock, 10) < 0)
	{
		perror("listen");
		return -4;
	}
	return sock;
}

static int get_line(int sock, char *buf, int len)
{
	char ch='\0';
	int i = 0;
	while(i < len - 1 && ch != '\n')
	{
		//在Linux中的\n,\r是有区别的，所以，要在获取一行的时候统一
		ssize_t s = recv(sock, &ch, 1, 0);
		if(s > 0)
		{
			if(ch == '\r')//当查找到的字符是\r的时候，应该对字符进行区别对待；
			{
				recv(sock, &ch, 1, MSG_PEEK); //最后一个MSD_PEEK是对查找字符的下一个字符进行查看但是不拿出来；
				if(ch == '\n')
					recv(sock, &ch, 1, 0);
				else
					ch = '\n';
			}
			buf[i++] = ch;
		}
	}
	buf[i] = 0;
	return i;
}
void request_404(int sock)
{
//	char* path = "wwwroot/404.html";
//	struct stat ispath;
//	if(stat(path, &ispath) < 0)
//	{
//		return;
//	}
//	int fd = open(path, O_RDONLY);
//	if(fd < 0)
//	{
//		print_log("open failed", FATAL);
//		return;
//	}
//	char buf[SIZE];
//	memset(buf, 0, SIZE);
//	sprintf(buf, "HTTP/1.0 404 NOT Found! \r\n\r\n");
//	if(send(sock, buf, strlen(buf), 0) < 0)
//	{
//		print_log("send filed", FATAL);
//		return;
//	}
//	if(sendfile(sock, fd, NULL, sizeof(buf)) < 0)
//	{
//		print_log("sendfile failed", FATAL);
//		return;
//	}
//	close(fd);
}

void print_log(const char *msg, int level)
{
	const char *level_msg[] = {
		"NOTICE",
		"WARNING",
		"FATAL"
	};
	printf("[%s][%s]\n", msg, level_msg[level]);
}

void echo_error(int fd, int errno_num)
{
	switch(errno_num)
	{
		case 401:
			break;
		case 404:
			request_404(fd);
			break;
		case 500:
			break;
		case 501:
			break;
		case 503:
			break;
		default:
			break;
	}
}

int echo_www(int fd, const char *path, int size)
{
	int new_fd = open(path, O_RDONLY);
	if(new_fd < 0)
	{
		print_log("open file error!", FATAL);
		return 404;
	}

	const char *echo_line="HTTP/1.0 200 OK\r\n";
	send(fd, echo_line, strlen(echo_line), 0);
	const char *blank_line = "\r\n";
	send(fd, blank_line, strlen(blank_line), 0);

	if(sendfile(fd, new_fd, NULL, size) < 0)
	{
		print_log("send file error!", FATAL);
		return 200;
	}
	close(new_fd);
}

void drop_header(int fd)
{
	char buff[SIZE];
	int ret = -1;
	do{
		ret = get_line(fd, buff, sizeof(buff));
	}while(ret > 0 && strcmp(buff, "\n"));
}

int exe_cgi(int fd, const char *method, const char* path,\
			const char* query_string)
{
	int content_len = -1;
	char METHOD[SIZE/10];
	char QUERY_STRING[SIZE];
	char CONTENT_LENGTH[SIZE];
	if(strcasecmp(method, "GET") == 0)
	{
		drop_header(fd);
	}
	else
	{
		char buff[SIZE];
		int ret = -1;
		do
		{
			ret = get_line(fd, buff, sizeof(buff));
			if(strncasecmp(buff, "Content-Length: ", 16) == 0)
			{
				content_len = atoi(&buff[16]);
			}
		}while(ret > 0 && strcmp(buff, "\n"));
		if(content_len == -1)
		{
			echo_error(fd, 401);
			return -1;
		}
	}
	printf("cgi: path: %s\n", path);
	int input[2];
	int output[2];
	if(pipe(input) < 0)
	{
		echo_error(fd, 401);
		return -2;
	}
	if(pipe(output) < 0)
	{
		echo_error(fd, 401);
		return -3;
	}

	const char *echo_line = "HTTP/1.0 200 OK\r\n";
	send(fd, echo_line, strlen(echo_line), 0);
	const char *type = "Content-Type:text/html;charset=ISO-8859-1\r\n";
	send(fd, type, strlen(type), 0);
	const char *blank_line = "\n\r";
	send(fd, blank_line, strlen(blank_line), 0);

	pid_t id = fork();
	if(id < 0)
	{
		echo_error(fd, 501);
		return -2;
	}
	else if(id == 0)
	{ //child
		close(input[1]);
		close(output[0]);
		sprintf(METHOD, "METHOD=%s", method);
		putenv(METHOD); //添加环境变量
		if(strcasecmp(method, "GET") == 0)
		{
			sprintf(QUERY_STRING, "QUERY_STRING=%s", query_string);
			putenv(QUERY_STRING); //GET方法在环境变量中添加query_string
		}
		else
		{
			sprintf(CONTENT_LENGTH, "CONTENT_LENGTH=%d", content_len);
			putenv(CONTENT_LENGTH);//POST方法在环境变量中添加content_len
		}
		dup2(input[0], 0); 
		dup2(output[1], 1);
		execl(path, path, NULL); //进行exec替换，环境便令不受影响
		exit(1);
	}
	close(input[0]);
	close(output[1]);

	int i = 0;
	char c = '\0';
	for(; i < content_len; i++)
	{
		recv(fd, &c, 1, 0);
		write(input[1], &c, 1);
	}

	while(1)
	{
		ssize_t s = read(output[0], &c, 1);
		if(s > 0)
		{
			send(fd, &c, 1, 0);
		}
		else
			break;
	}
	waitpid(id, NULL, 0);
	close(input[1]);
	close(output[0]);
}

void* handler_request(void* argc)
{
	int fd = (int)argc;
	int errno_num = 200;
	int cgi = 0;
	char *query_string = NULL;
#ifdef _DEBUG_
	printf("###########################");
	char buff[SIZE];
	int ret = -1;
	do{
		ret = get_line(fd, buff, sizeof(buff));
		printf("%s",buff);
	}while(ret > 0 && strcmp(buff, "\n"));
	printf("###########################");
#else
	char method[SIZE/10];
	char url[SIZE];
	char path[SIZE];
	char buff[SIZE];
	int i, j;
	if(get_line(fd, buff, sizeof(buff)) <= 0)
	{
		print_log("get request line error", FATAL);
		errno_num = 501;
		goto end;
	}
	i = 0, j = 0;
	while(i < sizeof(method) - 1 && j < sizeof(buff) && \
			!isspace(buff[j]))
	{
		method[i] = buff[j];   //得到访问的方法
		i++, j++;
	}
	method[i] = 0;
	while(isspace(buff[j]) && j < sizeof(buff))
	{
		j++;
	}
	i = 0;
	while(i < sizeof(url) && j < sizeof(buff) && \
			!isspace(buff[j]))
	{
		url[i] = buff[j];   //得到url
		i++, j++;
	}
	url[i] = 0;
	printf("method: %s, url: %s\n", method, url);
	if(strcasecmp(method, "GET") && strcasecmp(method, "POST"))  //这里只研究GET和POST方法，别的方法直接报错
	{
		print_log("method is not ok!", FATAL);
		errno_num = 501;
		goto end;
	}
	if(strcasecmp(method, "POST") == 0) //POST方法直接执行cgi模式
	{
		cgi = 1;
	}
	query_string = url;  //在GET方法中，我们都知道url中包含了body中的内容，
	while(*query_string != 0)
	{
		if(*query_string == '?') //在url中查找是否有？号，有表示body中有内容，就执行cgi模式，并且把body中的内容拿到query_string中
		{
			cgi = 1;
			*query_string = '\0';
			query_string++;
			break;
		}
		query_string++;
	}
	sprintf(path, "wwwroot%s", url); //对路径进行设置
	if(path[strlen(path)-1] == '/')
	{
		strcat(path, "index.html");
	}
	printf("path: %s\n", path);

	struct stat st;
	if(stat(path, &st) < 0)
	{
		print_log("path not found!", FATAL);
		errno_num = 404;
		goto end;
	}
	else
	{
		if(S_ISDIR(st.st_mode))
		{
			strcat(path, "/index.html");
		}
		else
		{
			if((st.st_mode & S_IXUSR) ||\
			   (st.st_mode & S_IXGRP) ||\
			   (st.st_mode & S_IXOTH) )
			{
				cgi = 1;
			}
		}
	}

	if(cgi)
	{
		exe_cgi(fd, method, path, query_string);
	}
	else  //不是cgi模式，直接处理
	{
		drop_header(fd);
		errno_num = echo_www(fd, path, st.st_size);
	}
#endif
end:
	echo_error(fd, errno_num);
	close(fd);
}




