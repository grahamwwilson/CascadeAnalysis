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

// Declare histograms globally here
#include "AnaHistos.h"

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

    std::vector<unsigned int> vlidx;

    int nleptons = 0; int nleptons_hipt = 0;
    for (unsigned int i = 0; i < LepQual_lep.GetSize(); ++i){
        if(LepQual_lep[i] <=1){
            nleptons ++;
            if(PT_lep[i] > 5.0) {
                nleptons_hipt++;
                vlidx.push_back(i);
            }
        }
    }
    hnleptons->Fill(nleptons, wt);
    hnleptons_hipt->Fill(nleptons_hipt, wt);

    bool surviveBtagVeto = true;     
    for (unsigned int i = 0; i < BtagID_jet.GetSize(); ++i){
        if(BtagID_jet[i] >=2) surviveBtagVeto = false;
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

    if(nleptons_hipt==3){

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
