/**
 * @file Generavimas.h
 * @brief Studentų failų generavimo ir skirstymo į grupes funkcijų deklaracijos.
 *
 * Apibrėžia `SkirstymoStrategija` enumeraciją, `Skirstymorezultatas` struktūrą
 * ir visas su studentų generavimu bei grupavimu susijusias funkcijas.
 *
 * @author Studentas
 * @version 2.0
 */

#pragma once

#include "konteineris.h"

#include <string>

 /**
  * @enum SkirstymoStrategija
  * @brief Nurodo, kokiu algoritmu skirstyti studentus į grupes.
  *
  * | Reikšmė | Aprašymas |
  * |---------|-----------|
  * | Pirma   | Du nauji konteineriai (vargsiukai ir protai) |
  * | Antra   | Vargsiukai ištraukiami iš bendro konteinerio; protai lieka |
  * | Trecia  | Efektyvus `std::partition` algoritmas |
  */

enum class SkirstymoStrategija
{
    Pirma = 1,
    Antra = 2,
    Trecia = 3
};

/**
 * @struct Skirstymorezultatas
 * @brief Saugo skirstymo į grupes rezultatus.
 */

struct Skirstymorezultatas
{
    StudContainer vargsiukai;
    StudContainer protai;
};

/**
 * @brief Generuoja studentų duomenų failą.
 *
 * Interaktyviai prašo vartotojo įvesti failo pavadinimą ir studentų kiekį.
 * Studentų vardai, pavardės ir pažymiai generuojami automatiškai.
 */
void generuotiFaila();

/**
 * @brief Sugeneruoja unikalų failo pavadinimą.
 *
 * Jei failas su nurodytu pavadinimu jau egzistuoja, automatiškai
 * pridedamas skaitinis sufiksas (pvz. `_2`, `_3`).
 *
 * @param bazinisPavadinimas Bazinis failo pavadinimas be plėtinio.
 * @return Unikalus failo pavadinimas su `.txt` plėtiniu.
 */

std::string failoPavadinimas(const std::string& bazinisPavadinimas);

/**
 * @brief Interaktyviai prašo vartotojo pasirinkti skirstymo strategiją.
 *
 * Išveda meniu į konsolę ir nuskaito vartotojo pasirinkimą.
 *
 * @return Pasirinkta `SkirstymoStrategija`.
 * @throws std::runtime_error Jei vartotojo įvestis neteisinga.
 */

SkirstymoStrategija pasirinktiStrategija();

/**
 * @brief Skirsto studentus pagal 1-ą strategiją.
 *
 * Eina per visus studentus ir juos kopijuoja į atskirus konteinerius.
 * Originalus konteineris nekeičiamas.
 *
 * @param studis Studentų konteineris (const – nekeičiamas).
 * @return `Skirstymorezultatas` su dviem užpildytais konteineriais.
 */

Skirstymorezultatas skirstymasStrategija1(const StudContainer& studis);

/**
 * @brief Skirsto studentus pagal 2-ą strategiją.
 *
 * Vargsiukai ištraukiami iš originalaus konteinerio (erase).
 * Likę studentai (protai) perkeliami į rezultatą.
 *
 * @param studis Studentų konteineris (keičiamas – iš jo šalinami vargsiukai).
 * @return `Skirstymorezultatas`.
 */

Skirstymorezultatas skirstymasStrategija2(StudContainer& studis);

/**
 * @brief Skirsto studentus pagal 3-ą strategiją (efektyviausia).
 *
 * Naudoja `std::partition` algoritmą, kuris perskirsto elementus
 * vietoje, tuomet siunčia į du atskirus konteinerius.
 *
 * @param studis Studentų konteineris (keičiamas).
 * @return `Skirstymorezultatas`.
 */

Skirstymorezultatas skirstymasStrategija3(StudContainer& studis);

/**
 * @brief Bendro naudojimo skirstymo funkcija.
 *
 * Išsaugo laiko matavimus į globalų `timers` objektą.
 * Pagal parametrus gali rašyti į failus ir prašyti rikiavimo pasirinkimo.
 *
 * @param studis Studentų konteineris.
 * @param strategija Naudojama skirstymo strategija.
 * @param irasytiIFailus Jei `true`, rezultatai išsaugomi į failus.
 * @param paprasytiRikiavimo Jei `true`, vartotojas gali pasirinkti rikiavimą.
 * @return `Skirstymorezultatas` su vargsiukais ir protais.
 */

Skirstymorezultatas skirstymasGrupes(
    StudContainer& studis,
    SkirstymoStrategija strategija,
    bool irasytiIFailus = true,
    bool paprasytiRikiavimo = true);
