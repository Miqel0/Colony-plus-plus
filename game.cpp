#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <cctype>
#include <iomanip>
#include <vector>
#include <algorithm>

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
    stringToProducer["kop_kamien"]   = TypProducer::KOPALNIA_KAMIENIA;
    stringToProducer["kop_tytan"]    = TypProducer::KOPALNIA_TYTANU;
    stringToProducer["zaaw_kop"] = TypProducer::ZAAWANSOWANA_KOPALNIA;
    
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
        if(arg1=="full"){//Wszystkich budynków szczegolowo
            kolonia.prntBuildings();
        }
        else if(isdigit(arg1[0])){//Sam jeden budynek szczegółowo
            kolonia.prntBuilding(stoi(arg1));
  
        }
        else if(arg1.empty()){
            kolonia.prntBuildingsShort();
        }
        else{
            cout<<RED<<"Nie ma takiej komendy!"<<RESET<<endl;
        }
    }
    else if(command=="colony"){
        kolonia.prnt();//prostsza wersja kolonii
    }
    else if(command=="next"){//Kolejna runda
        if(!kolonia.nextRound()){
            cout<<RED<<BOLD<<">>>>>>> KONIEC GRY!! <<<<<<<"<<endl;
            cout<<">>>>>>> PRZEGRANA!! <<<<<<<"<<RESET<<endl;
            running=false;
        }

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
        if(kolonia.getRuch()==3){
            cout<<YELLOW<<"Juz wykorzystales "<<BOLD<<"3/3"<<NO_BOLD<<" ruchow w tej turze!! \nWpisz "<<WHITE<<BG_BLACK<<"next "<<RESET<<YELLOW<<"aby przejsc do kolejnej rundy!"<<RESET<<endl;
            return;
        }

        string arg1;
        ss>>arg1;
        if(stringToBudynku.count(arg1)){
            if(kolonia.zbudujBudynek(stringToBudynku[arg1])){
                kolonia.setRuch(kolonia.getRuch()+1);
            }
        }else if(stringToEnergy.count(arg1)){
            if(kolonia.zbudujBudynek(stringToEnergy[arg1])){
                kolonia.setRuch(kolonia.getRuch()+1);
            }
        }else if(stringToDomy.count(arg1)){
            if(kolonia.zbudujBudynek(stringToDomy[arg1])){
                kolonia.setRuch(kolonia.getRuch()+1);
            }
        }else if(stringToFarm.count(arg1)){
            if(kolonia.zbudujBudynek(stringToFarm[arg1])){
                kolonia.setRuch(kolonia.getRuch()+1);
            }
        }else if(stringToProducer.count(arg1)){
            if(kolonia.zbudujBudynek(stringToProducer[arg1])){
                kolonia.setRuch(kolonia.getRuch()+1);
            }
        }else if(arg1.empty()){
            prntBudynki();
            return;
        }
        else{
            cout<<RED<<"Nie istnieje taki budynek!"<<RESET<<endl;
            return;
        }
        
        if(kolonia.getRuch()==3){
            cout<<YELLOW<<"Wlasnie wykorzystales "<<BOLD<<MAGENTA<<"3/3"<<NO_BOLD<<YELLOW<<" ruchow w tej turze!! \nWpisz "<<WHITE<<BG_BLACK<<"next "<<RESET<<YELLOW<<"aby przejsc do kolejnej tury!"<<RESET<<endl;
            
        }else{
            cout<<YELLOW<<"Wykorzystales "<<BOLD<<MAGENTA<<kolonia.getRuch()<<"/3"<<NO_BOLD<<YELLOW<<" ruchow w tej turze!!" <<RESET<<endl;
         
        }
    }
    else if(command=="destroy"){//Niszczenie konkretnego budynku.
        int arg1;
        if(ss>>arg1){
            kolonia.zburzBudynek(arg1);//dac jak arg1 jest strigniem
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

void Game::prntBudynki(){
    vector<string> y_energy   = {"Wiatrak", "Panele"};
    vector<string> y_farm     = {"Szklarnia", "Pole"};
    vector<string> y_housing  = {"Barak", "Rezydencja"};
    vector<string> y_producer = {"Kop_Kamien", "Kop_Tytan", "Zaaw_kop"};
    //tu dodawac jak jakies nowe klasy / budynki

    vector<vector<string>> colums= {y_energy, y_farm, y_housing, y_producer};
    vector<string> headers={"ENERGY","FARM","HOUSING","PRODUCER"};//pamietaco o dodaniu tutaj jak nowe
    const int width=15;
    const string sep="|";
    cout<<endl;
    prntHeader("DOSTEPNE BUDYNKI");
    //linia przed
    for (int i = 0; i < 4; i++) {
        cout << YELLOW<<"+" << string(width + 1, '-')<<RESET;
    }
        cout << YELLOW<<"+" << RESET<<endl;
        cout<<endl;
    for(const auto&h:headers){
        cout<<YELLOW<<sep<<" "<<BOLD<<BLUE<<left<<setw(width)<<h<<RESET;
    }
    //linia gorna
    cout<<YELLOW<<sep<<RESET<<endl;
    for (int i = 0; i < 4; i++) {
        cout << YELLOW<<sep << string(width + 1, '-')<<RESET; 
    }
    cout <<YELLOW<< sep << RESET<<endl;
    int size=0;
    for(const auto&col:colums){
        if(col.size()>size){
            size=col.size();
        }
    }
    for(int wiersz=0;wiersz<size;wiersz++){
        cout <<YELLOW<< sep << RESET;
        for(const auto& col:colums){
            if(wiersz<col.size()){
                cout<<" "<<left<<setw(width)<<col[wiersz];
            }
            else{
                cout<<" "<<setw(width)<<"";
            }
            cout <<YELLOW<< sep << RESET; 
        }
        cout <<endl;
    }
    //linia dol
    for (int i = 0; i < 4; i++) {
        cout << YELLOW<<"+" << string(width + 1, '-')<<RESET;
    }
        cout << YELLOW<<"+" << RESET<<endl;
        cout<<endl;
}   
