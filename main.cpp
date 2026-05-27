#include "Generavimas.h"
#include "konteineris.h"
#include "laikai.h"
#include "meniu.h"
#include "skaitymas.h"
#include "spausdinam.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>

namespace
{
int gautiApdorojimoPasirinkima()
{
    std::cout << "\nKą daryti su gautais studentais?\n";
    std::cout << "1 - Spausdinti visus studentus\n";
    std::cout << "2 - Suskirstyti i vargsiukus ir kietiakus\n";
    std::cout << "3 - Abu veiksmai\n";

    while (true)
    {
        int pasirinkimas;
        std::cin >> pasirinkimas;

        if (!std::cin || pasirinkimas < 1 || pasirinkimas > 3)
        {
            std::cerr << "Neteisinga ivestis. Bandykite is naujo." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        return pasirinkimas;
    }
}
}

void pradetiSpausdint(const StudContainer& studis)
{
    int faila;
    std::cout << "I konsole ar i faila?\n1 - Konsole\n2 - Faila" << std::endl;

    while (true)
    {
        std::cin >> faila;

        if (!std::cin)
        {
            std::cerr << "Neteisinga ivestis. Bandykite is naujo." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        break;
    }

    if (faila == 1)
    {
        std::cout << std::left << std::setw(20) << "Vardas"
                  << std::setw(20) << "Pavarde"
                  << std::setw(20) << "Galutinis(Vid.)"
                  << std::setw(15) << "Galutinis(Med.)" << '\n';
        std::cout << "-------------------------------------------------------\n";

        for (const auto& i : studis)
        {
            std::cout << std::left << std::setw(20) << i.vard
                      << std::setw(20) << i.pav
                      << std::setw(20) << std::fixed << std::setprecision(2) << i.galrezVid
                      << std::setw(15) << std::fixed << std::setprecision(2) << i.galrezMed
                      << '\n';
        }
    }
    else if (faila == 2)
    {
        std::string pav;
        std::cout << "Iveskite pavadinima:" << std::endl;

        while (true)
        {
            std::cin >> pav;

            if (!std::cin)
            {
                std::cerr << "Neteisinga ivestis. Bandykite is naujo." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            break;
        }

        spausdinam(studis, pav);
    }
    else
    {
        std::cout << "Neteisingas pasirinkimas." << std::endl;
    }
}

void laikoRez(double progTrukme)
{
    std::cout << "Failo skaitymas: " << timers.skaitymas << '\n';
    std::cout << "Studentu rikiavimas: " << timers.rusiavimas << '\n';
    std::cout << "Studentu skirstymas: " << timers.skirstymas << '\n';
    std::cout << "Failu isvedimas: " << timers.isvedimas << '\n';
    std::cout << "Visos programos trukme: " << progTrukme << '\n';
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    try
    {
        while (true)
        {
            nunulintiLaikus();

            std::cout << "\nAktyvus studentu konteineris: "<< aktyvausKonteinerioPavadinimas() << '\n';

            StudContainer studis;

            if (!vykdytiMeniu(studis))
            {
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                laikoRez(duration.count());
                return 0;
            }

            if (studis.empty())
            {
                std::cout << "Nera ivestu studentu.\n";
                continue;
            }

            suskaiciuotiGalutinius(studis);

            const int apdorojimas = gautiApdorojimoPasirinkima();

            if (apdorojimas == 1 || apdorojimas == 3)
            {
                rikiuotiStudentus(studis, "studentus");
                pradetiSpausdint(studis);
            }

            if (apdorojimas == 2 || apdorojimas == 3)
            {
                const SkirstymoStrategija strategija = pasirinktiStrategija();
                skirstymasGrupes(studis, strategija, true, true);
            }

            char gr;
            std::cout << "Grizti i pradzia? (y/n): ";
            std::cin >> gr;

            if (!std::cin)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            if (gr == 'y' || gr == 'Y')
            {
                continue;
            }

            break;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Klaida: " << e.what() << std::endl;
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    laikoRez(duration.count());
    return 0;
}
