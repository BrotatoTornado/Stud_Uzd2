# Studentų duomenų apdorojimo programa v1.5

## Projekto aprašymas

**v1.5** versija sukurta remiantis **v1.2** baze. Pagrindinis pokytis – įvesta paveldėjimo hierarchija:

- Nauja **abstrakti bazinė klasė `Zmogus`** – aprašo bendrą žmogaus sąvoką
-`Studentas` dabar yra **išvestinė klasė** iš `Zmogus`
-`Zmogus` objektų kurti **negalima** – klasė abstrakti (turi grynąsias virtualias funkcijas)
- Visi **Rule of Five** metodai ir **I/O operatoriai** iš v1.2 išlaikyti ir pritaikyti paveldėjimui
- Visi v1.2 testai **išliko galiojantys** ir papildyti abstraktumo demonstravimu

---

## Projekto struktūra

```
Zmogus.h            - abstrakti bazinė klasė (v1.5 nauja)
Zmogus.cpp          - Zmogus realizacija      (v1.5 nauja)
Studentas.h         - išvestinė klasė iš Zmogus (v1.5 atnaujinta)
Studentas.cpp       - Studentas realizacija     (v1.5 atnaujinta)
testas.h            - testų antraštinis failas
testas.cpp          - testai (v1.5: papildytas abstraktumo testu)
konteineris.h       - studentu konteineris<Studentas>
main.cpp
meniu.cpp -- meniu.h
skaitymas.cpp -- skaitymas.h
Generavimas.cpp -- Generavimas.h
spausdinam.cpp -- spausdinam.h
tyrimas.cpp -- tyrimas.h
laikai.cpp -- laikai.h
Makefile
```

---

# Sistemos parametrai

- CPU: 12th Gen Intel Core i5-12400F
- RAM: 32.0 GB (31.8 GB usable)
- Diskas: HDD
- OS: Windows 10

---

## Klasių hierarchija

```
Zmogus          (abstrakti – objektų kurti negalima)
│   # protected:
│       vardas, pavarde
│   + getVardas(), getPavarde()
│   + setVardas(), setPavarde()
│   + virtual suskaiciuotiGalutini() = 0   ← grynoji virtuali
│   + virtual spausdinti(ostream&)  = 0   ← grynoji virtuali
│   + virtual ~Zmogus()
│
└── Studentas   (išvestinė – objektų kurti galima)
        # private:
            egzaminas, nd, vidurkis, galrezVid, galrezMed
        + Rule of Five (visi 5 metodai)
        + suskaiciuotiGalutini() override
        + spausdinti(ostream&)  override
        + operator<<, operator>>
        + readStudent(), readNdInteractive(), parinktiAtsitiktinius()
```

---

## Zmogus – abstrakti bazinė klasė

`Zmogus` yra abstrakti, nes turi **grynąsias virtualias funkcijas** (`= 0`).
Tai reiškia, kad:

```cpp
Zmogus z;           // KLAIDA – cannot declare variable of abstract type
Zmogus* p = ...;   // Gerai – rodyklė į išvestinę klasę
```

### Grynosios virtualios funkcijos

| Funkcija | Paskirtis |
|---|---|
| `virtual void suskaiciuotiGalutini() = 0` | Kiekviena išvestinė klasė turi pati apskaičiuoti galutinį rezultatą pagal savo logiką |
| `virtual void spausdinti(std::ostream&) const = 0` | Kiekviena išvestinė klasė spausdina save pagal savo formatą |

### Zmogus Rule of Five

Kadangi `Zmogus` neturi dinaminių išteklių (tik `std::string`), visi specialieji metodai deklaruoti su `= default`. Virtualus destruktorius **būtinas**, nes klasė turi virtualių funkcijų.

| Metodas | Deklaracija |
|---|---|
| Default konstruktorius | `= default` |
| Kopijavimo konstruktorius | `= default` |
| Kopijavimo priskyrimo op. | `= default` |
| Perkėlimo konstruktorius | `= default` |
| Perkėlimo priskyrimo op. | `= default` |
| Destruktorius | `virtual ~Zmogus() = default` |

---

## Studentas – Rule of Five su paveldėjimu

Kiekvienas `Studentas` specialusis metodas **grandininai kviečia** atitinkamą `Zmogus` metodą, kad teisingai tvarkytų ir bazinę dalį (`vardas`, `pavarde`).

| Metodas | Kaip kviečia bazę |
|---|---|
| Kopijavimo konstruktorius | `: Zmogus(other)` |
| Kopijavimo priskyrimo op. | `Zmogus::operator=(other)` |
| Perkėlimo konstruktorius | `: Zmogus(std::move(other))` |
| Perkėlimo priskyrimo op. | `Zmogus::operator=(std::move(other))` |
| Destruktorius | `~Studentas() override = default` |

---

## Įvesties ir išvesties operatoriai

### `operator<<` – išvestis į srautą

`operator<<` kviečia virtualią `spausdinti()` funkciją – tai leidžia polimorfiškai spausdinti per `Zmogus*` rodyklę.

**Išvesties formatas:**
```
Jonas           Jonaitis       Egz: 10  ND: [8 7 9 6]  Gal.Vid: 7.60  Gal.Med: 7.60
```

---

### `operator>>` – įvestis iš srauto

Nuskaito duomenis **failo formatu**: `Vardas Pavarde ND1 ND2 ... Egzaminas`

---

## Duomenų įvestis ir išvestis – apžvalga

### Duomenų įvestis

| Būdas | Aprašymas | Kur naudojama |
|---|---|---|
| **Rankinis įvedimas** | Vartotojas klaviatūra įveda vardą, pavardę, pažymius, egzaminą | `skaitomRanka()`, meniu 1 |
| **Automatinis generavimas** | Pažymiai atsitiktinai, vardas/pavardė automatiškai | `parinktiAtsitiktinius()`, meniu 3, 4 |
| **Skaitymas iš failo** | Duomenys nuskaitomi iš tekstinio failo | `failoSkaitymas()`, meniu 2; naudoja `Studentas(istream&)` ir `operator>>` |

### Duomenų išvestis

| Būdas | Aprašymas | Kur naudojama |
|---|---|---|
| **Išvestis į ekraną** | Suformatuota lentelė | `main.cpp` → `pradetiSpausdint()` |
| **Išvestis į failą** | Lentelė įrašoma į `.txt` failą | `spausdinam()`, po skirstymo |
| **`operator<<`** | Per `spausdinti()` virtualią f-ją bet kuriam srautui | `testas.cpp`, visi naudotojai |
| **Polimorfiškai per `Zmogus*`** | `z->spausdinti(os)` | Galima naudoti su bet kuria išvestine klase |

---

## Testai (testas.cpp)

Testai paleidžiami pasirinkus **meniu pasirinikimą 7**. Galima patikrinti abstraktumą per **meniu pasirinkimą 8**.

### Testuojami metodai (v1.5)

| Testas | Tikrinama |
|---|---|
| **Zmogus abstraktumas** | `Zmogus*` rodo į `Studentas`; virtualios f-jos veikia polimorfiškai |
| Default konstruktorius | Visi laukai = 0 / tuščia |
| Srautinis konstruktorius | Teisingas nuskaitymas iš `istream` |
| Kopijavimo konstruktorius | Gili kopija; bazinė dalis kopijuojama per `Zmogus(other)` |
| Kopijavimo priskyrimo op. | Teisingas priskyrimas; saugus savipriskyrimas |
| Perkėlimo konstruktorius | Ištekliai perkelti; šaltinis „tuščias"; bazė perkelta per `Zmogus(move)` |
| Perkėlimo priskyrimo op. | Ištekliai perkelti; saugus saviperkelimas |
| Destruktorius | Nenulūžta po objekto sunaikinimo |
| `operator>>` | Teisingas nuskaitymas ir `suskaiciuotiGalutini()` |
| `operator<<` | Visi laukai išvedami teisingai |

---

# Studentų skirstymo strategijos

### Strategija 1
Iš bendro konteinerio sukuriami **du nauji** – vargšiukai ir protai.

### Strategija 2
Kuriamas tik **vargšiukų** konteineris; vargšiukai ištrinami iš bendro konteinerio, likę = protai.

### Strategija 3 (optimizuota)
Naudojamas `std::partition` – vienas perėjimas, minimalus kopijavimas.

---

# Spartos palyginimas: struct (v1.0) vs class (v1.1)

Testuota su **std::vector**, **Strategija 3**. Testavimas paleistas 10 kartų, rezultatai yra vidurkis.

### 1 000 000 įrašų – std::vector, Strategija 3

| Versija | Skaitymas (s) | Rikiavimas (s) | Skirstymas (s) | Iš viso (s) |
|---|---:|---:|---:|---:|
| v1.0 (`struct Stud`) | 1.887719 | 1.974143 | 0.425847 | 4.287709 |
| v1.1 (`class Studentas`) | 1.905067 | 1.874186 | 0.251693 | 4.030946 |

---

# Optimizavimo flagų tyrimas

### Sparta (1 000 000 įrašų, Strategija 3)

| Flag'as | Skaitymas (s) | Rikiavimas (s) | Skirstymas (s) | Iš viso (s) |
|---|---:|---:|---:|---:|
| `-O1` | 1.348002 | 0.542123 | 0.106222 | 1.996347 |
| `-O2` | 1.255266 | 0.536366 | 0.107149 | 1.898781 |
| `-O3` | 1.311384 | 0.538993 | 0.106197 | 1.958474 |

---

# Išvados

- `Zmogus` abstrakti bazinė klasė užtikrina, kad negalima sukurti „bendro žmogaus" objekto – tik konkrečias išvestines klases (pvz. `Studentas`).
- `Studentas` Rule of Five metodai kviečia `Zmogus` atitinkamus metodus – bazinė dalis (`vardas`, `pavarde`) visada tinkamai kopijuojama / perkraunama.
- `operator<<` per virtualią `spausdinti()` leidžia polimorfiškai spausdinti per `Zmogus*` rodyklę.
- Visi 64 testai praėjo – v1.2 sąsaja visiškai išlaikyta.

---

# Kompiliavimas ir paleidimas

## Pagrindiniai
```
make
```

## Su optimizavimo flagais
```
make opt
```

## Paleidimas
```
./programa_vector
./programa_list
./programa_deque
```

## Valymas
```
make clean
```
