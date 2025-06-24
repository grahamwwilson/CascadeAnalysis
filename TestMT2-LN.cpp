
#include "asymm_mt2_lester_bisect.h"  // Lester-Nachman implementation
#include <iostream>
#include <iomanip>

int main(int argc, char * argv[]) {

// Take example from Oxbridge library and adapt to standalone header case

    // The input parameters associated with the particle
    // (or collection of particles) associated with the 
    // first "side" of the event:
    const double massOfSystemA =  100; // GeV
    const double pxOfSystemA   =  410; // GeV
    const double pyOfSystemA   =   20; // GeV
    
    // The input parameters associated with the particle
    // (or collection of particles) associated with the 
    // second "side" of the event:
    const double massOfSystemB =  100; // GeV
    const double pxOfSystemB   = -210; // GeV
    const double pyOfSystemB   = -300; // GeV
    
    // The missing transverse momentum:
    const double pxMiss        = -200; // GeV
    const double pyMiss        =  280; // GeV

    // The mass of the "inivisible" particle presumed to have
    // been produced at the end of the decay chain in each
    // "half" of the event:
    const double invis_massA    = 100; // GeV
    const double invis_massB    = 100; // GeV

    double desiredPrecisionOnMt2 = 0.0001; // Must be >=0.  If 0 alg aims for machine precision.
                                      // If >0, MT2 computed to supplied absolute precision.

    std::cout << "Going to calculate MT2 with\n"
	      << "   ltv_Vis_A  = " << massOfSystemA << " " << pxOfSystemA << " " << pyOfSystemA  << "\n"
	      << "   ltv_Vis_B  = " << massOfSystemB << " " << pxOfSystemB << " " << pyOfSystemB  << "\n"
	      << "   pT_Miss    = " << pxMiss    << " " << pyMiss << "\n"
	      << "   invis_massA = " << invis_massA << "\n"
	      << "   invis_massB = " << invis_massB << "\n" 
          << "   with specified precision " << desiredPrecisionOnMt2 << std::endl;
    
    // Now that we have some visible momenta and some missing transverse
    // momentum we can calculate MT2.

    asymm_mt2_lester_bisect::disableCopyrightMessage();

    double MT2 = asymm_mt2_lester_bisect::get_mT2(
               massOfSystemA, pxOfSystemA, pyOfSystemA,
               massOfSystemB, pxOfSystemB, pyOfSystemB,
               pxMiss, pyMiss,
               invis_massA, invis_massB,
               desiredPrecisionOnMt2);
    
    // Now we print out the result:
    std::cout << "ANSWER: mt2 = " << std::setprecision(16) << MT2 << std::endl; 

    // Result is ANSWER: mt2 = 395.6774676874312
    // 
    
    return 0;

}
