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

TH1D *hmll3 = new TH1D("hmll3","Trilepton Events; mll [GeV]; Dileptons per bin",400,0.0,400.0);

TH1D *hMT2LN0 = new TH1D("hMT2LN0", "M(IA), M(IB) = 0;  MT2 (GeV); Events per bin", 800, 0.0, 800.0);
TH1D *hMT2LN0SMALL = new TH1D("hMT2LN0SMALL", "M(IA), M(IB) = 0;  MT2 (GeV); Events per bin", 1000, 0.0, 2.0);

TH1D* habetaz = new TH1D("habetaz","|Beta z| for dileptons; |#beta_{z}|",50,0.0,1.0);

TH1D *hm3l = new TH1D("hm3l","Trilepton Mass; mlll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hpt3l = new TH1D("hpt3l","Trilepton Events; pt3l [GeV]; Events per bin",100,0.0,200.0);
TH1D *hm3lbtv = new TH1D("hm3lbtv","Trileptons with loose b-tag veto; mlll [GeV]; Events per bin",400,0.0,400.0);
TH1D *h3lcharge = new TH1D("h3lcharge","Trileptons ; Trilepton Charge; Events per bin", 4, -4.0, 4.0);
TH1D *h3lchargetight = new TH1D("h3lchargetight","Trileptons ; Trilepton Charge (Tight Charge Cut); Events per bin", 4, -4.0, 4.0);
TH1D *h3lflavor = new TH1D("h3lflavor","Trileptons; ; Events per bin scaled to 400 inv fb", 4, 2.5, 6.5);
TH1D *hminmll3l = new TH1D("hminmll3l","Trileptons ; min(mll) [GeV]; Events per bin",100,0.0,100.0);
TH1D *hmaxmll3l = new TH1D("hmaxmll3l","Trileptons ; max(mll) [GeV]; Events per bin",80,0.0,200.0);
TH1D *h3lMET = new TH1D("h3lMET","Trileptons; MET [GeV]; Events per bin",80,0.0,200.0);
TH1D *h3lHTid = new TH1D("h3lHTid","Trileptons; HT [GeV]; Events per bin",80,0.0,200.0);
TH1D *h3lHTnoid = new TH1D("h3lHTnoid","Trileptons; HT [GeV]; Events per bin",80,0.0,200.0);
TH1D *h3lnjets = new TH1D("h3lnjets","Trileptons; N jets; Events per bin",12,-0.5,11.5);

TH1D *hm3lF3 = new TH1D("hm3lF3","Trilepton Mass (eee); mlll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm3lF4 = new TH1D("hm3lF4","Trilepton Mass (ee#mu); mlll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm3lF5 = new TH1D("hm3lF5","Trilepton Mass (e#mu#mu); mlll [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm3lF6 = new TH1D("hm3lF6","Trilepton Mass (#mu#mu#mu); mlll [GeV]; Events per bin",400,0.0,400.0);

TH1D *hm4l = new TH1D("hm4l","Four leptons; Mass (4l) [GeV]; Events per bin",400,0.0,400.0);
TH1D *hm4lbtv = new TH1D("hm4lbtv","Four leptons with loose b-tag veto; Mass (4l) [GeV]; Events per bin",400,0.0,400.0);
TH1D *h4lcharge = new TH1D("h4lcharge","Four leptons ; Four-lepton Charge; Events per bin", 5, -5.0, 5.0);
TH1D *h4lchargetight = new TH1D("h4lchargetight","Four leptons ; Four-lepton Charge (Tight Charge Cut); Events per bin", 5, -5.0, 5.0);
TH1D *h4lflavor = new TH1D("h4lflavor","Four leptons; Four-lepton Flavor; Events per bin scaled to 400 inv fb", 5, 3.5, 8.5);

TH1D* hmT = new TH1D("hmT", "Trileptons;  Transverse mass [GeV]; Leptons per bin", 200, 0.0, 400.0);
TH1D* hmT1 = new TH1D("hmT1", "Trileptons; L1 transverse mass [GeV]; Events per bin", 200, 0.0, 400.0);
TH1D* hmT2 = new TH1D("hmT2", "Trileptons; L2 transverse mass [GeV]; Events per bin", 200, 0.0, 400.0);
TH1D* hmT3 = new TH1D("hmT3", "Trileptons; L3 transverse mass [GeV]; Events per bin", 200, 0.0, 400.0);
TH1D* hmT3l = new TH1D("hmT3l", "Trileptons; L123 transverse mass [GeV]; Events per bin", 200, 0.0, 400.0);
TH1D* h3lmaxmT = new TH1D("h3lmaxmT", "Trileptons; Max. single lepton mT [GeV]; Events per bin", 80, 0.0, 400.0);

TH1D* htrisel = new TH1D("htrisel","Trileptons; Selection word; Events per bin scaled to 400 inv fb",128,-0.5,127.5);
TH1D* hmaxSIP3D = new TH1D("hmaxSIP3D","Trileptons; max(SIP3D); Events per bin scaled to 400 inv fb",80, 0.0 ,8.0);
TH1D* hmaxSIP3DSel = new TH1D("hmaxSIP3DSel","Trileptons; max(SIP3D) Sel; Events per bin scaled to 400 inv fb",80, 0.0 ,8.0);
TH1D* hmaxSIP3DFail = new TH1D("hmaxSIP3DFail","Trileptons; max(SIP3D) Fail; Events per bin scaled to 400 inv fb",80, 0.0 ,8.0);
TH1D* hPtOne = new TH1D("hPtOne","Trileptons; PtOne [GeV]; Events per bin scaled to 400 inv fb",40, 0.0, 100.0);
TH1D* hPtTwo = new TH1D("hPtTwo","Trileptons; PtTwo [GeV]; Events per bin scaled to 400 inv fb",40, 0.0, 100.0);
TH1D* hPtThree = new TH1D("hPtThree","Trileptons; PtThree [GeV]; Events per bin scaled to 400 inv fb",40, 0.0, 100.0);
TH1D* hPtFourVeto = new TH1D("hPtFourVeto","Trileptons; PtFourVeto [GeV]; Events per bin scaled to 400 inv fb",55, -5.0, 55.0);

TH1D* hCutFlow = new TH1D("hCutFlow", "Trileptons; Cut Flow; Events per bin scaled to 400 inv fb", 9, -1.5, 7.5);
TH1D* hDiCutFlow = new TH1D("hDiCutFlow", "Dileptons; Cut Flow; Events per bin scaled to 400 inv fb", 8, -1.5, 6.5);

