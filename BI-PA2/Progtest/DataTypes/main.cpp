#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDataTypes{
    string name;
    int size;
public:
    CDataTypes( const string & name,const int size){
        this->name = name;
        this->size = size;
    }
    virtual ~CDataTypes() = default;
    virtual CDataTypes * clone () const = 0;

    string getName() const {
        return name;
    };

    virtual int getSize() const {
        return size;
    };

    virtual ostream & printStuff(ostream & out) const{
        return out << name;
    };

    virtual bool operator == (const CDataTypes & other) const = 0;
    bool operator != (const CDataTypes & src) const {
        return !(*this==src);
    };


};

ostream & operator << (ostream & out, const CDataTypes & source) {
    return source.printStuff(out);
}

class CDataTypeInt : public CDataTypes
{

public:
    CDataTypeInt() : CDataTypes("int", 4){};
    ~CDataTypeInt() override = default;

    CDataTypes * clone() const override{
        auto temp = new CDataTypeInt(*this);
        return temp;
    };

    bool operator == (const CDataTypes & other) const override {
        if(this->getName() == other.getName()){
            return true;
        }
        return false;
    }

};
class CDataTypeDouble : public CDataTypes
{

public:

    CDataTypeDouble() : CDataTypes("double", 8){};

    ~CDataTypeDouble() override = default;

    CDataTypes * clone() const override{
        auto temp = new CDataTypeDouble(*this);
        return temp;
    };

    bool operator == (const CDataTypes & other) const override {
        if(this->getName() == other.getName()){
            return true;
        }
        return false;
    }
};
class CDataTypeEnum : public CDataTypes
{
  vector<string> storage;

public:
    CDataTypeEnum() : CDataTypes("enum", 4){};
    ~CDataTypeEnum() override = default;


    CDataTypeEnum & add(const string & src){
        for(const auto & i : storage){
            if(i == src){
                throw invalid_argument("Duplicate enum value: " + src);
            }
        }
        storage.push_back(src);
        return *this;
    }

    ostream & printStuff(ostream & out) const override{
        out << getName() + "{";
        for(const auto & i : storage){
            out << i;
            if(i != storage[storage.size()-1]){
                out << ",";
            }
        }
        out << "}";
        return out;
    };

    CDataTypes * clone() const override{
        auto temp = new CDataTypeEnum(*this);
        return temp;
    };

    bool operator == (const CDataTypes & other) const override{
        auto enumOther = dynamic_cast<const CDataTypeEnum*>(&other);
        if(enumOther == nullptr){
            return false;
        }
        if(this->getName() != enumOther->getName() || this->storage.size() != enumOther->storage.size()){
            return false;
        }
        for(long unsigned int i = 0; i < this->storage.size(); i++){
            if(this->storage[i] != enumOther->storage[i]){
                return false;
            }
        }
        return true;
    }
};
class CDataTypeStruct : public CDataTypes
{
  vector<pair<shared_ptr<CDataTypes>, string>> content;

public:
    CDataTypeStruct() : CDataTypes("struct",0){};
    ~CDataTypeStruct() override = default;

    CDataTypes * clone() const override{
        auto temp = new CDataTypeStruct(*this);
        return temp;
    };

    ostream & printStuff(ostream & out) const override{
        out << getName() + "{";
        for(const auto & i : content){
            out << *(i.first);
            out << i.second + ";";
        }
        out << "}";
        return out;
    };

    CDataTypeStruct & addField(const string & fieldName, const CDataTypes & type){
        for(const auto & i : content){
            if(i.second == fieldName){
                throw invalid_argument("Duplicate field: " + fieldName );
            }
        }
        content.emplace_back(type.clone(), fieldName);
        return *this;
    };

    bool operator == (const CDataTypes & other) const override{
        auto otherStruct = dynamic_cast<const CDataTypeStruct*>(&other);
        if(otherStruct == nullptr){
            return false;
        }

        if(this->getName() != other.getName() || this->content.size() != otherStruct->content.size()){
            return false;
        }
        for(long unsigned int i = 0; i < this->content.size(); i++){
            if(*(this->content[i].first) != *(otherStruct->content[i].first)){
                return false;
            }
        }
        return true;
    }

    CDataTypes & field(const string & name) const{
        for(const auto & i : content){
            if(i.second == name){
                return *(i.first);
            }
        }
        throw invalid_argument("Unknown field: " + name);
    };

    int getSize() const override{
        int totalSize = 0;
        for(const auto & i : content){
            //cout << i.first->getName() << "and value " << i.first->getSize() << endl;
            totalSize += i.first->getSize();
        }
        //cout << "Total size je: " << totalSize << endl;
        return totalSize;
    };
};
#ifndef __PROGTEST__
static bool whitespaceMatch ( const string & a, const string & b )
{
  // todo
  return true;
}

template <typename T_>
static bool whitespaceMatch ( const T_ & x, const string & ref ){
    ostringstream oss;
    oss << x;
    return whitespaceMatch ( oss . str (), ref );
}

int main ()
{

    CDataTypeStruct s;
    s.addField("bla", CDataTypeEnum().add("h").add("aha"));
    cout << s.field("bla");

    CDataTypeStruct  a = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () .
                          add ( "NEW" ) .
                          add ( "FIXED" ) .
                          add ( "BROKEN" ) .
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );

    CDataTypeStruct b = CDataTypeStruct () .
                         addField ( "m_Length", CDataTypeInt () ) .
                         addField ( "m_Status", CDataTypeEnum () .
                           add ( "NEW" ) .
                           add ( "FIXED" ) .
                           add ( "BROKEN" ) .
                           add ( "READY" ) ).
                         addField ( "m_Ratio", CDataTypeDouble () );

     CDataTypeStruct c =  CDataTypeStruct () .
                         addField ( "m_First", CDataTypeInt () ) .
                         addField ( "m_Second", CDataTypeEnum () .
                           add ( "NEW" ) .
                           add ( "FIXED" ) .
                           add ( "BROKEN" ) .
                           add ( "DEAD" ) ).
                         addField ( "m_Third", CDataTypeDouble () );

     CDataTypeStruct  d = CDataTypeStruct () .
                         addField ( "m_Length", CDataTypeInt () ) .
                         addField ( "m_Status", CDataTypeEnum () .
                           add ( "NEW" ) .
                           add ( "FIXED" ) .
                           add ( "BROKEN" ) .
                           add ( "DEAD" ) ).
                         addField ( "m_Ratio", CDataTypeInt () );/**/

     assert ( whitespaceMatch ( a, "struct\n"
     "{\n"
     "  int m_Length;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    DEAD\n"
     "  } m_Status;\n"
     "  double m_Ratio;\n"
     "}") );

     assert ( whitespaceMatch ( b, "struct\n"
     "{\n"
     "  int m_Length;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    READY\n"
     "  } m_Status;\n"
     "  double m_Ratio;\n"
     "}") );

     assert ( whitespaceMatch ( c, "struct\n"
     "{\n"
     "  int m_First;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    DEAD\n"
     "  } m_Second;\n"
     "  double m_Third;\n"
     "}") );

     assert ( whitespaceMatch ( d, "struct\n"
     "{\n"
     "  int m_Length;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    DEAD\n"
     "  } m_Status;\n"
     "  int m_Ratio;\n"
     "}") );

     assert ( a != b );
     assert ( a == c );
     assert ( a != d );
     assert ( a . field ( "m_Status" ) == CDataTypeEnum () . add ( "NEW" ) . add ( "FIXED" ) . add ( "BROKEN" ) . add ( "DEAD" ) );
     assert ( a . field ( "m_Status" ) != CDataTypeEnum () . add ( "NEW" ) . add ( "BROKEN" ) . add ( "FIXED" ) . add ( "DEAD" ) );
     assert ( a != CDataTypeInt() );
     assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
     "{\n"
     "  NEW,\n"
     "  FIXED,\n"
     "  BROKEN,\n"
     "  DEAD\n"
     "}") );

     CDataTypeStruct aOld = a;
     b . addField ( "m_Other", CDataTypeDouble ());

     a . addField ( "m_Sum", CDataTypeInt ());

     assert ( a != aOld );
     assert ( a != c );
     assert ( aOld == c );
     assert ( whitespaceMatch ( a, "struct\n"
     "{\n"
     "  int m_Length;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    DEAD\n"
     "  } m_Status;\n"
     "  double m_Ratio;\n"
     "  int m_Sum;\n"
     "}") );

     assert ( whitespaceMatch ( b, "struct\n"
     "{\n"
     "  int m_Length;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    READY\n"
     "  } m_Status;\n"
     "  double m_Ratio;\n"
     "  double m_Other;\n"
     "}") );

     c . addField ( "m_Another", a . field ( "m_Status" ));

     assert ( whitespaceMatch ( c, "struct\n"
     "{\n"
     "  int m_First;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    DEAD\n"
     "  } m_Second;\n"
     "  double m_Third;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    DEAD\n"
     "  } m_Another;\n"
     "}") );

     d . addField ( "m_Another", a . field ( "m_Ratio" ));

     assert ( whitespaceMatch ( d, "struct\n"
     "{\n"
     "  int m_Length;\n"
     "  enum\n"
     "  {\n"
     "    NEW,\n"
     "    FIXED,\n"
     "    BROKEN,\n"
     "    DEAD\n"
     "  } m_Status;\n"
     "  int m_Ratio;\n"
     "  double m_Another;\n"
     "}") );

     assert ( a . getSize () == 20 );
     assert ( b . getSize () == 24 );
     try
     {
     a . addField ( "m_Status", CDataTypeInt () );
     assert ( "addField: missing exception!" == nullptr );
     }
     catch ( const invalid_argument & e )
     {
     assert ( e . what () == "Duplicate field: m_Status"sv );
     }

     try
     {
     cout << a . field ( "m_Fail" ) << endl;
     assert ( "field: missing exception!" == nullptr );
     }
     catch ( const invalid_argument & e )
     {
     assert ( e . what () == "Unknown field: m_Fail"sv );
     }

     try
     {
     CDataTypeEnum en;
     en . add ( "FIRST" ) .
          add ( "SECOND" ) .
          add ( "FIRST" );
     assert ( "add: missing exception!" == nullptr );
     }
     catch ( const invalid_argument & e )
     {
     assert ( e . what () == "Duplicate enum value: FIRST"sv );
     }

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
