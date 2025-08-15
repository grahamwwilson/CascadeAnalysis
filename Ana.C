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

#include "Ana.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <string>
#include <tuple>
#include "FourVec.h"
#include "DiCuts.h"       // Dilepton selection
#include "TriCuts.h"      // Trilepton selection
#include "QuadCuts.h"     // Quadlepton selection
#include <algorithm>
#include <bitset> 
#include <TMath.h>
#include <array>
#include <cassert>

// Declare histograms globally here
#include "AnaHistos.h"    // Try moving to SlaveBegin ?
// Declare helper functions with bits for selection
#include "SelectionBits.h"

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


double CascadeInfoOld(int evn, int tree, int prod, int dk1, int dk2, int N2dk1, int N2dk2, bool apply=false, bool debug=false){

// Initially use this to debug the event-level cascade model generator information
// so as to permit reweighting to other branching fractions.

    double newWeight = 1.0;

// Decode slepton IDs. Don't worry for now about slepton/anti-slepton. And use PDGIDs for leptons as a short-hand
    int slep1 = 0; int slep2 = 0;
    if(prod == 1 || prod == 2 || prod == 3){
        slep1 = 11;
    }
    if(prod == 2 || prod == 3 || prod == 4){
        slep2 = 12;
    }
    if(prod == 1)slep2 = 11;
    if(prod == 4)slep1 = 12;

    if(prod == 5 || prod == 6 || prod == 7){
        slep1 = 13;
    }
    if(prod == 6 || prod == 7 || prod == 8){
        slep2 = 14;
    }
    if(prod == 5)slep2 = 13;
    if(prod == 8)slep1 = 14;

// Current branching fraction triangle settings (correct for LSP-180 case)
    double bslepA = 0.50; double bslepB = 0.25; double bslepC = 0.25;
    double bsneuA = 0.25; double bsneuB = 0.25; double bsneuC = 0.50;
    double bneutA = 0.04; double bneutB = 0.48; double bneutC = 0.48;
//    double bneutA = 0.05; double bneutB = 0.85; double bneutC = 0.15;

// Target branching fractions
    double tbslepA = 0.00; double tbslepB = 1.00; double tbslepC = 0.00;
    double tbsneuA = 0.00; double tbsneuB = 1.00; double tbsneuC = 0.00;
    double tbneutA = 0.00; double tbneutB = 0.00; double tbneutC = 1.00;

// Now identify the actual decay mode for each slepton - here we obey slepton universality ..
    if(slep1 == 11 || slep1 == 13){
        if(dk1 == 1)newWeight*=(tbslepA/bslepA);
        if(dk1 == 3)newWeight*=(tbslepC/bslepC);
        if(dk1 == 2){
            newWeight*=(tbslepB/bslepB);
            if(N2dk1==1)newWeight*=(tbneutA/bneutA);
            if(N2dk1==2)newWeight*=(tbneutB/bneutB);
            if(N2dk1==0||N2dk1==3)newWeight*=(tbneutC/bneutC);
        }
    }
    if(slep2 == 11 || slep2 == 13){
        if(dk2 == 1)newWeight*=(tbslepA/bslepA);
        if(dk2 == 3)newWeight*=(tbslepC/bslepC);
        if(dk2 == 2){
            newWeight*=(tbslepB/bslepB);
            if(N2dk2==1)newWeight*=(tbneutA/bneutA);
            if(N2dk2==2)newWeight*=(tbneutB/bneutB);
            if(N2dk2==0||N2dk2==3)newWeight*=(tbneutC/bneutC);
        }
    }
    if(slep1 == 12 || slep1 == 14){
        if(dk1 == 1)newWeight*=(tbsneuA/bsneuA);
        if(dk1 == 3)newWeight*=(tbsneuC/bsneuC);
        if(dk1 == 2){
            newWeight*=(tbsneuB/bsneuB);
            if(N2dk1==1)newWeight*=(tbneutA/bneutA);
            if(N2dk1==2)newWeight*=(tbneutB/bneutB);
            if(N2dk1==0||N2dk1==3)newWeight*=(tbneutC/bneutC);
        }
    }
    if(slep2 == 12 || slep2 == 14){
        if(dk2 == 1)newWeight*=(tbsneuA/bsneuA);
        if(dk2 == 3)newWeight*=(tbsneuC/bsneuC);
        if(dk2 == 2){
            newWeight*=(tbsneuB/bsneuB);
            if(N2dk2==1)newWeight*=(tbneutA/bneutA);
            if(N2dk2==2)newWeight*=(tbneutB/bneutB);
            if(N2dk2==0||N2dk2==3)newWeight*=(tbneutC/bneutC);
        }
    }

    if ( debug ){
        std::cout << " " << std::endl;
        std::cout << "Event " << evn << std::endl;
        std::cout << "prod " << prod << " tree " << tree << std::endl;
        std::cout << "slep1, slep2 pseudoPDGIDs " << slep1 << " " << slep2 << std::endl;
        std::cout << "Slepton decays: " << dk1 << " " << dk2 << std::endl;
        std::cout << "N2 decays:      " << N2dk1 << " " << N2dk2 << std::endl; 
        std::cout << "apply flag: "<< apply << " New relative weight " << newWeight << std::endl;
    }
    if(!apply)newWeight = 1.0;
    return newWeight;

}

std::tuple<double, double, int, double> TriLeptonInfo(double m12, double m13, double m23, bool os12, bool os13, bool os23){

// Calculate a number of quantities related to trilepton topologies and return in a tuple.

// 1. The min dilepton mass independent of flavor/charge
     double minmll = std::min( { m12, m13, m23 });
// 2. The max dilepton mass independent of flavor/charge
     double maxmll = std::max( { m12, m13, m23 });
     const double MZ = 91.2;
// 3. The number of opposite-sign same flavor (OSSF) dileptons
     int nossf = 0;
// 4. For OSSF dileptons the smallest SIGNED deviation with respect to the nominal Z mass. If no 
     double devZ = -95.0;   // default value
     if( os12 ){
         nossf++;
         double thisdevZ = m12 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( os13 ){
         nossf++;
         double thisdevZ = m13 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( os23 ){
         nossf++;
         double thisdevZ = m23 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
 
     return std::make_tuple(minmll, maxmll, nossf, devZ);
 
}

std::tuple<double, double, int, double> QuadLeptonInfo(double m12, double m13, double m14, double m23, double m24, double m34, 
                                                       bool os12, bool os13, bool os14, bool os23, bool os24, bool os34){

// Calculate a number of quantities related to trilepton topologies and return in a tuple.

// 1. The min dilepton mass independent of flavor/charge
     double minmll = std::min( { m12, m13, m14, m23, m24, m34 });
// 2. The max dilepton mass independent of flavor/charge
     double maxmll = std::max( { m12, m13, m14, m23, m24, m34 });
     const double MZ = 91.2;
// 3. The number of opposite-sign same flavor (OSSF) dileptons
     int nossf = 0;
// 4. For OSSF dileptons the smallest SIGNED deviation with respect to the nominal Z mass. If no 
     double devZ = -95.0;   // default value
     if( os12 ){
         nossf++;
         double thisdevZ = m12 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( os13 ){
         nossf++;
         double thisdevZ = m13 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( os14 ){
         nossf++;
         double thisdevZ = m14 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( os23 ){
         nossf++;
         double thisdevZ = m23 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( os24 ){
         nossf++;
         double thisdevZ = m24 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( os34 ){
         nossf++;
         double thisdevZ = m34 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
 
     return std::make_tuple(minmll, maxmll, nossf, devZ);
 
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

void FillDiCutFlow(unsigned int sel, double wt) {
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

    const double MLLCUT = 24.0;        //  58/24/16
    const double TARGETLUMI = 400.0;

    double wt = TARGETLUMI*(*weight);
    if(inputFilePrefix=="SlepSneu-180"){
        double correctedweight = 3.639e-4;   // Zach's E-mail of 7/28/2025
        wt = TARGETLUMI*correctedweight;
    }

#if ANA_NTUPLE_VERSION == 3
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

    bool reWeight= true; 
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
    FourVec fMET = FourVec::FromPtEtaPhiM(100, *MET, 0.0, *MET_phi, 0.0);

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

// FIXME - keep track of b-tag MVA score 
    bool surviveBtagVeto = true;     
    for (unsigned int i = 0; i < BtagID_jet.GetSize(); ++i){
        if(BtagID_jet[i] >=2) surviveBtagVeto = false;
    }

// Trigger info
    bool passTrig = false;
    if (*SingleElectrontrigger || *SingleMuontrigger || *DoubleElectrontrigger || *DoubleMuontrigger || *EMutrigger)passTrig = true;

// Start on 3-lepton selection using enum class TriCuts
    unsigned int trisel = 0;
    std::vector<double> ptcuts3l = {20.0, 12.5, 7.5};
    double ptcutVeto3l = 3.0;
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

    if ( ngs < 3 ){
        trisel = setFailureBit(trisel, TriCuts::GSNumber);
    }
    else{   // We have at least 3 G + S leptons
        if ( PT_lep[vlidx[0]] < ptcuts3l[0] ) trisel = setFailureBit(trisel, TriCuts::PtOne);
        if ( PT_lep[vlidx[1]] < ptcuts3l[1] ) trisel = setFailureBit(trisel, TriCuts::PtTwo);
        if ( PT_lep[vlidx[2]] < ptcuts3l[2] ) trisel = setFailureBit(trisel, TriCuts::PtThree);
// Implement 4th lepton veto.  Loop over all leptons. If lepton is not already one considered as one of the 3 high quality leptons, 
// then set veto flag if any are above the 4th lepton veto pT cut. This also includes bronze leptons. 
        for (unsigned int i = 0; i < LepQual_lep.GetSize(); ++i){             // All leptons
             if( i != vlidx[0] && i != vlidx[1] && i != vlidx[2] ){           // Not lepton already considered
                 ptFourthLepton = PT_lep[i];
                 if ( PT_lep[i] > ptcutVeto3l ) {
                     failFourthLeptonVeto = true;
                 }
                 break;                       // break out so we store the highest pT additional lepton independent of whether it asserts the veto
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
        double m12 = l12.M();   double m13 = l13.M();  double m23 = l23.M();
        bool os12 = l1.osdil(l2);   bool os13 = l1.osdil(l3);  bool os23 = l2.osdil(l3);
        auto result = TriLeptonInfo(m12, m13, m23, os12, os13, os23);
        minmll3l = std::get<0>(result);
        maxmll3l = std::get<1>(result);
        nossf3l  = std::get<2>(result);
        devZ3l   = std::get<3>(result);
        if(minmll3l < 4.0) trisel = setFailureBit(trisel, TriCuts::MinMll);
        if(minmll3l > MLLCUT) trisel = setFailureBit(trisel, TriCuts::MxMinMll);  // This is signal hypothesis sensitive.  35 GeV is OK for LSP=260 and 270, but NOT 220.
        if(std::abs(devZ3l) < 7.5) trisel = setFailureBit(trisel, TriCuts::OffZ);
    }
    FillCutFlow(trisel, wt);

// 4l selection
// Start on 4-lepton selection using enum class QuadCuts
    unsigned int quadsel = 0;
    std::vector<double> ptcuts4l = {20.0, 12.5, 7.5, 5.0};
    bool failFifthLeptonVeto = false;
    double ptFifthLepton = -1.5;  // Default to invalid value
    double ptcutVeto4l = 3.0;
    double maxSIP3D4l{};
    double SIP3DChisq4l{};
    double minmll4l = 0.0;
    double maxmll4l = 0.0;
    int nossf4l = 0;
    double devZ4l = 0.0;

    if ( ngs < 4 ){
        quadsel = setFailureBit(quadsel, QuadCuts::GSNumber);
    }
    else{   // We have at least 3 G + S leptons
        if ( PT_lep[vlidx[0]] < ptcuts4l[0] ) quadsel = setFailureBit(quadsel, QuadCuts::PtOne);
        if ( PT_lep[vlidx[1]] < ptcuts4l[1] ) quadsel = setFailureBit(quadsel, QuadCuts::PtTwo);
        if ( PT_lep[vlidx[2]] < ptcuts4l[2] ) quadsel = setFailureBit(quadsel, QuadCuts::PtThree);
        if ( PT_lep[vlidx[3]] < ptcuts4l[3] ) quadsel = setFailureBit(quadsel, QuadCuts::PtFour);
// Implement 4th lepton veto.  Loop over all leptons. If lepton is not already one considered as one of the 3 high quality leptons, 
// then set veto flag if any are above the 4th lepton veto pT cut. This also includes bronze leptons. 
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
        double m12 = l12.M();   double m13 = l13.M();  double m14 = l14.M(); double m23 = l23.M(); double m24 = l24.M(); double m34 = l34.M();
        bool os12 = l1.osdil(l2);   bool os13 = l1.osdil(l3);  bool os23 = l2.osdil(l3);
        bool os14 = l1.osdil(l4);   bool os24 = l2.osdil(l4);  bool os34 = l3.osdil(l4);
        auto result = QuadLeptonInfo(m12, m13, m14, m23, m24, m24, os12, os13, os14, os23, os24, os34);
        minmll4l = std::get<0>(result);
        maxmll4l = std::get<1>(result);
        nossf4l = std::get<2>(result);
        devZ4l = std::get<3>(result);
        if(minmll4l < 4.0) quadsel = setFailureBit(quadsel, QuadCuts::MinMll);
        if(minmll4l > MLLCUT) quadsel = setFailureBit(quadsel, QuadCuts::MxMinMll);        // Needs to be checked/adjusted for each signal hypothesis
        if(std::abs(devZ4l) < 7.5) quadsel = setFailureBit(quadsel, QuadCuts::OffZ);
    }
    FillQuadCutFlow(quadsel, wt);

// Similar logic for 2-lepton selection using enum class DiCuts
    unsigned int disel = 0;
    std::vector<double> ptcuts2l = {25.0, 20.0};
    double ptcutVeto2l = 5.0;
    double sip3dcut2l = 3.0;
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
    FillDiCutFlow(disel, wt);

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

    if( isSelected( disel) ){

         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec dilepton = l1 + l2;
         hmll->Fill(dilepton.M(), wt);
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
         
    }

    if( isSelected( trisel ) ){

         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec l3 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[2]], PT_lep[vlidx[2]], Eta_lep[vlidx[2]], Phi_lep[vlidx[2]], M_lep[vlidx[2]]);

         FourVec l12 = l1 + l2;
         FourVec l13 = l1 + l3;
         FourVec l23 = l2 + l3;
         FourVec l123 = l12 + l3;
         int charge3 = l1.lcharge() + l2.lcharge() + l3.lcharge();
         int flavor3 = l1.lflavor() + l2.lflavor() + l3.lflavor();
         h3lflavor->Fill(flavor3, wt);
         int TightCharge3 = TightCharge_lep[vlidx[0]]*TightCharge_lep[vlidx[1]]*TightCharge_lep[vlidx[2]];
         hmll3->Fill(l12.M(), wt);
         hmll3->Fill(l13.M(), wt);
         hmll3->Fill(l23.M(), wt);
         hm3l->Fill(l123.M(), wt);
         hpt3l->Fill(l123.Pt(), wt);
         hminmll3l->Fill( minmll3l , wt );
         hmaxmll3l->Fill( maxmll3l , wt );
         h3lnossf->Fill(nossf3l, wt);
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
         h3lHTid->Fill(*HT_eta24_id, wt);
         h3lHTnoid->Fill(*HT_eta24, wt);
         h3lmaxmT->Fill(std::max( {l1.mtp(fMET), l2.mtp(fMET), l3.mtp(fMET) } )  , wt);
         h3lnjets->Fill(*Njet, wt);
         hminSIP3D->Fill(minSIP3D3l, wt);
         h3lSIP3DRChisq->Fill(SIP3DChisq3l/3.0, wt);   // divide by degrees of freedom for this plot
         int ndof = 3;
         double pfit = TMath::Prob(SIP3DChisq3l, ndof);
         double cdf = TMath::Gamma(ndof/2.0, SIP3DChisq3l/2.0);  // CDF of chi-squared
         double signed_z = TMath::NormQuantile(cdf);             // z such that P(Z < z) = CDF
// Now fill the other related histogram representations
         h3lSIP3DProb->Fill(pfit, wt);
         h3lSIP3DLogProb->Fill(std::log10(pfit), wt);
         h3lSIP3DSignificance->Fill(signed_z, wt);

#if ANA_NTUPLE_VERSION == 3
         hTriProcess->Fill(*cascades_prod, wt);
#endif 

    }

    if( isSelected(quadsel) ){

         h4lnjets->Fill(*Njet, wt);

         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec l3 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[2]], PT_lep[vlidx[2]], Eta_lep[vlidx[2]], Phi_lep[vlidx[2]], M_lep[vlidx[2]]);
         FourVec l4 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[3]], PT_lep[vlidx[3]], Eta_lep[vlidx[3]], Phi_lep[vlidx[3]], M_lep[vlidx[3]]);

         FourVec l12 = l1 + l2;
         FourVec l34 = l3 + l4;
         FourVec l1234 = l12 + l34;
         int charge4 = l1.lcharge() + l2.lcharge() + l3.lcharge() + l4.lcharge();
         int TightCharge4 = TightCharge_lep[vlidx[0]]*TightCharge_lep[vlidx[1]]*TightCharge_lep[vlidx[2]]*TightCharge_lep[vlidx[3]];

         int flavor4 = l1.lflavor() + l2.lflavor() + l3.lflavor() + l4.lflavor();
         h4lflavor->Fill(flavor4, wt);

         hm4l->Fill(l1234.Mass(), wt);
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
         double signed_z = TMath::NormQuantile(cdf);             // z such that P(Z < z) = CDF
// Now fill the other related histogram representations
         h4lSIP3DProb->Fill(pfit, wt);
         h4lSIP3DLogProb->Fill(std::log10(pfit), wt);
         h4lSIP3DSignificance->Fill(signed_z, wt);

         if(charge4==-2 || charge4==2){
             hQuadQ2->Fill(flavor4, wt);
             if(TightCharge4==16)hQuadQ2Tight->Fill(flavor4,wt);
         }

#if ANA_NTUPLE_VERSION == 3
         hQuadProcess->Fill(*cascades_prod, wt);
#endif 
    }

    return true;

}

void Ana::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

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
    hCutFlow->GetXaxis()->SetBinLabel(13, "Selected");
    std::cout << "Trilepton selected event count " << hCutFlow->GetBinContent(13) << " " << hCutFlow->GetBinError(13) << std::endl;

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
    hQuadCutFlow->GetXaxis()->SetBinLabel(14, "Selected");
    std::cout << "Quadlepton selected event count " << hQuadCutFlow->GetBinContent(14) << " " << hQuadCutFlow->GetBinError(14) << std::endl;

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

    hprocesslowt->GetXaxis()->SetBinLabel(1,"ee");
    hprocesslowt->GetXaxis()->SetBinLabel(2,"ev+");
    hprocesslowt->GetXaxis()->SetBinLabel(3,"ev-");
    hprocesslowt->GetXaxis()->SetBinLabel(4,"veve");
    hprocesslowt->GetXaxis()->SetBinLabel(5,"#mu#mu");
    hprocesslowt->GetXaxis()->SetBinLabel(6,"#muv+");
    hprocesslowt->GetXaxis()->SetBinLabel(7,"#muv-");
    hprocesslowt->GetXaxis()->SetBinLabel(8,"v#muv#mu");
    hprocesslowt->GetXaxis()->SetLabelSize(0.07);

    hprocesshiwt->GetXaxis()->SetBinLabel(1,"ee");
    hprocesshiwt->GetXaxis()->SetBinLabel(2,"ev+");
    hprocesshiwt->GetXaxis()->SetBinLabel(3,"ev-");
    hprocesshiwt->GetXaxis()->SetBinLabel(4,"veve");
    hprocesshiwt->GetXaxis()->SetBinLabel(5,"#mu#mu");
    hprocesshiwt->GetXaxis()->SetBinLabel(6,"#muv+");
    hprocesshiwt->GetXaxis()->SetBinLabel(7,"#muv-");
    hprocesshiwt->GetXaxis()->SetBinLabel(8,"v#muv#mu");
    hprocesshiwt->GetXaxis()->SetLabelSize(0.07);

    hTriProcess->GetXaxis()->SetBinLabel(1,"ee");
    hTriProcess->GetXaxis()->SetBinLabel(2,"ev+");
    hTriProcess->GetXaxis()->SetBinLabel(3,"ev-");
    hTriProcess->GetXaxis()->SetBinLabel(4,"veve");
    hTriProcess->GetXaxis()->SetBinLabel(5,"#mu#mu");
    hTriProcess->GetXaxis()->SetBinLabel(6,"#muv+");
    hTriProcess->GetXaxis()->SetBinLabel(7,"#muv-");
    hTriProcess->GetXaxis()->SetBinLabel(8,"v#muv#mu");
    hTriProcess->GetXaxis()->SetLabelSize(0.07);

    hQuadProcess->GetXaxis()->SetBinLabel(1,"ee");
    hQuadProcess->GetXaxis()->SetBinLabel(2,"ev+");
    hQuadProcess->GetXaxis()->SetBinLabel(3,"ev-");
    hQuadProcess->GetXaxis()->SetBinLabel(4,"veve");
    hQuadProcess->GetXaxis()->SetBinLabel(5,"#mu#mu");
    hQuadProcess->GetXaxis()->SetBinLabel(6,"#muv+");
    hQuadProcess->GetXaxis()->SetBinLabel(7,"#muv-");
    hQuadProcess->GetXaxis()->SetBinLabel(8,"v#muv#mu");
    hQuadProcess->GetXaxis()->SetLabelSize(0.07);

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
    hQuadTrigger->GetXaxis()->SetLabelSize(0.04);
    

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
