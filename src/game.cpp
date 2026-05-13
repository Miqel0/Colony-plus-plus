#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <cctype>
#include <iomanip>
#include <vector>

#include <algorithm>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <optional>

using namespace std;
#include "game.h"

// ==========================================
// KONSTRUKTOR 
// ==========================================

Game::Game():running(true){

    //Definiowanie mapy stringow do typu danej budynku (nie dalo sie tego automatycznie zrobic)

    // ENERGY
    stringToEnergy["maly_wiatrak"]    = TypEnergy::MALY_WIATRAK;
    stringToEnergy["duzy_panel"]      = TypEnergy::DUZY_PANEL;
    stringToEnergy["reaktor_jadrowy"] = TypEnergy::REAKTOR_JADROWY;
    stringToEnergy["fuzja_zimna"]     = TypEnergy::FUZJA_ZIMNA;

    // FARM
    stringToFarm["pole_ziemniakow"]   = TypFarm::POLE_ZIEMNIAKOW;
    stringToFarm["szklarnia_hydro"]   = TypFarm::SZKLARNIA_HYDRO;
    stringToFarm["farma_alg"]         = TypFarm::FARMA_ALG;
    stringToFarm["syntezator_bialka"] = TypFarm::SYNTEZATOR_BIALKA;

    // HOUSING
    stringToDomy["barak_robotniczy"]  = TypDomy::BARAK_ROBOTNICZY;
    stringToDomy["kwatery_zalogi"]    = TypDomy::KWATERY_ZALOGI;
    stringToDomy["kopula_mieszkalna"] = TypDomy::KOPULA_MIESZKALNA;
    stringToDomy["metropolia"]        = TypDomy::METROPOLIA;

    // PRODUCER
    stringToProducer["odkrywka_kamienia"]  = TypProducer::ODKRYWKA_KAMIENIA;
    stringToProducer["wiertlo_glebinowe"]  = TypProducer::WIERTLO_GLEBINOWE;
    stringToProducer["kombinat_gorniczy"]  = TypProducer::KOMBINAT_GORNICZY;
    stringToProducer["automat_wydobywczy"] = TypProducer::AUTOMAT_WYDOBYWCZY;
    stringToProducer["kopalnia_tytanu"] = TypProducer::KOPALNIA_TYTANU;

    // TERR
    stringToTerr["stacja_badawcza"]  = TypTerr::STACJA_BADAWCZA;
    stringToTerr["kominy_cieplne"]   = TypTerr::KOMINY_CIEPLNE;
    stringToTerr["generator_o2"]     = TypTerr::GENERATOR_O2;
    stringToTerr["lustra_orbitalne"] = TypTerr::LUSTRA_ORBITALNE;

    //Wczytywanie parametrow budynkow
    loadGameData();
}

// ==========================================
// GRA
// ==========================================

void Game::UIrun(){
    kolonia.load();
    grafika.prntAll(kolonia);
}

void Game::run(){
    cout << CLEAR_SCREEN;

    
    string tryb;
    while(true) {
        
        cout << YELLOW << "WYBIERZ TRYB GRY:" << RESET << endl;
        cout << " [" << BOLD << "1" << NO_BOLD << "] NORMALNY (Podstawowa wersja)" << endl;
        cout << " [" << BOLD << "2" << NO_BOLD << "] SANDBOX / TESTY (Duzo surowcow, odblokowane budynki)" << endl;
        cout << " [" << BOLD << "3" << NO_BOLD << "] CUSTOM (Wpisz wlasne wartosci)" << endl;
        cout << BLUE << ">> " << RESET;
        cin>>tryb;
        
        if(tryb=="1"|tryb=="2"|tryb=="3") { 
            int b = stoi(tryb);
            switch(stoi(tryb)){
                case 1:
                    break;
                case 2:
                    kolonia.setSandbox();
                    saveConfig();
                    break;
                case 3:
                    kolonia.setCustom();
                    saveConfig();
                    break;
                default:
                    break;
            }
            break;
        }else{
            cout << RED << "To nie jest liczba! Wpisz 1, 2 lub 3: " << BLUE << endl;
            cout<<endl;
        }
    }
    
    cout <<YELLOW<< endl << string(50, '-') <<RESET<< endl; 
    
    cout<<YELLOW<<"Czy chcesz przejsc samouczek? (y/n)"<<RESET<<endl;
    string odp;
    cin>>odp;
    if(odp=="y"||odp=="yes"||odp=="tak"||odp=="t"){
        startTutorial();
    }


    cout<<YELLOW<<"Czy chcesz wczytac zapisana gre? (y/n)"<<RESET<<endl;

    cin>>odp;
    if(odp=="y"||odp=="yes"||odp=="tak"||odp=="t"){
        kolonia.load();
    }
    

    // //Podstawowa wrsja poczatku gry - bez testowania

    // cout <<YELLOW<< endl << string(50, '-') <<RESET<< endl;
    // if(checkConfig()){
    //     cout<<YELLOW<<"Czy chcesz wczytac zapisana gre? (y/n)"<<RESET<<endl;
    //     string odp;
    //     cin>>odp;
    //     if(odp=="y"||odp=="yes"||odp=="tak"||odp=="t"){
    //         kolonia.load();
    //     }else{
    //         startTutorial();
    //     }
    // } else{
    //     startTutorial();
    //     saveConfig();
    // }

    cout <<YELLOW<< endl << string(50, '-') <<RESET<< endl; 

    while(running){
        commands();
    }
}

void Game::startTutorial() {
    cout << CLEAR_SCREEN;
    cout << RED << "------------------------------------------------------------" << RESET << endl;
    cout << BOLD << MAGENTA << "                 ROK 2142. SEKTOR OMEGA-4." << RESET << endl;
    cout << RED << "------------------------------------------------------------" << RESET << endl << endl;
    
    cout << YELLOW << "Komputer pokladowy: " << GREEN << "ONLINE" << RESET << endl;
    cout << YELLOW << "Systemy podtrzymywania zycia: " << GREEN << "STABILNE" << RESET << endl;
    cout << "Witaj, Kapitanie. Ladownik osiadl na powierzchni Marsa." << endl << endl;

    //  NAZWA 
    cout << CYAN << "[NARRATOR]:" << RESET << " Pierwszym krokiem jest zarejestrowanie nazwy naszej placowki w systemie." << endl;
    kolonia.setNazwa(); 
    cout << endl;

    // KOLONIA
    cout << CYAN << "[NARRATOR]:" << RESET << " Swietnie. Zanim zaczniemy budowe, musisz ocenic nasze zasoby." << endl;
    cout << "Wpisz komende " << BG_BLACK << RED << " colony " << RESET << ", aby wyswietlic stan magazynow." << endl;
    if (cin.peek() == '\n') cin.ignore();
    string komenda;

    while(true) {
        cout << BLUE << ">>" << RESET;
        getline(cin, komenda);
        for(auto &c : komenda) c = tolower(c);

        if(komenda == "colony") {
            kolonia.prnt();
            break;
        } else {
            cout << RED << "[SYSTEM]: Niepoprawna komenda. Prosze wpisac 'colony'." << RESET << endl;
        }
    }

    // BUILD WIATRAK 
    cout << endl;
    cout << CYAN << "[NARRATOR]:" << RESET << " Mamy troche Kamienia, ale zero Energii. A bez pradu nie beda dzialac zadne budynki." << endl;
    cout << "Musimy postawic " << BLUE << "Maly_Wiatrak" << RESET << ". Zapewni on zasilanie dla reszty bazy." << endl;
    cout << "Wpisz: " << BG_BLACK << BLUE << " build maly_wiatrak " << RESET << endl;

    while(true) {
        cout << BLUE << ">>" << RESET;
        getline(cin, komenda);
        for(auto &c : komenda) c = tolower(c);

        if(komenda == "build maly_wiatrak") {
            if(stringToEnergy.count("maly_wiatrak")){
                build(bazaDanych["maly_wiatrak"]);
                kolonia.setRuch(kolonia.getRuch()+1);
            }
            break;
        } else {
            cout << RED << "[SYSTEM]: Blad. Priorytetem jest 'build maly_wiatrak'." << RESET << endl;
        }
    }

    // BUILD FARM
    cout << endl;
    cout << CYAN << "[NARRATOR]:" << RESET << " Doskonale! Turbiny sie kreca wiec mamy prad!. Teraz czas na jedzenie." << endl;
    cout << "Zapasy sa male. Musimy zasiac " << GREEN << "Pole_Ziemniakow" << RESET << ", zanim zaloga zacznie glodowac." << endl;
    cout << "Wpisz: " << BG_BLACK << GREEN << " build pole_ziemniakow " << RESET << endl;

    while(true) {
        cout << BLUE << ">>" << RESET;
        getline(cin, komenda);
        for(auto &c : komenda) c = tolower(c);

        if(komenda == "build pole_ziemniakow") {
            if(stringToFarm.count("pole_ziemniakow")){
                build(bazaDanych["pole_ziemniakow"]);
                kolonia.setRuch(kolonia.getRuch()+1);
            }
            break;
        } else {
            cout << RED << "[SYSTEM]: Zaloga jest glodna! Wpisz 'build pole_ziemniakow'." << RESET << endl;
        }
    }
    // KAMIEN
    cout << endl;
    cout << CYAN << "[NARRATOR]:" << RESET << " Jedzenie mamy zapewnione, ale jak tak dalej pojdzie to w koncu nasz magazyn z Kamieniem bedzie pusty!" << endl;
    cout << "Aby budowac dalej, musimy wydobywac surowce z powierzchni. Zbudujmy " << YELLOW << "Odkrywke_Kamienia" << RESET << "." << endl;
    cout << "Wpisz: " << BG_BLACK << YELLOW << " build odkrywka_kamienia " << RESET << endl;

    while(true) {
        cout << BLUE << ">>" << RESET;
        getline(cin, komenda);
        for(auto &c : komenda) c = tolower(c);

        if(komenda == "build odkrywka_kamienia") {
            
            build(bazaDanych["odkrywka_kamienia"]);
                
            
            break;
        } else {
            cout << RED << "[SYSTEM]: Brak surowcow do dalszej ekspansji! Wpisz 'build odkrywka_kamienia'." << RESET << endl;
        }
    }
    // NEXT
    cout << endl;
    cout << CYAN << "[NARRATOR]:"<<RESET<<" W kazdej turze mozesz wykonac maksymalnie " << BOLD <<CYAN<< "3 AKCJE BUDOWANIA" << NO_BOLD <<WHITE<< "." << endl;
    cout << "Wykorzystales juz limit "<<RED<<"(3/3)"<<WHITE<<". Aby nasi ludzie mogli odpoczac, a maszyny wyprodukowac surowce, musisz zakonczyc ture." << endl;
    cout << endl;
    cout << "Wpisz komende " << BG_BLACK << CYAN << " next " << RESET << ", aby przejsc dalej." << endl;

    while(true) {
        cout << BLUE << ">>" << RESET;
        getline(cin, komenda);
        for(auto &c : komenda) c = tolower(c);

        if(komenda == "next") {
            int wynik=90;
            wynik = kolonia.nextRound();
            if(wynik!=0){
                break;
            }else{
                cout << RED << "[SYSTEM]: Nie mozesz nic wiecej zbudowac! Musisz przejsc do kolejnej rundy aby dalej kontynuowac samouczek." << RESET << endl;
            }
        } else {
            cout << RED << "[SYSTEM]: Nie mozesz nic wiecej zbudowac! Musisz wpisac 'next'." << RESET << endl;
        }
    }
    
    
    // STACJA BADAWCZA
    cout << endl;
    cout << CYAN << "[NARRATOR]:" << RESET << "Mamy surowce, ale potrzebuje miec ich stala produkcje. Aby ,a zeby zdobyc " << MAGENTA << "Tytan" << RESET << ", musimy poczynic postepy w Terraformacji." << endl;
    cout << "Musimy zaczac zwiekszac wspolczynnik Terraformacji. Zbuduj " << MAGENTA << "Stacje_Badawcza" << RESET << ", aby zwiekszac poziom Terraformacji." << endl;
    cout << "Dzieki niej odblokujesz lepsze budynki i kopalnie tytanu." << endl;
    cout << "Wpisz: " << BG_BLACK << MAGENTA << " build stacja_badawcza " << RESET << endl;

    while(true) {
        cout << BLUE << ">>" << RESET;
        getline(cin, komenda);
        for(auto &c : komenda) c = tolower(c);

        if(komenda == "build stacja_badawcza") {
            if(stringToTerr.count("stacja_badawcza")){
                build(bazaDanych["stacja_badawcza"]);
                kolonia.setRuch(kolonia.getRuch()+1);
            }
            break;
        } else {
            cout << RED << "[SYSTEM]: Wpisz 'build stacja_badawcza'." << RESET << endl;
        }
    }
    kolonia.setRuch(1);
    // ZAKONCZENIE 
    cout << endl;
    cout << CYAN << "[NARRATOR]:" << RESET << " Baza operacyjna gotowa. Podstawowe systemy dzialaja." << endl;
    cout << "Teraz wszystko w Twoich rekach, Kapitanie." << endl;
    cout << "Pamietaj o rozwijaniu " << MAGENTA << "TERRAFORMACJI" << RESET << ", aby odblokowac lepsze technologie." << endl;
    cout << "Jakbys sie zgubil, wpisz "<<YELLOW<<"help" <<RESET<<" albo "<<YELLOW<<"rules"<<RESET<< endl;
    cout << "Powodzenia. Bez odbioru." << endl;
    cout << RED << "------------------------------------------------------------" << RESET << endl << endl;
}

void runGUI(){
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Colony ++");
    auto cos = ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {

            ImGui::SFML::ProcessEvent(window, *event);
            
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

        }
        
        ImGui::SFML::Update(window, deltaClock.restart());

        
        ImGui::SetNextWindowPos(ImVec2(100, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(240, 300), ImGuiCond_Once);

    ImGui::Begin("Panel Sterowania",nullptr,ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::Separator();
   
    ImGui::End();
        
        window.clear();
        
        
        ImGui::SFML::Render(window);
        window.display();
    }
    
    ImGui::SFML::Shutdown();
}


// ==========================================
// KOMENDY
// ==========================================

void Game::grafikaStart(){
    grafika.prntAll(kolonia);
}
/**
 * @brief Stara główna pętla gry z pisaniem komend
 * 
 */
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
        if(arg1.empty()){//Pokazanie wszystkich bzudowanych budynków
            kolonia.prntBuildingsSumm();
        }
        else if(bazaDanych.count(arg1)){//Pokazywanie informacji o zbudowanym budynku
            string poprawnaNazwa = bazaDanych[arg1].nazwa;
            kolonia.prntBuilding(poprawnaNazwa);
        }
        else{
            cout<<RED<<"Nie ma takiej komendy!"<<RESET<<endl;
        }
    }
    else if(command=="colony"){//Wyświetlanie danych kolonii
        kolonia.prnt();
    }
    else if(command=="next"){//Kolejna runda
        int wynik=kolonia.nextRound();
        if(wynik==2){
            prntNewLvlTerr();//Wyswietlanie odblokowanych budynkow
        }else if(wynik==-1){
            cout<<RED<<BOLD<<">>>>>>> KONIEC GRY!! <<<<<<<"<<endl;
            cout<<">>>>>>> PRZEGRANA!! <<<<<<<"<<RESET<<endl;
            running=false;
        }
    }
    else if(command=="exit"){//Wyjscie z gry

        string dec;
        string arg1;
        ss>>arg1;
        cout<<YELLOW<<"Czy na pewno chcesz wyjsc z gry? Stracisz niezapisany postep!\n(y/n)\n"<<RESET;
        cin>>dec;
        if(dec=="y"||dec=="yes"||dec=="tak"||dec=="t"){
            cout<<CYAN<<BOLD<<"Zamykanie gry!"<<YELLOW<<"\n<> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> "<<RESET<<endl;
            running=false;
        }else{
            cout<<CYAN<<BOLD<<"Anulowano zamykanie gry!\n"<<RESET<<endl;
        }
    }
    else if(command=="x"){//testowe wychodzenie z gry
        cout<<CYAN<<BOLD<<"Zamykanie gry!"<<YELLOW<<"\n<> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> "<<RESET<<endl;
        running=false;
    }


    else if(command=="build"){//Budowanie konkretnego budynku 
        string arg1;
        int ilosc=kolonia.getIloscBudynkow();
        ss>>arg1;
        
        if(arg1.empty()){ 
            cout << YELLOW << "Aby zbudowac, wpisz: " <<WHITE<< BG_BLACK << "build [nazwa]" << RESET << endl<<endl;
            cout << YELLOW << "Aby zobaczyc liste budynkow, wpisz: " <<WHITE<< BG_BLACK << "info" << RESET << endl;
            return;
        }
        for(auto &c : arg1) c = tolower(c);
        
        if(kolonia.getRuch()==3){ //Sprawdzanie warunku z iloscia ruchow w turze
            cout<<YELLOW<<"Juz wykorzystales "<<BOLD<<MAGENTA<<"3/3"<<NO_BOLD<<YELLOW<<" ruchow w tej turze!! \nWpisz "<<WHITE<<BG_BLACK<<"next "<<RESET<<YELLOW<<"aby przejsc do kolejnej rundy!"<<RESET<<endl;
            return;
        }
        arg1[0]=tolower(arg1[0]);
        
        if(bazaDanych.count(arg1)){
            BuildingInfo info = bazaDanych[arg1];
            if(info.lvlTerr>kolonia.getLvlTerr()){
                cout<<RED<<"Nie istnieje taki budynek!"<<RESET<<endl;
                return;
            }
            build(bazaDanych[arg1]);
        }else{
            cout<<RED<<"Nie istnieje taki budynek!"<<RESET<<endl;
            return;
        }
        if(kolonia.getRuch()==3){
            cout<<YELLOW<<"Wlasnie wykorzystales "<<BOLD<<MAGENTA<<"3/3"<<NO_BOLD<<YELLOW<<" ruchow w tej turze!! \nWpisz "<<WHITE<<BG_BLACK<<"next "<<RESET<<YELLOW<<"aby przejsc do kolejnej tury!"<<RESET<<endl;
            
        }else{
            if(ilosc==kolonia.getIloscBudynkow()){
                //cout<<YELLOW<<"Nie wykorzystales nowego ruchu! Aktualnie wykrozystales "<<BOLD<<MAGENTA<<kolonia.getRuch()<<"/3"<<NO_BOLD<<YELLOW<<" ruchow w tej turze!!" <<RESET<<endl;
            }else{
                cout<<YELLOW<<"Wykorzystales "<<BOLD<<MAGENTA<<kolonia.getRuch()<<"/3"<<NO_BOLD<<YELLOW<<" ruchow w tej turze!!" <<RESET<<endl;
            }
        }
    } 
    else if(command=="destroy"){//Niszczenie konkretnego budynku.

        string arg1;

        ss>>arg1;

        if(bazaDanych.count(arg1)){

            kolonia.zburzBudynek(arg1);
        }else{
            cout<<RED<<"Nie ma takiego budynku!!"<<RESET<<endl;
        }

    }else if(command=="info"){ //Wyswietlanie informacji o danych budynkach (przed zbudowniem)
        string arg1;
        ss>>arg1;
        if(arg1.empty()){ //Wyswietlanie kategorii
            prntCategories();
        }else{
            prntInfo(arg1); //Wyswietlanie infrmacji o danej kategorii
        }
    }

    else if(command=="rename"){ //Zmiana nazwy kolonii
        kolonia.setNazwa();
    }
    else if(command=="save"){//Zapis gry - wszystkich danych do plikow
        kolonia.save();
    }
    else if(command=="load"){//Wczytanie gry z plikow
        kolonia.load();
    }
    else if(command=="help"){//WYswietlenie helpa
        prntHelp();
    }
    else if(command=="rules"){//Wyswietlnie zasad / instrukcja
        prntRules();
    }
    else if(command=="imgui"){
        grafika.prntAll(kolonia);
    }
    else if(command=="cheat"){//Wlaczenie trybu z nieskonczonymi zasobami w trakcie gry
        kolonia.setSandbox();
    }
    
    else{//Zła komenda
        cout<<RED<<BOLD<<"Nieznana komenda!!\n\n"<<RESET;
    }
    cout<<endl;
}

// ==========================================
// RZECZY Z PLIKAMI (LOAD)
// ==========================================

bool Game::checkConfig(){ //Sprawdzanie config
    ifstream plik("data/config.txt");
    int czy;
    if (!plik.is_open()) {
        cout << RED<<BOLD << "BLAD: Nie mozna otworzyc config.txt!" << RESET << endl;
        return false;
    }else{
        plik>>czy;
        plik.close();
    }
    if(czy==1){
        return true;
    }else if(czy==0){
        return false;
    }else{
        return false;
    }
}

void Game::saveConfig(){ //Zapisywanie config
     ofstream plik("data/config.txt");
    if(plik.is_open()){
        plik<<1;
        plik.close();
        }
}

void Game::loadGameData(){ //Wczytywanie danych o budynkach - tylko na poczatku gry
    ifstream plik("data/gamedata.txt");
    if (!plik.is_open()) {
        cout << RED<<BOLD << "BLAD: Nie mozna otworzyc gamedata.txt!" << RESET << endl;
        return;
    }

    string n, type, opis;
    double kK, kT, kE, gk, gt, gi; 
    int w, x,lt;

    while(plik>>n>>type>>kK>>kT>>kE>>w>>gk>>gt>>gi>>x>>lt>>opis){
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
        nowy.lvlTerr=lt;
        nowy.opis=opis;
        string klucz =n;
        for(auto &c : klucz) c = tolower(c);

        bazaDanych[klucz]=nowy;
    }
    plik.close();
}

// ==========================================
// BUDOWANIE
// ==========================================

void Game::build(BuildingInfo info){//Budowanie budynkow w zaleznosci od danej kategorii
    string typ=info.type;
    string nazwa=info.nazwa;
    if(typ=="FARM"){
        if(kolonia.zbudujFarm(nazwa,info.reqEnergy,info.kKamien,info.kTytan,info.genInne,stringToFarm[nazwa],info.workers,info.x,0)){
            kolonia.setRuch(kolonia.getRuch()+1);
        }
    }
    else if(typ=="ENERGY"){
        if(kolonia.zbudujEnergy(nazwa,info.reqEnergy,info.kKamien,info.kTytan,info.genInne,stringToEnergy[nazwa],info.workers)){
            kolonia.setRuch(kolonia.getRuch()+1);
        }
    }
    else if(typ=="HOUSING"){
        if(kolonia.zbudujHousing(nazwa,info.reqEnergy,info.kKamien,info.kTytan,info.genInne,stringToDomy[nazwa],info.workers)){
            kolonia.setRuch(kolonia.getRuch()+1);
        }
    }
    else if(typ=="PRODUCER"){
        if(kolonia.zbudujProducer(nazwa,info.reqEnergy,info.kKamien,info.kTytan,info.genKamien,stringToProducer[nazwa],info.workers,info.genTytan)){
            kolonia.setRuch(kolonia.getRuch()+1);
        }
    }
    else if(typ=="TERR"){
        if(kolonia.zbudujTerr(nazwa,info.reqEnergy,info.kKamien,info.kTytan,info.genInne,stringToTerr[nazwa],info.workers)){
            kolonia.setRuch(kolonia.getRuch()+1);
        }
    }else{
        return;
    }
}

// ==========================================
// WYSWIETLANIE
// ==========================================

void Game::prntNewLvlTerr(){//Wyswietlanie komunikatu i nowych budynkow przy osiagnieciu nowego poziomu

        cout<<MAGENTA<<"Osiagnales kolejny poziom terraformacji!! \n"<<BOLD<<"Aktualny poziom "<<kolonia.getLvlTerr()<<RESET<<endl;
    
        //Sprawdzanie czy istnieje jakis budynek ktory sie odblokowal
        int a=0;
        for(const auto&[klucz,info]:bazaDanych){
            if(info.lvlTerr==kolonia.getLvlTerr()){
                a=1;
            }
        }
        if(a==0){
            return;}
        

        cout<<endl;
        cout<<GREEN<<"Odblokowywujesz nowe budynki!"<<RESET<<endl;
        cout<<endl;
        cout << MAGENTA << BOLD << left << setw(20) << "NAZWA" << RESET << YELLOW << " | " << "Opis" << RESET << endl;
        cout<<YELLOW<<string(42,'-')<<RESET<<endl;
        for(const auto &[k,info]:bazaDanych){//Wypisanie kazdego budynku, ktory odblokowuje sie na danym poziomie
            if(info.lvlTerr==kolonia.getLvlTerr()){
                cout << MAGENTA << BOLD << left << setw(20) << info.nazwa << RESET << YELLOW << " | " << info.opis << RESET << endl;
            }
        }
        cout<<endl;
}

void Game::prntInfo(string cat){//Wyswietlanie informacji o danej kategorii budynkow
    for (auto &c : cat) c = toupper(c);
    if(!(cat=="ENERGY"||cat=="HOUSING"||cat=="FARM"||cat=="PRODUCER"||cat=="TERR")){
        cout<<RED<<BOLD<<"Nie ma takiej kategorii!!"<<RESET<<endl;
        return;
    }
    prntHeader("SZCZEGOLY KATEGORI: "+cat);
    cout<<YELLOW<<string(120,'-')<<RESET<<endl;
    string sep=" | ";
    const int w_n = 20;
    const int w = 15;
    const int w_op = 30;

    cout << BLUE <<BOLD<< left << setw(w_n) << "NAZWA"<<YELLOW<<NO_BOLD<<sep<<BOLD<<CYAN<< setw(w-2) << "koszt: KAMIEN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<CYAN<< setw(w-3) << "koszt: TYTAN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-3) << "req. WORKERS"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-6) << "req. PRAD";
    if(cat=="ENERGY"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-6) << "gen. PRAD"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }else     
    if(cat=="HOUSING"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-6) << "residents"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }else 
    if(cat=="FARM"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-2) << "gen. jedzenie"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-11) << "TIME"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }else 
    if(cat=="PRODUCER"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-5) << "gen. STONE"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-5) << "gen. TYTAN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }else 
    if(cat=="TERR"){
        cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-6) << "gen. terr"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    }
    cout<<YELLOW<<string(120,'-')<<RESET<<endl;

    for(const auto &[k,info]:bazaDanych){
        if(cat==info.type&&info.lvlTerr<=kolonia.getLvlTerr()){
            cout<<left<<setw(w_n)<<info.nazwa<<YELLOW<<sep<<RESET<<CYAN<<setw(w-2)<<info.kKamien<<YELLOW<<sep<<RESET<<CYAN<<setw(w-3)<<info.kTytan<<YELLOW<<sep<<RESET<<setw(w-3)<<info.workers<<YELLOW<<sep<<RESET<<setw(w-6)<<info.reqEnergy;
            if(cat=="ENERGY"){
                cout<<YELLOW<<sep<<RESET<< setw(w-6) << info.genInne<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }else     
            if(cat=="HOUSING"){
                cout<<YELLOW<<sep<<RESET<< setw(w-6) << info.genInne<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }else
            if(cat=="FARM"){
                cout<<YELLOW<<sep<<RESET<< setw(w-2) << info.genInne<<YELLOW<<sep<<RESET<<setw(w-11)<<info.x<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }else
            if(cat=="PRODUCER"){
                cout<< YELLOW<<sep<<RESET<<setw(w-5) << info.genKamien<<YELLOW<<sep<<RESET<<setw(w-5)<<info.genTytan<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }else
            if(cat=="TERR"){
                cout<< YELLOW<<sep<<RESET<<setw(w-6) << info.genInne<<YELLOW<<sep<<info.opis<<RESET<<endl;
            }
        
        
        }
    }
}

void Game::prntCategories(){//Wyswietlanie wszystkich kategorii budynkow (same kategorie + krotki opis)
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
    cout << BLUE << BOLD << left << setw(w) << "TERR" << RESET << YELLOW << sep << "Inzynieria terraformacyjna. Przystosowuje atmosfere Marsa (odblokowuje nowe technologie)." << RESET << endl;
    cout << endl;

}

void Game::prntRules() {//Wyswietlenie instrukcji
    prntHeader("PODRECZNIK KOLONIZATORA - ZASADY GRY");

    // 1. CEL GRY
    cout << YELLOW << BOLD << "1. CEL GLOWNY: TERRAFORMACJA" << RESET << endl;
    cout << "   Twoim zadaniem jest przystosowanie Marsa do zycia. Buduj budynki w nowej kolonii" << MAGENTA << "TERR" << RESET << "." << endl;
    cout << "   - Zwiekszanie " << MAGENTA << "Wskaznika Terraformacji" << RESET << " odblokowuje nowe technologie (i dostep do tytan!)." << endl;

    // 2. PRZETRWANIE (Warunki porażki)
    cout << YELLOW << BOLD << "2. EKONOMIA ZYCIA (WAZNE!)" << RESET << endl;
    cout << "   - " << GREEN << "JEDZENIE" << RESET << ": Jest pobierane " << BOLD << "automatycznie" << NO_BOLD << " na poczatku kazdej nowej tury, kazdy mieszkaniec potrzebuje 2 jedzenia na ture." << endl;
    cout << "     Jesli magazyn jedzenia bedzie pusty -> " << RED << BOLD << "KOLONIA UMIERA (KONIEC GRY)" << RESET << "." << endl;
    cout << "   - " << YELLOW << "ENERGIA" << RESET << ": Musisz produkowac wiecej pradu niz zuzywasz (albo dokladnie tyle samo)." << endl;
    cout << "     Brak energii = " << RED << "STOP PRODUKCJI" << RESET << " we wszystkich budynkach (farmy tez przestaja dzialac!)." << endl << endl;

    // 3. SYSTEM TUR
    cout << YELLOW << BOLD << "3. MECHANIKA TURY" << RESET << endl;
    cout << "   - W kazdej turze masz do dyspozycji " << BOLD << "3 PUNKTY RUCHU" << NO_BOLD << "." << endl;
    cout << "   - " << CYAN << "1 Budynek = 1 Ruch" << RESET << "." << endl;
    cout << "   - Gdy wykorzystasz ruchy (lub wczesniej), wpisz " << BG_BLACK << "next" << RESET << "." << endl;
    cout << "   - Dopiero po wpisaniu 'next' nastepuje produkcja surowcow i konsumpcja jedzenia." << endl << endl;

    // 4. ZARZADZANIE
    cout << YELLOW << BOLD << "4. BUDOWANIE I NISZCZENIE" << RESET << endl;
    cout << "   - Uzywaj " << BG_BLACK << "info" << RESET << ", aby sprawdzic wymagania i koszt budynkow." << endl;
    cout << "   - Aby zdobyc pracownikow, musisz budowac domy (" << BLUE << "Housing" << RESET << ")." << endl;
    cout << "   - Mozesz zburzyc budynek i odzyksac z niego polowe surowcow, uzywajac komendy " << BG_BLACK << "destroy [nazwa]" << RESET << "." << endl;
    cout <<  BLUE << "Zburzenie zwalnia pracownikow" << RESET<< endl;
    
    cout << endl;
}

void Game::prntHelp(){//Help - inormacje o komendach
    const int w = 30; 
    const string sep = " | ";

    prntHeader("LISTA DOSTEPNYCH KOMEND");

    cout << YELLOW << BOLD << left << setw(w) << "KOMENDA" << sep << "OPIS" << RESET << endl;
    cout << YELLOW << string(w, '-') << "-+-" << string(60, '-') << RESET << endl; 

    //  INFORMACJE O GRZE 
    cout << BG_BLACK << left << setw(w) << "info" << RESET << YELLOW << sep << "Lista dostepnych kategorii budynkow." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "info [kategoria]" << RESET << YELLOW << sep << "Szczegoly budynkow z kategorii (np. info ENERGY)." << RESET << endl;
    
    //  ZARZADZANIE KOLONIA 
    cout << BG_BLACK << left << setw(w) << "colony" << RESET << YELLOW << sep << "Stan surowcow, energii i pracownikow." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "show" << RESET << YELLOW << sep << "Lista wybudowanych przez Ciebie budynkow." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "show [nazwa]" << RESET << YELLOW << sep << "Szczegoly Twojego wybudowanego budynku." << RESET << endl;

    // AKCJE 
    cout << BG_BLACK << left << setw(w) << "build [nazwa]" << RESET << YELLOW << sep << "Buduje budynek (kosztuje 1 ruch)." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "destroy [nazwa]" << RESET << YELLOW << sep << "Niszczy budynek (odzyskuje polowe surowcow oraz wszystkich pracownikow)." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "rename" << RESET << YELLOW << sep << "Zmiana nazwy kolonii." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "next" << RESET << YELLOW << sep << "Zakoncz ture (produkcja/konsumpcja)." << RESET << endl;

    // SYSTEM
    cout << BG_BLACK << left << setw(w) << "save / load" << RESET << YELLOW << sep << "Zapis i wczytanie stanu gry." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "rules" << RESET << YELLOW << sep << "Wyswietla zasady gry." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "cheat" << RESET << YELLOW << sep << "Aktywuje tryb Sandbox (surowce)." << RESET << endl;
    cout << BG_BLACK << left << setw(w) << "exit" << RESET << YELLOW << sep << "Wyjscie z gry." << RESET << endl;

    cout << endl;
}
