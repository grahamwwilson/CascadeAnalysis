
#include "FourVec.h"
#include "asymm_mt2_lester_bisect.h"  // Lester-Nachman implementation
#include <iostream>
#include <iomanip>
#include <limits>

int main(int argc, char * argv[]) {

    using Precise::pdouble;

    std::cout << "Using pdouble implementation: " << Precise::implementation << "\n";
    std::cout << "Precision for Precise::pdouble:\n";
    std::cout << "  Binary digits: " << std::numeric_limits<pdouble>::digits << "\n";
    std::cout << "  Decimal digits: " << std::numeric_limits<pdouble>::digits10 << "\n";
    std::cout << "  Max value: " << std::numeric_limits<pdouble>::max() << "\n";
    std::cout << "  Epsilon: " << std::numeric_limits<pdouble>::epsilon() << "\n";

// 80-bit extended precision
/*    std::cout << "sizeof(long double): " << sizeof(long double) << " bytes\n";
    std::cout << "Precision: " << std::numeric_limits<long double>::digits10 << " decimal digits\n";

    std::cout << "sizeof(double): " << sizeof(double) << " bytes\n";
    std::cout << "Precision: " << std::numeric_limits<double>::digits10 << " decimal digits\n";
*/

    const double ME  = 0.5109989500e-3;
    const double MMU = 0.1056583755;
    const double MTAU = 1.77693;       // Actually PDG has a new measurement from BELLEII !

    FourVec l1(11, 1000.0, 1000.0, 1000.0, ME);
    FourVec l2(13,  900.0, 1000.0, 2000.0, MMU);
    FourVec l3(15,  800.0, 1000.0, 3000.0, MTAU);
    FourVec l12 = l1+l2;
    FourVec l123 = l12 + l3;

// DRY principle -> should make this some kind of function ...
    std::cout << "l1 " << l1.pdgID << " " << std::setprecision(14) << l1.Px() << " " << l1.Py() << " " << l1.Pz() << " " << l1.M() << std::endl;
    std::cout << "l2 " << l2.pdgID << " " << l2.Px() << " " << l2.Py() << " " << l2.Pz() << " " << l2.M() << std::endl;
    std::cout << "l3 " << l3.pdgID << " " << l3.Px() << " " << l3.Py() << " " << l3.Pz() << " " << l3.M() << std::endl;
    std::cout << "l12 " << l12.pdgID << " " << l12.Px() << " " << l12.Py() << " " << l12.Pz() << " " << l12.M() << std::endl;
    std::cout << "l123 " << l123.pdgID << " " << l123.Px() << " " << l123.Py() << " " << l123.Pz() << " " << l123.M() << std::endl;

// Try boosting these 4-vectors.
    ROOT::Math::XYZVector beta(-0.3, -0.5, -0.4);
    FourVec l1b = l1.Boost(beta);
    FourVec l2b = l2.Boost(beta);
    FourVec l3b = l3.Boost(beta);
    FourVec l12b = l12.Boost(beta);
    FourVec l123b = l123.Boost(beta);

    std::cout << "l1b " << l1b.pdgID << " " << std::setprecision(14) << l1b.Px() << " " << l1b.Py() << " " << l1b.Pz() << " " << l1b.M() << std::endl;
    std::cout << "l2b " << l2b.pdgID << " " << l2b.Px() << " " << l2b.Py() << " " << l2b.Pz() << " " << l2b.M() << std::endl;
    std::cout << "l3b " << l3b.pdgID << " " << l3b.Px() << " " << l3b.Py() << " " << l3b.Pz() << " " << l3b.M() << std::endl;
    std::cout << "l12b " << l12b.pdgID << " " << l12b.Px() << " " << l12b.Py() << " " << l12b.Pz() << " " << l12b.M() << std::endl;
    std::cout << "l123b " << l123b.pdgID << " " << l123b.Px() << " " << l123b.Py() << " " << l123b.Pz() << " " << l123b.M() << std::endl;

    return 0;

}
