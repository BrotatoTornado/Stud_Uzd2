#pragma once

#include "konteineris.h"

#include <string>

enum class SkirstymoStrategija
{
    Pirma = 1,
    Antra = 2,
    Trecia = 3
};

struct Skirstymorezultatas
{
    StudContainer vargsiukai;
    StudContainer protai;
};

void generuotiFaila();

std::string failoPavadinimas(const std::string& bazinisPavadinimas);

SkirstymoStrategija pasirinktiStrategija();

Skirstymorezultatas skirstymasStrategija1(const StudContainer& studis);
Skirstymorezultatas skirstymasStrategija2(StudContainer& studis);
Skirstymorezultatas skirstymasStrategija3(StudContainer& studis);

Skirstymorezultatas skirstymasGrupes(
    StudContainer& studis,
    SkirstymoStrategija strategija,
    bool irasytiIFailus   = true,
    bool paprasytiRikiavimo = true);
