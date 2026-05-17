/**
 * @file Studentas.cpp
 * @brief Studentas klasės metodų realizacija.
 *
 * Realizuojami visi Rule of Five metodai, įvesties/išvesties operatoriai,
 * galutinio balo skaičiavimas ir pagalbinės lyginimo funkcijos.
 *
 * @author Studentas
 * @version 2.0
 */

#include "Studentas.h"

#include <iomanip>
#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>
#include <sstream>


 /** @brief Numatytasis konstruktorius. Inicializuoja skaičinius laukus į 0. */
Studentas::Studentas() : egzaminas(0), vidurkis(0.0f), galrezVid(0.0f), galrezMed(0.0f)
{
}

/**
 * @brief Srautinis konstruktorius.
 * @details Nuskaito eilutę iš srauto per `readStudent()`.
 * @param is Įvesties srautas (pvz. `std::istringstream`).
 */

Studentas::Studentas(std::istream& is): egzaminas(0), vidurkis(0.0f), galrezVid(0.0f), galrezMed(0.0f)
{
    readStudent(is);
}

/**
 * @brief Kopijavimo konstruktorius. Sukuria gilią kopiją.
 * @param other Kopijuojamas objektas.
 */

Studentas::Studentas(const Studentas& other): Zmogus(other),
    egzaminas(other.egzaminas),
    nd(other.nd),
    vidurkis(other.vidurkis),
    galrezVid(other.galrezVid),
    galrezMed(other.galrezMed)
{
}

/**
 * @brief Kopijavimo priskyrimo operatorius.
 * @details Naudoja savipriskyrimo apsaugą (`if (this == &other)`).
 * @param other Priskiriamas objektas.
 * @return Nuoroda į šį objektą.
 */

Studentas& Studentas::operator=(const Studentas& other)
{
    if (this == &other)
    {
        return *this;
    }

    Zmogus::operator=(other); //vardas, pavarde
    egzaminas = other.egzaminas;
    nd = other.nd;
    vidurkis = other.vidurkis;
    galrezVid = other.galrezVid;
    galrezMed = other.galrezMed;

    return *this;
}

/**
 * @brief Perkėlimo konstruktorius.
 * @details Po perkėlimo šaltinio skaičiniai laukai nulinami,
 * `nd` vektorius perkeliamas.
 * @param other Perkeliamas objektas (rvalue).
 */

Studentas::Studentas(Studentas&& other): Zmogus(std::move(other)),
    egzaminas(other.egzaminas),
    nd(std::move(other.nd)),
    vidurkis(other.vidurkis),
    galrezVid(other.galrezVid),
    galrezMed(other.galrezMed)
{
    other.egzaminas = 0;
    other.vidurkis = 0.0f;
    other.galrezVid = 0.0f;
    other.galrezMed = 0.0f;
}

/**
 * @brief Perkėlimo priskyrimo operatorius.
 * @details Savipriskyrimo apsauga + resursų perkėlimas.
 * @param other Perkeliamas objektas (rvalue).
 * @return Nuoroda į šį objektą.
 */

Studentas& Studentas::operator=(Studentas&& other)
{
    if (this == &other)
    {
        return *this;
    }

    Zmogus::operator=(std::move(other));
    egzaminas = other.egzaminas;
    nd = std::move(other.nd);
    vidurkis = other.vidurkis;
    galrezVid = other.galrezVid;
    galrezMed = other.galrezMed;

    other.egzaminas = 0;
    other.vidurkis = 0.0f;
    other.galrezVid = 0.0f;
    other.galrezMed = 0.0f;

    return *this;
}


/**
 * @brief Išveda studento duomenis į srautą.
 * @details Realizuoja `Zmogus::spausdinti()`.
 * @param os Išvesties srautas.
 */

void Studentas::spausdinti(std::ostream& os) const
{
    os << std::left << std::setw(15) << vardas
        << std::setw(15) << pavarde
        << "Egz: " << std::setw(4) << egzaminas << "ND: [";

    for (std::size_t i = 0; i < nd.size(); ++i)
    {
        if (i > 0) os << ' ';
        os << nd[i];
    }

    os << ']'
        << std::fixed << std::setprecision(2)
        << "  Gal.Vid: " << galrezVid
        << "  Gal.Med: " << galrezMed;
}

/**
 * @brief Apskaičiuoja galutinį įvertinimą.
 * @details Realizuoja `Zmogus::suskaiciuotiGalutini()`.
 *
 * Formulė:
 * - `galrezVid = 0.4 * vidurkis + 0.6 * egzaminas`
 * - `galrezMed = 0.4 * mediana  + 0.6 * egzaminas`
 *
 * Mediana skaičiuojama iš surūšiuoto `nd` vektoriaus kopijos.
 */

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
        med = (static_cast<float>(surikiuoti[n / 2]) +
            static_cast<float>(surikiuoti[n / 2 - 1])) / 2.0f;
    }

    galrezVid = 0.4f * vidurkis + 0.6f * static_cast<float>(egzaminas);
    galrezMed = 0.4f * med + 0.6f * static_cast<float>(egzaminas);
}

/**
 * @brief Išvesties operatorius.
 * @details Delegacija į `Studentas::spausdinti()` (virtualus dispatch).
 */

std::ostream& operator<<(std::ostream& os, const Studentas& s)
{
    s.spausdinti(os);
    return os;
}

/**
 * @brief Įvesties operatorius.
 * @details Nuskaito duomenis ir apskaičiuoja galutinį balą.
 */

std::istream& operator>>(std::istream& is, Studentas& s)
{
    s.readStudent(is);
    s.suskaiciuotiGalutini();
    return is;
}

/**
 * @brief Nuskaito studento duomenis iš srauto (vidinė funkcija).
 * @details Formatas: `Vardas Pavardė ND1 ND2 ... NDn Egzaminas`
 * Paskutinis skaičius laikomas egzamino pažymiu.
 */

std::istream& Studentas::readStudent(std::istream& is)
{
    std::string line;
    if (!std::getline(is, line) || line.empty())
    {
        return is;
    }
    std::istringstream ss(line);

    ss >> vardas >> pavarde;

    nd.clear();
    vidurkis = 0.0f;
    egzaminas = 0;

    int x = 0;

    while (ss >> x)
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

/**
 * @brief Interaktyviai įveda namų darbų pažymius.
 * @throws std::runtime_error Jei neįvestas nė vienas pažymys.
 */

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

/**
 * @brief Sugeneruoja atsitiktinius namų darbų ir egzamino pažymius.
 * @details Naudoja `std::mt19937` su `std::random_device` sėkla.
 * Pažymiai – intervale [1, 10].
 */

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

/** @brief Rikiavimas pagal vardą (didėjančiai). */
bool comparePagalVarda(const Studentas& a, const Studentas& b)
{
    return a.getVardas() < b.getVardas();
}

/** @brief Rikiavimas pagal pavardę (didėjančiai). */
bool comparePagalPavarde(const Studentas& a, const Studentas& b)
{
    return a.getPavarde() < b.getPavarde();
}

/** @brief Rikiavimas pagal galutinį vidurkio balą (didėjančiai). */
bool comparePagalGalVid(const Studentas& a, const Studentas& b)
{
    return a.getgalrezVid() < b.getgalrezVid();
}

/** @brief Rikiavimas pagal galutinį medianos balą (didėjančiai). */
bool comparePagalGalMed(const Studentas& a, const Studentas& b)
{
    return a.getgalrezMed() < b.getgalrezMed();
}
