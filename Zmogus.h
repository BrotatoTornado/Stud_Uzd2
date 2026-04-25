#pragma once

#include <iostream>
#include <string>


class Zmogus
{
protected:
    std::string vardas;
    std::string pavarde;

public:
    // --- Rule of Five (bazinė klasė) ---
    Zmogus() = default;
    Zmogus(const std::string& v, const std::string& p);
    Zmogus(const Zmogus&) = default;
    Zmogus& operator=(const Zmogus&) = default;
    Zmogus(Zmogus&&) = default;
    Zmogus& operator=(Zmogus&&) = default;
    virtual ~Zmogus() = default;


    virtual std::string getVardas()  const { return vardas;  }
    virtual std::string getPavarde() const { return pavarde; }

    void setVardas  (const std::string& v) { vardas = v; }
    void setPavarde (const std::string& p) { pavarde = p; }


    virtual void suskaiciuotiGalutini() = 0;
    virtual void spausdinti(std::ostream& os) const = 0;
};
