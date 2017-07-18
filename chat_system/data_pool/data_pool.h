#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

#include <iostream>
using namespace std;
#include <vector>
class DataPool
{
	public:
		datapool();
		void getData(string& out);
		void putData(string& in);
	private:
		~datapool();
		int size;
		int cap;
		sem_t p_sem;
		sem_t c_sem;
		std::vector<string> pool;
};


#endif
