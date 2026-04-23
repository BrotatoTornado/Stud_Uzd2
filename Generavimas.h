#pragma once
#ifndef GENERAVIMAS_H
#define GENERAVIMAS_H

#include "konteineris.h"

#include <string>

enum class SkirstymoStrategija
{
    Pirma = 1,
    Antra = 2,
    Trecia = 3
};

struct Skirstymorezultatasultatas
{
    StudContainer vargsiukai;
    StudContainer protai;
};

void generuotiFaila();

std::string failoPavadinimas(const std::string& bazinisPavadinimas);

SkirstymoStrategija pasirinktiStrategija();

Skirstymorezultatasultatas skirstymasStrategija1(const StudContainer& studis);
Skirstymorezultatasultatas skirstymasStrategija2(StudContainer& studis);
Skirstymorezultatasultatas skirstymasStrategija3(StudContainer& studis);

Skirstymorezultatasultatas skirstymasGrupes(
    StudContainer& studis,
    SkirstymoStrategija strategija,
    bool irasytiIFailus   = true,
    bool paprasytiRikiavimo = true);

#endif
