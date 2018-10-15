#include <memory>
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <string>

class proces {
public:
    proces(std::vector<int> zadania_): //konstruktor, wsadzasz do nuiego wektor i ten wektor zapelniasz gdy wczytujesz z pliku
        zadania { zadania_} {}
    std::vector<int> zadania;
};

int oblicz_cmax(std::vector<std::shared_ptr<proces>> procesy, std::vector<std::shared_ptr<proces>> cmaxy_procesow)
{
        int wiersz = 1; //maszyny
         // obliczmy cmax_procesow korzystajac z algorytmu z zajec wiersz-kolumna - potem
        int kolumna = 1; //zadania
        for (auto in: procesy) {
            kolumna = 1;
            for (auto zadanie: in->zadania){
                cmaxy_procesow[wiersz]->zadania[kolumna] =
                        std::max(cmaxy_procesow[wiersz-1]->zadania[kolumna],
                        cmaxy_procesow[wiersz]->zadania[kolumna-1]) + zadanie; //czas wykonania zadania aktualnie znajdujacego sie na maszynie ktory wejdzie do permutacji
                ++kolumna;
            }
            ++wiersz;
        }
        return cmaxy_procesow[--wiersz]->zadania[--kolumna];
}

std::vector<std::shared_ptr<proces>> wczytaj_z_pliku( // wczytywanie z pliku jak zawsze,
        int numer,                              // ale teraz mamy dodatkowo tablice c_maxow-potem
        std::vector<std::shared_ptr<proces>>& procesy)
{
    std::fstream plik;
    std::string nazwa_pliku { "NEH" };
    nazwa_pliku += std::to_string(numer);
    nazwa_pliku += ".DAT";
    plik.open(nazwa_pliku, std::ios::in);
    if (plik.good()) {
        int n, m, dane;
        plik >> n;
        plik >> m;
        std::vector<int> zadania;
        for (int i=0; i < n; ++i) {
            for (int j=0; j < m; ++j) {
                plik >> dane;
                zadania.push_back(dane);
            }
            procesy.push_back(std::make_shared<proces>(zadania));
            zadania.clear();
        }
        plik.close();
        n++;
        std::vector<std::shared_ptr<proces>> cmaxy_procesow;
        for (int i = 0; i < n; i++) {
            cmaxy_procesow.push_back(std::make_shared<proces>(std::vector<int>(m + 1, 0)));
        }
        return cmaxy_procesow;
    } else {
        std::cout << "Nie ma takiego pliku" << std::endl;
        return std::vector<std::shared_ptr<proces>>();
    }
}



bool jak_sortowac(std::shared_ptr<proces> pierwszy, std::shared_ptr<proces> drugi) //szeregujemy nierosnaco po sumach czasow wykonania na poszczegolnych maszynach
{
    int pierwsza_suma = 0;
    for (const auto& zadanie: pierwszy->zadania) {
        pierwsza_suma += zadanie;
    }
    int druga_suma = 0;
    for (const auto& zadanie: drugi->zadania) {
        druga_suma += zadanie;
    }
    return (pierwsza_suma > druga_suma);
}

void swap(std::shared_ptr<proces>& lewy, std::shared_ptr<proces>& prawy) //funkcja zamieniania zadan
{
    std::shared_ptr<proces> temp = prawy;
    prawy = lewy;
    lewy = temp;
}

int NEH(int numer)
{
    std::vector<std::shared_ptr<proces>> procesy;
    std::vector<std::shared_ptr<proces>> cmaxy_procesow = wczytaj_z_pliku(numer, procesy); // wczytujemy z pliku i mamy teraz
                                                                                // procesy i pusta tablice cmaxow
    std::sort(procesy.begin(), procesy.end(), jak_sortowac); // sortujemy rosnaco od sumy zadan
    std::vector<std::shared_ptr<proces>> aktualna;
    std::vector<std::shared_ptr<proces>> najlepsza;;
    int Cmax;
    int obecny_cmax;
    for (int j = 0; j < procesy.size(); ++j) {
        aktualna.push_back(procesy[j]);               // dodajemy kolejny proces do vectora
        Cmax = oblicz_cmax(aktualna, cmaxy_procesow); // obliczamy cmax
        najlepsza = aktualna;                         // narazie najlepsze ustawienie to te pierwsze
        for (int i = aktualna.size() - 1; i > 0; --i) {
            swap(aktualna[i], aktualna[i - 1]); // zmieniamy nowy proces jak dodalismy czyli ostatni z wczesneijszym potem z
                                                // przed przed ostatnim, czwartym od konca itd..
            obecny_cmax = oblicz_cmax(aktualna, cmaxy_procesow); // oliczamy nowy cmax
            if (Cmax > obecny_cmax) { // jesli krotszy
                najlepsza = aktualna; // to nowe najlepsze ustawienie
                Cmax = obecny_cmax;   // nowy cmax
            }
        }
        aktualna = najlepsza; // dla obecnej ilosci procesow do aktualnej przypisujemy najlepsze ustawienie
    }
    return Cmax; // zwracamy cmax najlepszego ustawienia do wszystkich procesow
}
//funkcja celu ktorej parametrem jest czas wykonywania zadania, zwraca najkrotszy
//faza wstepna - sortowanie
//faza zasadnicza - generowany jest ciag permutacji dla kazdej obliczany jest cmax
//wstawic k-te zadanie na miejsce ktore minimalizuje okres produkcyjny spsrod k mozliwych

int main()
{
    int numer = 1;
    std::cout << "numer pliku?: " << std::endl;
    std::cin >> numer;
    std::cout << "Wynik: " << NEH(numer) << std::endl;
    return 1;
}
