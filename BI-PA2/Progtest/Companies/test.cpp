#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CVATRegister
{
public:
    CVATRegister   ( void );
    ~CVATRegister  ( void );
    bool          newCompany     ( const string    & name, const string & addr, const string & taxID );
    bool          cancelCompany  ( const string    & name, const string & addr );
    bool          cancelCompany  ( const string    & taxID );
    bool          invoice        ( const string    & taxID,  unsigned int      amount );
    bool          invoice        ( const string    & name, const string & addr, unsigned int amount );
    bool          audit          ( const string    & name, const string & addr, unsigned int & sumIncome ) const;
    bool          audit          ( const string    & taxID, unsigned int & sumIncome ) const;
    bool          firstCompany   ( string          & name, string & addr ) const;
    bool          nextCompany    ( string          & name, string & addr ) const;
    unsigned int  medianInvoice  ( void ) const;
private:
    // todo

    class company{
    public:
        string name;
        string address;
        string id;
        unsigned int sum;
        static bool comparefunc(const company & a, const company & b);
        static bool comparefuncid(const company & a, const company & b);
    };

    vector<company> companies;
    vector<company> companiesID;
    vector<unsigned int> minimal;
    vector<unsigned int> maximal;
};

bool CVATRegister::company::comparefunc(const company & a, const company & b){
    int namecomparing = strcasecmp(a.name.c_str(), b.name.c_str());

    if (namecomparing){
        return namecomparing < 0;
    }
    return strcasecmp(a.address.c_str(), b.address.c_str()) < 0;
}

bool CVATRegister::company::comparefuncid(const company & a, const company & b){
    return strcmp(a.id.c_str(), b.id.c_str()) < 0;
}

bool CVATRegister::newCompany(const string    & name, const string & addr, const string & taxID){
    // Check if the input already exists in the array
    for(long unsigned int i = 0; i < companies.size(); i++){
        if(strcasecmp(name.c_str(), (companies[i].name).c_str()) == 0){
            if(strcasecmp(addr.c_str(), (companies[i].address).c_str()) == 0){
                return false;
            }
        }
        if(taxID == companies[i].id){
            return false;
        }
    }

    // Creating an object from the input
    company temp;
    temp.name = name;
    temp.address = addr;
    temp.id = taxID;
    temp.sum = 0;

    // Saving to array sorted by names and addresses
    auto it = lower_bound(companies.begin(), companies.end(), temp, company::comparefunc);
    companies.insert(it, temp);

    //Saving to array sorted by ID's
    auto itID = lower_bound(companiesID.begin(), companiesID.end(), temp, company::comparefuncid);
    companiesID.insert(itID, temp);

    return true;
}

bool CVATRegister::cancelCompany(const string & name, const string & addr){
    company temp;
    temp.name = name;
    temp.address = addr;

    auto it = lower_bound(companies.begin(), companies.end(), temp, company::comparefunc);

    if(it == companies.end()){
        return false;
    }

    //cout << "POZICE JE: " << it - companies.begin() << endl;

    if(strcasecmp((companies[it - companies.begin()].name).c_str(), name.c_str()) == 0 && strcasecmp((companies[it - companies.begin()].address).c_str(), addr.c_str()) == 0){
        company temp2;
        temp2.id = companies[it - companies.begin()].id;
        companies.erase(it);

        auto itID = lower_bound(companiesID.begin(), companiesID.end(), temp2, company::comparefuncid);
        companiesID.erase(itID);
        return true;
    }

    return false;
}

bool CVATRegister::cancelCompany(const string & taxID){
    company temp;
    temp.id = taxID;

    auto itID = lower_bound(companiesID.begin(), companiesID.end(), temp, company::comparefuncid);

    if(itID == companiesID.end()){
        return false;
    }

    // cout << "POZICE JE: " << itID - companiesID.begin() << endl;

    if(taxID == companiesID[itID - companiesID.begin()].id){
        company temp2;
        temp2.name = companiesID[itID - companiesID.begin()].name;
        temp2.address = companiesID[itID - companiesID.begin()].address;
        companiesID.erase(itID);

        auto it = lower_bound(companies.begin(), companies.end(), temp2, company::comparefunc);
        companies.erase(it);
        return true;
    }
    return false;
}

bool CVATRegister::invoice(const string & name, const string & addr, unsigned int amount){
    company temp;
    temp.name = name;
    temp.address = addr;

    auto it = lower_bound(companies.begin(), companies.end(), temp, company::comparefunc);

    if(it == companies.end()){
        return false;
    }
    //cout << "POZICE JE: " << it - companies.begin() << endl;

    if(strcasecmp((companies[it - companies.begin()].name).c_str(), name.c_str()) == 0 && strcasecmp((companies[it - companies.begin()].address).c_str(), addr.c_str()) == 0){
        company temp2;
        temp2.id = companies[it - companies.begin()].id;
        companies[it - companies.begin()].sum += amount;

        auto itID = lower_bound(companiesID.begin(), companiesID.end(), temp2, company::comparefuncid);
        companiesID[itID - companiesID.begin()].sum += amount;

        if(maximal.empty() || amount < maximal.front()){
            maximal.push_back(amount);
            push_heap(maximal.begin(), maximal.end());
            if(maximal.size() > minimal.size() + 1){
                pop_heap(maximal.begin(), maximal.end());
                minimal.push_back(maximal.back());
                maximal.pop_back();
                push_heap(minimal.begin(), minimal.end(), greater<unsigned int>{});
            }
        }
        else {
            minimal.push_back(amount);
            push_heap(minimal.begin(), minimal.end(), greater<unsigned int>{});
            if(minimal.size() > maximal.size()){
                pop_heap(minimal.begin(), minimal.end(), greater<unsigned int>{});
                maximal.push_back(minimal.back());
                minimal.pop_back();
                push_heap(maximal.begin(), maximal.end());
            }
        }


        return true;
    }
    return false;
}

bool CVATRegister::invoice(const string & taxID,  unsigned int amount){
    company temp;
    temp.id = taxID;

    auto itID = lower_bound(companiesID.begin(), companiesID.end(), temp, company::comparefuncid);

    if(itID == companiesID.end()){
        return false;
    }
    //cout << "POZICE JE: " << itID - companiesID.begin() << endl;
    if(taxID == companiesID[itID - companiesID.begin()].id){
        company temp2;
        temp2.name = companiesID[itID - companiesID.begin()].name;
        temp2.address = companiesID[itID - companiesID.begin()].address;
        companiesID[itID - companiesID.begin()].sum += amount;

        auto it = lower_bound(companies.begin(), companies.end(), temp2, company::comparefunc);
        companies[it - companies.begin()].sum += amount;

        if(maximal.empty() || amount < maximal.front()){
            maximal.push_back(amount);
            push_heap(maximal.begin(), maximal.end());
            if(maximal.size() > minimal.size() + 1){
                pop_heap(maximal.begin(), maximal.end());
                minimal.push_back(maximal.back());
                maximal.pop_back();
                push_heap(minimal.begin(), minimal.end(), greater<unsigned int>{});
            }
        }
        else {
            minimal.push_back(amount);
            push_heap(minimal.begin(), minimal.end(), greater<unsigned int>{});
            if(minimal.size() > maximal.size()){
                pop_heap(minimal.begin(), minimal.end(), greater<unsigned int>{});
                maximal.push_back(minimal.back());
                minimal.pop_back();
                push_heap(maximal.begin(), maximal.end());
            }
        }

        return true;
    }
    //cout << companiesID[itID - companiesID.begin()].id << " tu neni" << endl;
    return false;
}

unsigned int CVATRegister::medianInvoice()const{

    /*for(long unsigned int i = 0; i < invoices.size(); i++){
        cout << invoices[i] << endl;
    }*/
    /*cout << "MIN HEAP JE: " << endl;
    for(const unsigned int t : minimal){
          cout << t << " ";
    }
    cout << "\n" << endl;
    cout << "MAX HEAP JE: " << endl;
    for(const unsigned int t : maximal){
        cout << t << " ";
    }*/

    //cout << "\n" << endl;
    if(maximal.size() == minimal.size()){
        if(maximal.empty()){
            return 0;
        }
        return minimal.front();
    }
    return maximal.front();
}

bool CVATRegister::audit(const string & name, const string & addr, unsigned int & sumIncome)const{ // const
    company temp;
    temp.name = name;
    temp.address = addr;

    auto it = lower_bound(companies.begin(), companies.end(), temp, company::comparefunc);

    if(it == companies.end()){
        return false;
    }

    if(strcasecmp((companies[it - companies.begin()].name).c_str(), name.c_str()) == 0 && strcasecmp((companies[it - companies.begin()].address).c_str(), addr.c_str()) == 0){
        sumIncome = companies[it - companies.begin()].sum;
        return true;
    }
    return false;
}

bool CVATRegister::audit(const string & taxID, unsigned int & sumIncome)const{
    company temp;
    temp.id = taxID;

    auto itID = lower_bound(companiesID.begin(), companiesID.end(), temp, company::comparefuncid);

    if(itID == companiesID.end()){
        return false;
    }

    if(taxID == companiesID[itID - companiesID.begin()].id){
        sumIncome = companiesID[itID - companiesID.begin()].sum;
        return true;
    }
    return false;
}

bool CVATRegister::firstCompany(string & name, string & addr)const{
    if(companies.empty()){
        return false;
    }
    name = companies[0].name;
    addr = companies[0].address;
    return true;
}

bool CVATRegister::nextCompany(string & name, string & addr)const{
    company temp;
    temp.name = name;
    temp.address = addr;

    auto it = lower_bound(companies.begin(), companies.end(), temp, company::comparefunc);

    if(it == companies.end()){
        return false;
    }

    if(strcasecmp((companies[it - companies.begin()].name).c_str(), name.c_str()) == 0 && strcasecmp((companies[it - companies.begin()].address).c_str(), addr.c_str()) == 0){
        if((it + 1) == companies.end()){
            return false;
        }
        name = companies[it - companies.begin() + 1].name;
        addr = companies[it - companies.begin() + 1].address;
        return true;
    }
    return false;
}

CVATRegister::CVATRegister(void){}
CVATRegister::~CVATRegister(void){}

#ifndef __PROGTEST__
int               main           ( void )
{
    string name, addr;
    unsigned int sumIncome;

    CVATRegister b1;
    assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
    assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
    assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
    assert ( b1 . invoice ( "666/666", 2000 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "666/666/666", 3000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 4000 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
    assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
    assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
    assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . cancelCompany ( "666/666" ) );
    assert ( b1 . medianInvoice () == 4000 );
    assert ( b1 . invoice ( "123456", 100 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 300 ) );
    assert ( b1 . medianInvoice () == 3000 );
    assert ( b1 . invoice ( "123456", 200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 230 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 830 ) );
    assert ( b1 . medianInvoice () == 830 );
    assert ( b1 . invoice ( "123456", 1830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 1830 );
    assert ( b1 . invoice ( "123456", 2830 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . invoice ( "123456", 3200 ) );
    assert ( b1 . medianInvoice () == 2000 );
    assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
    assert ( ! b1 . nextCompany ( name, addr ) );
    assert ( b1 . cancelCompany ( "123456" ) );
    assert ( ! b1 . firstCompany ( name, addr ) );

    CVATRegister b2;
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
    assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
    assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
    assert ( b2 . medianInvoice () == 0 );
    assert ( b2 . invoice ( "ABCDEF", 1000 ) );
    assert ( b2 . medianInvoice () == 1000 );
    assert ( b2 . invoice ( "abcdef", 2000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . invoice ( "1234567", 100 ) );
    assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
    assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
    assert ( ! b2 . audit ( "1234567", sumIncome ) );
    assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
    assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
    assert ( ! b2 . cancelCompany ( "1234567" ) );
    assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
    assert ( b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . medianInvoice () == 2000 );
    assert ( ! b2 . cancelCompany ( "abcdef" ) );
    assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
    assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
    assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
