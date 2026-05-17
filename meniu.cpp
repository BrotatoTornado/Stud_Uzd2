#include "meniu.h"

#include "Generavimas.h"
#include "skaitymas.h"
#include "tyrimas.h"

#include <iostream>
#include <stdexcept>
#include <string>

bool vykdytiMeniu(StudContainer& studis)
{
    while (true)
    {
        std::cout << "\nPasirinkite programos eiga:\n"
            << "1 - Ivesti studenta ranka\n"
            << "2 - Skaityti is failo ir iskart apdoroti\n"
            << "3 - Prideti studenta su atsitiktiniais pazymiais (su vardu)\n"
            << "4 - Prideti sugeneruota studenta (auto vardas/pavarde)\n"
            << "5 - Sugeneruoti studentu faila\n"
            << "6 - Atlikti konteineriu ir strategiju tyrima\n"
            << "7 - Baigti darba\n";

        int pasirinkimas = 0;
        std::cin >> pasirinkimas;

        if (!std::cin)
        {
            throw std::runtime_error("Bloga ivestis.");
        }

        switch (pasirinkimas)
        {
        case 1:
        {
            Studentas s;
            skaitomRanka(s);
            studis.push_back(s);
            break;
        }

        case 2:
        {
            if (failoSkaitymas(studis))
            {
                return true;
            }
            break;
        }

        case 3:
        {
            Studentas s;
            std::string v, p;
            std::cout << "Iveskite varda: ";
            std::cin >> v;
            s.setVardas(v);
            std::cout << "Iveskite pavarde: ";
            std::cin >> p;
            s.setPavarde(p);
            s.parinktiAtsitiktinius();
            studis.push_back(s);
            break;
        }

        case 4:
        {
            static int nr = 1;
            Studentas s;
            s.setVardas("Vardas" + std::to_string(nr));
            s.setPavarde("Pavarde" + std::to_string(nr));
            nr++;
            s.parinktiAtsitiktinius();
            studis.push_back(s);
            break;
        }

        case 5:
        {
            generuotiFaila();
            break;
        }

        case 6:
        {
            vykdytiTyrima();
            break;
        }
        case 7:
        {
            return false;
        }

        default:
            std::cout << "Neteisingas pasirinkimas.\n";
            break;
        }
    }
}
