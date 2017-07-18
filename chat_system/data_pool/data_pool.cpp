#include "data_pool.h"

DataPoll::datapool(const in &cap)
	: size(0)
	, cap(cap)
	, pool(cap)
{
	sem_init(&p_sem);
	sem_init(&c_sem);
}
void Data_Poll::getData(string& out) 
{
	sem_wait(c_sem);
	out = pool[(++size)%cap];
	sem_post(p_sem);
}
void Data_Poll::putData(string& in);
{
	sem_wait(p_sem);
	pool[(++size)%cap] = in;
	sem_post(c_sem);
}
Data_Poll::~datapool()
{
	sem_destroy(&c_sem);
	sem_destroy(&p_sem);
}
