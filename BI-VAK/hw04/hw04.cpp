#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(){
    string input;
    int pocetMuzi, pocetZeny;

    getline(cin, input);
        
    //cout << "Cteni velikosti" << endl;
    const char *p = input.c_str();
    sscanf(p, "%d %d", &pocetMuzi, &pocetZeny);

    //cout << "Pocet Muzi: " << pocetMuzi << endl;
    //cout << "Pocet Zeny: " << pocetZeny << endl;

    vector<vector<int>> agenti(pocetMuzi + pocetZeny, vector<int> (0));

    int loop;
    int temp;
    for(int i = 0; i < (pocetMuzi + pocetZeny); i++){
        //cout << "LOOOPP" << endl;
        scanf("%d", &loop);
        int a = 0;
        if(loop == 0){
            agenti[i].resize(2);
            agenti[i][0] = -1;
            agenti[i][1] = 0;
        }
        else{
            agenti[i].resize(loop+1);
            for(int k = 0; k < loop; k++){
                scanf("%d", &temp);
                //cout << "Cislo: " << temp << endl;
                agenti[i][k] = temp;
                a = k;
            }
            agenti[i][a+1] = 0;
        }
    }

    /*cout << "POPRVY AGENTI JSOU: " << endl;
    for (long unsigned int i = 0; i < agenti.size(); i++){
        for (long unsigned int j = 0; j < agenti[i].size(); j++){
            cout << agenti[i][j] << " ";
        }   
        cout << endl;
    }*/

    vector<int> res(pocetMuzi + pocetZeny);
    vector<int> sameNum(vector<int> (0));
    
    int status = 0;
    int repeatingNumber = 0;
    int rep = 0;

    while(1){
        for(int i = 0; i < pocetMuzi; i++){
            if(agenti[i].back() < int(agenti[i].size()-1)){
                res[i] = agenti[i][agenti[i].back()];
            }
            else{
                res[i] = -1;
            }
        }
        /*cout << "RESULT PO PRVNIM: " << endl;
        for (long unsigned int i = 0; i < res.size(); i++){
            cout << res[i] << " "; 
            cout << endl;
        }*/

        for(int i = 0; i < pocetMuzi; i++){
            status = 0;
            if(res[i] != -1){
                for(long unsigned int k = 0; k < agenti[res[i] - 1].size()-1; k++){
                    if(i+1 == agenti[res[i] - 1][k]){
                        status = 1;
                        break;
                    }
                }
                //cout << "STATUS NA: " << i << " JE: " << status << endl;
                if(status == 0){
                    agenti[i].back()++;
                    if(agenti[i].back() < int(agenti[i].size()-1)){
                        res[i] = agenti[i][agenti[i].back()];
                    }
                    else{
                        res[i] = -1;
                    }
                }
            }
        }
        status = 0;

        /*cout << "AGENTI JSOU: " << endl;
        for (long unsigned int i = 0; i < agenti.size(); i++){
            for (long unsigned int j = 0; j < agenti[i].size(); j++){
                cout << agenti[i][j] << " ";
            }   
            cout << endl;
        }*/
        
        /*cout << "RESULT: " << endl;
        for (long unsigned int i = 0; i < res.size(); i++){
            cout << res[i] << " "; 
            cout << endl;
        }*/
       
        for(int i = 0; i < pocetMuzi; i++){
            for(int k = (i+1); k < pocetMuzi; k++){
                if(res[i] == res[k] && res[i] != -1){
                    if(status == 0){
                        sameNum.resize(sameNum.size()+1);
                        sameNum[rep] = i;
                        repeatingNumber = res[i];
                        rep++;
                    }
                    sameNum.resize(sameNum.size()+1);
                    sameNum[rep] = k;
                    rep++; 
                    status = 1;
                }
            }
            if(status){
                break;
            }
        }

        /*cout << "SAMENUM: " << endl;
        for (long unsigned int i = 0; i < sameNum.size(); i++){
            cout << sameNum[i] << " "; 
            cout << endl;
        
        }*/

        // Pokud je sameNum = 0 tak to znamena že nevznikly žádné kolize
        if(sameNum.size() == 0){
            break;
        }


        repeatingNumber--;
        //cout << "REPEATING NUMEBR: " << repeatingNumber << endl;

        status = 0;
        int chosenOne = 0;
        for(long unsigned int i = 0; i < agenti[repeatingNumber].size()-1; i++){
            for(long unsigned int k = 0; k < sameNum.size(); k++){
                if(agenti[repeatingNumber][i] == sameNum[k] + 1){
                    chosenOne = sameNum[k];
                    status = 1;
                    break;
                }
            }
            if(status){
                break;
            }
        }
        repeatingNumber++;
        //cout << "THE CHOOSEN ONE: " << chosenOne << endl;

        for(int i = 0; i < pocetMuzi; i++){
            if(res[i] == repeatingNumber && i != chosenOne){
                res[i] = -1;
                agenti[i].back()++;
            }
        }

        /*cout << "RESULT NA KONCI: " << endl;
        for (long unsigned int i = 0; i < res.size(); i++){
            cout << res[i] << " "; 
            cout << endl;
        }*/

        sameNum.resize(0);
        repeatingNumber = 0;
        rep = 0;
        status = 0;

        /*cout << "AGENTI NA KONCI JSOU: " << endl;
        for (long unsigned int i = 0; i < agenti.size(); i++){
            for (long unsigned int j = 0; j < agenti[i].size(); j++){
                cout << agenti[i][j] << " ";
            }   
            cout << endl;
        }*/
    }
    // Pridavani zen k muzum
    for(int i = 0; i < pocetMuzi; i++){
            status = 0;
            if(res[i] != -1){
                for(long unsigned int k = 0; k < agenti[res[i] - 1].size()-1; k++){
                    if(i+1 == agenti[res[i] - 1][k]){
                        status = 1;
                        break;
                    }
                }
                //cout << "STATUS NA: " << i << " JE: " << status << endl;
                if(status == 0){
                    agenti[i].back()++;
                    if(agenti[i].back() < int(agenti[i].size()-1)){
                        res[i] = agenti[i][agenti[i].back()];
                    }
                    else{
                        res[i] = -1;
                    }
                }
            }
        }

    // finalni koukani jestli je parovani vubec mozne, jestli ma zena muze v preferencich
    for(int i = pocetMuzi; i < (pocetMuzi + pocetZeny); i++){
        status = 0;
        for(int k = 0; k < pocetMuzi; k++){
            if(i+1 == res[k]){
                res[i] = k+1;
                status = 1;
                break;
            }
        }
        if(status == 0){
            res[i] = -1;
        }
    }

    //cout << "FINAL RESULT IS: " << endl;
    for(long unsigned int i = 0; i < res.size(); i++){
        cout << res[i] << endl;
    }


    return 0;
}