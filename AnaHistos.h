TH1D *hMET = new TH1D("hMET","MET; MET [GeV]; Events per bin",100,0.0,1000.0);
TH1D *hMET_phi = new TH1D("hMET_phi","MET phi; MET phi [rad]; Events per bin",100,-6.3,6.3); //probably should be [-pi, pi]
TH1D *haltMET = new TH1D("haltMET","altMET; altMET [GeV]; Events per bin",100,0.0,1000.0);
TH1D *haltMET_phi = new TH1D("haltMET_phi","altMET phi; altMET phi [rad]; Events per bin",100,-6.3,6.3); //probably should be [-pi, pi]
TH1D *hleptonPT = new TH1D("hleptonPT","PT (lepton); PT [GeV]; Leptons per bin",200,0.0,200.0);
TH1D *hnleptons = new TH1D("hnleptons"," G+S leptons; Nleptons (Gold+Silver); Events per bin",10,-0.5,9.5);
TH1D *hnleptons_hipt = new TH1D("hnleptons_hipt"," G+S leptons with high pT; Nleptons (Gold+Silver); Events per bin",10,-0.5,9.5);

TH1D *hngs = new TH1D("hngs"," G+S leptons; Nleptons (Gold+Silver); Events per bin",10,-0.5,9.5);
TH1D *hnbronze = new TH1D("hnbronze"," Bronze leptons; Nleptons (Bronze); Events per bin",10,-0.5,9.5);

TH1D *hmll = new TH1D("hmll","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hptll = new TH1D("hptll","Dilepton Events; ptll [GeV]; Events per bin",100,0.0,200.0);
TH1D *hphistar = new TH1D("hphistar","Dilepton Events; phistar; Events per bin",100,0.0,5.0);
TH1D *hacop = new TH1D("hacop","Dilepton Events; Acoplanarity [rad]; Events per bin",200,0.0,M_PI);
TH1D *hcosthEta = new TH1D("hcosthEta","Dilepton Events; costh(Eta); Events per bin",50,0.0,1.0);
TH1D *h2lnjets = new TH1D("h2lnjets","Dilepton Events; N jets; Events per bin",12,-0.5,11.5);
TH1D *h2lMET = new TH1D("h2lMET","Dileptons; MET [GeV]; Events per bin",80,0.0,200.0);
TH1D *h2lHTid = new TH1D("h2lHTid","Dileptons; HT [GeV]; Events per bin",80,0.0,200.0);
TH1D *h2lflavor   = new TH1D("h2lflavor","Dileptons; Two-lepton flavor; Events per bin scaled to 400 inv fb", 3, 1.5, 4.5);
TH1D *h2lflavorOS = new TH1D("h2lflavorOS","Dileptons; Two-lepton flavor OS; Events per bin scaled to 400 inv fb", 3, 1.5, 4.5);
TH1D *h2lflavorSS = new TH1D("h2lflavorSS","Dileptons; Two-lepton flavor SS; Events per bin scaled to 400 inv fb", 3, 1.5, 4.5);
TH1D *h2lflavorSST = new TH1D("h2lflavorSST","Dileptons; Two-lepton flavor SS (with Tight-Charge = 2); Events per bin scaled to 400 inv fb", 3, 1.5, 4.5);

TH1D *hmllEE = new TH1D("hmllEE","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hmllEM = new TH1D("hmllEM","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hmllMM = new TH1D("hmllMM","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);

TH1D *hmllEEOS = new TH1D("hmllEEOS","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hmllEMOS = new TH1D("hmllEMOS","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hmllMMOS = new TH1D("hmllMMOS","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);

TH1D *hmllEESS = new TH1D("hmllEESS","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hmllEMSS = new TH1D("hmllEMSS","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hmllMMSS = new TH1D("hmllMMSS","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);

TH1D *hmllEESST = new TH1D("hmllEESST","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hmllEMSST = new TH1D("hmllEMSST","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hmllMMSST = new TH1D("hmllMMSST","Dilepton Events; mll [GeV]; Events per bin",400,0.0,400.0);

TH1D *hmll3 = new TH1D("hmll3","Trilepton Events; mll [GeV]; Dileptons per bin",400,0.0,400.0);

TH1D *hMT2LN0 = new TH1D("hMT2LN0", "M(IA), M(IB) = 0;  MT2 (GeV); Events per bin", 800, 0.0, 800.0);
TH1D *hMT2LN0SMALL = new TH1D("hMT2LN0SMALL", "M(IA), M(IB) = 0;  MT2 (GeV); Events per bin", 1000, 0.0, 2.0);

TH1D* habetaz = new TH1D("habetaz","|Beta z| for dileptons; |#beta_{z}|",50,0.0,1.0);

TH1D *hm3l = new TH1D("hm3l","Trilepton Mass; mlll [GeV]; Events per bin",200,0.0,400.0);
TH1D *hpt3l = new TH1D("hpt3l","Trilepton Events; pt3l [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt3lall = new TH1D("hpt3lall","Trilepton Events; pt3l [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt3ljets = new TH1D("hpt3ljets","Trilepton Events; pt (3l + jets) [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt3l0jets = new TH1D("hpt3l0jets","Four lepton Events; pt (3l) [GeV] (No jets); Events per bin",100,0.0,100.0);
TH1D *hpt3lwjets = new TH1D("hpt3lwjets","Four lepton Events; pt (3l + >=1 jet) [GeV]; Events per bin",100,0.0,100.0);

TH1D *hm3lbtv = new TH1D("hm3lbtv","Trileptons with loose b-tag veto; mlll [GeV]; Events per bin",200,0.0,400.0);
TH1D *h3lcharge = new TH1D("h3lcharge","Trileptons ; Trilepton Charge; Events per bin", 4, -4.0, 4.0);
TH1D *h3lchargetight = new TH1D("h3lchargetight","Trileptons ; Trilepton Charge (Tight Charge Cut); Events per bin", 4, -4.0, 4.0);
TH1D *h3lflavor = new TH1D("h3lflavor","Trileptons; Three-lepton flavor; Events per bin scaled to 400 inv fb", 4, 2.5, 6.5);
TH1D *hminmll = new TH1D("hminmll","Trileptons ; min(mll) [GeV]; Events per bin",200,0.0,200.0);
TH1D *hminmll3l = new TH1D("hminmll3l","Trileptons ; min(mll) [GeV]; Events per bin",100,0.0,100.0);
TH1D *hmaxmll3l = new TH1D("hmaxmll3l","Trileptons ; max(mll) [GeV]; Events per bin",80,0.0,200.0);

TH1D *h3lMET = new TH1D("h3lMET","Trileptons; MET [GeV]; Events per bin",200,0.0,200.0);
TH1D *ph3lMET = new TH1D("ph3lMET","Trileptons; MET [GeV]; Events per bin",200,0.0,200.0);
TH1D *nh3lMET = new TH1D("nh3lMET","Trileptons; MET [GeV]; Events per bin",200,0.0,200.0);
TH1D *uph3lMET = new TH1D("uph3lMET","Trileptons; MET [GeV]; Events per bin",200,0.0,200.0);
TH1D *unh3lMET = new TH1D("unh3lMET","Trileptons; MET [GeV]; Events per bin",200,0.0,200.0);

TH1D *h3lHTid = new TH1D("h3lHTid","Trileptons; HT [GeV]; Events per bin",80,0.0,200.0);
TH1D *h3lHTnoid = new TH1D("h3lHTnoid","Trileptons; HT [GeV]; Events per bin",80,0.0,200.0);
TH1D *h3lnjets = new TH1D("h3lnjets","Trileptons; N jets; Events per bin",12,-0.5,11.5);
TH1D *h3lnossf = new TH1D("h3lnossf","Trileptons; n OSSF; Events per bin",4,-0.5,3.5);
TH1D *h3ldevZ = new TH1D("h3ldevZ","Trileptons; On Z (mll - mZ) variable [GeV]; Events per bin",78,-97.5,97.5);
TH1D *h3ldevZf = new TH1D("h3ldevZf","Trileptons; On Z (mll - mZ) variable [GeV]; Events per bin",390,-97.5,97.5);
TH1D *h3ldevZV = new TH1D("h3ldevZV","Trileptons; On Z (mlll - mZ) variable [GeV]; Events per bin",390,-97.5,97.5);

TH1D *h4lnossf = new TH1D("h4lnossf","Quadleptons; n OSSF (combinations); Events per bin",7,-0.5,6.5);
TH1D *h4ldevZ = new TH1D("h4ldevZ","Quadleptons; On Z (mll - mZ) variable [GeV]; Events per bin",78,-97.5,97.5);
TH1D *h4ldevZf = new TH1D("h4ldevZf","Quadleptons; On Z (mll - mZ) variable [GeV]; Events per bin",390,-97.5,97.5);
TH1D *h4ldevZV = new TH1D("h4ldevZV","Quadleptons; On Z (m4l - mZ) variable [GeV]; Events per bin",390,-97.5,97.5);
TH1D *h4ldevZVNZZL = new TH1D("h4ldevZVNZZL","Quadleptons; On Z (m4l - mZ) variable [GeV]; Events per bin",390,-97.5,97.5);
TH1D *h4lnjets = new TH1D("h4lnjets","Quadleptons; N jets; Events per bin",12,-0.5,11.5);
TH1D *h4lMET = new TH1D("h4lMET","Quadleptons; MET [GeV]; Events per bin",80,0.0,200.0);
TH1D *h4lHTid = new TH1D("h4lHTid","Quadleptons; HT [GeV]; Events per bin",80,0.0,200.0);
TH1D *h4lHTnoid = new TH1D("h4lHTnoid","Quadleptons; HT [GeV]; Events per bin",80,0.0,200.0);
TH1D *hpt4l = new TH1D("hpt4l","Four lepton Events; pt4l [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt4ljets = new TH1D("hpt4ljets","Four lepton Events; pt (4l + jets) [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt4l0jets = new TH1D("hpt4l0jets","Four lepton Events; pt (4l) [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt4lwjets = new TH1D("hpt4lwjets","Four lepton Events; pt (4l + >=1 jet) [GeV]; Events per bin",100,0.0,100.0);

TH1D *hm3lF3 = new TH1D("hm3lF3","Trilepton Mass (eee); mlll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm3lF4 = new TH1D("hm3lF4","Trilepton Mass (ee#mu); mlll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm3lF5 = new TH1D("hm3lF5","Trilepton Mass (e#mu#mu); mlll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm3lF6 = new TH1D("hm3lF6","Trilepton Mass (#mu#mu#mu); mlll [GeV]; Events per bin",400,0.0,400.0);

TH1D *hm4l = new TH1D("hm4l","Four leptons; Mass (4l) [GeV]; Events per bin",200,0.0,400.0);
TH1D *hm4lf = new TH1D("hm4lf","Four leptons; Mass (4l) [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm4lZZL = new TH1D("hm4lZZL","Four leptons; Mass (4l) [GeV]; Events per bin",200,0.0,400.0);
TH1D *hm4lfZZL = new TH1D("hm4lfZZL","Four leptons; Mass (4l) [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm4lffZZL = new TH1D("hm4lffZZL","Four leptons; Mass (4l) [GeV]; Events per bin",800,0.0,400.0);
TH1D *hm4lNZZL = new TH1D("hm4lNZZL","Four leptons; Mass (4l) [GeV]; Events per bin",200,0.0,400.0);
TH1D *hm4lfNZZL = new TH1D("hm4lfNZZL","Four leptons; Mass (4l) [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm4lffNZZL = new TH1D("hm4lffNZZL","Four leptons; Mass (4l) [GeV]; Events per bin",800,0.0,400.0);

TH1D *hm4lbtv = new TH1D("hm4lbtv","Four leptons with loose b-tag veto; Mass (4l) [GeV]; Events per bin",400,0.0,400.0);
TH1D *h4lcharge = new TH1D("h4lcharge","Four leptons ; Four-lepton charge; Events per bin", 5, -5.0, 5.0);
TH1D *h4lchargetight = new TH1D("h4lchargetight","Four leptons ; Four-lepton charge (tight charge cut); Events per bin", 5, -5.0, 5.0);
TH1D *h4lflavor = new TH1D("h4lflavor","Four leptons; Four-lepton flavor; Events per bin scaled to 400 inv fb", 5, 3.5, 8.5);

TH1D* hmT = new TH1D("hmT", "Trileptons;  Transverse mass [GeV]; Leptons per bin", 200, 0.0, 400.0);
TH1D* hmT1 = new TH1D("hmT1", "Trileptons; L1 transverse mass [GeV]; Events per bin", 200, 0.0, 400.0);
TH1D* hmT2 = new TH1D("hmT2", "Trileptons; L2 transverse mass [GeV]; Events per bin", 200, 0.0, 400.0);
TH1D* hmT3 = new TH1D("hmT3", "Trileptons; L3 transverse mass [GeV]; Events per bin", 200, 0.0, 400.0);
TH1D* hmT3l = new TH1D("hmT3l", "Trileptons; L123 transverse mass [GeV]; Events per bin", 200, 0.0, 400.0);
TH1D* h3lmaxmT = new TH1D("h3lmaxmT", "Trileptons; Max. single lepton mT [GeV]; Events per bin", 80, 0.0, 400.0);

TH1D* htrisel = new TH1D("htrisel","Trileptons; Selection word; Events per bin scaled to 400 inv fb",128,-0.5,127.5);
TH1D* hminSIP3D = new TH1D("hminSIP3D","Trileptons; min(SIP3D); Events per bin scaled to 400 inv fb",80, 0.0 ,8.0);
TH1D* hmaxSIP3D = new TH1D("hmaxSIP3D","Trileptons; max(SIP3D); Events per bin scaled to 400 inv fb",80, 0.0 ,8.0);
TH1D* hmaxSIP3DSel = new TH1D("hmaxSIP3DSel","Trileptons; max(SIP3D) Sel; Events per bin scaled to 400 inv fb",80, 0.0 ,8.0);
TH1D* hmaxSIP3DFail = new TH1D("hmaxSIP3DFail","Trileptons; max(SIP3D) Fail; Events per bin scaled to 400 inv fb",80, 0.0 ,8.0);
TH1D* hPtOne = new TH1D("hPtOne","Trileptons; PtOne [GeV]; Events per bin scaled to 400 inv fb",160, 0.0, 40.0);
TH1D* hPtTwo = new TH1D("hPtTwo","Trileptons; PtTwo [GeV]; Events per bin scaled to 400 inv fb",160, 0.0, 40.0);
TH1D* hPtThree = new TH1D("hPtThree","Trileptons; PtThree [GeV]; Events per bin scaled to 400 inv fb",160, 0.0, 40.0);

TH1D* hPtOne4l = new TH1D("hPtOne4l","Four leptons; PtOne [GeV]; Events per bin scaled to 400 inv fb",160, 0.0, 40.0);
TH1D* hPtTwo4l = new TH1D("hPtTwo4l","Four leptons; PtTwo [GeV]; Events per bin scaled to 400 inv fb",160, 0.0, 40.0);
TH1D* hPtThree4l = new TH1D("hPtThree4l","Four leptons; PtThree [GeV]; Events per bin scaled to 400 inv fb",160, 0.0, 40.0);
TH1D* hPtFour4l = new TH1D("hPtFour4l","Four leptons; PtFour [GeV]; Events per bin scaled to 400 inv fb",160, 0.0, 40.0);

TH1D* hPtFourVeto = new TH1D("hPtFourVeto","Trileptons; PtFourVeto [GeV]; Events per bin scaled to 400 inv fb",100, -5.0, 20.0);
TH1D* hPtFiveVeto = new TH1D("hPtFiveVeto","Four leptons; PtFiveVeto [GeV]; Events per bin scaled to 400 inv fb",60, -5.0, 55.0);
TH1D* hmaxSIP3D4l = new TH1D("hmaxSIP3D4l","Four leptons; max(SIP3D); Events per bin scaled to 400 inv fb",80, 0.0, 8.0);
TH1D* hminmll4l = new TH1D("hminmll4l","Four leptons ; min(mll) [GeV]; Events per bin",400,0.0,100.0);
TH1D* hothermll4l = new TH1D("hothermll4l","Four leptons ; Other mll [GeV]; Events per bin",200,0.0,200.0);
TH1D* hminm3lfor4l = new TH1D("hminm3lfor4l","Four leptons ; min m3l [GeV]; Events per bin",200,0.0,200.0);
TH1D* hmincmass4l = new TH1D("hmincmass4l","Four leptons ; min complementary mass [GeV]; Events per bin",200,0.0,200.0);
TH1D* hmincmassp4l = new TH1D("hmincmassp4l","Four leptons ; alt. min complementary mass [GeV]; Events per bin",200,0.0,200.0);
TH1D* hmincmasspp4l = new TH1D("hmincmasspp4l","Four leptons ; ALT. min complementary mass [GeV]; Events per bin",200,0.0,200.0);

TH2D* h4lmasses  = new TH2D("h4lmasses  ","Four leptons; min mll mass; other mll mass",12,0.0,24.0,25,0.0,100.0);
TH2D* h4lmasses1 = new TH2D("h4lmasses1 ","Four leptons; min mll mass; other mll mass",12,0.0,24.0,25,0.0,100.0);
TH2D* h4lmasses2 = new TH2D("h4lmasses2 ","Four leptons; min mll mass; other mll mass",12,0.0,24.0,25,0.0,100.0);
TH2D* h4lmasses3 = new TH2D("h4lmasses3 ","Four leptons; min mll mass; other mll mass",12,0.0,24.0,25,0.0,100.0);

TH2D* h4lOmasses  = new TH2D("h4lOmasses  ","Four leptons; min 3l mass; other mll mass",25,0.0,100.0,25,0.0,100.0);
TH2D* h4lOmasses1 = new TH2D("h4lOmasses1 ","Four leptons; min 3l mass; other mll mass",25,0.0,100.0,25,0.0,100.0);
TH2D* h4lOmasses2 = new TH2D("h4lOmasses2 ","Four leptons; min 3l mass; other mll mass",25,0.0,100.0,25,0.0,100.0);
TH2D* h4lOmasses3 = new TH2D("h4lOmasses3 ","Four leptons; min 3l mass; other mll mass",25,0.0,100.0,25,0.0,100.0);

TH2D* h4lPmasses  = new TH2D("h4lPmasses  ","Four leptons; min mll mass; min 3l mass",12,0.0,24.0,25,0.0,100.0);
TH2D* h4lPmasses1 = new TH2D("h4lPmasses1 ","Four leptons; min mll mass; min 3l mass",12,0.0,24.0,25,0.0,100.0);
TH2D* h4lPmasses2 = new TH2D("h4lPmasses2 ","Four leptons; min mll mass; min 3l mass",12,0.0,24.0,25,0.0,100.0);
TH2D* h4lPmasses3 = new TH2D("h4lPmasses3 ","Four leptons; min mll mass; min 3l mass",12,0.0,24.0,25,0.0,100.0);

TH1D* h3lSIP3DRChisq = new TH1D("h3lSIP3DRChisq","Trileptons; Event SIP3D Chisq/dof; Events per bin scaled to 400 inv fb",100, 0.0, 20.0);
TH1D* h3lSIP3DProb = new TH1D("h3lSIP3DProb","Trileptons; Event SIP3D pfit; Events per bin scaled to 400 inv fb",100, 0.0, 1.0);
TH1D* h3lSIP3DLogProb = new TH1D("h3lSIP3DLogProb","Trileptons; Event SIP3D log10(pfit); Events per bin scaled to 400 inv fb",100, -9.0, 1.0);
TH1D* h3lSIP3DSignificance = new TH1D("h3lSIP3DSignificance","Trileptons; Event SIP3D Significance; Events per bin scaled to 400 inv fb",160,-8.0,8.0);
TH1D* h4lSIP3DRChisq = new TH1D("h4lSIP3DRChisq","Four leptons; Event SIP3D Chisq/dof; Events per bin scaled to 400 inv fb",100, 0.0, 20.0);
TH1D* h4lSIP3DProb = new TH1D("h4lSIP3DProb","Four leptons; Event SIP3D pfit; Events per bin scaled to 400 inv fb",100, 0.0, 1.0);
TH1D* h4lSIP3DLogProb = new TH1D("h4lSIP3DLogProb","Four leptons; Event SIP3D log10(pfit); Events per bin scaled to 400 inv fb",100, -9.0, 1.0);
TH1D* h4lSIP3DSignificance = new TH1D("h4lSIP3DSignificance","Four leptons; Event SIP3D Significance; Events per bin scaled to 400 inv fb",160,-8.0,8.0);

TH1D* hDiCutFlow = new TH1D("hDiCutFlow", "Dileptons; Cut Flow; Events per bin scaled to 400 inv fb", 8, -1.5, 6.5);
TH1D* hDiCutFlowNonTau = new TH1D("hDiCutFlowNonTau", "Dileptons; Cut Flow; Events per bin scaled to 400 inv fb", 8, -1.5, 6.5);
TH1D* hCutFlow = new TH1D("hCutFlow", "Trileptons; Cut Flow; Events per bin scaled to 400 inv fb", 17, -1.5, 15.5);   // Can we make these be adjusted automatically according to the enum?
TH1D* hQuadCutFlow = new TH1D("hQuadCutFlow", "Quadleptons; Cut Flow; Events per bin scaled to 400 inv fb", 17, -1.5, 15.5);

TH1D* hXDiCutFlow = new TH1D("hXDiCutFlow", "Dileptons; Exclusive Cut Flow; Events per bin scaled to 400 inv fb", 7, -1.5, 5.5);
TH1D* hXCutFlow = new TH1D("hXCutFlow", "Trileptons; Exclusive Cut Flow; Events per bin scaled to 400 inv fb", 16, -1.5, 14.5);
TH1D* hXQuadCutFlow = new TH1D("hXQuadCutFlow", "Quadleptons; Exclusive Cut Flow; Events per bin scaled to 400 inv fb", 16, -1.5, 14.5);

TH1D* hprocesslowt = new TH1D("hprocesslowt", " ; Production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hprocesshiwt = new TH1D("hprocesshiwt", " ; Production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);

TH1D* hTriProcess = new TH1D("hTriProcess", "Trileptons; Production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hQuadProcess = new TH1D("hQuadProcess", "Four leptons; Production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);

TH1D* hDiTrigger  = new TH1D("hDiTrigger", "Dileptons; Trigger; Events per bin scaled to 400 inv fb", 12, -0.5, 11.5);
TH1D* hTriTrigger  = new TH1D("hTriTrigger", "Trileptons; Trigger; Events per bin scaled to 400 inv fb", 12, -0.5, 11.5);
TH1D* hQuadTrigger = new TH1D("hQuadTrigger", "Four leptons; Trigger; Events per bin scaled to 400 inv fb", 12, -0.5, 11.5);

TH1D* hTriQ3 = new TH1D("hTriQ3"," Trileptons (|Q|=3);  ; Events per bin scaled to 400 inv fb",4,2.5,6.5);
TH1D* hTriQ3Tight = new TH1D("hTriQ3Tight"," Trileptons (|Q|=3) Tight;  ; Events per bin scaled to 400 inv fb",4,2.5,6.5);
TH1D* hQuadQ2 = new TH1D("hQuadQ2"," Four leptons (|Q|=2);  ; Events per bin scaled to 400 inv fb",5,3.5,8.5);
TH1D* hQuadQ2Tight = new TH1D("hQuadQ2Tight"," Four leptons (|Q|=2) Tight;  ; Events per bin scaled to 400 inv fb",5,3.5,8.5);

TH1D* h2lgenMET = new TH1D("h2lgenMET","Dileptons; GenMET [GeV]; Events per bin scaled to 400 inv fb",101,-0.5,100.5);
TH1D* h3lgenMET = new TH1D("h3lgenMET","Trileptons; GenMET [GeV]; Events per bin scaled to 400 inv fb",101,-0.5,100.5);
TH1D* h4lgenMET = new TH1D("h4lgenMET","Four leptons; GenMET [GeV]; Events per bin scaled to 400 inv fb",101,-0.5,100.5);

TH1D* h2lgenMETf = new TH1D("h2lgenMETf","Dileptons; GenMET; Events per bin scaled to 400 inv fb",101,-0.005,1.005);
TH1D* h3lgenMETf = new TH1D("h3lgenMETf","Trileptons; GenMET; Events per bin scaled to 400 inv fb",101,-0.005,1.005);
TH1D* h4lgenMETf = new TH1D("h4lgenMETf","Four leptons; GenMET; Events per bin scaled to 400 inv fb",101,-0.005,1.005);

TH1D* h2lgenMETvf = new TH1D("h2lgenMETvf","Dileptons; GenMET; Events per bin scaled to 400 inv fb",100,-2.0e-7,2.0e-7);
TH1D* h3lgenMETvf = new TH1D("h3lgenMETvf","Trileptons; GenMET; Events per bin scaled to 400 inv fb",100,-2.0e-7,2.0e-7);
TH1D* h4lgenMETvf = new TH1D("h4lgenMETvf","Four leptons; GenMET; Events per bin scaled to 400 inv fb",100,-2.0e-7,2.0e-7);

TH1D *hpt2l = new TH1D("hpt2l","Dilepton Events; pt2l [GeV]; Events per bin",100,0.0,200.0);

TH1D *hpt2lall = new TH1D("hpt2lall","Dilepton Events; pt2l [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt2ljets = new TH1D("hpt2ljets","Two lepton Events; pt (2l + jets) [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt2l0jets = new TH1D("hpt2l0jets","Two lepton Events; pt (2l) + No jets [GeV]; Events per bin",100,0.0,100.0);
TH1D *hpt2lwjets = new TH1D("hpt2lwjets","Two lepton Events; pt (2l + >=1 jet) [GeV]; Events per bin",100,0.0,100.0);

TH1D* h2ltaunus = new TH1D("h2ltaunus","Dileptons; Number of generator-level tau neutrinos; Events per bin scaled to 400 inv fb",10,-0.5,9.5);
TH1D* h3ltaunus = new TH1D("h3ltaunus","Trileptons; Number of generator-level tau neutrinos; Events per bin scaled to 400 inv fb",10,-0.5,9.5);
TH1D* h4ltaunus = new TH1D("h4ltaunus","Quadleptons; Number of generator-level tau neutrinos; Events per bin scaled to 400 inv fb",10,-0.5,9.5);

TH1D* h2lnus = new TH1D("h2lnus","Dileptons; Number of generator-level neutrinos; Events per bin scaled to 400 inv fb",10,-0.5,9.5);
TH1D* h3lnus = new TH1D("h3lnus","Trileptons; Number of generator-level neutrinos; Events per bin scaled to 400 inv fb",10,-0.5,9.5);
TH1D* h4lnus = new TH1D("h4lnus","Quadleptons; Number of generator-level neutrinos; Events per bin scaled to 400 inv fb",10,-0.5,9.5);

TH1D* h2lnleptons = new TH1D("h2lnleptons","Dileptons; Number of G+S leptons; Events per bin scaled to 400 inv fb",10,-0.5,9.5);
TH1D* h3lnleptons = new TH1D("h3lnleptons","Trileptons; Number of G+S leptons; Events per bin scaled to 400 inv fb",10,-0.5,9.5);
TH1D* h4lnleptons = new TH1D("h4lnleptons","Quadleptons; Number of G+S leptons; Events per bin scaled to 400 inv fb",10,-0.5,9.5);

TH1D* hselection = new TH1D("hselection","Event Overlaps; Selection Code; Events per bin scaled to 400 inv fb",7,1.5,8.5);

TH1D* hsel4 = new TH1D("hsel4","Quadleptons (equal 4L); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel5 = new TH1D("hsel5","Quadleptons (ge 5L); ;Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel6 = new TH1D("hsel6","Quadleptons (ge 6L); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);

TH1D* hsel5process = new TH1D("hsel5process","Quadleptons (ge 5L); ;Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4Q2process = new TH1D("hsel4Q2process","Quadleptons (Q2); ;Events per bin scaled to 400 inv fb",3,0.5,3.5);

TH1D* hsel3C1 = new TH1D("hsel3C1","Trileptons; eee ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel3C2 = new TH1D("hsel3C2","Trileptons; ee#mu ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel3C3 = new TH1D("hsel3C3","Trileptons; e#mu#mu ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel3C4 = new TH1D("hsel3C4","Trileptons; #mu#mu#mu ; Events per bin scaled to 400 inv fb",3,0.5,3.5);

TH1D* hsel3P1 = new TH1D("hsel3P1","Trileptons; (OSe#mu, e^{+}) ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel3P2 = new TH1D("hsel3P2","Trileptons; (OSe#mu, #mu^{+}) ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel3P3 = new TH1D("hsel3P3","Trileptons; (OSe#mu, e^{-}) ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel3P4 = new TH1D("hsel3P4","Trileptons; (OSe#mu, #mu^{-}) ; Events per bin scaled to 400 inv fb",3,0.5,3.5);

TH1D* hsel4C1 = new TH1D("hsel4C1","Quadleptons (eeee); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C2 = new TH1D("hsel4C2","Quadleptons (eeem); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C3 = new TH1D("hsel4C3","Quadleptons (eemm); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C4 = new TH1D("hsel4C4","Quadleptons (emmm); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C5 = new TH1D("hsel4C5","Quadleptons (mmmm); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C6 = new TH1D("hsel4C6","Quadleptons (Q2); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C7 = new TH1D("hsel4C7","Quadleptons (Q2T); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C8 = new TH1D("hsel4C8","Quadleptons (Q2 or 5L); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C9 = new TH1D("hsel4C9","Quadleptons (Q2T or 5L); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);
TH1D* hsel4C10 = new TH1D("hsel4C10","Quadleptons (5L); ; Events per bin scaled to 400 inv fb",3,0.5,3.5);

TH1D* hsel4Exclusive = new TH1D("hsel4Exclusive","Quadleptons; Exclusive Category; Events per bin scaled to 400 inv fb",7,0.5,7.5);

TH1D* hcentraleta3l = new TH1D("hcentraleta3l","Trileptons; Most central |eta|; Events per bin scaled to 400 inv fb",25,0.0,2.5);
TH1D* hcentraleta4l = new TH1D("hcentraleta4l","Quadleptons; Most central |eta|; Events per bin scaled to 400 inv fb",25,0.0,2.5);
TH1D* hforwardeta3l = new TH1D("hforwardeta3l","Trileptons; Most forward |eta|; Events per bin scaled to 400 inv fb",25,0.0,2.5);
TH1D* hforwardeta4l = new TH1D("hforwardeta4l","Quadleptons; Most forward |eta|; Events per bin scaled to 400 inv fb",25,0.0,2.5);

TH2D* hpairCodes4l = new TH2D("hpairCodes4l","Quadleptons; minPairCode; compPairCode",6,0.5,6.5,6,0.5,6.5);
TH2D* hpairCodesA4l = new TH2D("hpairCodesA4l","Quadleptons; minPairCode; compPairCode",4,0.5,4.5,4,0.5,4.5);
TH2D* hpairCodesQ4l = new TH2D("hpairCodesQ4l","Quadleptons; minPairCode; compPairCode",2,0.5,4.5,2,0.5,4.5);
TH2D* hpairCodesF4l = new TH2D("hpairCodesF4l","Quadleptons; minPairCode; compPairCode",2,-0.5,1.5,2,-0.5,1.5);
TH2D* hpairCodesL4l = new TH2D("hpairCodesL4l","Quadleptons; minPairCode; compPairCode",3,-0.5,2.5,3,-0.5,2.5);
TH2D* hpairCodesC4l = new TH2D("hpairCodesC4l","Quadleptons; minPairCode; compPairCode",9,0.5,9.5,9,0.5,9.5);
TH2D* hpairCodesCQ4l = new TH2D("hpairCodesCQ4l","Quadleptons; minPairCode; compPairCode",3,0.5,9.5,3,0.5,9.5);

TH2D* hCodes3l = new TH2D("hCodes3l","Trileptons; minPairCode; compLepCode",9,0.5,9.5,4,0.5,4.5);

TH1D* hPTISR3L = new TH1D("hPTISR3L","Trileptons; PTISR_LEP [GeV]; Events per bin scaled to 400 inv fb",100,0.0,1000.0);
TH1D* hPTISR4L = new TH1D("hPTISR4L","Quadleptons; PTISR_LEP [GeV]; Events per bin scaled to 400 inv fb",100,0.0,1000.0);

TH1D* hDiProcessEE = new TH1D("hDiProcessEE", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hDiProcessEM = new TH1D("hDiProcessEM", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hDiProcessMM = new TH1D("hDiProcessMM", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);

TH1D* hDiProcessEEOS = new TH1D("hDiProcessEEOS", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hDiProcessEMOS = new TH1D("hDiProcessEMOS", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hDiProcessMMOS = new TH1D("hDiProcessMMOS", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);

TH1D* hDiProcessEESS = new TH1D("hDiProcessEESS", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hDiProcessEMSS = new TH1D("hDiProcessEMSS", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hDiProcessMMSS = new TH1D("hDiProcessMMSS", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);

TH1D* hDiProcessEESST = new TH1D("hDiProcessEESST", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hDiProcessEMSST = new TH1D("hDiProcessEMSST", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);
TH1D* hDiProcessMMSST = new TH1D("hDiProcessMMSST", "Dileptons; Signal production process; Events per bin scaled to 400 inv fb", 8, 0.5, 8.5);

TH1D* hgenHT = new TH1D("hgenHT","Dileptons; Gen HT [GeV]; Events per bin scaled to 400 fb^{-1}",202,-1.0,201.0);

TH1D* hMinm3lOverm4l = new TH1D("hMinm3lOverm4l","Quadleptons; min(m3l)/m4l; Events per bin scaled to 400 fb^{-1}",50,0.0,1.0);
TH1D* hMin2lMassOverm4l  = new TH1D("hMin2lMassOverm4l","Quadleptons; min(#sqrt{m_{ij}^{2} + m_{kl}^{2}} )/m_{4l}; Events per bin scaled to 400 fb^{-1}",50,0.0,1.0);
TH2D* h4lMassRatios  = new TH2D("h4lMassRatios","Quadleptons; 2l pairing mass ratio; 3l mass ratio",10,0.0,1.0,10,0.0,1.0);
TH2D* h4lMassRatiosFine  = new TH2D("h4lMassRatiosFine","Quadleptons; 2l pairing mass ratio; 3l mass ratio",20,0.0,1.0,20,0.0,1.0);

TH1D* hAMinm3l = new TH1D("hAMinm3l","Quadleptons; min(m3l); Events per bin scaled to 400 fb^{-1}",50,0.0,200.0);
TH1D* hAMin2lMass = new TH1D("hAMin2lMass","Quadleptons; min(#sqrt{m_{ij}^{2} + m_{kl}^{2}} ); Events per bin scaled to 400 fb^{-1}",50,0.0,200.0);
TH2D* hA4lMasses  = new TH2D("hA4lMasses","Quadleptons; 2l pairing mass; 3l min mass",20,0.0,200.0,20,0.0,200.0);
TH2D* hA4lMassesFine  = new TH2D("hA4lMassesFine","Quadleptons; 2l pairing mass ; 3l min mass",40,0.0,200.0,40,0.0,200.0);

TH1D* h4lFactor = new TH1D("h4lFactor","Quadleptons; (2+2)/( (2+2) + (3+1) ); Events per bin scaled to 400 fb^{-1}",50,0.0,1.0);

