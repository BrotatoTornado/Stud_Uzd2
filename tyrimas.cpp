#include "tyrimas.h"

#include "Generavimas.h"
#include "konteineris.h"
#include "laikai.h"
#include "skaitymas.h"
#include "spausdinam.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


struct TyrimoIrasas
{
    std::string failas;
    std::size_t irasuKiekis = 0;
    double skaitymas   = 0.0;
    double rusiavimas  = 0.0;
    double skirstymas  = 0.0;
};

namespace
{

bool failasEgzistuoja(const std::string& pav)
{
    std::ifstream f(pav);
    return f.good();
}

TyrimoIrasas atliktiBandymuSerija(
    const std::string& failas,
    SkirstymoStrategija strategija,
    int kartojimai)
{
    TyrimoIrasas vidurkiai;
    vidurkiai.failas = failas;

    for (int i = 0; i < kartojimai; ++i)
    {
        nunulintiLaikus();

        StudContainer studis;
        if (!failoSkaitymas(studis, failas)) // naudoja Studentas() vid.
        {
            return {};
        }

        suskaiciuotiGalutinius(studis);
        vidurkiai.irasuKiekis = studis.size();

        surikiuotiPagalGalutiniVid(studis);
        skirstymasGrupes(studis, strategija, false, false);

        vidurkiai.skaitymas  += timers.skaitymas;
        vidurkiai.rusiavimas += timers.rusiavimas;
        vidurkiai.skirstymas += timers.skirstymas;
    }

    vidurkiai.skaitymas  /= kartojimai;
    vidurkiai.rusiavimas /= kartojimai;
    vidurkiai.skirstymas /= kartojimai;

    return vidurkiai;
}

void spausdintiLentele(const std::vector<TyrimoIrasas>& rezultatasultatai)
{
    std::cout << '\n'
              << std::left
              << std::setw(22) << "Failas"
              << std::setw(16) << "Irasu kiekis"
              << std::setw(16) << "Skaitymas (s)"
              << std::setw(18) << "Rusiavimas (s)"
              << std::setw(18) << "Skirstymas (s)"
              << "Is viso (s)\n"
              << std::string(90, '-') << '\n';

    for (const auto& r : rezultatasultatai)
    {
        const double isViso = r.skaitymas + r.rusiavimas + r.skirstymas;

        std::cout << std::left
                  << std::setw(22) << r.failas
                  << std::setw(16) << r.irasuKiekis
                  << std::setw(16) << std::fixed << std::setprecision(6) << r.skaitymas
                  << std::setw(18) << std::fixed << std::setprecision(6) << r.rusiavimas
                  << std::setw(18) << std::fixed << std::setprecision(6) << r.skirstymas
                  << std::fixed << std::setprecision(6) << isViso << '\n';
    }
}

void iREADME(const std::vector<TyrimoIrasas>& rezultatasultatai, SkirstymoStrategija strategija)
{
    const std::string pav = "benchmark_" + aktyvausKonteinerioTrumpasPavadinimas() + "_S" + std::to_string(static_cast<int>(strategija)) + ".md";

    std::ofstream write(pav);
    if (!write.is_open()) { return; }

    write << "# " << aktyvausKonteinerioPavadinimas() << " - strategija " << static_cast<int>(strategija) << "\n\n";
    write << "| Failas | Irasu kiekis | Skaitymas (s) | Rusiavimas (s) | Skirstymas (s) | Is viso (s) |\n";
    write << "|---|---:|---:|---:|---:|---:|\n";

    for (const auto& r : rezultatasultatai)
    {
        const double isViso = r.skaitymas + r.rusiavimas + r.skirstymas;
        write << "| " << r.failas
              << " | " << r.irasuKiekis
              << " | " << std::fixed << std::setprecision(6) << r.skaitymas
              << " | " << std::fixed << std::setprecision(6) << r.rusiavimas
              << " | " << std::fixed << std::setprecision(6) << r.skirstymas
              << " | " << std::fixed << std::setprecision(6) << isViso<< " |\n";
    }

    std::cout << "Rezultatu lentele issaugota: " << pav << '\n';
}

std::vector<std::string> sudarytiNumatytujuFailuSarasa(const std::string& prefiksas)
{
    return {
        prefiksas + "1000.txt",
        prefiksas + "10000.txt",
        prefiksas + "100000.txt",
        prefiksas + "1000000.txt",
        prefiksas + "10000000.txt"
    };
}

}

void vykdytiTyrima()
{
    std::cout << "\nAktyvus konteineris: " << aktyvausKonteinerioPavadinimas() << '\n';

    const SkirstymoStrategija strategija = pasirinktiStrategija();

    std::cout << "Kiek kartu kartoti kiekviena testa? ";
    int kartojimai = 1;
    std::cin >> kartojimai;

    if (!std::cin || kartojimai < 1)
    {
        throw std::runtime_error("Neteisingas kartojimu skaicius.");
    }

    std::cout << "\n1 - Testuoti viena faila\n"
              << "2 - Testuoti numatytuosius failus (1k, 10k, 100k, 1M, 10M)\n";

    int pasirinkimas = 0;
    std::cin >> pasirinkimas;

    if (!std::cin || pasirinkimas < 1 || pasirinkimas > 2)
    {
        throw std::runtime_error("Neteisingas tyrimo meniu pasirinkimas.");
    }

    std::vector<std::string> failai;

    if (pasirinkimas == 1)
    {
        std::cout << "Iveskite pilna failo pavadinima: ";
        std::string f;
        std::cin >> f;
        failai.push_back(f);
    }
    else
    {
        std::cout << "Iveskite failu prefiksa be dydzio ir .txt (pvz. studentai): ";
        std::string prefiksas;
        std::cin >> prefiksas;
        failai = sudarytiNumatytujuFailuSarasa(prefiksas);
    }

    std::vector<TyrimoIrasas> rezultatasultatai;

    for (const auto& failas : failai)
    {
        if (!failasEgzistuoja(failas))
        {
            std::cout << "Failas nerastas, praleidziamas: " << failas << '\n';
            continue;
        }

        auto rezultatas = atliktiBandymuSerija(failas, strategija, kartojimai);
        if (rezultatas.irasuKiekis != 0)
        {
            rezultatasultatai.push_back(rezultatas);
        }
    }

    if (rezultatasultatai.empty())
    {
        std::cout << "Tyrimui tinkamu failu nerasta.\n";
        return;
    }

    spausdintiLentele(rezultatasultatai);
    iREADME(rezultatasultatai, strategija);
}
