#include "graphics.h"
#include "game.h"
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <optional>
#include <map>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cfloat>


/**
 * @brief Funckja do wczytywania grafik / testur itp!
 * 
 * @tparam T 
 * @param sciezka sciezka w ""
 * @param obiektSFML jaki obiekt zdefiniowany wczesniej
 * @return true udalo sie wczytac
 * @return false nei udalo sie
 */
template <typename T>
bool WczytajGrafike(const std::string& sciezka, T& obiektSFML) {
    std::ifstream file(sciezka, std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku: " << sciezka << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        return obiektSFML.loadFromMemory(buffer.data(), buffer.size());
    }
    
    std::cerr << "Blad: Nie udalo sie odczytac bajtow z pliku: " << sciezka << std::endl;
    return false;
}


Graphics::Graphics(unsigned int szer_,unsigned int wys_):szer(szer_),wys(wys_),window(sf::VideoMode({szer_, wys_}), "Colony ++"),czyhelp(false),czyBudynki(false),czyBudowanie(false),wybranaKategoriaBudowa(""),czyBudowanieCategory(false),czyBudowanieWyniki(false),czyNextRound(false),czyNextRound1(false),czyWyburzanie(false),czyWyburzanie1(false),ekran(TypEkranu::MAIN_MENU),czyGra(false),czyLoad(false),czySave(false),czyBudowa(false),czasWyswietlaniaBledu(0.0f),czyWyburzanieKlick(false),czyBudynekInfo(false),kafelek("",-1){}
Graphics::Graphics():screenSize(sf::VideoMode::getDesktopMode()), window(screenSize, "Colony ++",sf::State::Fullscreen),szer(screenSize.size.x),wys(screenSize.size.y),czyhelp(false),czyBudynki(false),czyBudowanie(false),wybranaKategoriaBudowa(""),czyBudowanieCategory(false),czyBudowanieWyniki(false),czyNextRound(false),czyNextRound1(false),czyWyburzanie(false),czyWyburzanie1(false),ekran(TypEkranu::MAIN_MENU),czyGra(false),czyLoad(false),czySave(false),czyBudowa(false),czasWyswietlaniaBledu(0.0f),czyWyburzanieKlick(false),czyBudynekInfo(false),kafelek("",-1){}

/**
 * @brief Tymczasowe wyświeltanie głównego menu z przyciskami.
 * 
 */
void Graphics::prntMenu(){
    ImGui::SetNextWindowPos(ImVec2(10, 70), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(240, 400), ImGuiCond_Once);
    ImGui::Begin("Panel Sterowania",nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
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
    
    if(ImGui::Button("Wyburzanie")){
        czyWyburzanieKlick=!czyWyburzanieKlick;
    }
    if(czyWyburzanieKlick){
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "WYBURZANIE");
    }
    ImGui::Separator();
    ImVec2 pozycjaMyszy = ImGui::GetMousePos();
    ImGui::TextWrapped("Pozycja myszy: X = %.0f, Y = %.0f", pozycjaMyszy.x, pozycjaMyszy.y);

    //X=300
    //Y=100
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
void Graphics::prntStatystykiToolTop(const Colony& kolonia, map<string,int>& licznik, string cat, const map<string, BuildingInfo>& bazaDanych) {
    if (fontDefault != nullptr) ImGui::PushFont(fontDefault);
    ImVec2 pozycja = ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y + 12.0f);
    ImGui::SetNextWindowPos(pozycja);
    ImGui::BeginTooltip(); 


    auto CenterTitle = [](const char* text, ImVec4 color) {
        float winWidth = ImGui::GetWindowSize().x;
        float textWidth = ImGui::CalcTextSize(text).x;
        float posX = (winWidth - textWidth) * 0.5f;
        if (posX > 0) ImGui::SetCursorPosX(posX); 
        ImGui::TextColored(color, "%s", text);
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 2.0f));
    };


    struct ResInfo {
        string nazwa;
        int gen;
        int dem;
        int ilo;
    };

    vector<ResInfo> gen;
    vector<ResInfo> straty;
    int sumaDodatkowa = 0; 

    // --- GŁÓWNA LOGIKA ---
    if (cat == "prad") {
        CenterTitle("Energia", ImVec4(1.0f, 0.8f, 0.0f, 1.0f)); 

        for (const auto& [nazwa, ilosc] : licznik) {
            string nazwa_ = nazwa;
            for (auto &c : nazwa_) c = tolower(c);
            
  
            const auto& info = bazaDanych.at(nazwa_); 

            if (info.type == "ENERGY") {
                // BEZPIECZNE PRZYPISYWANIE (rozwiązuje błędy z wektorem)
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = info.genInne * ilosc;
                wynik.dem = 0;
                wynik.ilo = ilosc;
                gen.push_back(wynik);
            }
            if (info.reqEnergy != 0) {
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = 0;
                wynik.dem = info.reqEnergy * ilosc;
                wynik.ilo = ilosc;
                straty.push_back(wynik);
            }
        }

        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "PRODUKCJA: %d", kolonia.getGenEnergy());
        for (const auto& wpis : gen) ImGui::BulletText("%s (x%d): +%d", wpis.nazwa.c_str(), wpis.ilo, wpis.gen);
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "ZAPOTRZEBOWANIE: %d", kolonia.getReqEnergy());
        for (const auto& wpis : straty) ImGui::BulletText("%s (x%d): -%d", wpis.nazwa.c_str(), wpis.ilo, wpis.dem);
        ImGui::Separator();
        int suma = kolonia.getGenEnergy() - kolonia.getReqEnergy();
        ImGui::TextColored(suma >= 0 ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "BILANS: %d", suma);
    }
    
    else if (cat == "ludzie") {
        CenterTitle("Kolonisci", ImVec4(1.0f, 0.6f, 0.2f, 1.0f)); 

        for (const auto& [nazwa, ilosc] : licznik) {
            string nazwa_ = nazwa;
            for (auto &c : nazwa_) c = tolower(c);
            
            const auto& info = bazaDanych.at(nazwa_);

            if (info.type == "HOUSING") {
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = info.genInne * ilosc;
                wynik.dem = 0;
                wynik.ilo = ilosc;
                gen.push_back(wynik);
            }
            if (info.workers != 0) {
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = 0;
                wynik.dem = info.workers * ilosc;
                wynik.ilo = ilosc;
                straty.push_back(wynik);
            }
        }
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "DOSTEPNI: %d", kolonia.getAllWorkers());
        ImGui::BulletText("Baza: +10");
        for (const auto& wpis : gen) ImGui::BulletText("%s (x%d): +%d", wpis.nazwa.c_str(), wpis.ilo, wpis.gen);
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "ZATRUDNIENI: %d", kolonia.getDemandWorkers());
        for (const auto& wpis : straty) ImGui::BulletText("%s (x%d): -%d", wpis.nazwa.c_str(), wpis.ilo, wpis.dem);
        
        ImGui::Separator();
        int suma = kolonia.getAllWorkers() - kolonia.getDemandWorkers();
        ImGui::TextColored(suma >= 0 ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "WOLNI: %d", suma);
    }
    
    else if (cat == "jedzenie") {
        CenterTitle("Zywnosc", ImVec4(0.6f, 1.0f, 0.2f, 1.0f)); 
        int suma=0;
        for (const auto& [nazwa, ilosc] : licznik) {
            string nazwa_ = nazwa;
            for (auto &c : nazwa_) c = tolower(c);
            
            const auto& info = bazaDanych.at(nazwa_);

            if (info.type == "FARM") {
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = info.genInne * ilosc;
                wynik.dem = info.x;
                wynik.ilo = ilosc;
                suma+=(wynik.gen-1+info.x)/info.x;
                gen.push_back(wynik);
            }
            if (info.type == "HOUSING") {
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = 0;
                wynik.dem = 2 * info.genInne * ilosc;
                wynik.ilo = ilosc;
                straty.push_back(wynik);
            }
        }

        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "PRODUKCJA: %d", suma);
        for (const auto& wpis : gen) ImGui::BulletText("%s (x%d): +%d co %d tur", wpis.nazwa.c_str(), wpis.ilo, wpis.gen,wpis.dem);//wpis.dem to co ile tur!
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "ZAPOTRZEBOWANIE: %d", kolonia.getReqFood());
        ImGui::BulletText("Baza: -20");
        for (const auto& wpis : straty) ImGui::BulletText("%s (x%d): -%d", wpis.nazwa.c_str(), wpis.ilo, wpis.dem);
        
        ImGui::Separator();
        int suma_ = suma - kolonia.getReqFood();
        ImGui::TextColored(suma_ >= 0 ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "BILANS: %d", suma_);
    }
    
    else if (cat == "kamien") {
        CenterTitle("Kamien", ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); 

        for (const auto& [nazwa, ilosc] : licznik) {
            string nazwa_ = nazwa;
            for (auto &c : nazwa_) c = tolower(c);
            
            const auto& info = bazaDanych.at(nazwa_);

            if (info.type == "PRODUCER" && info.genKamien != 0) {
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = info.genKamien * ilosc;
                wynik.dem = 0;
                wynik.ilo = ilosc;
                gen.push_back(wynik);
                sumaDodatkowa += wynik.gen;
            }
        }

        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "WYDOBYCIE NA TURE: %d", sumaDodatkowa);
        for (const auto& wpis : gen) ImGui::BulletText("%s (x%d): +%d", wpis.nazwa.c_str(), wpis.ilo, wpis.gen);
    }
    
    else if (cat == "tytan") {
        CenterTitle("Tytan", ImVec4(0.4f, 0.7f, 1.0f, 1.0f)); 

        for (const auto& [nazwa, ilosc] : licznik) {
            string nazwa_ = nazwa;
            for (auto &c : nazwa_) c = tolower(c);
            
            const auto& info = bazaDanych.at(nazwa_);

            if (info.type == "PRODUCER" && info.genTytan != 0) {
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = info.genTytan * ilosc;
                wynik.dem = 0;
                wynik.ilo = ilosc;
                gen.push_back(wynik);
                sumaDodatkowa += wynik.gen;
            }
        }

        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "WYDOBYCIE NA TURE: %d", sumaDodatkowa);
        for (const auto& wpis : gen) ImGui::BulletText("%s (x%d): +%d", wpis.nazwa.c_str(), wpis.ilo, wpis.gen);
    }
    
    else if (cat == "terr") {
        CenterTitle("Terraformacja", ImVec4(0.9f, 0.3f, 1.0f, 1.0f)); 
        ImGui::TextWrapped("Do następnego poziomu brakuje: %d pkt", kolonia.getToNextLvlTerr());

        for (const auto& [nazwa, ilosc] : licznik) {
            string nazwa_ = nazwa;
            for (auto &c : nazwa_) c = tolower(c);
            
            const auto& info = bazaDanych.at(nazwa_);

            if (info.type == "TERR") {
                ResInfo wynik;
                wynik.nazwa = cleanString(info.nazwa);
                wynik.gen = info.genInne * ilosc;
                wynik.dem = 0;
                wynik.ilo = ilosc;
                gen.push_back(wynik);
                sumaDodatkowa += wynik.gen;
            }
        }

        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "PUNKTY NA TURE: %d", sumaDodatkowa);
        for (const auto& wpis : gen) ImGui::BulletText("%s (x%d): +%d", wpis.nazwa.c_str(), wpis.ilo, wpis.gen);
    }

    ImGui::EndTooltip(); 
    if (fontDefault != nullptr) ImGui::PopFont();
}

/**
 * @brief Wyświetlanie parametrów kolonii
 * 
 * @param kolonia wskaźnik do kolonii
 */
void Graphics::prntStatystyki(const Colony& kolonia,  const map<string, BuildingInfo>& bazaDanych) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    auto licznik= kolonia.UIprntBuildingsSumm();
    float gruboscPaska = 60.0f; 
    ImGui::SetNextWindowSize(ImVec2((float)szer, gruboscPaska)); 

    ImGuiWindowFlags flagiHUD = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | 
                                ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Begin("Główny HUD", nullptr, flagiHUD)) {
        if (fontHUD != nullptr) {
            ImGui::PushFont(fontHUD);
        }
        

        float srodekY = (gruboscPaska - ImGui::GetTextLineHeight()) * 0.5f;
        ImGui::SetCursorPosY(srodekY);

         string nazwa = (kolonia.getNazwa() == "XX") ? "Kolonia" : cleanString(kolonia.getNazwa());
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f)); 
        ImGui::Text("%s", nazwa.c_str());
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered()) {
            if (fontDefault != nullptr) ImGui::PushFont(fontDefault);
            ImVec2 pozycja = ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y + 12.0f);
            ImGui::SetNextWindowPos(pozycja);
            ImGui::BeginTooltip(); 
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Nazwa twojej kolonii.");
            ImGui::EndTooltip(); 
            if (fontDefault != nullptr) ImGui::PopFont();
        }
        
        ImGui::SameLine(0.0f, 40.0f);
        
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f),"Prad:");
        if (ImGui::IsItemHovered()) {
            prntStatystykiToolTop(kolonia,licznik,"prad",bazaDanych);
        }
        ImGui::SameLine();
        if (kolonia.getReqEnergy() > kolonia.getGenEnergy()) {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%d/%d", kolonia.getGenEnergy(), kolonia.getReqEnergy()); // Czerwony
        } else {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%d/%d", kolonia.getGenEnergy(), kolonia.getReqEnergy()); // Zielony
        }


        ImGui::SameLine(0.0f, 25.0f);

        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f),"Koloniści: %d/%d", kolonia.getDemandWorkers(), kolonia.getAllWorkers());
        if (ImGui::IsItemHovered()) {
            prntStatystykiToolTop(kolonia,licznik,"ludzie",bazaDanych);
        }
        ImGui::SameLine(0.0f, 25.0f);

        ImGui::TextColored(ImVec4(0.6f, 1.0f, 0.2f, 1.0f),"Jedzenie:");
         if (ImGui::IsItemHovered()) {
            prntStatystykiToolTop(kolonia,licznik,"jedzenie",bazaDanych);
        }
        ImGui::SameLine();

        int suma=0;
        for (const auto& [nazwa, ilosc] : licznik) {
            string nazwa_ = nazwa;
            for (auto &c : nazwa_) c = tolower(c);
            
            const auto& info = bazaDanych.at(nazwa_);
            if (info.type == "FARM") {
                
                int gen = info.genInne * ilosc;
                suma+=gen/info.x;
            }
        }
        suma-=kolonia.getReqFood();
        if (kolonia.getFood() > 2 * kolonia.getReqFood()) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%d (%d)", kolonia.getFood(), suma); // Zielony
        } else if (kolonia.getFood() >= kolonia.getReqFood()) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d (%d)", kolonia.getFood(), suma); // Żółty 
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%d (%d)", kolonia.getFood(), suma); // Czerwony
        }
        ImGui::SameLine(0.0f, 40.0f); 

        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Kamień: %d", kolonia.getStone());
         if (ImGui::IsItemHovered()) {
            prntStatystykiToolTop(kolonia,licznik,"kamien",bazaDanych);
        }
        ImGui::SameLine(0.0f, 40.0f);

        ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "Tytan: %d", kolonia.getTitan());
         if (ImGui::IsItemHovered()) {
            prntStatystykiToolTop(kolonia,licznik,"tytan",bazaDanych);
        }
        ImGui::SameLine(0.0f, 40.0f);

        ImGui::TextColored(ImVec4(0.9f, 0.3f, 1.0f, 1.0f), "Terraformacja: %d", kolonia.getLvlTerr());
        // ImGui::SameLine();
        // ImGui::Text("(Do nast: %d)", kolonia.getToNextLvlTerr());
        if (ImGui::IsItemHovered()) {
           prntStatystykiToolTop(kolonia,licznik,"terr",bazaDanych);
       }

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
 * @brief Wyswietlanie ifnoramcji o kliknietym budynku
 * 
 * @param bazaDanych 
 * @param kolonia 
 * @param inf 
 */
void Graphics::prntBudynekInfo( const map<string, BuildingInfo>& bazaDanych,const Colony &kolonia,pair<string,int> inf){
    ImVec2 pozycjaMyszki = ImGui::GetMousePos();
    ImGui::SetNextWindowPos(pozycjaMyszki, ImGuiCond_Appearing);
    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 0.0f), ImVec2(500, FLT_MAX));
    ImGui::Begin(cleanString(inf.first).c_str(),&czyBudynekInfo,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    kolonia.UIprntBuildingID(inf.second);
    prntOpis(inf.first,bazaDanych);
    ImGui::End();
}


/**
 * @brief Wyświetlanie listy zbudowanych budynków jako podsumowanie ilości danego budynku 
 * 
 * @param kolonia wskaźnik do koloniii
 * @param bazaDanych wskaźnik do bazy danych 
 */
void Graphics::prntBudynki(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra){
    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 0.0f), ImVec2(500, FLT_MAX));
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
                ImGui::PushID(i);
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
                        if(destroyWynik.czy){
                            siatka.wczytajBudynki(kolonia);
                        }
                        czyWyburzanie1 = true;
                        czyWyburzanie=false;

                    }
                    ImGui::PopStyleColor(2);
                }

                
                ImGui::PopID();
                i++;
                
                
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
                ImGui::BulletText("Kamien: %d", destroyWynik.sur.first);
                ImGui::PopStyleColor();
            }
            if (destroyWynik.sur.second > 0) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.7f, 1.0f, 1.0f)); 
                ImGui::BulletText("Tytan: %d", destroyWynik.sur.second);
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
    ImGui::SetNextWindowSizeConstraints(ImVec2(350, 0.0f), ImVec2(300, FLT_MAX));
    
    ImGui::Begin("Dostępne kategorie budynków",&czyBudowanie, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    vector <string> kategorie={"ENERGY","FARM","HOUSING","PRODUCER","TERR"};
    vector <string> opisy = {"Budynki generujące prąd potrzebny w wszystkich innych budynkach", "Budynki generujące jedzenie, które potrzebują mieszkańcy do przeżycia. Niektóre rośliny potrzebują czasu, żeby wyrosnąć.", "Budynki te zapewniają mieszkania dla mieszkańców kolonii.", "Te budynki generują różnego rodzaju surowce potrzebne do budowania budynków.", "Te budynki odpowiadają za zwiększanie terraformacji planety, dzięki nim możesz odblokować nowe budynki."};//FIXME dodac sensowny spsoob opisów tych klas
    for (int i =0;i<kategorie.size();i++) {
                
        ImGui::Separator();
        if(ImGui::Button(kategorie[i].c_str())){
                    wybranaKategoriaBudowa=kategorie[i];
                    czyBudowanieCategory=!czyBudowanieCategory;
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
    czyBudowanieCategory=false;
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
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.2f, 1.0f)); 
        ImGui::TableHeadersRow();
        ImGui::PopStyleColor();
        for (const auto &[k,info]:bazaDanych) {
            if(cat==info.type&&info.lvlTerr<=kolonia.getLvlTerr()){
                string nazwa_ = cleanString(info.nazwa);
                string nazwa=info.nazwa;
                for(auto &c:nazwa) c =tolower(c);
                
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); 
                if (ImGui::Selectable(nazwa_.c_str())) {
                    trzymanyBudynek=bazaDanych.at(nazwa);
                    czyBudowa=true;
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip(); 
                    
                    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Kliknięcie spowoduje zbudowanie tego budynku!");
                    ImGui::Separator();
                    ImGui::Text("Zbudowanie kosztuje:\n kamień: %d \n tytan: %d", info.kKamien,info.kTytan);
                    ImGui::Separator();
                    string opis =info.opis;
                    prntOpis(opis);

                    ImGui::EndTooltip(); 
                }
                
                ImGui::TableNextColumn();
                if(info.kKamien==0){
                    ImGui::Text("-"); 
                }else{
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "-%d", info.kKamien); 
                }
                ImGui::TableNextColumn();
                
                if(info.kTytan==0){
                    ImGui::Text("-"); 
                }else{
                    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "-%d", info.kTytan); 
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
                    ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "-%d kW", info.reqEnergy);
                }
                ImGui::TableNextColumn();
                if(cat=="ENERGY"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d kW", info.genInne); 
                }else 
                if(cat=="HOUSING"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d miejsc", info.genInne);
                }else 
                if(cat=="FARM"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d jedzenia", info.genInne);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d tur", info.x);
                }else 
                if(cat=="PRODUCER"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d", info.genKamien);
                    ImGui::TableNextColumn();
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d", info.genTytan);
                }else 
                if(cat=="TERR"){
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d", info.genInne);
                }
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();

    if(!(cat=="ENERGY"||cat=="HOUSING"||cat=="FARM"||cat=="PRODUCER"||cat=="TERR")){
        cout<<"Nie ma takiej kategorii!!"<<endl;
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
                    
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Kliknięcie spowoduje przejście do kolejnej rundy!");

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
    ImGui::SetNextWindowSize(ImVec2(375, 175)); 
    ImGui::Begin("Koniec Tury", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

     string pyt = "Czy na pewno chcesz zakonczyc ture?";
    float textWidth = ImGui::CalcTextSize(pyt.c_str()).x;
    ImGui::SetCursorPosX((375.0f - textWidth) * 0.5f); 
    ImGui::Text("%s", pyt.c_str());

    ImGui::Dummy(ImVec2(0.0f, 20.0f)); 
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    float buttonWidth = 100.0f;
    float spaceBetween = 40.0f;
    float buttonsStartX = (375.0f - (2 * buttonWidth + spaceBetween)) * 0.5f;

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
    if (nextWynik.czy && nextWynik.energy && nextWynik.food) {
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

        int zjedzone = kolonia.getReqFood(); 
        int bilansJedzenia = nextWynik.c_food - zjedzone;

        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Gospodarka żywnościowa:");
        
        
        ImGui::Bullet(); ImGui::Text("Wyprodukowano:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "+%d", nextWynik.c_food);
        
        
        ImGui::Bullet(); ImGui::Text("Zjedzono:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "-%d", zjedzone);
        
       
        if (bilansJedzenia >= 0) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "  -> Bilans: +%d", bilansJedzenia);
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "  -> Bilans: %d (Zapasy maleją!)", bilansJedzenia);
        }

        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Wydobycie i nauka:");
        
        
        ImGui::Bullet(); ImGui::Text("Kamień:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "+%d", nextWynik.c_stone);
        
       
        ImGui::Bullet(); ImGui::Text("Tytan:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "+%d", nextWynik.c_titan);
        
        ImGui::Bullet(); ImGui::Text("Terraformacja:");
        ImGui::SameLine(); ImGui::TextColored(ImVec4(0.9f, 0.3f, 1.0f, 1.0f), "+%d", nextWynik.c_terr);

        if (nextWynik.terr) {
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            
        
            ImGui::TextColored(ImVec4(0.9f, 0.3f, 1.0f, 1.0f), "AWANS TERRAFORMACJI!");
            ImGui::Separator();
            ImGui::Text("Odblokowano nowe technologie:");
            ImGui::Dummy(ImVec2(0.0f, 5.0f));

            
            vector<BuildingInfo> wynik = gra.UIprntNewLvlTerr();

        
            if (wynik.empty()) {
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Brak nowych schematow na tym poziomie.");
            } else {
                for (const auto& info : wynik) {
                    
                    string czystaNazwa = cleanString(info.nazwa);
                    
                    ImGui::Bullet();
                    ImGui::SameLine();
                    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%s", czystaNazwa.c_str());

                    if (ImGui::IsItemHovered()) {
                        ImGui::BeginTooltip();
                        
                        prntOpis(info.nazwa, bazaDanych); 
                        
                        ImGui::EndTooltip();
                    }
                }
            }
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
 * */
void Graphics::prntPomoc() {
    ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Appearing);
    ImGui::Begin("PODRECZNIK KOLONIZATORA - ZASADY GRY", &czyhelp, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    // ==========================================
    // 1. CEL GRY
    // ==========================================
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "1. CEL GLOWNY: TERRAFORMACJA");
    ImGui::Separator();
    
    ImGui::Bullet(); 
    ImGui::TextWrapped("Twoim zadaniem jest przystosowanie Marsa do zycia. Buduj budynki w nowej kolonii, aby przetrwac.");
    
    ImGui::Bullet(); 
    ImGui::TextWrapped("Budynki z kategorii TERR zwiekszaja Wskaznik Terraformacji. Odblokowuje to nowe technologie oraz daje dostep do tytanu.");
    
    ImGui::Dummy(ImVec2(0.0f, 10.0f)); 

    // ==========================================
    // 2. PRZETRWANIE
    // ==========================================
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "2. EKONOMIA ZYCIA (WAZNE!)");
    ImGui::Separator();

    ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "[ JEDZENIE ]");
    ImGui::Indent(); 
    ImGui::TextWrapped("Pobierane automatycznie na poczatku kazdej nowej tury. Kazdy mieszkaniec potrzebuje 2 jedzenia na ture.");
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "UWAGA: Jesli magazyn jedzenia bedzie pusty -> KOLONIA UMIERA (KONIEC GRY).");
    ImGui::Unindent();

    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[ ENERGIA ]");
    ImGui::Indent();
    ImGui::TextWrapped("Musisz produkowac wiecej pradu niz zuzywasz (albo dokladnie tyle samo).");
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "UWAGA: Brak energii = STOP PRODUKCJI we wszystkich budynkach \n(farmy tez przestaja dzialac!).");
    ImGui::Unindent();

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    // ==========================================
    // 3. SYSTEM TUR
    // ==========================================
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "3. MECHANIKA TURY");
    ImGui::Separator();

    ImGui::Bullet(); 
    ImGui::TextWrapped("W kazdej turze masz do dyspozycji 3 PUNKTY RUCHU (1 Budynek = 1 Ruch).");
    
    ImGui::Bullet(); 
    ImGui::TextWrapped("Gdy wykorzystasz ruchy (lub wczesniej, jesli chcesz), kliknij przycisk [NEXT], aby zakonczyc ture.");
    
    ImGui::Bullet(); 
    ImGui::TextWrapped("Dopiero po przejsciu do nastepnej tury nastepuje produkcja surowcow i konsumpcja jedzenia.");

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    // ==========================================
    // 4. ZARZADZANIE
    // ==========================================
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "4. BUDOWANIE I NISZCZENIE");
    ImGui::Separator();

    ImGui::Bullet(); 
    ImGui::TextWrapped("Uzywaj panelu budowania, aby sprawdzic wymagania i koszt budynkow.");
    
    ImGui::Bullet(); 
    ImGui::TextWrapped("Aby zdobyc pracownikow, musisz budowac domy (Kategoria: Housing).");

    ImGui::Bullet(); 
    ImGui::TextWrapped("Mozesz zburzyc budynek, aby odzyskac polowe surowcow. Zburzenie zwalnia rowniez przypisanych pracownikow.");

    ImGui::End();
}


/**
 * @brief Funkcja przy starcie gry
 * 
 * @param kolonia 
 * @param bazaDanych 
 * @param gra 
 */
void Graphics::UIBegin(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra){
    auto cos = ImGui::SFML::Init(window);

    //Wczytywanie ikonki na pasku
    sf::Image icon;
    WczytajGrafike("assets/ikonka_test.png", icon);
    window.setIcon(icon);

    //Ustawianie czcionki!
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    static const ImWchar ranges[] ={0x0020, 0x00FF, 0x0100, 0x017F,0,};//Polskie znaki
    io.Fonts->AddFontFromFileTTF("assets/fonts/ChakraPetch-Regular.ttf", 25.0f,NULL,ranges);
    fontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/ChakraPetch-Regular.ttf", 25.0f, NULL, ranges);
    fontHUD = io.Fonts->AddFontFromFileTTF("assets/fonts/ChakraPetch-Bold.ttf", 32.0f, NULL, ranges);
    fontMENU = io.Fonts->AddFontFromFileTTF("assets/fonts/ChakraPetch-Bold.ttf", 48.0f, NULL, ranges);
    auto a=ImGui::SFML::UpdateFontTexture();
    siatka.wczytajSiatkaDane(kolonia,bazaDanych);

    //Wczytanie tła
    if (WczytajGrafike("assets/tlo_menu.png", tlo_menu_tekstura)) { 
    tlo_menu_sprite.setTexture(tlo_menu_tekstura, true);
    } else {
    cout << "Nie udalo sie wczytac tla menu!" << endl;
    }
    if (WczytajGrafike("assets/tlo_gra.png", tlo_gra_tekstura)) { 
    tlo_gra_sprite.setTexture(tlo_gra_tekstura, true);
    } else {
    cout << "Nie udalo sie wczytac gra menu!" << endl;
    }
    prntAll(kolonia,bazaDanych,gra);
}

/**
 * @brief Funckja wyświetlająca ustawienia
 * */
void Graphics::prntUstawienia(Game& gra){
    sf::Clock deltaClock;
    static char nazwaNowejKolonii[64] = ""; 
    while (const  optional<sf::Event> event = window.pollEvent()) {
        
        ImGui::SFML::ProcessEvent(window, *event);
        
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        if (const auto* wcisnietyKlawisz = event->getIf<sf::Event::KeyPressed>()) {
            if (wcisnietyKlawisz->scancode == sf::Keyboard::Scancode::Escape) {
                if(czyGra){
                    ekran=TypEkranu::GAME;
                }else{
                    ekran=TypEkranu::MAIN_MENU;
                }
                nazwaNowejKolonii[0] = '\0';
            }
        }
        
    } 
    ImGui::SFML::Update(window, deltaClock.restart());
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    float w = 400; 
    float h = 600;
    ImGui::SetNextWindowSize(ImVec2(w,h)); 
    
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::Begin("USTAWIENIA", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    
    if (fontMENU != nullptr) {
        ImGui::PushFont(fontMENU);
    }
    
    
   
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    float text_width = ImGui::CalcTextSize("USTAWIENIA").x;
    ImGui::SetCursorPosX((w - text_width) * 0.5f); 
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f),"USTAWIENIA");
    
    if(czyGra){
        if (fontMENU != nullptr) {
            ImGui::PopFont();
        }
        if (fontDefault != nullptr) {
            ImGui::PushFont(fontDefault);
        }
        ImGui::Separator();
        ImGui::Text("Zmień nazwę kolonii:");
        ImGui::SetNextItemWidth(w - 150.0f); 
        ImGui::InputText("##NowaNazwaKoloniii", nazwaNowejKolonii, sizeof(nazwaNowejKolonii));
        
        ImGui::SameLine();

        if (ImGui::Button("ZMIEŃ", ImVec2(125, 0))) {
            if (strlen(nazwaNowejKolonii) > 0) {
                gra.setNazwa(nazwaNowejKolonii);
                nazwaNowejKolonii[0] = '\0';
            }
        }
        if (fontDefault != nullptr) {
            ImGui::PopFont();
        }
        if (fontMENU != nullptr) {
        ImGui::PushFont(fontMENU);
    }
    }
    ImGui::Separator();
    
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    

   
    float btn_w = 320;
    float btn_h = 75;
    ImGui::SetCursorPos(ImVec2((w - btn_w) * 0.5f, h - 120)); 

    if (ImGui::Button("WRÓĆ", ImVec2(btn_w, btn_h))) {
        if(czyGra){
            ekran =TypEkranu::GAME; 
        }else{
            ekran =TypEkranu::MAIN_MENU; 
        }
        nazwaNowejKolonii[0] = '\0';
    }
    
    
    if (fontMENU != nullptr) {
            ImGui::PopFont();
        }
    ImGui::End();
    ImGui::PopStyleVar(); // Koniec zaokrąglania
}

/**
 * @brief Funckja wyświetlająca credits
 * */
void Graphics::prntCredits(){
    sf::Clock deltaClock;
    while (const  optional<sf::Event> event = window.pollEvent()) {
        
        ImGui::SFML::ProcessEvent(window, *event);
        
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        if (const auto* wcisnietyKlawisz = event->getIf<sf::Event::KeyPressed>()) {
            if (wcisnietyKlawisz->scancode == sf::Keyboard::Scancode::Escape) {
                ekran=TypEkranu::MAIN_MENU;
            }
        }
    } 
    ImGui::SFML::Update(window, deltaClock.restart());
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    float w = 400;
    float h = 600;
    ImGui::SetNextWindowSize(ImVec2(w,h)); 
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::Begin("CREDITS", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    if (fontHUD != nullptr) {
        ImGui::PushFont(fontMENU);
    }
    
   
    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    float text_width = ImGui::CalcTextSize("CREDITS").x;
    ImGui::SetCursorPosX((w - text_width) * 0.5f); 
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f),"CREDITS");
    
    
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Separator();

    float btn_w = 320;
    float btn_h = 75;
    ImGui::SetCursorPos(ImVec2((w - btn_w) * 0.5f, h - 120));
    if (ImGui::Button("WRÓĆ", ImVec2(btn_w, btn_h))) {
        ekran =TypEkranu::MAIN_MENU; 
    }
    
    if (fontMENU != nullptr) {
        ImGui::PopFont();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}


void Graphics::prntBladBudowanie(){
    if (czasWyswietlaniaBledu > 0.0f) {
    czasWyswietlaniaBledu -= zegarBledu.restart().asSeconds();

    ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Always, ImVec2(0.0f, 1.0f));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.1f, 0.1f, 0.9f)); // Ciemnoczerwone tło
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));     // Biały tekst
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));   // Jasnoczerwona ramka

    ImGuiWindowFlags flagi = ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
    
    ImGui::Begin("BladBudowyTooltip", nullptr, flagi);
    
    if(czyBudowa){
        ImGui::Text("Ten kafelek jest zajety!");
    }else if(czyWyburzanieKlick){
        ImGui::Text("Ten kafelek jest pusty!");
    }else{
        ImGui::Text("Ten kafelek jest pusty!");
    }
    ImGui::End();

    ImGui::PopStyleColor(3);
    } 
    else {
        zegarBledu.restart(); 
    }
}


/**
 * @brief Funckja wyświetlająca load - pliki do wczytania i zapisywania
 * @param gra Referencja do obiektu gry 
 * */
void Graphics::prntLoad(Game& gra,const Colony &kolonia){
    sf::Clock deltaClock;
    static char nazwaNowegoZapisu[64] = ""; 
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        
        ImGui::SFML::ProcessEvent(window, *event);
        
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        if (const auto* wcisnietyKlawisz = event->getIf<sf::Event::KeyPressed>()) {
            if (wcisnietyKlawisz->scancode == sf::Keyboard::Scancode::Escape) {
                ekran=TypEkranu::MENU_GAME;
                nazwaNowegoZapisu[0] = '\0';
            }
        }
    } 
    ImGui::SFML::Update(window, deltaClock.restart());
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    float w = 500; 
    float h = 800;
    ImGui::SetNextWindowSize(ImVec2(w,h)); 
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::Begin("ZAPISY GRY", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    if (fontHUD != nullptr) {
        ImGui::PushFont(fontHUD);
    }
   
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    float text_width = ImGui::CalcTextSize("ZAPISY GRY").x;
    ImGui::SetCursorPosX((w - text_width) * 0.5f); 
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f),"ZAPISY GRY");
    
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Utwórz nowy zapis:");
    
    
    ImGui::SetNextItemWidth(w - 150.0f); 
    ImGui::InputText("##NowyZapis", nazwaNowegoZapisu, sizeof(nazwaNowegoZapisu));
    
    ImGui::SameLine();
    
    if (ImGui::Button("ZAPISZ", ImVec2(100, 0))) {
        if (strlen(nazwaNowegoZapisu) > 0) {
            gra.save(std::string(nazwaNowegoZapisu));
            gra.setOstatniZapis(std::string(nazwaNowegoZapisu));
            gra.setZapisy();
            nazwaNowegoZapisu[0] = '\0';
        }
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Dostępne zapisy:");
    
    std::vector<std::string> dostepneZapisy = gra.getZapisy();

    if (ImGui::BeginTable("TabelaZapisow", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, h - 320))) {
        
        ImGui::TableSetupColumn("Nazwa", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Wczytaj", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Zapisz", ImGuiTableColumnFlags_WidthFixed, 90.0f);
        ImGui::TableHeadersRow();

        for (const auto& zapis : dostepneZapisy) {
            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();
            ImGui::Text("%s", zapis.c_str());

            ImGui::TableNextColumn();
            std::string idWczytaj = "Wczytaj##" + zapis; 
            if (ImGui::Button(idWczytaj.c_str(), ImVec2(100, 0))) {
                gra.load(zapis);
                gra.setOstatniZapis(zapis);
                gra.setZapisy();
                siatka.wczytajBudynki(kolonia);
                nazwaNowegoZapisu[0] = '\0';
                ekran = TypEkranu::GAME; 
            }

            ImGui::TableNextColumn();
            std::string idZapisz = "Zapisz##" + zapis; 
            if (ImGui::Button(idZapisz.c_str(), ImVec2(90, 0))) {
                gra.save(zapis);
                gra.setOstatniZapis(zapis);
            }
        }
        ImGui::EndTable();
    }

    float btn_w = 320;
    float btn_h = 60;
    ImGui::SetCursorPos(ImVec2((w - btn_w) * 0.5f, h - 80));
    if (ImGui::Button("WRÓĆ", ImVec2(btn_w, btn_h))) {
        nazwaNowegoZapisu[0] = '\0';
        ekran = TypEkranu::MENU_GAME; 
    }
    
    if (fontHUD != nullptr) {
        ImGui::PopFont();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}


/**
 * @brief Funkcja wyświetlająca menu gry 
 * * @param gra 
 */
void Graphics::prntMenuGra(Game& gra) {
    sf::Clock deltaClock; 
    while (const  optional<sf::Event> event = window.pollEvent()) {
        
        ImGui::SFML::ProcessEvent(window, *event);
        
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        if (const auto* wcisnietyKlawisz = event->getIf<sf::Event::KeyPressed>()) {
            if (wcisnietyKlawisz->scancode == sf::Keyboard::Scancode::Escape) { 
                ekran=TypEkranu::GAME;
            }
        }
    }
    ImGui::SFML::Update(window, deltaClock.restart());
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    float w = 320;
    float h = 450;
    ImGui::SetNextWindowSize(ImVec2(w,h)); 
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::Begin("Menu Gry", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    if (fontMENU != nullptr) {
        ImGui::PushFont(fontMENU);
    }
    
    ImGui::Dummy(ImVec2(0.0f, 15.0f));
    float text_width = ImGui::CalcTextSize("PAUZA").x;
    ImGui::SetCursorPosX((w - text_width) * 0.5f); 
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f),"PAUZA");
    if (fontMENU != nullptr) {
        ImGui::PopFont();
    }

     if (fontHUD != nullptr) {
        ImGui::PushFont(fontHUD);
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::Separator();
    
    float btn_w = 260;
    float btn_h = 60;
    float btn_x = (w - btn_w) * 0.5f;


    ImGui::SetCursorPos(ImVec2(btn_x, 100));
    if (ImGui::Button("ZAPISY GRY", ImVec2(btn_w, btn_h))) {
        //gra.load("basic"); 
        ekran =TypEkranu::LOAD; 
    }

    ImGui::SetCursorPos(ImVec2(btn_x, 180));
    if (ImGui::Button("USTAWIENIA", ImVec2(btn_w, btn_h))) {
        ekran =TypEkranu::SETTINGS; 
    }

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
    ImGui::SetCursorPos(ImVec2(btn_x, 260));
    if (ImGui::Button("WYJDŹ Z GRY", ImVec2(btn_w, btn_h))) {
        window.close();
    }
    
    ImGui::SetCursorPos(ImVec2(btn_x, 340));
    if (ImGui::Button("WYJDŹ DO MENU", ImVec2(btn_w, btn_h))) {
        ekran=TypEkranu::MAIN_MENU;
        czyGra=false;
    }
    ImGui::PopStyleColor(3);

    if (fontHUD != nullptr) {
        ImGui::PopFont();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

/**
 * @brief Funkcja wyświetlająca menu główne 
 * * @param gra 
 */
void Graphics::prntMenuGlowne(Game& gra,const Colony &kolonia) {
    sf::Clock deltaClock; 
    while (const  optional<sf::Event> event = window.pollEvent()) {
        
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
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    center.x-=425;
    center.y+=100;
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    
    float w = 450;
    float h = 680;
    ImGui::SetNextWindowSize(ImVec2(w,h)); 
    
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f); 
    ImGui::Begin("Menu Główne", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    if (fontMENU != nullptr) {
        ImGui::PushFont(fontMENU);
    }
    
    // ImGui::Dummy(ImVec2(0.0f, 20.0f));
    // float text_width = ImGui::CalcTextSize("COLONY ++").x;
    // ImGui::SetCursorPosX((w - text_width) * 0.5f); 
    // ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f),"COLONY ++");
    
    // ImGui::Dummy(ImVec2(0.0f, 15.0f));
    ImGui::Separator();

    float btn_w = 380;
    float btn_h = 80;
    float btn_x = (w - btn_w) * 0.5f;

    ImGui::SetCursorPos(ImVec2(btn_x, 40));
    if (ImGui::Button("Kontynuuj GRĘ", ImVec2(btn_w, btn_h))) {
        gra.load(gra.getOstatniZapis());
        siatka.wczytajBudynki(kolonia);
        ekran =TypEkranu::GAME; 
        czyGra=true;
    }

    ImGui::SetCursorPos(ImVec2(btn_x, 140));
    if (ImGui::Button("NOWA GRA", ImVec2(btn_w, btn_h))) {
        ekran =TypEkranu::GAME;
        czyGra=true;
    }

    ImGui::SetCursorPos(ImVec2(btn_x, 240));
    if (ImGui::Button("USTAWIENIA", ImVec2(btn_w, btn_h))) {
        ekran =TypEkranu::SETTINGS; 
    }

    ImGui::SetCursorPos(ImVec2(btn_x, 340));
    if (ImGui::Button("CREDITS", ImVec2(btn_w, btn_h))) {
        ekran =TypEkranu::CREDITS; 
    }
    
    ImGui::SetCursorPos(ImVec2(btn_x, 540));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); 
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
    if (ImGui::Button("WYJDŹ", ImVec2(btn_w, btn_h))) {
        window.close();
    }
    ImGui::PopStyleColor(3);

    if (fontMENU != nullptr) {
        ImGui::PopFont();
    }
    ImGui::End();
    ImGui::PopStyleVar();

}

/**
 * @brief WYswietlanie gry okien itp
 * 
 * @param kolonia 
 * @param bazaDanych 
 * @param gra 
 */
void Graphics::prntGRA(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra){
    sf::Clock deltaClock;        
    while (const  optional<sf::Event> event = window.pollEvent()) {

        ImGui::SFML::ProcessEvent(window, *event);
            
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
            
        if (const auto* wcisnietyKlawisz = event->getIf<sf::Event::KeyPressed>()) {
            if (wcisnietyKlawisz->scancode == sf::Keyboard::Scancode::Escape) {
                ekran=TypEkranu::MENU_GAME;
            }
        }
            
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
            if (czyBudowa) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    pair<int,int> kliknietePole = siatka.getPozMysz();
                    
                    if (kliknietePole.first != -1 && kliknietePole.second != -1) {
                        if(siatka.getTypKafelka()==TypKafelka::ZAJETY){
                            czasWyswietlaniaBledu = 1.0f; 
                            zegarBledu.restart();
                        }else{
                            
                            ostatniWynik=gra.UIbuild(trzymanyBudynek,kliknietePole);
                            if(ostatniWynik.czy){siatka.wczytajBudynki(kolonia);}
                            czyBudowa = false;
                            czyBudowanieWyniki=true;
                        }
                    }else{
                        czyBudowa = false;
                    }
                }
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    czyBudowa = false;
                }
            }
            if (czyWyburzanieKlick) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    pair<int,int> kliknietePole = siatka.getPozMysz();
                    
                    if (kliknietePole.first != -1 && kliknietePole.second != -1) {
                        if(siatka.getTypKafelka()==TypKafelka::PUSTY){
                            czasWyswietlaniaBledu = 1.0f; 
                            zegarBledu.restart();
                        }else{
                            
                            destroyWynik=gra.UIZburzID(siatka.getID(kliknietePole));
                            if(destroyWynik.czy){
                                siatka.wczytajBudynki(kolonia);
                            }
                            czyWyburzanie1 = true;
                            czyWyburzanieKlick=false;
                            
                        }
                    }else{
                        czyWyburzanieKlick = false;
                    }
                }
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    czyWyburzanieKlick = false;
                }
            }
            else{
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    pair<int,int> kliknietePole = siatka.getPozMysz();
                    
                    if (kliknietePole.first != -1 && kliknietePole.second != -1) {
                        TypKafelka typ=siatka.getTypKafelka();
                        if(typ==TypKafelka::PUSTY){
                            czasWyswietlaniaBledu = 1.0f;
                            zegarBledu.restart();
                        }else{
                            string nazwa__=siatka.getName(kliknietePole);
                            for (auto &c:nazwa__) c=tolower(c);
                            kafelek={nazwa__,siatka.getID(kliknietePole)};
                            czyBudynekInfo=true;
                        }
                    }else{
                        
                    }
                }

                    
            }
            
        }
    }

        }
        
        ImGui::SFML::Update(window, deltaClock.restart());
        
       
        prntMenu();
        prntStatystyki(kolonia,bazaDanych);
        prntNextRoundButton();
        if(!czyBudowa&&(!czyWyburzanieKlick)){
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
            if(czyBudynekInfo){
                prntBudynekInfo(bazaDanych,kolonia,kafelek);
            }
        }
        else{
            prntBladBudowanie();
        }
}

/**
 * @brief Ogólna funkcja, która wyświetla wszystkie rzeczy na ekran, sprawdzajac różne warunki
 * 
 * @param kolonia wskaźnik do kolonii
 * @param bazaDanych wskaźnik do mapy z informacjami o wszystkich budynkach.
 */
void Graphics::prntAll(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych, Game& gra){
    
    while (window.isOpen()) {

        if(ekran==TypEkranu::MAIN_MENU){
            prntMenuGlowne(gra,kolonia);
        }
        else if(ekran==TypEkranu::GAME){
            prntGRA(kolonia, bazaDanych,gra);
        }
        else if(ekran==TypEkranu::MENU_GAME){
            prntMenuGra(gra);
        }
        else if(ekran==TypEkranu::SETTINGS){
            prntUstawienia(gra);
        }
        else if(ekran==TypEkranu::CREDITS){
            prntCredits();
        }
        else if(ekran==TypEkranu::LOAD){
            prntLoad(gra,kolonia);
        }

        window.clear();
        
        if(ekran==TypEkranu::MAIN_MENU){ window.draw(tlo_menu_sprite);}
        if(ekran==TypEkranu::GAME){ window.draw(tlo_gra_sprite);}
        //rysowanie elementow (tla)
        // window.draw(siatka);
        // window.draw(menu);
        ImVec2 pozycjaMyszy = ImGui::GetMousePos();
        if(ekran==TypEkranu::GAME){

            siatka.prntSiatka(window,pozycjaMyszy,bazaDanych,czyBudowa,trzymanyBudynek.nazwa);
        }
        
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}




bool Graphics::getCzyBudowa(){return czyBudowa;}
BuildingInfo Graphics::getTrzymanyBudynek(){return trzymanyBudynek;}