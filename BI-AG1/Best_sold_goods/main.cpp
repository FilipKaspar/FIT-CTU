#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>

#endif

// TODO implement
template < typename Product >
struct Bestsellers {
    struct Node{
        Node * parent;
        Node * c_Left;
        Node * c_Right;
        Product Name;
        size_t a_Sold;
        size_t a_Subnodes;
        size_t a_SubSold;
        int height;
    };
    Node * root = nullptr;

    std::unordered_map<Product, size_t> product_list;

    void deleteRec(Node * node){
        if (node)
        {
            deleteRec(node->c_Left);
            deleteRec(node->c_Right);
            delete node;
        }
    }

    ~Bestsellers(){
        auto head = root;
        deleteRec(head);
    }

    int getHeight(Node * node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getBiggerHeight(int h1, int h2) {
        if(h1 > h2) return h1;
        return h2;
    }

    int getHeightDifference(Node * node) {
        if (node == nullptr)
            return 0;
        return getHeight(node->c_Left) - getHeight(node->c_Right);
    }

    size_t getSubSold(Node * node) const{
        if(node == nullptr) return 0;
        return node->a_SubSold;
    }

    Node * rotateLeft(Node * x) {
        Node * y = x->c_Right;
        Node * y_right = y->c_Left;

        size_t temp = x->a_SubSold;
        x->a_SubSold = x->a_SubSold - getSubSold(x->c_Right) + y->a_SubSold - y->a_Sold - getSubSold(y->c_Right);
        y->a_SubSold = temp;

        y->c_Left = x;
        x->c_Right = y_right;
        x->height = getBiggerHeight(getHeight(x->c_Left), getHeight(x->c_Right)) + 1;
        y->height = getBiggerHeight(getHeight(y->c_Left), getHeight(y->c_Right)) + 1;

        y->parent = x->parent;
        x->parent = y;
        if(y_right == nullptr) {
            x->a_Subnodes = x->a_Subnodes - y->a_Subnodes;
            y->a_Subnodes = y->a_Subnodes + x->a_Subnodes;
        }
        else {
            y_right->parent = x->c_Left;
            x->a_Subnodes = x->a_Subnodes - y->a_Subnodes + y_right->a_Subnodes;
            y->a_Subnodes = y->a_Subnodes + x->a_Subnodes - y_right->a_Subnodes;
        }
        return y;
    }

    Node * rotateRight(Node * x) {
        Node * y = x->c_Left;
        Node * y_right = y->c_Right;

        size_t temp = x->a_SubSold;
        x->a_SubSold = x->a_SubSold - getSubSold(x->c_Left) + y->a_SubSold - y->a_Sold - getSubSold(y->c_Left);
        y->a_SubSold = temp;

        y->c_Right = x;
        x->c_Left = y_right;
        x->height = getBiggerHeight(getHeight(x->c_Left), getHeight(x->c_Right)) + 1;
        y->height = getBiggerHeight(getHeight(y->c_Left), getHeight(y->c_Right)) + 1;

        y->parent = x->parent;
        x->parent = y;
        if(y_right == nullptr) {
            x->a_Subnodes = x->a_Subnodes - y->a_Subnodes;
            y->a_Subnodes = y->a_Subnodes + x->a_Subnodes;
        }
        else {
            y_right->parent = x->c_Left;
            x->a_Subnodes = x->a_Subnodes - y->a_Subnodes + y_right->a_Subnodes;
            y->a_Subnodes = y->a_Subnodes + x->a_Subnodes - y_right->a_Subnodes;
        }
        return y;
    }

    Node * addNode(Node * cur_node,Node * node){
        if(cur_node == nullptr){
            return node;
        }

        cur_node->a_Subnodes++;
        cur_node->a_SubSold += node->a_SubSold;
        if (node->a_Sold == cur_node->a_Sold) {
            if (node->Name < cur_node->Name) {
                cur_node->c_Left = addNode(cur_node->c_Left, node);
            } else if (node->Name > cur_node->Name) {
                cur_node->c_Right = addNode(cur_node->c_Right, node);
            } else {
                return node;
            }
        } else if (node->a_Sold < cur_node->a_Sold) {
            cur_node->c_Left = addNode(cur_node->c_Left, node);
        } else {
            cur_node->c_Right = addNode(cur_node->c_Right, node);
        }

        cur_node->height = 1 + getBiggerHeight(getHeight(cur_node->c_Left),getHeight(cur_node->c_Right));
        if(cur_node->c_Left) cur_node->c_Left->parent = cur_node;
        if(cur_node->c_Right) cur_node->c_Right->parent = cur_node;

        int balanceFactor = getHeightDifference(cur_node);
        if (balanceFactor > 1) {
            if (getHeightDifference(cur_node->c_Left) >= 0) {
                return rotateRight(cur_node);
            } else{
                cur_node->c_Left = rotateLeft(cur_node->c_Left);
                return rotateRight(cur_node);
            }
        }
        if (balanceFactor < -1) {
            if (getHeightDifference(cur_node->c_Right) <= 0) {
                return rotateLeft(cur_node);
            } else{
                cur_node->c_Right = rotateRight(cur_node->c_Right);
                return rotateLeft(cur_node);
            }
        }

        return cur_node;
    }

    size_t getTotalSubSold(Node * node){
        size_t new_subsold = 0;
        if(node->c_Left) new_subsold += node->c_Left->a_SubSold;
        if(node->c_Right) new_subsold += node->c_Right->a_SubSold;

        return new_subsold;
    }

    Node * deleteNode(Node * cur_node, const Product & searched_name, const size_t & searched_amount, const size_t & new_amount){
        if(cur_node == nullptr){
            return cur_node;
        }

        cur_node->a_Subnodes--;
        cur_node->a_SubSold -= new_amount + 1;
        if(searched_amount == cur_node->a_Sold){
            if(searched_name == cur_node->Name){
                if(cur_node->c_Left == nullptr || cur_node->c_Right == nullptr){
                    Node * temp;
                    if(cur_node->c_Left != nullptr) temp = cur_node->c_Left;
                    else temp = cur_node->c_Right;

                    if(temp == nullptr){
                        temp = cur_node;
                        cur_node = nullptr;
                    }
                    else{
                        *cur_node = *temp;
                    }
                    delete temp;
                }
                else {
                    Node * temp = cur_node->c_Right;
                    while (temp->c_Left != nullptr) {
                        temp = temp->c_Left;
                    }

                    cur_node->Name = temp->Name;
                    cur_node->a_Sold = temp->a_Sold;

                    cur_node->c_Right = deleteNode(cur_node->c_Right, temp->Name, temp->a_Sold, new_amount);
                }
            }
            else if(searched_name < cur_node->Name){
                cur_node->c_Left = deleteNode(cur_node->c_Left, searched_name, searched_amount, new_amount);
            }
            else{
                cur_node->c_Right = deleteNode(cur_node->c_Right, searched_name, searched_amount, new_amount);
            }

        }
        else if(searched_amount < cur_node->a_Sold){
            cur_node->c_Left = deleteNode(cur_node->c_Left, searched_name, searched_amount, new_amount);
        }
        else{
            cur_node->c_Right = deleteNode(cur_node->c_Right, searched_name, searched_amount, new_amount);
        }

        if(cur_node == nullptr) return cur_node;

        cur_node->height = 1 + getBiggerHeight(getHeight(cur_node->c_Left), getHeight(cur_node->c_Right));
        cur_node->a_SubSold = cur_node->a_Sold + getTotalSubSold(cur_node);
        if(cur_node->c_Left) cur_node->c_Left->parent = cur_node;
        if(cur_node->c_Right) cur_node->c_Right->parent = cur_node;

        int balanceFactor = getHeightDifference(cur_node);
        if (balanceFactor > 1) {
            if (getHeightDifference(cur_node->c_Left) >= 0) {
                return rotateRight(cur_node);
            } else {
                cur_node->c_Left = rotateLeft(cur_node->c_Left);
                return rotateRight(cur_node);
            }
        }
        if (balanceFactor < -1) {
            if (getHeightDifference(cur_node->c_Right) <= 0) {
                return rotateLeft(cur_node);
            } else {
                cur_node->c_Right = rotateRight(cur_node->c_Right);
                return rotateLeft(cur_node);
            }
        }
        return cur_node;
    }

    size_t getSoldAmount(size_t & rank, const bool & include) const{
        Node * head = root;
        size_t total_sold = 0;

        rank = root->a_Subnodes - rank + 1;

        size_t curr_subnodes;
        size_t overall_subnodes = 0;

        while(true){
            if(head == nullptr) throw std::out_of_range("");
            if(head->c_Right == nullptr){
                curr_subnodes = head->a_Subnodes;
            }
            else{
                curr_subnodes = head->a_Subnodes - head->c_Right->a_Subnodes;
            }

            if(rank == curr_subnodes + overall_subnodes){
                if(include){
                    total_sold += head->a_SubSold - getSubSold(head->c_Left);
                    //total_sold += head->a_Sold;
                }
                else{
                    if(head->c_Right){
                        total_sold += head->a_SubSold - getSubSold(head->c_Left) - head->a_Sold;
                    }
                }
                break;
            }
            else if(rank < curr_subnodes + overall_subnodes){
                total_sold += head->a_SubSold - getSubSold(head->c_Left);
                head = head->c_Left;
            }
            else {
                overall_subnodes += curr_subnodes;
                head = head->c_Right;
            }
        }

        return total_sold;
    }

    void sell(const Product& p, size_t amount){
        auto product = product_list.find(p);

        if(product == product_list.end()){
            product_list.emplace(p,amount);

            Node * new_Node = new Node();
            new_Node->Name = p;
            new_Node->a_Sold = amount;
            new_Node->a_SubSold = amount;
            new_Node->height = 1;
            new_Node->a_Subnodes = 1;

            root = addNode(root,new_Node);
        }
        else{
            root = deleteNode(root, product->first, product->second, amount);
            product->second += amount;

            Node * new_Node = new Node();
            new_Node->Name = product->first;
            new_Node->a_Sold = product->second;
            new_Node->a_SubSold = product->second;
            new_Node->height = 1;
            new_Node->a_Subnodes = 1;
            root = addNode(root,new_Node);
        }
    };

    // The total number of tracked products
    size_t products() const{
        return root->a_Subnodes;
    };

    // The most sold product has rank 1
    size_t rank(const Product& p) const{
        auto head = root;
        auto item = product_list.find(p);
        if(item == product_list.end()) throw std::out_of_range("ano");
        size_t amount = item->second;
        size_t rank_backwards = 0;

        while(true){
            if(amount == head->a_Sold){
                if(p == head->Name){
                    if(head->c_Right == nullptr) {
                        rank_backwards += head->a_Subnodes;
                    }
                    else {
                        rank_backwards += head->a_Subnodes - head->c_Right->a_Subnodes;
                    }
                    break;
                }
                else if(p < head->Name){
                    head = head->c_Left;
                }
                else{
                    if(head->c_Right == nullptr) {
                        rank_backwards += head->a_Subnodes;
                    }
                    else {
                        rank_backwards += head->a_Subnodes - head->c_Right->a_Subnodes;
                    }
                    head = head->c_Right;
                }
            }
            else if(amount < head->a_Sold){
                head = head->c_Left;
            }
            else {
                if(head->c_Right == nullptr) {
                    rank_backwards += head->a_Subnodes;
                }
                else {
                    rank_backwards += head->a_Subnodes - head->c_Right->a_Subnodes;
                }
                head = head->c_Right;
            }
        }

        rank_backwards = root->a_Subnodes - rank_backwards + 1;
        return rank_backwards;
    };

    const Product& product(size_t rank) const{
        rank = root->a_Subnodes - rank + 1;
        auto head = root;

        size_t curr_subnodes;
        size_t overall_subnodes = 0;

        while(true){
            if(head == nullptr) throw std::out_of_range("");
            if(head->c_Right == nullptr){
                curr_subnodes = head->a_Subnodes;
            }
            else{
                curr_subnodes = head->a_Subnodes - head->c_Right->a_Subnodes;
            }

            if(rank == curr_subnodes + overall_subnodes){
                return head->Name;
            }
            else if(rank < curr_subnodes + overall_subnodes){
                head = head->c_Left;
            }
            else {
                overall_subnodes += curr_subnodes;
                head = head->c_Right;
            }
        }

    };

    // How many copies of product with given rank were sold
    size_t sold(size_t rank) const{
        Product name = product(rank);
        return product_list.find(name)->second;
    };

    // The same but sum over interval of products (including from and to)
    // It must hold: sold(x) == sold(x, x)
    size_t sold(size_t from, size_t to) const{
        //3.5 points variant
        if(from == 0 || to == 0 || to < from || from > root->a_Subnodes || to > root->a_Subnodes ) throw std::out_of_range("");
        /*size_t amount_sold = 0;
        for(size_t i = from; i <= to; i++){
            amount_sold += sold(i);
        }
        return amount_sold;*/
        //10 points variant
        return getSoldAmount(to, true) - getSoldAmount(from, false);
    }


    // Bonus only, ignore if you are not interested in bonus
    // The smallest (resp. largest) rank with sold(rank) == sold(r)
    size_t first_same(size_t r) const {
        Product name = product(r);
        auto item = product_list.find(name);
        if(item == product_list.end()) throw std::out_of_range("");
        size_t amount = item->second;
        auto head = root;

        Node * final = head;

        while(true) {
            if(head == nullptr) break;
            if (amount == head->a_Sold) {
                if(!head->c_Right){
                    if(head->a_Sold == amount) final = head;
                    break;
                }
                if (head->a_Sold != head->c_Right->a_Sold) {
                    final = head;
                }
                head = head->c_Right;
            } else if (amount < head->a_Sold) {
                head = head->c_Left;
            } else {
                head = head->c_Right;
            }
        }

        return rank(final->Name);
    }
    size_t last_same(size_t r) const {
        Product name = product(r);
        auto item = product_list.find(name);
        if(item == product_list.end()) throw std::out_of_range("");
        size_t amount = item->second;
        auto head = root;

        Node * final = head;

        while(true) {
            if(head == nullptr) break;
            if (amount == head->a_Sold) {
                if(!head->c_Left){
                    if(head->a_Sold == amount) final = head;
                    break;
                }
                if (head->a_Sold != head->c_Left->a_Sold) {
                    final = head;
                }
                head = head->c_Left;
            } else if (amount < head->a_Sold) {
                head = head->c_Left;
            } else {
                head = head->c_Right;
            }
        }

        return rank(final->Name);
    }
};

#ifndef __PROGTEST__

void test1() {
    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);
    assert(T.products() == 2);
    T.sell("ham", 2);
    T.sell("mushrooms", 12);

    assert(T.products() == 4);
    assert(T.rank("ham") == 3);
    assert(T.rank("coke") == 1);
    assert(T.sold(1, 3) == 46);
    assert(T.product(2) == "mushrooms");

    T.sell("ham", 11);
    assert(T.products() == 4);
    assert(T.product(2) == "ham");
    assert(T.sold(2) == 13);
    assert(T.sold(2, 2) == 13);
    assert(T.sold(1, 2) == 45);
}

void test2() {
# define CATCH(expr) \
  try { expr; assert(0); } catch (const std::out_of_range&) { assert(1); };

    Bestsellers<std::string> T;
    T.sell("coke", 32);
    T.sell("bread", 1);

    CATCH(T.rank("ham"));
    CATCH(T.product(3));
    CATCH(T.sold(0));
    CATCH(T.sold(9));
    CATCH(T.sold(0, 1));
    CATCH(T.sold(3, 2));
    CATCH(T.sold(1, 9));

#undef CATCH
}
void test3() {
# define CATCH(expr) \
  try { expr; assert(0); } catch (const std::out_of_range&) { assert(1); };
    Bestsellers<std::string> T;


    T.sell("coke", 10);
    T.sell("bread", 5);
    T.sell("dumb", 16);
    T.sell("dzum", 20);
    T.sell("doodle", 13);
    T.sell("dzam", 17);
    T.sell("dzzm", 28);
    T.sell("dzem", 19);
    T.sell("dumb", 10);
    T.sell("dzum", 2);
    T.sell("bread", 8);
    T.sell("doodle", 1);
    T.sell("bable", 13);
    T.sell("bae", 13);

    std::cout << T.rank("dzum") << std::endl;
    std::cout << T.product(3);
    assert(T.product(T.rank("bae")) == "bae");
    size_t amount = T.sold(1,10);
    std::cout << amount << std::endl;
#undef CATCH
}

void test4(){
# define CATCH(expr) \
  try { expr; assert(0); } catch (const std::out_of_range&) { assert(1); };
    Bestsellers<std::string> T;

    T.sell("J", 3);
    T.sell("E", 2);
    T.sell("B", 5);
    T.sell("F", 4);
    T.sell("D", 13);
    T.sell("G", 4);
    T.sell("L", 13);
    T.sell("M", 10);
    T.sell("N", 13);
    T.sell("H", 4);
    T.sell("Z", 13);
    T.sell("T", 2);
    T.sell("R", 13);
    T.sell("K", 3);
    T.sell("C", 5);
    T.sell("Y", 5);
    T.sell("A", 4);

    assert(T.last_same(10) == 13);
    assert(T.first_same(9) == 7);


#undef CATCH
}

void test5(){

# define CATCH(expr) \
  try { expr; assert(0); } catch (const std::out_of_range& e) { \
   std::cout << e.what() << std::endl; \
  assert(1); };
    Bestsellers<std::string> T;


#undef CATCH

}

int main() {
    test1();
    test2();
    test3();
    test4();
    //test5();

    return 0;
}

#endif


