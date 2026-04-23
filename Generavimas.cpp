#include "Generavimas.h"

#include "laikai.h"
#include "spausdinam.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>


static bool galimasPavadinimas(const std::string& pav)
{
    std::ifstream f(pav);
    return f.is_open();
}

std::string failoPavadinimas(const std::string& bazinisPavadinimas)
{
    std::string pavadinimas = bazinisPavadinimas + ".txt";
    int count = 1;

    while (galimasPavadinimas(pavadinimas))
    {
        count++;
        pavadinimas = bazinisPavadinimas + "_" + std::to_string(count) + ".txt";
    }

    return pavadinimas;
}

SkirstymoStrategija pasirinktiStrategija()
{
    std::cout << "\nPasirinkite studentu skirstymo strategija:\n"
              << "1 - Du nauji konteineriai (vargsiukai ir protai)\n"
              << "2 - Tik vargsiuku konteineris; protus palikti bendrame\n"
              << "3 - Efektyvi strategija su std::partition\n";

    int p = 0;
    std::cin >> p;

    if (!std::cin || p < 1 || p > 3)
    {
        throw std::runtime_error("Neteisingas strategijos pasirinkimas.");
    }

    return static_cast<SkirstymoStrategija>(p);
}

Skirstymorezultatasultatas skirstymasStrategija1(const StudContainer& studis)
{
    Skirstymorezultatasultatas rezultatas;

    for (const auto& s : studis)
    {
        if (s.getgalrezVid() < 5.0f)
        {
            rezultatas.vargsiukai.push_back(s);
        }
        else
        {
            rezultatas.protai.push_back(s);
        }
    }

    return rezultatas;
}

Skirstymorezultatasultatas skirstymasStrategija2(StudContainer& studis)
{
    Skirstymorezultatasultatas rezultatas;

    for (auto i = studis.begin(); i != studis.end(); )
    {
        if (i->getgalrezVid() < 5.0f)
        {
            rezultatas.vargsiukai.push_back(*i);
            i = studis.erase(i);
        }
        else
        {
            i++;
        }
    }

    rezultatas.protai = std::move(studis);
    return rezultatas;
}

Skirstymorezultatasultatas skirstymasStrategija3(StudContainer& studis)
{
    Skirstymorezultatasultatas rezultatas;

    auto riba = std::partition(studis.begin(), studis.end(),
        [](const Studentas& s)
        {
            return s.getgalrezVid() >= 5.0f;
        });

    rezultatas.vargsiukai.assign(riba, studis.end());
    rezultatas.protai.assign(studis.begin(), riba);

    studis.clear();

    return rezultatas;
}

Skirstymorezultatasultatas skirstymasGrupes(
    StudContainer& studis,
    SkirstymoStrategija strategija,
    bool irasytiIFailus,
    bool paprasytiRikiavimo)
{
    auto start = std::chrono::high_resolution_clock::now();

    Skirstymorezultatasultatas rezultatas;

    switch (strategija)
    {
        case SkirstymoStrategija::Pirma:
        {
            rezultatas = skirstymasStrategija1(studis);
            break;
        }
        case SkirstymoStrategija::Antra:
        {
            rezultatas = skirstymasStrategija2(studis);
            break;
        }
        case SkirstymoStrategija::Trecia:
        {
            rezultatas = skirstymasStrategija3(studis);
            break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    timers.skirstymas = std::chrono::duration<double>(end - start).count();

    if (!irasytiIFailus)
    {
        timers.isvedimas = 0.0;
        return rezultatas;
    }

    if (paprasytiRikiavimo)
    {
        rikiuotiStudentus(rezultatas.vargsiukai, "vargsiukus");
        rikiuotiStudentus(rezultatas.protai, "protus");
    }

    auto startPrint = std::chrono::high_resolution_clock::now();

    const std::string sufiksas =
        "_" + aktyvausKonteinerioTrumpasPavadinimas() +
        "_S" + std::to_string(static_cast<int>(strategija));

    spausdinam(rezultatas.vargsiukai, failoPavadinimas("Vargsiukai" + sufiksas));
    spausdinam(rezultatas.protai, failoPavadinimas("protai"     + sufiksas));

    auto endPrint = std::chrono::high_resolution_clock::now();
    timers.isvedimas = std::chrono::duration<double>(endPrint - startPrint).count();

    std::cout << "Studentai suskirstyti. Vargsiuku: " << rezultatas.vargsiukai.size()
              << ", protu: " << rezultatas.protai.size() << std::endl;

    return rezultatas;
}

void generuotiFaila()
{
    std::cout << "Iveskite failo pavadinima: ";
    std::string pav;
    std::cin >> pav;

    std::ofstream write(pav);
    if (!write.is_open())
    {
        std::cerr << "Nepavyko sukurti failo." << std::endl;
        return;
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 10);

    std::cout << "Iveskite studentu kieki: ";
    int kiekis = 0;
    std::cin >> kiekis;

    auto start = std::chrono::high_resolution_clock::now();

    const int ndKiek = dist(gen);

    write << std::left << std::setw(15) << "Vardas" << std::setw(15) << "Pavarde";
    for (int j = 0; j < ndKiek; ++j)
    {
        write << std::setw(8) << ("ND" + std::to_string(j + 1));
    }
    write << std::setw(8) << "Egz." << '\n';

    for (int i = 0; i < kiekis; ++i)
    {
        write << std::left
              << std::setw(15) << ("Vardas"  + std::to_string(i + 1))
              << std::setw(15) << ("Pavarde" + std::to_string(i + 1));

        for (int j = 0; j < ndKiek; ++j)
        {
            write << std::setw(8) << dist(gen);
        }

        write << std::setw(8) << dist(gen) << '\n'; // egzaminas
    }

    auto end = std::chrono::high_resolution_clock::now();
    timers.generavimas = std::chrono::duration<double>(end - start).count();

    std::cout << "Failas \"" << pav << "\" sugeneruotas per "
              << timers.generavimas << " s." << std::endl;
}
