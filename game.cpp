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

    // 6. Mapa Terr - TypTerr
    stringToTerr["cos1"]      = TypTerr::cos1;
    stringToTerr["cos2"] = TypTerr::cos2;
    
    // Skróty 
    stringToProducer["kop_kamien"]   = TypProducer::KOPALNIA_KAMIENIA;
    stringToProducer["kop_tytan"]    = TypProducer::KOPALNIA_TYTANU;
    stringToProducer["zaaw_kop"] = TypProducer::ZAAWANSOWANA_KOPALNIA;

    loadGameData();
    
}

void Game::run(){
   
    cout<<CLEAR_SCREEN<<YELLOW<<"Witaj przybyszu!"<<RESET<<endl;
    kolonia.setNazwa();
  
    while(running){
        commands();
    }
}

void Game::commands(){
    cout<<BLUE<<">>"<<RESET;
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
        if(arg1=="full"){//Wszystkich budynków szczegolowo - testowe
            kolonia.prntBuildings();
        }
        else if(isdigit(arg1[0])){//Sam jeden budynek szczegółowo - testowe
            kolonia.prntBuilding(stoi(arg1));
        }
        else if(arg1=="d"){//Testowe
            kolonia.prntBuildingsShort();
        }
        else if(arg1.empty()){
            //kolonia.prntBuildingsShort();
            kolonia.prntBuildingsSumm();
        }
        else if(kolonia.czyBudynek(arg1)){
            kolonia.prntBuilding(arg1);
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
        string arg1;
        ss>>arg1;

        if(arg1.empty()){
            cout << YELLOW << "Aby zbudowac, wpisz: " <<WHITE<< BG_BLACK << "build [nazwa]" << RESET << endl<<endl;
            cout << YELLOW << "Aby zobaczyc liste budynkow, wpisz: " <<WHITE<< BG_BLACK << "info" << RESET << endl;
            return;
        }
        
        if(kolonia.getRuch()==3){
            cout<<YELLOW<<"Juz wykorzystales "<<BOLD<<"3/3"<<NO_BOLD<<" ruchow w tej turze!! \nWpisz "<<WHITE<<BG_BLACK<<"next "<<RESET<<YELLOW<<"aby przejsc do kolejnej rundy!"<<RESET<<endl;
            return;
        }

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
        }else if(stringToTerr.count(arg1)){
            if(kolonia.zbudujBudynek(stringToTerr[arg1])){
                kolonia.setRuch(kolonia.getRuch()+1);
            }
        
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
   
    } else if(command=="info"){
        string arg1;
        ss>>arg1;
        if(arg1.empty()){
            prntCategories();
        }else{
            prntInfo(arg1);
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
    else if(command=="help"){
        prntHelp();
    }
    else if(command=="destroy"){//Niszczenie konkretnego budynku.

        string arg1;

        ss>>arg1;

        if(isdigit(arg1[0])){

            kolonia.zburzBudynek(stoi(arg1));//Testowe

        }else{

            kolonia.zburzBudynek(arg1);

        }
    }
    else{//Zła komenda
        cout<<RED<<BOLD<<"Nieznana komenda!!\n\n"<<RESET;
        
    }
}



void Game::loadGameData(){
    ifstream plik("gamedata.txt");
    if (!plik.is_open()) {
        cout << RED<<BOLD << "BLAD: Nie mozna otworzyc gamedata.txt!" << RESET << endl;
        return;
    }

    string n, type, opis;
    double kK, kT, kE, gk, gt, gi; 
    int w, x;

    while(plik>>n>>type>>kK>>kT>>kE>>w>>gk>>gt>>gi>>x>>opis){
        replace(opis.begin(), opis.end(), '_', ' ');

        BuildingInfo nowy;
        nowy.nazwa=n;
        nowy.type=type;
        nowy.kKamien=kK;
        nowy.kTytan=kT;
        nowy.reqEnergy=kE;
        nowy.workers=w;
        nowy.genKamien=gk;
        nowy.genTytan=gt;
        nowy.genInne=gi;
        nowy.x=x;
        nowy.opis=opis;
        string klucz =n;
        klucz[0]=tolower(klucz[0]);
        bazaDanych[klucz]=nowy;
    }
    plik.close();
}

void Game::prntInfo(string cat){
    for (auto &c : cat) c = toupper(c);
    if(!(cat=="ENERGY"||cat=="HOUSING"||cat=="FARM"||cat=="PRODUCER"||cat=="TERR")){
        cout<<RED<<BOLD<<"Nie ma takiej kategorii!!"<<RESET<<endl;
        return;
    }
    prntHeader("SZCZEGOLY KATEGORI: "+cat);
    cout<<YELLOW<<string(120,'-')<<RESET<<endl;
    string sep=" | ";
    const int w_n = 20;
    const int w = 20;
    const int w_op = 30;

    cout << BLUE <<BOLD<< left << setw(w_n) << "NAZWA"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "koszt budowy: KAMIEN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "koszt budowy: TYTAN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "WORKERS"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "wymagany PRAD";
    if(cat=="ENERGY"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "gen. PRAD"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }else     
    if(cat=="HOUSING"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "residents"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }else 
    if(cat=="FARM"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "gen. jedzenie"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "TIME"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }else 
    if(cat=="PRODUCER"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "gen. STONE"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "gTYTAN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }else 
    if(cat=="TERR"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w) << "gen. terr"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }
    cout<<YELLOW<<string(120,'-')<<RESET<<endl;

    for(const auto &[k,info]:bazaDanych){
        if(cat==info.type){
            cout<<left<<setw(w_n)<<info.nazwa<<YELLOW<<sep<<RESET<<setw(w)<<info.kKamien<<YELLOW<<sep<<RESET<<setw(w)<<info.kTytan<<YELLOW<<sep<<RESET<<setw(w)<<info.workers<<YELLOW<<sep<<RESET<<setw(w)<<info.reqEnergy;
            if(cat=="ENERGY"){
                cout<<YELLOW<<sep<<RESET<< setw(w) << info.genInne<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }else     
            if(cat=="HOUSING"){
                cout<<YELLOW<<sep<<RESET<< setw(w) << info.genInne<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }else
            if(cat=="FARM"){
                cout<<YELLOW<<sep<<RESET<< setw(w) << info.genInne<<YELLOW<<sep<<RESET<<setw(w)<<info.x<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }else
            if(cat=="PRODUCER"){
                cout<< YELLOW<<sep<<RESET<<setw(w) << info.genKamien<<YELLOW<<sep<<RESET<<setw(w)<<info.genTytan<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }else
            if(cat=="TERR"){
                cout<< YELLOW<<sep<<RESET<<setw(w) << info.genInne<<RESET<<endl;
            }
        
        
        }
    }
}

void Game::prntBudynki(){
    vector<string> y_energy   = {"Wiatrak", "Panele"};
    vector<string> y_farm     = {"Szklarnia", "Pole"};
    vector<string> y_housing  = {"Barak", "Rezydencja"};
    vector<string> y_producer = {"Kop_Kamien", "Kop_Tytan", "Zaaw_kop"};
    vector<string> y_terr = {"Cos1","Cos2"};
    //tu dodawac jak jakies nowe klasy / budynki

    vector<vector<string>> colums= {y_energy, y_farm, y_housing, y_producer,y_terr};
    vector<string> headers={"ENERGY","FARM","HOUSING","PRODUCER","TERR"};//pamietaco o dodaniu tutaj jak nowe
    const int width=15;
    const string sep="|";
    cout<<endl;
    prntHeader("DOSTEPNE BUDYNKI");
    //linia przed
    for (int i = 0; i < 5; i++) {
        cout << YELLOW<<"+" << string(width + 1, '-')<<RESET;
    }
    cout << YELLOW<<"+" << RESET<<endl;
    for(const auto&h:headers){
        cout<<YELLOW<<sep<<" "<<BOLD<<BLUE<<left<<setw(width)<<h<<RESET;
    }
    //linia gorna
    cout<<YELLOW<<sep<<RESET<<endl;
    for (int i = 0; i < 5; i++) {
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
    for (int i = 0; i < 5; i++) {
        cout << YELLOW<<"+" << string(width + 1, '-')<<RESET;
    }
        cout << YELLOW<<"+" << RESET<<endl;
        cout<<endl;
}   


void Game::prntCategories(){
    const int w = 25;
    const string sep = " | ";

    prntHeader("LISTA KATEGORII BUDYNKOW");

    cout << YELLOW << BOLD << left << setw(w) << "KATEGORIA" << sep << "OPIS" << RESET << endl;
    cout << YELLOW << string(w, '-') << "-+-" << string(60, '-') << RESET << endl; 

    // ENERGY
    cout << BLUE << BOLD << left << setw(w) << "ENERGY" << RESET << YELLOW << sep << "Systemy zasilania. Niezbedne do funkcjonowania kopaln i farm." << RESET << endl;

    // HOUSING 
    cout << BLUE << BOLD << left << setw(w) << "HOUSING" << RESET << YELLOW << sep<< "Kwatery dla zalogi. Zwieksza limit populacji i sile robocza." << RESET << endl;

    // FARM
    cout << BLUE << BOLD << left << setw(w) << "FARM" << RESET << YELLOW << sep << "Produkcja zywnosci. Kluczowa do utrzymania kolonistow przy zyciu." << RESET << endl;

    // PRODUCER
    cout << BLUE << BOLD << left << setw(w) << "PRODUCER" << RESET << YELLOW << sep << "Przemysl wydobywczy. Pozyskuje surowce konstrukcyjne (Kamien, Tytan)." << RESET << endl;

    // TERR
    cout << BLUE << BOLD << left << setw(w) << "TERR" << RESET << YELLOW << sep << "Inzynieria planetarna. Przystosowuje atmosfere Marsa (odblokowuje nowe technologie)." << RESET << endl;
         
    cout << endl;

}

void Game::prntHelp(){
    const int w = 25;
    const string sep = " | ";

    prntHeader("LISTA DOSTEPNYCH KOMEND");

    cout << YELLOW << BOLD << left << setw(w) << "KOMENDA" << sep << "OPIS" << RESET << endl;
    cout << YELLOW << string(w, '-') << "-+-" << string(60, '-') << RESET << endl; 

    // INFO 
    cout << BG_BLACK << left << setw(w) << "info" << RESET << YELLOW << sep << "Wyswietla liste dostepnych kategorii budynkow." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "info [kategoria]" << RESET << YELLOW << sep << "Pokazuje szczegoly budynkow z danej kategorii (np. info ENERGY)." << RESET << endl;

    // BUILD
    cout << BG_BLACK << left << setw(w) << "build [nazwa]" << RESET << YELLOW << sep << "Buduje budynek (np. " << BG_BLACK << WHITE << "build wiatrak" << RESET << YELLOW << ")." << RESET << endl;

    // SHOW
    cout << BG_BLACK << left << setw(w) << "show" << RESET << YELLOW << sep << "Podsumowanie ilosci zbudowanych budynkow." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "show full" << RESET << YELLOW << sep << "Szczegolowa lista wszystkich zbudowanych budynkow." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "show [id]" << RESET << YELLOW << sep << "Szczegoly konkretnego zbudowanego budynku (po ID)." << RESET << endl;

    // COLONY 
    cout << BG_BLACK << left << setw(w) << "colony" << RESET << YELLOW << sep << "Podsumowanie surowcow, energii i pracownikow." << RESET << endl;
    
    // NEXT
    cout << BG_BLACK << left << setw(w) << "next" << RESET << YELLOW << sep << "Konczy ture (produkcja/konsumpcja)." << RESET << endl;

    // DESTROY 
    cout << BG_BLACK << left << setw(w) << "destroy [id/nazwa]" << RESET << YELLOW << sep << "Niszczy budynek (odzyskuje czesc surowcow/ludzi)." << RESET << endl;
    
    // RENAME
    cout << BG_BLACK << left << setw(w) << "rename" << RESET << YELLOW << sep << "Zmiana nazwy kolonii." << RESET << endl;

    // SAVE/LOAD , EXIT
    cout << BG_BLACK << left << setw(w) << "save / load" << RESET << YELLOW << sep << "Zapis i wczytanie stanu gry." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "exit" << RESET << YELLOW << sep << "Wyjscie z gry." << RESET << endl;

    cout << endl;
}

