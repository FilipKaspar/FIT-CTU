#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

struct huffTree{
    int           m_Bytes;
    unsigned char m_Value[5];
    huffTree *    m_Left;
    huffTree *    m_Right;
};

void deleteTree(huffTree * tr){
    if(tr == nullptr){
        return;
    }
    deleteTree(tr->m_Left);
    deleteTree(tr->m_Right);

    free(tr);
}

bool readFile(queue<int> & storage, const char * inFileName){
    ifstream file(inFileName, ios::binary | ios::in);

    if(file.is_open()){
        char c;
        int tmp;
        while (file.get(c))
        {   
            if(!file.good()){
                return false;
            }

            for (int i = 7; i >= 0; i--){ // Code used from https://stackoverflow.com/questions/22390641/read-file-by-bits-c . Only those 2 lines
            tmp = ((c >> i) & 1);
            //cout << tmp;
            storage.push(tmp);
            }
        }
    } else{
        return false;
    }
    if(!file.eof()){
        return false;
    }

    return true;
}

int binToAscii(queue<int> & bits){
    int num = 0;
    int temp = 128; // Bude 8 bitů
    for(int x = 7; x >= 0; x--){ // Taktéž 8 bitů
        if(bits.front() == 1){
            num = num + temp;
        }
        temp = temp / 2;
        bits.pop();
    }
    return num;
}

void createList(huffTree * tr, queue<int> & storage){
    if(storage.empty()){
        throw invalid_argument("NO");
    }
    if(storage.front() == 0){
        // cout << "Tree made" << endl;
        tr->m_Left = (huffTree*)malloc(sizeof(huffTree));
        tr->m_Right = (huffTree*)malloc(sizeof(huffTree));
        storage.pop();
        createList(tr->m_Left, storage);
        createList(tr->m_Right, storage);
    } else{
       // cout << "ASCII inserted" << endl;
        storage.pop();
        int repeats = 0;
        queue<int> bits;
        int rep = 0;
        tr->m_Bytes = 0;
        if(storage.front() == 1){
            for(int i = 0; i < 8; i++){ //muze byt 5x jednicka == neni utf dodelat
                if(storage.front() == 1 && rep == 0){
                    repeats++;
                    //cout << "REPEATS: " << repeats << endl;
                    if(repeats >= 5){
                        throw invalid_argument("NO");
                    }

                    tr->m_Bytes++;
                } else {
                    rep = 1;
                }
                bits.push(storage.front());
                storage.pop();
            }
            if(repeats == 1){
                throw invalid_argument("NO");
            }
            for(int i = 0; i < repeats-1; i++){
                int num = binToAscii(bits);
                //cout << "NUM: " << num;
                tr->m_Value[i] = num;

                for(int k = 0; k < 8; k++){ //muze byt 5x jednicka == neni utf dodelat
                    bits.push(storage.front());
                    if(k == 0 && storage.front() != 1){
                        throw invalid_argument("NO");
                    } else if(k == 1 && storage.front() != 0){
                        throw invalid_argument("NO");
                    }
                    storage.pop();
                }
            }
            tr->m_Value[repeats-1] = binToAscii(bits);
            tr->m_Left = nullptr;
            tr->m_Right = nullptr;
            unsigned int a = ((unsigned int)(((unsigned char)tr->m_Value[0]) << 24)
            | (((unsigned char)tr->m_Value[1]) << 16)
            | (((unsigned char)tr->m_Value[2]) << 8)
            | ((unsigned char)tr->m_Value[3]));
            if(repeats == 4 && a > 4103061439){
                cout << a;
                throw invalid_argument("NO");
            }
        } else{
            int num = binToAscii(storage);
            tr->m_Bytes = 1;
            tr->m_Value[0] = num;
            tr->m_Left = nullptr;
            tr->m_Right = nullptr;
        }
        
    }
}

string getValue(huffTree * tr, queue<int> * storage){
    string c;
    if((*storage).front() == 0){
        if(tr->m_Left == nullptr){
            return "-1-1-1-11-+2+7";
        }
        tr = tr->m_Left;
        //cout << "Left" << endl;
    } else {
       if(tr->m_Right == nullptr){
            return "-1-1-1-11-+2+7";
        }
        tr = tr->m_Right;
        //cout << "Right" << endl;
    }

    (*storage).pop();
    if(tr->m_Left == nullptr && tr->m_Right == nullptr){
        //cout << "saving value" << endl;
        string temp = "";
        stringstream ss;
        ss.write(reinterpret_cast<const char*>(tr->m_Value), tr->m_Bytes);
        //cout << ss.str();
        temp = temp + ss.str();
        return temp;        
    }
    c = getValue(tr, storage);

    return c;
}

string getString(huffTree * tr, queue<int> * storage){
    string res = "";
    string c;
    int x = 0;

    int amount = 0;
    int tmp = 2048;

    if((*storage).front() == 0){
        (*storage).pop();
        for(int y = 0; y < 12; y++){
            if((*storage).front() == 1){
                amount = amount + tmp;
            }
            //cout << (*storage).front() << " amount: " << amount << " tmp: " << tmp << endl;
            tmp = tmp / 2;
            (*storage).pop();
        }
    } else {
        (*storage).pop();
        amount = 4096;
    }

    //cout << amount << endl;
    while(x < amount){ //Need to know how many characters I want to extract
        c = getValue(tr, storage);
        if(c == "-1-1-1-11-+2+7"){
            return "-1-1-1-11-+2+7";
        }
        //cout << c;
        res = res + c;
        x++;
        //cout << (*storage).size() << endl;
        if(x < amount && (*storage).size() == 0){
            return "-1-1-1-11-+2+7";
        }
    }
    return res;
}

string saveFile(huffTree * tr, queue<int> * storage){
    string total = "";
    string temp;
    //cout << (*storage).front() << endl;
    while((*storage).front() == 1){
        temp = getString(tr, storage);
        if(temp == "-1-1-1-11-+2+7"){
            return temp;
        }
        total = total + temp;
    }
    temp = getString(tr, storage);
    if(temp == "-1-1-1-11-+2+7"){
        return temp;
    }
    total = total + temp;
    return total;
}

bool decompressFile ( const char * inFileName, const char * outFileName )
{
    string result = "";
    queue<int> storage;
    bool status;

    status = readFile(storage, inFileName);
    if(!status){
        return false;
    }

    huffTree * head = (huffTree*)malloc(sizeof(huffTree));
    head->m_Left = nullptr;
    head->m_Right = nullptr;

    try{
        createList(head, storage);
    } catch(...){
        return false;
    }
    
    result = saveFile(head, &storage);
    if(result == "-1-1-1-11-+2+7"){
        deleteTree(head);
        return false;
    }

    ofstream out;
    out.open(outFileName);
    out << result;
    if(!out.good()){
        remove(outFileName);
        deleteTree(head);
        return false;
    }
    out.close();

    deleteTree(head);

    return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
    ifstream orig(fileName1);
    ifstream temp(fileName2);
    string s1 = "";
    string s2 = "";
    string tmp;

    while(getline(orig, tmp)) {
        s1 = s1 + tmp;
    }
    while(getline(temp, tmp)) {
        s2 = s2 + tmp;
    }
    //cout << s1 << endl;
    //cout << s2 << endl;
    if(s1 == s2){
        return true;
    }
    return false;
}

int main ( void )
{
  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );

  assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );

  return 0;
}
#endif /* __PROGTEST__ */
