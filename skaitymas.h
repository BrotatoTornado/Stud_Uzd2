#pragma once
#ifndef SKAITOM
#define SKAITOM

#include "konteineris.h"

#include <string>

void skaitomRanka(Studentas& s);

bool failoSkaitymas(StudContainer& studis);

bool failoSkaitymas(StudContainer& studis, const std::string& pav);

void suskaiciuotiGalutinius(StudContainer& studis);

#endif
