    // Define histograms (partial set, expand as needed)
    TH1D* hparticles = new TH1D("hparticles", "; Number of particles", 20, -0.5, 19.5);
    TH1D* hcosth = new TH1D("hcosth", "; |cos#theta|", 1000, 0.0, 1.0);
    TH1D* hsinth = new TH1D("hsinth", "; sin#theta", 1000, 0.0, 1.0);
    TH1D* hmass = new TH1D("hpmass", ";Partonic Center of Mass Energy [GeV]", 400, 0.0, 2000.0);
    TH1D* hphmass = new TH1D("hfmass", ";Final-State Invariant Mass [GeV]", 400, 0.0, 2000.0);

    // Example extra histogram
    TH1D* hZmass = new TH1D("hZmass", "; Di-electron mass [GeV]", 500, 0.0, 250.0);

    TH1D* h2lOSeemass = new TH1D("h2lOSeemass", "; 2l OS Di-electron mass [GeV]", 125, 0.0, 250.0);
    TH1D* h2lOSemmass = new TH1D("h2lOSemmass", "; 2l OS Electron-muon mass [GeV]", 125, 0.0, 250.0);
    TH1D* h2lOSmmmass = new TH1D("h2lOSmmmass", "; 2l OS Di-muon mass [GeV]", 125, 0.0, 250.0);
    TH1D* h2lSSeemass = new TH1D("h2lSSeemass", "; 2l SS Di-electron mass [GeV]", 125, 0.0, 250.0);
    TH1D* h2lSSemmass = new TH1D("h2lSSemmass", "; 2l SS Electron-muon mass [GeV]", 125, 0.0, 250.0);
    TH1D* h2lSSmmmass = new TH1D("h2lSSmmmass", "; 2l SS Di-muon mass [GeV]", 125, 0.0, 250.0);

    TH1D* hOSeemass = new TH1D("hOSeemass", "; 3l OS Di-electron mass [GeV]", 125, 0.0, 250.0);
    TH1D* hOSemmass = new TH1D("hOSemmass", "; 3l OS Electron-muon mass [GeV]", 125, 0.0, 250.0);
    TH1D* hOSmmmass = new TH1D("hOSmmmass", "; 3l OS Di-muon mass [GeV]", 125, 0.0, 250.0);
    TH1D* hSSeemass = new TH1D("hSSeemass", "; 3l SS Di-electron mass [GeV]", 125, 0.0, 250.0);
    TH1D* hSSemmass = new TH1D("hSSemmass", "; 3l SS Electron-muon mass [GeV]", 125, 0.0, 250.0);
    TH1D* hSSmmmass = new TH1D("hSSmmmass", "; 3l SS Di-muon mass [GeV]", 125, 0.0, 250.0);

    TH1D* hPosMumass = new TH1D("hPosMumass", "; Positron-Muon mass [GeV]", 500, 0.0, 250.0);
    TH1D* hEleMumass = new TH1D("hEleMumass", "; Electron-Muon mass [GeV]", 500, 0.0, 250.0);
    TH1D* hWmass = new TH1D("hWmass", "; Muon-Neutrino mass [GeV]", 500, 0.0, 250.0);
    TH1D* h4lCharge = new TH1D("h4lCharge", "; 4l charge", 9, -4.5, 4.5);

    TH1D* hTriLeptonMass = new TH1D("hTriLeptonMass", "; Trilepton mass [GeV]", 250, 0.0, 500.0);
    TH1D* hTriLeptonMassOnZ = new TH1D("hTriLeptonMassOnZ", "; OnZ Trilepton mass [GeV]", 250, 0.0, 500.0);
    TH1D* hTriLeptonMassOffZ = new TH1D("hTriLeptonMassOffZ", "; OffZ Trilepton mass [GeV]", 250, 0.0, 500.0);

    TH1D* hTriLeptonMass0 = new TH1D("hTriLeptonMass0", "; Trilepton mass [GeV] in 0 OSSF events", 50, 0.0, 500.0);
    TH1D* hTriLeptonMassQ3 = new TH1D("hTriLeptonMassQ3", "; |Q|=3 Trilepton mass [GeV]", 50, 0.0, 500.0);
    TH1D* hTriLeptonMass0Q1 = new TH1D("hTriLeptonMass0Q1", "; Trilepton mass [GeV] in |Q|=1 0 OSSF events", 50, 0.0, 500.0);

    TH1D* hQuadLeptonMass57 = new TH1D("hQuadLeptonMass57", "; Four-lepton mass [GeV] in 3e-1mu and 1e-3mu events", 75, 0.0, 750.0);
    TH1D* hQuadLeptonMass57OffZ = new TH1D("hQuadLeptonMass57OffZ", "; OffZ Four-lepton mass [GeV] in 3e-1mu and 1e-3mu events", 75, 0.0, 750.0);
    TH1D* hQuadLeptonMass57OnZ = new TH1D("hQuadLeptonMass57OnZ", "; OnZ Four-lepton mass [GeV] in 3e-1mu and 1e-3mu events", 75, 0.0, 750.0);

    TH1D* hQuadLeptonMass = new TH1D("hQuadLeptonMass", "; Four-lepton mass [GeV]", 250, 0.0, 500.0);
    TH1D* hQuadLeptonMassOffZ = new TH1D("hQuadLeptonMassOffZ", "; OffZ Four-lepton mass [GeV]", 250, 0.0, 500.0);
    TH1D* hQuadLeptonMassOnZ = new TH1D("hQuadLeptonMassOnZ", "; OnZ Four-lepton mass [GeV]", 250, 0.0, 500.0);

    TH1D* hQuadLeptonMassQ2 = new TH1D("hQuadLeptonMassQ2", "; |Q|=2 Four-lepton mass [GeV]", 50, 0.0, 500.0);
    TH1D* hQuadLeptonMassQ2OnZ = new TH1D("hQuadLeptonMassQ2OnZ", "; |Q|=2 OnZ Four-lepton mass [GeV]", 50, 0.0, 500.0);
    TH1D* hQuadLeptonMassQ2OffZ = new TH1D("hQuadLeptonMassQ2OffZ", "; |Q|=2 OffZ Four-lepton mass [GeV]", 50, 0.0, 500.0);

    TH1D* hQuadLeptonMass57Q2 = new TH1D("hQuadLeptonMass57Q2", "; 57 |Q|=2 Four-lepton mass [GeV]", 50, 0.0, 500.0);
    TH1D* hQuadLeptonMass57Q2OnZ = new TH1D("hQuadLeptonMass57Q2OnZ", "; 57 |Q|=2 OnZ Four-lepton mass [GeV]", 50, 0.0, 500.0);
    TH1D* hQuadLeptonMass57Q2OffZ = new TH1D("hQuadLeptonMass57Q2OffZ", "; 57 |Q|=2 OffZ Four-lepton mass [GeV]", 50, 0.0, 500.0);

    TH1D* hnupT = new TH1D("hnupT", "; Neutrino pT [GeV]", 100, 0.0, 200.0);
    TH1D* hZpT = new TH1D("hZpT", "; Di-electron pT [GeV]", 100, 0.0, 200.0);
    TH1D* hWmT = new TH1D("hWmT", "; Muon-neutrino transverse mass [GeV]", 200, 0.0, 200.0);
    TH1D* hmT = new TH1D("hmT", "; Transverse mass [GeV]", 125, 0.0, 500.0);
    TH1D* hmTNew1 = new TH1D("hmTNew1", "nOSSF = 1; Transverse mass [GeV]", 80, 0.0, 400.0);
    TH1D* hmTNew2 = new TH1D("hmTNew2", "nOSSF = 2; Minimum transverse mass [GeV]", 80, 0.0, 400.0);
    TH1D* hmI = new TH1D("hmI", "; Mass of the invisible system (Eq 3)", 375, 0.0, 750.0);

    TH1D* heleEta = new TH1D("heleEta", "; Electron Eta", 150, -7.5, 7.5);
    TH1D* hposEta = new TH1D("hposEta", "; Positron Eta", 150, -7.5, 7.5);
    TH1D* hmuEta  = new TH1D("hmuEta", "; Muon Eta", 150, -7.5, 7.5);

    TH1D* heleRap = new TH1D("heleRap", "; Electron Rapidity", 150, -7.5, 7.5);
    TH1D* hposRap = new TH1D("hposRap", "; Positron Rapidity", 150, -7.5, 7.5);
    TH1D* hmuRap  = new TH1D("hmuRap", "; Muon Rapidity", 150, -7.5, 7.5);

    TH1D* helepT = new TH1D("helepT", "; Electron pT", 100, 0.0, 100.0);
    TH1D* hpospT = new TH1D("hpospT", "; Positron pT", 100, 0.0, 100.0);
    TH1D* hmupT  = new TH1D("hmupT", "; Muon pT", 100, 0.0, 100.0);

    TH1D* hZRap  = new TH1D("hZRap", "; Z Rapidity", 150, -7.5, 7.5);
    TH1D* hWRap  = new TH1D("hWRap", "; W Rapidity", 150, -7.5, 7.5);
    TH1D* hTriLRap  = new TH1D("hTriLRap", "; Trilepton Rapidity", 150, -7.5, 7.5);
    TH1D* hMETTwo  = new TH1D("hMETTwo", "; Dilepton MET [GeV]", 200, 0.0, 500.0);
    TH1D* hMETThree  = new TH1D("hMETThree", "; Trilepton MET [GeV]", 200, 0.0, 500.0);
    TH1D* hMETFour  = new TH1D("hMETFour", "; Quadlepton MET [GeV]", 200, 0.0, 500.0);
    TH1D* hmTFour  = new TH1D("hmTFour", "; Quadlepton mT [GeV]", 200, 0.0, 500.0);

    TH1D* hDRposele = new TH1D("hDRposele", "; #Delta R(e+, e-)", 100, 0.0, 10.0);
    TH1D* hDRelemu = new TH1D("hDRelemu", "; #Delta R(e-, mu+)", 100,  0.0, 10.0);
    TH1D* hDRposmu = new TH1D("hDRposmu", "; #Delta R(e+, mu+)", 100, 0.0, 10.0);

    TH1D* hJetsPT = new TH1D("hJetsPT", "; Jet system pT [GeV] ", 200, 0.0, 2000.0);

    TH1D* hdisc = new TH1D("hdisc", "; Quadratic discriminant", 2, -1.0e6, 1.0e6);
    TH1D* hnOSSF = new TH1D("hnOSSF", "; nOSSF lepton pairs in trilepton event", 3, -0.5, 2.5);
    TH1D* h3lCharge = new TH1D("h3lCharge", "; Trilepton event charge", 9, -5.5, 3.5);
    TH2D* hnelmu = new TH2D("hnelmu", "; Number of electrons; Number of muons", 4, -0.5, 3.5, 4, -0.5, 3.5);
    TH1D* hnleptons = new TH1D("hnleptons", "; Number of accepted leptons", 7, -0.5, 6.5);

    TH1D* h2LPtOne = new TH1D("h2LPtOne", "; Highest lepton pT [GeV]", 200, 0.0, 200.0);
    TH1D* h2LPtTwo = new TH1D("h2LPtTwo", "; Lowest lepton pT [GeV]", 200, 0.0, 200.0);

    TH1D* hPtOne = new TH1D("hPtOne", "; Highest lepton pT [GeV]", 200, 0.0, 200.0);
    TH1D* hPtTwo = new TH1D("hPtTwo", "; Middle lepton pT [GeV]", 200, 0.0, 200.0);
    TH1D* hPtThree = new TH1D("hPtThree", "; Lowest lepton pT [GeV]", 200, 0.0, 200.0);
    TH1D* hPtFour = new TH1D("hPtFour", "; Lowest lepton pT [GeV in 4-lepton events]", 100, 0.0, 100.0);

    TH1D* h2Code = new TH1D("h2Code", "; Two-lepton signed flavor code (OS= +ve, SS= -ve)", 9, -4.5, 4.5);
    TH1D* h3Code = new TH1D("h3Code", "; Three-lepton flavor code ", 4, 2.5, 6.5);
    TH1D* h4Code = new TH1D("h4Code", "; Four-lepton flavor code ", 5, 3.5, 8.5);
    TH1D* h5Code = new TH1D("h5Code", "; Five-lepton flavor code ", 6, 4.5, 10.5);
    TH1D* h6Code = new TH1D("h6Code", "; Six-lepton flavor code ", 7, 5.5, 12.5);

    TH1D* hSROSon = new TH1D("hSROSon", "; SROS-on category", 12, 0.5, 12.5);
    TH1D* hSROSoff = new TH1D("hSROSoff", "; SROS-off category", 12, 0.5, 12.5);
    TH1D* hSRSS = new TH1D("hSRSS", "; SRSS category", 4, 0.5, 4.5);

    TH2D* hmZvsyZ = new TH2D("hmZvsyZ", "; Di-electron mass [GeV] vs Z |rapidity|", 50, 0.0, 7.5, 100, 0.0, 250.0);

    TH2D* hwhich = new TH2D("hwhich", "; Process ID; NParticles", 12, 0.5, 12.5, 12, 5.5, 17.5);
    TH2D* hwhich2 = new TH2D("hwhich2", "; Process ID; N Accepted Leptons", 12, 0.5, 12.5, 7, -0.5, 6.5);
    TH1D* hprocess = new TH1D("hprocess", "; Process ID; ", 12, 0.5, 12.5);

    TH1D* hOnZ1 = new TH1D("hOnZ1", "; (mll - mZ); ", 100, -100.0, 100.0);
    TH2D* hOnZ2 = new TH2D("hOnZ2", "; (mll - mZ)_0; (mll - mz)_1 ", 20, -20.0, 20.0, 20, -20.0, 20.0);

    TH1D* h2lOnZ = new TH1D("h2lOnZ", "; (mll - mZ); ", 150, -100.0, 200.0);
    TH1D* h3lOnZ = new TH1D("h3lOnZ", "; (mll - mZ); ", 150, -100.0, 200.0);
    TH1D* h4lOnZ = new TH1D("h4lOnZ", "; (mll - mZ); ", 150, -100.0, 200.0);
    TH1D* h5lOnZ = new TH1D("h5lOnZ", "; (mll - mZ); ", 150, -100.0, 200.0);
    TH1D* h6lOnZ = new TH1D("h6lOnZ", "; (mll - mZ); ", 150, -100.0, 200.0);

    std::vector<std::string> SRSS_labels = {"eee", "ee#mu", "e#mu#mu", "#mu#mu#mu"};
    std::vector<std::string> SROSon_labels = {
        "a-eee", "a-ee#mu", "a-e#mu#mu", "a-#mu#mu#mu",
        "b-eee", "b-ee#mu", "b-e#mu#mu", "b-#mu#mu#mu",
        "c-eee", "c-ee#mu", "c-e#mu#mu", "c-#mu#mu#mu"
    };
    std::vector<std::string> SROSoff_labels = SROSon_labels;  // same as above

    for (size_t i = 0; i < SRSS_labels.size(); ++i) {
        hSRSS->GetXaxis()->SetBinLabel(i + 1, SRSS_labels[i].c_str());
        hSRSS->GetXaxis()->SetLabelSize(0.06);
    }

    for (size_t i = 0; i < SROSon_labels.size(); ++i) {
        hSROSon->GetXaxis()->SetBinLabel(i + 1, SROSon_labels[i].c_str());
        hSROSoff->GetXaxis()->SetBinLabel(i + 1, SROSoff_labels[i].c_str());
    }

    hSROSon->GetXaxis()->LabelsOption("h");
    hSROSoff->GetXaxis()->LabelsOption("h");

    // MT2 histograms
    double ptmax = 550;
    double sd4 = 24.265 * 4;
    double mma = 0.0;
    std::string heading = "MT2";
    std::string xaxis = "MT2 [GeV]";
    std::string title = heading + ";" + xaxis + "; Weighted Event Count";

    TH1D* hmT2 = new TH1D("hmT2", title.c_str(), 125, 0.0, 250.0);
    TH1D* hmT2P1 = new TH1D("hmT2P1", title.c_str(), 125, 0.0, 250.0);
    TH1D* hmT2P2 = new TH1D("hmT2P2", title.c_str(), 125, 0.0, 250.0);
    TH1D* hmT2P3 = new TH1D("hmT2P3", title.c_str(), 125, 0.0, 250.0);
    TH1D* hmT2P4 = new TH1D("hmT2P4", title.c_str(), 125, 0.0, 250.0);
    TH1D* hmT2P5678 = new TH1D("hmT2P5678", title.c_str(), 125, 0.0, 250.0);

    TH1D* hnit = new TH1D("hnit", "Number of iterations", 10001, -0.5, 10000.5);
    TH1D* hcalls = new TH1D("hcalls", "Number of calls", 10001, -0.5, 10000.5);

    TH1D* hbetaz = new TH1D("hbetaz","Beta z for dileptons",100,-1.0,1.0);
    TH1D* habetaz = new TH1D("habetaz","|Beta z| for dileptons; |#beta_{z}|",50,0.0,1.0);
    TH1D* hacosthstar = new TH1D("hacosthstar","costh* for dilepton; |cos#theta*|",50,0.0,1.0);

