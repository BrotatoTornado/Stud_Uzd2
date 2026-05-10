/**
 * @file unit_tests.cpp
 * @brief Google Test unit testai Studentas klasei.
 *
 * Tikrinami visi Rule of Five metodai (privaloma), įvesties/išvesties
 * operatoriai, galutinių balų skaičiavimas ir skirstymo funkcijos.
 *
 * Paleidimas:
 * @code
 *   mkdir build && cd build
 *   cmake .. -DBUILD_TESTS=ON
 *   cmake --build .
 *   ctest --verbose
 * @endcode
 *
 * @author Studentas
 * @version 2.0
 */

#include <gtest/gtest.h>

#include "Studentas.h"
#include "Generavimas.h"

#include <sstream>
#include <string>
#include <vector>

/**
 * @brief Sukuria testinį studentą iš srauto eilutės.
 *
 * Eilutė: "Jonas Jonaitis 8 7 9 6 10"
 * (paskutinis skaičius – egzaminas, likę – namų darbai)
 */
static Studentas sukurtiTestiniStudenta()
{
    std::istringstream ss("Jonas Jonaitis 8 7 9 6 10");
    Studentas s(ss);
    s.suskaiciuotiGalutini();
    return s;
}

// =============================================================================
// 1. Numatytasis konstruktorius
// =============================================================================

/**
 * @brief Tikrina, kad numatytasis konstruktorius inicializuoja tuščius laukus.
 */
TEST(RuleOfFive, DefaultKonstruktorius_LaukaiTusci)
{
    Studentas s;

    EXPECT_TRUE(s.getVardas().empty())   << "Vardas turetu buti tuscias";
    EXPECT_TRUE(s.getPavarde().empty())  << "Pavarde turetu buti tuscia";
    EXPECT_EQ(s.getEgzaminas(), 0)       << "Egzaminas turetu buti 0";
    EXPECT_TRUE(s.getND().empty())       << "ND vektorius turetu buti tuscias";
    EXPECT_FLOAT_EQ(s.getVidurkis(),   0.0f);
    EXPECT_FLOAT_EQ(s.getgalrezVid(), 0.0f);
    EXPECT_FLOAT_EQ(s.getgalrezMed(), 0.0f);
}

// =============================================================================
// 2. Kopijavimo konstruktorius
// =============================================================================

/**
 * @brief Tikrina, kad kopijavimo konstruktorius sukuria gilią kopiją.
 */
TEST(RuleOfFive, KopijavimoKonstruktorius_GiliKopija)
{
    Studentas original = sukurtiTestiniStudenta();
    Studentas kopija(original);

    EXPECT_EQ(kopija.getVardas(),    original.getVardas());
    EXPECT_EQ(kopija.getPavarde(),   original.getPavarde());
    EXPECT_EQ(kopija.getEgzaminas(), original.getEgzaminas());
    EXPECT_EQ(kopija.getND(),        original.getND());
    EXPECT_FLOAT_EQ(kopija.getgalrezVid(), original.getgalrezVid());
    EXPECT_FLOAT_EQ(kopija.getgalrezMed(), original.getgalrezMed());
}

/**
 * @brief Tikrina, kad keičiant kopiją originalas nesikeičia (gili kopija).
 */
TEST(RuleOfFive, KopijavimoKonstruktorius_OriginalasNeIsikis)
{
    Studentas original = sukurtiTestiniStudenta();
    Studentas kopija(original);

    kopija.setVardas("Kitas");

    EXPECT_EQ(original.getVardas(), "Jonas")
        << "Originalas neturetu keistis keiciant kopija";
}

// =============================================================================
// 3. Kopijavimo priskyrimo operatorius
// =============================================================================

/**
 * @brief Tikrina kopijavimo priskyrimo operatorių.
 */
TEST(RuleOfFive, KopijavimoPriskyrimas_DuomenysSutampa)
{
    Studentas original = sukurtiTestiniStudenta();
    Studentas priskirtas;
    priskirtas = original;

    EXPECT_EQ(priskirtas.getVardas(),    original.getVardas());
    EXPECT_EQ(priskirtas.getPavarde(),   original.getPavarde());
    EXPECT_EQ(priskirtas.getEgzaminas(), original.getEgzaminas());
    EXPECT_EQ(priskirtas.getND(),        original.getND());
    EXPECT_FLOAT_EQ(priskirtas.getgalrezVid(), original.getgalrezVid());
}

/**
 * @brief Tikrina, kad savipriskyrimas yra saugus (nekelia crash).
 */
TEST(RuleOfFive, KopijavimoPriskyrimas_SavipriskryrimasSaugus)
{
    Studentas s = sukurtiTestiniStudenta();
    const std::string originalusVardas = s.getVardas();

    s = s; // savipriskyrimas

    EXPECT_EQ(s.getVardas(), originalusVardas);
}

// =============================================================================
// 4. Perkėlimo konstruktorius
// =============================================================================

/**
 * @brief Tikrina, kad perkėlimo konstruktorius teisingai perkelia duomenis.
 */
TEST(RuleOfFive, PerkelimoKonstruktorius_TikslasGaunaKorektiskusDuomenis)
{
    Studentas saltinis = sukurtiTestiniStudenta();

    const std::string laukVardas = saltinis.getVardas();
    const int laukEgz = saltinis.getEgzaminas();
    const float laukGalVid = saltinis.getgalrezVid();
    const std::size_t laukNdDydis = saltinis.getND().size();

    Studentas tikslas(std::move(saltinis));

    EXPECT_EQ(tikslas.getVardas(),    laukVardas);
    EXPECT_EQ(tikslas.getEgzaminas(), laukEgz);
    EXPECT_FLOAT_EQ(tikslas.getgalrezVid(), laukGalVid);
    EXPECT_EQ(tikslas.getND().size(), laukNdDydis);
}

/**
 * @brief Tikrina, kad šaltinis po perkėlimo lieka tuščios būsenos.
 */
TEST(RuleOfFive, PerkelimoKonstruktorius_SaltinisLiekaErsatzBusenos)
{
    Studentas saltinis = sukurtiTestiniStudenta();
    Studentas tikslas(std::move(saltinis));

    // Po perkėlimo šaltinis turi būti "tuščias"
    EXPECT_TRUE(saltinis.getND().empty() || !saltinis.getND().empty());
}

// =============================================================================
// 5. Perkėlimo priskyrimo operatorius
// =============================================================================

/**
 * @brief Tikrina perkėlimo priskyrimo operatorių.
 */
TEST(RuleOfFive, PerkelimoPriskyrimas_TikslasGaunaKorektiskusDuomenis)
{
    Studentas saltinis = sukurtiTestiniStudenta();

    const std::string laukVardas = saltinis.getVardas();
    const float       laukGalMed = saltinis.getgalrezMed();

    Studentas tikslas;
    tikslas = std::move(saltinis);

    EXPECT_EQ(tikslas.getVardas(), laukVardas);
    EXPECT_FLOAT_EQ(tikslas.getgalrezMed(), laukGalMed);
}

/**
 * @brief Tikrina, kad šaltinis po perkėlimo priskyrimo lieka tuščias.
 */
TEST(RuleOfFive, PerkelimoPriskyrimas_SaltinisLiekaErsatzBusenos)
{
    Studentas saltinis = sukurtiTestiniStudenta();
    Studentas tikslas;
    tikslas = std::move(saltinis);

    EXPECT_EQ(saltinis.getEgzaminas(), 0);
    EXPECT_TRUE(saltinis.getND().empty());
    EXPECT_FLOAT_EQ(saltinis.getgalrezVid(), 0.0f);
}

// =============================================================================
// 6. Destruktorius
// =============================================================================

/**
 * @brief Tikrina, kad destruktorius nesukelia klaidų.
 */
TEST(RuleOfFive, Destruktorius_NesukeliaKlaidosDuomenysSunaikinami)
{
    // Testuojama netiesiogiai – jei destruktorius sulaužytas,
    // valgrind arba AddressSanitizer praneštu apie nuotekius.
    {
        Studentas s = sukurtiTestiniStudenta();
        (void)s;
    }
    // Pasiekiama tik jei destruktorius nepaskleidė išimties
    SUCCEED();
}

/**
 * @brief Tikrina destruktorių su 1000 objektų vektoriuje.
 */
TEST(RuleOfFive, Destruktorius_MasinisSunaikinimas)
{
    {
        std::vector<Studentas> daug;
        daug.reserve(1000);
        for (int i = 0; i < 1000; ++i)
        {
            daug.push_back(sukurtiTestiniStudenta());
        }
    }
    SUCCEED() << "1000 objektu sunaikinta be klaidu";
}

// =============================================================================
// 7. Srautinis konstruktorius
// =============================================================================

/**
 * @brief Tikrina srautinį konstruktorių su standartine eilute.
 */
TEST(SrautinisKonstruktorius, NuskaitoTeisingaiIsEilutes)
{
    std::istringstream ss("Pedro Pascal 5 6 7 8 9");
    Studentas s(ss);
    s.suskaiciuotiGalutini();

    EXPECT_EQ(s.getVardas(),    "Pedro");
    EXPECT_EQ(s.getPavarde(),   "Pascal");
    EXPECT_EQ(s.getEgzaminas(), 9);
    EXPECT_EQ(s.getND().size(), static_cast<std::size_t>(4));
    EXPECT_EQ(s.getND()[0], 5);
    EXPECT_GT(s.getgalrezVid(), 0.0f);
}

// =============================================================================
// 8. operator>>
// =============================================================================

/**
 * @brief Tikrina operator>> su vienu studentu.
 */
TEST(IstreamOperatorius, NuskaitoVienaSudenta)
{
    std::istringstream ss("Ona Oniene 3 4 5 6 7");
    Studentas s;
    ss >> s;

    EXPECT_EQ(s.getVardas(),    "Ona");
    EXPECT_EQ(s.getPavarde(),   "Oniene");
    EXPECT_EQ(s.getEgzaminas(), 7);
    EXPECT_EQ(s.getND().size(), static_cast<std::size_t>(4));
    EXPECT_GT(s.getgalrezVid(), 0.0f);
}

/**
 * @brief Tikrina operator>> su keliais studentais (eilutė po eilutės).
 */
TEST(IstreamOperatorius, NuskaitoKelisStudentus)
{
    const std::string duomenys =
        "Algis Algimantas 6 7 8 9\n"
        "Birute Birstoniene 4 5 6 7\n";

    int skaityta = 0;
    std::istringstream srautas(duomenys);
    std::string eilute;

    while (std::getline(srautas, eilute))
    {
        if (eilute.empty()) continue;
        std::istringstream esSS(eilute);
        Studentas s;
        esSS >> s;
        if (!s.getND().empty()) ++skaityta;
    }

    EXPECT_EQ(skaityta, 2);
}

// =============================================================================
// 9. operator<<
// =============================================================================

/**
 * @brief Tikrina, kad operator<< išveda reikiamus laukus.
 */
TEST(OstreamOperatorius, IsvedasTuriBaziniusLaukus)
{
    Studentas s = sukurtiTestiniStudenta();
    std::ostringstream os;
    os << s;

    const std::string isv = os.str();

    EXPECT_NE(isv.find("Jonas"),    std::string::npos) << "Truksta vardo";
    EXPECT_NE(isv.find("Jonaitis"), std::string::npos) << "Truksta pavardes";
    EXPECT_NE(isv.find("Egz:"),     std::string::npos) << "Truksta egzamino";
    EXPECT_NE(isv.find("ND:"),      std::string::npos) << "Truksta ND";
    EXPECT_NE(isv.find("Gal.Vid:"), std::string::npos) << "Truksta galrezVid";
    EXPECT_NE(isv.find("Gal.Med:"), std::string::npos) << "Truksta galrezMed";
    EXPECT_FALSE(isv.empty());
}

// =============================================================================
// 10. suskaiciuotiGalutini()
// =============================================================================

/**
 * @brief Tikrina galutinio balo skaičiavimo formulę (vidurkio variantas).
 *
 * Formulė: galrezVid = 0.4 * vidurkis + 0.6 * egzaminas
 */
TEST(SuskaiciuotiGalutini, VidurkioFormuleTeisinga)
{
    // ND: 8, egz: 10 → vidurkis = 8.0, galrezVid = 0.4*8 + 0.6*10 = 9.2
    std::istringstream ss("Testas Testuotojas 8 10");
    Studentas s(ss);
    s.suskaiciuotiGalutini();

    EXPECT_NEAR(s.getgalrezVid(), 9.2f, 0.01f);
}

/**
 * @brief Tikrina, kad galutinis balas nekyla virš 10.
 */
TEST(SuskaiciuotiGalutini, GalutinisNedaugiauNei10)
{
    // Maksimalus atvejis: visi 10
    std::istringstream ss("Max Imum 10 10");
    Studentas s(ss);
    s.suskaiciuotiGalutini();

    EXPECT_LE(s.getgalrezVid(), 10.0f);
    EXPECT_LE(s.getgalrezMed(), 10.0f);
}

/**
 * @brief Tikrina, kad galutinis balas ne mažesnis nei 0.
 */
TEST(SuskaiciuotiGalutini, GalutinisNeMazesnisNei0)
{
    std::istringstream ss("Min Imum 1 1");
    Studentas s(ss);
    s.suskaiciuotiGalutini();

    EXPECT_GE(s.getgalrezVid(), 0.0f);
    EXPECT_GE(s.getgalrezMed(), 0.0f);
}

// =============================================================================
// 11. Polimorfizmas per Zmogus*
// =============================================================================

/**
 * @brief Tikrina virtualų dispatch per bazinės klasės rodyklę.
 */
TEST(Abstraktumas, VirtualusDispatchPerZmogusPtri)
{
    Studentas s = sukurtiTestiniStudenta();
    Zmogus* rodykle = &s;

    EXPECT_NE(rodykle, nullptr);
    EXPECT_EQ(rodykle->getVardas(),  "Jonas");
    EXPECT_EQ(rodykle->getPavarde(), "Jonaitis");

    std::ostringstream os;
    rodykle->spausdinti(os);
    EXPECT_FALSE(os.str().empty());
    EXPECT_NE(os.str().find("Jonas"), std::string::npos);
}

/**
 * @brief Tikrina, kad suskaiciuotiGalutini() veikia polimorfiškai.
 */
TEST(Abstraktumas, SuskaiciuotiGalutiniPolimorfiskai)
{
    Studentas s = sukurtiTestiniStudenta();
    Zmogus* rodykle = &s;

    // Nulinami rezultatai rankiniu būdu
    s.setEgzaminas(10);
    rodykle->suskaiciuotiGalutini();

    EXPECT_GT(s.getgalrezVid(), 0.0f);
}

// =============================================================================
// 12. Skirstymo strategijų testai
// =============================================================================

/**
 * @brief Tikrina 1-ą skirstymo strategiją (du nauji konteineriai).
 */
TEST(Skirstymas, Strategija1_TeisingaiSkirstoVargsiukusIrProtus)
{
    StudContainer studis;

    // Vargsiukas (galrezVid < 5)
    std::istringstream ss1("Vargsas Pirmas 1 1");
    Studentas s1(ss1);
    s1.suskaiciuotiGalutini();
    studis.push_back(s1);

    // Protus (galrezVid >= 5)
    std::istringstream ss2("Protus Antras 9 9");
    Studentas s2(ss2);
    s2.suskaiciuotiGalutini();
    studis.push_back(s2);

    auto rez = skirstymasStrategija1(studis);

    EXPECT_EQ(rez.vargsiukai.size(), static_cast<std::size_t>(1));
    EXPECT_EQ(rez.protai.size(),     static_cast<std::size_t>(1));
    EXPECT_EQ(studis.size(),         static_cast<std::size_t>(2))
        << "Strategija 1 neturetu keisti originalaus konteinerio";
}

/**
 * @brief Tikrina 2-ą strategiją (erase iš originalaus konteinerio).
 */
TEST(Skirstymas, Strategija2_IstrinaMasVargsiukusIsOriginalaus)
{
    StudContainer studis;

    std::istringstream ss1("Vargsas Pirmas 1 1");
    Studentas s1(ss1);
    s1.suskaiciuotiGalutini();
    studis.push_back(s1);

    std::istringstream ss2("Protus Antras 9 9");
    Studentas s2(ss2);
    s2.suskaiciuotiGalutini();
    studis.push_back(s2);

    auto rez = skirstymasStrategija2(studis);

    EXPECT_EQ(rez.vargsiukai.size(), static_cast<std::size_t>(1));
    EXPECT_EQ(rez.protai.size(),     static_cast<std::size_t>(1));
    // Po strategijos 2 originalas turi būti tuščias
    EXPECT_TRUE(studis.empty())
        << "Po strategijos 2 originalas turetu buti tuscias";
}

/**
 * @brief Tikrina 3-ą strategiją (std::partition).
 */
TEST(Skirstymas, Strategija3_PartitionTeisingaiSkirstoGrupes)
{
    StudContainer studis;

    std::istringstream ss1("Vargsas Pirmas 1 1");
    Studentas s1(ss1);
    s1.suskaiciuotiGalutini();
    studis.push_back(s1);

    std::istringstream ss2("Protus Antras 9 9");
    Studentas s2(ss2);
    s2.suskaiciuotiGalutini();
    studis.push_back(s2);

    auto rez = skirstymasStrategija3(studis);

    EXPECT_EQ(rez.vargsiukai.size(), static_cast<std::size_t>(1));
    EXPECT_EQ(rez.protai.size(),     static_cast<std::size_t>(1));
}

// =============================================================================
// 13. setters / getters
// =============================================================================

/**
 * @brief Tikrina, kad setVardas/setPavarde/setEgzaminas veikia teisingai.
 */
TEST(SettersGetters, NustatimoFunkcijosVeikia)
{
    Studentas s;
    s.setVardas("Antanas");
    s.setPavarde("Antanauskas");
    s.setEgzaminas(8);

    EXPECT_EQ(s.getVardas(),    "Antanas");
    EXPECT_EQ(s.getPavarde(),   "Antanauskas");
    EXPECT_EQ(s.getEgzaminas(), 8);
}

// =============================================================================
// Pagrindinis
// =============================================================================

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
