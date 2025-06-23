#include <iostream>
#include <string>
#include <iomanip>
#include "CLI11.hpp"
#include "mt2_chenghan_bisect.h"    // Cheng-Han bisection algorithm

int main(int argc, char **argv) {

    CLI::App app{"Test MT2 algorithm with Cheng-Han implementation"};
    CLI11_PARSE(app, argc, argv);

    mt2_bisect::mt2 mt2_event;


// Numbers from Oxbridge library that gives MT2=395.67746769 (with mB=100 GeV) amd MT2=412.62766846 (with mB=150 GeV)
// Here we use helloWorld_Mt2_Without_Minuit2_Example
    double pa[3]    = {100.0,  410.0,   20.0};
    double pb[3]    = {100.0, -210.0, -300.0};
    double pmiss[3] = {  0.0, -200.0,  280.0};
    double mn = 100.0;

// With mB=100.0, find MT2 = 395.6795698307595 using this Cheng-Han implementation
// with mB=150.0, find MT2 = 412.6288381090941 using this Cheng-Han implementation

    mt2_event.set_momenta( pa, pb, pmiss );
    mt2_event.set_mn( mn );

    double mt2_value = mt2_event.get_mt2();

    std::cout << "MT2 = " << std::setprecision(16) << mt2_value << std::endl;

    return 0;

}
