#ifndef _SQL_H_
#define _SQL_H_

#include <iostream>
#include <string>
#include <string.h>
#include <mysql.h>
#include <stdio.h>
using namespace std;

#define SIZE 1024

class sql
{
public:
	sql(const string& _user, const string& _ip,const string& _passwd, const string& _db, const int& _port);
	int connect();
	int insert(const string& name, const string& sex,const string& school, const string& hobby);
	int select();
	~sql();
private:
	sql(const sql& o);
	MYSQL *conn;
	string user;
	string ip;
	string passwd;
	string db;
	int port;
};


#endif
