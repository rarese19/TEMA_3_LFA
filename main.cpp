#include <bits/stdc++.h>
#include <windows.h>

using namespace std;
ifstream f("dfa.in");

class DFA {
    string dfa[50][50];
    int stare_initiala, nr_stari_finale, nr_stari;
    vector<int>stari_finale;
    set<char> alfabet, stari;

    int check(string dfa[50][50], int start, char litera){
        int stareCrt = start;

        for (int i=0; i<50; i++)
            if (dfa[stareCrt][i].find(litera) != string::npos){
                return i;
            }
        return -1;
    }
public:
    DFA(){
        for (int i=0; i<50; i++)
            stari_finale.push_back(-1);

        this->nr_stari = 0;
    }

    void read();

    void display();

    void language();

    void minimize();
};

void DFA::read() {
    f>>this->stare_initiala;

    f>>this->nr_stari_finale;

    for (int i=0; i<this->nr_stari_finale; i++){
        f>>stari_finale[i];}

    int stare1, stare2;
    string conditie;

    while (f>>stare1){
        f>>stare2>>conditie;
        this->stari.insert(stare1 - '0'), this->stari.insert(stare2 - '0');
        for (int i=0; i<conditie.length(); i+=2)
            this->alfabet.insert(conditie[i]);

        this->dfa[stare1][stare2] = conditie;

        this->nr_stari = max(nr_stari, stare1);
        this->nr_stari = max(nr_stari, stare2);
    }

    ++nr_stari;
}

void DFA::display() {
    cout<<"  ";
    for (int i=0; i<nr_stari; i++)
        cout<<i<<" ";
    cout<<endl;

    for (int i=0; i<nr_stari; i++){
        cout<<i<<" ";
        for (int j=0; j<nr_stari; j++){
            cout<<this->dfa[i][j]<<" ";
            if (this->dfa[i][j] == "")
                cout<<" ";
        }
        cout<<endl;
    }
}

void DFA::language() {
    for (auto a : this->alfabet)
        cout<<a<<" ";
    cout<<endl;
}

void DFA::minimize() {
    int n[50][50];

    for (int i=0; i<nr_stari; i++)
        for (int j=0; j<nr_stari; j++){
            n[i][j] = 1;
            if (i != j) {
                if ((find(stari_finale.begin(), stari_finale.end(), i) != stari_finale.end() &&
                     find(stari_finale.begin(), stari_finale.end(), j) == stari_finale.end()) ||
                    (find(stari_finale.begin(), stari_finale.end(), j) != stari_finale.end() &&
                     find(stari_finale.begin(), stari_finale.end(), i) == stari_finale.end()))
                    n[i][j] = 0;
            } else n[i][j] = -1;
        }

    bool ok = true;
    while (ok == true){
        ok = false;
        for (int i=stare_initiala+1; i<stare_initiala + nr_stari; i++)
            for (int j=stare_initiala; j<i; j++){
                for (auto litera : alfabet){
                    int i1 = check(dfa, i, litera);
                    int j1 = check(dfa, j, litera);
                    if (i1 < j1)
                        swap(i1, j1);
                    if (n[i1][j1] == 0 && n[i][j] != 0 && i1 != j1){
                        n[i][j] = 0;
                        ok = true;
                        break;
                    }
                }
            }
    }
//    for (int i=0; i<nr_stari; i++){
//        for (int j=0; j<nr_stari; j++)
//            cout<<n[i][j]<<" ";
//        cout<<endl;
//    }

    int k = 0;
    for (int i=0; i<nr_stari; i++){
        for (int j=0; j<nr_stari; j++)
            if (i > j && n[i][j] != 0)
                k = 1;
    }
    if (k == 0){
        cout<<"Automatul este minimal.\n";
        return;
    }

    int c[50] = {}, nounr = 0;
    vector<string> newStari;
    for (int i=stare_initiala; i<stare_initiala + nr_stari; i++){
        if (!c[i]){
            ++c[i], ++nounr;
            newStari.push_back(to_string(i));

            for (int j=i+1; j<nr_stari + stare_initiala; j++){
                if (n[j][i] == 1 && j > i) {
                    newStari.back() += to_string(j);
                    ++c[j];
                }
            }
        }
    }
    vector<string> newFinale;
    for (int i=0; i<newStari.size(); i++){
        for (int j=0; j<stari_finale.size(); j++){
            if (newStari[i].find(to_string(stari_finale[j])) != string::npos)
                newFinale.push_back(newStari[i]);
        }
    }

    string newInitiala;
    for (int i=0; i<newStari.size(); i++){
        if (newStari[i].find(to_string(stare_initiala)) != string::npos)
            newInitiala = newStari[i];
    }

    cout<<"Numar stari: "<<nounr<<endl;

    cout<<"Stare initiala: "<<newInitiala<<endl;

    cout<<"Numar stari finale: "<<newFinale.size()<<endl;
    cout<<"Stari finale: ";
    for (int i=0; i<newFinale.size(); i++)
        cout<<newFinale[i]<<" ";
    cout<<endl<<endl;

    cout<<"\tTRANZITII: "<<endl;
    for (int i=0; i<newStari.size(); i++){
        cout<<"Stare start: "<<newStari[i]<<"; ";
        for (int j=0; j<newStari[i].length(); j++) {
            int ok = 0;
            for (auto litera: alfabet) {
                int li = check(dfa, newStari[i][j] - '0', litera);
                if (li >= 0) {
                    for (int k=0; k<newStari.size(); k++)
                        if (newStari[k].find(to_string(li)) != string::npos){
                            if (ok == 1)
                                cout<<"Stare start: "<<newStari[i]<<"; ";
                            ok = 1;
                            cout<<"Stare stop: "<<newStari[k]<<"; "<<"Conditie: "<<litera<<endl;
                            break;
                        }
                }
            }
            if (ok)
                break;
        }
    }
}

int main(){
    DFA dfa;

    dfa.read();

    system("cls");
    while (true){
        system("cls");
        cout<<"1. Afisati matricea asociata DFA-ului\n";
        cout<<"2. Afisati limbajul automatului.\n";
        cout<<"3. Efectuati minimizarea.\n";
        cout<<"4. Iesire\n";
        cout<<"Introduceti alegerea: ";
        int choice;
        cin>>choice;
        switch (choice){
            case 1:{
                system("cls");
                dfa.display();
                system("pause");
                break;
            }

            case 2:{
                system("cls");
                dfa.language();
                system("pause");
                break;
            }

            case 3:{
                system("cls");
                dfa.minimize();
                system("pause");
                break;
            }

            case 4:{
                exit(0);
            }
        }
    }
}
