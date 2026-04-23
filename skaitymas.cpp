#include "skaitymas.h"
#include "laikai.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

void skaitomRanka(Studentas& s)
{
    std::string v, p;

    std::cout << "Iveskite varda: ";
    std::cin >> v;
    s.setVardas(v);

    std::cout << "Iveskite pavarde: ";
    std::cin >> p;
    s.setPavarde(p);

    char c = 'y';
    std::cout << "Ar naudoti atsitiktinai parinktus n.d. rezultatus? (y/n): ";
    std::cin >> c;

    if (c == 'n' || c == 'N')
    {
        s.readNdInteractive();

        int egz = 0;
        std::cout << "Iveskite egzamino rezultatasultata (1-10): ";
        std::cin >> egz;
        s.setEgzaminas(egz);
    }
    else
    {
        s.parinktiAtsitiktinius();
    }
}

bool failoSkaitymas(StudContainer& studis)
{
    std::cout << "Iveskite failo pavadinima: ";
    std::string pav;
    std::cin >> pav;
    return failoSkaitymas(studis, pav);
}


bool failoSkaitymas(StudContainer& studis, const std::string& pav)
{
    auto start = std::chrono::high_resolution_clock::now();

    try
    {
        std::ifstream read(pav);

        if (!read.is_open())
        {
            throw std::runtime_error("Neatidarem failo: " + pav);
        }

        std::string eilute;
        std::getline(read, eilute);

        while (std::getline(read, eilute))
        {
            if (eilute.empty())
            {
                continue;
            }

            // Studentas konstruktorius kviečia readStudent()
            std::istringstream ss(eilute);
            Studentas s(ss);

            if (s.getND().empty())
            {
                continue;
            }

            studis.push_back(std::move(s));
        }

        auto end = std::chrono::high_resolution_clock::now();
        timers.skaitymas = std::chrono::duration<double>(end - start).count();

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Klaida skaitant faila: " << e.what() << std::endl;
        return false;
    }
}

void suskaiciuotiGalutinius(StudContainer& studis)
{
    for (auto& s : studis)
    {
        s.suskaiciuotiGalutini();
    }
}
