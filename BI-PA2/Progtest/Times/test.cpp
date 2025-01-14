#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
public:
    InvalidDateException ( )
            : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
    return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
    // todo
public:
    CDate(int y, int m, int d);
    ~CDate(){};
    void toDays();
    void toDate();
    bool checkDate(int y, int m, int d);
    ostringstream toFormat() const;

    friend ostream & operator << (ostream & out, CDate c){
        string ye = to_string(c.year);
        string me;
        string de;

        if(c.month < 10){
            me = "0" + to_string(c.month);
        }
        else{
            me = to_string(c.month);
        }
        if(c.day < 10){
            de = "0" + to_string(c.day);
        }
        else{
            de = to_string(c.day);
        }

        out << ye + "-" + me + "-" + de;

        return out;
    };

    friend bool operator >> (istream & in, CDate & a){
        char c;
        int y = a.year;
        int m = a.month;
        int d = a.day;

        in >> a.year >> c >> a.month >> c >> a.day;

        if(!a.checkDate(a.year, a.month, a.day)){
            a.year = y;
            a.month = m;
            a.day = d;
            return false;
        }
        a.toDays();
        return true;
    };

    friend CDate & operator + (CDate & c, int add){
        c.totalDays += add;
        c.toDate();
        //cout << "After adding: " << c.totalDays << endl;
        return c;
    };

    friend CDate & operator - (CDate & c, int sub){
        c.totalDays -= sub;
        c.toDate();
        //cout << "After subtracting: " << c.totalDays << endl;
        return c;
    };

    friend int operator - (CDate & a, CDate & b){
        int res = a.totalDays - b.totalDays;
        //cout << "Amount of days after subtracting two dates: " << res << endl;
        return res;
    };

    friend CDate & operator ++ (CDate & a){
        a.totalDays++;
        a.toDate();
        return a;
    };

    friend CDate & operator -- (CDate & a){
        a.totalDays--;
        a.toDate();
        cout << "OOTALTATLAT: " << a.totalDays << endl;
        return a;
    };

    friend CDate operator ++ (CDate & a, int) {
        CDate tmp(a);
        a.totalDays++;
        a.toDate();
        return tmp;
    };

    friend CDate operator -- (CDate & a, int) {
        CDate tmp(a);
        a.totalDays--;
        a.toDate();
        cout << "OOTALTATLAT: " << a.totalDays << endl;
        return tmp;
    };

    friend bool operator == (CDate & a, CDate & b){
        return a.totalDays == b.totalDays;
    };

    friend bool operator != (CDate & a, CDate & b){
        return a.totalDays != b.totalDays;
    };

    friend bool operator > (CDate & a, CDate & b){
        return a.totalDays > b.totalDays;
    };

    friend bool operator >= (CDate & a, CDate & b){
        return a.totalDays >= b.totalDays;
    };

    friend bool operator < (CDate & a, CDate & b){
        return a.totalDays < b.totalDays;
    };

    friend bool operator <= (CDate & a, CDate & b){
        return a.totalDays <= b.totalDays;
    };

    void printInside();
private:
    int totalDays;
    int year;
    int month;
    int day;
};

ostringstream CDate::toFormat() const{
    string ye = to_string(year);
    string me;
    string de;

    ostringstream res;

    if(month < 10){
        me = "0" + to_string(month);
    }
    else{
        me = to_string(month);
    }
    if(day < 10){
        de = "0" + to_string(day);
    }
    else{
        de = to_string(day);
    }

    res << ye + "-" + me + "-" + de;

    return res;
}

void CDate::toDays(){
    int y = 2000;
    int m = 1;
    totalDays = 0;
    int leap = 0;

    if(year % 4 == 0){
        leap = 1;
    }

    while(y < year){
        if(y % 4 == 0){
            totalDays += 366;
        }
        else{
            totalDays += 365;
        }
        y++;
    }
    while(m < month){
        if(m <= 7) {
            if(m == 2){
                if(leap == 1){
                    totalDays += 29;
                }
                else{
                    totalDays += 28;
                }
            }
            else if(m % 2 == 0){
                totalDays += 30;
            }
            else{
                totalDays += 31;
            }
        }
        else{
            if(m % 2 == 0){
                totalDays += 31;
            }
            else{
                totalDays += 30;
            }
        }
        m++;
    }
    totalDays += day ;
    cout << "TOTAL DAYS: " << totalDays << endl;
}

void CDate::toDate(){
    int days = totalDays;
    int cntYear = 0;
    int cntMonth = 1;
    year = 2000;
    month = 1;
    day = 1;

    while(1){
        if(cntYear % 4 == 0){
            if(days > 366){
                year ++;
                days -= 366;
            }
            else{
                break;
            }
        }
        else{
            if(days > 365){
                year++;
                days -= 365;
            }
            else{
                break;
            }
        }
        cntYear++;
    }

    while(1){
        if(cntMonth <= 7){
            if(cntMonth == 2){
                if(cntYear % 4 == 0){
                    if(days > 29){
                        month++;
                        days -= 29;
                    }
                    else {
                        break;
                    }
                }
                else{
                    if(days > 28){
                        month++;
                        days -= 28;
                    }
                    else{
                        break;
                    }
                }
            }
            else if(cntMonth % 2 == 0){
                if(days > 30){
                    month++;
                    days -= 30;
                }
                else{
                    break;
                }
            }
            else{
                if(days > 31){
                    month++;
                    days -= 31;
                }
                else{
                    break;
                }
            }
        }
        else{
            if(cntMonth % 2 == 0){
                if(days > 31){
                    month++;
                    days -= 31;
                }
                else{
                    break;
                }
            }
            else{
                if(days > 30){
                    month++;
                    days -= 30;
                }
                else{
                    break;
                }
            }
        }
        cntMonth++;
    }

    day = days;


    //cout << "Left days: " << days << endl;

}

bool CDate::checkDate(int y, int m, int d) {
    int leap = 0;

    if(y < 2000 || y > 2030 || m < 1 || m > 12 || d < 1) {
        return false;
    }
    if(y%4 == 0){
        leap = 1;
    }

    if(m <= 7){
        if(m == 2){
            if(leap == 0 && d > 28){
                return false;
            }
            else if(leap == 1 && d > 29){
                return false;
            }
        }
        else if(m%2 == 0 && d > 30) {
            return false;
        }
        else if(m%2 == 1 && d > 31){
            return false;
        }
    }
    else{
        if(m%2 == 0 && d > 31){
            return false;
        }
        else if(m%2 == 1 && d > 30){
            return false;
        }
    }
    year = y;
    month = m;
    day = d;
    return true;
}

CDate::CDate(int y, int m, int d){
    //cout << "Constructor called!" << endl;

    if(!checkDate(y, m, d)){
        throw InvalidDateException();
    }

    CDate::toDays();
}

void CDate::printInside(){
    cout << "Year: " << year << endl;
    cout << "Month: " << month << endl;
    cout << "Day: " << day << endl;
    cout << "---------------" << endl;

}


#ifndef __PROGTEST__
int main ( void )
{
    ostringstream oss;
    istringstream iss;
/*
    CDate a(2000,1,1);
    a.printInside();

    CDate a ( 2000, 1, 2 );
    CDate b ( 2010, 2, 3 );
    CDate c ( 2004, 2, 10 );
    a.printInside();
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2000-01-02" );
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2010-02-03" );
    oss . str ("");
    oss << c;
    assert ( oss . str () == "2004-02-10" );
    a = a + 1500;
    a.printInside();
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2004-02-10" );
    b = b - 2000;
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2004-08-13" );
    assert ( b - a == 185 );
    assert ( ( b == a ) == false );
    assert ( ( b != a ) == true );
     assert ( ( b <= a ) == false );
     assert ( ( b < a ) == false );
     assert ( ( b >= a ) == true );
     assert ( ( b > a ) == true );
     assert ( ( c == a ) == true );
     assert ( ( c != a ) == false );
     assert ( ( c <= a ) == true );
     assert ( ( c < a ) == false );
     assert ( ( c >= a ) == true );
     assert ( ( c > a ) == false );
    a.printInside();
    c.printInside();
     a = ++c;
    a.printInside();
    c.printInside();
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-11" );
    a = --c;
    a.printInside();
    c.printInside();
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-10" );
    a = c++;
    a.printInside();
    c.printInside();
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-11" );
    a = c--;
    a.printInside();
    c.printInside();
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-10" );
    iss . clear ();
    iss . str ( "2015-09-03" );
    assert ( ( iss >> a ) );
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-09-03" );
    a.printInside();
    a = a + 70;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-11-12" );

    CDate d ( 2000, 1, 1 );
    try
    {
      CDate e ( 2000, 32, 1 );
      assert ( "No exception thrown!" == nullptr );
    }
    catch ( ... )
    {
    }
    iss . clear ();
    iss . str ( "2000-12-33" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-11-31" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-02-29" );
    assert ( ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );
    iss . clear ();
    iss . str ( "2001-02-29" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );
*/
    //-----------------------------------------------------------------------------
    // bonus test examples
    //-----------------------------------------------------------------------------
    /*CDate f ( 2000, 5, 12 );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    oss . str ("");
    oss << date_format ( "%Y/%m/%d" ) << f;
    assert ( oss . str () == "2000/05/12" );
    oss . str ("");
    oss << date_format ( "%d.%m.%Y" ) << f;
    assert ( oss . str () == "12.05.2000" );
    oss . str ("");
    oss << date_format ( "%m/%d/%Y" ) << f;
    assert ( oss . str () == "05/12/2000" );
    oss . str ("");
    oss << date_format ( "%Y%m%d" ) << f;
    assert ( oss . str () == "20000512" );
    oss . str ("");
    oss << date_format ( "hello kitty" ) << f;
    assert ( oss . str () == "hello kitty" );
    oss . str ("");
    oss << date_format ( "%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%" ) << f;
    assert ( oss . str () == "121212121212050505200020002000%%%%%" );
    oss . str ("");
    oss << date_format ( "%Y-%m-%d" ) << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-01-1" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-1-01" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-001-01" );
    assert ( ! ( iss >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2000-05-12" );
    iss . clear ();
    iss . str ( "2001-01-02" );
    assert ( ( iss >> date_format ( "%Y-%m-%d" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2001-01-02" );
    iss . clear ();
    iss . str ( "05.06.2003" );
    assert ( ( iss >> date_format ( "%d.%m.%Y" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2003-06-05" );
    iss . clear ();
    iss . str ( "07/08/2004" );
    assert ( ( iss >> date_format ( "%m/%d/%Y" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2004-07-08" );
    iss . clear ();
    iss . str ( "2002*03*04" );
    assert ( ( iss >> date_format ( "%Y*%m*%d" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2002-03-04" );
    iss . clear ();
    iss . str ( "C++09format10PA22006rulez" );
    assert ( ( iss >> date_format ( "C++%mformat%dPA2%Yrulez" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2006-09-10" );
    iss . clear ();
    iss . str ( "%12%13%2010%" );
    assert ( ( iss >> date_format ( "%%%m%%%d%%%Y%%" ) >> f ) );
    oss . str ("");
    oss << f;
    assert ( oss . str () == "2010-12-13" );

    CDate g ( 2000, 6, 8 );
    iss . clear ();
    iss . str ( "2001-11-33" );
    assert ( ! ( iss >> date_format ( "%Y-%m-%d" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "29.02.2003" );
    assert ( ! ( iss >> date_format ( "%d.%m.%Y" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "14/02/2004" );
    assert ( ! ( iss >> date_format ( "%m/%d/%Y" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "2002-03" );
    assert ( ! ( iss >> date_format ( "%Y-%m" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "hello kitty" );
    assert ( ! ( iss >> date_format ( "hello kitty" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "2005-07-12-07" );
    assert ( ! ( iss >> date_format ( "%Y-%m-%d-%m" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-06-08" );
    iss . clear ();
    iss . str ( "20000101" );
    assert ( ( iss >> date_format ( "%Y%m%d" ) >> g ) );
    oss . str ("");
    oss << g;
    assert ( oss . str () == "2000-01-01" );
  */
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
