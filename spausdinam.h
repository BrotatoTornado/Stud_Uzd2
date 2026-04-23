#pragma once
#ifndef SPAUSDINAM_H
#define SPAUSDINAM_H

#include "konteineris.h"

#include <string>

void spausdinam(const StudContainer& studis, const std::string& pav);
void rikiuotiStudentus(StudContainer& studis, const std::string& target);
void surikiuotiStudentus(StudContainer& studis, int pasirinkimas);
void surikiuotiPagalGalutiniVid(StudContainer& studis);

#endif
