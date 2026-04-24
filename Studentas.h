#pragma once

#include <iostream>
#include <string>
#include <vector>


class Studentas
{
private:
    std::string vardas;
    std::string pavarde;
    int egzaminas;
    std::vector<int> nd;
    float vidurkis;
    float galrezVid;
    float galrezMed;

public:
    //Rule Of Five
    Studentas();                                // Default konstruktorius
    explicit Studentas(std::istream& is);       // stream konstruktorius
    Studentas(const Studentas& other);          // Kopijavimo konstruktorius
    Studentas& operator=(const Studentas& other); // Kopijavimo priskyrimo operatorius
    Studentas(Studentas&& other);        // Perkėlimo konstruktorius
    Studentas& operator=(Studentas&& other); // Perkėlimo priskyrimo operatorius
    ~Studentas();                               // Destruktorius

    inline const std::vector<int>& getND() const { return nd; }

    inline const std::string getVardas() const { return vardas; }
    inline const std::string getPavarde() const { return pavarde; }
    inline int getEgzaminas() const { return egzaminas; }

    inline float getVidurkis() const { return vidurkis; }
    inline float getgalrezVid() const { return galrezVid; }
    inline float getgalrezMed() const { return galrezMed; }

    // rankinio įvedimo atvejui
    void setVardas(const std::string& v) { vardas = v; }
    void setPavarde(const std::string& p) { pavarde = p; }
    void setEgzaminas(int e) { egzaminas = e; }

    // --- member funkcijos ---
    std::istream& readStudent(std::istream& is); // skaito vardas/pavardė/nd/egz iš stream
    void readNdInteractive();
    void parinktiAtsitiktinius();
    void suskaiciuotiGalutini();
};

//Ivesties/Išvesties operatoriai
std::ostream& operator<<(std::ostream& os, const Studentas& s);
std::istream& operator>>(std::istream& is, Studentas& s);

bool comparePagalVarda   (const Studentas& a, const Studentas& b);
bool comparePagalPavarde (const Studentas& a, const Studentas& b);
bool comparePagalGalVid  (const Studentas& a, const Studentas& b);
bool comparePagalGalMed  (const Studentas& a, const Studentas& b);
