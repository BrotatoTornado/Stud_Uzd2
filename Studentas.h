/**
 * @file Studentas.h
 * @brief Studento klasės apibrėžimas.
 *
 * Realizuoja `Zmogus` abstrakčią klasę konkretaus studento atveju.
 * Saugo namų darbų pažymius, egzamino pažymį ir skaičiuoja galutinius
 * įvertinimus pagal vidurkį bei medianą.
 *
 * @author Studentas
 * @version 2.0
 */

#pragma once
#include "Zmogus.h"

#include <iostream>
#include <string>
#include <vector>


 /**
  * @class Studentas
  * @brief Konkreti klasė, paveldinti iš Zmogus.
  *
  * Saugo ir apdoroja studento namų darbų pažymius, egzamino rezultatą,
  * skaičiuoja galutinius įvertinimus (pagal vidurkį ir medianą).
  *
  * ### Rule of Five
  * Klasė realizuoja visus penkis specialiuosius metodus:
  * - Numatytąjį konstruktorių
  * - Kopijavimo konstruktorių
  * - Kopijavimo priskyrimo operatorių
  * - Perkėlimo konstruktorių
  * - Perkėlimo priskyrimo operatorių
  * - Destruktorių (paveldi iš `Zmogus`)
  *
  * ### Galutinio balo formulė
  * @f[
  *   \text{galrezVid} = 0.4 \cdot \text{vidurkis} + 0.6 \cdot \text{egzaminas}
  * @f]
  * @f[
  *   \text{galrezMed} = 0.4 \cdot \text{mediana} + 0.6 \cdot \text{egzaminas}
  * @f]
  */

class Studentas : public Zmogus
{
private:
    int egzaminas;
    std::vector<int> nd;
    float vidurkis;
    float galrezVid;
    float galrezMed;

public:
    /** @name Rule of Five
     *  @{
     */

     /**
      * @brief Numatytasis konstruktorius.
      *
      * Inicializuoja visus skaičinius laukus į 0, o vektorių – į tuščią.
      */
    Studentas();                                // Default konstruktorius

    /**
     * @brief Srautinis konstruktorius.
     *
     * Nuskaito studento duomenis iš nurodyto įvesties srauto
     * (pvz. failo eilutės pateiktos kaip `std::istringstream`).
     *
     * @param is Įvesties srautas.
     */
    explicit Studentas(std::istream& is);       // stream konstruktorius

    /**
     * @brief Kopijavimo konstruktorius.
     *
     * Sukuria gilią kopiją kito `Studentas` objekto.
     *
     * @param other Kopijuojamas objektas.
     */
    Studentas(const Studentas& other);          // Kopijavimo konstruktorius

    /**
     * @brief Kopijavimo priskyrimo operatorius.
     *
     * Nukopijuoja visus duomenis iš `other` į šį objektą.
     * Saugiai tvarko savipriskyrimo atvejį.
     *
     * @param other Priskiriamas objektas.
     * @return Nuoroda į šį objektą (`*this`).
     */
    Studentas& operator=(const Studentas& other); // Kopijavimo priskyrimo operatorius

    /**
     * @brief Perkėlimo konstruktorius.
     *
     * Perkelia resursus iš `other` į šį objektą.
     * Po perkėlimo `other` lieka tuščios būsenos.
     *
     * @param other Perkeliamas objektas (rvalue nuoroda).
     */
    Studentas(Studentas&& other);        // Perkėlimo konstruktorius

    /**
     * @brief Perkėlimo priskyrimo operatorius.
     *
     * Perkelia resursus iš `other` į šį objektą priskyrimo metu.
     * Saugiai tvarko savipriskyrimo atvejį.
     *
     * @param other Perkeliamas objektas (rvalue nuoroda).
     * @return Nuoroda į šį objektą (`*this`).
     */
    Studentas& operator=(Studentas&& other); // Perkėlimo priskyrimo operatorius

    /**
     * @brief Destruktorius.
     *
     * Paveldi iš `Zmogus`. Automatiškai atlaisvina `nd` vektoriaus atmintį.
     */
    ~Studentas() override = default;        // Destruktorius

    /**
     * @brief Grąžina namų darbų pažymių vektorių (tik skaitymui).
     * @return Konstantinė nuoroda į `nd` vektorių.
     */
    inline const std::vector<int>& getND() const { return nd; }

    /**
     * @brief Grąžina egzamino pažymį.
     * @return Egzamino pažymys (1–10).
     */
    inline int getEgzaminas() const { return egzaminas; }

    /**
     * @brief Grąžina namų darbų pažymių vidurkį.
     * @return Vidurkis kaip `float`.
     */
    inline float getVidurkis() const { return vidurkis; }

    /**
     * @brief Grąžina galutinį balą apskaičiuotą pagal vidurkį.
     * @return Galutinis balas (vidurkio variantas).
     */
    inline float getgalrezVid() const { return galrezVid; }

    /**
     * @brief Grąžina galutinį balą apskaičiuotą pagal medianą.
     * @return Galutinis balas (medianos variantas).
     */
    inline float getgalrezMed() const { return galrezMed; }

    // -------------------------------------------------------------------------
    /** @name Keitimo metodai (setters)
     *  @{
     */

    //@brief rankinio įvedimo atvejui
    /**
     * @brief Nustato studento vardą.
     * @param v Naujas vardas.
     */
    void setVardas(const std::string& v) { vardas = v; }

    /**
     * @brief Nustato studento pavardę.
     * @param p Nauja pavardė.
     */
    void setPavarde(const std::string& p) { pavarde = p; }

    /**
     * @brief Nustato egzamino pažymį.
     * @param e Egzamino pažymys (rekomenduojama 1–10).
     */
    void setEgzaminas(int e) { egzaminas = e; }

    /** @} */

    // -------------------------------------------------------------------------
    /** @name Funkcijos
     *  @{
     */

     /**
      * @brief Nuskaito studento duomenis iš srauto.
      *
      * Formatas: `Vardas Pavardė ND1 ND2 ... NDn Egzaminas`
      * Paskutinis skaičius laikomas egzamino pažymiu.
      *
      * @param is Įvesties srautas.
      * @return Nuoroda į srautą (grandininiam kvietimui).
      */
    std::istream& readStudent(std::istream& is); // skaito vardas/pavardė/nd/egz iš stream

    /**
     * @brief Interaktyviai įveda namų darbų pažymius iš konsolės.
     *
     * Prašo vartotojo įvesti pažymius vieną po kito.
     * Įvedimas baigiamas naudojant 0 arba neigiamą skaičių.
     *
     * @throws std::runtime_error Jei nepavyksta nuskaityti nė vieno pažymio.
     */
    void readNdInteractive();

    /**
     * @brief Sugeneruoja atsitiktinius namų darbų ir egzamino pažymius.
     *
     * Naudoja `std::mt19937` generatorių su `std::random_device` sėkla.
     * Pažymiai generuojami intervale [1, 10].
     */
    void parinktiAtsitiktinius();

    /**
     * @brief Apskaičiuoja galutinį įvertinimą (vidurkio ir medianos variantus).
     *
     * Realizuoja grynąją virtualią funkciją iš `Zmogus`.
     *
     * Formulė:
     * - galrezVid = 0.4 * vidurkis + 0.6 * egzaminas
     * - galrezMed = 0.4 * mediana  + 0.6 * egzaminas
     */
    void suskaiciuotiGalutini() override;

    /**
     * @brief Išveda studento duomenis į nurodytą srautą.
     *
     * Realizuoja grynąją virtualią funkciją iš `Zmogus`.
     *
     * @param os Išvesties srautas.
     */
    void spausdinti(std::ostream& os) const override;

    /** @} */
};

// -----------------------------------------------------------------------------
/** @name Laisvosios funkcijos
 *  @relates Studentas
 *  @{
 */

 /**
  * @brief Išvesties operatorius srautui.
  *
  * Delegacija į `Studentas::spausdinti()`.
  *
  * @param os Išvesties srautas.
  * @param s  Studento objektas.
  * @return Nuoroda į srautą.
  */
std::ostream& operator<<(std::ostream& os, const Studentas& s);

/**
 * @brief Įvesties operatorius srautui.
 *
 * Nuskaito studento duomenis ir iškarto apskaičiuoja galutinį įvertinimą.
 *
 * @param is Įvesties srautas.
 * @param s  Studento objektas, į kurį rašoma.
 * @return Nuoroda į srautą.
 */
std::istream& operator>>(std::istream& is, Studentas& s);

/**
 * @brief Lyginimo funkcija rikiavimui pagal vardą.
 * @param a Pirmas studentas.
 * @param b Antras studentas.
 * @return `true` jei `a.vardas < b.vardas`.
 */
bool comparePagalVarda   (const Studentas& a, const Studentas& b);

/**
 * @brief Lyginimo funkcija rikiavimui pagal pavardę.
 * @param a Pirmas studentas.
 * @param b Antras studentas.
 * @return `true` jei `a.pavarde < b.pavarde`.
 */
bool comparePagalPavarde (const Studentas& a, const Studentas& b);

/**
 * @brief Lyginimo funkcija rikiavimui pagal galutinį vidurkio balą.
 * @param a Pirmas studentas.
 * @param b Antras studentas.
 * @return `true` jei `a.galrezVid < b.galrezVid`.
 */
bool comparePagalGalVid  (const Studentas& a, const Studentas& b);

/**
 * @brief Lyginimo funkcija rikiavimui pagal galutinį medianos balą.
 * @param a Pirmas studentas.
 * @param b Antras studentas.
 * @return `true` jei `a.galrezMed < b.galrezMed`.
 */
bool comparePagalGalMed  (const Studentas& a, const Studentas& b);
/** @} */
