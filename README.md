# Kolonizacja Marsa?

Tekstowa gra strategiczno-survivalowa napisana w C++, polegająca na zarządzaniu kolonią na Marsie, wydobywaniu surowców i terraformacji planety.

## O Projekcie

Celem gracza jest przetrwanie w trudnych warunkach marsjańskich oraz osiągnięcie pełnej terraformacji planety. Gra działa w systemie turowym, gdzie każda decyzja wpływa na zasoby (Energia, Jedzenie, Tytan, Kamień) oraz losy kolonistów.

**Kluczowe wyzwania:**
* **Pożywienie:** Jego brak skutkuje śmiercią kolonistów i końcem gry.
* **Energia:** Bez niej żaden z budynków produkcyjnych nie będzie działać.

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
* **Dziedziczenie i Polimorfizm:** Fundamentem jest klasa bazowa `Building`. Klasy pochodne (`Farm`, `Energy`, `Producer`, `Housing`, `Terr`) implementują wirtualne metody takie jak `work()` (logika tury), `prnt()` (wyświetlanie) czy `save()` (zapis do pliku).
* **Enkapsulacja:** Klasa `Logistics` przechowuje wszystkie statystyki i zasoby jako pola prywatne, udostępniając je klasie `Colony` oraz budynkom jedynie poprzez gettery i settery.

### 2. Zarządzanie Pamięcią
* **Smart Pointers:** Wykorzystanie `std::unique_ptr` do zarządzania obiektami budynków w kontenerze `vector<unique_ptr<Building>>`. Gwarantuje to bezpieczne zarządzanie pamięcią i automatyczne usuwanie obiektów przy niszczeniu budynków lub zamykaniu gry.

### 3. Typy Wyliczeniowe (Enums) i Bezpieczeństwo Typów
* Zamiast używać "magicznych liczb" (np. 1, 2, 3), projekt wykorzystuje enumy (`enum class`) do identyfikacji:
    * Kategorii budynków: `TypBudynku` (np. `ENERGY`, `FARM`).
    * Konkretnych typów wewnątrz kategorii: `TypEnergy`, `TypProducer` itp.
* Zwiększa to czytelność kodu i zapobiega błędom logicznym przy tworzeniu nowych obiektów.

### 4. Interfejs Użytkownika (UI) i Formatowanie
* **Kolory ANSI:** Zdefiniowane w pliku nagłówkowym stałe (np. `RED`, `GREEN`, `BOLD`, `RESET`) pozwalają na kolorowanie tekstu w terminalu, co znacznie poprawia czytelność (np. błędy na czerwono, zasoby na zielono).
* **Funkcje Pomocnicze UI:**
    * `prntHeader(string)`: Standardyzuje nagłówki sekcji.
    * `prntTablica(...)`: Przeciążona funkcja (overloaded), pozwalająca na elastyczne wyświetlanie danych w tabelach o różnej liczbie kolumn.

### 5. Struktura Projektu i Organizacja Plików
Projekt został podzielony na logiczne moduły. Każda główna klasa posiada własny plik nagłówkowy (`.h`) z deklaracjami oraz plik źródłowy (`.cpp`) z implementacją logiczną.

* **`Game` (`game.h` / `game.cpp`)**
    * Główny kontroler aplikacji. Odpowiada za pętlę gry (`run()`), obsługę wejścia użytkownika (`commands()`) oraz interakcję z graczem (wyświetlanie menu, samouczka, komunikatów).
* **`Colony` (`colony.h` / `colony.cpp`)**
    * Klasa zarządzająca kolekcją budynków. Przechowuje wektor wskaźników na obiekty `Building`, odpowiada za ich dodawanie (`zbudujBudynek()`), usuwanie (`zburzBudynek()`) oraz wyszukiwanie. Pełni rolę fasady między interfejsem gry a logiką surowców.
* **`Logistics` (`logistics.h` / `logistics.cpp`)**
    * "Mózg" ekonomiczny gry. Przechowuje wszystkie prywatne statystyki (ilość surowców, energii, populację, poziom terraformacji) i wykonuje na nich obliczenia. Odpowiada za logikę przejścia do następnej rundy (`nextRound()`) oraz sprawdzanie warunków przegranej.
* **`Building` (`building.h` / `building.cpp`)**
    * Klasa bazowa dla wszystkich struktur w grze. Definiuje wspólne atrybuty (nazwa, koszty, wymagania) oraz interfejs metod wirtualnych, które są nadpisywane przez klasy pochodne.
* **Klasy Pochodne (`farm.h/cpp`, `energy.h/cpp` itd.)**
    * Specjalistyczne implementacje klasy `Building`. Każda posiada unikalne właściwości:
        * `Energy`: Zarządzanie produkcją prądu.
        * `Farm`: Logika produkcji żywności (cykle wzrostu).
        * `Housing`: Zwiększanie limitu populacji (dostępni pracownicy).
        * `Producer`: Wydobycie surowców (Kamień/Tytan) w zależności od typu kopalni.
        * `Terr`: Generowanie punktów terraformacji odblokowujących nowe technologie.
* **`Enums`**
    * Zawiera definicje `enum class` oraz stałe kolorów ANSI używane w całym projekcie.
 
  
## Pliki Danych

Gra opiera się na zewnętrznych plikach tekstowych:

* **`gamedata.txt`**: "Baza danych" gry. Zawiera statystyki wszystkich dostępnych budynków (koszty, produkcja, wymagany poziom, opis). Dzięki temu balansowanie gry nie wymaga rekompilacji kodu.
* **`config.txt`**: Przechowuje ustawienia początkowe (jeśli dotyczy).
* **Pliki zapisu (`*_buildings.txt`, `*_colony.txt`)**:
    * Jeden plik przechowuje stan globalny kolonii (surowce, tura, nazwa).
    * Drugi plik przechowuje listę zbudowanych obiektów wraz z ich unikalnym stanem (ID, typ).

## Jak uruchomić (Instalacja)

1.  Przejdź do zakładki **[Releases](../../releases)** po prawej stronie.
2.  Pobierz najnowsze archiwum `.zip` (np. `v1.0-alpha`).
3.  **Wypakuj całe archiwum** do dowolnego folderu.
4.  **Ważne:** Upewnij się, że plik `gamedata.txt` znajduje się w tym samym folderze co plik `.exe`.
5.  Uruchom plik `.exe` w terminalu obsługującym kolory (np. Windows Terminal - na Win 11, PowerShell, VS Code terminal).

## Lista Komend

W trakcie gry mozliwe jest wpisywanie poniższych komend:

| Komenda | Argumenty | Opis |
| :--- | :--- | :--- |
| **`info`** | *-brak-* | Wyświetla listę dostępnych kategorii budynków. |
| **`info`** | `[kategoria]` | Pokazuje szczegóły i koszty budynków z danej kategorii (np. `info ENERGY`). |
| **`build`** | `[nazwa]` | Buduje budynek, jeśli masz surowce (np. `build wiatrak`). |
| **`destroy`** | `[nazwa]` | Niszczy budynek i odzyskuje część zasobów/ludzi. |
| **`show`** | *-brak-* | Wyświetla podsumowanie ilości posiadanych budynków. |
| **`show`** | `[nazwa]` | Pokazuje szczegóły konkretnego typu zbudowanego budynku. |
| **`colony`** | *-brak-* | Wyświetla główny panel statystyk (surowce, energia, pracownicy). |
| **`next`** | *-brak-* | Kończy turę. Następuje produkcja i konsumpcja zasobów. |
| **`rename`** | *-brak-* | Pozwala zmienić nazwę kolonii. |
| **`rules`** | *-brak-* | Wyświetla krótki poradnik i zasady przetrwania. |
| **`help`** | *-brak-* | Wyświetla listę dostępnych komend, wraz z ich opisami i zastosowaniami. |
| **`save`** | *-brak-* | Zapisuje stan gry do pliku. |
| **`load`** | *-brak-* | Wczytuje stan gry z pliku. |
| **`exit`** | *-brak-* | Wyjście z gry. |

## Autor

**Michał Pawlica / @Miquel** - Projekt wykonany na zaliczenie przedmiotu *Języki Programowania*.
