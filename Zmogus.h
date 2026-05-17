/**
 * @file Zmogus.h
 * @brief Abstrakti bazinė klasė žmogui.
 *
 * Apibrėžia bendrą sąsają visoms žmogaus tipo klasėms.
 * Negali būti tiesiogiai instantijuojama, nes turi grynąsias
 * virtualias funkcijas.
 *
 * @author Studentas
 * @version 3.0
 */

#pragma once

#include <iostream>
#include <string>

 /**
  * @class Zmogus
  * @brief Abstrakti bazinė klasė, apibrėžianti žmogaus sąsają.
  *
  * Saugo vardą ir pavardę, bei deklaruoja grynąsias virtualias
  * funkcijas, kurias privalo realizuoti išvestinės klasės.
  * Realizuoja Rule of Five naudojant `= default` direktyvą.
  */

class Zmogus
{
protected:
    std::string vardas;
    std::string pavarde;

public:
    /** @name Rule of Five
     *  @{
     */

     /** @brief Numatytasis konstruktorius. Inicializuoja tuščius laukus. */
    Zmogus() = default;

    /**
     * @brief Konstruktorius su parametrais.
     * @param v Vardas.
     * @param p Pavardė.
     */
    Zmogus(const std::string& v, const std::string& p);

    /** @brief Kopijavimo konstruktorius. */
    Zmogus(const Zmogus&) = default;

    /** @brief Kopijavimo priskyrimo operatorius. */
    Zmogus& operator=(const Zmogus&) = default;

    /** @brief Perkėlimo konstruktorius. */
    Zmogus(Zmogus&&) = default;

    /** @brief Perkėlimo priskyrimo operatorius. */
    Zmogus& operator=(Zmogus&&) = default;

    /**
     * @brief Virtualus destruktorius.
     *
     * Užtikrina teisingą išvestinių klasių sunaikinimą per bazinės
     * klasės rodyklę.
     */
    virtual ~Zmogus() = default;


    virtual std::string getVardas()  const { return vardas;  }
    virtual std::string getPavarde() const { return pavarde; }

    void setVardas  (const std::string& v) { vardas = v; }
    void setPavarde (const std::string& p) { pavarde = p; }

    // -------------------------------------------------------------------------
    /** @name Grynosios virtualios funkcijos
     *  Išvestinės klasės privalo šias funkcijas realizuoti.
     *  @{
     */

     /**
      * @brief Apskaičiuoja galutinį įvertinimą.
      *
      * Konkretus skaičiavimo algoritmas priklauso nuo išvestinės klasės.
      */
    virtual void suskaiciuotiGalutini() = 0;

    /**
     * @brief Išspausdina objekto duomenis į srautą.
     * @param os Išvesties srautas.
     */
    virtual void spausdinti(std::ostream& os) const = 0;
    /** @} */
};
