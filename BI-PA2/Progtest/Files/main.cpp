#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

class CFile // Class to store versions
{
public:
    CFile ():versions(nullptr), curVersion(0), maxVersion(0){}; // Setting default values in the constructor
    ~CFile ();
    CFile (const CFile & source);
    CFile & operator = (const CFile & source);

    bool seek ( uint32_t offset );
    uint32_t read ( uint8_t * dst, uint32_t bytes );
    uint32_t write ( const uint8_t * src, uint32_t bytes );
    uint32_t fileSize () const;
    void addVersion ();
    void truncate ();
    bool undoVersion ();

    void checkData() const;
    void checkVersions() const;
private:

    struct DataState{ // Nested class to store the data
        DataState():data(nullptr),cntInst(new int(1)),curSize(0),maxSize(0),pos(0) {};
        DataState deepCpy() const{ // Method that takes current values of a class instance and allocate separate memory for them (deep copy)
            DataState temp;
            temp.curSize = curSize;
            temp.maxSize = maxSize;
            temp.pos = pos;
            temp.data = new uint8_t[temp.maxSize];
            for(uint32_t i = 0; i < curSize; i++){ // Deep copy = copying every single element to its own new space in the memory
                temp.data[i] = data[i];
            }
            return temp;
        };

        void checkDeep(){ // Checking if instance of DataState class has more than 1 instances (shallow copies), if so, we will convert the instance to deep copy
                          // Because otherwise we would be rewriting some older versions
            if(*(this->cntInst) > 1){
                *this = this->deepCpy();
            }
        };

        DataState (const DataState & source){ // Shallow copy - Copy constructor
            curSize = source.curSize;
            maxSize = source.maxSize;
            pos = source.pos;
            data = source.data;
            cntInst = source.cntInst;
            (*cntInst)++; // Incrementing cntInst to know when we should call delete for dynamic variables AKA shared pointer
        };

        void deleteData() const{ // If instance "dies" this function is called, first it will decrement cntInst by one and then check if all instances "died"
            (*cntInst)--;        // If so, both, dynamic array and cntInst will be deleted
            if(*cntInst == 0){
                delete[] data;
                delete cntInst;
            }
        };

        ~DataState(){ // Destructor that calls different function that takes care of deleting the dynamic variables
            deleteData();
        }

        DataState & operator =(const DataState & source){ // Simply overloaded = operator, very similar to copy constructor in this class
            if(this == &source){                          // Except we are calling method deleteData here to make sure if we are rewriting the last instance of the specific shallow copy
                return *this;                             // Even if that wouldn't be the last instance we need to at least decrement it by one, because that data (pointer) is going to be rewritten
            }

            (*source.cntInst)++;
            curSize = source.curSize;
            maxSize = source.maxSize;
            pos = source.pos;
            deleteData();
            data = source.data;
            cntInst = source.cntInst;
            return *this; // Returning object with new values
        }

        uint8_t * data; // Defining the variables for DataState class
        int * cntInst;

        uint32_t curSize;
        uint32_t maxSize;
        uint32_t pos;
    };
    DataState currentState; // Defining the variables for CFile class

    DataState * versions;
    uint32_t curVersion;
    uint32_t maxVersion;
};

CFile::~CFile() { // There won't be any shallow copies of versions, so simple delete[] would work just fine
    delete[] versions;
}

CFile &CFile::operator = (const CFile & source) { // Overloading operator for deep copy
    if(this == &source){                          // Same as copy constructor, except again we need to make sure we are deleting previous array of versions, delete to null pointer is fine
        return *this;                             // Delete function is checking that
    }

    currentState = source.currentState;
    curVersion = source.curVersion;
    maxVersion = source.maxVersion;
    delete[] versions;
    versions = new DataState[maxVersion];
    for(uint32_t i = 0; i < curVersion; i++){
        versions[i] = source.versions[i];
    }
    return *this;
}

CFile::CFile(const CFile & source) { // Deep copy constructor for versions, no need for delete versions, because it's being copied while creating, that means no previous data in there
    currentState = source.currentState;
    curVersion = source.curVersion;
    maxVersion = source.maxVersion;
    versions = new DataState[maxVersion];

    for(uint32_t i = 0; i < curVersion; i++){
        versions[i] = source.versions[i];
    }
}

void CFile::addVersion() { // Adding version to array of DataState versions
    if(curVersion >= maxVersion){ // Checking if array is big enough, if not reallocating the array to bigger size using extra tempArray
        maxVersion = maxVersion*2+5;
        auto * tempArray = new DataState[maxVersion];

        for(uint32_t i = 0; i < curVersion; i++){
            tempArray[i] = versions[i];
        }

        delete[] versions;

        versions = tempArray;
    }
    versions[curVersion] = currentState; // Adding another version to the end of the versions array
    curVersion++;
}

bool CFile::undoVersion() { // Going back one version
    if(curVersion == 0){ // In case version is 0, it is the last version, we can't go back, therefore false has to be returned
        return false;
    }
    curVersion--;

    currentState = versions[curVersion]; // Making currentState (currentData) the same as the last version in versions array
    return true;
}

uint32_t CFile::write(const uint8_t *src, uint32_t bytes) { // Writing new data to currentState
    uint32_t index = currentState.pos;
    currentState.checkDeep(); // This is going to be in every single method where we change some variables in currentState, described above
    if(currentState.curSize+bytes >= currentState.maxSize){ // Reallocating the data array inside currentState
        currentState.maxSize = currentState.maxSize*2+bytes+5;
        auto *tempArray = new uint8_t[currentState.maxSize];

        for(uint32_t i = 0; i < currentState.curSize; i++){
            tempArray[i] = currentState.data[i];
        }

        delete[] currentState.data;

        currentState.data = tempArray;
    }

    if(currentState.pos + bytes > currentState.curSize){
        currentState.curSize += (currentState.pos + bytes) - currentState.curSize; // Setting the currentSize of the array, depending on bytes and if they will exceed the currentSize
    }
    currentState.pos += bytes;

    for(uint32_t i = 0; i < bytes; i++){ // Finally saving each element to data
        currentState.data[index] = src[i];
        index++;
    }
    return bytes; // Always have to return bytes, because what we get, have to be saved to data
}

uint32_t CFile::read(uint8_t *dst, uint32_t bytes) { // Saving desired amount (if possible) of values from our data to dst array
    uint32_t dstSize = 0;
    currentState.checkDeep(); // Again checking
    for(uint32_t i = 0; i < bytes; i++){ // Saving values from data to dst until we reach the end of our "file" (array) or we have already copied the amount of bytes that we wanted
        if(currentState.pos < currentState.curSize) {
            dst[i] = currentState.data[currentState.pos];
            currentState.pos++;
            dstSize++;
        }
        else{
            break;
        }
    }

    return dstSize; // Returning how many bytes we have read total
}

bool CFile::seek(uint32_t offset) {
    currentState.checkDeep();
    if(offset <= currentState.curSize){ // Number is unsigned, can't be smaller than 0, so we are only checking if it's within our current size
        currentState.pos = offset; // Setting new position
        return true;
    }
    return false;
}

uint32_t CFile::fileSize() const { // Method that returns current size
    return currentState.curSize;
}

void CFile::truncate() { // Method that shrinks the data array to the value of position, we don't mind data behind, either they will get overwritten or deleted by destructor
    currentState.checkDeep();
    currentState.curSize = currentState.pos;
}

void CFile::checkData() const{ // Rest are my functions that I used to check values
    for(uint32_t i = 0; i < currentState.curSize; i++){
        cout << "ELEMENT: " << i << " is : " << currentState.data[i] << endl;
    }
    cout << endl;
}

void CFile::checkVersions() const{
    cout << "Amount of versions: " << curVersion << endl;
    for(uint32_t i = 0; i < curVersion; i++){
        cout << "Version " << i << " pos is: " << versions[i].pos << endl;
        cout << "DATA: " << endl;
        for(uint32_t k = 0; k < currentState.curSize; k++){
            cout << currentState.data[k] << ", ";
        }
        cout << endl;
    }
    cout << endl;
}

#ifndef __PROGTEST__
bool writeTest ( CFile & x,  const initializer_list<uint8_t> & data, uint32_t wrLen ){
    return x . write ( data . begin (), data . size () ) == wrLen;
}

bool readTest ( CFile & x, const initializer_list<uint8_t> & data, uint32_t rdLen ){
    uint8_t  tmp[100];
    uint32_t idx = 0;

    if ( x . read ( tmp, rdLen ) != data . size ()) {
        return false;
    }
    for ( auto v : data )
        if ( tmp[idx++] != v ) {
            return false;
        }
    return true;
}

int main ()
{
    // Some of my tests that I was using
/*
    CFile f0;
    assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
    f0 . addVersion();
    f0 . seek(0);
    assert ( writeTest ( f0, { 10, 20, 30, 40 }, 4 ) );
    f0 . addVersion();
    f0 . seek(0);
    assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
    f0 . addVersion();
    f0 . seek(0);
    assert ( writeTest ( f0, { 10, 20, 30, 40, 50 }, 5 ) );
    f0 . addVersion();
    f0 . undoVersion();
    f0. seek(5);
    f0 . addVersion();
    f0 . undoVersion();
    f0 . undoVersion();


    assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
    f0 . addVersion();
    f0 . seek(0);
    assert ( writeTest ( f0, { 10, 20, 30, 40 }, 4 ) );
    f0 . addVersion();
    f0 . seek(0);
    assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
    f0 . addVersion();
    f0 . seek(2);
    assert ( writeTest ( f0, { 50, 80, 90, 100 }, 4 ) );
    f0 . addVersion();
    f0 . undoVersion();
    f0 . undoVersion();
    f0 . seek(3);
    assert ( writeTest ( f0, { 77, 88, 99 }, 3 ) );


    assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
    assert ( f0 . fileSize () == 3 );
    assert ( writeTest ( f0, { 60, 70, 80 }, 3 ) );
    assert ( f0 . fileSize () == 6 );
    assert ( f0 . seek ( 2 ));
    assert ( writeTest ( f0, { 5, 4 }, 2 ) );
    assert ( f0 . fileSize () == 6 );
    assert ( f0 . seek ( 1 ));
    assert ( readTest ( f0, { 20, 5, 4, 70, 80 }, 7 ));
    assert ( f0 . seek ( 3 ));
    f0 . addVersion();
    assert ( f0 . seek ( 6 ));
    assert ( writeTest ( f0, { 100, 101, 102, 103 }, 4 ) );
    f0 . addVersion();
    assert ( f0 . seek ( 5 ));
    CFile f1 ( f0 );
    f0 . truncate ();
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70 }, 20 ));
    assert ( f0 . undoVersion () );
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20 ));
    assert ( f0 . undoVersion () );
    assert ( f0 . seek ( 0 ));
    assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80 }, 20 ));
    assert ( !f0 . seek ( 100 ));
    assert ( writeTest ( f1, { 200, 210, 220 }, 3 ) );
    assert ( f1 . seek ( 0 ));
    assert ( readTest ( f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20 ));
    assert ( f1 . undoVersion () );
    assert ( f1 . undoVersion () );
    assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
    assert ( !f1 . undoVersion () );
*/
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
