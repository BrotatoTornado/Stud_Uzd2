/**
 * @file konteineris.h
 * @brief Kompiliavimo laiku pasirenkamas studentų konteineris.
 *
 * Naudojant preprocesorių galima pasirinkti vieną iš trijų STL
 * konteinerių tipų:
 * - `std::vector` (numatytasis)
 * - `std::list`   (kompiliuoti su `-DUSE_LIST`)
 * - `std::deque`  (kompiliuoti su `-DUSE_DEQUE`)
 *
 * @author Studentas
 * @version 2.0
 */

#pragma once

#include "Studentas.h"

#include <algorithm>
#include <deque>
#include <list>
#include <string>
#include <vector>

#if defined(USE_LIST)
 /**
  * @brief Studentų konteinerio tipo alias (std::list variantas).
  * @tparam T Elementų tipas.
  */

template <typename T>
using StudentuKonteineris = std::list<T>;

/**
 * @brief Grąžina aktyvaus konteinerio pilną pavadinimą.
 * @return `"std::list"`
 */

inline std::string aktyvausKonteinerioPavadinimas()
{
    return "std::list";
}

/**
 * @brief Grąžina aktyvaus konteinerio trumpąjį pavadinimą.
 * @return `"list"`
 */

inline std::string aktyvausKonteinerioTrumpasPavadinimas()
{
    return "list";
}

#elif defined(USE_DEQUE)

 /**
  * @brief Studentų konteinerio tipo alias (std::deque variantas).
  * @tparam T Elementų tipas.
  */

template <typename T>
using StudentuKonteineris = std::deque<T>;

/**
 * @brief Grąžina aktyvaus konteinerio pilną pavadinimą.
 * @return `"std::deque"`
 */

inline std::string aktyvausKonteinerioPavadinimas()
{
    return "std::deque";
}

/**
 * @brief Grąžina aktyvaus konteinerio trumpąjį pavadinimą.
 * @return `"deque"`
 */

inline std::string aktyvausKonteinerioTrumpasPavadinimas()
{
    return "deque";
}

#else

 /**
  * @brief Studentų konteinerio tipo alias (std::vector variantas – numatytasis).
  * @tparam T Elementų tipas.
  */

template <typename T>
using StudentuKonteineris = std::vector<T>;

/**
 * @brief Grąžina aktyvaus konteinerio pilną pavadinimą.
 * @return `"std::vector"`
 */

inline std::string aktyvausKonteinerioPavadinimas()
{
    return "std::vector";
}

/**
 * @brief Grąžina aktyvaus konteinerio trumpąjį pavadinimą.
 * @return `"vector"`
 */

inline std::string aktyvausKonteinerioTrumpasPavadinimas()
{
    return "vector";
}
#endif

/**
 * @typedef StudContainer
 * @brief Pagrindinis studentų konteineris naudojamas per visą programą.
 *
 * Tipas priklauso nuo kompiliacijos laiko makro (`USE_LIST`, `USE_DEQUE`
 * arba numatytasis `std::vector`).
 */

// StudContainer dabar laiko objektus ( yay :) )
using StudContainer = StudentuKonteineris<Studentas>;

/**
 * @brief Rikiavimo specializacija `std::list` konteineriui.
 *
 * `std::list` neturi iteratoriaus aritmetikos, todėl reikia naudoti
 * `list::sort()` vietoj `std::sort()`.
 *
 * @tparam T Elementų tipas.
 * @tparam Alloc Alokatorius.
 * @tparam Compare Lyginimo funktorius.
 * @param konteineris Rikiuojamas sąrašas.
 * @param comp Lyginimo funktorius.
 */

template <typename T, typename Alloc, typename Compare>
void konteinerioSort(std::list<T, Alloc>& konteineris, Compare comp)
{
    konteineris.sort(comp);
}

/**
 * @brief Universali rikiavimo funkcija bet kuriam atsitiktinės prieigos konteineriui.
 *
 * Naudoja `std::sort` su pradžios ir pabaigos iteratoriais.
 *
 * @tparam Container Konteinerio tipas (pvz. `std::vector`, `std::deque`).
 * @tparam Compare   Lyginimo funktorius.
 * @param konteineris Rikiuojamas konteineris.
 * @param comp        Lyginimo funktorius.
 */

template <typename Container, typename Compare>
void konteinerioSort(Container& konteineris, Compare comp)
{
    std::sort(konteineris.begin(), konteineris.end(), comp);
}
