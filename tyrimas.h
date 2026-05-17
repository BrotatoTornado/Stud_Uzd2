/**
 * @file tyrimas.h
 * @brief Konteinerių ir skirstymo strategijų greitaveikos tyrimo funkcijos.
 *
 * Atlieka laiko matavimus skaitant failus, rikiuojant ir skirstant studentus
 * bei išveda rezultatus į konsolę ir Markdown lentelę.
 *
 * @author Studentas
 * @version 3.0
 */

#pragma once

 /**
  * @brief Paleidžia interaktyvų konteinerių greitaveikos tyrimą.
  *
  * Vartotojas gali pasirinkti:
  * - skirstymo strategiją;
  * - kartojimų skaičių (rezultatai vidurkinami);
  * - testuojamą failą arba standartinį failų rinkinį (1k–10M eilučių).
  *
  * Rezultatai išvedami į konsolę ir išsaugomi kaip Markdown lentelė
  * faile `benchmark_<konteineris>_S<strategija>.md`.
  */

void vykdytiTyrima();
