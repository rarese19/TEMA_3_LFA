#include <bits/stdc++.h>
#include <windows.h>

using namespace std;
ifstream f("dfa.in");

class DFA {
    string dfa[50][50], stare_initiala_nou;
    int stare_initiala, nr_stari_finale, nr_stari, o;
    vector<int>stari_finale;
    set<char> alfabet, stari;
    map<string, vector<pair<string, char>>> stari_noi;
    int nr_stari_finale_nou, nr_stari_nou;
    set<string> stari_finale_noi;

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

        this->o = 0;
        this->nr_stari = 0;
        this->nr_stari_nou = 0;
    }

    void read();

    void display();

    void language();

    void minimize(int o = 0);

    int getMinimized() const {return this->o;}
    void setMinimized() {if (this->o == 1) this->o = 0;}
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
    if (this->o == 0) {
        cout<<"Numar stari: "<<this->nr_stari<<endl;
        cout<<"Stare initiala: "<<this->stare_initiala<<endl;
        cout<<"Numar stari finale: "<<this->nr_stari_finale<<endl;
        cout<<"Stari finale: ";
        for (int i=0; i<stari_finale.size(); i++)
            if (stari_finale[i] != -1)
                cout << stari_finale[i] << " ";
        cout<<endl<<endl;
        cout << "  ";
        for (int i = 0; i < nr_stari; i++)
            cout << i << " ";
        cout << endl;

        for (int i = 0; i < nr_stari; i++) {
            cout << i << " ";
            for (int j = 0; j < nr_stari; j++) {
                cout << this->dfa[i][j] << " ";
                if (this->dfa[i][j] == "")
                    cout << " ";
            }
            cout << endl;
        }
    } else {
        cout<<"Numar stari: "<<this->nr_stari_nou<<endl;
        cout<<"Stare initiala: "<<this->stare_initiala_nou<<endl;
        cout<<"Numar stari finale: "<<this->nr_stari_finale_nou<<endl;
        for (auto stari : stari_finale_noi)
            cout<<stari<<" ";
        cout<<endl<<endl;
        cout<<"\tTRANZITII\n";
        for (auto pair : stari_noi){
            for (int i=0; i<pair.second.size(); i++){
                cout<<"Stare start: ";
                cout<<pair.first<<"; ";
                cout<<"Stare stop: ";
                cout<<pair.second[i].first<<"; ";
                cout<<"Conditie: ";
                cout<<pair.second[i].second<<endl;
            }
        }
    }
}

void DFA::language() {
    for (auto a : this->alfabet)
        cout<<a<<" ";
    cout<<endl;
}

void DFA::minimize(int o) {
    if (this->o == 1){
        cout<<"Automatul este minimal.\n";
        return;
    }
    this->o = o;
    int n[50][50];
    this->nr_stari_finale_nou = 0;
    this->nr_stari_nou = 0;
    this->stari_finale_noi.clear();
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
    // for (int i=0; i<nr_stari; i++){
    //     for (int j=0; j<nr_stari; j++)
    //         cout<<n[i][j]<<" ";
    //     cout<<endl;
    // }

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
            ++c[i], ++nr_stari_nou;
            newStari.push_back(to_string(i));

            for (int j=i+1; j<nr_stari + stare_initiala; j++){
                if (n[j][i] == 1 && j > i) {
                    newStari.back() += to_string(j);
                    ++c[j];
                }
            }
        }
    }
    //vector<string> newFinale;
    for (int i=0; i<newStari.size(); i++){
        for (int j=0; j<stari_finale.size(); j++){
            if (newStari[i].find(to_string(stari_finale[j])) != string::npos) {
                this->stari_finale_noi.insert(newStari[i]);
            }
        }
    }
    this->nr_stari_finale_nou = stari_finale_noi.size();
    string newInitiala;
    for (int i=0; i<newStari.size(); i++){
        if (newStari[i].find(to_string(stare_initiala)) != string::npos)
            stare_initiala_nou = newStari[i];
    }

//    cout<<"Numar stari: "<<nr_stari_nou<<endl;

    cout<<"Stare initiala: "<<stare_initiala_nou<<endl;

    cout<<"Numar stari finale: "<<stari_finale_noi.size()<<endl;
    cout<<"Stari finale: ";
    for (auto stari : stari_finale_noi)
        cout<<stari<<" ";
    cout<<endl<<endl;

//    cout<<"\tTRANZITII: "<<endl;
    for (int i=0; i<newStari.size(); i++){
//        cout<<"Stare start: "<<newStari[i]<<"; ";
        stari_noi[newStari[i]] = {};
        for (int j=0; j<newStari[i].length(); j++) {
            int ok = 0;
            for (auto litera: alfabet) {
                int li = check(dfa, newStari[i][j] - '0', litera);
                if (li >= 0) {
                    for (int k=0; k<newStari.size(); k++)
                        if (newStari[k].find(to_string(li)) != string::npos){
//                            if (ok == 1)
//                                cout<<"Stare start: "<<newStari[i]<<"; ";
                            ok = 1;
//                            cout<<"Stare stop: "<<newStari[k]<<"; "<<"Conditie: "<<litera<<endl;
                            stari_noi[newStari[i]].push_back({newStari[k], litera});
                            break;
                        }
                }
            }
            if (ok)
                break;
        }
    }
    vector<string> to_delete;
    for (int j=0; j<nr_stari; j++){
        int ok = 0;
        for (int i=0; i<nr_stari; i++)
            if (dfa[i][j] == "")
                ++ok;
        if (ok == nr_stari){
            for (auto pair : stari_noi)
                if (pair.first.find(to_string(j)) != string::npos)
                    to_delete.push_back(pair.first);
        }
    }
    for (int i=0; i<to_delete.size(); i++)
        stari_noi.erase(to_delete[i]), --nr_stari_nou;

    cout<<"\tTRANZITII\n";
    for (auto pair : stari_noi){
        for (int i=0; i<pair.second.size(); i++){
            cout<<"Stare start: ";
            cout<<pair.first<<"; ";
            cout<<"Stare stop: ";
            cout<<pair.second[i].first<<"; ";
            cout<<"Conditie: ";
            cout<<pair.second[i].second<<endl;
        }
    }
}

int main(){
    DFA dfa;

    dfa.read();

    system("cls");
    while (true){
        system("cls");
        if (dfa.getMinimized() == 0)
            cout<<"1. Afisati matricea asociata DFA-ului\n";
        else cout<<"1. Afisati informatii despre DFA\n";
        cout<<"2. Afisati limbajul automatului.\n";
        cout<<"3. Efectuati minimizarea.\n";
        cout<<"4. Reveniti la automatul initial.\n";
        cout<<"5. Iesire\n";
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
                if (dfa.getMinimized() == 1){
                    cout<<"Automatul este minimal.\n";
                    system("pause");
                    break;
                }
                cout<<"Aplicati minimizarea? (0/1) ";
                cin>>choice;
                system("cls");
                dfa.minimize(choice);
                system("pause");
                break;
            }
            case 4:{
                system("cls");
                if (dfa.getMinimized() == 0){
                    cout<<"Automatul nu a fost modificat.\n";
                    system("pause");
                    break;
                }
                dfa.setMinimized();
                cout<<"Operatie efectuata.\n";
                system("pause");
                break;
            }
            case 5:{
                exit(0);
            }
        }
    }
}
