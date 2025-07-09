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
#include "DiCuts.h"       // Dilepton selection
#include "TriCuts.h"      // Trilepton selection
#include <algorithm>
#include <bitset> 

// Declare histograms globally here
#include "AnaHistos.h"
// Declare helper functions with bits for selection
#include "SelectionBits.h"

void FillCutFlow(unsigned int trisel, double wt) {

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

void FillDiCutFlow(unsigned int disel, double wt) {
// Note this histogram used to be defined in Ana.h



    // Fill the preselection bin
    hDiCutFlow->Fill(-1.0, wt);

    // Loop over all cuts
    for (unsigned int i = 0; i < static_cast<unsigned int>(DiCuts::NUMCUTS); ++i) {
        DiCuts cut = static_cast<DiCuts>(i);
        if (PassesAllCutsSoFar(disel, cut)) {
            hDiCutFlow->Fill(i, wt);
        }
    }

    // Optional: final bin if all cuts passed
    if (disel == 0) {
        hDiCutFlow->Fill(static_cast<unsigned int>(DiCuts::NUMCUTS), wt);
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
                 }
                 break;                       // break out so we store the highest pT additional lepton independent of whether it asserts the veto
             }
        }
        if ( failFourthLeptonVeto ) trisel = setFailureBit(trisel, TriCuts::PtFourVeto);
        if ( !surviveBtagVeto ) trisel = setFailureBit(trisel, TriCuts::BTagVeto);
        maxSIP3D = std::max({SIP3D_lep[vlidx[0]], SIP3D_lep[vlidx[1]], SIP3D_lep[vlidx[2]]});
        if ( maxSIP3D > sip3dcut ) trisel = setFailureBit(trisel, TriCuts::SIP3DCut);
    }
    htrisel->Fill(trisel, wt);
    FillCutFlow(trisel, wt);

// Similar logic for 2-lepton selection using enum class DiCuts
    unsigned int disel = 0;
    std::vector<double> qtcuts = {25.0, 20.0};
    ptcutVeto = 5.0;
    sip3dcut = 3.0;

    bool failThirdLeptonVeto = false;
    double ptThirdLepton = -1.5;  // Default to invalid value

    if ( ngs < 2 ){
        disel = setFailureBit(disel, DiCuts::GSNumber);
    }
    else{   // We have at least 3 G + S leptons
        if ( PT_lep[vlidx[0]] < qtcuts[0] ) disel = setFailureBit(disel, DiCuts::PtOne);
        if ( PT_lep[vlidx[1]] < qtcuts[1] ) disel = setFailureBit(disel, DiCuts::PtTwo);
// Implement 3rd lepton veto.  Loop over all leptons. If lepton is not already one considered as one of the 2 high quality leptons, 
// then set veto flag if any are above the 3rd lepton veto pT cut. This also includes bronze leptons. 
        for (unsigned int i = 0; i < LepQual_lep.GetSize(); ++i){             // All leptons
             if( i != vlidx[0] && i != vlidx[1] ){                            // Not lepton already considered
                 ptThirdLepton = PT_lep[i];
                 if ( PT_lep[i] > ptcutVeto ) {
                     failThirdLeptonVeto = true;
                 }
                 break;                       // break out so we store the highest pT additional lepton independent of whether it asserts the veto
             }
        }
        if ( failThirdLeptonVeto ) disel = setFailureBit(disel, DiCuts::PtThreeVeto);
        if ( !surviveBtagVeto ) disel = setFailureBit(disel, DiCuts::BTagVeto);
        maxSIP3D = std::max({SIP3D_lep[vlidx[0]], SIP3D_lep[vlidx[1]]});
        if ( maxSIP3D > sip3dcut ) disel = setFailureBit(disel, DiCuts::SIP3DCut);
    }

    FillDiCutFlow(disel, wt);

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
         hminmll3l->Fill( std::min( { l12.M(), l13.M(), l23.M() }) , wt );
         hmaxmll3l->Fill( std::max( { l12.M(), l13.M(), l23.M() }) , wt );

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

// These bin labels can be done at Terminate stage.

    hDiCutFlow->GetXaxis()->SetBinLabel(1, "All");
    hDiCutFlow->GetXaxis()->SetBinLabel(2, "GSNumber");
    hDiCutFlow->GetXaxis()->SetBinLabel(3, "PtOne");
    hDiCutFlow->GetXaxis()->SetBinLabel(4, "PtTwo");
    hDiCutFlow->GetXaxis()->SetBinLabel(5, "PtThreeVeto");
    hDiCutFlow->GetXaxis()->SetBinLabel(6, "BTagVeto");
    hDiCutFlow->GetXaxis()->SetBinLabel(7, "SIP3DCut");
    hDiCutFlow->GetXaxis()->SetBinLabel(8, "Selected");
    std::cout << "Dilepton selected event count  " << hDiCutFlow->GetBinContent(8) << std::endl;

    hCutFlow->GetXaxis()->SetBinLabel(1, "All");
    hCutFlow->GetXaxis()->SetBinLabel(2, "GSNumber");
    hCutFlow->GetXaxis()->SetBinLabel(3, "PtOne");
    hCutFlow->GetXaxis()->SetBinLabel(4, "PtTwo");
    hCutFlow->GetXaxis()->SetBinLabel(5, "PtThree");
    hCutFlow->GetXaxis()->SetBinLabel(6, "PtFourVeto");
    hCutFlow->GetXaxis()->SetBinLabel(7, "BTagVeto");
    hCutFlow->GetXaxis()->SetBinLabel(8, "SIP3DCut");
    hCutFlow->GetXaxis()->SetBinLabel(9, "Selected");
    std::cout << "Trilepton selected event count " << hCutFlow->GetBinContent(9) << std::endl;

    h3lflavor->GetXaxis()->SetBinLabel(1,"eee");
    h3lflavor->GetXaxis()->SetBinLabel(2,"ee#mu");
    h3lflavor->GetXaxis()->SetBinLabel(3,"e#mu#mu");
    h3lflavor->GetXaxis()->SetBinLabel(4,"#mu#mu#mu");
    h3lflavor->GetXaxis()->SetLabelSize(0.07);
    

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
