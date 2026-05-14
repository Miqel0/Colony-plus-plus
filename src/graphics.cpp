#include "graphics.h"
#include "game.h"
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <optional>
#include <map>

Graphics::Graphics(unsigned int szer_,unsigned int wys_):szer(szer_),wys(wys_),window(sf::VideoMode({szer_, wys_}), "Colony ++"),czyhelp(false),czyBudynki(false),czyBudowanie(false),wybranaKategoriaBudowa(""),czyBudowanieCategory(false){}
Graphics::Graphics():screenSize(sf::VideoMode::getDesktopMode()), window(screenSize, "Colony ++",sf::State::Fullscreen),szer(screenSize.size.x),wys(screenSize.size.y),czyhelp(false),czyBudynki(false),czyBudowanie(false),wybranaKategoriaBudowa(""),czyBudowanieCategory(false){}

/**
 * @brief Tymczasowe wyświeltanie głównego menu z przyciskami.
 * 
 */
void Graphics::prntMenu(){
    ImGui::SetNextWindowPos(ImVec2(10, 60), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(240, 800), ImGuiCond_Once);
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
 * @brief Tymczasowe wyświetlanie parametrów kolonii
 * 
 * @param kolonia wskaźnik do kolonii
 */
void Graphics::prntStatystyki(const Colony& kolonia){
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::SetNextWindowSize(ImVec2(szer, 35.0)); 

    ImGuiWindowFlags flagiHUD = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | 
                                ImGuiWindowFlags_NoSavedSettings;

    if (ImGui::Begin("Główny HUD", nullptr, flagiHUD)) {
        
        // 1. Nazwa i tura 
        std::string nazwa = (kolonia.getNazwa() == "XX") ? "Kolonia" : kolonia.getNazwa();
        ImGui::Text("%s | Tura: %d (%d/3)", nazwa.c_str(), kolonia.getTura(), kolonia.getRuch());
        ImGui::SameLine(0.0f, 20.0f); 
        
        // 2. Energia (format %.0f)
        ImGui::Text("Prad:");
        ImGui::SameLine();
        if (kolonia.getReqEnergy() > kolonia.getGenEnergy()) {
            // Czerwony 
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.0f/%.0f", kolonia.getGenEnergy(), kolonia.getReqEnergy());
        } else {
            // Zielony
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%.0f/%.0f", kolonia.getGenEnergy(), kolonia.getReqEnergy());
        }
        ImGui::SameLine(0.0f, 20.0f);

        // 3. Pracownicy 
        ImGui::Text("Ludzie: %d/%d", kolonia.getDemandWorkers(), kolonia.getAllWorkers());
        ImGui::SameLine(0.0f, 20.0f);

        // 4. Jedzenie 
        ImGui::Text("Jedzenie:");
        ImGui::SameLine();
        if (kolonia.getFood() > 2 * kolonia.getReqFood()) {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%.0f (-%.0f)", kolonia.getFood(), kolonia.getReqFood()); // Zielony
        } else if (kolonia.getFood() >= kolonia.getReqFood()) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.0f (-%.0f)", kolonia.getFood(), kolonia.getReqFood()); // Żółty 
        } else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%.0f (-%.0f)", kolonia.getFood(), kolonia.getReqFood()); // Czerwony
        }
        ImGui::SameLine(0.0f, 20.0f);

        // 5. Surowce stałe 
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Kamien: %d", kolonia.getStone());
        ImGui::SameLine(0.0f, 15.0f);
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.9f, 1.0f), "Tytan: %d", kolonia.getTitan());
        ImGui::SameLine(0.0f, 20.0f);

        // 6. Terraformacja 
        ImGui::Text("Poziom Terraformacji: %d (Do nast: %d)", kolonia.getLvlTerr(), kolonia.getToNextLvlTerr());

    }
    ImGui::End();
}

/**
 * @brief Wyświetlanie listy zbudowanych budynków jako podsumowanie ilości danego budynku 
 * 
 * @param kolonia wskaźnik do koloniii
 * @param bazaDanych wskaźnik do bazy danych 
 */
void Graphics::prntBudynki(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych){
    ImGui::SetNextWindowSize(ImVec2(400, 600)); 
    ImGui::Begin("Zbudowane Budynki",&czyBudynki, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    auto licznik= kolonia.UIprntBuildingsSumm();

    if(licznik.empty()){
        ImGui::Text("BRAK ZBUDOWANYCH BUDYNKOW!");

    }else{

        if (ImGui::BeginTable("TabelaBudynkow", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            
            ImGui::TableSetupColumn("Nazwa Budynku");
            ImGui::TableSetupColumn("Ilość");
            ImGui::TableHeadersRow();
            for (const auto& [nazwa, ilosc] : licznik) {
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
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

/**
 * @brief Wyświetlanie listy dostępnych budynków, oraz budowanie budynków
 * 
 * @param kolonia wskaźnik do koloniii
 * @param bazaDanych wskaźnik do bazy danych 
 * @param gra wskaznik do klasy Game
 */
void Graphics::prntBudowanie(const Colony& kolonia,const map<string, BuildingInfo>& bazaDanych,Game& gra){
    
    ImGui::SetNextWindowSize(ImVec2(400, 200)); 
    ImGui::Begin("Dostępne kategorie budynków",&czyBudowanie, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
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
        ImGui::Text("%s",wybranaKategoriaBudowa.c_str());
    ImGui::End();
}

/**
 * @brief Funkcja wyswietlajaca informacje o dostepnych budynkach do zbudowania z danej kategorii.
 * 
 */
void Graphics::prntBuildCategory(const string& cat, const Colony& kolonia, const map<string, BuildingInfo>& bazaDanych,Game& gra){//Wyswietlanie informacji o danej kategorii budynkow
    string title = "Budynki z kategorii: "+ cat;
    ImGui::SetNextWindowSize(ImVec2(800, 200)); 
    ImGui::Begin(title.c_str(),&czyBudowanieCategory, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    
    auto licznik= kolonia.UIprntBuildingsSumm();

    if(licznik.empty()){
        ImGui::Text("BRAK ZBUDOWANYCH BUDYNKOW!");

    }else{
        int il_kolum=6;
        if(cat=="FARM"||cat=="PRODUCER"){
            il_kolum=7;
        }

        if (ImGui::BeginTable("TabelaBudynkow", il_kolum, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            
            ImGui::TableSetupColumn("Nazwa Budynku");
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
            ImGui::TableHeadersRow();

            for (const auto &[k,info]:bazaDanych) {
                if(cat==info.type&&info.lvlTerr<=kolonia.getLvlTerr()){
                    string nazwa_ = cleanString(info.nazwa);
                    
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn(); 
                    if (ImGui::Selectable(nazwa_.c_str())) {
                        // Akcja po kliknięciu!
                        //FIXME Budowanie!!!!  do dodania
                        gra.build(info);
                    }
                    if (ImGui::IsItemHovered()) {
                        ImGui::BeginTooltip(); 
                        
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Kliknięcie spowoduje zbudowanie tego budynku!");
                        ImGui::Separator();
                        ImGui::Text("Zbudowanie kosztuje:\n kamień: %d \n tytan: %d", info.kKamien,info.kTytan);
                        ImGui::Separator();
                        string opis =info.opis;
                        prntOpis(opis);

                        ImGui::EndTooltip(); 
                    }
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", info.kKamien);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", info.kTytan);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", info.workers);
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", info.reqEnergy);
                    ImGui::TableNextColumn();

                    if(cat=="ENERGY"){
                        ImGui::Text("%d", info.genInne);
                    }else 
                    if(cat=="HOUSING"){
                        ImGui::Text("%d", info.genInne);
                    }else 
                    if(cat=="FARM"){
                        ImGui::Text("%d", info.genInne);
                        ImGui::TableNextColumn();
                        ImGui::Text("%d", info.x);
                    }else 
                    if(cat=="PRODUCER"){
                        ImGui::Text("%d", info.genKamien);
                        ImGui::TableNextColumn();
                        ImGui::Text("%d", info.genTytan);
                    }else 
                    if(cat=="TERR"){
                        ImGui::Text("%d", info.genInne);
                    }
                }
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();

    if(!(cat=="ENERGY"||cat=="HOUSING"||cat=="FARM"||cat=="PRODUCER"||cat=="TERR")){
        cout<<RED<<BOLD<<"Nie ma takiej kategorii!!"<<RESET<<endl;
        return;
    }
    //prntHeader("SZCZEGOLY KATEGORI: "+cat);
    //cout<<YELLOW<<string(120,'-')<<RESET<<endl;
    // string sep=" | ";
    // const int w_n = 20;
    // const int w = 15;
    // const int w_op = 30;

    // cout << BLUE <<BOLD<< left << setw(w_n) << "NAZWA"<<YELLOW<<NO_BOLD<<sep<<BOLD<<CYAN<< setw(w-2) << "koszt: KAMIEN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<CYAN<< setw(w-3) << "koszt: TYTAN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-3) << "req. WORKERS"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-6) << "req. PRAD";
    // if(cat=="ENERGY"){
    //     cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-6) << "gen. PRAD"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    // }else     
    // if(cat=="HOUSING"){
    //     cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-6) << "residents"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    // }else 
    // if(cat=="FARM"){
    //     cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-2) << "gen. jedzenie"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-11) << "TIME"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    // }else 
    // if(cat=="PRODUCER"){
    //     cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-5) << "gen. STONE"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-5) << "gen. TYTAN"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    // }else 
    // if(cat=="TERR"){
    //     cout<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<< setw(w-6) << "gen. terr"<<YELLOW<<NO_BOLD<<sep<<BOLD<<BLUE<<setw(w) << "OPIS"<<RESET<<endl;
    // }
    // cout<<YELLOW<<string(120,'-')<<RESET<<endl;

    // for(const auto &[k,info]:bazaDanych){
    //     if(cat==info.type&&info.lvlTerr<=kolonia.getLvlTerr()){
    //         cout<<left<<setw(w_n)<<info.nazwa<<YELLOW<<sep<<RESET<<CYAN<<setw(w-2)<<info.kKamien<<YELLOW<<sep<<RESET<<CYAN<<setw(w-3)<<info.kTytan<<YELLOW<<sep<<RESET<<setw(w-3)<<info.workers<<YELLOW<<sep<<RESET<<setw(w-6)<<info.reqEnergy;
    //         if(cat=="ENERGY"){
    //             cout<<YELLOW<<sep<<RESET<< setw(w-6) << info.genInne<<YELLOW<<sep<<info.opis<<RESET<<endl;
    //         }else     
    //         if(cat=="HOUSING"){
    //             cout<<YELLOW<<sep<<RESET<< setw(w-6) << info.genInne<<YELLOW<<sep<<info.opis<<RESET<<endl;
    //         }else
    //         if(cat=="FARM"){
    //             cout<<YELLOW<<sep<<RESET<< setw(w-2) << info.genInne<<YELLOW<<sep<<RESET<<setw(w-11)<<info.x<<YELLOW<<sep<<info.opis<<RESET<<endl;
    //         }else
    //         if(cat=="PRODUCER"){
    //             cout<< YELLOW<<sep<<RESET<<setw(w-5) << info.genKamien<<YELLOW<<sep<<RESET<<setw(w-5)<<info.genTytan<<YELLOW<<sep<<info.opis<<RESET<<endl;
    //         }else
    //         if(cat=="TERR"){
    //             cout<< YELLOW<<sep<<RESET<<setw(w-6) << info.genInne<<YELLOW<<sep<<info.opis<<RESET<<endl;
    //         }
        
        
    //     }
    // }
}

/**
 * @brief Wyświetlanie helpa / uniwersalnego okna
 * 
 */
void Graphics::prntPomoc(){
    
    ImGui::SetNextWindowSize(ImVec2(100, 600)); 

    ImGui::Begin("Pomoc",&czyhelp, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::Text("Różne pomocne  rzeczy.");
    
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
        prntStatystyki(kolonia);

        if(czyhelp){
            prntPomoc();
        }

        if(czyBudynki){
            prntBudynki(kolonia, bazaDanych);
        }

        if(czyBudowanie){
            prntBudowanie(kolonia, bazaDanych,gra);
        }
        if(czyBudowanieCategory){
            prntBuildCategory(wybranaKategoriaBudowa,kolonia,bazaDanych,gra);
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
