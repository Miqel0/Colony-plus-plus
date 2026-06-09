# Projekt: Colony++
![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![CMake](https://img.shields.io/badge/CMake-3.15+-red.svg)
![SFML](https://img.shields.io/badge/Graphics-SFML_3.0.2-8CC445.svg)
![ImGui](https://img.shields.io/badge/GUI-Dear_ImGui-purple.svg)


Strategiczno-survivalowa gra napisana w C++, polegająca na zarządzaniu kolonią na Marsie, wydobywaniu surowców i terraformacji planety. 

Projekt początkowo powstał jako w pełni tekstowa gra konsolowa. Obecnie został zaktualizowany do wersji z pełnoprawnym interfejsem graficznym. Cała oryginalna logika, statystyki i mechaniki pozostały nienaruszone, jednak wpisywanie komend w terminalu zostało całkowicie zastąpione przez nowoczesny system okienkowy i sterowanie myszą.

## O Projekcie

Celem gracza jest przetrwanie w trudnych warunkach marsjańskich oraz osiągnięcie pełnej terraformacji planety. Gra działa w systemie turowym, gdzie każda decyzja wpływa na zasoby (Energia, Jedzenie, Tytan, Kamień) oraz losy kolonistów.

**Kluczowe wyzwania:**
* **Pożywienie:** Każdy mieszkaniec kolonii potrzebuje jedzenia, a jego brak skutkuje śmiercią kolonistów i końcem gry.
* **Energia:** Jest potrzebna do funkcjonowania budynków. Bez niej żaden z budynków produkcyjnych nie będzie działać (brak energii nie kończy jednak gry od razu).

## Funkcjonalności

* **Rozbudowane GUI:** Gra wykorzystuje bibliotekę SFML oraz ImGui do renderowania paneli bocznych, interaktywnych tabel i przycisków, eliminując potrzebę znajomości komend tekstowych.
* **Wizualna Siatka Budowania:** Możliwość stawiania budynków bezpośrednio na dwuwymiarowej mapie za pomocą kliknięć myszką oraz prostej obsługi klawiszami.
* **Ekonomia i Zarządzanie:** Złożony system zależności między surowcami (np. brak prądu zatrzymuje wydobycie w kopalniach).
* **System Budowania:** Możliwość wznoszenia i burzenia budynków różnych kategorii (`Energy`, `Farm`, `Housing`, `Producer`, `Terr`). Są one reprezentowane jako klasy pochodne klasy bazowej `Building`.
* **Progresja (Terraformacja):** Dynamiczne odblokowywanie nowych technologii (lepszych i wydajniejszych budynków) wraz ze wzrostem poziomu terraformacji planety.
* **Zaawansowany Zapis i Odczyt:** Pełna serializacja stanu gry do plików tekstowych (`.txt`). Wprowadzono system niestandardowych (customowych) nazw zapisów, co pozwala graczowi na tworzenie i zarządzanie wieloma niezależnymi stanami gry jednocześnie bez nadpisywania poprzednich postępów.
* **Niestandardowe Zasoby Graficzne:** Gra korzysta z własnej czcionki oraz unikalnej ikony aplikacji. Wykorzystano również dedykowane kursory pobrane z zewnętrznego serwisu. Tekstury obiektów zostały wygenerowane przy pomocy modelu sztucznej inteligencji Gemini, a następnie zoptymalizowane do postaci jednego wspólnego pliku graficznego (spritesheet/atlas), z którego wczytywane są wszystkie elementy.

## Technologie i Rozwiązania (Code Perspective)

Projekt demonstruje praktyczne zastosowanie kluczowych paradygmatów języka C++ (Standard C++17):

### 1. Programowanie Obiektowe
* **Dziedziczenie i Polimorfizm:** Fundamentem jest klasa bazowa `Building`. Klasy pochodne implementują wirtualne metody takie jak `work()` (logika pracy budynku pod koniec tury), czy `save()` (zapis do pliku). Funkcja `prnt()`, która wcześniej wypisywała dane w konsoli, została zaadaptowana pod system renderowania w nowym interfejsie.
* **Enkapsulacja:** Klasa `Logistics` przechowuje wszystkie statystyki i zasoby jako pola prywatne, udostępniając je klasie `Colony` oraz budynkom jedynie poprzez gettery i settery.

### 2. Zarządzanie Pamięcią i Typy
* **Smart Pointers:** Wykorzystanie `std::unique_ptr` do zarządzania obiektami budynków w wektorze. Gwarantuje to bezpieczne zarządzanie pamięcią i automatyczne zwalnianie zasobów.
* **Typy Wyliczeniowe:** Użycie `enum class` do identyfikacji kategorii budynków, co zapobiega błędom logicznym w kodzie i ułatwia czytelność.

### 3. Nowy Interfejs Użytkownika (SFML + ImGui)
* Dawne formatowanie oparte na kolorach ANSI i ręcznie pisanych funkcjach (`prntHeader`, `prntTablica`) zostało całkowicie zastąpione przez biblioteki graficzne - SFML i ImGui.
* **SFML (Simple and Fast Multimedia Library):** Odpowiada za tworzenie głównego okna gry, renderowanie tła, siatki budynków, wczytywanie scalonych tekstur, obsługę własnych czcionek i kursorów oraz zdarzeń systemowych na platformie Windows.
* **Dear ImGui:** Obsługuje całą warstwę interaktywną (okna statystyk, przyciski tur, ostrzeżenia o brakach zasobów, dynamiczne tooltipy po najechaniu kursorem).

### 4. Struktura Projektu
Logika gry jest ściśle odseparowana od warstwy wizualnej. Główne moduły to m.in. `Game` (kontroler), `Colony` (zarządzanie obiektami na mapie), `Logistics` ("mózg" ekonomiczny) oraz poszczególne klasy budynków.

## Przejście z Konsoli na Interfejs Graficzny

Gra nadal opiera się na oryginalnych założeniach mechanicznych, jednak sposób interakcji uległ zmianie. Dawne komendy tekstowe mają teraz swoje bezpośrednie odpowiedniki w interfejsie graficznym:

* **Komenda info / show:** Przeniesione do dynamicznych okien i tooltipów. Najechanie na budynek na mapie lub w menu budowania automatycznie wyświetla wszystkie jego parametry i koszty w panelu ImGui.
* **Komenda build:** Wykonywana poprzez wybranie budynku z panelu bocznego i kliknięcie Lewym Przyciskiem Myszy na wolne pole na siatce głównej.
* **Komenda destroy:** Zastąpiona trybem niszczenia (lub dedykowanym przyciskiem w GUI), pozwalającym na kliknięcie istniejącego budynku w celu odzyskania zasobów i pracowników.
* **Komenda colony:** Zastąpiona stałym, widocznym przez cały czas panelem górnym/bocznym, który na bieżąco aktualizuje stan populacji, prądu i zasobów.
* **Komenda next:** Zastąpiona głównym przyciskiem "Kolejna Tura" na ekranie.
* **Komendy save, load, exit:** Dostępne w postaci przycisków w głównym menu pauzy wywoływanym klawiszem ESC.

## Pliki Danych i Zasoby

Gra korzysta z zewnętrznych plików do przechowywania stanu i konfiguracji:

* **Zasoby Graficzne (Assets):** Skonsolidowany plik tekstur, zdefiniowana ikona aplikacji oraz plik z własną czcionką wczytywaną przy starcie. Do gry dodano również niestandardowe pliki graficzne, które całkowicie modyfikują wygląd i zachowanie kursora myszy podczas rozgrywki.
* **gamedata.txt**: "Baza danych" statystyk wszystkich dostępnych budynków (koszty, produkcja, wymagania). Edycja tego pliku pozwala na łatwe balansowanie rozgrywki bez ingerencji w kod źródłowy.
* **config.txt**: Przechowuje różnego rodzaju ustawienia konfiguracyjne gry, w tym między innymi zapamiętuje nazwę ostatnio tworzonego lub wczytywanego zapisu.
* **Pliki zapisu (Foldery z nazwami zapisów)**: System zapisów opiera się na tworzeniu nowych folderów o niestandardowych nazwach wybranych przez gracza. Wewnątrz każdego takiego folderu generowane są pliki tekstowe o stałych nazwach (np. `buildings.txt`, `colony.txt`), które przechowują szczegółową listę postawionych budynków oraz stan globalny kolonii.

## Instalacja i Uruchomienie

Najprostszym sposobem na uruchomienie gry jest pobranie gotowej, skompilowanej wersji:
1. Przejdź do zakładki **Releases** na stronie głównej repozytorium GitHub.
2. Pobierz najnowszą wersję `.zip`.
3. Wypakuj archiwum w dowolne miejsce na dysku.
4. Uruchom plik `.exe`. Wszystkie niezbędne biblioteki oraz zasoby graficzne znajdują się już w paczce.

## Kompilacja Ręczna (System Windows)

Projekt korzysta z systemu budowania CMake. Wszystkie zależności zostały dołączone bezpośrednio do repozytorium, co maksymalnie upraszcza proces kompilacji.

1. Sklonuj lub pobierz kod źródłowy projektu.
2. Otwórz folder w programie **Visual Studio Code**.
3. Upewnij się, że posiadasz zainstalowane rozszerzenia C/C++ oraz CMake Tools. Wymagany jest również kompilator działający na Windowsie (rekomendowany MSVC wbudowany w Visual Studio Build Tools).
4. Biblioteki (SFML oraz ImGui) znajdują się bezpośrednio w folderze `/libs`, co zwalnia z konieczności ich zewnętrznego pobierania i konfigurowania ścieżek systemowych.
5. Zbuduj projekt, korzystając z paska dolnego w VS Code (przycisk *Build*) lub z terminala, uruchamiając komendy CMake.
6. Uruchom wygenerowany plik `.exe` (skrypt CMake został skonfigurowany tak, aby automatycznie skopiować niezbędne zasoby graficzne, czcionki i pliki tekstowe do folderu wyjściowego).

## Dokumentacja

Pełna, interaktywna dokumentacja kodu źródłowego została wygenerowana przy użyciu systemu Doxygen. 

**[Kliknij tutaj, aby otworzyć dokumentację Colony++](https://miqel0.github.io/Colony-plus-plus/html/index.html)**

## Autor

* Michał Pawlica / Miquel/ @Miqel0
* Projekt wykonany początkowo na zaliczenie przedmiotu *Języki Programowania*, a następnie niezależnie zmodernizowany do pełnej aplikacji okienkowej w ramach zaliczenia przedmiotu *Zaawansowane Programowanie w C++*.
