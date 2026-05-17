/**
 * @file spartos_analize.cpp
 * @brief Spartos analizė: std::vector vs Vector<T>
 *
 * Kompiliavimas:
 *   g++ -std=c++17 -O2 -I../src spartos_analize.cpp -o spartos_analize
 *
 * Paleidimas:
 *   ./spartos_analize
 */

#include "Vector.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using Clock = std::chrono::high_resolution_clock;
using Seconds = std::chrono::duration<double>;

inline double elapsed(const Clock::time_point& start)
{
    return Seconds(Clock::now() - start).count();
}

//1. push_back spartos testas

struct PushBackResult
{
    std::size_t n;
    double      stdTime;
    double      myTime;
};

PushBackResult benchPushBack(std::size_t n)
{
    PushBackResult r;
    r.n = n;

    // std::vector
    {
        auto t = Clock::now();
        std::vector<int> v;
        for (int i = 1; i <= static_cast<int>(n); ++i)
            v.push_back(i);
        r.stdTime = elapsed(t);
    }

    // Vector<T>
    {
        auto t = Clock::now();
        Vector<int> v;
        for (int i = 1; i <= static_cast<int>(n); ++i)
            v.push_back(i);
        r.myTime = elapsed(t);
    }

    return r;
}

void pushBackBenchmark()
{
    const std::size_t sizes[] = {
        10'000, 100'000, 1'000'000, 10'000'000, 100'000'000
    };

    std::cout << "\n=== 1. push_back() spartos palyginimas ===\n\n";
    std::cout << std::left
              << std::setw(15) << "Elementu sk."
              << std::setw(18) << "std::vector (s)"
              << std::setw(18) << "Vector<T>   (s)"
              << "Santykis (my/std)\n";
    std::cout << std::string(70, '-') << '\n';

    for (auto n : sizes)
    {
        auto res = benchPushBack(n);
        const double ratio = res.myTime / res.stdTime;

        std::cout << std::left
                  << std::setw(15) << n
                  << std::setw(18) << std::fixed << std::setprecision(6) << res.stdTime
                  << std::setw(18) << std::fixed << std::setprecision(6) << res.myTime
                  << std::fixed << std::setprecision(3) << ratio << '\n';
    }
}

//2. Perskirstymų skaičius

struct ReallocResult
{
    std::size_t n;
    int         stdReallocations;
    int         myReallocations;
};

ReallocResult countReallocations(std::size_t n)
{
    ReallocResult r;
    r.n = n;

    // std::vector
    {
        std::vector<int> v;
        r.stdReallocations = 0;
        std::size_t prevCap = v.capacity();
        for (int i = 1; i <= static_cast<int>(n); ++i)
        {
            v.push_back(i);
            if (v.capacity() != prevCap)
            {
                prevCap = v.capacity();
                ++r.stdReallocations;
            }
        }
    }

    // Vector<T>
    {
        Vector<int> v;
        r.myReallocations = 0;
        std::size_t prevCap = v.capacity();
        for (int i = 1; i <= static_cast<int>(n); ++i)
        {
            v.push_back(i);
            if (v.capacity() != prevCap)
            {
                prevCap = v.capacity();
                ++r.myReallocations;
            }
        }
    }

    return r;
}

void reallocBenchmark()
{
    std::cout << "\n=== 2. Atminties perskirstymu skaičius ===\n\n";
    std::cout << std::left
              << std::setw(15) << "Elementu sk."
              << std::setw(22) << "std::vector perskirst."
              << "Vector<T> perskirst.\n";
    std::cout << std::string(60, '-') << '\n';

    const std::size_t sizes[] = {
        100'000, 1'000'000, 10'000'000, 100'000'000
    };

    for (auto n : sizes)
    {
        auto res = countReallocations(n);
        std::cout << std::left
                  << std::setw(15) << n
                  << std::setw(22) << res.stdReallocations
                  << res.myReallocations << '\n';
    }
}

// 3. Kitos operacijos (insert, erase, reserve)

void otherOpsBenchmark()
{
    const int N = 1'000'000;

    std::cout << "\n=== 3. Kitos operacijos (N=" << N << ") ===\n\n";

    // --- reserve ---
    double stdT, myT;
    {
        auto t = Clock::now();
        std::vector<int> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i)
            v.push_back(i);
        stdT = elapsed(t);
    }
    {
        auto t = Clock::now();
        Vector<int> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i)
            v.push_back(i);
        myT = elapsed(t);
    }
    std::cout << std::left
              << std::setw(30) << "reserve(N) + push_back:"
              << "std=" << std::fixed << std::setprecision(6) << stdT
              << "s  my=" << std::fixed << std::setprecision(6) << myT << "s\n";

    // --- erase (priekis) ---
    const int kiekis = 10000;
    {
        std::vector<int> v(kiekis);
        for (int i = 0; i < v.size(); ++i)
        {
            v[i] = i;
        }
        auto t = Clock::now();
        while (!v.empty())
            v.erase(v.begin());
        stdT = elapsed(t);
    }
    {
        Vector<int> v(kiekis);
        for (int i = 0; i < v.size(); ++i)
        {
            v[i] = i;
        }
        auto t = Clock::now();
        while (!v.empty())
            v.erase(v.begin());
        myT = elapsed(t);
    }
    std::cout << std::left
              << std::setw(30) << "erase(begin()) x" + std::to_string(kiekis) + ":"
              << "std=" << std::fixed << std::setprecision(6) << stdT
              << "s  my=" << std::fixed << std::setprecision(6) << myT << "s\n";

    // --- shrink_to_fit ---
    {
        std::vector<int> v(N);
        v.resize(10);
        auto t = Clock::now();
        v.shrink_to_fit();
        stdT = elapsed(t);
    }
    {
        Vector<int> v(N);
        v.resize(10);
        auto t = Clock::now();
        v.shrink_to_fit();
        myT = elapsed(t);
    }
    std::cout << std::left
              << std::setw(30) << "shrink_to_fit (N->10):"
              << "std=" << std::fixed << std::setprecision(6) << stdT
              << "s  my=" << std::fixed << std::setprecision(6) << myT << "s\n";
}

//4. Paprastas korektiškumo patikrinimas

void correctnessCheck()
{
    std::cout << "\n=== 4. Greitas korektiškumo patikrinimas ===\n";

    bool ok = true;

    // push_back + at
    Vector<int> v;
    for (int i = 0; i < 1000; ++i)
        v.push_back(i * 2);
    for (int i = 0; i < 1000; ++i)
        if (v.at(i) != i * 2) { ok = false; break; }
    std::cout << (ok ? "  [OK] " : "  [FAIL] ")
              << "push_back + at\n";

    // insert viduryje
    Vector<int> v2 = {1, 3};
    v2.insert(v2.begin() + 1, 2);
    ok = (v2[0] == 1 && v2[1] == 2 && v2[2] == 3);
    std::cout << (ok ? "  [OK] " : "  [FAIL] ")
              << "insert(begin+1, value)\n";

    // erase
    Vector<int> v3 = {10, 20, 30, 40};
    v3.erase(v3.begin() + 1);
    ok = (v3.size() == 3 && v3[1] == 30);
    std::cout << (ok ? "  [OK] " : "  [FAIL] ")
              << "erase single\n";

    // swap
    Vector<int> a = {1, 2}, b = {9, 8, 7};
    a.swap(b);
    ok = (a.size() == 3 && b.size() == 2);
    std::cout << (ok ? "  [OK] " : "  [FAIL] ")
              << "swap\n";

    // erase(value) non-member
    Vector<int> v4 = {1, 2, 3, 2, 4};
    erase(v4, 2);
    ok = (v4.size() == 3 && v4[1] == 3);
    std::cout << (ok ? "  [OK] " : "  [FAIL] ")
              << "non-member erase(value)\n";

    std::cout << '\n';
}



int main()
{
    std::cout << "================================================================\n";
    std::cout << "        Vector<T> vs std::vector – Spartos analize v3.0\n";
    std::cout << "================================================================\n";

    correctnessCheck();
    pushBackBenchmark();
    reallocBenchmark();
    otherOpsBenchmark();

    std::cout << "\n[Baigta]\n";
    return 0;
}
