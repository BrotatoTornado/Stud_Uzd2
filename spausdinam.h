/**
 * @file spausdinam.h
 * @brief Studentų duomenų rikiavimo ir išvedimo funkcijų deklaracijos.
 *
 * Pateikia funkcijas studentų rikiavimui pagal įvairius kriterijus
 * ir duomenų išvedimui į failą.
 *
 * @author Studentas
 * @version 3.0
 */

#pragma once

#include "konteineris.h"

#include <string>

 /**
  * @brief Išveda studentų sąrašą į nurodytą failą.
  *
  * Rašo lentelę su kolonėlėmis: Vardas, Pavardė, Galutinis(Vid.), Galutinis(Med.).
  *
  * @param studis Studentų konteineris.
  * @param pav Išvesties failo pavadinimas.
  */

void spausdinam(const StudContainer& studis, const std::string& pav);

/**
 * @brief Interaktyviai prašo rikiavimo kriterijaus ir rikiuoja studentus.
 *
 * Išveda meniu į konsolę, leidžia pasirinkti rikiavimą pagal:
 * vardą, pavardę, galutinį vidurkio arba medianos balą.
 *
 * @param studis Rikiuojamas konteineris.
 * @param target Rodomas tekste vardas (pvz. `"studentus"`, `"vargsiukus"`).
 * @throws std::runtime_error Jei vartotojo įvestis neteisinga.
 */

void rikiuotiStudentus(StudContainer& studis, const std::string& target);

/**
 * @brief Rikiuoja studentus pagal nurodytą kriterijų (int kodas).
 *
 * @param studis Rikiuojamas konteineris.
 * @param pasirinkimas Rikiavimo kriterijus: 1–vardas, 2–pavardė, 3–Vid., 4–Med.
 * @throws std::runtime_error Jei `pasirinkimas` nėra 1–4.
 */

void surikiuotiStudentus(StudContainer& studis, int pasirinkimas);

/**
 * @brief Rikiuoja studentus pagal galutinį vidurkio balą (didėjančia tvarka).
 *
 * Patogiausia funkcija tyrimui - nenaudoja interaktyvaus meniu.
 *
 * @param studis Rikiuojamas konteineris.
 */

void surikiuotiPagalGalutiniVid(StudContainer& studis);
