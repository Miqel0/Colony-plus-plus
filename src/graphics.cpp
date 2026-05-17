#include "graphics.h"
#include "game.h"
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <optional>
#include <map>
#include <cfloat>


Graphics::Graphics(unsigned int szer_,unsigned int wys_):szer(szer_),wys(wys_),window(sf::VideoMode({szer_, wys_}), "Colony ++"),czyhelp(false),czyBudynki(false),czyBudowanie(false),wybranaKategoriaBudowa(""),czyBudowanieCategory(false),czyBudowanieWyniki(false),ostatniWynik({false, 0,0,0,"",0,false}),czyNextRound(false),czyNextRound1(false),nextWynik({false,false,false,false,"",0,0,0,0}),czyWyburzanie(false),czyWyburzanie1(false),destroyWynik({false,0,"",{0,0}}){}
Graphics::Graphics():screenSize(sf::VideoMode::getDesktopMode()), window(screenSize, "Colony ++",sf::State::Fullscreen),szer(screenSize.size.x),wys(screenSize.size.y),czyhelp(false),czyBudynki(false),czyBudowanie(false),wybranaKategoriaBudowa(""),czyBudowanieCategory(false),czyBudowanieWyniki(false),ostatniWynik({false, 0,0,0,"",0,false}),czyNextRound(false),czyNextRound1(false),nextWynik({false,false,false,false,"",0,0,0,0}),czyWyburzanie(false),czyWyburzanie1(false),destroyWynik({false,0,"",{0,0}}){}

/**
 * @brief Tymczasowe wyświeltanie głównego menu z przyciskami.
 * 
 */
void Graphics::prntMenu(){
    ImGui::SetNextWindowPos(ImVec2(10, 70), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(240, 400), ImGuiCond_Once);
    ImGui::Begin("Panel Sterowania",nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Separator();

    if(ImGui::Button("Pomoc")){
        czyhelp =!czyhelp;
    }
    ImGui::Separator();
    if(ImGui::Button("Zbudowane Budynki")){
        czyBudynki =!czyBudynki;
    }
    ImGui::Separator();
    if(ImGui::Button("Budowanie")){
        czyBudowanie =!czyBudowanie;
    }
    ImGui::Separator();
    ImGui::End();
}

/**
 * @brief 
 * 
 * @param kolonia 
 * @param licznik mapa z iloscia zbudowanych budynkow
 * @param cat nazwa kategorii do wyświetlenia
 * @param bazaDanych 
 */
void Graphics::prntStatystykiToolTop(const Colony& kolonia, map<string,int>& licznik, string cat,const map<string, BuildingInfo>& bazaDanych){
    if (fontDefault != nullptr) ImGui::PushFont(fontDefault);
    ImGui::BeginTooltip(); 
    if(cat=="prad"){
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Prąd");

        struct prad_info{
            string nazwa="";
            int gen=0;
            int dem=0;
            int ilo=0;
        };
        vector <prad_info> gen={};
        vector <prad_info> straty={};
        for(const auto& [nazwa, ilosc] : licznik){
            string nazwa_=nazwa;
            for(auto &c : nazwa_) c = tolower(c);
            const auto& info = bazaDanych.at(nazwa_);
            if(info.type=="ENERGY"){
                prad_info wynik;
                wynik.nazwa=cleanString(info.nazwa);
                wynik.gen=info.genInne;
                wynik.ilo=ilosc;
                gen.push_back(wynik);
            }
            if(info.reqEnergy!=0){
                prad_info wynik;
                wynik.nazwa=cleanString(info.nazwa);
                wynik.dem=info.reqEnergy;
                wynik.ilo=ilosc;
                straty.push_back(wynik);
            }
        }
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "PRODUKCJA: %d",(int)kolonia.getGenEnergy());
        for (const auto& wpis : gen) {
            ImGui::BulletText("%s (x%d): +%d", wpis.nazwa.c_str(), wpis.ilo, wpis.gen);
        }
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "ZAPOTRZEBOWANIE: %d",(int)kolonia.getReqEnergy());
        for (const auto& wpis : straty) {
            ImGui::BulletText("%s (x%d): -%d", wpis.nazwa.c_str(), wpis.ilo, wpis.dem);
        }
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "SUMA: %d",(int)kolonia.getGenEnergy()-(int)kolonia.getReqEnergy());
        

      }
      else if(cat=="ludzie"){
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Ludzie");

        struct ludzie_info{
            string nazwa="";
            int gen=0;
            int dem=0;
            int ilo=0;
        };
        vector <ludzie_info> gen={};
        vector <ludzie_info> straty={};
        for(const auto& [nazwa, ilosc] : licznik){
            string nazwa_=nazwa;
            for(auto &c : nazwa_) c = tolower(c);
            const auto& info = bazaDanych.at(nazwa_);
            if(info.type=="HOUSING"){
                ludzie_info wynik;
                wynik.nazwa=cleanString(info.nazwa);
                wynik.gen=info.genInne;
                wynik.ilo=ilosc;
                gen.push_back(wynik);
            }
            if(info.workers!=0){
                ludzie_info wynik;
                wynik.nazwa=cleanString(info.nazwa);
                wynik.dem=info.workers;
                wynik.ilo=ilosc;
                straty.push_back(wynik);
            }
        }
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "DOSTĘPNI: %d",kolonia.getAllWorkers());
        ImGui::BulletText("Baza: +10");
        for (const auto& wpis : gen) {
            ImGui::BulletText("%s (x%d): +%d", wpis.nazwa.c_str(), wpis.ilo, wpis.gen);
        }
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "ZAPOTRZEBOWANIE: %d",kolonia.getDemandWorkers());
        for (const auto& wpis : straty) {
            ImGui::BulletText("%s (x%d): -%d", wpis.nazwa.c_str(), wpis.ilo, wpis.dem);
        }
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "SUMA: %d",kolonia.getAllWorkers()-kolonia.getDemandWorkers());
      }
      else if(cat=="jedzenie"){
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Jedzenie");

        struct food_info{
            string nazwa="";
            int gen=0;
            int dem=0;
            int ilo=0;
        };
        vector <food_info> gen={};
        vector <food_info> straty={};
        for(const auto& [nazwa, ilosc] : licznik){
            string nazwa_=nazwa;
            for(auto &c : nazwa_) c = tolower(c);
            const auto& info = bazaDanych.at(nazwa_);
            if(info.type=="FARM"){
                food_info wynik;
                wynik.nazwa=cleanString(info.nazwa);
                wynik.gen=info.genInne;
                
                wynik.ilo=ilosc;
                gen.push_back(wynik);
            }
            if(info.type=="HOUSING"){
                food_info wynik;
                wynik.nazwa=cleanString(info.nazwa);
                wynik.dem=2*info.genInne;
                wynik.ilo=ilosc;
                straty.push_back(wynik);
            }
        }
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "PRODUKCJA: %d",0);
        for (const auto& wpis : gen) {
            ImGui::BulletText("%s (x%d): +%d", wpis.nazwa.c_str(), wpis.ilo, wpis.gen);
        }
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "ZAPOTRZEBOWANIE: %d",(int)kolonia.getReqFood());//FIXME wymyslec liczenie srednie na ture
        ImGui::BulletText("Baza: -20");
        for (const auto& wpis : straty) {
            ImGui::BulletText("%s (x%d): -%d", wpis.nazwa.c_str(), wpis.ilo, wpis.dem);
        }
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "SUMA: %d",0-(int)kolonia.getReqFood());
        

      }


    ImGui::EndTooltip(); 
    if (fontDefault != nullptr) ImGui::PopFont();
}


/**
 * @brief Tymczasowe wyświetlanie parametrów kolonii
 * 
 * @param kolonia wskaźnik do kolonii
 */
void Graphics::prntStatystyki(const Colony& kolonia,  const map<string, BuildingInfo>& bazaDanych) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    auto licznik= kolonia.UIprntBuildingsSumm();
    float gruboscPaska = 60.0f; 
    ImGui::SetNextWindowSize(ImVec2(szer, gruboscPaska)); 

    ImGuiWindowFlags flagiHUD = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | 
                                ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Begin("Główny HUD", nullptr, flagiHUD)) {
        if (fontHUD != nullptr) {
            ImGui::PushFont(fontHUD);
        }
        

        float srodekY = (gruboscPaska - ImGui::GetTextLineHeight()) * 0.5f;
        ImGui::SetCursorPosY(srodekY);

        std::string nazwa = (kolonia.getNazwa() == "XX") ? "Kolonia" : kolonia.getNazwa();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f)); 
        ImGui::Text("%s", nazwa.c_str());
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) {
            if (fontDefault != nullptr) ImGui::PushFont(fontDefault);
            ImGui::BeginTooltip(); 
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Nazwa twojej kolonii.");
            ImGui::EndTooltip(); 
            if (fontDefault != nullptr) ImGui::PopFont();
        }
        
        ImGui::SameLine(0.0f, 40.0f);
        
        ImGui::Text("Prad:");
        if (ImGui::IsItemHovered()) {
            prntStatystykiToolTop(kolonia,licznik,"prad",bazaDanych);
        }
        ImGui::SameLine();
        if (kolonia.getReqEnergy() > kolonia.getGenEnergy()) {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%.0f/%.0f", kolonia.getGenEnergy(), kolonia.getReqEnergy()); // Czerwony
        } else {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%.0f/%.0f", kolonia.getGenEnergy(), kolonia.getReqEnergy()); // Zielony
        }


        ImGui::SameLine(0.0f, 25.0f);

        ImGui::Text("Ludzie: %d/%d", kolonia.getDemandWorkers(), kolonia.getAllWorkers());
        if (ImGui::IsItemHovered()) {
            prntStatystykiToolTop(kolonia,licznik,"ludzie",bazaDanych);
        }
        ImGui::SameLine(0.0f, 25.0f);

        ImGui::Text("Jedzenie:");
         if (ImGui::IsItemHovered()) {
            prntStatystykiToolTop(kolonia,licznik,"jedzenie",bazaDanych);
        }
        ImGui::SameLine();
        if (kolonia.getFood() > 2 * kolonia.getReqFood()) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%.0f (-%.0f)", kolonia.getFood(), kolonia.getReqFood()); // Zielony
        } else if (kolonia.getFood() >= kolonia.getReqFood()) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f (-%.0f)", kolonia.getFood(), kolonia.getReqFood()); // Żółty 
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%.0f (-%.0f)", kolonia.getFood(), kolonia.getReqFood()); // Czerwony
        }
        ImGui::SameLine(0.0f, 40.0f); 

        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Kamien: %d", kolonia.getStone());
        ImGui::SameLine(0.0f, 20.0f);
        ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "Tytan: %d", kolonia.getTitan());
        ImGui::SameLine(0.0f, 40.0f);

        ImGui::TextColored(ImVec4(0.9f, 0.3f, 1.0f, 1.0f), "Terraformacja: %d", kolonia.getLvlTerr());
        ImGui::SameLine();
        ImGui::Text("(Do nast: %d)", kolonia.getToNextLvlTerr());

        char tekstTury[128];
        snprintf(tekstTury, sizeof(tekstTury), "Tura: %d | Ruchy: %d/3", kolonia.getTura(), kolonia.getRuch());
        
        float szerokoscTekstu = ImGui::CalcTextSize(tekstTury).x;
        
        ImGui::SameLine(ImGui::GetWindowWidth() - szerokoscTekstu - 20.0f);
        
  
        ImGui::TextColored(ImVec4(0.8f, 0.9f, 1.0f, 1.0f), "%s", tekstTury);
        if (fontHUD != nullptr) {
            ImGui::PopFont();
        }
    }
    ImGui::End();
}

/**
 * @brief Wyświetlanie listy zbudowanych budynków jako podsumowanie ilości danego budynku 
 * 
 * @param kolonia wskaźnik do koloniii
 * @param bazaDanych wskaźnik do bazy danych 
 */
void Graphics::prntBudynki(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra){
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 0.0f), ImVec2(300, FLT_MAX));
    ImGui::Begin("Zbudowane Budynki",&czyBudynki,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    auto licznik= kolonia.UIprntBuildingsSumm();

    if(licznik.empty()){
        ImGui::Text("BRAK ZBUDOWANYCH BUDYNKOW!");

    }else{
        int il =2;
        int i=0;
        if(czyWyburzanie){il=3;}
        if (ImGui::BeginTable("TabelaBudynkow", il, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            
            ImGui::TableSetupColumn("Nazwa Budynku");
            ImGui::TableSetupColumn("Ilość");
            if(czyWyburzanie){ImGui::TableSetupColumn("Akcja");}
            ImGui::TableHeadersRow();
            for (const auto& [nazwa, ilosc] : licznik) {
                if(czyWyburzanie){ImGui::PushID(i);}
                string nazwa_ = cleanString(nazwa);
                ImGui::TableNextRow();
                
                ImGui::TableNextColumn(); 
                ImGui::Text("%s", nazwa_.c_str()); 
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip(); 
                    
                    kolonia.UIprntBuilding(nazwa);
                    prntOpis(nazwa,bazaDanych);

                    ImGui::EndTooltip(); 
                }
                
                ImGui::TableNextColumn();
                ImGui::Text("%d", ilosc);
                if(czyWyburzanie){
                    ImGui::TableNextColumn();

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
            
                    if (ImGui::Button("Zburz", ImVec2(80, 0))) {
                        destroyWynik=gra.UIZburz(nazwa);
                        czyWyburzanie1 = true;
                        czyWyburzanie=true;

                    }
                    ImGui::PopStyleColor(2);
                }

                if(czyWyburzanie){
                    ImGui::PopID();
                    i++;
                }
                
            }
            ImGui::EndTable();
        }
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
        if(ImGui::Button("Wyburzanie!")){
        czyWyburzanie =!czyWyburzanie;
        }
        ImGui::PopStyleColor(2);
    }
    ImGui::End();
}

/**
 * @brief Funckja wyswietlajaca wynik wyburzenia budynku
 * 
 */
void Graphics::prntWyburz() {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
   
    if (destroyWynik.czy) {
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); 
    } else {
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); 
    }

    if (ImGui::Begin("Raport zburzenia", &czyWyburzanie1, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings)) {
        
        if (destroyWynik.czy) {
            
            ImGui::Text("Operacja zakonczona sukcesem!");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f)); 
            
            string nazwa_ = cleanString(destroyWynik.nazwa); 
            ImGui::Text("Budynek '%s' zostal pomyslnie zburzony.", nazwa_.c_str());
            
            // --- SEKCJA ODZYSKANYCH SUROWCÓW ---
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            ImGui::Text("Odzyskano surowce z budowy:");

            if (destroyWynik.sur.first > 0) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f)); 
                ImGui::BulletText("Kamien: %g", destroyWynik.sur.first);
                ImGui::PopStyleColor();
            }
            if (destroyWynik.sur.second > 0) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.7f, 1.0f, 1.0f)); 
                ImGui::BulletText("Tytan: %g", destroyWynik.sur.second);
                ImGui::PopStyleColor();
            }
            
            if (destroyWynik.sur.first <= 0 && destroyWynik.sur.second <= 0) {
                ImGui::BulletText("Brak surowcow do odzyskania.");
            }

            
        } else {
            
            string nazwa_ = cleanString(destroyWynik.nazwa);
            ImGui::Text("Nie mozna zburzyc budynku: %s!", nazwa_.c_str());
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            
            ImGui::Text("Usuniecie tego obiektu spowodowaloby kryzys na rynku pracy.");
            ImGui::Text("Inne pracujace budynki stracilyby obsluge!");
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
            ImGui::Text("Brakowaloby: %d kolonistow do utrzymania pracy!", destroyWynik.brakLudzi);
            ImGui::PopStyleColor();
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f)); 

        // Środek dla przycisku
        float windowWidth = ImGui::GetWindowSize().x;
        float buttonWidth = 100.0f;
        ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f); 
        
        if (ImGui::Button("Zrozumiano", ImVec2(buttonWidth, 0))) {
            czyWyburzanie1 = false;
        }
    }
    ImGui::End();
    
    ImGui::PopStyleColor(); 
}

/**
 * @brief Wyświetlanie listy dostępnych budynków, oraz budowanie budynków
 * 
 * @param kolonia wskaźnik do koloniii
 * @param bazaDanych wskaźnik do bazy danych 
 * @param gra wskaznik do klasy Game
 */
void Graphics::prntBudowanie(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra){
    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 0.0f), ImVec2(300, FLT_MAX));
    
    ImGui::Begin("Dostępne kategorie budynków",&czyBudowanie, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    vector <string> kategorie={"ENERGY","FARM","HOUSING","PRODUCER","TERR"};
    vector <string> opisy = {"Budynki generujące prąd potrzebny w wszystkich innych budynkach", "Budynki generujące jedzenie, które potrzebują mieszkańcy do przeżycia. Niektóre rośliny potrzebują czasu, żeby wyrosnąć.", "Budynki te zapewniają mieszkania dla mieszkańców kolonii.", "Te budynki generują różnego rodzaju surowce potrzebne do budowania budynków.", "Te budynki odpowiadają za zwiększanie terraformacji planety, dzięki nim możesz odblokować nowe budynki."};//FIXME dodac sensowny spsoob opisów tych klas
    for (int i =0;i<kategorie.size();i++) {
                
        ImGui::Separator();
        if(ImGui::Button(kategorie[i].c_str())){
                    wybranaKategoriaBudowa=kategorie[i];
                    czyBudowanieCategory=true;
                }
        if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip(); 
                    
                    prntOpis(kategorie[i],opisy[i]);

                    ImGui::EndTooltip(); 
                }
            }
    ImGui::End();
}

/**
 * @brief Funkcja wyświetlająca wyniki budowania budynku
 * 
 * @param gra 
 */
void Graphics::prntBudowanieWyniki(Game& gra) {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    // Kolor paska u góry okna
    if (ostatniWynik.czy) {
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); 
    } else {
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); 
    }

    if (ImGui::Begin("Raport z budowy", &czyBudowanieWyniki, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings)) {
        
        if (ostatniWynik.czy) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "SUKCES!");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            
            string nazwa_ = cleanString(ostatniWynik.nazwa);
            ImGui::Text("Budynek '%s' zostal pomyslnie zbudowany.", nazwa_.c_str());
            
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            if (ostatniWynik.act_ruch == 1 || ostatniWynik.act_ruch == 2) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                ImGui::Text("Wykorzystano akcje: %d/3 w tej rundzie.", ostatniWynik.act_ruch);
                ImGui::PopStyleColor();
            } 
            else if (ostatniWynik.act_ruch == 3) {
\
                ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.0f, 1.0f), "Wykorzystano limit akcji (3/3)!");
                ImGui::Text("Aby zbudowac kolejny budynek, musisz przejsc do nastepnej rundy.");
            }
            
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "BLAD BUDOWY!");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
            
            if (ostatniWynik.ruch) {
                

                ImGui::Text("Osiagnieto maksymalny limit budowy w tej rundzie!");
                ImGui::Dummy(ImVec2(0.0f, 5.0f));
                
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                ImGui::Text("Wykorzystales juz wszystkie 3 akcje.");
                ImGui::PopStyleColor();
                
                ImGui::Text("Przejdz do nastepnej rundy, aby kontynuowac rozbudowe.");
                
            } else {
                string nazwa_ = cleanString(ostatniWynik.nazwa);
                ImGui::Text("Nie udalo sie rozpoczac budowy obiektu: %s", nazwa_.c_str());
                ImGui::Dummy(ImVec2(0.0f, 5.0f));
                ImGui::Text("Brakuje nastepujacych zasobow do sfinalizowania projektu:");
                
                if (ostatniWynik.kamien > 0) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); 
                    ImGui::BulletText("Kamien: %d szt.", ostatniWynik.kamien);
                    ImGui::PopStyleColor();
                }
                if (ostatniWynik.tytan > 0) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.7f, 1.0f, 1.0f)); 
                    ImGui::BulletText("Tytan: %d szt.", ostatniWynik.tytan);
                    ImGui::PopStyleColor();
                }
                if (ostatniWynik.workers > 0) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); 
                    ImGui::BulletText("Wolni kolonisci (sila robocza): %d", ostatniWynik.workers);
                    ImGui::PopStyleColor();
                }
            }
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));

        float windowWidth = ImGui::GetWindowSize().x;
        float buttonWidth = 100.0f;
        ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f); 
        
        if (ImGui::Button("Zrozumiano", ImVec2(buttonWidth, 0))) {
            czyBudowanieWyniki = false; 
        }
    }
    ImGui::End();
    
    ImGui::PopStyleColor();
}


/**
 * @brief Funkcja wyswietlajaca informacje o dostepnych budynkach do zbudowania z danej kategorii.
 * 
 */
void Graphics::prntBuildCategory(const string& cat, const Colony& kolonia, const map<string, BuildingInfo>& bazaDanych,Game& gra){//Wyswietlanie informacji o danej kategorii budynkow
    string title = "Budynki z kategorii: "+ cat;
    ImGui::SetNextWindowSizeConstraints(ImVec2(800, 0.0f), ImVec2(800, FLT_MAX));
    ImGui::Begin(title.c_str(),&czyBudowanieCategory, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    int il_kolum=6;
    if(cat=="FARM"||cat=="PRODUCER"){
        il_kolum=7;
    }

    if (ImGui::BeginTable("TabelaBudynkow", il_kolum, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        
        ImGui::TableSetupColumn("Nazwa Budynku", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("koszt: KAMIEN");
        ImGui::TableSetupColumn("koszt: TYTAN");
        ImGui::TableSetupColumn("req. WORKERS");
        ImGui::TableSetupColumn("req. PRĄD");
        if(cat=="ENERGY"){
            ImGui::TableSetupColumn("gen. PRAD");
        }else 
        if(cat=="HOUSING"){
            ImGui::TableSetupColumn("mieszkańcy");
        }else 
        if(cat=="FARM"){
            ImGui::TableSetupColumn("gen. jedzenie");
            ImGui::TableSetupColumn("czas");
        }else 
        if(cat=="PRODUCER"){
            ImGui::TableSetupColumn("gen. STONE");
            ImGui::TableSetupColumn("gen. TYTAN");
        }else 
        if(cat=="TERR"){
            ImGui::TableSetupColumn("gen. terr");
        }
        //ImGui::TableSetupColumn("Opis");
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f)); 
        ImGui::TableHeadersRow();
        ImGui::PopStyleColor();
        for (const auto &[k,info]:bazaDanych) {
            if(cat==info.type&&info.lvlTerr<=kolonia.getLvlTerr()){
                string nazwa_ = cleanString(info.nazwa);
                
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); 
                if (ImGui::Selectable(nazwa_.c_str())) {
                    ostatniWynik =gra.UIbuild(info);
                    czyBudowanieWyniki=true;
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip(); 
                    
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kliknięcie spowoduje zbudowanie tego budynku!");
                    ImGui::Separator();
                    ImGui::Text("Zbudowanie kosztuje:\n kamień: %d \n tytan: %d", (int)info.kKamien,(int)info.kTytan);
                    ImGui::Separator();
                    string opis =info.opis;
                    prntOpis(opis);

                    ImGui::EndTooltip(); 
                }
                
                ImGui::TableNextColumn();
                if(info.kKamien==0){
                    ImGui::Text("-"); 
                }else{
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "-%d", (int)info.kKamien); 
                }
                ImGui::TableNextColumn();
                
                if(info.kTytan==0){
                    ImGui::Text("-"); 
                }else{
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "-%d", (int)info.kTytan); 
                }
                ImGui::TableNextColumn();
                if(info.workers==0){
                    ImGui::Text("-"); 
                }else{
                    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%d osób", info.workers); 
                }
                ImGui::TableNextColumn();
                if(info.reqEnergy==0){
                    ImGui::Text("-"); 
                }else{
                    ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "-%d kW", (int)info.reqEnergy);
                }
                ImGui::TableNextColumn();
                if(cat=="ENERGY"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d kW", (int)info.genInne); 
                }else 
                if(cat=="HOUSING"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d miejsc", (int)info.genInne);
                }else 
                if(cat=="FARM"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d jedzenia", (int)info.genInne);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d tur", (int)info.x);
                }else 
                if(cat=="PRODUCER"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d", (int)info.genKamien);
                    ImGui::TableNextColumn();
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d", (int)info.genTytan);
                }else 
                if(cat=="TERR"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d", (int)info.genInne);
                }
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();

    if(!(cat=="ENERGY"||cat=="HOUSING"||cat=="FARM"||cat=="PRODUCER"||cat=="TERR")){
        cout<<RED<<BOLD<<"Nie ma takiej kategorii!!"<<RESET<<endl;
        return;
    }
}

/**
 * @brief Wyświetlanie przycisku przejscia do kolejnej rundy
 * 
 */
void Graphics::prntNextRoundButton(){

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 window_pos = ImVec2(viewport->WorkPos.x + viewport->WorkSize.x - 10, viewport->WorkPos.y + 60);

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(1.0f, 0.0f));

    if (ImGui::Begin("PrzyciskTuryFloating", nullptr,  ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground)) {
        ImGui::SetWindowFontScale(3.0f);
        if (ImGui::ArrowButton("##NextTurn", ImGuiDir_Right)) {
            czyNextRound=true;
        }
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip(); 
                    
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kliknięcie spowoduje przejście do kolejnej rundy!");

            ImGui::EndTooltip(); 
        }
    }
    ImGui::End();
}

/**
 * @brief Funkcja wyświetlająca okno z w
 * 
 * @param kolonia wskaźnik do koloniii
 * @param bazaDanych wskaźnik do bazy danych 
 * @param gra wskaznik do klasy Game
 */
void Graphics::prntCzyNextRound(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra){

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(350, 150)); 
    ImGui::Begin("Koniec Tury", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    std::string pyt = "Czy na pewno chcesz zakonczyc ture?";
    float textWidth = ImGui::CalcTextSize(pyt.c_str()).x;
    ImGui::SetCursorPosX((350.0f - textWidth) * 0.5f); 
    ImGui::Text("%s", pyt.c_str());

    ImGui::Dummy(ImVec2(0.0f, 20.0f)); 
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    float buttonWidth = 100.0f;
    float spaceBetween = 40.0f;
    float buttonsStartX = (350.0f - (2 * buttonWidth + spaceBetween)) * 0.5f;

    ImGui::SetCursorPosX(buttonsStartX);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));        
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));  
    if (ImGui::Button("TAK", ImVec2(buttonWidth, 30))) {
        
        nextWynik=gra.UINextRound();  
        czyNextRound1 = true;
        czyNextRound = false;
    }
    ImGui::PopStyleColor(3); 

    ImGui::SameLine(0, spaceBetween); 

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
    if (ImGui::Button("NIE", ImVec2(buttonWidth, 30))) {
        czyNextRound = false;
    }
    ImGui::PopStyleColor(3);

    ImGui::End();
}


/**
 * @brief Funkcja wyświetlająca okno z wynikiem kolejnej rundy
 * 
 * @param kolonia wskaźnik do koloniii
 * @param bazaDanych wskaźnik do bazy danych 
 * @param gra wskaznik do klasy Game
 */
void Graphics::prntNextRound(const Colony& kolonia, const map<string, BuildingInfo>& bazaDanych, Game& gra) {
    if (nextWynik.czy) {
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.6f, 0.2f, 1.0f)); 
    } else {
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); 
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Raport z kolonii", &czyNextRound1, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

    // Brak jedzenia!
    if (!nextWynik.food) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "KATASTROFA: KOLONIA UMARŁA Z GŁODU!");
        ImGui::Separator();
        ImGui::Text("Zabrakło jedzenia dla Twoich mieszkańców.");
        
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        if (ImGui::Button("Zakończ grę", ImVec2(120, 30))) {
            window.close(); 
        }
    } 
    else {
        
        if (!nextWynik.energy) {
            ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.0f, 1.0f), "OSTRZEŻENIE: BLACKOUT!");
            ImGui::Separator();
            ImGui::TextWrapped("Z powodu braku prądu produkcja stanęła w miejscu! Maszyny nie pracują.");
            ImGui::Dummy(ImVec2(0.0f, 5.0f));
        } else {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "PODSUMOWANIE TURY:");
            ImGui::Separator();
        }

        double zjedzone = kolonia.getReqFood(); 
        double bilansJedzenia = nextWynik.c_food - zjedzone;

        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Gospodarka żywnościowa:");
        
        
        ImGui::Bullet(); ImGui::Text("Wyprodukowano:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%.0f", nextWynik.c_food);
        
        
        ImGui::Bullet(); ImGui::Text("Zjedzono:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "-%.0f", zjedzone);
        
       
        if (bilansJedzenia >= 0) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "  -> Bilans: +%.0f", bilansJedzenia);
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "  -> Bilans: %.0f (Zapasy maleją!)", bilansJedzenia);
        }

        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Wydobycie i nauka:");
        
        
        ImGui::Bullet(); ImGui::Text("Kamień:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "+%.0f", nextWynik.c_stone);
        
       
        ImGui::Bullet(); ImGui::Text("Tytan:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "+%.0f", nextWynik.c_titan);
        
        ImGui::Bullet(); ImGui::Text("Terraformacja:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(0.9f, 0.3f, 1.0f, 1.0f), "+%.0f", nextWynik.c_terr);

        if (nextWynik.terr) {
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "AWANS TERRAFORMACJI!");
            ImGui::Text("Odblokowano nowe technologie.");
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::Separator();
        
        float buttonWidth = 120.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
        if (ImGui::Button("Dalej", ImVec2(buttonWidth, 30))) {
            czyNextRound1 = false; 
        }
    }

    ImGui::End();
    ImGui::PopStyleColor();
}

/**
 * @brief Wyświetlanie helpa i instrukcji
 * 
 */
void Graphics::prntPomoc(){

    ImGui::SetNextWindowSize(ImVec2(600, 450), ImGuiCond_FirstUseEver);
    ImGui::Begin("PODRECZNIK KOLONIZATORA - ZASADY GRY",&czyhelp, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    // ==========================================
    // 1. CEL GRY
    // ==========================================
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "1. CEL GLOWNY: TERRAFORMACJA");
    ImGui::Separator();
    
    ImGui::Bullet(); ImGui::Text("Twoim zadaniem jest przystosowanie Marsa do zycia. Buduj budynki w nowej kolonii");
    ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "TERR.");
    
    ImGui::Bullet(); ImGui::Text("Zwiekszanie");
    ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Wskaznika Terraformacji");
    ImGui::SameLine(); ImGui::Text("odblokowuje nowe technologie (i dostep do tytanu!).");
    
    ImGui::Dummy(ImVec2(0.0f, 10.0f)); 

    // ==========================================
    // 2. PRZETRWANIE
    // ==========================================
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "2. EKONOMIA ZYCIA (WAZNE!)");
    ImGui::Separator();

    ImGui::Bullet(); ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "JEDZENIE:");
    ImGui::SameLine(); ImGui::TextWrapped("Pobierane automatycznie na poczatku kazdej nowej tury. Kazdy mieszkaniec potrzebuje 2 jedzenia na ture.");
    
    // Wcięcie dla kary za brak jedzenia
    ImGui::Indent(); 
    ImGui::Text("Jesli magazyn jedzenia bedzie pusty -> ");
    ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "KOLONIA UMIERA (KONIEC GRY).");
    ImGui::Unindent();

    ImGui::Bullet(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "ENERGIA:");
    ImGui::SameLine(); ImGui::TextWrapped("Musisz produkowac wiecej pradu niz zuzywasz (albo dokladnie tyle samo).");
    
    ImGui::Indent();
    ImGui::Text("Brak energii = ");
    ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "STOP PRODUKCJI");
    ImGui::SameLine(); ImGui::Text("we wszystkich budynkach (farmy tez przestaja dzialac!).");
    ImGui::Unindent();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    // ==========================================
    // 3. SYSTEM TUR
    // ==========================================
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "3. MECHANIKA TURY");
    ImGui::Separator();

    ImGui::Bullet(); ImGui::TextWrapped("W kazdej turze masz do dyspozycji 3 PUNKTY RUCHU.");
    
    ImGui::Bullet(); ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "1 Budynek = 1 Ruch.");
    
    ImGui::Bullet(); ImGui::Text("Gdy wykorzystasz ruchy (lub wczesniej), kliknij");
    ImGui::SameLine(); ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "[next]");
    
    ImGui::Bullet(); ImGui::TextWrapped("Dopiero po przejsciu do nastepnej tury nastepuje produkcja surowcow i konsumpcja jedzenia.");

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    // ==========================================
    // 4. ZARZADZANIE
    // ==========================================
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "4. BUDOWANIE I NISZCZENIE");
    ImGui::Separator();

    ImGui::Bullet(); ImGui::Text("Uzywaj panelu budowania, aby sprawdzic wymagania i koszt budynkow.");
    
    ImGui::Bullet(); ImGui::Text("Aby zdobyc pracownikow, musisz budowac domy (");
    ImGui::SameLine(); ImGui::TextColored(ImVec4(0.2f, 0.5f, 1.0f, 1.0f), "Housing");
    ImGui::SameLine(); ImGui::Text(").");

    ImGui::Bullet(); ImGui::Text("Mozesz zburzyc budynek i odzyskac polowe surowcow.");

    ImGui::Indent();
    ImGui::TextColored(ImVec4(0.2f, 0.5f, 1.0f, 1.0f), "Zburzenie zwalnia pracownikow.");
    ImGui::Unindent();

    ImGui::End();

}

/**
 * @brief Ogólna funkcja, która wyświetla wszystkie rzeczy na ekran, sprawdzajac różne warunki
 * 
 * @param kolonia wskaźnik do kolonii
 * @param bazaDanych wskaźnik do mapy z informacjami o wszystkich budynkach.
 */
void Graphics::prntAll(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra){
    auto cos = ImGui::SFML::Init(window);

    //Ustawianie czcionki!
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    static const ImWchar ranges[] ={0x0020, 0x00FF, 0x0100, 0x017F,0,};//Polskie znaki
    io.Fonts->AddFontFromFileTTF("fonts/ChakraPetch-Regular.ttf", 20.0f,NULL,ranges);
    fontDefault = io.Fonts->AddFontFromFileTTF("fonts/ChakraPetch-Regular.ttf", 20.0f, NULL, ranges);
    fontHUD = io.Fonts->AddFontFromFileTTF("fonts/ChakraPetch-Medium.ttf", 32.0f, NULL, ranges);
    ImGui::SFML::UpdateFontTexture();

    sf::Clock deltaClock;
    
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {

            ImGui::SFML::ProcessEvent(window, *event);
            
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            if (const auto* wcisnietyKlawisz = event->getIf<sf::Event::KeyPressed>()) {
                if (wcisnietyKlawisz->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close(); 
                }
            }
            
        }
        
        ImGui::SFML::Update(window, deltaClock.restart());
        
       
        prntMenu();
        prntStatystyki(kolonia,bazaDanych);
        prntNextRoundButton();

        if(czyhelp){
            prntPomoc();
        }

        if(czyBudynki){
            prntBudynki(kolonia, bazaDanych,gra);
        }

        if(czyBudowanie){
            prntBudowanie(kolonia, bazaDanych,gra);
        }
        if(czyBudowanieCategory){
            prntBuildCategory(wybranaKategoriaBudowa,kolonia,bazaDanych,gra);
        }
        if(czyBudowanieWyniki){
            prntBudowanieWyniki(gra);
        }
        if(czyNextRound){
            prntCzyNextRound(kolonia, bazaDanych,gra);
        }
        if(czyNextRound1){
            prntNextRound(kolonia, bazaDanych,gra);
        }
        if(czyWyburzanie1){
            prntWyburz();
        }
        
        
        
        //updatowanie rzeczy
        
        window.clear();
        //rysowanie elementow (tla)
        // window.draw(siatka);
        // window.draw(menu);
        
        ImGui::SFML::Render(window);
        window.display();
    }
    
    ImGui::SFML::Shutdown();
}
