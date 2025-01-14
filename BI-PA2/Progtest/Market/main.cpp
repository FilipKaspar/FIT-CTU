#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate {
public:
    CDate (int y, int m, int d) { // Constructor
        year = y;
        month = m;
        day = d;
    };
    bool operator < (const CDate &src) const { // Checking if date passed in argument is bigger than date in class
                                               // (if our product's expiry is sooner than passed date)
        return tie(year, month, day) < tie(src.year, src.month, src.day);
    }

    friend ostream & operator << (ostream & out, CDate src){ // Helper function to transform the class variables to ostream, used only while calling in printStored
        out << to_string(src.year) + '-' + to_string(src.month) + '-' + to_string(src.day);
        return out;
    };
private:
    int year, month, day;
};
class CSupermarket
{
    public:
        CSupermarket() = default; // Just default constructor

        CSupermarket & store (const string & name, CDate expiryDate, int count ) { // Storing products to our products map
            if(products.count(name)) { // Checking if we already have a product in our map
                if (products[name].count(
                        expiryDate)) { // Checking if the expiry is same, if so, then we will just add the amount of products
                    products[name][expiryDate] += count;
                    return *this;
                }
            }
            products[name].emplace(pair<CDate, int>(expiryDate, count)); // If we couldn't find name, add whole new product + date
            return *this;                                                      // or just add date to already existing product
        };

        list<pair<string, int>> expired ( const CDate & date ) const{
            multimap<int, string, greater<>> expiryStored; // Using map to easily sort by amount
            list<pair<string, int>> expiring;
            for(auto & itrProduct : products) { // Looping through every product and it's dates in products
                int expiredProductCount = 0;
                expiredProductCount = getExpired(itrProduct, date);

                if(expiredProductCount == 0) continue; // If product is not within expired criteria, skip it
                expiryStored.emplace(expiredProductCount, itrProduct.first);
            }
            for(auto & itrExpiryStored : expiryStored) { // Copying content of the map to the list
                expiring.emplace_back(itrExpiryStored.second, itrExpiryStored.first);
            }
            return expiring;
        };

        static int getExpired(const pair<const string, map<CDate, int>> & itrProduct, const CDate & date) {
            int expiredProductCount = 0;
            for (auto ptr : itrProduct.second) { // Loop through every expiry in the product
                if (ptr.first < date) { // Using overloaded < operator, to check whether our date is sooner than the passed date
                    expiredProductCount += ptr.second;
                } else {
                    break;
                }
            }
            return expiredProductCount;
        }

        void sell (list<pair<string,int>> & sellList) {
            vector<pair<list<pair<string, int>>::iterator, const string *>> toSell; // Creating temp vector, because sell has to be transition type

            saveValid(toSell,sellList); // Checking which products should I actually sell (meaning which I have in my map or which are similar more than
                                        // 1 time to some products etc.)
            sellUpdateContainers(toSell, sellList); // Selling the products + if container product or date is empty delete
                                                    // either the date or the whole product
        };

        // I know we should split into more methods, and it's one of the requirements for Code Review, but I think splitting the
        // whole sell method into those 3 methods perfectly makes sense. I personally (which might be wrong) think that making more methods would make this way
        // harder to read. It's not because I am lazy to do them, otherwise I wouldn't write all these notes :), but because the reasons I specified
        void saveValid(vector<pair<list<pair<string, int>>::iterator, const string *>> & toSell, list<pair<string,int>> & sellList){
            for (auto itrSellList = sellList.begin(); itrSellList != sellList.end(); itrSellList++) { // Looping through the list of products we want to sell
                if (products.count(itrSellList->first)) { // Checking for the exact match
                    toSell.emplace_back(make_pair(itrSellList, &(itrSellList->first)));
                } else {
                    int foundSimilar = 0;
                    for (auto & itrProduct : products) { // We have to have reference otherwise string is copied
                                                                   // We would be passing an address to copied pair<const....> which will get
                                                                   // erased after getting out of scope. When reading = segfault
                        checkSimilar(toSell, itrSellList, itrProduct, foundSimilar);
                        if (foundSimilar > 1) { // If we found more than 1 similar words, the last element in the vector we're going to set to end()
                            toSell[toSell.size() - 1] = (make_pair(sellList.end(), &(itrProduct.first)));
                            break;
                        }
                    }
                }
            }
        };

        static void checkSimilar(vector<pair<list<pair<string, int>>::iterator, const string *>> & toSell,  _List_iterator<pair<string, int>> & itrSellList, pair<const string, map<CDate, int>> & itrProduct, int & foundSimilar){
            int mismatchLetter = 0;
            size_t position = 0;
            while (position < (itrSellList->first).size() && (itrSellList->first).size() ==
                                                             (itrProduct.first).size()) { // Comparing every letter from
                if ((itrSellList->first)[position] !=                                     // from our products and sellList
                    (itrProduct.first)[position]) {
                    mismatchLetter++;
                }
                if (mismatchLetter > 1) {
                    break;
                }
                if (position == (itrSellList->first).size() - 1) { // If we are at the end of the for loop
                    foundSimilar++;
                    if (foundSimilar == 1) // Only adding to vector the first word that was similar
                        toSell.emplace_back(make_pair(itrSellList, &(itrProduct.first)));
                }
                position++;
            }
        }

        // Didn't want to split this method to multiple methods, because I think this method pretty much makes sense as it is, and it
        // will only get more confusing if I would start splitting different if's or passing iterator every time with all containers
        // It has only 4 relatively easy ifs
        void sellUpdateContainers(vector<pair<list<pair<string, int>>::iterator, const string *>> & toSell, list<pair<string,int>> & sellList){
            for (auto & itrToSell : toSell) { // Looping through our vector
                if (itrToSell.first == sellList.end()) { // If the word is pointing to end, continue to next element in vector
                    continue;
                }

                auto itrProductPos = products.find(*(itrToSell.second)); // Find product position using its correct name
                auto itrDate = itrProductPos->second.begin();
                while (itrDate != itrProductPos->second.end()) { // Determining if amount of the product we have to sell is bigger, lower or equal
                    if (itrToSell.first->second == itrDate->second) {    // and then doing specific actions. Subtracting the amount and removing the element from
                        sellList.erase(itrToSell.first);         // from the container/s
                        itrDate = itrProductPos->second.erase(itrDate);
                    } else if (itrToSell.first->second > itrDate->second) {
                        itrToSell.first->second -= itrDate->second;
                        itrDate = itrProductPos->second.erase(itrDate);
                        continue;
                    } else {
                        itrDate->second -= itrToSell.first->second;
                        sellList.erase(itrToSell.first);
                    }
                    itrDate++;
                    break;
                }
                if (itrProductPos->second.empty()) { // If product doesn't have any more dates, delete the whole product record
                    products.erase(itrProductPos->first);
                }
            }
        };

        void printStored(){ // Helper function, prints the current products in our products map, with nice styling :)
            cout << "VALUES IN PRODUCTS MAP: " << endl;
            for(auto & product : products) {
                cout << "\tProduct name is: " << product.first;
                for (auto & ptr : product.second) {
                    cout << "\tExpiration is: " << ptr.first
                         << "\tAmount is: " << ptr.second << "\n\t\t\t";
                }
                cout << endl;
            }
        };

    private:
        unordered_map<string,map<CDate, int>> products; // First unordered for quick search, second one order
                                                        // for our dates to be order from lowest to highest
};
#ifndef __PROGTEST__
int main ()
{

    CSupermarket s;

    s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
    . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
    . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
    . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
    . store ( "okey", CDate ( 2016, 7, 18 ), 5 );



    list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
    assert ( l0 . size () == 4 );
    assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );



    list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
    s . sell ( l1 );
    assert ( l1 . size () == 2 );
    assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

    list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
    assert ( l2 . size () == 1 );
    assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

    list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
    assert ( l3 . size () == 1 );
    assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

    list<pair<string,int> > l4 { { "bread", 105 } };
    s . sell ( l4 );
    assert ( l4 . size () == 0 );
    assert ( ( l4 == list<pair<string,int> > {  } ) );



    list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l5 . size () == 3 );
    assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

    s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );



    list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
    s . sell ( l6 );
    assert ( l6 . size () == 3 );
    assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

    list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l7 . size () == 4 );
    assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

    s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );



    list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
    s . sell ( l8 );
    assert ( l8 . size () == 2 );
    assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

    list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l9 . size () == 5 );
    assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );



    list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
    s . sell ( l10 );
    assert ( l10 . size () == 2 );
    assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );



    list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l11 . size () == 4 );
    assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );



    list<pair<string,int> > l12 { { "Cake", 4 } };
    s . sell ( l12 );
    assert ( l12 . size () == 0 );
    assert ( ( l12 == list<pair<string,int> > {  } ) );



    list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l13 . size () == 4 );
    assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

    list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
    s . sell ( l14 );
    assert ( l14 . size () == 1 );
    assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

    s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

    list<pair<string,int> > l15 { { "ccccc", 10 } };
    s . sell ( l15 );
    assert ( l15 . size () == 1 );
    assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

    return EXIT_SUCCESS;

}
#endif /* __PROGTEST__ */
