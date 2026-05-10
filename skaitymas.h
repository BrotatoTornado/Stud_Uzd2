/**
 * @file skaitymas.h
 * @brief Studentų duomenų skaitymo funkcijų deklaracijos.
 *
 * Pateikia funkcijas duomenų nuskaitymui rankiniu būdu arba iš failo,
 * bei galutinių balų apskaičiavimui visiems studentams konteineryje.
 *
 * @author Studentas
 * @version 2.0
 */

#pragma once

#include "konteineris.h"

#include <string>

 /**
  * @brief Interaktyviai nuskaito vieno studento duomenis iš konsolės.
  *
  * Prašo vartotojo įvesti vardą, pavardę ir (pagal pasirinkimą)
  * namų darbų pažymius bei egzamino rezultatą rankiniu būdu arba
  * sugeneruoja juos atsitiktinai.
  *
  * @param s Studento objektas, į kurį įrašomi duomenys.
  */

void skaitomRanka(Studentas& s);

/**
 * @brief Interaktyviai prašo failo pavadinimo ir nuskaito studentus.
 *
 * Pirmoji perkrovos versija – prašo vartotojo įvesti failo pavadinimą
 * ir perduoda jį antrajai versijai.
 *
 * @param studis Konteineris, į kurį pridedami nuskaityti studentai.
 * @return `true` jei failas sėkmingai nuskaitytas, `false` klaidos atveju.
 */

bool failoSkaitymas(StudContainer& studis);

/**
 * @brief Nuskaito studentus iš nurodyto failo.
 *
 * Antrarinė perkrovos versija su eksplicitiniu failo pavadinimu.
 * Failo formatas:
 * ```
 * Vardas Pavardė ND1 ND2 ... NDn Egzaminas
 * ```
 * Pirma eilutė (antraštė) ignoruojama.
 * Matuoja nuskaitymo laiką ir išsaugo į globalų `timers` objektą.
 *
 * @param studis Konteineris, į kurį pridedami nuskaityti studentai.
 * @param pav Failo pavadinimas.
 * @return `true` jei failas sėkmingai nuskaitytas, `false` klaidos atveju.
 */

bool failoSkaitymas(StudContainer& studis, const std::string& pav);

/**
 * @brief Apskaičiuoja galutinius balus visiems studentams konteineryje.
 *
 * Iškviečia `Studentas::suskaiciuotiGalutini()` kiekvienam studentui.
 *
 * @param studis Studentų konteineris.
 */

void suskaiciuotiGalutinius(StudContainer& studis);
