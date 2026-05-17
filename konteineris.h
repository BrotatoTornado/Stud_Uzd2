#pragma once

#include "Studentas.h"

#include <algorithm>
#include <deque>
#include <list>
#include <string>
#include <vector>

#if defined(USE_LIST)
template <typename T>
using StudentuKonteineris = std::list<T>;

inline std::string aktyvausKonteinerioPavadinimas()
{
    return "std::list";
}

inline std::string aktyvausKonteinerioTrumpasPavadinimas()
{
    return "list";
}

#elif defined(USE_DEQUE)
template <typename T>
using StudentuKonteineris = std::deque<T>;

inline std::string aktyvausKonteinerioPavadinimas()
{
    return "std::deque";
}

inline std::string aktyvausKonteinerioTrumpasPavadinimas()
{
    return "deque";
}

#elif defined(USE_MYVECTOR)

#include "Vector.h"

template <typename T>
using StudentuKonteineris = Vector<T>;

inline std::string aktyvausKonteinerioPavadinimas()
{
    return "Vector<T> (nuosava realizacija)";
}

inline std::string aktyvausKonteinerioTrumpasPavadinimas()
{
    return "myvector";
}

#else
template <typename T>
using StudentuKonteineris = std::vector<T>;

inline std::string aktyvausKonteinerioPavadinimas()
{
    return "std::vector";
}

inline std::string aktyvausKonteinerioTrumpasPavadinimas()
{
    return "vector";
}
#endif

// StudContainer dabar laiko objektus ( yay :) )
using StudContainer = StudentuKonteineris<Studentas>;

template <typename T, typename Alloc, typename Compare>
void konteinerioSort(std::list<T, Alloc>& konteineris, Compare comp)
{
    konteineris.sort(comp);
}

template <typename Container, typename Compare>
void konteinerioSort(Container& konteineris, Compare comp)
{
    std::sort(konteineris.begin(), konteineris.end(), comp);
}
