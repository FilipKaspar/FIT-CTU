#ifndef __PROGTEST__

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <variant>
#include <vector>

using State = unsigned int;
using Symbol = uint8_t;

struct NFA {
    std::set<State> m_States;
    std::set<Symbol> m_Alphabet;
    std::map<std::pair<State, Symbol>, std::set<State>> m_Transitions;
    State m_InitialState;
    std::set<State> m_FinalStates;
};

struct DFA {
    std::set<State> m_States;
    std::set<Symbol> m_Alphabet;
    std::map<std::pair<State, Symbol>, State> m_Transitions;
    State m_InitialState;
    std::set<State> m_FinalStates;
};

#endif

void printFinalStates(const DFA & automata){
    std::cout << std::endl;
    for(const auto & i : automata.m_FinalStates){
        std::cout << i << " " << std::flush;
    }
}

void printDFA(const DFA & automata){
    int prev = -1;
    for(auto & i : automata.m_Transitions){
        if(i.first.first == unsigned(prev)){
            std::cout << "   " << std::flush;
        }
        else{
            std::cout << std::endl;
        }
        prev = int(i.first.first);
        std::cout << i.first.first << " : " << i.first.second << " -> " << std::flush;
        std::cout << i.second << ", " << std::flush;
    }
}

void printNFA(const NFA & automata){
    int prev = -1;
    for(auto & i : automata.m_Transitions){
        if(i.first.first == unsigned(prev)){
            std::cout << "   " << std::flush;
        }
        else{
            std::cout << std::endl;
        }
        prev = int(i.first.first);
        std::cout << i.first.first << " : " << i.first.second << " -> " << std::flush;
        for(auto & k : i.second){
            std::cout << k << ", " << std::flush;
        }
    }
    std::cout << std::endl;
}

DFA updateAutomata(const DFA & input_automata, const std::vector<std::vector<unsigned int>> & transition_table){
    DFA final_automata;

    final_automata.m_Alphabet = input_automata.m_Alphabet;
    final_automata.m_InitialState = input_automata.m_InitialState;

    unsigned int counter = 0;
    for(auto & row : transition_table){

        if(row.empty() || final_automata.m_States.find(row[0]) != final_automata.m_States.end()){
            counter++;
            continue;
        }
        final_automata.m_States.emplace(row[0]);
        auto symbol = final_automata.m_Alphabet.begin();
        for(long unsigned int i = 1; i < row.size(); i++){
            if(row[i] == unsigned(-1)){
                symbol++;
                continue;
            }
            final_automata.m_Transitions.emplace(std::make_pair(counter, *symbol), row[i]);
            symbol++;
        }
        if(input_automata.m_FinalStates.find(counter) != input_automata.m_FinalStates.end()){
            final_automata.m_FinalStates.emplace(counter);
        }
        counter++;
    }

    //Edge case
    if(final_automata.m_States.empty()){
        final_automata.m_States.emplace(input_automata.m_InitialState);
    }

    return final_automata;
}

DFA removeUnreachableStates(const DFA & input_automata, std::vector<std::vector<unsigned int>> & transition_table){
    std::queue<unsigned int> states;
    std::set<unsigned int> discovered;

    std::set<unsigned int> final_states = input_automata.m_States;

    if(transition_table.empty() || transition_table[0].empty()){
        return input_automata;
    }

    states.push(transition_table[0][0]);
    discovered.emplace(transition_table[0][0]);
    final_states.erase(transition_table[0][0]);
    while(!states.empty()){
        unsigned int curr_state = states.front();
        for(long unsigned int i = 1; i < transition_table[curr_state].size(); i++){
            if(discovered.find(transition_table[curr_state][i]) == discovered.end()) {
                states.push(transition_table[curr_state][i]);
                discovered.emplace(transition_table[curr_state][i]);
                final_states.erase(transition_table[curr_state][i]);
            }
        }
        states.pop();
    }

    if(!final_states.empty()){
        for(auto & i : final_states){
            transition_table[i].clear();
        }
    }


    return input_automata;
}

DFA removeUselessStates(const DFA & input_automata, std::vector<std::vector<unsigned int>> & transition_table){
    for(long unsigned int k = 0; k < transition_table.size();){
        bool found_final = false;
        std::queue<int> BFS;
        std::set<int> already_visited;
        if(input_automata.m_FinalStates.find(k) != input_automata.m_FinalStates.end()){
            k++;
            continue;
        }

        BFS.push(k);
        already_visited.emplace(k);
        while(!BFS.empty()) {
            long unsigned int curr_state = BFS.front();
            for (long unsigned int i = 1; i < transition_table[curr_state].size(); i++) {
                if (curr_state == transition_table[curr_state][i]) {
                    continue;
                }
                if (input_automata.m_FinalStates.find(transition_table[curr_state][i]) != input_automata.m_FinalStates.end()) {
                    found_final = true;
                    break;
                }
                if(already_visited.find(int(transition_table[curr_state][i])) == already_visited.end()) {
                    BFS.emplace(transition_table[curr_state][i]);
                    already_visited.emplace(transition_table[curr_state][i]);
                }
            }
            if(found_final) break;

            BFS.pop();
        }
        if(!found_final){
            transition_table[k].clear();
        }
        k++;
    }

    return input_automata;
}

DFA minimize(const DFA & input_automata){
    DFA final_automata;

    //Declaring our vector in order to use table method
    std::vector<std::vector<unsigned int>> equivalence_table;
    equivalence_table.resize(input_automata.m_States.size());

    // Fill table with transition values and equivalent symbol for final and non-final states
    int prev = 0;
    int counter = 0;
    int prev_counter = -1;
    unsigned int general_final_state = -1;
    unsigned int general_nonfinal_state = -1;
    for(auto & i : input_automata.m_Transitions){
        if(i.first.first != unsigned(prev)){
            counter++;
        }
        if(prev_counter != counter){
            if(input_automata.m_FinalStates.find(i.first.first) != input_automata.m_FinalStates.end()) {
                if(general_final_state == unsigned(-1)){
                    equivalence_table[counter].push_back(i.first.first);
                    general_final_state = i.first.first;
                }
                else {
                    equivalence_table[counter].push_back(general_final_state);
                }
            }
            else{
                if(general_nonfinal_state == unsigned(-1)){
                    equivalence_table[counter].push_back(i.first.first);
                    general_nonfinal_state = i.first.first;
                }
                else {
                    equivalence_table[counter].push_back(general_nonfinal_state);
                }
            }
        }
        prev = int(i.first.first);
        prev_counter = counter;
        equivalence_table[counter].push_back(i.second);
    }

    final_automata = removeUselessStates(removeUnreachableStates(input_automata, equivalence_table), equivalence_table);
    std::vector<std::vector<unsigned int>> starting_table;
    starting_table = equivalence_table;

    std::vector<unsigned int> prev_column;
    while(true) {
        std::map<std::list<int>, int> equivalent_states;

        // Check if the transition column is the same the previous one
        bool equivalent_found = true;
        if (!prev_column.empty()) {
            for (long unsigned int i = 0; i < equivalence_table.size(); i++) {
                if(equivalence_table[i].empty()) continue;
                if (prev_column[i] != equivalence_table[i][0]) {
                    equivalent_found = false;
                    break;
                }
            }
            if (equivalent_found) break;
        }

        // Clear prev_column
        prev_column.clear();

        // Calculate new transfer state for each state and symbol
        counter = 0;
        for (auto &row: equivalence_table) {
            for (long unsigned int i = 1; i < row.size(); i++) {
                if(starting_table[counter][i] == unsigned(-1) || equivalence_table[starting_table[counter][i]].empty()){ // Use normal table not equivalence table!!!!!!!!!!!!!
                    row[i] = -1;
                }
                else {
                    row[i] = equivalence_table[starting_table[counter][i]][0];
                }
            }
            if(row.empty()){
                prev_column.push_back(0);
            }
            else prev_column.push_back(row[0]);
            counter++;
        }

        // Calculate new transition state for each state
        int row_counter = 0;
        for (auto &row: equivalence_table) {
            if(row.empty()){
                row_counter++;
                continue;
            }
            std::list<int> transitions;
            for (long unsigned int i = 0; i < row.size(); i++) {
                transitions.push_back(row[i]);
            }

            auto possible_state = equivalent_states.find(transitions);
            if(possible_state != equivalent_states.end() &&
               (((input_automata.m_FinalStates.find(row_counter) == input_automata.m_FinalStates.end() && (input_automata.m_FinalStates.find(possible_state->second) == input_automata.m_FinalStates.end()))
                 || (((input_automata.m_FinalStates.find(row_counter) != input_automata.m_FinalStates.end() && (input_automata.m_FinalStates.find(possible_state->second) != input_automata.m_FinalStates.end()))))))){
                row[0] = possible_state->second;
            }
            else{
                row[0] = row_counter;
                equivalent_states.emplace(transitions, row_counter);
            }
            row_counter++;
        }
    }

    final_automata = updateAutomata(input_automata, equivalence_table);

    return final_automata;
}

DFA determinize(const NFA & nfa_automata, bool operation){ // 0 - intersection, 1 - unify
    DFA determinized_automata;

    //Declaring variables for coding states. (0,0) would be 0, (2,4) would be 1, and so on depending on how BFS going to assign values
    std::map<std::set<int>, int> referencing_values;
    std::set<int> curr_state_for_coding;
    curr_state_for_coding.emplace(nfa_automata.m_InitialState);
    referencing_values.emplace(curr_state_for_coding, 0);
    int reference_counter = 1;

    // Preparing for BFS
    std::queue<std::set<int>> all_states;

    // Copying all known information
    determinized_automata.m_States.emplace(0);
    determinized_automata.m_InitialState = 0;
    determinized_automata.m_Alphabet = nfa_automata.m_Alphabet;

    all_states.push(curr_state_for_coding);
    while(!all_states.empty()){
        std::set<int> curr_state = all_states.front();
        for(auto & curr_symbol : nfa_automata.m_Alphabet){
            std::set<int> new_state;
            if(!operation && nfa_automata.m_Transitions.find(std::make_pair(referencing_values.find(curr_state)->second, curr_symbol)) == nfa_automata.m_Transitions.end()){
                //continue;
            }
            for(auto & state : curr_state) {
                for (auto &transition: nfa_automata.m_Transitions.find(std::make_pair(state, curr_symbol))->second) {
                    new_state.emplace(transition);
                }
            }

            if(referencing_values.find(new_state) == referencing_values.end()){
                referencing_values.emplace(new_state, reference_counter);
                determinized_automata.m_States.emplace(reference_counter);
                all_states.push(new_state);
                reference_counter++;
            }
            determinized_automata.m_Transitions.emplace(std::make_pair(referencing_values.find(curr_state)->second, curr_symbol), referencing_values.find(new_state)->second);
        }
        for(auto & state : curr_state) { // Determining final states
            if(nfa_automata.m_FinalStates.find(state) != nfa_automata.m_FinalStates.end()) {
                determinized_automata.m_FinalStates.emplace(referencing_values.find(curr_state)->second);
                break;
            }
        }
        all_states.pop();
    }

    //printDFA(determinized_automata);
    //printFinalStates(determinized_automata);
    return determinized_automata;
}

NFA unifyIntersect(const NFA& a, const NFA& b, bool operation){ // 0 - intersection, 1 - unify
    NFA final_nfa;

    //Declaring variables for coding states. (0,0) would be 0, (2,4) would be 1, and so on depending on how BFS going to assign values
    std::map<std::pair<int, int>, int> referencing_values;
    referencing_values.emplace(std::make_pair(a.m_InitialState,b.m_InitialState), 0);
    int reference_counter = 1;

    // Preparing for BFS
    std::queue<std::pair<int, int>> all_states;

    // Copying all known information
    final_nfa.m_States.emplace(0);
    final_nfa.m_InitialState = 0;
    final_nfa.m_Alphabet = a.m_Alphabet;
    for(auto & symbol : b.m_Alphabet){
        final_nfa.m_Alphabet.emplace(symbol);
    }

    // Starting BFS
    all_states.push(std::make_pair(a.m_InitialState, b.m_InitialState));
    while(!all_states.empty()){
        std::pair<int, int> curr_state = all_states.front();
        for(auto & curr_symbol : final_nfa.m_Alphabet){ // Cartesian product from a.alphabet a.m_Alphabet
            std::set<State> values_a;
            std::set<State> values_b;
            std::set<State> cartesian_product;
            if(a.m_Alphabet.find(curr_symbol) == a.m_Alphabet.end()) { // Finish when only one automate has curr_symbol + decide if intersect or unify
                if(!operation) {
                    //continue;
                }
            }
            else {
                if(a.m_Transitions.find(std::make_pair(curr_state.first, curr_symbol)) != a.m_Transitions.end()){
                    values_a = a.m_Transitions.find(std::make_pair(curr_state.first, curr_symbol))->second;
                }
            }
            if(b.m_Alphabet.find(curr_symbol) == b.m_Alphabet.end()) { // Finish when only one automate has curr_symbol + decide if intersect or unify
                if(!operation) {
                    //continue;
                }
            }
            else{
                if(b.m_Transitions.find(std::make_pair(curr_state.second, curr_symbol)) != b.m_Transitions.end()){
                    values_b = b.m_Transitions.find(std::make_pair(curr_state.second, curr_symbol))->second; // (empty set) !!!! unsigned int (converts to 429469xxx)!!!!
                }
            }
            if(values_a.empty()) values_a.emplace(-1);
            if(values_b.empty()) values_b.emplace(-1);
            for(auto & state_a : values_a){ // Cartesian product of transitions
                for(auto & state_b : values_b){
                    if(referencing_values.find(std::make_pair(state_a, state_b)) == referencing_values.end()){
                        referencing_values.emplace(std::make_pair(state_a,state_b), reference_counter);
                        final_nfa.m_States.emplace(reference_counter);
                        all_states.push(std::make_pair(state_a, state_b));
                        reference_counter++;
                    }
                    cartesian_product.emplace(referencing_values.find(std::make_pair(state_a,state_b))->second);
                }
            }
            final_nfa.m_Transitions.emplace(std::make_pair(referencing_values.find(curr_state)->second, curr_symbol), cartesian_product);


        }
        if(operation){ // Determining final states
            if(a.m_FinalStates.find(curr_state.first) != a.m_FinalStates.end() || b.m_FinalStates.find(curr_state.second) != b.m_FinalStates.end()){
                final_nfa.m_FinalStates.emplace(referencing_values.find(curr_state)->second);
            }
        }
        else{
            if(a.m_FinalStates.find(curr_state.first) != a.m_FinalStates.end() && b.m_FinalStates.find(curr_state.second) != b.m_FinalStates.end()){
                final_nfa.m_FinalStates.emplace(referencing_values.find(curr_state)->second);
            }
        }
        all_states.pop();
    }

    //printNFA(final_nfa);

    return final_nfa;
}

void printFinal(const DFA & a){
    std::cout << "M.States: ";
    for(auto & i : a.m_States){
        std::cout << i << "," << std::flush;
    }

    std::cout << "\n" << "Alphabet: ";
    for(auto & i : a.m_Alphabet){
        std::cout << i << "," << std::flush;
    }

    printDFA(a);

    std::cout << "\nInitial State: " << a.m_InitialState;
    std::cout << "\nFinal States: " << std::endl;
    for(auto & i : a.m_FinalStates){
        std::cout << i << "," << std::flush;
    }
    std::cout << "\n" << std::endl;
}

DFA unify(const NFA& a, const NFA& b){
    DFA MDFA;
    MDFA = minimize((determinize(unifyIntersect(a, b, true), true)));

    //printFinal(MDFA);
    return MDFA;
}

DFA intersect(const NFA& a, const NFA& b){
    DFA MDFA;
    MDFA = minimize(determinize(unifyIntersect(a, b, false), false));

    //printFinal(MDFA);
    return MDFA;
}

#ifndef __PROGTEST__

// You may need to update this function or the sample data if your state naming strategy differs.
bool operator==(const DFA& a, const DFA& b)
{
    return std::tie(a.m_States, a.m_Alphabet, a.m_Transitions, a.m_InitialState, a.m_FinalStates) == std::tie(b.m_States, b.m_Alphabet, b.m_Transitions, b.m_InitialState, b.m_FinalStates);
}

int main()
{
    NFA a1{
            {0, 1, 2},
            {'a', 'b'},
            {
                    {{0, 'a'}, {0, 1}},
                    {{0, 'b'}, {0}},
                    {{1, 'a'}, {2}},
            },
            0,
            {2},
    };
    NFA a2{
            {0, 1, 2},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{1, 'a'}, {2}},
                    {{2, 'a'}, {2}},
                    {{2, 'b'}, {2}},
            },
            0,
            {2},
    };
    DFA a{
            {0, 1, 2, 3, 4},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{1, 'a'}, {2}},
                    {{2, 'a'}, {2}},
                    {{2, 'b'}, {3}},
                    {{3, 'a'}, {4}},
                    {{3, 'b'}, {3}},
                    {{4, 'a'}, {2}},
                    {{4, 'b'}, {3}},
            },
            0,
            {2},
    };
    //assert(intersect(a1, a2) == a);
    intersect(a1, a2);

    NFA b1{
            {0, 1, 2, 3, 4},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{0, 'b'}, {2}},

                    {{2, 'a'}, {2, 3}},
                    {{2, 'b'}, {2}},

                    {{3, 'a'}, {4}},
            },
            0,
            {1, 4},
    };
    NFA b2{
            {0, 1, 2, 3, 4},
            {'a', 'b'},
            {
                    {{0, 'b'}, {1}},

                    {{1, 'a'}, {2}},

                    {{2, 'b'}, {3}},

                    {{3, 'a'}, {4}},

                    {{4, 'a'}, {4}},
                    {{4, 'b'}, {4}},
            },
            0,
            {4},
    };
    DFA b{
            {0, 1, 2, 3, 4, 5, 6, 7, 8},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{0, 'b'}, {2}},

                    {{2, 'a'}, {3}},
                    {{2, 'b'}, {4}},

                    {{3, 'a'}, {5}},
                    {{3, 'b'}, {6}},

                    {{4, 'a'}, {7}},
                    {{4, 'b'}, {4}},

                    {{5, 'a'}, {5}},
                    {{5, 'b'}, {4}},

                    {{6, 'a'}, {8}},
                    {{6, 'b'}, {4}},

                    {{7, 'a'}, {5}},
                    {{7, 'b'}, {4}},

                    {{8, 'a'}, {8}},
                    {{8, 'b'}, {8}},
            },
            0,
            {1, 5, 8},
    };
    //assert(unify(b1, b2) == b);
    unify(b1, b2);

    NFA c1{
            {0, 1, 2, 3, 4},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{0, 'b'}, {2}},
                    {{2, 'a'}, {2, 3}},
                    {{2, 'b'}, {2}},
                    {{3, 'a'}, {4}},
            },
            0,
            {1, 4},
    };
    NFA c2{
            {0, 1, 2},
            {'a', 'b'},
            {
                    {{0, 'a'}, {0}},
                    {{0, 'b'}, {0, 1}},
                    {{1, 'b'}, {2}},
            },
            0,
            {2},
    };
    DFA c{
            {0},
            {'a', 'b'},
            {},
            0,
            {},
    };
    //assert(intersect(c1, c2) == c);
    intersect(c1, c2);

    NFA d1{
            {0, 1, 2, 3},
            {'i', 'k', 'q'},
            {
                    {{0, 'i'}, {2}},
                    {{0, 'k'}, {1, 2, 3}},
                    {{0, 'q'}, {0, 3}},
                    {{1, 'i'}, {1}},
                    {{1, 'k'}, {0}},
                    {{1, 'q'}, {1, 2, 3}},
                    {{2, 'i'}, {0, 2}},
                    {{3, 'i'}, {3}},
                    {{3, 'k'}, {1, 2}},
            },
            0,
            {2, 3},
    };
    NFA d2{
            {0, 1, 2, 3},
            {'i', 'k'},
            {
                    {{0, 'i'}, {3}},
                    {{0, 'k'}, {1, 2, 3}},
                    {{1, 'k'}, {2}},
                    {{2, 'i'}, {0, 1, 3}},
                    {{2, 'k'}, {0, 1}},
            },
            0,
            {2, 3},
    };
    DFA d{
            {0, 1, 2, 3},
            {'i', 'k', 'q'},
            {
                    {{0, 'i'}, {1}},
                    {{0, 'k'}, {2}},
                    {{2, 'i'}, {3}},
                    {{2, 'k'}, {2}},
                    {{3, 'i'}, {1}},
                    {{3, 'k'}, {2}},
            },
            0,
            {1, 2, 3},
    };
    //assert(intersect(d1, d2) == d);
    intersect(d1, d2);

    


    NFA j1{ // First segfault
            {0},
            {'a'},
            {
            },
            0,
            {0},
    };
    NFA j2{
            {0,1,2,3,4},
            {'g'},
            {
                    {{0, 'g'}, {0}},
            },
            0,
            {0,1,2,3},
    };
    intersect(j1,j2);

    NFA k1{
            {0},
            {'a'},
            {
            },
            0,
            {},
    };

    NFA k2{
            {0,1,2,3,4},
            {'a', 'b'},
            {
                    {{0, 'a'}, {1}},
                    {{1, 'b'}, {2}},
                    {{2, 'a'}, {3}},
                    {{3, 'b'}, {4}},
            },
            0,
            {4},
    };
    unify(k1,k2);

    NFA t1 {
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19} ,
            {'E','c','o'},
            {
                    {{0,'E'},{1}},
                    {{0,'c'},{1}},
                    {{0,'o'},{2}},
                    {{1,'o'},{17}},
                    {{2,'E'},{4}},
                    {{4,'o'},{10}},
                    {{5,'c'},{0}},
                    {{5,'o'},{12}},
                    {{7,'o'},{5}},
                    {{9,'o'},{7}},
                    {{11,'c'},{12}},
                    {{12,'o'},{10}},
                    {{15,'E'},{4}},
                    {{16,'E'},{1}},
                    {{16,'c'},{1}},
                    {{16,'o'},{15}},
                    {{18,'c'},{12}},
                    {{19,'o'},{10}},
            },
            0 ,
            {1, 2, 3, 4, 5, 6, 7, 8, 15, 17, 19}
    };


    NFA t2 {
            {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} ,
            {'c','o'},
            {
                    {{0,'c'},{1,2,3,4}},
                    {{0,'o'},{0,1,3,9,10}},
                    {{1,'c'},{2,4,8,10}},
                    {{1,'o'},{3,4,8,9}},
                    {{2,'c'},{1,3,4,9,10,12}},
                    {{2,'o'},{10}},
                    {{3,'c'},{0,1,2,3,4,10}},
                    {{3,'o'},{3,9}},
                    {{4,'c'},{0,2,3,4,9}},
                    {{4,'o'},{0,2,3,10,12}},
                    {{5,'c'},{3,5,10,11}},
                    {{5,'o'},{1,3,7,10,12}},
                    {{6,'c'},{0,1,3,4,6,8,9,10}},
                    {{6,'o'},{1,2,4,6,9,10,13}},
                    {{7,'c'},{1,2,3,6,11}},
                    {{7,'o'},{1,3,6,8}},
                    {{8,'c'},{8}},
                    {{8,'o'},{10,12}},
                    {{9,'c'},{10}},
                    {{9,'o'},{8, 10}},
                    {{10,'c'},{10}},
                    {{10,'o'},{10,12}},
                    {{11,'c'},{3,10,11,13}},
                    {{11,'o'},{1,3,7,10,12}},
                    {{12,'c'},{10}},
                    {{12,'o'},{8, 10}},
                    {{13,'c'},{3, 10, 11}},
                    {{13,'o'},{1, 3, 7, 10, 12}},
            },
            1 ,
            {0, 7}
    };

    intersect(t1, t2);
}
#endif

