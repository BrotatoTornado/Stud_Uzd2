#pragma once
#ifndef LAIKAI_H
#define LAIKAI_H

struct TimerResults
{
    double generavimas = 0.0;
    double skaitymas = 0.0;
    double rusiavimas = 0.0;
    double skirstymas = 0.0;
    double isvedimas = 0.0;
};

extern TimerResults timers;

void nunulintiLaikus();

#endif
