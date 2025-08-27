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

// Move all the variables in the TTree into a version dependent include statement that I plan to switch among using symbolic links.
// Need to also make sure that it does get recompiled ...
#include "AnaVersions.h"

#if ANA_NTUPLE_VERSION == 2
#include "Ana_NtupleVariables_V2.h"
#elif ANA_NTUPLE_VERSION == 3
#include "Ana_NtupleVariables_V3.h"
#elif ANA_NTUPLE_VERSION == 4
#include "Ana_NtupleVariables_V4.h"
#else
#error "Unsupported ANA_NTUPLE_VERSION"
#endif

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

// Make sure we know which ntuple version the program thinks it is using.
#if defined(ANA_NTUPLE_VERSION)
   std::cout << ">>> Initializing Ana with ANA_NTUPLE_VERSION = " << ANA_NTUPLE_VERSION << std::endl;
#else
   std::cout << ">>> Initializing Ana with undefined ANA_NTUPLE_VERSION!" << std::endl;
#endif

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
