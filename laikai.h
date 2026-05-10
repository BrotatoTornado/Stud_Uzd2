/**
 * @file laikai.h
 * @brief Laiko matavimų struktūra ir pagalbinės funkcijos.
 *
 * Apibrėžia globalų `timers` objektą, kuriame kaupiami kiekvienos
 * programos fazės trukmės matavimai.
 *
 * @author Studentas
 * @version 2.0
 */


#pragma once

 /**
  * @struct TimerResults
  * @brief Saugo kiekvienos programos fazės laiko matavimus (sekundėmis).
  */

struct TimerResults
{
    double generavimas = 0.0;
    double skaitymas = 0.0;
    double rusiavimas = 0.0;
    double skirstymas = 0.0;
    double isvedimas = 0.0;
};

/** @brief Globalus laiko matavimų objektas. */
extern TimerResults timers;

/**
 * @brief Nulinina visus laiko matavimus.
 *
 * Iškviečiama kiekvienos naujos iteracijos pradžioje.
 */

void nunulintiLaikus();
