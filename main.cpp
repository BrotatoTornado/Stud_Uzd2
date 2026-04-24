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
    std::cout << "\nKa daryti su gautais studentais?\n"
              << "1 - Spausdinti visus studentus\n"
              << "2 - Suskirstyti i vargsiukus ir protus\n"
              << "3 - Abu veiksmai\n";

    while (true)
    {
        int p = 0;
        std::cin >> p;

        if (!std::cin || p < 1 || p > 3)
        {
            std::cerr << "Neteisinga ivestis. Bandykite is naujo.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        return p;
    }
}

void pradetiSpausdint(const StudContainer& studis)
{
    std::cout << "I konsole ar i faila?\n1 - Konsole\n2 - Faila\n";

    int failas = 0;
    while (true)
    {
        std::cin >> failas;

        if (!std::cin)
        {
            std::cerr << "Neteisinga ivestis. Bandykite is naujo.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        break;
    }

    if (failas == 1)
    {
        // Naudojame get'erius – tiesioginis prieigos prie laukų nėra
        std::cout << std::left
                  << std::setw(20) << "Vardas"
                  << std::setw(20) << "Pavarde"
                  << std::setw(20) << "Galutinis(Vid.)"
                  << std::setw(15) << "Galutinis(Med.)" << '\n';
        std::cout << std::string(75, '-') << '\n';

        for (const auto& s : studis)
        {
            std::cout << std::left
                      << std::setw(20) << s.getVardas()
                      << std::setw(20) << s.getPavarde()
                      << std::setw(20) << std::fixed << std::setprecision(2) << s.getgalrezVid()
                      << std::setw(15) << std::fixed << std::setprecision(2) << s.getgalrezMed()
                      << '\n';
        }
    }
    else if (failas == 2)
    {
        std::cout << "Iveskite pavadinima: ";
        std::string pav;

        while (true)
        {
            std::cin >> pav;

            if (!std::cin)
            {
                std::cerr << "Neteisinga ivestis. Bandykite is naujo.\n";
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
        std::cout << "Neteisingas pasirinkimas.\n";
    }
}

void laikorezultatas(double progTrukme)
{
    std::cout << "\n--- Laiko rezultatai ---\n"
              << "Failo skaitymas:      " << timers.skaitymas  << " s\n"
              << "Studentu rikiavimas:  " << timers.rusiavimas << " s\n"
              << "Studentu skirstymas:  " << timers.skirstymas << " s\n"
              << "Failu isvedimas:      " << timers.isvedimas  << " s\n"
              << "Visos programos trukme: " << progTrukme      << " s\n";
}

}


int main()
{
    auto progStart = std::chrono::high_resolution_clock::now();

    try
    {
        while (true)
        {
            nunulintiLaikus();

            std::cout << "\nAktyvus konteineris: " << aktyvausKonteinerioPavadinimas() << '\n';

            StudContainer studis;

            if (!vykdytiMeniu(studis))
            {
                break;
            }

            if (studis.empty())
            {
                std::cout << "Nera ivestu studentu.\n";
                continue;
            }

            //kviečia s.suskaiciuotiGalutinius() kiekvienam
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

            char gr = 'n';
            std::cout << "Grizti i pradzia? (y/n): ";
            std::cin >> gr;

            if (!std::cin)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            if (gr != 'y' && gr != 'Y')
            {
                break;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Klaida: " << e.what() << std::endl;
        return 1;
    }

    auto progEnd = std::chrono::high_resolution_clock::now();
    laikorezultatas(std::chrono::duration<double>(progEnd - progStart).count());

    return 0;
}
