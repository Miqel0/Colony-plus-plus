#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <memory>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;
#include "game.h"

Game::Game():running(true){

}

void Game::run(){
    while(running){
        commands();
    }
}

void Game::commands(){
    cout<<YELLOW<<">>"<<RESET;
    string linia;
    getline(cin,linia);
    if(linia.empty()){return;}
    stringstream ss(linia);
    string command;
    ss>>command;

    if(command=="show"){
        string arg1;
        ss>>arg1;
        if(arg1=="kolonia"){
        kolonia.prnt();
        }
        else if(arg1=="budynki"){
            string arg2;
            ss>>arg2;
            if(arg2=="full"){
                kolonia.prntBuildings();
            }
            else{
            kolonia.prntBuildingsShort();
            }
        }
        else if(arg1=="budynek"){
            int arg2;
            ss>>arg2;
            if(arg2>=0){
                kolonia.prntBuilding(arg2);
            }else{
                cout<<RED<<BOLD<<"BLAD!"<<RESET<<RED<<" Podaj nr budynku, ktory chcesz wyburzyc!!\n";
                cout<<endl;
            }
        }
    }
    else if(command=="next"){
        kolonia.nextRound();
    }
    else if(command=="exit"){

        string dec;
        string arg1;
        ss>>arg1;
        if(arg1=="tak"){
            cout<<CYAN<<BOLD<<"Zamykanie gry!"<<YELLOW<<"\n<> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> "<<RESET<<endl;
            running=false;
        }else{
        cout<<YELLOW<<"Czy na pewno chcesz wyjsc z gry? Stracisz niezapisany postep!\n(TAK/NIE)\n"<<RESET;
        cin>>dec;
        if(dec=="TAK"||dec=="tak"||dec=="Tak"){
        cout<<CYAN<<BOLD<<"Zamykanie gry!"<<YELLOW<<"\n<> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> "<<RESET<<endl;
        running=false;
        }else{
            cout<<CYAN<<BOLD<<"Anulowano zamykanie gry!\n"<<RESET<<endl;
        }}
    }
    else if(command=="zbuduj"){
        kolonia.zbudujBudynek(TypDomy::REZYDENCJA);
        kolonia.zbudujBudynek(TypEnergy::WIATRAK);
        kolonia.zbudujBudynek(TypEnergy::WIATRAK);
        kolonia.zbudujBudynek(TypFarm::POLE);
    }


    else{
        cout<<RED<<BOLD<<"Nieznana komenda!!\n\n"<<RESET;
        
    }
}