//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jun 19 09:34:23 2025 by ROOT version 6.32.12
// from TTree KUAnalysis/KUAnalysis
// found on file: WZto3LNu-1Jets-4FS_TuneCP5_13p6TeV_amcatnloFXFX-pythia8_Summer23BPix_130X.root
//////////////////////////////////////////////////////////

#ifndef Ana_h
#define Ana_h

#include <TH1.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <string>
#include "asymm_mt2_lester_bisect.h"

// Headers needed by this particular selector
#include <vector>
#include "PreciseDouble.h"

class Ana : public TSelector {
public :

// Populate these strings in Init
   std::string     inputFilePrefix;
   std::string     treeName;

//   TH1D* hCutFlow = nullptr;  // member variable

   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Bool_t> event_skipped = {fReader, "event_skipped"};
   TTreeReaderValue<vector<bool>> treeSkipped = {fReader, "treeSkipped"};
   TTreeReaderValue<Double_t> weight = {fReader, "weight"};
   TTreeReaderValue<Double_t> genweight = {fReader, "genweight"};
   TTreeReaderValue<Double_t> PUweight = {fReader, "PUweight"};
   TTreeReaderValue<Double_t> PUweight_up = {fReader, "PUweight_up"};
   TTreeReaderValue<Double_t> PUweight_down = {fReader, "PUweight_down"};
   TTreeReaderValue<Double_t> MuFweight = {fReader, "MuFweight"};
   TTreeReaderValue<Double_t> MuFweight_up = {fReader, "MuFweight_up"};
   TTreeReaderValue<Double_t> MuFweight_down = {fReader, "MuFweight_down"};
   TTreeReaderValue<Double_t> MuRweight = {fReader, "MuRweight"};
   TTreeReaderValue<Double_t> MuRweight_up = {fReader, "MuRweight_up"};
   TTreeReaderValue<Double_t> MuRweight_down = {fReader, "MuRweight_down"};
   TTreeReaderValue<Double_t> PDFweight = {fReader, "PDFweight"};
   TTreeReaderValue<Double_t> PDFweight_up = {fReader, "PDFweight_up"};
   TTreeReaderValue<Double_t> PDFweight_down = {fReader, "PDFweight_down"};
   TTreeReaderValue<Double_t> BtagHFSFweight = {fReader, "BtagHFSFweight"};
   TTreeReaderValue<Double_t> BtagHFSFweight_up = {fReader, "BtagHFSFweight_up"};
   TTreeReaderValue<Double_t> BtagHFSFweight_down = {fReader, "BtagHFSFweight_down"};
   TTreeReaderValue<Double_t> BtagLFSFweight = {fReader, "BtagLFSFweight"};
   TTreeReaderValue<Double_t> BtagLFSFweight_up = {fReader, "BtagLFSFweight_up"};
   TTreeReaderValue<Double_t> BtagLFSFweight_down = {fReader, "BtagLFSFweight_down"};
   TTreeReaderValue<Double_t> elIDSFweight = {fReader, "elIDSFweight"};
   TTreeReaderValue<Double_t> elIDSFweight_up = {fReader, "elIDSFweight_up"};
   TTreeReaderValue<Double_t> elIDSFweight_down = {fReader, "elIDSFweight_down"};
   TTreeReaderValue<Double_t> elISOSFweight = {fReader, "elISOSFweight"};
   TTreeReaderValue<Double_t> elISOSFweight_up = {fReader, "elISOSFweight_up"};
   TTreeReaderValue<Double_t> elISOSFweight_down = {fReader, "elISOSFweight_down"};
   TTreeReaderValue<Double_t> elSIPSFweight = {fReader, "elSIPSFweight"};
   TTreeReaderValue<Double_t> elSIPSFweight_up = {fReader, "elSIPSFweight_up"};
   TTreeReaderValue<Double_t> elSIPSFweight_down = {fReader, "elSIPSFweight_down"};
   TTreeReaderValue<Double_t> elVLSFweight = {fReader, "elVLSFweight"};
   TTreeReaderValue<Double_t> elVLSFweight_up = {fReader, "elVLSFweight_up"};
   TTreeReaderValue<Double_t> elVLSFweight_down = {fReader, "elVLSFweight_down"};
   TTreeReaderValue<Double_t> muIDSFweight = {fReader, "muIDSFweight"};
   TTreeReaderValue<Double_t> muIDSFweight_up = {fReader, "muIDSFweight_up"};
   TTreeReaderValue<Double_t> muIDSFweight_down = {fReader, "muIDSFweight_down"};
   TTreeReaderValue<Double_t> muISOSFweight = {fReader, "muISOSFweight"};
   TTreeReaderValue<Double_t> muISOSFweight_up = {fReader, "muISOSFweight_up"};
   TTreeReaderValue<Double_t> muISOSFweight_down = {fReader, "muISOSFweight_down"};
   TTreeReaderValue<Double_t> muSIPSFweight = {fReader, "muSIPSFweight"};
   TTreeReaderValue<Double_t> muSIPSFweight_up = {fReader, "muSIPSFweight_up"};
   TTreeReaderValue<Double_t> muSIPSFweight_down = {fReader, "muSIPSFweight_down"};
   TTreeReaderValue<Double_t> muVLSFweight = {fReader, "muVLSFweight"};
   TTreeReaderValue<Double_t> muVLSFweight_up = {fReader, "muVLSFweight_up"};
   TTreeReaderValue<Double_t> muVLSFweight_down = {fReader, "muVLSFweight_down"};
   TTreeReaderValue<Double_t> MetTrigSFweight = {fReader, "MetTrigSFweight"};
   TTreeReaderValue<Double_t> MetTrigSFweight_up = {fReader, "MetTrigSFweight_up"};
   TTreeReaderValue<Double_t> MetTrigSFweight_down = {fReader, "MetTrigSFweight_down"};
   TTreeReaderValue<Int_t> MetTrigSFCurveIndex = {fReader, "MetTrigSFCurveIndex"};
   TTreeReaderValue<Int_t> runnum = {fReader, "runnum"};
   TTreeReaderValue<Int_t> luminum = {fReader, "luminum"};
   TTreeReaderValue<Long64_t> eventnum = {fReader, "eventnum"};
   TTreeReaderValue<Int_t> NPV = {fReader, "NPV"};
   TTreeReaderValue<Bool_t> EventFilter = {fReader, "EventFilter"};
   TTreeReaderValue<Bool_t> FastSimEventVeto = {fReader, "FastSimEventVeto"};
   TTreeReaderValue<Double_t> PrefireWeight = {fReader, "PrefireWeight"};
   TTreeReaderValue<Double_t> PrefireWeight_up = {fReader, "PrefireWeight_up"};
   TTreeReaderValue<Double_t> PrefireWeight_down = {fReader, "PrefireWeight_down"};
   TTreeReaderValue<Bool_t> METtrigger = {fReader, "METtrigger"};
   TTreeReaderValue<Bool_t> METORtrigger = {fReader, "METORtrigger"};
   TTreeReaderValue<Bool_t> DoubleElectrontrigger = {fReader, "DoubleElectrontrigger"};
   TTreeReaderValue<Bool_t> DoubleMuontrigger = {fReader, "DoubleMuontrigger"};
   TTreeReaderValue<Bool_t> EventFlag_FailJetID = {fReader, "EventFlag_FailJetID"};
   TTreeReaderValue<Bool_t> EventFlag_JetInHEM = {fReader, "EventFlag_JetInHEM"};
   TTreeReaderValue<Bool_t> EventFlag_JetInHEM_Pt20 = {fReader, "EventFlag_JetInHEM_Pt20"};
   TTreeReaderValue<Bool_t> EventFlag_JetInHEM_Pt20_JetID = {fReader, "EventFlag_JetInHEM_Pt20_JetID"};
   TTreeReaderValue<Bool_t> HEM_Veto = {fReader, "HEM_Veto"};
   TTreeReaderValue<Bool_t> SingleElectrontrigger = {fReader, "SingleElectrontrigger"};
   TTreeReaderValue<Bool_t> SingleMuontrigger = {fReader, "SingleMuontrigger"};
   TTreeReaderValue<Bool_t> EMutrigger = {fReader, "EMutrigger"};
   TTreeReaderValue<Double_t> MET = {fReader, "MET"};
   TTreeReaderValue<Double_t> MET_phi = {fReader, "MET_phi"};
   TTreeReaderValue<Double_t> altMET = {fReader, "altMET"};
   TTreeReaderValue<Double_t> altMET_phi = {fReader, "altMET_phi"};
   TTreeReaderValue<Double_t> LHE_HT = {fReader, "LHE_HT"};
   TTreeReaderValue<Double_t> LHE_HTIncoming = {fReader, "LHE_HTIncoming"};
   TTreeReaderValue<Double_t> HT_eta24 = {fReader, "HT_eta24"};
   TTreeReaderValue<Double_t> HT_eta24_id = {fReader, "HT_eta24_id"};
   TTreeReaderValue<Double_t> HT_eta3 = {fReader, "HT_eta3"};
   TTreeReaderValue<Double_t> HT_eta3_id = {fReader, "HT_eta3_id"};
   TTreeReaderValue<Double_t> HT_eta5 = {fReader, "HT_eta5"};
   TTreeReaderValue<Double_t> HT_eta5_id = {fReader, "HT_eta5_id"};
   TTreeReaderValue<Int_t> Nele = {fReader, "Nele"};
   TTreeReaderValue<Int_t> Nlele = {fReader, "Nlele"};
   TTreeReaderValue<Int_t> Nmu = {fReader, "Nmu"};
   TTreeReaderValue<Int_t> Nlep = {fReader, "Nlep"};
   TTreeReaderArray<double> PT_lep = {fReader, "PT_lep"};
   TTreeReaderArray<double> Eta_lep = {fReader, "Eta_lep"};
   TTreeReaderArray<double> Phi_lep = {fReader, "Phi_lep"};
   TTreeReaderArray<double> M_lep = {fReader, "M_lep"};
   TTreeReaderArray<int> Charge_lep = {fReader, "Charge_lep"};
   TTreeReaderArray<int> PDGID_lep = {fReader, "PDGID_lep"};
   TTreeReaderArray<int> ID_lep = {fReader, "ID_lep"};
   TTreeReaderArray<int> SourceID_lep = {fReader, "SourceID_lep"};
   TTreeReaderArray<int> LepQual_lep = {fReader, "LepQual_lep"};
   TTreeReaderArray<int> IsLowPt_lep = {fReader, "IsLowPt_lep"};
   TTreeReaderArray<int> TightCharge_lep = {fReader, "TightCharge_lep"};
   TTreeReaderArray<int> Index_lep = {fReader, "Index_lep"};
   TTreeReaderArray<double> RelIso_lep = {fReader, "RelIso_lep"};
   TTreeReaderArray<double> MiniIso_lep = {fReader, "MiniIso_lep"};
   TTreeReaderArray<double> Dxy_lep = {fReader, "Dxy_lep"};
   TTreeReaderArray<double> DxyErr_lep = {fReader, "DxyErr_lep"};
   TTreeReaderArray<double> Dz_lep = {fReader, "Dz_lep"};
   TTreeReaderArray<double> DzErr_lep = {fReader, "DzErr_lep"};
   TTreeReaderArray<double> IP3D_lep = {fReader, "IP3D_lep"};
   TTreeReaderArray<double> SIP3D_lep = {fReader, "SIP3D_lep"};
   TTreeReaderValue<Int_t> Njet = {fReader, "Njet"};
   TTreeReaderValue<Int_t> Nbjet = {fReader, "Nbjet"};
   TTreeReaderArray<double> PT_jet = {fReader, "PT_jet"};
   TTreeReaderArray<double> Eta_jet = {fReader, "Eta_jet"};
   TTreeReaderArray<double> Phi_jet = {fReader, "Phi_jet"};
   TTreeReaderArray<double> M_jet = {fReader, "M_jet"};
   TTreeReaderArray<double> Btag_jet = {fReader, "Btag_jet"};
   TTreeReaderArray<int> BtagID_jet = {fReader, "BtagID_jet"};
   TTreeReaderArray<double> Flavor_jet = {fReader, "Flavor_jet"};
   TTreeReaderArray<int> index_jet_a = {fReader, "index_jet_a"};
   TTreeReaderArray<int> index_jet_b = {fReader, "index_jet_b"};
   TTreeReaderArray<int> index_jet_ISR = {fReader, "index_jet_ISR"};
   TTreeReaderArray<int> index_jet_S = {fReader, "index_jet_S"};
   TTreeReaderArray<double> PT_Genjet = {fReader, "PT_Genjet"};
   TTreeReaderArray<double> Eta_Genjet = {fReader, "Eta_Genjet"};
   TTreeReaderArray<double> Phi_Genjet = {fReader, "Phi_Genjet"};
   TTreeReaderArray<double> M_Genjet = {fReader, "M_Genjet"};
   TTreeReaderArray<int> Index_jet = {fReader, "Index_jet"};
   TTreeReaderValue<Int_t> Njet_ISR = {fReader, "Njet_ISR"};
   TTreeReaderValue<Int_t> Njet_S = {fReader, "Njet_S"};
   TTreeReaderValue<Int_t> Nbjet_ISR = {fReader, "Nbjet_ISR"};
   TTreeReaderValue<Int_t> Nbjet_S = {fReader, "Nbjet_S"};
   TTreeReaderValue<Int_t> Nlep_ISR = {fReader, "Nlep_ISR"};
   TTreeReaderValue<Int_t> Nlep_S = {fReader, "Nlep_S"};
   TTreeReaderArray<int> index_lep_ISR = {fReader, "index_lep_ISR"};
   TTreeReaderArray<int> index_lep_S = {fReader, "index_lep_S"};
   TTreeReaderArray<double> dphi_lep_S = {fReader, "dphi_lep_S"};
   TTreeReaderArray<double> cos_lep_S = {fReader, "cos_lep_S"};
   TTreeReaderArray<double> dphi_jet_S = {fReader, "dphi_jet_S"};
   TTreeReaderArray<double> cos_jet_S = {fReader, "cos_jet_S"};
   TTreeReaderArray<double> dphiMET_lep_S = {fReader, "dphiMET_lep_S"};
   TTreeReaderArray<double> dphiMET_jet_S = {fReader, "dphiMET_jet_S"};
   TTreeReaderValue<Int_t> Njet_a = {fReader, "Njet_a"};
   TTreeReaderValue<Int_t> Njet_b = {fReader, "Njet_b"};
   TTreeReaderValue<Int_t> Nbjet_a = {fReader, "Nbjet_a"};
   TTreeReaderValue<Int_t> Nbjet_b = {fReader, "Nbjet_b"};
   TTreeReaderValue<Int_t> Nlep_a = {fReader, "Nlep_a"};
   TTreeReaderValue<Int_t> Nlep_b = {fReader, "Nlep_b"};
   TTreeReaderArray<int> index_lep_a = {fReader, "index_lep_a"};
   TTreeReaderArray<int> index_lep_b = {fReader, "index_lep_b"};
   TTreeReaderValue<Double_t> PTCM = {fReader, "PTCM"};
   TTreeReaderValue<Double_t> PzCM = {fReader, "PzCM"};
   TTreeReaderValue<Double_t> cosCM = {fReader, "cosCM"};
   TTreeReaderValue<Double_t> dphiCM = {fReader, "dphiCM"};
   TTreeReaderValue<Double_t> dphiCMI = {fReader, "dphiCMI"};
   TTreeReaderValue<Double_t> dphiMET_V = {fReader, "dphiMET_V"};
   TTreeReaderValue<Double_t> Mperp = {fReader, "Mperp"};
   TTreeReaderValue<Double_t> gammaT = {fReader, "gammaT"};
   TTreeReaderValue<Double_t> EJ_BoostT = {fReader, "EJ_BoostT"};
   TTreeReaderValue<Double_t> EL_BoostT = {fReader, "EL_BoostT"};
   TTreeReaderValue<Double_t> PTISR = {fReader, "PTISR"};
   TTreeReaderValue<Double_t> RISR = {fReader, "RISR"};
   TTreeReaderValue<Double_t> EtaCM = {fReader, "EtaCM"};
   TTreeReaderValue<Double_t> PhiCM = {fReader, "PhiCM"};
   TTreeReaderValue<Double_t> MCM = {fReader, "MCM"};
   TTreeReaderValue<Double_t> EtaS = {fReader, "EtaS"};
   TTreeReaderValue<Double_t> PhiS = {fReader, "PhiS"};
   TTreeReaderValue<Double_t> LAB_Pt = {fReader, "LAB_Pt"};
   TTreeReaderValue<Double_t> LAB_Eta = {fReader, "LAB_Eta"};
   TTreeReaderValue<Double_t> LAB_Phi = {fReader, "LAB_Phi"};
   TTreeReaderValue<Double_t> LAB_M = {fReader, "LAB_M"};
   TTreeReaderValue<Double_t> MSperpCM0 = {fReader, "MSperpCM0"};
   TTreeReaderValue<Double_t> MaPerpCM0 = {fReader, "MaPerpCM0"};
   TTreeReaderValue<Double_t> MbPerpCM0 = {fReader, "MbPerpCM0"};
   TTreeReaderValue<Double_t> MaVPerpCM0 = {fReader, "MaVPerpCM0"};
   TTreeReaderValue<Double_t> MbVPerpCM0 = {fReader, "MbVPerpCM0"};
   TTreeReaderValue<Double_t> MQperpCM0 = {fReader, "MQperpCM0"};
   TTreeReaderValue<Double_t> gammaPerpCM0 = {fReader, "gammaPerpCM0"};
   TTreeReaderValue<Double_t> MVisAperpCM0 = {fReader, "MVisAperpCM0"};
   TTreeReaderValue<Double_t> MVisBperpCM0 = {fReader, "MVisBperpCM0"};
   TTreeReaderValue<Double_t> MSCM0 = {fReader, "MSCM0"};
   TTreeReaderValue<Double_t> MaCM0 = {fReader, "MaCM0"};
   TTreeReaderValue<Double_t> MbCM0 = {fReader, "MbCM0"};
   TTreeReaderValue<Double_t> MaVCM0 = {fReader, "MaVCM0"};
   TTreeReaderValue<Double_t> MbVCM0 = {fReader, "MbVCM0"};
   TTreeReaderValue<Double_t> MQCM0 = {fReader, "MQCM0"};
   TTreeReaderValue<Double_t> gammaCM0 = {fReader, "gammaCM0"};
   TTreeReaderValue<Double_t> MVisACM0 = {fReader, "MVisACM0"};
   TTreeReaderValue<Double_t> MVisBCM0 = {fReader, "MVisBCM0"};
   TTreeReaderValue<Double_t> MS = {fReader, "MS"};
   TTreeReaderValue<Double_t> PS = {fReader, "PS"};
   TTreeReaderValue<Double_t> cosS = {fReader, "cosS"};
   TTreeReaderValue<Double_t> dphiS = {fReader, "dphiS"};
   TTreeReaderValue<Double_t> dphiSI = {fReader, "dphiSI"};
   TTreeReaderValue<Double_t> PTS = {fReader, "PTS"};
   TTreeReaderValue<Double_t> PzS = {fReader, "PzS"};
   TTreeReaderValue<Double_t> EVa = {fReader, "EVa"};
   TTreeReaderValue<Double_t> EVb = {fReader, "EVb"};
   TTreeReaderValue<Double_t> PVa = {fReader, "PVa"};
   TTreeReaderValue<Double_t> PVb = {fReader, "PVb"};
   TTreeReaderValue<Double_t> EJa = {fReader, "EJa"};
   TTreeReaderValue<Double_t> EJb = {fReader, "EJb"};
   TTreeReaderValue<Double_t> PJa = {fReader, "PJa"};
   TTreeReaderValue<Double_t> PJb = {fReader, "PJb"};
   TTreeReaderValue<Double_t> MX2a = {fReader, "MX2a"};
   TTreeReaderValue<Double_t> MX2b = {fReader, "MX2b"};
   TTreeReaderValue<Double_t> ELa = {fReader, "ELa"};
   TTreeReaderValue<Double_t> ELb = {fReader, "ELb"};
   TTreeReaderValue<Double_t> PLa = {fReader, "PLa"};
   TTreeReaderValue<Double_t> PLb = {fReader, "PLb"};
   TTreeReaderValue<Double_t> MV = {fReader, "MV"};
   TTreeReaderValue<Double_t> PV = {fReader, "PV"};
   TTreeReaderValue<Double_t> MVa = {fReader, "MVa"};
   TTreeReaderValue<Double_t> MVb = {fReader, "MVb"};
   TTreeReaderValue<Double_t> PV_lab = {fReader, "PV_lab"};
   TTreeReaderValue<Double_t> MJa = {fReader, "MJa"};
   TTreeReaderValue<Double_t> MJb = {fReader, "MJb"};
   TTreeReaderValue<Double_t> MLa = {fReader, "MLa"};
   TTreeReaderValue<Double_t> MLb = {fReader, "MLb"};
   TTreeReaderValue<Double_t> cosJa = {fReader, "cosJa"};
   TTreeReaderValue<Double_t> cosJb = {fReader, "cosJb"};
   TTreeReaderValue<Double_t> cosLa = {fReader, "cosLa"};
   TTreeReaderValue<Double_t> cosLb = {fReader, "cosLb"};
   TTreeReaderValue<Double_t> MJ = {fReader, "MJ"};
   TTreeReaderValue<Double_t> ML = {fReader, "ML"};
   TTreeReaderValue<Double_t> EJ = {fReader, "EJ"};
   TTreeReaderValue<Double_t> EL = {fReader, "EL"};
   TTreeReaderValue<Double_t> PJ = {fReader, "PJ"};
   TTreeReaderValue<Double_t> PL = {fReader, "PL"};
   TTreeReaderValue<Double_t> PX2 = {fReader, "PX2"};
   TTreeReaderValue<Double_t> PX2_BoostT = {fReader, "PX2_BoostT"};
   TTreeReaderValue<Double_t> MX2a_BoostT = {fReader, "MX2a_BoostT"};
   TTreeReaderValue<Double_t> MX2b_BoostT = {fReader, "MX2b_BoostT"};
   TTreeReaderValue<Double_t> PV_BoostT = {fReader, "PV_BoostT"};
   TTreeReaderValue<Double_t> EVa_BoostT = {fReader, "EVa_BoostT"};
   TTreeReaderValue<Double_t> EVb_BoostT = {fReader, "EVb_BoostT"};
   TTreeReaderValue<Double_t> PVa_BoostT = {fReader, "PVa_BoostT"};
   TTreeReaderValue<Double_t> PVb_BoostT = {fReader, "PVb_BoostT"};
   TTreeReaderValue<Double_t> PJ_BoostT = {fReader, "PJ_BoostT"};
   TTreeReaderValue<Double_t> PL_BoostT = {fReader, "PL_BoostT"};
   TTreeReaderValue<Double_t> PISR = {fReader, "PISR"};
   TTreeReaderValue<Double_t> RISRT = {fReader, "RISRT"};
   TTreeReaderValue<Double_t> MISR = {fReader, "MISR"};
   TTreeReaderValue<Int_t> NPU = {fReader, "NPU"};
   TTreeReaderValue<Double_t> genMET = {fReader, "genMET"};
   TTreeReaderValue<Double_t> genMET_phi = {fReader, "genMET_phi"};
   TTreeReaderValue<Int_t> genNele = {fReader, "genNele"};
   TTreeReaderValue<Int_t> genNmu = {fReader, "genNmu"};
   TTreeReaderValue<Int_t> genNlep = {fReader, "genNlep"};
   TTreeReaderArray<double> genPT_lep = {fReader, "genPT_lep"};
   TTreeReaderArray<double> genEta_lep = {fReader, "genEta_lep"};
   TTreeReaderArray<double> genPhi_lep = {fReader, "genPhi_lep"};
   TTreeReaderArray<double> genM_lep = {fReader, "genM_lep"};
   TTreeReaderArray<int> genCharge_lep = {fReader, "genCharge_lep"};
   TTreeReaderArray<int> genPDGID_lep = {fReader, "genPDGID_lep"};
   TTreeReaderArray<int> genMomPDGID_lep = {fReader, "genMomPDGID_lep"};
   TTreeReaderArray<int> genSourceID_lep = {fReader, "genSourceID_lep"};
   TTreeReaderArray<int> genIndex_lep = {fReader, "genIndex_lep"};
   TTreeReaderArray<int> genMomIndex_lep = {fReader, "genMomIndex_lep"};
   TTreeReaderValue<Int_t> genNnu = {fReader, "genNnu"};
   TTreeReaderArray<double> genPT_nu = {fReader, "genPT_nu"};
   TTreeReaderArray<double> genEta_nu = {fReader, "genEta_nu"};
   TTreeReaderArray<double> genPhi_nu = {fReader, "genPhi_nu"};
   TTreeReaderArray<int> genPDGID_nu = {fReader, "genPDGID_nu"};
   TTreeReaderArray<int> genMomPDGID_nu = {fReader, "genMomPDGID_nu"};
   TTreeReaderValue<Int_t> genNboson = {fReader, "genNboson"};
   TTreeReaderArray<double> genPT_boson = {fReader, "genPT_boson"};
   TTreeReaderArray<double> genEta_boson = {fReader, "genEta_boson"};
   TTreeReaderArray<double> genPhi_boson = {fReader, "genPhi_boson"};
   TTreeReaderArray<double> genM_boson = {fReader, "genM_boson"};
   TTreeReaderArray<int> genPDGID_boson = {fReader, "genPDGID_boson"};
   TTreeReaderArray<int> genMomPDGID_boson = {fReader, "genMomPDGID_boson"};
   TTreeReaderValue<Int_t> genNsusy = {fReader, "genNsusy"};
   TTreeReaderArray<double> genPT_susy = {fReader, "genPT_susy"};
   TTreeReaderArray<double> genEta_susy = {fReader, "genEta_susy"};
   TTreeReaderArray<double> genPhi_susy = {fReader, "genPhi_susy"};
   TTreeReaderArray<double> genM_susy = {fReader, "genM_susy"};
   TTreeReaderArray<int> genPDGID_susy = {fReader, "genPDGID_susy"};
   TTreeReaderArray<int> genMomPDGID_susy = {fReader, "genMomPDGID_susy"};


   Ana(TTree * /*tree*/ =0) { }
   ~Ana() override { }
   Int_t   Version() const override { return 2; }
   void    Begin(TTree *tree) override;
   void    SlaveBegin(TTree *tree) override;
   void    Init(TTree *tree) override;
   bool    Notify() override;
   bool    Process(Long64_t entry) override;
   Int_t   GetEntry(Long64_t entry, Int_t getall = 0) override { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   void    SetOption(const char *option) override { fOption = option; }
   void    SetObject(TObject *obj) override { fObject = obj; }
   void    SetInputList(TList *input) override { fInput = input; }
   TList  *GetOutputList() const override { return fOutput; }
   void    SlaveTerminate() override;
   void    Terminate() override;

   ClassDefOverride(Ana,0);

};

#endif

#ifdef Ana_cxx
void Ana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

// Make clear which pdouble we have under the hood.
    using Precise::pdouble;

    std::cout << "Using pdouble implementation: " << Precise::implementation << "\n";
    std::cout << "Precision for Precise::pdouble\n";
    std::cout << "Binary digits: " << std::numeric_limits<pdouble>::digits << "\n";
    std::cout << "Decimal digits: " << std::numeric_limits<pdouble>::digits10 << "\n";
    std::cout << "Max value: " << std::numeric_limits<pdouble>::max() << "\n";
    std::cout << "Epsilon: " << std::numeric_limits<pdouble>::epsilon() << "\n";


// Turn-off Lester-Nachman bisection code copyright notice
    asymm_mt2_lester_bisect::disableCopyrightMessage(); 

    fReader.SetTree(tree);

    if (tree) {
        treeName = tree->GetName();
        TFile *f = tree->GetCurrentFile();
        if (f) {
            inputFilePrefix = f->GetName();
            inputFilePrefix.resize(inputFilePrefix.size() - 5);  // remove the ".root" from the file name
            std::cout << "Init() -> inputFilePrefix: " << inputFilePrefix << " analyzing tree " << treeName << std::endl;
        }
    }

/*
    TH1D* old = (TH1D*)gDirectory->Get("hCutFlow");
    if (old) delete old;
    // Should move this into its own function
    hCutFlow = new TH1D("hCutFlow", "Trileptons; Cut Flow; Events per bin scaled to 400 inv fb", 9, -1.5, 7.5);
    hCutFlow->GetXaxis()->SetBinLabel(1, "All");
    hCutFlow->GetXaxis()->SetBinLabel(2, "GSNumber");
    hCutFlow->GetXaxis()->SetBinLabel(3, "PtOne");
    hCutFlow->GetXaxis()->SetBinLabel(4, "PtTwo");
    hCutFlow->GetXaxis()->SetBinLabel(5, "PtThree");
    hCutFlow->GetXaxis()->SetBinLabel(6, "PtFourVeto");
    hCutFlow->GetXaxis()->SetBinLabel(7, "BTagVeto");
    hCutFlow->GetXaxis()->SetBinLabel(8, "SIP3DCut");
    hCutFlow->GetXaxis()->SetBinLabel(9, "Selected");
*/

}

bool Ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}


#endif // #ifdef Ana_cxx
