#include "spausdinam.h"
#include "laikai.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

void surikiuotiStudentus(StudContainer& studis, int pasirinkimas)
{
    if (studis.empty())
    {
        timers.rusiavimas = 0.0;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    switch (pasirinkimas)
    {
        case 1:
            konteinerioSort(studis,
                [](const Studentas& a, const Studentas& b)
                { return a.getVardas() < b.getVardas(); });
            break;
        case 2:
            konteinerioSort(studis,
                [](const Studentas& a, const Studentas& b)
                { return a.getPavarde() < b.getPavarde(); });
            break;
        case 3:
            konteinerioSort(studis,
                [](const Studentas& a, const Studentas& b)
                { return a.getgalrezVid() < b.getgalrezVid(); });
            break;
        case 4:
            konteinerioSort(studis,
                [](const Studentas& a, const Studentas& b)
                { return a.getgalrezMed() < b.getgalrezMed(); });
            break;
        default:
            throw std::runtime_error("Neteisingas rusiavimo pasirinkimas.");
    }

    auto end = std::chrono::high_resolution_clock::now();
    timers.rusiavimas = std::chrono::duration<double>(end - start).count();
}

void surikiuotiPagalGalutiniVid(StudContainer& studis)
{
    surikiuotiStudentus(studis, 3);
}

void rikiuotiStudentus(StudContainer& studis, const std::string& target)
{
    if (studis.empty())
    {
        return;
    }

    std::cout << "\nPagal ka surikiuoti " << target << "?\n";
    std::cout << "1 - Varda\n2 - Pavarde\n3 - Galutinis (Vid.)\n4 - Galutinis (Med.)\n";

    int p = 0;
    std::cin >> p;

    if (!std::cin || p < 1 || p > 4)
    {
        throw std::runtime_error("Neteisingas rusiavimo pasirinkimas.");
    }

    surikiuotiStudentus(studis, p);
}

void spausdinam(const StudContainer& studis, const std::string& pav)
{
    std::ofstream write(pav);

    if (!write.is_open())
    {
        std::cerr << "Nepavyko atidaryti failo: " << pav << std::endl;
        return;
    }

    write << std::left
          << std::setw(20) << "Vardas"
          << std::setw(20) << "Pavarde"
          << std::setw(20) << "Galutinis(Vid.)"
          << std::setw(15) << "Galutinis(Med.)" << '\n';
    write << std::string(75, '-') << '\n';

    for (const auto& s : studis)
    {
        write << std::left
              << std::setw(20) << s.getVardas()
              << std::setw(20) << s.getPavarde()
              << std::setw(20) << std::fixed << std::setprecision(2) << s.getgalrezVid()
              << std::setw(15) << std::fixed << std::setprecision(2) << s.getgalrezVid() << '\n';
    }
}
