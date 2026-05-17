# Studentų duomenų apdorojimo programa v3.0

> Studentų duomenų valdymo sistema su nuosava `Vector<T>` klase – pilnaverčiu `std::vector` pakaitalu, spartos analize, Google Test testais ir Inno Setup diegimo paketu.

---

## Turinys

- [Aprašymas](#aprašymas)
- [Versijų istorija](#versijų-istorija)
- [Projekto struktūra](#projekto-struktūra)
- [Vector klasė](#vector-klasė)
- [Klasių hierarchija](#klasių-hierarchija)
- [Rule of Five](#rule-of-five)
- [Sistemos parametrai](#sistemos-parametrai)
- [Įdiegimas ir kompiliavimas](#įdiegimas-ir-kompiliavimas)
- [Doxygen dokumentacija](#doxygen-dokumentacija)
- [Diegimo paketas](#diegimo-paketas)
- [Konteinerių tyrimas](#konteinerių-tyrimas)

---

## Aprašymas

Projektas realizuoja studentų duomenų valdymo sistemą naudojant C++17.
Pagrindinė klasė `Studentas` paveldi iš abstrakčios `Zmogus` bazės ir
pilnai realizuoja **Rule of Five**. Sistema palaiko keturis konteinerius:
`std::vector`, `std::list`, `std::deque` bei **nuosavą `Vector<T>`** klasę,
kuri apima ne mažiau 80% `std::vector` funkcijų. Pateikiamas Inno Setup
diegimo paketas su Start meniu ir darbastalio nuorodomis.

---

## Versijų istorija

### v3.0 (dabartinė)
- Sukurta **nuosava `Vector<T>`** klasė – pilnavertė `std::vector` alternatyva
- Atlikta **spartos analizė**: `std::vector` vs `Vector<T>` (push_back, insert, erase)
- Palyginti **atminties perskirstymų** skaičiai užpildant iki 100 000 000 elementų
- Pridėta ** Google Test** testų `Vector<T>` klasei
- Sukurtas **Inno Setup** diegimo paketas (`setup.iss` → `Setup.exe`)

---

## Projekto struktūra

```
.
├── main.cpp                  # Pagrindinis failas
├── Zmogus.h / .cpp           # Abstrakti bazinė klasė
├── Studentas.h / .cpp        # Konkreti klasė (Rule of Five)
├── Vector.h                  # Nuosava Vector<T> klasė (v3.0)
├── konteineris.h             # Kompiliavimo laiku pasirenkamas konteineris
├── Generavimas.h / .cpp      # Generavimas ir skirstymas į grupes
├── skaitymas.h / .cpp        # Duomenų skaitymas
├── spausdinam.h / .cpp       # Rikiavimas ir išvedimas
├── meniu.h / .cpp            # Interaktyvus meniu
├── tyrimas.h / .cpp          # Greitaveikos tyrimas
├── testas.h / .cpp           # Vidinis testavimo modulis
├── laikai.h / .cpp           # Laiko matavimų struktūra
├── vector_tests.cpp          # Google Test testai Vector<T> (v3.0)
├── spartos_analize.cpp       # Spartos analizės programa (v3.0)
├── Makefile                  # Makefile sistema
├── Doxyfile                  # Doxygen konfigūracija
├── setup.iss                 # Inno Setup skriptas
└── docs/
    ├── refman.pdf
    ├── html/              # HTML dokumentacija
    └── latex/             # LaTeX dokumentacija + PDF
```

---

## Vector klasė

`Vector<T>` – nuosava dinaminė masyvo realizacija, apimanti:

### Member types
| Tipas | Aprašymas |
|-------|-----------|
| `value_type` | Saugomo elemento tipas `T` |
| `size_type` | `std::size_t` |
| `iterator` | `T*` rodyklė |
| `reverse_iterator` | `std::reverse_iterator<iterator>` |
| `allocator_type` | `std::allocator<T>` |

### Realizuotos funkcijos (pavyzdžiai)

**1. `push_back` / `emplace_back`**
```cpp
Vector<int> v;
v.push_back(1);
v.push_back(2);
v.emplace_back(3);
// v = {1, 2, 3}
```

**2. `insert`**
```cpp
Vector<int> v = {1, 3, 4};
v.insert(v.begin() + 1, 2);
// v = {1, 2, 3, 4}
```

**3. `erase`**
```cpp
Vector<int> v = {1, 2, 3, 4};
v.erase(v.begin() + 1, v.begin() + 3);
// v = {1, 4}
```

**4. `resize` ir `reserve`**
```cpp
Vector<int> v;
v.reserve(100);   // rezervuoja atmintį, size=0
v.resize(5, 99);  // v = {99, 99, 99, 99, 99}
```

**5. Palyginimo operatoriai**
```cpp
Vector<int> a = {1, 2, 3};
Vector<int> b = {1, 2, 4};
bool r1 = (a == a);  // true
bool r2 = (a < b);   // true
bool r3 = (a != b);  // true
```

### Funkcijų padengimas

| Kategorija | Funkcijos | Realizuota |
|-----------|-----------|-----------|
| Konstruktoriai | 8 | 8 |
| Elementų prieiga | `at`, `[]`, `front`, `back`, `data` | 5 |
| Iteratoriai | `begin/end`, `rbegin/rend`, `cbegin/cend` | 8 |
| Talpa | `size`, `capacity`, `empty`, `reserve`, `shrink_to_fit` | 6 |
| Modifikatoriai | `push_back`, `pop_back`, `insert`, `erase`, `emplace`, `emplace_back`, `clear`, `resize`, `swap`, `assign` | 10 |
| Non-member | `==`, `!=`, `<`, `<=`, `>`, `>=`, `swap`, `erase`, `erase_if` | 9 |

---

## Klasių hierarchija

```
Zmogus  (abstrakti)
│   ├── vardas : string
│   ├── pavarde : string
│   ├── getVardas() : string
│   ├── getPavarde() : string
│   ├── suskaiciuotiGalutini() = 0  ← grynoji virtuali
│   └── spausdinti(ostream&) = 0   ← grynoji virtuali
│
└── Studentas  (konkreti)
        ├── egzaminas : int
        ├── nd : vector<int>
        ├── vidurkis : float
        ├── galrezVid : float
        ├── galrezMed : float
        ├── suskaiciuotiGalutini() override
        └── spausdinti(ostream&) override
```

---

## Rule of Five

| Metodas | Realizacija |
|---------|-------------|
| Default konstruktorius | Inicializuoja laukus į 0 / tuščius |
| Kopijavimo konstruktorius | Gili kopija (`nd` vektorius) |
| Kopijavimo priskyrimo op. | Savipriskyrimo apsauga |
| Perkėlimo konstruktorius | Perkelia `nd`; šaltinis lieka tuščias |
| Perkėlimo priskyrimo op. | Savipriskyrimo apsauga + perkėlimas |
| Destruktorius | `= default` (vektorius atlaisvinamas automatiškai) |

### Galutinio balo formulė

```
galrezVid = 0.4 × vidurkis  + 0.6 × egzaminas
galrezMed = 0.4 × mediana   + 0.6 × egzaminas
```

---

## Sistemos parametrai

| Parametras | Reikšmė |
|------------|---------|
| CPU | 12th Gen Intel Core i5-12400F |
| RAM | 32 GB DDR4 |
| Diskas | HDD |
| OS | Windows 10 |
| Kompiliatorius | GCC / MSVC |

---

## Įdiegimas ir kompiliavimas

### Reikalavimai
- C++17 palaikantis kompiliatorius (GCC ≥ 9, Clang ≥ 10, MSVC 2019+)
- (Neprivaloma) Google Test – unit testams
- (Neprivaloma) Doxygen – dokumentacijai
- (Neprivaloma) Inno Setup – diegimo paketui

### Kompiliavimo instrukcijos

```bash
# Visi 4 konteineriai su -O2
make

# Atskiras konteineris
make programa_vector      # std::vector
make programa_manoVector  # Vector<T> nuosava
make programa_list        # std::list
make programa_deque       # std::deque

# Optimizuotos std::vector versijos (O1, O2, O3)
make opt_vector

# Optimizuotos Vector<T> versijos (O1, O2, O3)
make opt_manoVector

# Spartos analizė
make benchmark

# Unit testai (reikia įdiegti GTest: sudo apt install libgtest-dev)
make test

# Doxygen dokumentacija
make docs

# Valymas
make clean
```

### Paleidimas

```bash
# Linux/macOS
./programa_vector
./programa_manoVector

# Windows
.\programa_vector.exe
.\programa_manoVector.exe
```

---

## Doxygen dokumentacija

### Generavimas

```bash
make docs
# arba tiesiogiai:
doxygen Doxyfile
```

Dokumentacija sukuriama:
- `docs/html/index.html` – HTML versija naršyklėje

---

## Diegimo paketas

### Reikalavimai
- [Inno Setup 6.x](https://jrsoftware.org/isdl.php)

### Diegimo sukūrimas

```bash
# 1. Sukompiliuoti
make programa_manoVector

# 2. Paruošti struktūrą
#    build\Release\programa_manoVector.exe
#    data\studentai10000.txt
#    data\studentai100000.txt

# 3. Kompiliuoti .iss
iscc installer\setup.iss
# Arba atidaryti Inno Setup IDE ir paspausti Ctrl+F9
``
