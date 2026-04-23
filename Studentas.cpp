#include "Studentas.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>


Studentas::Studentas() : egzaminas(0), vidurkis(0.0f), galrezVid(0.0f), galrezMed(0.0f)
{
}

Studentas::Studentas(std::istream& is): egzaminas(0), vidurkis(0.0f), galrezVid(0.0f), galrezMed(0.0f)
{
    readStudent(is);
}

// Destruktorius
Studentas::~Studentas() = default;

std::istream& Studentas::readStudent(std::istream& is)
{
    is >> vardas >> pavarde;

    nd.clear();
    vidurkis = 0.0f;
    egzaminas = 0;

    int x = 0;
    while (is >> x)
    {
        nd.push_back(x);
    }

    if (!nd.empty())
    {
        egzaminas = nd.back();
        nd.pop_back();

        for (const int p : nd)
        {
            vidurkis += static_cast<float>(p);
        }

        if (!nd.empty())
        {
            vidurkis /= static_cast<float>(nd.size());
        }
    }

    return is;
}

void Studentas::readNdInteractive()
{
    std::cout << "Iveskite namu darbu pazymius (0 – pabaiga):" << std::endl;

    nd.clear();
    vidurkis = 0.0f;

    while (true)
    {
        int x = 0;
        std::cin >> x;

        if (!std::cin)
        {
            throw std::runtime_error("Kazkas ne taip su cin");
        }

        if (x <= 0)
        {
            break;
        }

        if (x > 10)
        {
            std::cout << "Netinkamas skaicius. Iveskite tarp 1 ir 10." << std::endl;
            continue;
        }

        nd.push_back(x);
        vidurkis += static_cast<float>(x);
    }

    if (nd.empty())
    {
        throw std::runtime_error("Nepavyko nuskaityti ne vieno pazymio!");
    }

    vidurkis /= static_cast<float>(nd.size());
}

void Studentas::parinktiAtsitiktinius()
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 10);

    const int kiekND = dist(gen);

    nd.clear();
    vidurkis = 0.0f;

    for (int i = 0; i < kiekND; i++)
    {
        const int balas = dist(gen);
        nd.push_back(balas);
        vidurkis += static_cast<float>(balas);
    }

    vidurkis  /= static_cast<float>(nd.size());
    egzaminas  = dist(gen);
}

// suskaiciuotiGalutini – skaičiuoja galrezVid ir galrezMed
void Studentas::suskaiciuotiGalutini()
{
    std::vector<int> surikiuoti = nd;
    std::sort(surikiuoti.begin(), surikiuoti.end());

    const int n = static_cast<int>(surikiuoti.size());

    if (n == 0)
    {
        galrezVid = 0.0f;
        galrezMed = 0.0f;
        return;
    }

    float med = 0.0f;
    if (n % 2 == 1)
    {
        med = static_cast<float>(surikiuoti[n / 2]);
    }
    else
    {
        med = (static_cast<float>(surikiuoti[n / 2]) + static_cast<float>(surikiuoti[n / 2 - 1])) / 2.0f;
    }

    galrezVid = 0.4f * vidurkis + 0.6f * static_cast<float>(egzaminas);
    galrezMed = 0.4f * med + 0.6f * static_cast<float>(egzaminas);
}


bool comparePagalVarda(const Studentas& a, const Studentas& b)
{
    return a.getVardas() < b.getVardas();
}

bool comparePagalPavarde(const Studentas& a, const Studentas& b)
{
    return a.getPavarde() < b.getPavarde();
}

bool comparePagalGalVid(const Studentas& a, const Studentas& b)
{
    return a.getgalrezVid() < b.getgalrezVid();
}

bool comparePagalGalMed(const Studentas& a, const Studentas& b)
{
    return a.getgalrezMed() < b.getgalrezMed();
}
