#include <iostream>
using namespace std;

class Date
{
public:
	Date(int year, int month, int day)
	{
		if(month < 1 || month > 12)
		{
			cout << "Month is not OK!!!"<<endl;
			exit(1);
		}
		switch(month)
		{
			case 1:
				if(day < 1 || day > 31)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 2:
				if(IsLeapYear(year))
				{
					if(day < 1 || day > 29)
					{
						cout<<"day is not ok"<<endl;
						exit(1);
					}
				}
				else
				{
					if(day < 1 || day > 28)
					{
						cout<<"day is not ok"<<endl;
						exit(1);
					}
				}
				break;
			case 3:
				if(day < 1 || day > 31)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 4:
				if(day < 1 || day > 30)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 5:
				if(day < 1 || day > 31)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 6:
				if(day < 1 || day > 30)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 7:
				if(day < 1 || day > 31)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 8:
				if(day < 1 || day > 31)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 9:
				if(day < 1 || day > 30)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 10:
				if(day < 1 || day > 31)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 11:
				if(day < 1 || day > 30)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			case 12:
				if(day < 1 || day > 31)
				{
					cout<<"day is not ok"<<endl;
					exit(1);
				}
				break;
			default:
				break;
		}
		_year = year;
		_month = month;
		_day = day;
	}

	Date(const Date& d)
		: _year(d._year)
		, _month(d._month)
		, _day(d._day)
	{}

	Date operator=(const Date& d)
	{
		if(this != &d)
		{
			_year = d._year;
			_month = d._month;
			_day = d._day;
		}
		return *this;
	}

	int operator<(const Date& d)
	{
		if ((_year < d._year) || ((_year == d._year) && (_month < d._month)) || ((_year == d._year) && (_month == d._month) && (_day < d._day)))
			return 1;
		return 0;
	}
	int operator>(const Date& d)
	{
		if ((_year > d._year) || ((_year == d._year) && (_month > d._month)) || ((_year == d._year) && (_month == d._month) && (_day > d._day)))
			return 1;
		return 0;
	}

	int operator<=(const Date& d)
	{
		if(*this > d)
			return 0;
		return 1;
	}

	int operator>=(const Date& d)
	{
		if(*this < d)
			return 0;
		return 1;
	}

	int operator!=(const Date& d)
	{
		if(_year != d._year || _month != d._month || _day != d._day)
			return 1;
		return 0;
	}

	int operator==(const Date& d)
	{
		if(_year == d._year && _month == d._month && _day == d._day)
			return 1;
		return 0;
	}

	Date operator+(int day)
	{
		if(day < 0)
		{
			day = 0 - day;
			return operator-(day);
		}
		Date tmp(*this);
		tmp._day += day;

		while(tmp._day > GetDaysInMonth(tmp._year, tmp._month))
		{
			tmp._day -= GetDaysInMonth(tmp._year, tmp._month);
			if(tmp._month == 12)
			{
				tmp._year++;
				tmp._month = 1;
			}
			else
				tmp._month++;
		}
		return tmp;
	}

	Date operator-(int day)
	{
		Date tmp(*this);
		tmp._day -= day;
		while(tmp._day < 0)
		{
			if(tmp._month == 1)
			{
				tmp._year--;
				tmp._month = 12;
			}
			else
				tmp._month--;
			tmp._day += GetDaysInMonth(tmp._year, tmp._month);
		}
		return tmp;
	}

	int operator-(const Date& d)
	{
		Date MaxDate(*this);
		Date MinDate(d);
		if(MaxDate < MinDate)
		{
			MaxDate = d;
			MinDate = *this;
		}
		int day = 0;
		while(1)
		{
			if(MinDate + day == MaxDate)
				break;
			day++;
		}
		return day;
	}

	Date operator++()
	{
		return (*this = *this + 1);
	}
	Date operator++(int)
	{
		Date tmp(*this);
		*this = *this + 1;
		return tmp;
	}

	Date operator--()
	{
		return (*this = *this - 1);
	}
	Date operator--(int)
	{
		Date tmp(*this);
		*this = *this - 1;
		return tmp;
	}
	~Date()
	{}
private:
	int IsLeapYear(int year)
	{
		if((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
			return 1;
		return 0;
	}
	int GetDaysInMonth(int year, int month)
	{
		int months[] = {0, 31, 28, 31, 30, 31, 30, 31, 31,30, 31, 30, 31};
		if(IsLeapYear(year))
			months[2] = 29;
		return months[month];

	}
public:
		int _year;
		int _month;
		int _day;
};

//int main()
//{
//	Date t1(2017, 7, 24);
//	Date t2(2017, 3, 3);
//	t2 = t1 - 999;
//	cout<<t2._year<<"/"<<t2._month<<"/"<<t2._day<<endl;
//	t2 = t1 + (-999);
//	cout<<t2._year<<"/"<<t2._month<<"/"<<t2._day<<endl;
//	t2 = t1 + 999;
//	cout<<t2._year<<"/"<<t2._month<<"/"<<t2._day<<endl;
//}
