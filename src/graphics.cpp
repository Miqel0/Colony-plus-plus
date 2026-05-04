#include "graphics.h"
#include <SFML/System/Clock.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <optional>
#include <map>

Graphics::Graphics(unsigned int szer_,unsigned int wys_):szer(szer_),wys(wys_),window(sf::VideoMode({szer_, wys_}), "Colony ++"),czyhelp(false),czyBudynki(false){}
Graphics::Graphics():szer(1920),wys(1080),window(sf::VideoMode({1920, 1080}), "Colony ++"),czyhelp(false),czyBudynki(false){}

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
    ImGui::End();
}

void Graphics::prntStatystyki(const Colony& kolonia){
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::SetNextWindowSize(ImVec2(szer, 35)); 

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

void Graphics::prntBudynki(const Colony& kolonia){
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
                ImGui::TableNextRow();
                
                ImGui::TableNextColumn(); 
                ImGui::Text("%s", nazwa.c_str()); 
                
                ImGui::TableNextColumn();
                ImGui::Text("%d", ilosc);
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

void Graphics::prntPomoc(){
    
    ImGui::SetNextWindowSize(ImVec2(100, 600)); 

    ImGui::Begin("Pomoc",&czyhelp, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::Text("Różne pomocne  rzeczy.");
    
    ImGui::End();
}

void Graphics::prntAll(const Colony& kolonia){
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
                    //pauza = !pauza; 
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
            prntBudynki(kolonia);
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
