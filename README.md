# Projekt: **Colony ++**

Tekstowa gra strategiczno-survivalowa napisana w C++, polegająca na zarządzaniu kolonią na Marsie, wydobywaniu surowców i terraformacji planety.

## O Projekcie

Celem gracza jest przetrwanie w trudnych warunkach marsjańskich oraz osiągnięcie pełnej terraformacji planety. Gra działa w systemie turowym, gdzie każda decyzja wpływa na zasoby (Energia, Jedzenie, Tytan, Kamień) oraz losy kolonistów.

**Kluczowe wyzwania:**
* **Pożywienie:** Każdy mieszkaniec kolonii potrzebuje jedzenia, a przy jego braku skutkuje śmiercią kolonistów i końcem gry.
* **Energia:** Jest potrzebna do funkcjonowania budynków, bez niej żaden z budynków nie będzie działać (alee gra się nie kończy).

## Funkcjonalności

* **Ekonomia i Zarządzanie:** Złożony system zależności między surowcami (np. brak prądu zatrzymuje wydobycie w kopalniach).
* **System Budowania:** Możliwość wznoszenia i burzenia budynków różnych kategorii (`Energy`, `Farm`, `Housing`, `Producer`, `Terr`). Są one reprezentowane jako klasy pochodne klasy bazowej `Building`, posiadające unikalne parametry i metody.
* **Progresja (Terraformacja):** Dynamiczne odblokowywanie nowych technologii (lepszych i wydajniejszych budynków) wraz ze wzrostem poziomu terraformacji planety.
* **Tryby Gry:**
    * *Normalny* (standardowy balans, z samouczkiem).
    * *Sandbox* (nieskończone surowce – **tryb testowy**).
    * *Custom* (własna konfiguracja startowa zasobów – **tryb testowy**).
* **Zapis i Odczyt:** Pełna serializacja stanu gry do plików tekstowych (`.txt`), pozwalająca na kontynuowanie rozgrywki.
* **Interaktywny Samouczek:** Wprowadzenie fabularne, które krok po kroku tłumaczy nowym graczom podstawowe mechaniki i komendy.

## Technologie i Rozwiązania (Code Perspective)

Projekt demonstruje praktyczne zastosowanie kluczowych paradygmatów języka C++:

### 1. Programowanie Obiektowe
* **Dziedziczenie i Polimorfizm:** Fundamentem jest klasa bazowa `Building`. Klasy pochodne (`Farm`, `Energy`, `Producer`, `Housing`, `Terr`) implementują wirtualne metody takie jak `work()` (logika pracy budynku pod koniec tury), `prnt()` (wyświetlanie parametrów budynków) czy `save()` (zapis do pliku).
* **Enkapsulacja:** Klasa `Logistics` przechowuje wszystkie statystyki i zasoby jako pola prywatne, udostępniając je klasie `Colony` oraz budynkom jedynie poprzez gettery i settery.

### 2. Zarządzanie Pamięcią
* **Smart Pointers:** Wykorzystanie `std::unique_ptr` do zarządzania obiektami budynków w kontenerze `vector<unique_ptr<Building>>`. Gwarantuje to bezpieczne zarządzanie pamięcią i automatyczne usuwanie obiektów przy niszczeniu budynków lub zamykaniu gry.

### 3. Typy Wyliczeniowe (Enumy)
* Zamiast używać "magicznych liczb" (np. 1, 2, 3), projekt wykorzystuje enumy (`enum class`) do identyfikacji:
    * Kategorii budynków: `TypBudynku` (np. `TypBudynku::ENERGY`, `TypBudynku::FARM`).
    * Konkretnych typów wewnątrz kategorii: `TypEnergy`, `TypProducer` itp.
* Zwiększa to czytelność kodu i zapobiega błędom logicznym przy tworzeniu nowych obiektów.

### 4. Interfejs Użytkownika (UI) i Formatowanie
* **Kolory ANSI:** Zdefiniowane w pliku nagłówkowym stałe (np. `RED`, `GREEN`, `BOLD`, `RESET`) pozwalają na odpowiednie formatowanie tekstu (zmiana kolorów, tła, grubość itp.) w terminalu, co znacznie poprawia czytelność (np. błędy na czerwono, zasoby na zielono).
* **Funkcje Pomocnicze UI:**
    * `prntHeader(string)`: Wyświetla w taki sam sposób wszelkiego rodzaju nagłówki.
    * `prntTablica(...)`: Przeciążona funkcja, pozwalająca na elastyczne wyświetlanie danych w tabelach o różnej liczbie wierszy.

### 5. Struktura Projektu i Organizacja Plików
Projekt został podzielony na logiczne moduły. Każda główna klasa posiada własny plik nagłówkowy (`.h`) z deklaracjami oraz plik źródłowy (`.cpp`) z implementacją logiczną.

* **`Game` (`game.h` / `game.cpp`)**
    * Główny kontroler aplikacji. Odpowiada za pętlę gry (`run()`), obsługę wejścia użytkownika (`commands()`) oraz interakcję z graczem (wyświetlanie menu, samouczka, komunikatów).
* **`Colony` (`colony.h` / `colony.cpp`)**
    * Klasa zarządzająca kolekcją budynków. Przechowuje wektor wskaźników na obiekty `Building`, odpowiada za ich dodawanie (`zbudujBudynek()`), usuwanie (`zburzBudynek()`) oraz wyszukiwanie. Pełni rolę łącznika między interfejsem gry a logiką budynków.
* **`Logistics` (`logistics.h` / `logistics.cpp`)**
    * "Mózg" ekonomiczny gry. Przechowuje wszystkie prywatne statystyki kolonii (ilość surowców, energii, populację, poziom terraformacji) i wykonuje na nich obliczenia. Odpowiada za logikę przejścia do następnej rundy (`nextRound()`) oraz sprawdzanie warunków przegranej.
* **`Building` (`building.h` / `building.cpp`)**
    * Klasa bazowa dla wszystkich budynków w grze. Definiuje wspólne atrybuty (nazwa, koszty utrzymania, wymagania przy budowaniu) oraz interfejs metod wirtualnych, które są nadpisywane przez klasy pochodne.
* **Klasy Pochodne (`farm.h/cpp`, `energy.h/cpp` itd.)**
    * Specjalistyczne implementacje klasy `Building`. Każda posiada unikalne właściwości:
        * `Energy`: Zarządzanie produkcją prądu.
        * `Farm`: Logika produkcji żywności (cykle wzrostu).
        * `Housing`: Zwiększanie limitu populacji (dostępni pracownicy).
        * `Producer`: Wydobycie surowców (Kamień/Tytan) w zależności od typu kopalni.
        * `Terr`: Generowanie punktów terraformacji odblokowujących nowe technologie.
* **`utils`**
    * Zawiera definicje `enum class` oraz stałe kolorów ANSI używane w całym projekcie, oraz kilka uniwersalnych funkcji przydatnych do wyświetlania danych w konsoli (jak np. `prntHeader(...)` czy `prntTablica(...)`).
 
  
## Pliki Danych

Gra korzysta z zewnętrznych plików tekstowych, w których są zapisywane informaacje podczas zapisywania gry, albo są trzymane parametry samych budynków (w celu ułatwienia zmiany parametrów budynków):

* **`gamedata.txt`**: "Baza danych" używanych budynków w grze. Zawiera statystyki wszystkich dostępnych budynków (koszty, produkcja, wymagany poziom, opis). Dzięki temu balansowanie gry nie wymaga wprowadzania licznych zmian w kodzie.
* **`config.txt`**: Przechowuje ustawienia początkowe (jeśli dotyczy).
* **Pliki zapisu (`*_buildings.txt`, `*_colony.txt`)**:
    * Jeden plik przechowuje stan globalny kolonii (surowce, tura, liczba pracowników, nazwa, itp.).
    * Drugi plik przechowuje listę zbudowanych obiektów wraz z ich unikalnym stanem (ID, typ budynku i jego wszystkie parametry).

## Jak uruchomić - Instalacja

1.  Przejdź do zakładki **[Releases](../../releases)** po prawej stronie.
2.  Pobierz najnowszą pełną wersję `.zip` ([`v0.1.1`](https://github.com/Miqel0/Projekt_Kolonizacja_Marsa/releases/tag/v0.1.1)) albo i dowolną wersję testową `Pre-release`.
3.  **Wypakuj cały folder** do dowolnego folderu.
4.  **Ważne:** Nie zmieniaj żadnego położenia plików, jakakolwiek zmiana będzie skutkować zepsciem programu.
5.  Uruchom plik `.exe` w terminalu obsługującym kolory (np. Windows Terminal - na Win 11, PowerShell, VS Code terminal).

## Pobieranie Kodu Źródłowego - Kompilacja ręczna

Jeśli chcesz zobaczyć kod, edytować go lub skompilować grę samodzielnie:

1. Przejdź do zakładki **[Releases](../../releases)** (znajduje się po prawej stronie na GitHubie).
2. Znajdź najnowszą wersję (oznaczoną zieloną etykietą **Latest**).
3. Przewiń do sekcji **Assets** (na dole opisu tej wersji) i kliknij **Source code (zip)**.
4. Wypakuj pobrane archiwum w dowolne miejsce.
5. Otwórz wypakowany folder w **Visual Studio Code**.
6. Wciśnij **`Ctrl + Shift + B`**, aby zbudować projekt (gra skorzysta ze skonfigurowanego `buildtask-a` dla Windowsa).

**Ważna informacja:**
Projekt został napisany w **czystym C++**.
Nie używam **żadnych zewnętrznych, niestandardowych bibliotek**, które trzeba by doinstalowywać. Jeśli masz działający kompilator C++ (np. MSVC w Visual Studio), gra skompiluje się od razu po pobraniu, bez błędów o brakujących plikach.


## Szybki Start - zapis podstawowej kolonii

Nie musisz budować kolonii od zera, aby sprawdzić jak działają mechaniki gry!
W plikach gry znajduje się **przykładowy zapis** z gotową, funkcjonującą kolonią.

Aby go uruchomić:
1. Włącz grę.
2. Po wybraniu rodzaju gry możesz wybrać opcję wczytania gry, albo zawsze później możesz wpisać komendę **`load`**.
To pozwoli Ci od razu przetestować produkcję surowców, działanie farm i zużycie energii w podstawowej bazie.

## Lista Komend

W trakcie gry mozliwe jest wpisywanie poniższych komend:

| Komenda | Argumenty | Opis |
| :--- | :--- | :--- |
| **`info`** | *-brak-* | Wyświetla listę dostępnych kategorii budynków. |
| **`info`** | `[kategoria]` | Pokazuje szczegóły i koszty budynków z danej kategorii (np. `info ENERGY`). |
| **`build`** | `[nazwa]` | Buduje budynek, jeśli masz surowce i wystarczającą liczbę pracowników (np. `build wiatrak`). |
| **`destroy`** | `[nazwa]` | Niszczy budynek i odzyskuje część zasobów, oraz wszytkich ludzi. |
| **`show`** | *-brak-* | Wyświetla podsumowanie ilości zbudowanych budynków. |
| **`show`** | `[nazwa]` | Pokazuje szczegóły konkretnego zbudowanego budynku. (wszystkie jego parametry oraz ilość tego budynku w kolonii) |
| **`colony`** | *-brak-* | Wyświetla główny panel statystyk kolonii (surowce, energia, pracownicy). |
| **`next`** | *-brak-* | Kończy turę. Następuje sprawdzenie czy wystarcza prądu i jedzenie, a następnie zachodzi produkcja wszystkich zasobów. |
| **`rename`** | *-brak-* | Pozwala zmienić nazwę kolonii (uwzględniając użycie `_`, co następnie jest zastępowane spacją np. `rename New_Vegas` -> `New Vegas`. |
| **`rules`** | *-brak-* | Wyświetla informacje o celu gry i wszelkiego rodzaju zasadach, przydatne jak nie rozumie się jakiejś mechaniki. |
| **`help`** | *-brak-* | Wyświetla listę dostępnych komend, wraz z ich opisami i zastosowaniami. |
| **`save`** | *-brak-* | Zapisuje stan gry do pliku (wszystkie parametry które wpływają na grę!). |
| **`load`** | *-brak-* | Wczytuje stan gry z pliku. |
| **`exit`** | *-brak-* | Wyjście z gry. |

---

## Autor

**Michał Pawlica / @Miqel0** - Projekt wykonany na zaliczenie przedmiotu *Języki Programowania*.
