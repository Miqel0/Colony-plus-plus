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
#include <map>

using namespace std;
#include "game.h"

Game::Game():running(true){

    // Mapa głównych kategorii - TypBudynku
    stringToBudynku["energy"]   = TypBudynku::ENERGY;
    stringToBudynku["farm"]     = TypBudynku::FARM;
    stringToBudynku["housing"]  = TypBudynku::HOUSING;
    stringToBudynku["producer"] = TypBudynku::PRODUCER;

    // Mapa Energy - TypEnergy
    stringToEnergy["wiatrak"]          = TypEnergy::WIATRAK;
    stringToEnergy["panele"]           = TypEnergy::PANELE;

    // 3. Mapa Farm - TypFarm
    stringToFarm["pole"]      = TypFarm::POLE;
    stringToFarm["szklarnia"] = TypFarm::SZKLARNIA;


    // 4. Mapa Domy - TypDomy
    stringToDomy["barak"]      = TypDomy::BARAK;
    stringToDomy["rezydencja"] = TypDomy::REZYDENCJA;


    // 5. Mapa Producer - TypProducer
    stringToProducer["kopalnia_kamienia"]     = TypProducer::KOPALNIA_KAMIENIA;
    stringToProducer["kopalnia_tytanu"]       = TypProducer::KOPALNIA_TYTANU;
    stringToProducer["zaawansowana_kopalnia"] = TypProducer::ZAAWANSOWANA_KOPALNIA;
    
    // Skróty 
    stringToProducer["kamien"]   = TypProducer::KOPALNIA_KAMIENIA;
    stringToProducer["tytan"]    = TypProducer::KOPALNIA_TYTANU;
    stringToProducer["zaaw"] = TypProducer::ZAAWANSOWANA_KOPALNIA;
    
}

void Game::run(){
   
    cout<<CLEAR_SCREEN<<YELLOW<<"Witaj przybyszu!"<<RESET<<endl;
    kolonia.setNazwa();
  
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
    //Opisane kolejne komendy

    
    if(command=="show"){//Pokazywanie konkretych elementów:
        string arg1;
        ss>>arg1;
        if(arg1=="kolonia"){//Danych o koloniii
        kolonia.prnt();
        }
        else if(arg1=="budynki"){//Wszystkich budynków
            string arg2;
            ss>>arg2;
            if(arg2=="full"){//Wszystkie budynki szczegółowo
                kolonia.prntBuildings();
            }
            else{//Wszystkie budynki ale same nazwy
            kolonia.prntBuildingsShort();
            }
        }
        else if(arg1=="budynek"){//Sam jeden budynek szczegółowo
            int arg2;
            if(ss>>arg2){
                kolonia.prntBuilding(arg2);
            }else{
                cout<<RED<<BOLD<<"BLAD!"<<RESET<<RED<<" Podaj poprawny nr budynku, ktorego chcesz zobaczyc!!\n";
                cout<<endl;
            }
        }
        else if(arg1.empty()){
            kolonia.prntBuildingsShort();
        }
    }
    else if(command=="colony"){
        kolonia.prnt();//prostsza wersja kolonii
    }
    else if(command=="next"){//Kolejna runda
        kolonia.nextRound();
    }
    else if(command=="exit"){//Wyjscie z gry

        string dec;
        string arg1;
        ss>>arg1;
        if(arg1=="tak"){//testowe
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
    else if(command=="x"){//testowe
        cout<<CYAN<<BOLD<<"Zamykanie gry!"<<YELLOW<<"\n<> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> "<<RESET<<endl;
        running=false;
    }


    else if(command=="build"){//Budowanie konkretnego budynku 
        string arg1;
        ss>>arg1;
        if(stringToBudynku.count(arg1)){
            kolonia.zbudujBudynek(stringToBudynku[arg1]);
        }else if(stringToEnergy.count(arg1)){
            kolonia.zbudujBudynek(stringToEnergy[arg1]);
        }else if(stringToDomy.count(arg1)){
            kolonia.zbudujBudynek(stringToDomy[arg1]);
        }else if(stringToFarm.count(arg1)){
            kolonia.zbudujBudynek(stringToFarm[arg1]);
        }else if(stringToProducer.count(arg1)){
            kolonia.zbudujBudynek(stringToProducer[arg1]);
        }else{
            cout<<RED<<"Nie istnieje taki budynek!"<<RESET<<endl;
        }
    }
    else if(command=="destroy"){//Niszczenie konkretnego budynku.
        int arg1;
        if(ss>>arg1){
            kolonia.zburzBudynek(arg1);
        }
    }
    else if(command=="rename"){
        kolonia.setNazwa();
    }
    else if(command=="save"){
        kolonia.save();
    }
    else if(command=="load"){
        kolonia.load();
    }

    else{//Zła komenda
        cout<<RED<<BOLD<<"Nieznana komenda!!\n\n"<<RESET;
        
    }
}