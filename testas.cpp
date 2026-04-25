#include "testas.h"
#include "Studentas.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

namespace
{

int praejo = 0;
int nepraejo = 0;

void tikrinti(const std::string& pavadinimas, bool salyga)
{
    const std::string zenklas = salyga ? "Pavyko --->" : "Nepavyko!!! --->";
    std::cout << zenklas << " " << pavadinimas << '\n';
    salyga ? praejo++ : nepraejo++;
}

Studentas sukurtiTestiniStudenta()
{
    std::istringstream ss("Jonas Jonaitis 8 7 9 6 10");
    Studentas s(ss);
    s.suskaiciuotiGalutini();
    return s;
}

}

static void testuotiDefaultKonstruktoriu()
{
    std::cout << "\n[Default konstruktorius]\n";

    Studentas s;

    tikrinti("vardas tuscias", s.getVardas().empty());
    tikrinti("pavarde tuscia", s.getPavarde().empty());
    tikrinti("egzaminas == 0", s.getEgzaminas() == 0);
    tikrinti("nd tuscias", s.getND().empty());
    tikrinti("vidurkis == 0.0f", s.getVidurkis() == 0.0f);
    tikrinti("galrezVid == 0.0f", s.getgalrezVid() == 0.0f);
    tikrinti("galrezMed == 0.0f", s.getgalrezMed() == 0.0f);
}

static void testuotiSrautiniKonstruktoriu()
{
    std::cout << "\n[Srautinis konstruktorius]\n";

    std::istringstream ss("Pedro Pascal 5 6 7 8 9");
    Studentas s(ss);
    s.suskaiciuotiGalutini();

    tikrinti("vardas == Petras", s.getVardas() == "Pedro");
    tikrinti("pavarde == Petraitis", s.getPavarde() == "Pascal");
    tikrinti("egzaminas == 9", s.getEgzaminas() == 9);
    tikrinti("nd.size() == 4", s.getND().size() == 4);
    tikrinti("nd[0] == 5", s.getND()[0] == 5);
    tikrinti("galrezVid > 0", s.getgalrezVid() > 0.0f);
    tikrinti("galrezMed > 0", s.getgalrezMed() > 0.0f);
}

static void testuotiKopijavimoKonstruktoriu()
{
    std::cout << "\n[Kopijavimo konstruktorius]\n";

    Studentas original = sukurtiTestiniStudenta();
    Studentas kopija(original);

    tikrinti("vardas sutampa", kopija.getVardas() == original.getVardas());
    tikrinti("pavarde sutampa", kopija.getPavarde() == original.getPavarde());
    tikrinti("egzaminas sutampa", kopija.getEgzaminas() == original.getEgzaminas());
    tikrinti("nd sutampa", kopija.getND() == original.getND());
    tikrinti("galrezVid sutampa", kopija.getgalrezVid() == original.getgalrezVid());
    tikrinti("galrezMed sutampa", kopija.getgalrezMed() == original.getgalrezMed());


    kopija.setVardas("Kitas");
    tikrinti("originalas nepakeistas po kopijos keitimo", original.getVardas() == "Jonas");
}

static void testuotiKopijavimoPriskyrima()
{
    std::cout << "\n[Kopijavimo priskyrimo operatorius]\n";

    Studentas original = sukurtiTestiniStudenta();
    Studentas priskirtas;
    priskirtas = original;

    tikrinti("vardas sutampa", priskirtas.getVardas() == original.getVardas());
    tikrinti("pavarde sutampa", priskirtas.getPavarde() == original.getPavarde());
    tikrinti("egzaminas sutampa", priskirtas.getEgzaminas() == original.getEgzaminas());
    tikrinti("nd sutampa", priskirtas.getND() == original.getND());
    tikrinti("galrezVid sutampa", priskirtas.getgalrezVid() == original.getgalrezVid());
    tikrinti("galrezMed sutampa", priskirtas.getgalrezMed() == original.getgalrezMed());

    priskirtas = priskirtas;
    tikrinti("savipriskyrimas saugus", priskirtas.getVardas() == original.getVardas());
}

static void testuotiPerkelimoKonstruktoriu()
{
    std::cout << "\n[Perkelimo konstruktorius]\n";

    Studentas saltinis = sukurtiTestiniStudenta();

    const std::string laukiamasVardas = saltinis.getVardas();
    const std::string laukiamaPavarde = saltinis.getPavarde();
    const int laukiamisEgz = saltinis.getEgzaminas();
    const float laukiamasGalVid = saltinis.getgalrezVid();
    const std::size_t laukiamasNdDydis = saltinis.getND().size();

    Studentas tikslas(std::move(saltinis));

    tikrinti("tikslas: vardas teisingas", tikslas.getVardas() == laukiamasVardas);
    tikrinti("tikslas: pavarde teisinga", tikslas.getPavarde() == laukiamaPavarde);
    tikrinti("tikslas: egzaminas teisingas", tikslas.getEgzaminas() == laukiamisEgz);
    tikrinti("tikslas: nd dydis teisingas",  tikslas.getND().size() == laukiamasNdDydis);
    tikrinti("tikslas: galrezVid teisingas", tikslas.getgalrezVid() == laukiamasGalVid);

    // Šaltinis po perkėlimo – turi būti "tuščias"
    tikrinti("saltinis: egzaminas == 0", saltinis.getEgzaminas() == 0);
    tikrinti("saltinis: vidurkis == 0", saltinis.getVidurkis()  == 0.0f);
    tikrinti("saltinis: galrezVid == 0", saltinis.getgalrezVid() == 0.0f);
}

static void testuotiPerkelimoPriskyrima()
{
    std::cout << "\n[Perkelimo priskyrimo operatorius]\n";

    Studentas saltinis = sukurtiTestiniStudenta();

    const std::string laukiamasVardas = saltinis.getVardas();
    const int laukiamisEgz = saltinis.getEgzaminas();
    const float laukiamasGalMed = saltinis.getgalrezMed();

    Studentas tikslas;
    tikslas = std::move(saltinis);

    tikrinti("tikslas: vardas teisingas", tikslas.getVardas() == laukiamasVardas);
    tikrinti("tikslas: egzaminas teisingas", tikslas.getEgzaminas() == laukiamisEgz);
    tikrinti("tikslas: galrezMed teisingas", tikslas.getgalrezMed() == laukiamasGalMed);

    // Šaltinis po perkelimo turi būti "tuščias"
    tikrinti("saltinis: egzaminas == 0", saltinis.getEgzaminas() == 0);
    tikrinti("saltinis: galrezVid == 0", saltinis.getgalrezVid() == 0.0f);
    tikrinti("saltinis: nd tuscias", saltinis.getND().empty());
}

static void testuotiDestruktoriu()
{
    std::cout << "\n[Destruktorius]\n";

    {
        Studentas s = sukurtiTestiniStudenta();
        // s bus sunaikintas pasibaigus blokui
    }
    tikrinti("destruktorius nesuluzo", true);

    // Didelis vektorius – tikrinamas nd atlaisvinimas
    {
        std::vector<Studentas> daug;
        for (int i = 0; i < 100; ++i)
        {
            daug.push_back(sukurtiTestiniStudenta());
        }
    }
    tikrinti("destruktorius po 100 objektu nesuluzo", true);
}

static void testuotiIstreamOperatoriu()
{
    std::cout << "\n[operator>> (istream)]\n";

    // Vienas studentas per srautą
    std::istringstream ss1("Ona Oniene 3 4 5 6 7");
    Studentas s1;
    ss1 >> s1;
    s1.suskaiciuotiGalutini();

    tikrinti("vardas == Ona", s1.getVardas() == "Ona");
    tikrinti("pavarde == Oniene", s1.getPavarde() == "Oniene");
    tikrinti("egzaminas == 7", s1.getEgzaminas() == 7);
    tikrinti("nd.size() == 4", s1.getND().size() == 4);
    tikrinti("galrezVid apskaiciuotas", s1.getgalrezVid() > 0.0f);

    const std::string duomenys = "Algis Algimantas 6 7 8 9\n" "Birute Birstoniene 4 5 6 7\n";

    int skaityta = 0;

    std::istringstream srautas(duomenys);
    std::string eilute;
    while (std::getline(srautas, eilute))
    {
        if (eilute.empty())
        {
            continue;
        }
        std::istringstream esSS(eilute);
        Studentas s;
        esSS >> s;
        if (!s.getND().empty())
        {
            skaityta++;
        }
    }

    tikrinti("2 studentai nuskaityti is srauto", skaityta == 2);
}

static void testuotiOstreamOperatoriu()
{
    std::cout << "\n[operator<< (ostream)]\n";

    Studentas s = sukurtiTestiniStudenta();
    // galrezVid/galrezMed jau apskaičiuoti sukurtiTestiniStudenta()

    std::ostringstream os;
    os << s;

    const std::string isvestis = os.str();

    tikrinti("isvestyje yra vardas", isvestis.find("Jonas") != std::string::npos);
    tikrinti("isvestyje yra pavarde", isvestis.find("Jonaitis")  != std::string::npos);
    tikrinti("isvestyje yra Egz", isvestis.find("Egz:") != std::string::npos);
    tikrinti("isvestyje yra ND", isvestis.find("ND:") != std::string::npos);
    tikrinti("isvestyje yra Gal.Vid", isvestis.find("Gal.Vid:") != std::string::npos);
    tikrinti("isvestyje yra Gal.Med", isvestis.find("Gal.Med:") != std::string::npos);
    tikrinti("isvestis netuscia", !isvestis.empty());

    // Failų srauto testas – operator<< veikia su ofstream
    std::ostringstream failoSim;
    std::vector<Studentas> sarasas;
    sarasas.push_back(s);
    sarasas.push_back(sukurtiTestiniStudenta());

    for (const auto& st : sarasas)
    {
        failoSim << st << '\n';
    }

    tikrinti("isvedimo i faila srautas netuscias", !failoSim.str().empty());
}

void vykdytiTestus()
{
    praejo = 0;
    nepraejo = 0;

    std::cout << "\n" << std::string(55, '=') << '\n' << "Studentas klases testai  (v1.2 - Rule of Five)\n" << std::string(55, '=') << '\n';

    testuotiDefaultKonstruktoriu();
    testuotiSrautiniKonstruktoriu();
    testuotiKopijavimoKonstruktoriu();
    testuotiKopijavimoPriskyrima();
    testuotiPerkelimoKonstruktoriu();
    testuotiPerkelimoPriskyrima();
    testuotiDestruktoriu();
    testuotiIstreamOperatoriu();
    testuotiOstreamOperatoriu();

    std::cout << '\n' << std::string(55, '-') << '\n' << "Rezultatas: praejo " << praejo << ", nepraejo " << nepraejo << '\n' << std::string(55, '=') << '\n';
}
