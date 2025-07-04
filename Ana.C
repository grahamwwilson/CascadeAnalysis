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
#include "FourVec.h"
#include "TriCuts.h"
#include <algorithm>
#include <bitset> 

// Declare histograms globally here
#include "AnaHistos.h"
// Declare helper functions with bits for selection
#include "SelectionBits.h"

void FillCutFlow(unsigned int trisel, TH1* hCutFlow, double wt) {
// Note this histogram is defined in Ana.h

    // Fill the preselection bin
    hCutFlow->Fill(-1.0, wt);

    // Loop over all cuts
    for (unsigned int i = 0; i < static_cast<unsigned int>(TriCuts::NUMCUTS); ++i) {
        TriCuts cut = static_cast<TriCuts>(i);
        if (PassesAllCutsSoFar(trisel, cut)) {
            hCutFlow->Fill(i, wt);
        }
    }

    // Optional: final bin if all cuts passed
    if (trisel == 0) {
        hCutFlow->Fill(static_cast<unsigned int>(TriCuts::NUMCUTS), wt);
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

    double wt = 400.0*(*weight);

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

// Start on 3-lepton selection using enum class TriCuts
    unsigned int trisel = 0;
    std::vector<double> ptcuts = {20.0, 15.0, 10.0};
    double ptcutVeto = 5.0;
    double sip3dcut = 3.5;
    double maxSIP3D{};

    bool failFourthLeptonVeto = false;
    double ptFourthLepton = -1.5;  // Default to invalid value

    if ( ngs < 3 ){
        trisel = setFailureBit(trisel, TriCuts::GSNumber);
    }
    else{   // We have at least 3 G + S leptons
        if ( PT_lep[vlidx[0]] < ptcuts[0] ) trisel = setFailureBit(trisel, TriCuts::PtOne);
        if ( PT_lep[vlidx[1]] < ptcuts[1] ) trisel = setFailureBit(trisel, TriCuts::PtTwo);
        if ( PT_lep[vlidx[2]] < ptcuts[2] ) trisel = setFailureBit(trisel, TriCuts::PtThree);
// Implement 4th lepton veto.  Loop over all leptons. If lepton is not already one considered as one of the 3 high quality leptons, 
// then set veto flag if any are above the 4th lepton veto pT cut. This also includes bronze leptons. 
        for (unsigned int i = 0; i < LepQual_lep.GetSize(); ++i){             // All leptons
             if( i != vlidx[0] && i != vlidx[1] && i != vlidx[2] ){           // Not lepton already considered
                 ptFourthLepton = PT_lep[i];
                 if ( PT_lep[i] > ptcutVeto ) {
                     failFourthLeptonVeto = true;
                     break;
                 }
             }
        }
        if ( failFourthLeptonVeto ) trisel = setFailureBit(trisel, TriCuts::PtFourVeto);
        if ( !surviveBtagVeto ) trisel = setFailureBit(trisel, TriCuts::BTagVeto);
        maxSIP3D = std::max({SIP3D_lep[vlidx[0]], SIP3D_lep[vlidx[1]], SIP3D_lep[vlidx[2]]});
        if ( maxSIP3D > sip3dcut ) trisel = setFailureBit(trisel, TriCuts::SIP3DCut);
    }

//    std::cout << "trisel = " << trisel << std::endl;
    htrisel->Fill(trisel, wt);

/*    hCutFlow->Fill(-1.0, wt);
    if(isPassingCut( trisel, TriCuts::GSNumber)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::GSNumber), wt);  // FIXME - make this a function
    if(isPassingCut( trisel, TriCuts::PtOne)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::PtOne), wt);
    if(isPassingCut( trisel, TriCuts::PtTwo)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::PtTwo), wt);
    if(isPassingCut( trisel, TriCuts::PtThree)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::PtThree), wt);
    if(isPassingCut( trisel, TriCuts::PtFourVeto)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::PtFourVeto), wt);
    if(isPassingCut( trisel, TriCuts::BTagVeto)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::BTagVeto), wt);
    if(isPassingCut( trisel, TriCuts::SIP3DCut)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::SIP3DCut), wt);
    if(trisel==0)hCutFlow->Fill(7.0, wt);
*/

/*
    hCutFlow->Fill(-1.0, wt);
    if(PassesAllCutsSoFar( trisel, TriCuts::GSNumber)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::GSNumber), wt);  // FIXME - make this a function
    if(PassesAllCutsSoFar( trisel, TriCuts::PtOne)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::PtOne), wt);
    if(PassesAllCutsSoFar( trisel, TriCuts::PtTwo)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::PtTwo), wt);
    if(PassesAllCutsSoFar( trisel, TriCuts::PtThree)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::PtThree), wt);
    if(PassesAllCutsSoFar( trisel, TriCuts::PtFourVeto)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::PtFourVeto), wt);
    if(PassesAllCutsSoFar( trisel, TriCuts::BTagVeto)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::BTagVeto), wt);
    if(PassesAllCutsSoFar( trisel, TriCuts::SIP3DCut)) hCutFlow->Fill( static_cast<unsigned int>(TriCuts::SIP3DCut), wt); 
    if(trisel==0)hCutFlow->Fill(7.0, wt);  
*/
    FillCutFlow(trisel, hCutFlow, wt);

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::SIP3DCut ) ) {
        hmaxSIP3D->Fill(maxSIP3D , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::PtOne )) {
        hPtOne->Fill(PT_lep[vlidx[0]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::PtTwo )) {
        hPtTwo->Fill(PT_lep[vlidx[1]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::PtThree )) {
        hPtThree->Fill(PT_lep[vlidx[2]] , wt);
    }

    if( isSelectedOrFailsJustOneCut( trisel, TriCuts::PtFourVeto )) {
        hPtFourVeto->Fill(ptFourthLepton , wt);
    }

    if(nleptons_hipt==2){

         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec dilepton = l1 + l2;
//         double mll = l1.mtwo(l2);
         hmll->Fill(dilepton.Mass(), wt);
         habetaz->Fill(std::abs(dilepton.Betaz()), wt);

// Calculate MT2 using the Lester-Nachman implementation for various assumptions on LSP mass
         double MT2LN0   = l1.MT2(l2, fMET,   0.0,   0.0,  0.0);
         hMT2LN0->Fill ( MT2LN0, wt);
         hMT2LN0SMALL->Fill ( MT2LN0, wt);
         
    }

//    if(trisel ==0){
//
//    }



    if( isSelected( trisel ) ){

         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec l3 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[2]], PT_lep[vlidx[2]], Eta_lep[vlidx[2]], Phi_lep[vlidx[2]], M_lep[vlidx[2]]);

         FourVec l12 = l1 + l2;
         FourVec l13 = l1 + l3;
         FourVec l23 = l2 + l3;
         FourVec l123 = l12 + l3;
         int charge3 = l1.lcharge() + l2.lcharge() + l3.lcharge();
         int TightCharge3 = TightCharge_lep[vlidx[0]]*TightCharge_lep[vlidx[1]]*TightCharge_lep[vlidx[2]];
         hmll3->Fill(l12.Mass(), wt);
         hmll3->Fill(l13.Mass(), wt);
         hmll3->Fill(l23.Mass(), wt);
         hm3l->Fill(l123.Mass(), wt);
         if(surviveBtagVeto){
             hm3lbtv->Fill(l123.Mass(), wt);
             h3lcharge->Fill(charge3, wt);
             if(TightCharge3==8)h3lchargetight->Fill(charge3, wt);
         }

         hmT1->Fill(l1.mtp(fMET), wt);
         hmT2->Fill(l2.mtp(fMET), wt);
         hmT3->Fill(l3.mtp(fMET), wt);
// Also fill inclusively
         hmT->Fill(l1.mtp(fMET), wt);
         hmT->Fill(l2.mtp(fMET), wt);
         hmT->Fill(l3.mtp(fMET), wt);

         hmT3l->Fill(l123.mtp(fMET), wt);

    }

    if(nleptons_hipt==4){
         FourVec l1 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[0]], PT_lep[vlidx[0]], Eta_lep[vlidx[0]], Phi_lep[vlidx[0]], M_lep[vlidx[0]]);
         FourVec l2 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[1]], PT_lep[vlidx[1]], Eta_lep[vlidx[1]], Phi_lep[vlidx[1]], M_lep[vlidx[1]]);
         FourVec l3 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[2]], PT_lep[vlidx[2]], Eta_lep[vlidx[2]], Phi_lep[vlidx[2]], M_lep[vlidx[2]]);
         FourVec l4 = FourVec::FromPtEtaPhiM(PDGID_lep[vlidx[3]], PT_lep[vlidx[3]], Eta_lep[vlidx[3]], Phi_lep[vlidx[3]], M_lep[vlidx[3]]);

         FourVec l12 = l1 + l2;
         FourVec l34 = l3 + l4;
         FourVec l1234 = l12 + l34;
         int charge4 = l1.lcharge() + l2.lcharge() + l3.lcharge() + l4.lcharge();
         int TightCharge4 = TightCharge_lep[vlidx[0]]*TightCharge_lep[vlidx[1]]*TightCharge_lep[vlidx[2]]*TightCharge_lep[vlidx[3]];

         hm4l->Fill(l1234.Mass(), wt);
         if(surviveBtagVeto){
             hm4lbtv->Fill(l1234.Mass(), wt);
             h4lcharge->Fill(charge4, wt);
             if(TightCharge4==16)h4lchargetight->Fill(charge4, wt);
         }
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
