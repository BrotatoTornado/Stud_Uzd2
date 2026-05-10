/**
 * @file meniu.h
 * @brief Pagrindinio programos meniu funkcijos deklaracija.
 *
 * @author Studentas
 * @version 2.0
 */

#pragma once

#include "konteineris.h"

/**
 * @brief Paleidžia pagrindinį interaktyvų programos meniu.
 *
 * Leidžia vartotojui pasirinkti vieną iš šių veiksmų:
 * - įvesti studentą rankiniu būdu;
 * - nuskaityti studentus iš failo;
 * - pridėti atsitiktinius studentus;
 * - generuoti duomenų failą;
 * - atlikti konteinerių greitaveikos tyrimą;
 * - paleisti klasės unit testus;
 * - tikrinti Zmogus abstraktumą;
 * - baigti darbą.
 *
 * @param studis Studentų konteineris, į kurį pridedami / keičiami duomenys.
 * @return `true` jei reikia tęsti programą (grįžti į pagrindinį ciklą),
 *         `false` jei vartotojas pasirinko baigti darbą.
 * @throws std::runtime_error Klaidos įvesties atveju.
 */

bool vykdytiMeniu(StudContainer& studis);
