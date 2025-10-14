#define Ana_cxx
// The class definition in Ana.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("Ana.C")
// root> T->Process("Ana.C","some options")
// root> T->Process("Ana.C+")
//

#include <numeric>
#include "Ana.h"
#include <TH1.h>
#include <TH2.h>
#include <TAxis.h>
#include <TStyle.h>
#include <string>
#include <tuple>
#include "FourVec.h"
#include "DiCuts.h"       // Dilepton selection
#include "TriCuts.h"      // Trilepton selection
#include "QuadCuts.h"     // Quadlepton selection
#include <algorithm>
#include <utility>
#include <bitset> 
#include <TMath.h>
#include <array>
#include <cassert>
#include <map>

// Declare histograms globally here
#include "AnaHistos.h"    // Try moving to SlaveBegin ?
// Declare helper functions with bits for selection
#include "SelectionBits.h"

// S/B estimates in % for the various tri-lepton categories
// The key = {compLepCode, minPairCode} see histogram h3Codes where each column corresponds
// to a row of this map
//             e+             mu+            e-             mu- 
std::map<std::pair<int,int>, double> triMap = {
        {{1,1}, 3.11 }, {{2,1}, 0.77}, {{3,1}, 2.01}, {{4,1}, 0.57},  //OSee
        {{1,2}, 0.43 }, {{2,2}, 3.21}, {{3,2}, 0.31}, {{4,2}, 2.19},  //OSmm
        {{1,3}, 10.75}, {{2,3}, 6.53}, {{3,3}, 7.89}, {{4,3}, 4.44},  //OSem
        {{1,4}, 0.82 }, {{2,4}, 0.26}, {{3,4}, 0.01}, {{4,4}, 0.01},  //NSSee
        {{1,5}, 0.13 }, {{2,5}, 1.26}, {{3,5}, 0.01}, {{4,5}, 0.01},  //NSSmm
        {{1,6}, 1.34 }, {{2,6}, 0.72}, {{3,6}, 0.01}, {{4,6}, 0.01},  //NSSem
        {{1,7}, 0.01 }, {{2,7}, 0.01}, {{3,7}, 1.18}, {{4,7}, 0.16},  //PSSee
        {{1,8}, 0.01 }, {{2,8}, 0.01}, {{3,8}, 0.36}, {{4,8}, 2.31},  //PSSmm
        {{1,9}, 0.01 }, {{2,9}, 0.01}, {{3,9}, 1.54}, {{4,9}, 1.16}   //PSSem
};

struct BranchingFractions {
    std::array<double, 3> slep;     // Modes (l N1), (l N2), (v C1)
    std::array<double, 3> sneu;     // Modes (v N1), (v N2), (l C1)
    std::array<double, 3> neut;     // Modes (photon N1), (Z N1), (W C1)
};

// Option 1: default (used in the LSP-180 samples)
const BranchingFractions bf_default = {
    {0.50, 0.25, 0.25},  // slep
    {0.25, 0.25, 0.50},  // sneu
    {0.04, 0.48, 0.48}   // neut
};

// Option 2: deprecated (used in the LSP-220, LSP-260, and LSP-270 samples)
const BranchingFractions bf_deprecated = {
    {0.50, 0.25, 0.25},  // slep
    {0.20, 0.25, 0.50},  // sneu
    {0.05, 0.85, 0.10}   // neut
};

double get_reweight(int mode, const std::array<double,3>& target, const std::array<double,3>& current) {
    assert(mode >= 0 && mode < 3);
    if (current[mode] == 0.0) return 0.0; // avoid divide-by-zero
    return target[mode] / current[mode];
}

double CascadeInfo(int evn, int tree, int prod, int dk1, int dk2, int N2dk1, int N2dk2, 
                   bool apply=false, bool debug=false, const std::string& currBFscheme = "default",
                   BranchingFractions bf_target = {
                       {0.00, 1.00, 0.00},  // slep
                       {0.00, 1.00, 0.00},  // sneu
                       {0.00, 0.00, 1.00}   // neut
                   }) {

    double newWeight = 1.0;

// First get a hold of the current branching fractions used in the generated signal sample
    BranchingFractions bf_curr;
    if (currBFscheme == "default") {   // As used in the LSP-180 sample
        bf_curr = bf_default;
    } 
    else if (currBFscheme == "deprecated" ) {  // As used in the initial samples.
        bf_curr = bf_deprecated;
    } 
    else {
        std::cerr << "Unknown BF scheme: " << currBFscheme << ", using deprecated one " << std::endl;
        bf_curr = bf_deprecated;
    }

// Map prod values to slepton types using pseudo-PDG IDs
    int slep1 = 0, slep2 = 0;

    switch (prod) {
        case 1: slep1 = slep2 = 11; break;
        case 2: slep1 = 11; slep2 = 12; break;
        case 3: slep1 = 11; slep2 = 12; break;
        case 4: slep1 = slep2 = 12; break;
        case 5: slep1 = slep2 = 13; break;
        case 6: slep1 = 13; slep2 = 14; break;
        case 7: slep1 = 13; slep2 = 14; break;
        case 8: slep1 = slep2 = 14; break;
    }

// Lambda to apply weight for slepton decay
    auto weight_slepton = [&](int slep, int dk, int N2dk) {
        if (slep == 11 || slep == 13) { // slepton
            if (dk == 1) newWeight *= get_reweight(0, bf_target.slep, bf_curr.slep); // N1
            if (dk == 3) newWeight *= get_reweight(2, bf_target.slep, bf_curr.slep); // C1
            if (dk == 2) {
                newWeight *= get_reweight(1, bf_target.slep, bf_curr.slep); // N2
                if (N2dk == 1) newWeight *= get_reweight(0, bf_target.neut, bf_curr.neut); // photon
                else if (N2dk == 2) newWeight *= get_reweight(1, bf_target.neut, bf_curr.neut); // Z
                else newWeight *= get_reweight(2, bf_target.neut, bf_curr.neut); // W
            }
        } else if (slep == 12 || slep == 14) { // sneutrino
            if (dk == 1) newWeight *= get_reweight(0, bf_target.sneu, bf_curr.sneu); // N1
            if (dk == 3) newWeight *= get_reweight(2, bf_target.sneu, bf_curr.sneu); // C1
            if (dk == 2) {
                newWeight *= get_reweight(1, bf_target.sneu, bf_curr.sneu); // N2
                if (N2dk == 1) newWeight *= get_reweight(0, bf_target.neut, bf_curr.neut); // photon
                else if (N2dk == 2) newWeight *= get_reweight(1, bf_target.neut, bf_curr.neut); // Z
                else newWeight *= get_reweight(2, bf_target.neut, bf_curr.neut); // W
            }
        }
    };

// Apply weight for each slepton decay chain
    weight_slepton(slep1, dk1, N2dk1);
    weight_slepton(slep2, dk2, N2dk2);

    if (debug) {
        std::cout << "\nEvent " << evn << "\n";
        std::cout << "prod " << prod << " tree " << tree << "\n";
        std::cout << "slep1, slep2 pseudoPDGIDs " << slep1 << " " << slep2 << "\n";
        std::cout << "Slepton decays: " << dk1 << " " << dk2 << "\n";
        std::cout << "N2 decays:      " << N2dk1 << " " << N2dk2 << "\n";
        std::cout << "apply flag: " << apply << " New relative weight: " << newWeight << "\n";
    }

    if (!apply) newWeight = 1.0;
    return newWeight;
}

int ProcessInfo(int prod, int dk1, int dk2, int N2dk1, int N2dk2){

    int value = 2;                         // slepton-sneutrino associated production
    if(prod == 1 || prod == 5) value = 1;  // slepton pair production
    if(prod == 4 || prod == 8) value = 3;  // sneutrino pair production
    return value;

}

int NewPairCode(int pairCode) {

// Map paircode (in range 1 to 6) to values from 1 to 4 with 1=OSSF, 2=OSDF, 3=SSSF, 4=SSDF

    int code = 1;
    if (pairCode != 1) code = pairCode - 1;
    if (pairCode >= 5) code--; 

    return code;

}

int PairCode(const std::pair<int,int>& lepPair, const std::array<int,4>& qfs) {

    // pair indices are 1-based in QuadLeptonInfo, so subtract 1

    int i = lepPair.first  - 1;
    int j = lepPair.second - 1;

    int qfi = qfs[i];
    int qfj = qfs[j];

    int code = 0;

    if (qfi * qfj < 0) {
        // opposite sign
        if (std::abs(qfi) == std::abs(qfj)) {
            if(std::abs(qfi) == 1){
                code = 1; // OSSF-ee
            }
            else {
                code = 2; // OSSF-mm
            }
        } 
        else {  // different flavor
            code = 3;     // OSDF-em
        }
    } 
    else {
        // same sign
        if (std::abs(qfi) == std::abs(qfj)) {
            if(std::abs(qfi) == 1){
                code = 4; // SSSF-ee
            }
            else {
                code = 5; // SSSF-mm
            }
        } 
        else {  // different flavor
            code = 6;     // SSDF-em
        }
    }
    return code;
}

template <size_t N>
int QPairCode(const std::pair<int,int>& lepPair, const std::array<int,N>& qfs) {

    // pair indices are 1-based in TriLeptonInfo, so subtract 1

    int i = lepPair.first  - 1;
    int j = lepPair.second - 1;

    int qfi = qfs[i];
    int qfj = qfs[j];

    int code = 0;

    if (qfi * qfj < 0) {
        // opposite sign
        if (std::abs(qfi) == std::abs(qfj)) {
            if(std::abs(qfi) == 1){
                code = 1; // OSSF-ee
            }
            else {
                code = 2; // OSSF-mm
            }
        } 
        else {  // different flavor
            code = 3;     // OSDF-em
        }
    } 
    else {   // same sign
        if (std::abs(qfi) == std::abs(qfj)) {   // same flavor
            if( qfi == -1){
                code = 4; // NSS-ee
            }
            else if ( qfi == -2 ){
                code = 5; // NSS-mm
            }
            else if ( qfi ==  1 ){
                code = 7; // PSS-ee
            }
            else if ( qfi ==  2 ){
                code = 8; // PSS-mm
            }
            else{
                cout << "Shouldn't be here in QPairCode " << qfi << " " << qfj << endl;
            }
        } 
        else {  // different flavor
            if( (qfi == -1 && qfj == -2) || (qfi == -2 && qfj == -1) ){
                code = 6;  // NSS-em
            }
            else if ( (qfi ==  1 && qfj ==  2) || (qfi ==  2 && qfj ==  1) ){
                code = 9;  // PSS-em
            }
            else{
                cout << "Shouldn't be here either in QPairCode " << qfi << " " << qfj << endl; 
            }
        }
    }
    return code;
}

int CompLepCode(int compLep, const std::array<int,3>& qfs) {

    // pair indices are 1-based in TriLeptonInfo, so subtract 1

    int i = compLep - 1;
    int qf = qfs[i];

    int code = 0;
    if(qf == 1)code = 1;
    if(qf == 2)code = 2;
    if(qf == -1)code = 3;
    if(qf == -2)code = 4;

    return code;
}



std::tuple<double, double, int, double, int, int> TriLeptonInfo(double m12, double m13, double m23, bool ossf12, bool ossf13, bool ossf23, int qf1, int qf2, int qf3){

// Calculate a number of quantities related to trilepton topologies and return in a tuple.

// 1. The min dilepton mass independent of flavor/charge
//     double minmll = std::min( { m12, m13, m23 });
// 2. The max dilepton mass independent of flavor/charge
     double maxmll = std::max( { m12, m13, m23 });
     const double MZ = 91.2;
// 3. The number of opposite-sign same flavor (OSSF) dileptons
     int nossf = 0;
// 4. For OSSF dileptons the smallest SIGNED deviation with respect to the nominal Z mass. If no 
     double devZ = -95.0;   // default value
     if( ossf12 ){
         nossf++;
         double thisdevZ = m12 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf13 ){
         nossf++;
         double thisdevZ = m13 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf23 ){
         nossf++;
         double thisdevZ = m23 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }

   // 3. Find min pair and complementary lepton
     struct PairEntry {
         double mass;
         std::pair<int,int> pair;
         int comp; // complementary lepton
     };

     std::array<PairEntry,3> pairs{{
         { m12, {1,2}, 3 },
         { m13, {1,3}, 2 },
         { m23, {2,3}, 1 }
     }};

     auto min_it = std::min_element(pairs.begin(), pairs.end(),
                                   [](auto &a, auto &b){ return a.mass < b.mass; });

     auto minmll   = min_it->mass;
     auto min_pair = min_it->pair;
     int comp_lep  = min_it->comp;

     std::array<int,3> qfs = { qf1, qf2, qf3};

     int minPairCode  = QPairCode(min_pair, qfs);
     int compLepCode  = CompLepCode(comp_lep, qfs);
 
     return std::make_tuple(minmll, maxmll, nossf, devZ, minPairCode, compLepCode);
 
}

std::tuple<double, double, int, double, double, double, int, int, int, int, double> QuadLeptonInfo(double m12, double m13, double m14, 
                                                                       double m23, double m24, double m34, 
                                                                       double m123, double m124, double m234,
                                                                       bool ossf12, bool ossf13, bool ossf14, 
                                                                       bool ossf23, bool ossf24, bool ossf34,
                                                                       int qf1, int qf2, int qf3, int qf4){
// Input arguments for 4 leptons are: 
// The masses of each pair, the masses of each triplet, flags for each pair as to wehther opposite-sign same flavor, 
// and the charge*flavor values for each lepton.

// Calculate a number of quantities related to four-lepton topologies and return in a tuple.
// 1. The min dilepton mass independent of flavor/charge
//     double minmll = std::min( { m12, m13, m14, m23, m24, m34 });

    // Entry: (mass, complementary_mass, indices of min pair, indices of complementary pair)
    struct Entry {
        double mass;
        double comp_mass;
        std::pair<int,int> pair;
        std::pair<int,int> comp_pair;
    };

    std::array<Entry,6> masses {{
        { m12, m34, {1,2}, {3,4} },
        { m13, m24, {1,3}, {2,4} },
        { m14, m23, {1,4}, {2,3} },
        { m23, m14, {2,3}, {1,4} },
        { m24, m13, {2,4}, {1,3} },
        { m34, m12, {3,4}, {1,2} }
    }};

// Find the entry with smallest first element
    auto min_it = std::min_element(masses.begin(), masses.end(),
                                   [](auto &a, auto &b){ return a.mass < b.mass; });

    double min_val   = min_it->mass;
    double comp_val  = min_it->comp_mass;
    auto   min_pair  = min_it->pair;
    auto   comp_pair = min_it->comp_pair;

//    std::cout << "Minimum mass = " << min_val
//              << ", complementary mass = " << comp_val << "\n";

    double minmll = min_val;
    double othermll = comp_val;

// 2. The max dilepton mass independent of flavor/charge
     double maxmll = std::max( { m12, m13, m14, m23, m24, m34 });

// 3. The number of opposite-sign same flavor (OSSF) dileptons
     int nossf = 0;
// 4. For OSSF dileptons the smallest SIGNED deviation with respect to the nominal Z mass. If none defaults to -95.0. 
     const double MZ = 91.2;
     double devZ = -95.0;   // default value
     if( ossf12 ){
         nossf++;
         double thisdevZ = m12 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf13 ){
         nossf++;
         double thisdevZ = m13 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf14 ){
         nossf++;
         double thisdevZ = m14 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf23 ){
         nossf++;
         double thisdevZ = m23 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf24 ){
         nossf++;
         double thisdevZ = m24 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf34 ){
         nossf++;
         double thisdevZ = m34 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }

// 5. The complementary dilepton mass (using the other two leptons) to the pairing that 
//    results in the minimum dilepton mass. The variable is othermll.

// 6. The minimum trilepton mass

     double minm3l = std::min( { m123, m124, m234 } );

     std::array<int,4> qfs = { qf1, qf2, qf3, qf4 };

     int minPairCode  = PairCode(min_pair,  qfs);
     int compPairCode = PairCode(comp_pair, qfs);

     int minQPairCode  = QPairCode(min_pair,  qfs);
     int compQPairCode = QPairCode(comp_pair, qfs);

// Look at the 3 possible di-lepton pairings.
     double msq1 = m12*m12 + m34*m34;
     double msq2 = m13*m13 + m24*m24;
     double msq3 = m14*m14 + m23*m23;
     double minmsq = std::min( {msq1, msq2, msq3} );
     double min2lpairedmass = std::sqrt(minmsq);
 
     return std::make_tuple(minmll, maxmll, nossf, devZ, othermll, minm3l, minPairCode, compPairCode, minQPairCode, compQPairCode, min2lpairedmass);
 
}

void FillCutFlow(unsigned int sel, double wt) {

    // Fill the preselection bin
    hCutFlow->Fill(-1.0, wt);

    // Loop over all cuts
    for (unsigned int i = 0; i < static_cast<unsigned int>(TriCuts::NUMCUTS); ++i) {
        TriCuts cut = static_cast<TriCuts>(i);
        if (PassesAllCutsSoFar(sel, cut)) {
            hCutFlow->Fill(i, wt);
        }
    }

    // Optional: final bin if all cuts passed
    if (sel == 0) {
        hCutFlow->Fill(static_cast<unsigned int>(TriCuts::NUMCUTS), wt);
    }

// Now do the exclusive one.
    if (sel == 0) {
        hXCutFlow->Fill(-1.0, wt);    // this bin is for those events passing all cuts to encode the level
    }
    // Loop over all cuts
    for (unsigned int i = 0; i < static_cast<unsigned int>(TriCuts::NUMCUTS); ++i) {
        TriCuts cut = static_cast<TriCuts>(i);
        if (isOnlyBitSet(sel, cut)) {                 // event fails this and only this one
            hXCutFlow->Fill(i, wt);
        }
    }

}

void FillDiCutFlow(unsigned int sel, int ntaunus, double wt) {
// Note this histogram used to be defined in Ana.h

    // Fill the preselection bin
    hDiCutFlow->Fill(-1.0, wt);

    // Loop over all cuts
    for (unsigned int i = 0; i < static_cast<unsigned int>(DiCuts::NUMCUTS); ++i) {
        DiCuts cut = static_cast<DiCuts>(i);
        if (PassesAllCutsSoFar(sel, cut)) {
            hDiCutFlow->Fill(i, wt);
        }
    }

    // Optional: final bin if all cuts passed
    if (sel == 0) {
        hDiCutFlow->Fill(static_cast<unsigned int>(DiCuts::NUMCUTS), wt);
    }

    if(ntaunus == 0){    // Diagnostics for rates.

    // Fill the preselection bin
        hDiCutFlowNonTau->Fill(-1.0, wt);

    // Loop over all cuts
       for (unsigned int i = 0; i < static_cast<unsigned int>(DiCuts::NUMCUTS); ++i) {
            DiCuts cut = static_cast<DiCuts>(i);
            if (PassesAllCutsSoFar(sel, cut)) {
                hDiCutFlowNonTau->Fill(i, wt);
            }
        }

    // Optional: final bin if all cuts passed
        if (sel == 0) {
            hDiCutFlowNonTau->Fill(static_cast<unsigned int>(DiCuts::NUMCUTS), wt);
        }

    }



// Now do the exclusive one.
    if (sel == 0) {
        hXDiCutFlow->Fill(-1.0, wt);    // this bin is for those events passing all cuts to encode the level
    }
    // Loop over all cuts
    for (unsigned int i = 0; i < static_cast<unsigned int>(DiCuts::NUMCUTS); ++i) {
        DiCuts cut = static_cast<DiCuts>(i);
        if (isOnlyBitSet(sel, cut)) {                 // event fails this and only this one
            hXDiCutFlow->Fill(i, wt);
        }
    }
}

void FillQuadCutFlow(unsigned int sel, double wt) {
// Note this histogram used to be defined in Ana.h

    // Fill the preselection bin
    hQuadCutFlow->Fill(-1.0, wt);

    // Loop over all cuts
    for (unsigned int i = 0; i < static_cast<unsigned int>(QuadCuts::NUMCUTS); ++i) {
        QuadCuts cut = static_cast<QuadCuts>(i);
        if (PassesAllCutsSoFar(sel, cut)) {
            hQuadCutFlow->Fill(i, wt);
        }
    }

    // Optional: final bin if all cuts passed
    if (sel == 0) {
        hQuadCutFlow->Fill(static_cast<unsigned int>(QuadCuts::NUMCUTS), wt);
    }

// Now do the exclusive one.
    if (sel == 0) {
        hXQuadCutFlow->Fill(-1.0, wt);    // this bin is for those events passing all cuts to encode the level
    }
    // Loop over all cuts
    for (unsigned int i = 0; i < static_cast<unsigned int>(QuadCuts::NUMCUTS); ++i) {
        QuadCuts cut = static_cast<QuadCuts>(i);
        if (isOnlyBitSet(sel, cut)) {                 // event fails this and only this one
            hXQuadCutFlow->Fill(i, wt);
        }
    }
}


void Ana::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
    TH1::SetDefaultSumw2(kTRUE);

}

void Ana::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
//#include "AnaHistos.h"

}

bool ZZlikeEvent(int qf1, int qf2, int qf3, int qf4){

// Check whether flavors are ZZ -> 4e, 4mu, or 2e2mu consistent
// For now ignore charge info.

   bool value = false;
   int code = std::abs(qf1) + std::abs(qf2) + std::abs(qf3) + std::abs(qf4);
   if(code == 4 || code == 6 || code == 8)value = true;
   return value;
}

FourVec CombineJetFV(const std::vector<int>& indices, const TTreeReaderArray<double>& PT_jet, const TTreeReaderArray<double>& Eta_jet, const TTreeReaderArray<double>& Phi_jet, const TTreeReaderArray<double>& M_jet) {
    FourVec total;
    for (int idx : indices) {
// just use 1 as ID for now
        total += FourVec::FromPtEtaPhiM(1, PT_jet[idx], Eta_jet[idx], Phi_jet[idx], M_jet[idx]);
    }
    return total;
}

bool Ana::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

    fReader.SetLocalEntry(entry);

    const double MLLCUT3 = 21.0;        //  58/24/16
    const double MLLCUT4 = 21.0;
    const double TARGETLUMI = 400.0;

    double wt = TARGETLUMI*(*weight);
    if(inputFilePrefix=="SlepSneu-180"){
        double correctedweight = 3.63941e-4;   // Zach's E-mail of 7/28/2025 said 3.639e-4.
        wt = TARGETLUMI*correctedweight;
    }

#if ANA_NTUPLE_VERSION >= 3 && ANA_NTUPLE_FLAVOR_CODE == SIGNAL_CODE
// Access BR info etc
    int evn = *eventnum;
    int ctree = *cascades_tree;
    int cprod = *cascades_prod;
    int dk1 = *cascades_SlepSneu_1stDecay;
    int dk2 = *cascades_SlepSneu_2ndDecay;
    int N2dk1 = *cascades_N2_1stDecay;
    int N2dk2 = *cascades_N2_2ndDecay;

// New branching fractions. Currently just for slep while keeping B(N2)=B(C1)
//    BranchingFractions myTarget = {
//        {0.50, 0.25, 0.25},   // slep (N1, N2, C1)
//        {0.25, 0.25, 0.50},   // sneu (N1, N2, C1)
//        {0.04, 0.48, 0.48}    // neut (photon, Z, W)
//    };
#include "NewBranchingFractions.h"    // This defines myTarget

// "default" branching fractions for the recent production by Justin (LSP=180).
    BranchingFractions myTarget1 = {
        {0.50, 0.25, 0.25},   // slep (N1, N2, C1)
        {0.25, 0.25, 0.50},   // sneu (N1, N2, C1)
        {0.04, 0.48, 0.48}    // neut (photon, Z, W)
    };

// "deprecated" branching fractions from the original productions
    BranchingFractions myTarget2 = {
        {0.50, 0.25, 0.25},   // slep (N1, N2, C1)
        {0.25, 0.25, 0.50},   // sneu (N1, N2, C1)
        {0.05, 0.85, 0.10}    // neut (photon, Z, W)
    };

    bool reWeight= false; 
    if(reWeight){
        double newWeight = 1.0;
        if(inputFilePrefix=="SlepSneu-180"){
            newWeight = CascadeInfo(evn, ctree, cprod, dk1, dk2, N2dk1, N2dk2, 
                                    true, false, "default", myTarget);
        }
        else if (inputFilePrefix=="SlepSneu-220" || 
                 inputFilePrefix=="SlepSneu-260" || 
                 inputFilePrefix=="SlepSneu-270"){
            newWeight = CascadeInfo(evn, ctree, cprod, dk1, dk2, N2dk1, N2dk2, 
                                    true, false, "deprecated", myTarget);
        }
        else{
            std::cout << "Should we be here? " << std::endl;
        }
        wt*=newWeight;
    }

// Specifically for highlighting observed different weight issue
    if(*weight<0.00032){
        hprocesslowt->Fill(*cascades_prod, wt);
    }
    else{
        hprocesshiwt->Fill(*cascades_prod, wt);
    }
#endif

    hMET->Fill(*MET, wt);
    hMET_phi->Fill(*MET_phi, wt);
    haltMET->Fill(*altMET, wt);
    haltMET_phi->Fill(*altMET_phi, wt);

#if ANA_NTUPLE_VERSION >= 3
    double myPTISR = *PTISR_LEP;  // Only available in V3 and beyond
// peg underflows and overflows in histogram range
    if(myPTISR < 1.0)myPTISR = 1.0;
    if(myPTISR > 999.0)myPTISR = 999.0;
#endif

    FourVec fMET = FourVec::FromPtEtaPhiM(100, *MET, 0.0, *MET_phi, 0.0);

//    std::cout << "Event weight = " << 1.0e6*(*weight) << std::endl;

//   std::cout << "New event " << std::endl;
    for (unsigned int i = 0; i < PT_lep.GetSize(); ++i){
        hleptonPT->Fill(PT_lep[i], wt);
//       std::cout << "Lepton " << i << " pT = " << PT_lep[i] << std::endl;
    }

    std::vector<unsigned int> vlidx;    // For gold and silver quality leptons of arbitrary pt
    std::vector<unsigned int> vbidx;    // For bronze leptons of arbitrary pt

    int nleptons = 0; int nleptons_hipt = 0;
    int nbronze = 0;
    int ngs = 0;
// Loop over leptons separating into two quality categories
    for (unsigned int i = 0; i < LepQual_lep.GetSize(); ++i){
        if(LepQual_lep[i] <=1){
            ngs ++;                // Gold or silver
            vlidx.push_back(i);
        }
        else{
            nbronze++;             // Bronze
            vbidx.push_back(i);
        }
    }
    hnleptons->Fill(nleptons, wt);
    hnleptons_hipt->Fill(nleptons_hipt, wt);
    hngs->Fill(ngs, wt);
    hnbronze->Fill(nbronze, wt);

// Count tau neutrinos at generator level
    int ntaunus = 0;
    for (unsigned int i = 0; i < genPDGID_nu.GetSize(); ++i){
        if(genPDGID_nu[i] == 16 && genMomPDGID_nu[i] == 15)ntaunus++;
        if(genPDGID_nu[i] == -16 && genMomPDGID_nu[i] == -15)ntaunus++;
    }

// FIXME - keep track of b-tag MVA score 
    bool surviveBtagVeto = true;     
    for (unsigned int i = 0; i < BtagID_jet.GetSize(); ++i){
//        if(BtagID_jet[i] >=2) surviveBtagVeto = false;    // Loose
        if(BtagID_jet[i] >=3) surviveBtagVeto = false;    // Medium
    }

// Trigger info
    bool passTrig = false;
    if (*SingleElectrontrigger || *SingleMuontrigger || *DoubleElectrontrigger || *DoubleMuontrigger || *EMutrigger)passTrig = true;

#if ANA_NTUPLE_VERSION == 4
    bool newTrig = false;
    if (*TripleElectrontrigger || *DiEleMutrigger || *DiMuEleLowPTtrigger || *DiMuEleHighPTtrigger || *TripleMuonLowPTtrigger || *TripleMuonHighPTtrigger)newTrig = true;
    if (passTrig || newTrig) passTrig = true;
#endif

#if ANA_NTUPLE_VERSION == 5
//    passTrig = false;
//    if (*SingleElectrontrigger || *SingleMuontrigger || *DoubleElectrontrigger || *DoubleMuontrigger || *EMuMutrigger)passTrig = true;
    bool newTrig = false;
    if (*TripleElectrontrigger || *DiEleMutrigger || *DiMuEleLowPTtrigger || *DiMuEleHighPTtrigger 
                               || *TripleMuonLowPTtrigger || *TripleMuonHighPTtrigger  || *METDoubleMutrigger )newTrig = true;
    if (passTrig || newTrig) passTrig = true;
#endif

// Form jet system.
    FourVec allJets;  // default constructor gives zero 4-vector

    if (*Njet > 0) {
        std::vector<int> indices(*Njet);
        std::iota(indices.begin(), indices.end(), 0);
        allJets = CombineJetFV(indices, PT_jet, Eta_jet, Phi_jet, M_jet);
    }

//    std::cout << "Event " << *eventnum << " Jet info " << *Njet << " " << *MET << " " << *NPU << std::endl;
//    allJets.Print();

// Start on 3-lepton selection using enum class TriCuts
    unsigned int trisel = 0;
//    std::vector<double> ptcuts3l = {20.0, 12.5, 7.5};
    std::vector<double> ptcuts3l = {24.0, 12.0, 6.0};
//    std::vector<double> ptcuts3l = {10.0,  7.5, 5.0};
//    std::vector<double> ptcuts3l = {7.5,  7.5,  7.5};
    double ptcutVeto3l = 2.0;
    double sip3dcut3l = 3.5;
    double maxSIP3D3l{};
    double minSIP3D3l{};
    double SIP3DChisq3l{};
    bool failFourthLeptonVeto = false;
    double ptFourthLepton = -1.5;  // Default to invalid value
    double minmll3l = 0.0;
    double maxmll3l = 0.0;
    int nossf3l = 0;
    double devZ3l = 0.0;
    double m3l = 0.0;
    int minPairCode3l = 0;
    int compLepCode = 0;
    double pt3l = 0.0;
    double pt3ljets = 0.0;

    bool vetoall = true;

    if ( ngs < 3 ){
        trisel = setFailureBit(trisel, TriCuts::GSNumber);
    }
    else{   // We have at least 3 G + S leptons
        if ( PT_lep[vlidx[0]] < ptcuts3l[0] ) trisel = setFailureBit(trisel, TriCuts::PtOne);
        if ( PT_lep[vlidx[1]] < ptcuts3l[1] ) trisel = setFailureBit(trisel, TriCuts::PtTwo);
        if ( PT_lep[vlidx[2]] < ptcuts3l[2] ) trisel = setFailureBit(trisel, TriCuts::PtThree);
// Implement 4th lepton veto.  Loop over all leptons. If lepton is not already one considered as one of the 3 high quality leptons, 
// then set veto flag if any are above the 4th lepton veto pT cut. This also includes bronze leptons. 

        if(vetoall){
            for (unsigned int i = 0; i < LepQual_lep.GetSize(); ++i){             // All leptons
                 if( i != vlidx[0] && i != vlidx[1] && i != vlidx[2] ){           // Not lepton already considered
                     ptFourthLepton = PT_lep[i];
                     if ( PT_lep[i] > ptcutVeto3l ) {
                         failFourthLeptonVeto = true;
                     }
                     break;                       // break out so we store the highest pT additional lepton independent of whether it asserts the veto
                 }
            }
        }
        else{   // veto only using gold and silver
             if(ngs>=4){
                 ptFourthLepton = PT_lep[vlidx[3]];
                 if ( ptFourthLepton > ptcutVeto3l ) {
                     failFourthLeptonVeto = true;
                 }
             }
        }
        if ( !passTrig ) trisel = setFailureBit(trisel, TriCuts::Trigger);
        if ( failFourthLeptonVeto ) trisel = setFailureBit(trisel, TriCuts::PtFourVeto);
        if ( !surviveBtagVeto ) trisel = setFailureBit(trisel, TriCuts::BTagVeto);
        maxSIP3D3l = std::max({SIP3D_lep[vlidx[0]], SIP3D_lep[vlidx[1]], SIP3D_lep[vlidx[2]]});
        minSIP3D3l = std::max({SIP3D_lep[vlidx[0]], SIP3D_lep[vlidx[1]], SIP3D_lep[vlidx[2]]});
        SIP3DChisq3l = ( std::pow(SIP3D_lep[vlidx[0]], 2) + std::pow(SIP3D_lep[vlidx[1]], 2) + std::pow(SIP3D_lep[vlidx[2]], 2) );
        if ( maxSIP3D3l > sip3dcut3l ) trisel = setFailureBit(trisel, TriCuts::SIP3DCut);
// Kinematics for the 3 G+S leptons
        FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
        FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
        FourVec l3 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[2]], PT_lep[vlidx[2]], Eta_lep[vlidx[2]], Phi_lep[vlidx[2]], M_lep[vlidx[2]]);
        FourVec l12 = l1 + l2;
        FourVec l13 = l1 + l3;
        FourVec l23 = l2 + l3;
        FourVec l123 = l1 + l23;
        double m12 = l12.M();   double m13 = l13.M();  double m23 = l23.M();
        bool ossf12 = l1.osdil(l2);   bool ossf13 = l1.osdil(l3);  bool ossf23 = l2.osdil(l3);
        auto result = TriLeptonInfo(m12, m13, m23, ossf12, ossf13, ossf23, l1.qf(), l2.qf(), l3.qf() );
        minmll3l = std::get<0>(result);
        maxmll3l = std::get<1>(result);
        nossf3l  = std::get<2>(result);
        devZ3l   = std::get<3>(result);
        minPairCode3l  = std::get<4>(result);
        compLepCode    = std::get<5>(result);
        std::pair<int,int> key = {compLepCode, minPairCode3l};
        if ( triMap[key] < 4.0 ) trisel = setFailureBit(trisel, TriCuts::FQSB);    // Currently choose cut at 4%.
        if(minmll3l < 4.0) trisel = setFailureBit(trisel, TriCuts::MinMll);
        if(minmll3l > MLLCUT3) trisel = setFailureBit(trisel, TriCuts::MxMinMll);  // This is signal hypothesis sensitive.  35 GeV is OK for LSP=260 and 270, but NOT 220.
        if(std::abs(devZ3l) < 10.0) trisel = setFailureBit(trisel, TriCuts::OffZ);
        m3l = l123.M();
        if(std::abs(m3l - 91.2) < 10.0) trisel = setFailureBit(trisel, TriCuts::M3LZV);
        if(nossf3l == 0) trisel = setFailureBit(trisel, TriCuts::OSSF);
        pt3l = l123.Pt();
        FourVec l123Jets = l123 + allJets;
        pt3ljets = l123Jets.Pt();
        if( (*Njet == 0 && pt3l < 10.0) || (*Njet >=1 && pt3ljets < 30.0)) trisel = setFailureBit(trisel, TriCuts::Pt3L);
    }
    FillCutFlow(trisel, wt);

// 4l selection
// Start on 4-lepton selection using enum class QuadCuts
    unsigned int quadsel = 0;
    std::vector<double> ptcuts4l = {18.0, 8.0, 6.0, 2.0};
//    std::vector<double> ptcuts4l = {20.0, 12.5, 7.5, 5.0};
//    std::vector<double> ptcuts4l = {6.0, 6.0, 6.0, 6.0};
    bool failFifthLeptonVeto = false;
    double ptFifthLepton = -1.5;  // Default to invalid value
//    double ptcutVeto4l = 3.0;
    double ptcutVeto4l = 99999.0;   // Turn this fifth lepton veto off
    double maxSIP3D4l{};
    double SIP3DChisq4l{};
    double minmll4l = 0.0;
    double maxmll4l = 0.0;
    int nossf4l = 0;
    double devZ4l = 0.0;
    double pt4l = 0.0;
    double pt4ljets = 0.0;
    double m4l = 0.0;
    double othermll4l = 0.0;
    double minm3lfor4l = 0.0;
    int minPairCode = 0;
    int compPairCode = 0;
    int minQPairCode = 0;
    int compQPairCode = 0;
    double min2lPairedMass = 0.0;
    bool ZZlike = false;
  
    if ( ngs < 4 ){
        quadsel = setFailureBit(quadsel, QuadCuts::GSNumber);
    }
    else{   // We have at least 4 G + S leptons
        if ( PT_lep[vlidx[0]] < ptcuts4l[0] ) quadsel = setFailureBit(quadsel, QuadCuts::PtOne);
        if ( PT_lep[vlidx[1]] < ptcuts4l[1] ) quadsel = setFailureBit(quadsel, QuadCuts::PtTwo);
        if ( PT_lep[vlidx[2]] < ptcuts4l[2] ) quadsel = setFailureBit(quadsel, QuadCuts::PtThree);
        if ( PT_lep[vlidx[3]] < ptcuts4l[3] ) quadsel = setFailureBit(quadsel, QuadCuts::PtFour);
// Implement 5th lepton veto.  Loop over all leptons. If lepton is not already one considered as one of the 4 high quality leptons, 
// then set veto flag if any are above the 5th lepton veto pT cut. This also includes bronze leptons. 
        for (unsigned int i = 0; i < LepQual_lep.GetSize(); ++i){             // All leptons
             if( i != vlidx[0] && i != vlidx[1] && i != vlidx[2] && i!=vlidx[3]){           // Not lepton already considered
                 ptFifthLepton = PT_lep[i];
                 if ( PT_lep[i] > ptcutVeto4l ) {
                     failFifthLeptonVeto = true;
                 }
                 break;                       // break out so we store the highest pT additional lepton independent of whether it asserts the veto
             }
        }
        if ( !passTrig ) quadsel = setFailureBit(quadsel, QuadCuts::Trigger); 
        if ( failFifthLeptonVeto ) quadsel = setFailureBit(quadsel, QuadCuts::PtFiveVeto);
        if ( !surviveBtagVeto ) quadsel = setFailureBit(quadsel, QuadCuts::BTagVeto);
        maxSIP3D4l = std::max({SIP3D_lep[vlidx[0]], SIP3D_lep[vlidx[1]], SIP3D_lep[vlidx[2]], SIP3D_lep[vlidx[3]]});
        SIP3DChisq4l = std::pow(SIP3D_lep[vlidx[0]], 2) + std::pow(SIP3D_lep[vlidx[1]], 2) + std::pow(SIP3D_lep[vlidx[2]], 2) + std::pow(SIP3D_lep[vlidx[3]], 2) ;
        if ( maxSIP3D4l > 5.0 ) quadsel = setFailureBit(quadsel, QuadCuts::SIP3DCut);
// Kinematics for the 3 G+S leptons
        FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
        FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
        FourVec l3 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[2]], PT_lep[vlidx[2]], Eta_lep[vlidx[2]], Phi_lep[vlidx[2]], M_lep[vlidx[2]]);
        FourVec l4 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[3]], PT_lep[vlidx[3]], Eta_lep[vlidx[3]], Phi_lep[vlidx[3]], M_lep[vlidx[3]]);
        FourVec l12 = l1 + l2;
        FourVec l13 = l1 + l3;
        FourVec l14 = l1 + l4;
        FourVec l23 = l2 + l3;
        FourVec l24 = l2 + l4;
        FourVec l34 = l3 + l4;
        FourVec l123 = l12 + l3;
        FourVec l124 = l12 + l4;
        FourVec l234 = l23 + l4;
        double m12 = l12.M();   double m13 = l13.M();  double m14 = l14.M(); double m23 = l23.M(); double m24 = l24.M(); double m34 = l34.M();
        double m123 = l123.M();  double m124 = l124.M();  double m234= l234.M();
        bool ossf12 = l1.osdil(l2);   bool ossf13 = l1.osdil(l3);  bool ossf23 = l2.osdil(l3);
        bool ossf14 = l1.osdil(l4);   bool ossf24 = l2.osdil(l4);  bool ossf34 = l3.osdil(l4);
        auto result = QuadLeptonInfo(m12, m13, m14, m23, m24, m34, m123, m124, m234, 
                                     ossf12, ossf13, ossf14, ossf23, ossf24, ossf34,
                                     l1.qf(), l2.qf(), l3.qf(), l4.qf() );
        ZZlike = ZZlikeEvent( l1.qf(), l2.qf(), l3.qf(), l4.qf() );
        minmll4l = std::get<0>(result);
        maxmll4l = std::get<1>(result);
        nossf4l = std::get<2>(result);
        devZ4l = std::get<3>(result);
        othermll4l = std::get<4>(result);
        minm3lfor4l = std::get<5>(result);
        minPairCode = std::get<6>(result);
        compPairCode = std::get<7>(result);
        minQPairCode = std::get<8>(result);
        compQPairCode = std::get<9>(result);
        min2lPairedMass = std::get<10>(result);
        if(minmll4l < 4.0) quadsel = setFailureBit(quadsel, QuadCuts::MinMll);
        if(minmll4l > MLLCUT4) quadsel = setFailureBit(quadsel, QuadCuts::MxMinMll);        // Needs to be checked/adjusted for each signal hypothesis
        if(std::abs(devZ4l) < 7.5) quadsel = setFailureBit(quadsel, QuadCuts::OffZ);
        FourVec l1234 = l12 + l34;
        pt4l = l1234.Pt();
        FourVec ljets = l1234 + allJets;
        pt4ljets = ljets.Pt();
        if( (*Njet == 0 && pt4l < 10.0) || (*Njet >=1 && pt4ljets < 30.0)) quadsel = setFailureBit(quadsel, QuadCuts::Pt4L);
        m4l = l1234.M();
// May be better to separate these in future.
        if( ZZlike && ( std::abs(m4l - 91.2) < 10.0  || (m4l > 121.0 && m4l < 127.0) )  ) quadsel = setFailureBit(quadsel, QuadCuts::M4LZV);
        if(othermll4l>60.0) quadsel = setFailureBit(quadsel, QuadCuts::MxCompMll);
    }
    FillQuadCutFlow(quadsel, wt);

// Similar logic for 2-lepton selection using enum class DiCuts
    unsigned int disel = 0;
    std::vector<double> ptcuts2l = {23.0, 8.0};
    double ptcutVeto2l = 2.0;   // Turn this third lepton veto off
    double sip3dcut2l = 2.0;
    double maxSIP3D2l{};
    bool failThirdLeptonVeto = false;
    double ptThirdLepton = -1.5;  // Default to invalid value


    if ( ngs < 2 ){
        disel = setFailureBit(disel, DiCuts::GSNumber);
    }
    else{   // We have at least 3 G + S leptons
        if ( PT_lep[vlidx[0]] < ptcuts2l[0] ) disel = setFailureBit(disel, DiCuts::PtOne);
        if ( PT_lep[vlidx[1]] < ptcuts2l[1] ) disel = setFailureBit(disel, DiCuts::PtTwo);
// Implement 3rd lepton veto.  Loop over all leptons. If lepton is not already one considered as one of the 2 high quality leptons, 
// then set veto flag if any are above the 3rd lepton veto pT cut. This also includes bronze leptons. 
        for (unsigned int i = 0; i < LepQual_lep.GetSize(); ++i){             // All leptons
             if( i != vlidx[0] && i != vlidx[1] ){                            // Not lepton already considered
                 ptThirdLepton = PT_lep[i];
                 if ( PT_lep[i] > ptcutVeto2l ) {
                     failThirdLeptonVeto = true;
                 }
                 break;                       // break out so we store the highest pT additional lepton independent of whether it asserts the veto
             }
        }
        if ( failThirdLeptonVeto ) disel = setFailureBit(disel, DiCuts::PtThreeVeto);
        if ( !surviveBtagVeto ) disel = setFailureBit(disel, DiCuts::BTagVeto);
        maxSIP3D2l = std::max({SIP3D_lep[vlidx[0]], SIP3D_lep[vlidx[1]]});
        if ( maxSIP3D2l > sip3dcut2l ) disel = setFailureBit(disel, DiCuts::SIP3DCut);
    }
    FillDiCutFlow(disel, ntaunus, wt);

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::PtOne )) {
        hPtOne->Fill(PT_lep[vlidx[0]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::PtTwo )) {
        hPtTwo->Fill(PT_lep[vlidx[1]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::PtThree )) {
        hPtThree->Fill(PT_lep[vlidx[2]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::Trigger )) {
// Trigger logic
         hTriTrigger->Fill(0.0, wt);
         if (*SingleElectrontrigger) hTriTrigger->Fill(1.0, wt);
         if (*SingleMuontrigger) hTriTrigger->Fill(2.0, wt);
         if (*DoubleElectrontrigger) hTriTrigger->Fill(3.0, wt);
         if (*DoubleMuontrigger) hTriTrigger->Fill(4.0, wt);
         if (*EMutrigger) hTriTrigger->Fill(5.0, wt);
         if (*METORtrigger) hTriTrigger->Fill(6.0, wt);
         if (*SingleElectrontrigger || *SingleMuontrigger) hTriTrigger->Fill(7.0, wt);
         if (*DoubleElectrontrigger || *DoubleMuontrigger || *EMutrigger) hTriTrigger->Fill(8.0, wt);
         if (*SingleElectrontrigger || *SingleMuontrigger || *DoubleElectrontrigger || *DoubleMuontrigger || *EMutrigger) hTriTrigger->Fill(9.0, wt);   
         if (*SingleElectrontrigger || *SingleMuontrigger || *DoubleElectrontrigger || *DoubleMuontrigger || *EMutrigger || *METORtrigger) hTriTrigger->Fill(10.0, wt);
         if (passTrig) hTriTrigger->Fill(11.0, wt);                
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::PtFourVeto )) {
        hPtFourVeto->Fill(ptFourthLepton , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::SIP3DCut ) ) {
        hmaxSIP3D->Fill(maxSIP3D3l , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::MinMll ) || isSelectedOrFailsJustOneCut( trisel, TriCuts::MxMinMll )) {
        hminmll->Fill(minmll3l , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::OffZ )) {
        h3ldevZ->Fill(devZ3l , wt);
        h3ldevZf->Fill(devZ3l , wt);    //finely-binned version
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::M3LZV )) {
        hm3l->Fill(m3l , wt);
        h3ldevZV->Fill(m3l-91.2, wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::FQSB )) {
        hCodes3l->Fill(minPairCode3l, compLepCode, wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::OSSF )) {
        h3lnossf->Fill(nossf3l, wt);
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::Trigger )) {
         hQuadTrigger->Fill(0.0, wt);
         if (*SingleElectrontrigger) hQuadTrigger->Fill(1.0, wt);
         if (*SingleMuontrigger) hQuadTrigger->Fill(2.0, wt);
         if (*DoubleElectrontrigger) hQuadTrigger->Fill(3.0, wt);
         if (*DoubleMuontrigger) hQuadTrigger->Fill(4.0, wt);
         if (*EMutrigger) hQuadTrigger->Fill(5.0, wt);
         if (*METORtrigger) hQuadTrigger->Fill(6.0, wt);
         if (*SingleElectrontrigger || *SingleMuontrigger) hQuadTrigger->Fill(7.0, wt);
         if (*DoubleElectrontrigger || *DoubleMuontrigger || *EMutrigger) hQuadTrigger->Fill(8.0, wt);
         if (*SingleElectrontrigger || *SingleMuontrigger || *DoubleElectrontrigger || *DoubleMuontrigger || *EMutrigger) hQuadTrigger->Fill(9.0, wt);   
         if (*SingleElectrontrigger || *SingleMuontrigger || *DoubleElectrontrigger || *DoubleMuontrigger || *EMutrigger || *METORtrigger) hQuadTrigger->Fill(10.0, wt); 
         if (passTrig) hQuadTrigger->Fill(11.0, wt);   
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::PtFiveVeto )) {
        hPtFiveVeto->Fill(ptFifthLepton , wt);
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::SIP3DCut ) ) {
        hmaxSIP3D4l->Fill(maxSIP3D4l , wt);
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::MinMll ) || isSelectedOrFailsJustOneCut( quadsel, QuadCuts::MxMinMll )) {
        hminmll4l->Fill(minmll4l , wt);
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::OffZ )) {
        h4ldevZ->Fill(devZ4l , wt);
        h4ldevZf->Fill(devZ4l , wt);
    }
    
// Funky logic here. 
// We've combined the Pt4L into two complementary parts. So events passing the 
// following logic are selected or fail one of the two mutually exclusive cuts.
    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::Pt4L )) {
        if(*Njet == 0){                   // Events are exposed to the pt4l cut
            hpt4l->Fill(pt4l, wt);
            if(pt4l > 10.0)hpt4ljets->Fill(99.5, wt);    // pt4ljets cut instance is only applied for Njet >=1. 
                                                         // Add in the Njet = 0 events that pass the pT4l > 10.0 GeV cut
        }
        else{                             // Events are exposed to the pt4ljets cut
            hpt4ljets->Fill(pt4ljets, wt);
            if(pt4ljets > 30.0)hpt4l->Fill(99.5, wt);    // pt4l cut instance is only applied for Njet == 0. 
                                                         // Add in the Njet>=1 events that pass the pT4ljets > 30 GeV cut
        }
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::M4LZV )) {
        hm4l->Fill(m4l, wt);
        hm4lf->Fill(m4l, wt);
        if(ZZlike){
            hm4lZZL->Fill(m4l, wt);
            hm4lfZZL->Fill(m4l, wt); 
            hm4lffZZL->Fill(m4l, wt);            
        }
        else{
            hm4lNZZL->Fill(m4l, wt);
            hm4lfNZZL->Fill(m4l, wt);
            hm4lffNZZL->Fill(m4l, wt);
        }
        if(ZZlike){
            h4ldevZV->Fill(m4l-91.2, wt);
        }
        else{
            h4ldevZVNZZL->Fill(m4l-91.2, wt);
        }
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::PtOne )) {
        hPtOne4l->Fill(PT_lep[vlidx[0]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::PtTwo )) {
        hPtTwo4l->Fill(PT_lep[vlidx[1]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::PtThree )) {
        hPtThree4l->Fill(PT_lep[vlidx[2]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::PtFour )) {
        hPtFour4l->Fill(PT_lep[vlidx[3]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( quadsel, QuadCuts::MxCompMll )) {
       hothermll4l->Fill(othermll4l, wt);
    }

    if( isSelected( disel) ){

         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec dilepton = l1 + l2;

         FourVec lepsPlusJets = dilepton + allJets;
//         lepsPlusJets.Print();
//         hpt4ljets->Fill(lepsPlusJets.Pt(), wt);
         hpt2lall->Fill(std::min(dilepton.Pt(), 99.99), wt);
         hpt2ljets->Fill(std::min(lepsPlusJets.Pt(), 99.99), wt);
         if(allJets.Pt() > 1.0e-6){
             hpt2lwjets->Fill(std::min(lepsPlusJets.Pt(), 99.99),  wt); 
         }
         else{
             hpt2l0jets->Fill(std::min(lepsPlusJets.Pt(), 99.99), wt); 
         }

         double mll = dilepton.M();
         hmll->Fill(mll, wt);
         hgenHT->Fill(std::min(*LHE_HT, 200.5), wt);                   // Put overflows in the 200 GeV bin

//         int charge3 = l1.lcharge() + l2.lcharge() + l3.lcharge();
         int flavor2 = l1.lflavor() + l2.lflavor();
         h2lflavor->Fill(flavor2, wt);

         if(flavor2 == 2)hmllEE->Fill(mll, wt);
         if(flavor2 == 3)hmllEM->Fill(mll, wt);
         if(flavor2 == 4)hmllMM->Fill(mll, wt);
#if ANA_NTUPLE_VERSION >= 3 && ANA_NTUPLE_FLAVOR_CODE == SIGNAL_CODE
             if(flavor2 == 2)hDiProcessEE->Fill(*cascades_prod, wt);
             if(flavor2 == 3)hDiProcessEM->Fill(*cascades_prod, wt);
             if(flavor2 == 4)hDiProcessMM->Fill(*cascades_prod, wt);
#endif 

         if(l1.lcharge() == l2.lcharge()){
             h2lflavorSS->Fill(flavor2, wt);
             if(flavor2 == 2)hmllEESS->Fill(mll, wt);
             if(flavor2 == 3)hmllEMSS->Fill(mll, wt);
             if(flavor2 == 4)hmllMMSS->Fill(mll, wt);
#if ANA_NTUPLE_VERSION >= 3 && ANA_NTUPLE_FLAVOR_CODE == SIGNAL_CODE
                 if(flavor2 == 2)hDiProcessEESS->Fill(*cascades_prod, wt);
                 if(flavor2 == 3)hDiProcessEMSS->Fill(*cascades_prod, wt);
                 if(flavor2 == 4)hDiProcessMMSS->Fill(*cascades_prod, wt);
#endif 
             int TightCharge2 = TightCharge_lep[vlidx[0]]*TightCharge_lep[vlidx[1]];
             if(TightCharge2 == 4){
                 h2lflavorSST->Fill(flavor2, wt);
                 if(flavor2 == 2)hmllEESST->Fill(mll, wt);
                 if(flavor2 == 3)hmllEMSST->Fill(mll, wt);
                 if(flavor2 == 4)hmllMMSST->Fill(mll, wt);
#if ANA_NTUPLE_VERSION >= 3 && ANA_NTUPLE_FLAVOR_CODE == SIGNAL_CODE
                     if(flavor2 == 2)hDiProcessEESST->Fill(*cascades_prod, wt);
                     if(flavor2 == 3)hDiProcessEMSST->Fill(*cascades_prod, wt);
                     if(flavor2 == 4)hDiProcessMMSST->Fill(*cascades_prod, wt);
#endif 
             }
         }
         else{
             h2lflavorOS->Fill(flavor2, wt);
             if(flavor2 == 2)hmllEEOS->Fill(mll, wt);
             if(flavor2 == 3)hmllEMOS->Fill(mll, wt);
             if(flavor2 == 4)hmllMMOS->Fill(mll, wt);
#if ANA_NTUPLE_VERSION >= 3 && ANA_NTUPLE_FLAVOR_CODE == SIGNAL_CODE
                 if(flavor2 == 2)hDiProcessEEOS->Fill(*cascades_prod, wt);
                 if(flavor2 == 3)hDiProcessEMOS->Fill(*cascades_prod, wt);
                 if(flavor2 == 4)hDiProcessMMOS->Fill(*cascades_prod, wt);
#endif 
         }

         habetaz->Fill(std::abs(dilepton.Betaz()), wt);
         hptll->Fill(dilepton.Pt(), wt);
         hacop->Fill(l1.Acop(l2), wt);
         hphistar->Fill(l1.phistar(l2), wt);
         hcosthEta->Fill(l1.CosThetaStarEta(l2), wt);

// Calculate MT2 using the Lester-Nachman implementation for various assumptions on LSP mass
         double MT2LN0   = l1.MT2(l2, fMET,   0.0,   0.0,  0.0);
         hMT2LN0->Fill ( MT2LN0, wt);
         hMT2LN0SMALL->Fill ( MT2LN0, wt);

         h2lnjets->Fill(*Njet, wt);
         h2lMET->Fill(*MET, wt);
         h2lHTid->Fill(*HT_eta24_id, wt);

         h2lgenMET->Fill(std::min(*genMET,100.0), wt);
         h2lgenMETf->Fill(std::min(*genMET,1.0), wt);
         h2lgenMETvf->Fill(*genMET, wt);
         hpt2l->Fill(dilepton.Pt(), wt);

         h2ltaunus->Fill(ntaunus, wt);
         h2lnus->Fill(*genNnu, wt);
         h2lnleptons->Fill(ngs, wt);
         hselection->Fill(2.0,wt);
         
    }


    if( isSelected( trisel ) ){

//         hCodes3l->Fill(minPairCode3l, compLepCode, wt);

         if(minPairCode3l == 3 && compLepCode == 1)hsel3P1->Fill(1.0, wt);
         if(minPairCode3l == 3 && compLepCode == 2)hsel3P2->Fill(1.0, wt);
         if(minPairCode3l == 3 && compLepCode == 3)hsel3P3->Fill(1.0, wt);
         if(minPairCode3l == 3 && compLepCode == 4)hsel3P4->Fill(1.0, wt);

         hselection->Fill(3.0,wt);

#if ANA_NTUPLE_VERSION >= 3
         hPTISR3L->Fill(myPTISR, wt);
#endif

         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec l3 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[2]], PT_lep[vlidx[2]], Eta_lep[vlidx[2]], Phi_lep[vlidx[2]], M_lep[vlidx[2]]);

         hcentraleta3l->Fill( std::min( {std::abs(Eta_lep[vlidx[0]]), std::abs(Eta_lep[vlidx[1]]), std::abs(Eta_lep[vlidx[2]])}) , wt);
         hforwardeta3l->Fill( std::max( {std::abs(Eta_lep[vlidx[0]]), std::abs(Eta_lep[vlidx[1]]), std::abs(Eta_lep[vlidx[2]])}) , wt);

         FourVec l12 = l1 + l2;
         FourVec l13 = l1 + l3;
         FourVec l23 = l2 + l3;
         FourVec l123 = l12 + l3;
         int charge3 = l1.lcharge() + l2.lcharge() + l3.lcharge();
         int flavor3 = l1.lflavor() + l2.lflavor() + l3.lflavor();
         h3lflavor->Fill(flavor3, wt);

         if ( flavor3 == 3 )hsel3C1->Fill(1.0, wt);
         if ( flavor3 == 4 )hsel3C2->Fill(1.0, wt);
         if ( flavor3 == 5 )hsel3C3->Fill(1.0, wt);
         if ( flavor3 == 6 )hsel3C4->Fill(1.0, wt);

         int TightCharge3 = TightCharge_lep[vlidx[0]]*TightCharge_lep[vlidx[1]]*TightCharge_lep[vlidx[2]];
         hmll3->Fill(l12.M(), wt);
         hmll3->Fill(l13.M(), wt);
         hmll3->Fill(l23.M(), wt);
//         hm3l->Fill(l123.M(), wt);
         hpt3l->Fill(l123.Pt(), wt);
         hminmll3l->Fill( minmll3l , wt );
         hmaxmll3l->Fill( maxmll3l , wt );
//         h3lnossf->Fill(nossf3l, wt);
//         h3ldevZ->Fill(devZ,wt);

// May be better with an enum
         if(flavor3==3)hm3lF3->Fill(l123.Mass(), wt); 
         if(flavor3==4)hm3lF4->Fill(l123.Mass(), wt); 
         if(flavor3==5)hm3lF5->Fill(l123.Mass(), wt); 
         if(flavor3==6)hm3lF6->Fill(l123.Mass(), wt); 

         if(surviveBtagVeto){
             hm3lbtv->Fill(l123.Mass(), wt);
             h3lcharge->Fill(charge3, wt);
             if(TightCharge3==8)h3lchargetight->Fill(charge3, wt);
         }

         if(charge3==-3 || charge3==3){
             hTriQ3->Fill(flavor3, wt);
             if(TightCharge3==8)hTriQ3Tight->Fill(flavor3,wt);
         }

         hmT1->Fill(l1.mtp(fMET), wt);
         hmT2->Fill(l2.mtp(fMET), wt);
         hmT3->Fill(l3.mtp(fMET), wt);
// Also fill inclusively
         hmT->Fill(l1.mtp(fMET), wt);
         hmT->Fill(l2.mtp(fMET), wt);
         hmT->Fill(l3.mtp(fMET), wt);

         hmT3l->Fill(l123.mtp(fMET), wt);

         h3lMET->Fill(fMET.Pt(), wt);
// Negative weight tests
         if(wt > 1.0e-10){
             ph3lMET->Fill(fMET.Pt(), wt);
             uph3lMET->Fill(fMET.Pt(), 1.0);
         }
         else if(wt < -1.0e-10){
             nh3lMET->Fill(fMET.Pt(), -wt);
             unh3lMET->Fill(fMET.Pt(), 1.0);
         }

         h3lHTid->Fill(*HT_eta24_id, wt);
         h3lHTnoid->Fill(*HT_eta24, wt);
         h3lmaxmT->Fill(std::max( {l1.mtp(fMET), l2.mtp(fMET), l3.mtp(fMET) } )  , wt);
         h3lnjets->Fill(*Njet, wt);
         hminSIP3D->Fill(minSIP3D3l, wt);
         h3lSIP3DRChisq->Fill(SIP3DChisq3l/3.0, wt);   // divide by degrees of freedom for this plot
         int ndof = 3;
         double pfit = TMath::Prob(SIP3DChisq3l, ndof);
         double cdf = TMath::Gamma(ndof/2.0, SIP3DChisq3l/2.0);  // CDF of chi-squared
         double localeps = 1.0e-12;
         if(cdf<= localeps){
            std::cout << "FP ERROR: cdf <= 0.0 " << cdf << " setting to epsilon " << std::endl;
            cdf = localeps;
         }
         if(cdf>= 1.0-localeps){
            std::cout << "FP ERROR: cdf >= 1.0 " << cdf << " setting to 1 - epsilon " << std::endl;
            cdf = 1.0 - localeps;
         }
         double signed_z = TMath::NormQuantile(cdf);             // z such that P(Z < z) = CDF
// Now fill the other related histogram representations
         h3lSIP3DProb->Fill(pfit, wt);
         h3lSIP3DLogProb->Fill(std::log10(pfit), wt);
         h3lSIP3DSignificance->Fill(signed_z, wt);

         h3lgenMET->Fill(std::min(*genMET,100.0), wt);
         h3lgenMETf->Fill(std::min(*genMET,1.0), wt);
         h3lgenMETvf->Fill(*genMET, wt);

         h3ltaunus->Fill(ntaunus, wt);
         h3lnus->Fill(*genNnu, wt);
         h3lnleptons->Fill(ngs, wt);

         FourVec lepsPlusJets = l123 + allJets;
         hpt3lall->Fill(std::min(l123.Pt(), 99.99), wt);
         hpt3ljets->Fill(std::min(lepsPlusJets.Pt(), 99.99), wt);
         if(allJets.Pt() > 1.0e-6){
             hpt3lwjets->Fill(std::min(lepsPlusJets.Pt(), 99.99),  wt); 
         }
         else{
             hpt3l0jets->Fill(std::min(lepsPlusJets.Pt(), 99.99), wt); 
         }


//#if ANA_NTUPLE_VERSION == 3
#if ANA_NTUPLE_VERSION >= 3 && ANA_NTUPLE_FLAVOR_CODE == SIGNAL_CODE
         hTriProcess->Fill(*cascades_prod, wt);
#endif 

    }

    if( isSelected(quadsel) ){
         hselection->Fill(4.0,wt);

#if ANA_NTUPLE_VERSION >= 3
         hPTISR4L->Fill(myPTISR, wt);
#endif

         h4lnjets->Fill(*Njet, wt);

         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec l3 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[2]], PT_lep[vlidx[2]], Eta_lep[vlidx[2]], Phi_lep[vlidx[2]], M_lep[vlidx[2]]);
         FourVec l4 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[3]], PT_lep[vlidx[3]], Eta_lep[vlidx[3]], Phi_lep[vlidx[3]], M_lep[vlidx[3]]);

         hcentraleta4l->Fill( std::min( {std::abs(Eta_lep[vlidx[0]]), std::abs(Eta_lep[vlidx[1]]), std::abs(Eta_lep[vlidx[2]]), std::abs(Eta_lep[vlidx[3]])}) , wt);
         hforwardeta4l->Fill( std::max( {std::abs(Eta_lep[vlidx[0]]), std::abs(Eta_lep[vlidx[1]]), std::abs(Eta_lep[vlidx[2]]), std::abs(Eta_lep[vlidx[3]])}) , wt);

         FourVec l12 = l1 + l2;
         FourVec l13 = l1 + l3;
         FourVec l14 = l1 + l4;
         FourVec l23 = l2 + l3;
         FourVec l24 = l2 + l4;
         FourVec l34 = l3 + l4;
         FourVec l123 = l12 + l3;
         FourVec l124 = l12 + l4;
         FourVec l134 = l13 + l4;
         FourVec l234 = l23 + l4;
         FourVec l1234 = l12 + l34;
         std::cout << "Selected 4-lepton event " << *eventnum << endl;
         l1234.Print();
         allJets.Print();
         FourVec lepsPlusJets = l1234 + allJets;
         lepsPlusJets.Print();
//         hpt4ljets->Fill(lepsPlusJets.Pt(), wt);
         if(allJets.Pt() > 1.0e-6){
             hpt4lwjets->Fill(lepsPlusJets.Pt(), wt); 
         }
         else{
             hpt4l0jets->Fill(lepsPlusJets.Pt(), wt); 
             hpt4lwjets->Fill(99.5, wt);                  // So that we can easy calculate the performance metric effect of varying a cut on this histogram
         }

         int charge4 = l1.lcharge() + l2.lcharge() + l3.lcharge() + l4.lcharge();
         int TightCharge4 = TightCharge_lep[vlidx[0]]*TightCharge_lep[vlidx[1]]*TightCharge_lep[vlidx[2]]*TightCharge_lep[vlidx[3]];

         int flavor4 = l1.lflavor() + l2.lflavor() + l3.lflavor() + l4.lflavor();
         h4lflavor->Fill(flavor4, wt);

         if ( flavor4 == 4 )hsel4C1->Fill(1.0, wt);
         if ( flavor4 == 5 )hsel4C2->Fill(1.0, wt);
         if ( flavor4 == 6 )hsel4C3->Fill(1.0, wt);
         if ( flavor4 == 7 )hsel4C4->Fill(1.0, wt);
         if ( flavor4 == 8 )hsel4C5->Fill(1.0, wt);

    //     hm4l->Fill(l1234.Mass(), wt);
         if(surviveBtagVeto){
             hm4lbtv->Fill(l1234.Mass(), wt);
             h4lcharge->Fill(charge4, wt);
             if(TightCharge4==16)h4lchargetight->Fill(charge4, wt);
         }
         h4lnossf->Fill(nossf4l, wt);

         h4lSIP3DRChisq->Fill(SIP3DChisq4l/4.0, wt);   // divide by degrees of freedom for this plot
         int ndof = 4;
         double pfit = TMath::Prob(SIP3DChisq4l, ndof);
         double cdf = TMath::Gamma(ndof/2.0, SIP3DChisq4l/2.0);  // CDF of chi-squared
         double localeps = 1.0e-12;
         bool cdferror = false;
         if(cdf<= localeps){
            std::cout << "4l FP ERROR: cdf <= 0.0 " << cdf << " setting to epsilon " << " SIP3DChisq4l = " << SIP3DChisq4l << std::endl;
            cdf = localeps;
            cdferror = true;
         }
         if(cdf>= 1.0-localeps){
            std::cout << "4l FP ERROR: cdf >= 1.0 " << cdf << " setting to 1 - epsilon " << " SIP3DChisq4l = " << SIP3DChisq4l << std::endl;
            cdf = 1.0 - localeps;
            cdferror = true;
         }
         double signed_z = TMath::NormQuantile(cdf);             // z such that P(Z < z) = CDF
         if(cdferror)std::cout << "4l FP signed z " << signed_z << std::endl;
// Now fill the other related histogram representations
         h4lSIP3DProb->Fill(pfit, wt);
         h4lSIP3DLogProb->Fill(std::log10(pfit), wt);
         h4lSIP3DSignificance->Fill(signed_z, wt);

         if(charge4==-2 || charge4==2){
             hQuadQ2->Fill(flavor4, wt);
             if(TightCharge4==16)hQuadQ2Tight->Fill(flavor4,wt);
         }

         h4lgenMET->Fill(std::min(*genMET,100.0), wt);
         h4lgenMETf->Fill(std::min(*genMET,1.0), wt);
         h4lgenMETvf->Fill(*genMET, wt);
//         hpt4l->Fill(l1234.Pt(), wt);
         h4ltaunus->Fill(ntaunus, wt);
         h4lnus->Fill(*genNnu, wt);
         h4lnleptons->Fill(ngs, wt);
         if(ngs==4)hsel4->Fill(1.0, wt);
         if(ngs>=5)hsel5->Fill(1.0, wt);
         if(ngs>=6)hsel6->Fill(1.0, wt);
//         hothermll4l->Fill(othermll4l, wt);
         hminm3lfor4l->Fill(minm3lfor4l, wt);
         hmincmass4l->Fill(std::min(othermll4l, minm3lfor4l), wt);
         hmincmassp4l->Fill(std::min(othermll4l, 0.5*minm3lfor4l), wt);
         hmincmasspp4l->Fill(std::min(othermll4l, 0.75*minm3lfor4l), wt);

         h4lMET->Fill(fMET.Pt(), wt);
         h4lHTid->Fill(*HT_eta24_id, wt);
         h4lHTnoid->Fill(*HT_eta24, wt);

         h4lmasses->Fill(minmll4l, othermll4l, wt);
         h4lOmasses->Fill(minm3lfor4l, othermll4l, wt);
         h4lPmasses->Fill(minmll4l, minm3lfor4l ,wt);

         if(charge4==-2 || charge4==2)hsel4C6->Fill(1.0, wt);
         if( (charge4==-2 || charge4==2) && TightCharge4==16)hsel4C7->Fill(1.0, wt);
         if( charge4==-2 || charge4==2 || ngs>=5 )hsel4C8->Fill(1.0, wt);
         if( ((charge4==-2 || charge4==2) && TightCharge4==16)  || ngs >= 5 )hsel4C9->Fill(1.0, wt);
         if( ngs >= 5 )hsel4C10->Fill(1.0, wt);

         double m4l = l1234.Mass();

         if( ngs >= 5){
            hsel4Exclusive->Fill(1.0, wt);
         }
         else if( ((charge4==-2 || charge4==2) && TightCharge4==16) ){
            hsel4Exclusive->Fill(2.0, wt);            
         }
         else{
            hsel4Exclusive->Fill(flavor4 - 1, wt);
            hMinm3lOverm4l->Fill(minm3lfor4l/m4l, wt);  
            hMin2lMassOverm4l->Fill(min2lPairedMass/m4l, wt);
            h4lMassRatios->Fill(min2lPairedMass/m4l, minm3lfor4l/m4l, wt); 
            h4lMassRatiosFine->Fill(min2lPairedMass/m4l, minm3lfor4l/m4l, wt);  

            hAMinm3l->Fill(minm3lfor4l, wt);  
            hAMin2lMass->Fill(min2lPairedMass, wt);
            hA4lMasses->Fill(min2lPairedMass, minm3lfor4l, wt); 
            hA4lMassesFine->Fill(min2lPairedMass, minm3lfor4l, wt);   

            cout << "4l masses debug: Event " << *eventnum << endl;
            l1.Print();
            l2.Print();
            l3.Print();
            l4.Print();
            cout << "mij (12,13,14,23,24,34): " << l12.Mass() << " " << l13.Mass() << " " << l14.Mass() << " " << l23.Mass() << " " << l24.Mass() << " " << l34.Mass() << endl;  
            cout << "mij+mkl (12+34,13+24,14+23): " << l12.Mass()+l34.Mass() << " " << l13.Mass()+l24.Mass() << " " << l14.Mass()+l23.Mass() << endl; 
            cout << "mijk (123,124,134,234) : " << l123.Mass() << " " << l124.Mass() << " " << l134.Mass() << " " << l234.Mass() << endl; 

            cout << "minmll4l, othermll4l, maxmll4l " << minmll4l << " " << othermll4l << " " << maxmll4l << endl;
            cout << "m4l: " << m4l << " 3l: " << minm3lfor4l << " 2lPaired: " << min2lPairedMass << endl;
            double threePlusOneMass = std::sqrt(minm3lfor4l*minm3lfor4l/3.0);
            double twoPlusTwoMass = std::sqrt(min2lPairedMass*min2lPairedMass/2.0);
            double factor = twoPlusTwoMass/(twoPlusTwoMass+threePlusOneMass);
            cout << "3+1?: " << threePlusOneMass << " 2+2?: " << twoPlusTwoMass << " Factor (<0.5 =>2+2) " << factor << endl;
            h4lFactor->Fill(factor, wt);
          
         }

         hpairCodes4l->Fill(minPairCode, compPairCode, wt);          // These are in the range 1 to 6
         int altminPairCode = NewPairCode(minPairCode);              
         int altcompPairCode = NewPairCode(compPairCode);
         hpairCodesA4l->Fill(altminPairCode, altcompPairCode, wt);   // These are in the range 1 to 4 
         hpairCodesQ4l->Fill(altminPairCode, altcompPairCode, wt);
         hpairCodesF4l->Fill((altminPairCode+1)%2, (altcompPairCode+1)%2, wt);  // These are in the range 0, 1
         hpairCodesL4l->Fill((minPairCode+2)%3, (compPairCode+2)%3, wt);  // These are in the range 0, 1, 2

         hpairCodesC4l->Fill(minQPairCode, compQPairCode, wt);   // These are in the range 1 to 9.
         hpairCodesCQ4l->Fill(minQPairCode, compQPairCode, wt);   // These are in the range 1 to 9.

//         std::cout << "4l selected. " << *eventnum << " genNnu " << *genNnu << " ntaunus: " << ntaunus << " 4l pt: " << l1234.Pt() << " Njet: " << *Njet << " PTCM: " << *PTCM << " MET: " << *MET << " genMET " << *genMET << std::endl;

#if ANA_NTUPLE_VERSION >= 3 && ANA_NTUPLE_FLAVOR_CODE == SIGNAL_CODE
         hQuadProcess->Fill(*cascades_prod, wt);
         int iprocess = ProcessInfo(cprod, dk1, dk2, N2dk1, N2dk2);
         if(iprocess==1)h4lmasses1->Fill(minmll4l, othermll4l, wt);
         if(iprocess==2)h4lmasses2->Fill(minmll4l, othermll4l, wt);
         if(iprocess==3)h4lmasses3->Fill(minmll4l, othermll4l, wt);
         if(iprocess==1)h4lOmasses1->Fill(minm3lfor4l, othermll4l, wt);
         if(iprocess==2)h4lOmasses2->Fill(minm3lfor4l, othermll4l, wt);
         if(iprocess==3)h4lOmasses3->Fill(minm3lfor4l, othermll4l, wt);
         if(iprocess==1)h4lPmasses1->Fill(minmll4l, minm3lfor4l, wt);
         if(iprocess==2)h4lPmasses2->Fill(minmll4l, minm3lfor4l, wt);
         if(iprocess==3)h4lPmasses3->Fill(minmll4l, minm3lfor4l, wt);
         if(ngs>=5)hsel5process->Fill(iprocess, wt);
/*
         if( (charge4==-2 || charge4==2) && TightCharge4==16){
//         if(ngs>=5){
             hsel4Q2process->Fill(iprocess, wt);
             cout << "ngs = " << ngs << " Flavor4 = " << flavor4 << " charge4 = " << charge4 << endl;
             cout << "minmll = " << minmll4l << " othermll = " << othermll4l << " minm3lfor4l = " << minm3lfor4l << endl;
             double nw=1.0;
             if(inputFilePrefix=="SlepSneu-180"){
                 nw = CascadeInfo(evn, ctree, cprod, dk1, dk2, N2dk1, N2dk2, 
                                    false, true, "default", myTarget);
             }
             else if (inputFilePrefix=="SlepSneu-220" || 
                 inputFilePrefix=="SlepSneu-260" || 
                 inputFilePrefix=="SlepSneu-270"){
                 nw = CascadeInfo(evn, ctree, cprod, dk1, dk2, N2dk1, N2dk2, 
                                    false, true, "deprecated", myTarget);
             }
         }
*/
#endif 
    }

    if( isSelected(disel)  && isSelected(trisel)  )hselection->Fill(5.0, wt);
    if( isSelected(disel)  && isSelected(quadsel) )hselection->Fill(6.0, wt);
    if( isSelected(trisel) && isSelected(quadsel) )hselection->Fill(7.0, wt);
    if( isSelected(disel)  && isSelected(trisel) && isSelected(quadsel) )hselection->Fill(8.0, wt);

    return true;

}

void Ana::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void SetCodeLabels6(TH2* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labels = {"OSee", "OS#mu#mu", "OSe#mu", "SSee", "SS#mu#mu", "SSe#mu"};

    TAxis* xaxis = h->GetXaxis();
    TAxis* yaxis = h->GetYaxis();

    for (size_t i = 0; i < labels.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labels[i].c_str());
        if (bin <= yaxis->GetNbins()) yaxis->SetBinLabel(bin, labels[i].c_str());
    }

    xaxis->SetLabelSize(0.05);
    yaxis->SetLabelSize(0.05);

}

void SetCodeLabels4(TH2* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labels = {"OSSF", "OSDF", "SSSF", "SSDF"};

    TAxis* xaxis = h->GetXaxis();
    TAxis* yaxis = h->GetYaxis();

    for (size_t i = 0; i < labels.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labels[i].c_str());
        if (bin <= yaxis->GetNbins()) yaxis->SetBinLabel(bin, labels[i].c_str());
    }

    xaxis->SetLabelSize(0.05);
    yaxis->SetLabelSize(0.05);

}

void SetCodeLabels2Q(TH2* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labels = {"OS", "SS"};

    TAxis* xaxis = h->GetXaxis();
    TAxis* yaxis = h->GetYaxis();

    for (size_t i = 0; i < labels.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labels[i].c_str());
        if (bin <= yaxis->GetNbins()) yaxis->SetBinLabel(bin, labels[i].c_str());
    }

    xaxis->SetLabelSize(0.075);
    yaxis->SetLabelSize(0.075);

}

void SetCodeLabels2F(TH2* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labels = {"SF", "DF"};

    TAxis* xaxis = h->GetXaxis();
    TAxis* yaxis = h->GetYaxis();

    for (size_t i = 0; i < labels.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labels[i].c_str());
        if (bin <= yaxis->GetNbins()) yaxis->SetBinLabel(bin, labels[i].c_str());
    }

    xaxis->SetLabelSize(0.075);
    yaxis->SetLabelSize(0.075);

}

void SetCodeLabels3F(TH2* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labels = {"ee", "#mu#mu", "e#mu"};

    TAxis* xaxis = h->GetXaxis();
    TAxis* yaxis = h->GetYaxis();

    for (size_t i = 0; i < labels.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labels[i].c_str());
        if (bin <= yaxis->GetNbins()) yaxis->SetBinLabel(bin, labels[i].c_str());
    }

    xaxis->SetLabelSize(0.075);
    yaxis->SetLabelSize(0.075);

}

void SetCodeLabelsT(TH2* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labelsx = {"OSee", "OS#mu#mu", "OSe#mu", "NSSee", "NSS#mu#mu", "NSSe#mu", "PSSee", "PSS#mu#mu", "PSSe#mu"};
    std::vector<std::string> labelsy = {"e^{+}", "#mu^{+}", "e^{-}", "#mu^{-}"};

    TAxis* xaxis = h->GetXaxis();
    TAxis* yaxis = h->GetYaxis();

    for (size_t i = 0; i < labelsx.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labelsx[i].c_str());
    }

    for (size_t i = 0; i < labelsy.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= yaxis->GetNbins()) yaxis->SetBinLabel(bin, labelsy[i].c_str());
    }

    xaxis->SetLabelSize(0.045);
    yaxis->SetLabelSize(0.075);

}

void SetCodeLabelsC(TH2* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labelsx = {"OSee", "OS#mu#mu", "OSe#mu", "NSSee", "NSS#mu#mu", "NSSe#mu", "PSSee", "PSS#mu#mu", "PSSe#mu"};
    std::vector<std::string> labelsy = {"OSee", "OS#mu#mu", "OSe#mu", "NSSee", "NSS#mu#mu", "NSSe#mu", "PSSee", "PSS#mu#mu", "PSSe#mu"};

    TAxis* xaxis = h->GetXaxis();
    TAxis* yaxis = h->GetYaxis();

    for (size_t i = 0; i < labelsx.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labelsx[i].c_str());
    }

    for (size_t i = 0; i < labelsy.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= yaxis->GetNbins()) yaxis->SetBinLabel(bin, labelsy[i].c_str());
    }

    xaxis->SetLabelSize(0.045);
    yaxis->SetLabelSize(0.045);

}

void SetCodeLabelsCQ(TH2* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labelsx = {"OS", "NSS", "PSS"};
    std::vector<std::string> labelsy = {"OS", "NSS", "PSS"};

    TAxis* xaxis = h->GetXaxis();
    TAxis* yaxis = h->GetYaxis();

    for (size_t i = 0; i < labelsx.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labelsx[i].c_str());
    }

    for (size_t i = 0; i < labelsy.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= yaxis->GetNbins()) yaxis->SetBinLabel(bin, labelsy[i].c_str());
    }

    xaxis->SetLabelSize(0.075);
    yaxis->SetLabelSize(0.075);

}

void SetProcessLabels(TH1* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labels = { "ee", "ev+", "ev-", "veve", "#mu#mu", "#muv+", "#muv-", "v#muv#mu"};

    TAxis* xaxis = h->GetXaxis();

    for (size_t i = 0; i < labels.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labels[i].c_str());
    }

    xaxis->SetLabelSize(0.06);

}

void SetCodeLabels4E(TH1* h) {

    if (!h) return;  // safety check

    std::vector<std::string> labels = { "5L", "|Q|=2", "4e", "eee#mu", "ee#mu#mu", "e#mu#mu#mu", "4#mu"};

    TAxis* xaxis = h->GetXaxis();

    for (size_t i = 0; i < labels.size(); ++i) {
        int bin = i+1; // ROOT bins are 1-based
        if (bin <= xaxis->GetNbins()) xaxis->SetBinLabel(bin, labels[i].c_str());
    }

    xaxis->SetLabelSize(0.06);

}

void Ana::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

// These bin labels can be done at Terminate stage.

    hDiCutFlow->GetXaxis()->SetBinLabel(1, "All");
    hDiCutFlow->GetXaxis()->SetBinLabel(2, "GSNumber");
    hDiCutFlow->GetXaxis()->SetBinLabel(3, "Pt1");
    hDiCutFlow->GetXaxis()->SetBinLabel(4, "Pt2");
    hDiCutFlow->GetXaxis()->SetBinLabel(5, "Pt3Veto");
    hDiCutFlow->GetXaxis()->SetBinLabel(6, "BTagVeto");
    hDiCutFlow->GetXaxis()->SetBinLabel(7, "SIP3DCut");
    hDiCutFlow->GetXaxis()->SetBinLabel(8, "Selected");
    std::cout << "Dilepton selected event count " << hDiCutFlow->GetBinContent(8) << " " << hDiCutFlow->GetBinError(8) <<  std::endl;
    std::cout << "DileptonNonTau    event count " << hDiCutFlowNonTau->GetBinContent(8) << " " << hDiCutFlowNonTau->GetBinError(8) <<  std::endl;

    hXDiCutFlow->GetXaxis()->SetBinLabel(1, "Selected");
    hXDiCutFlow->GetXaxis()->SetBinLabel(2, "GSNumber");
    hXDiCutFlow->GetXaxis()->SetBinLabel(3, "Pt1");
    hXDiCutFlow->GetXaxis()->SetBinLabel(4, "Pt2");
    hXDiCutFlow->GetXaxis()->SetBinLabel(5, "Pt3Veto");
    hXDiCutFlow->GetXaxis()->SetBinLabel(6, "BTagVeto");
    hXDiCutFlow->GetXaxis()->SetBinLabel(7, "SIP3DCut");

    hCutFlow->GetXaxis()->SetBinLabel(1, "All");
    hCutFlow->GetXaxis()->SetBinLabel(2, "N(G+S)");
    hCutFlow->GetXaxis()->SetBinLabel(3, "Pt1");
    hCutFlow->GetXaxis()->SetBinLabel(4, "Pt2");
    hCutFlow->GetXaxis()->SetBinLabel(5, "Pt3");
    hCutFlow->GetXaxis()->SetBinLabel(6, "TRG");
    hCutFlow->GetXaxis()->SetBinLabel(7, "Pt4Veto");
    hCutFlow->GetXaxis()->SetBinLabel(8, "BTagVeto");
    hCutFlow->GetXaxis()->SetBinLabel(9, "SIP3DCut");
    hCutFlow->GetXaxis()->SetBinLabel(10,  "MnMll");
    hCutFlow->GetXaxis()->SetBinLabel(11, "MxMnMll");
    hCutFlow->GetXaxis()->SetBinLabel(12, "OffZ");
    hCutFlow->GetXaxis()->SetBinLabel(13, "Pt3L");
    hCutFlow->GetXaxis()->SetBinLabel(14, "M3LZV");
    hCutFlow->GetXaxis()->SetBinLabel(15, "FQSB");
    hCutFlow->GetXaxis()->SetBinLabel(16, "OSSF");
    hCutFlow->GetXaxis()->SetBinLabel(17, "Selected");
    std::cout << "Trilepton selected event count " << hCutFlow->GetBinContent(17) << " " << hCutFlow->GetBinError(17) << std::endl;

    hXCutFlow->GetXaxis()->SetBinLabel(1, "Selected");
    hXCutFlow->GetXaxis()->SetBinLabel(2, "N(G+S)");
    hXCutFlow->GetXaxis()->SetBinLabel(3, "Pt1");
    hXCutFlow->GetXaxis()->SetBinLabel(4, "Pt2");
    hXCutFlow->GetXaxis()->SetBinLabel(5, "Pt3");
    hXCutFlow->GetXaxis()->SetBinLabel(6, "TRG");
    hXCutFlow->GetXaxis()->SetBinLabel(7, "Pt4Veto");
    hXCutFlow->GetXaxis()->SetBinLabel(8, "BTagVeto");
    hXCutFlow->GetXaxis()->SetBinLabel(9, "SIP3DCut");
    hXCutFlow->GetXaxis()->SetBinLabel(10,  "MnMll");
    hXCutFlow->GetXaxis()->SetBinLabel(11, "MxMnMll");
    hXCutFlow->GetXaxis()->SetBinLabel(12, "OffZ");
    hXCutFlow->GetXaxis()->SetBinLabel(13, "Pt3L");
    hXCutFlow->GetXaxis()->SetBinLabel(14, "M3LZV");
    hXCutFlow->GetXaxis()->SetBinLabel(15, "FQSB");
    hXCutFlow->GetXaxis()->SetBinLabel(16, "OSSF");

    hQuadCutFlow->GetXaxis()->SetBinLabel(1, "All");
    hQuadCutFlow->GetXaxis()->SetBinLabel(2, "N(G+S)");
    hQuadCutFlow->GetXaxis()->SetBinLabel(3, "Pt1");
    hQuadCutFlow->GetXaxis()->SetBinLabel(4, "Pt2");
    hQuadCutFlow->GetXaxis()->SetBinLabel(5, "Pt3");
    hQuadCutFlow->GetXaxis()->SetBinLabel(6, "Pt4");
    hQuadCutFlow->GetXaxis()->SetBinLabel(7, "TRG");
    hQuadCutFlow->GetXaxis()->SetBinLabel(8, "Pt5Veto");
    hQuadCutFlow->GetXaxis()->SetBinLabel(9, "BTagVeto");
    hQuadCutFlow->GetXaxis()->SetBinLabel(10, "SIP3DCut");
    hQuadCutFlow->GetXaxis()->SetBinLabel(11,  "MnMll");
    hQuadCutFlow->GetXaxis()->SetBinLabel(12, "MxMnMll");
    hQuadCutFlow->GetXaxis()->SetBinLabel(13, "OffZ");
    hQuadCutFlow->GetXaxis()->SetBinLabel(14, "Pt4L");
    hQuadCutFlow->GetXaxis()->SetBinLabel(15, "M4LZV");
    hQuadCutFlow->GetXaxis()->SetBinLabel(16, "MxCompMll");
    hQuadCutFlow->GetXaxis()->SetBinLabel(17, "Selected");
    std::cout << "Quadlepton selected event count " << hQuadCutFlow->GetBinContent(17) << " " << hQuadCutFlow->GetBinError(17) << std::endl;

    hXQuadCutFlow->GetXaxis()->SetBinLabel(1, "Selected");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(2, "N(G+S)");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(3, "Pt1");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(4, "Pt2");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(5, "Pt3");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(6, "Pt4");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(7, "TRG");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(8, "Pt5Veto");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(9, "BTagVeto");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(10, "SIP3DCut");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(11,  "MnMll");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(12, "MxMnMll");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(13, "OffZ");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(14, "Pt4L");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(15, "M4LZV");
    hXQuadCutFlow->GetXaxis()->SetBinLabel(16, "MxCompMll");

    h3lflavor->GetXaxis()->SetBinLabel(1,"eee");
    h3lflavor->GetXaxis()->SetBinLabel(2,"ee#mu");
    h3lflavor->GetXaxis()->SetBinLabel(3,"e#mu#mu");
    h3lflavor->GetXaxis()->SetBinLabel(4,"#mu#mu#mu");
    h3lflavor->GetXaxis()->SetLabelSize(0.07);

    hTriQ3->GetXaxis()->SetBinLabel(1,"eee");
    hTriQ3->GetXaxis()->SetBinLabel(2,"ee#mu");
    hTriQ3->GetXaxis()->SetBinLabel(3,"e#mu#mu");
    hTriQ3->GetXaxis()->SetBinLabel(4,"#mu#mu#mu");
    hTriQ3->GetXaxis()->SetLabelSize(0.07);

    hTriQ3Tight->GetXaxis()->SetBinLabel(1,"eee");
    hTriQ3Tight->GetXaxis()->SetBinLabel(2,"ee#mu");
    hTriQ3Tight->GetXaxis()->SetBinLabel(3,"e#mu#mu");
    hTriQ3Tight->GetXaxis()->SetBinLabel(4,"#mu#mu#mu");
    hTriQ3Tight->GetXaxis()->SetLabelSize(0.07);

    h4lflavor->GetXaxis()->SetBinLabel(1,"eeee");
    h4lflavor->GetXaxis()->SetBinLabel(2,"eee#mu");
    h4lflavor->GetXaxis()->SetBinLabel(3,"ee#mu#mu");
    h4lflavor->GetXaxis()->SetBinLabel(4,"e#mu#mu#mu");
    h4lflavor->GetXaxis()->SetBinLabel(5,"#mu#mu#mu#mu");
    h4lflavor->GetXaxis()->SetLabelSize(0.06);

    hQuadQ2->GetXaxis()->SetBinLabel(1,"eeee");
    hQuadQ2->GetXaxis()->SetBinLabel(2,"eee#mu");
    hQuadQ2->GetXaxis()->SetBinLabel(3,"ee#mu#mu");
    hQuadQ2->GetXaxis()->SetBinLabel(4,"e#mu#mu#mu");
    hQuadQ2->GetXaxis()->SetBinLabel(5,"#mu#mu#mu#mu");
    hQuadQ2->GetXaxis()->SetLabelSize(0.06);

    hQuadQ2Tight->GetXaxis()->SetBinLabel(1,"eeee");
    hQuadQ2Tight->GetXaxis()->SetBinLabel(2,"eee#mu");
    hQuadQ2Tight->GetXaxis()->SetBinLabel(3,"ee#mu#mu");
    hQuadQ2Tight->GetXaxis()->SetBinLabel(4,"e#mu#mu#mu");
    hQuadQ2Tight->GetXaxis()->SetBinLabel(5,"#mu#mu#mu#mu");
    hQuadQ2Tight->GetXaxis()->SetLabelSize(0.06);

    SetProcessLabels(hprocesslowt);
    SetProcessLabels(hprocesshiwt);
    SetProcessLabels(hTriProcess);
    SetProcessLabels(hQuadProcess);

    SetProcessLabels(hDiProcessEE);
    SetProcessLabels(hDiProcessEEOS);
    SetProcessLabels(hDiProcessEESS);
    SetProcessLabels(hDiProcessEESST);

    SetProcessLabels(hDiProcessEM);
    SetProcessLabels(hDiProcessEMOS);
    SetProcessLabels(hDiProcessEMSS);
    SetProcessLabels(hDiProcessEMSST);

    SetProcessLabels(hDiProcessMM);
    SetProcessLabels(hDiProcessMMOS);
    SetProcessLabels(hDiProcessMMSS);
    SetProcessLabels(hDiProcessMMSST);

    hTriTrigger->GetXaxis()->SetBinLabel(1,"None");
    hTriTrigger->GetXaxis()->SetBinLabel(2,"1e");
    hTriTrigger->GetXaxis()->SetBinLabel(3,"1mu");
    hTriTrigger->GetXaxis()->SetBinLabel(4,"2e");
    hTriTrigger->GetXaxis()->SetBinLabel(5,"2mu");
    hTriTrigger->GetXaxis()->SetBinLabel(6,"emu");
    hTriTrigger->GetXaxis()->SetBinLabel(7,"MET");
    hTriTrigger->GetXaxis()->SetBinLabel(8,"1l");
    hTriTrigger->GetXaxis()->SetBinLabel(9,"2l");
    hTriTrigger->GetXaxis()->SetBinLabel(10,"1l+2l");
    hTriTrigger->GetXaxis()->SetBinLabel(11,"1l+2l+MET");
    hTriTrigger->GetXaxis()->SetBinLabel(12,"TRIG");
    hTriTrigger->GetXaxis()->SetLabelSize(0.04);

    hQuadTrigger->GetXaxis()->SetBinLabel(1,"None");
    hQuadTrigger->GetXaxis()->SetBinLabel(2,"1e");
    hQuadTrigger->GetXaxis()->SetBinLabel(3,"1mu");
    hQuadTrigger->GetXaxis()->SetBinLabel(4,"2e");
    hQuadTrigger->GetXaxis()->SetBinLabel(5,"2mu");
    hQuadTrigger->GetXaxis()->SetBinLabel(6,"emu");
    hQuadTrigger->GetXaxis()->SetBinLabel(7,"MET");
    hQuadTrigger->GetXaxis()->SetBinLabel(8,"1l");
    hQuadTrigger->GetXaxis()->SetBinLabel(9,"2l");
    hQuadTrigger->GetXaxis()->SetBinLabel(10,"1l+2l");
    hQuadTrigger->GetXaxis()->SetBinLabel(11,"1l+2l+MET");
    hQuadTrigger->GetXaxis()->SetBinLabel(12,"TRIG");
    hQuadTrigger->GetXaxis()->SetLabelSize(0.04);

    hselection->GetXaxis()->SetBinLabel(1,"2L");
    hselection->GetXaxis()->SetBinLabel(2,"3L");
    hselection->GetXaxis()->SetBinLabel(3,"4L");
    hselection->GetXaxis()->SetBinLabel(4,"23L");
    hselection->GetXaxis()->SetBinLabel(5,"24L");
    hselection->GetXaxis()->SetBinLabel(6,"34L");
    hselection->GetXaxis()->SetBinLabel(7,"234L");
    hselection->GetXaxis()->SetLabelSize(0.05);

    SetCodeLabels6(hpairCodes4l);
    SetCodeLabels4(hpairCodesA4l);
    SetCodeLabels2Q(hpairCodesQ4l);
    SetCodeLabels2F(hpairCodesF4l);
    SetCodeLabels3F(hpairCodesL4l);
    SetCodeLabelsC(hpairCodesC4l);
    SetCodeLabelsCQ(hpairCodesCQ4l);
    SetCodeLabelsT(hCodes3l);
    SetCodeLabels4E(hsel4Exclusive);

   // Get a list of all objects in memory (for saving histograms)
   TList *list = gDirectory->GetList();
   //   list->ls("-m");

   // Save the histograms to a file - good for eg. fitting in the future

   std:string rfile ="";
   if(treeName == "KUAnalysis"){
       rfile = inputFilePrefix + "_histos.root";
   }
   else{
       rfile = treeName + "_" + inputFilePrefix + "_histos.root";       
   }
   std::cout << "rfile = " << rfile << endl;
   TFile *fout = new TFile(rfile.c_str(),"RECREATE");

// We loop over all objects in memory and save any TH1 histograms. 
   TObject *obj;
   TIter iter(list);
// See https://root.cern.ch/root/roottalk/roottalk98/1015.html
   while( (obj = (TObject*) iter()) ){
        if( (obj->IsA()->InheritsFrom("TH1")) || (obj->IsA()->InheritsFrom("TH2"))  ){
           obj->Draw();
           obj->Write();
        }
   }
   fout->Close();

}
