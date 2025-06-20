# This does 1M events in 5 minutes on my laptop.
import random
import ROOT
import math
import analyzeArgs
import itertools
from FourVecDataClass import FourVec

nevsToRead, which, target, lumi, prefix, elPtMin, muPtMin = analyzeArgs.getArguments(None)
analyzeArgs.showArgs(nevsToRead, which, target, lumi, prefix, elPtMin, muPtMin)

# Calculate event weight for histograms
wt = target/lumi
print('Using event weight of ',wt)

#FIXME
#prefix="WF-V1/"
lhefile=prefix+"/"+which+".lhe"
print('Opening LHE file',lhefile)
f = open(lhefile, "r")
# Initialize output histogram file
rootfile="histos_"+which+".root"
fout = ROOT.TFile(rootfile, "recreate");

hparticles = ROOT.TH1D("hparticles","; Number of particles",20,-0.5,19.5)
hcosth = ROOT.TH1D("hcosth","; |costh|",1000,0.0,1.0)
hsinth = ROOT.TH1D("hsinth","; sinth",1000,0.0,1.0)
hmass = ROOT.TH1D("hpmass",";Partonic Center of Mass Energy [GeV]",400,0.0,2000.0)
hphmass = ROOT.TH1D("hfmass",";Final-State Invariant Mass [GeV]",400,0.0,2000.0)

hZmass = ROOT.TH1D("hZmass","; Di-electron mass [GeV]",500,0.0,250.0)

h2lOSeemass = ROOT.TH1D("h2lOSeemass","; 2l OS Di-electron mass [GeV]",125,0.0,250.0)
h2lOSemmass = ROOT.TH1D("h2lOSemmass","; 2l OS Electron-muon mass [GeV]",125,0.0,250.0)
h2lOSmmmass = ROOT.TH1D("h2lOSmmmass","; 2l OS Di-muon mass [GeV]",125,0.0,250.0)
h2lSSeemass = ROOT.TH1D("h2lSSeemass","; 2l SS Di-electron mass [GeV]",125,0.0,250.0)
h2lSSemmass = ROOT.TH1D("h2lSSemmass","; 2l SS Electron-muon mass [GeV]",125,0.0,250.0)
h2lSSmmmass = ROOT.TH1D("h2lSSmmmass","; 2l SS Di-muon mass [GeV]",125,0.0,250.0)

hOSeemass = ROOT.TH1D("hOSeemass","; 3l OS Di-electron mass [GeV]",125,0.0,250.0)
hOSemmass = ROOT.TH1D("hOSemmass","; 3l OS Electron-muon mass [GeV]",125,0.0,250.0)
hOSmmmass = ROOT.TH1D("hOSmmmass","; 3l OS Di-muon mass [GeV]",125,0.0,250.0)
hSSeemass = ROOT.TH1D("hSSeemass","; 3l SS Di-electron mass [GeV]",125,0.0,250.0)
hSSemmass = ROOT.TH1D("hSSemmass","; 3l SS Electron-muon mass [GeV]",125,0.0,250.0)
hSSmmmass = ROOT.TH1D("hSSmmmass","; 3l SS Di-muon mass [GeV]",125,0.0,250.0)
hPosMumass = ROOT.TH1D("hPosMumass","; Positron-Muon mass [GeV]",500,0.0,250.0)
hEleMumass = ROOT.TH1D("hEleMumass","; Electron-Muon mass [GeV]",500,0.0,250.0)
hWmass = ROOT.TH1D("hWmass","; Muon-Neutrino mass [GeV]",500,0.0,250.0)
h4lCharge = ROOT.TH1D("h4lCharge","; 4l charge",9,-4.5,4.5)

hTriLeptonMass = ROOT.TH1D("hTriLeptonMass","; Trilepton mass [GeV]",250,0.0,500.0)
hTriLeptonMassOnZ = ROOT.TH1D("hTriLeptonMassOnZ","; OnZ Trilepton mass [GeV]",250,0.0,500.0)
hTriLeptonMassOffZ = ROOT.TH1D("hTriLeptonMassOffZ","; OffZ Trilepton mass [GeV]",250,0.0,500.0)

hTriLeptonMass0 = ROOT.TH1D("hTriLeptonMass0","; Trilepton mass [GeV] in 0 OSSF events",50,0.0,500.0)
hTriLeptonMassQ3 = ROOT.TH1D("hTriLeptonMassQ3","; |Q|=3 Trilepton mass [GeV]",50,0.0,500.0)
hTriLeptonMass0Q1 = ROOT.TH1D("hTriLeptonMass0Q1","; Trilepton mass [GeV] in |Q|=1 0 OSSF events",50,0.0,500.0)

hQuadLeptonMass57 = ROOT.TH1D("hQuadLeptonMass57","; Four-lepton mass [GeV] in 3e-1mu and 1e-3mu events",75,0.0,750.0)
hQuadLeptonMass57OffZ = ROOT.TH1D("hQuadLeptonMass57OffZ","; OffZ Four-lepton mass [GeV] in 3e-1mu and 1e-3mu events",75,0.0,750.0)
hQuadLeptonMass57OnZ = ROOT.TH1D("hQuadLeptonMass57OnZ","; OnZ Four-lepton mass [GeV] in 3e-1mu and 1e-3mu events",75,0.0,750.0)

hQuadLeptonMass = ROOT.TH1D("hQuadLeptonMass","; Four-lepton mass [GeV]",250,0.0,500.0)
hQuadLeptonMassOffZ = ROOT.TH1D("hQuadLeptonMassOffZ","; OffZ Four-lepton mass [GeV]",250,0.0,500.0)
hQuadLeptonMassOnZ = ROOT.TH1D("hQuadLeptonMassOnZ","; OnZ Four-lepton mass [GeV]",250,0.0,500.0)

hQuadLeptonMassQ2 = ROOT.TH1D("hQuadLeptonMassQ2","; |Q|=2 Four-lepton mass [GeV]",50,0.0,500.0)
hQuadLeptonMassQ2OnZ = ROOT.TH1D("hQuadLeptonMassQ2OnZ","; |Q|=2 OnZ Four-lepton mass [GeV]",50,0.0,500.0)
hQuadLeptonMassQ2OffZ = ROOT.TH1D("hQuadLeptonMassQ2OffZ","; |Q|=2 OffZ Four-lepton mass [GeV]",50,0.0,500.0)

hQuadLeptonMass57Q2 = ROOT.TH1D("hQuadLeptonMass57Q2","; 57 |Q|=2 Four-lepton mass [GeV]",50,0.0,500.0)
hQuadLeptonMass57Q2OnZ = ROOT.TH1D("hQuadLeptonMass57Q2OnZ","; 57 |Q|=2 OnZ Four-lepton mass [GeV]",50,0.0,500.0)
hQuadLeptonMass57Q2OffZ = ROOT.TH1D("hQuadLeptonMass57Q2OffZ","; 57 |Q|=2 OffZ Four-lepton mass [GeV]",50,0.0,500.0)

hnupT = ROOT.TH1D("hnupT","; Neutrino pT [GeV]",100,0.0,200.0)
hZpT = ROOT.TH1D("hZpT","; Di-electron pT [GeV]",100,0.0,200.0)
hWmT = ROOT.TH1D("hWmT","; Muon-neutrino transverse mass [GeV]",200,0.0,200.0)
hmT = ROOT.TH1D("hmT","; Transverse mass [GeV]",125,0.0,500.0)
hmTNew1 = ROOT.TH1D("hmTNew1","nOSSF = 1; Transverse mass [GeV]",80,0.0,400.0)
hmTNew2 = ROOT.TH1D("hmTNew2","nOSSF = 2; Minimum transverse mass [GeV]",80,0.0,400.0)
hmI = ROOT.TH1D("hmI","; Mass of the invisible system (Eq 3)",375,0.0,750.0)

heleEta = ROOT.TH1D("heleEta","; Electron Eta",150,-7.5,7.5)
hposEta = ROOT.TH1D("hposEta","; Positron Eta",150,-7.5,7.5)
hmuEta  = ROOT.TH1D("hmuEta","; Muon Eta",150,-7.5,7.5)

heleRap = ROOT.TH1D("heleRap","; Electron Rapidity",150,-7.5,7.5)
hposRap = ROOT.TH1D("hposRap","; Positron Rapidity",150,-7.5,7.5)
hmuRap  = ROOT.TH1D("hmuRap","; Muon Rapidity",150,-7.5,7.5)

helepT = ROOT.TH1D("helepT","; Electron pT",100,0.0,100.0)
hpospT = ROOT.TH1D("hpospT","; Positron pT",100,0.0,100.0)
hmupT  = ROOT.TH1D("hmupT","; Muon pT",100,0.0,100.0)

hZRap  = ROOT.TH1D("hZRap","; Z Rapidity",150,-7.5,7.5)
hWRap  = ROOT.TH1D("hWRap","; W Rapidity",150,-7.5,7.5)
hTriLRap  = ROOT.TH1D("hTriLRap","; Trilepton Rapidity",150,-7.5,7.5)
hMETTwo  = ROOT.TH1D("hMETTwo","; Dilepton MET [GeV]",200,0.0,500.0)
hMETThree  = ROOT.TH1D("hMETThree","; Trilepton MET [GeV]",200,0.0,500.0)
hMETFour  = ROOT.TH1D("hMETFour","; Quadlepton MET [GeV]",200,0.0,500.0)
hmTFour  = ROOT.TH1D("hmTFour","; Quadlepton mT [GeV]",200,0.0,500.0)

hDRposele = ROOT.TH1D("hDRposele","; #Delta R(e+, e-)", 100, 0.0, 10.0)
hDRelemu = ROOT.TH1D("hDRelemu","; #Delta R(e-, mu+)", 100,  0.0, 10.0)
hDRposmu = ROOT.TH1D("hDRposmu","; #Delta R(e+, mu+)", 100, 0.0, 10.0)

hJetsPT = ROOT.TH1D("hJetsPT","; Jet system pT [GeV] ",200,0.0,2000.0)

hdisc = ROOT.TH1D("hdisc","; Quadratic discriminant",2,-1.0e6,1.0e6)
hnOSSF = ROOT.TH1D("hnOSSF","; nOSSF lepton pairs in trilepton event",3,-0.5,2.5)
# Add ASCII bin labels ....
h3lCharge = ROOT.TH1D("h3lCharge","; Trilepton event charge",9,-5.5,3.5)
hnelmu = ROOT.TH2D("hnelmu","; Number of electrons; Number of muons",4,-0.5,3.5,4,-0.5,3.5) 
hnleptons = ROOT.TH1D("hnleptons","; Number of accepted leptons",7,-0.5,6.5)

h2LPtOne = ROOT.TH1D("h2LPtOne","; Highest lepton pT [GeV]",200,0.0,200.0)
h2LPtTwo = ROOT.TH1D("h2LPtTwo","; Lowest lepton pT [GeV]",200,0.0,200.0)

hPtOne = ROOT.TH1D("hPtOne","; Highest lepton pT [GeV]",200,0.0,200.0)
hPtTwo = ROOT.TH1D("hPtTwo","; Middle lepton pT [GeV]",200,0.0,200.0)
hPtThree = ROOT.TH1D("hPtThree","; Lowest lepton pT [GeV]",200,0.0,200.0)
hPtFour = ROOT.TH1D("hPtFour","; Lowest lepton pT [GeV in 4-lepton events]",100,0.0,100.0)

h2Code = ROOT.TH1D("h2Code","; Two-lepton signed flavor code (OS= +ve, SS= -ve)",9,-4.5,4.5)
h3Code = ROOT.TH1D("h3Code","; Three-lepton flavor code ",4,2.5,6.5)
h4Code = ROOT.TH1D("h4Code","; Four-lepton flavor code ",5,3.5,8.5)
h5Code = ROOT.TH1D("h5Code","; Five-lepton flavor code ",6,4.5,10.5)
h6Code = ROOT.TH1D("h6Code","; Six-lepton flavor code ",7,5.5,12.5)

hSROSon = ROOT.TH1D("hSROSon","; SROS-on category",12,0.5,12.5)
hSROSoff = ROOT.TH1D("hSROSoff","; SROS-off category",12,0.5,12.5)
hSRSS = ROOT.TH1D("hSRSS","; SRSS category",4,0.5,4.5)

hmZvsyZ = ROOT.TH2D("hmZvsyZ","; Di-electron mass [GeV] vs Z |rapidity|",50,0.0,7.5,100,0.0,250.0)

hwhich = ROOT.TH2D("hwhich","; Process ID; NParticles",12,0.5,12.5,12,5.5,17.5)
hwhich2 = ROOT.TH2D("hwhich2","; Process ID; N Accepted Leptons",12,0.5,12.5,7,-0.5,6.5)
hprocess= ROOT.TH1D("hprocess","; Process ID; ",12,0.5,12.5)

hOnZ1 = ROOT.TH1D("hOnZ1","; (mll - mZ); ",100,-100.0,100.0)
hOnZ2 = ROOT.TH2D("hOnZ2","; (mll - mZ)_0; (mll - mz)_1",20,-20.0,20.0,20,-20.0,20.0)

h2lOnZ = ROOT.TH1D("h2lOnZ","; (mll - mZ); ",150,-100.0,200.0)
h3lOnZ = ROOT.TH1D("h3lOnZ","; (mll - mZ); ",150,-100.0,200.0)
h4lOnZ = ROOT.TH1D("h4lOnZ","; (mll - mZ); ",150,-100.0,200.0)
h5lOnZ = ROOT.TH1D("h5lOnZ","; (mll - mZ); ",150,-100.0,200.0)
h6lOnZ = ROOT.TH1D("h6lOnZ","; (mll - mZ); ",150,-100.0,200.0)

# Define alphanumeric bin labels for plots with ATLAS 2503.13135 categories
SRSS_labels = ["eee", "ee#mu", "e#mu#mu", "#mu#mu#mu"]
SROSon_labels = ["a-eee", "a-ee#mu", "a-e#mu#mu", "a-#mu#mu#mu", "b-eee", "b-ee#mu", "b-e#mu#mu", "b-#mu#mu#mu", "c-eee", "c-ee#mu", "c-e#mu#mu", "c-#mu#mu#mu"]
SROSoff_labels = ["a-eee", "a-ee#mu", "a-e#mu#mu", "a-#mu#mu#mu", "b-eee", "b-ee#mu", "b-e#mu#mu", "b-#mu#mu#mu", "c-eee", "c-ee#mu", "c-e#mu#mu", "c-#mu#mu#mu"]
for i in range(1, len(SRSS_labels)+1):
    hSRSS.GetXaxis().SetBinLabel(i, SRSS_labels[i-1])
    hSRSS.GetXaxis().SetLabelSize(0.06)  # default is 0.03 apparently
    
for i in range(1, len(SROSon_labels)+1):
    hSROSon.GetXaxis().SetBinLabel(i, SROSon_labels[i-1])
    hSROSoff.GetXaxis().SetBinLabel(i, SROSoff_labels[i-1]) 
hSROSon.GetXaxis().LabelsOption("h")
hSROSoff.GetXaxis().LabelsOption("h")


# mT2 stuff
ptmax=550 #maximum found pt
sd4=24.265*4 #4 times the standard deviation of the transverse momentum
mma=0.0
#mmb=90
heading="MT2"
xaxis="MT2 [GeV]"
title=heading+";"+xaxis+"; Weighted Event Count"
hmT2   = ROOT.TH1D("hmT2",title,125,0.0,250.0)
hmT2P1 = ROOT.TH1D("hmT2P1",title,125,0.0,250.0)
hmT2P2 = ROOT.TH1D("hmT2P2",title,125,0.0,250.0)
hmT2P3 = ROOT.TH1D("hmT2P3",title,125,0.0,250.0)
hmT2P4 = ROOT.TH1D("hmT2P4",title,125,0.0,250.0)
hmT2P5678 = ROOT.TH1D("hmT2P5678",title,125,0.0,250.0)

#titles=heading+": pt= "+str(sd4)+" GeV and missing mass of "+str(mmb)+" GeV;"+xaxis+"; Weighted Count"
#hSMmt2b4= ROOT.TH1D("hWWP3mt2b4",titles,375,0.0,750.0)
hnit = ROOT.TH1D("hnit","Number of iterations",10001,-0.5,10000.5)
hcalls = ROOT.TH1D("hcalls","Number of calls",10001,-0.5,10000.5)

hbetaz = ROOT.TH1D("hbetaz","Beta z for dileptons",100,-1.0,1.0)
habetaz = ROOT.TH1D("habetaz","|Beta z| for dileptons",100,0.0,1.0)

steps=50
psteps=50
 
event = 0

eventWT = 1.0
print("eventWT",eventWT)

while True:

# Read and make 4-vectors from LHE file

    header = (f.readline()).split()
    if not header:
        break
    
    if len(header)>0 and header[0]=="<event>":
        nextline = (f.readline()).split()
        nparticles = int(nextline[0])
        processID = int(nextline[1])
        sqrts = nextline[3]
        nfinal = nparticles - 2
        hprocess.Fill(processID, wt)
        hwhich.Fill(processID, nparticles, wt)
        event += 1
        
        if nevsToRead > -1 and event > nevsToRead:
            print("Stopping event reading after",event-1,"events")
            break

        partona = (f.readline()).split()
        partonb = (f.readline()).split() 
    
        if event%100000 ==0 or event <= 1:
            print("event",event)
       
# Initial state partons     
        fpartona = FourVec( int(partona[0]), float(partona[6]), float(partona[7]), float(partona[8]), float(partona[9]) )    
        fpartonb = FourVec( int(partonb[0]), float(partonb[6]), float(partonb[7]), float(partonb[8]), float(partonb[9]) ) 
    
        Particles = []
        Jets = []
        fZnull = FourVec(0, 0.0, 0.0, 0.0, 0.0)
        fWnull = FourVec(0, 0.0, 0.0, 0.0, 0.0)
        fJetsnull = FourVec(0, 0.0, 0.0, 0.0, 0.0)
        fDiLeptonnull = FourVec(0, 0.0, 0.0, 0.0, 0.0, 0.0)
        fTriLeptonnull = FourVec(0, 0.0, 0.0, 0.0, 0.0, 0.0)
        fQuadLeptonnull = FourVec(0, 0.0, 0.0, 0.0, 0.0, 0.0)       
        fMETnull = FourVec(0, 0.0, 0.0, 0.0, 0.0, 0.0)       
        Zlist = []
        Wlist = []
        Trilist = []
        METlist = []
        Nulist = []
        Mulist = []
        leptons = []
        genleptons = []
        for i in range(nfinal):
            p = (f.readline()).split()
#           print("p ",p)
# Add requirement that the particle is a final state particle
            if int(p[1]) != 1:
                continue
            particle = FourVec( int(p[0]), float(p[6]), float(p[7]), float(p[8]), float(p[9]) )          
            Particles.append(particle)
            if abs(particle.pdgID) == 11:
                genleptons.append(particle)
                if particle.accEl(elPtMin): 
                    leptons.append(particle)
                    Zlist.append(particle)
                    Trilist.append(particle)
                    if particle.pdgID == 11:
                        heleEta.Fill(particle.eta(),wt)
                        heleRap.Fill(particle.rapidity(),wt)
                        helepT.Fill(particle.pt(),wt)                                      
                    if particle.pdgID == -11:
                        hposEta.Fill(particle.eta(),wt)
                        hposRap.Fill(particle.rapidity(),wt)
                        hpospT.Fill(particle.pt(),wt)                                         
            if abs(particle.pdgID) == 13:
                genleptons.append(particle)
                if particle.accMu(muPtMin): 
                    leptons.append(particle)
                    Mulist.append(particle)
                    Wlist.append(particle)
                    Trilist.append(particle)
                    hmuEta.Fill(particle.eta(),wt)
                    hmuRap.Fill(particle.rapidity(),wt)
                    hmupT.Fill(particle.pt(),wt)                  
#            if abs(particle.pdgID) == 14:
#                Wlist.append(particle)
            if abs(particle.pdgID) == 12 or abs(particle.pdgID) == 14 or abs(particle.pdgID) == 16:
                Nulist.append(particle)
                hnupT.Fill(particle.pt(),wt)
                METlist.append(particle)
#                particle.print()                
            if particle.pdgID == 21 or abs(particle.pdgID) < 7:
                Jets.append(particle)
            if particle.pdgID == 1000022:
                METlist.append(particle)
#                particle.print()
                
#           particle.print()
            hcosth.Fill(particle.acosth(),wt)
            hsinth.Fill( math.sin(particle.theta() ), wt )
           
# sort leptons by pT in descending order
        leptons.sort(key=lambda particle: particle.pt(), reverse=True)
#       print("leptons  after sort: ",leptons)
        hnleptons.Fill(len(leptons),wt)
        hwhich2.Fill(processID, len(leptons), wt)
        nel = 0
        nmu = 0
       
        fMET = fMETnull.addlist(101, METlist)
#        fMET.print()
        
# Make "OnZ" information for all OS lepton pairs regardless of lepton multiplicity
        dmll = []
        if len(leptons) >= 2:
            for i, j in itertools.combinations(range(len(leptons)), 2):
                if leptons[i].osdil(leptons[j]):
                    dmll.append(leptons[i].mtwo(leptons[j]) - 91.2)
        sorted_dmll = sorted(dmll, key=abs)
        
        ZCUT = 12.5
        OnZFlag = False
        if sorted_dmll:
            if sorted_dmll[0] > -ZCUT and sorted_dmll[0] < ZCUT:
                OnZFlag = True       
       
        if len(leptons) == 2:
            if sorted_dmll:
                h2lOnZ.Fill(sorted_dmll[0],wt)        
            h2LPtOne.Fill(leptons[0].pt(),wt) 
            h2LPtTwo.Fill(leptons[1].pt(),wt)
            hMETTwo.Fill(fMET.pt(),wt)
           
            code = leptons[0].lflavor() + leptons[1].lflavor()
            q1 = leptons[0].lcharge()
            q2 = leptons[1].lcharge()
            if q1!=q2:
                h2Code.Fill(code, wt)
            else:
                h2Code.Fill(-code, wt)
# Fix me - would be good if it was easier to generalize this
            if leptons[0].osdiel(leptons[1]): 
                h2lOSeemass.Fill(leptons[0].mtwo(leptons[1]),wt)
#                print (event, " OS ee mass ", leptons[0].mtwo(leptons[1]) )
            if leptons[0].oselmu(leptons[1]): h2lOSemmass.Fill(leptons[0].mtwo(leptons[1]),wt)          
            if leptons[0].osdimu(leptons[1]): h2lOSmmmass.Fill(leptons[0].mtwo(leptons[1]),wt)            
            if leptons[0].ssdiel(leptons[1]): h2lSSeemass.Fill(leptons[0].mtwo(leptons[1]),wt)
            if leptons[0].sselmu(leptons[1]): h2lSSemmass.Fill(leptons[0].mtwo(leptons[1]),wt)
            if leptons[0].ssdimu(leptons[1]): h2lSSmmmass.Fill(leptons[0].mtwo(leptons[1]),wt)
            fDiLepton = fDiLeptonnull.addlist(100, leptons)
            hbetaz.Fill(fDiLepton.betaz(),wt)
            habetaz.Fill(abs(fDiLepton.betaz()),wt)
            
#            if processID==1:
#            value, nit, calls = fMET.MT2(leptons, mma, mma, sd4)
#            hmT2.Fill(value, wt)    # Fill for all events
#            hnit.Fill(nit, wt)
#            hcalls.Fill(calls, wt)
#            if processID == 1:
#                hmT2P1.Fill(value, wt)
#            if processID == 2:
#                hmT2P2.Fill(value, wt)
#            if processID == 3:
#                hmT2P3.Fill(value, wt)
#            if processID == 4:
#                hmT2P4.Fill(value, wt)
#            if processID >= 5 and processID <= 8:
#                hmT2P5678.Fill(value, wt)                
#            print('Event ',event,'MT2 = ',value,'nit = ',nit)
#                hSP90mt2b4.Fill(fMET.MT2(leptons,mmb,sd4,steps,psteps),wt)            
       
        if len(leptons) == 3:       
            hPtOne.Fill(leptons[0].pt(),wt) 
            hPtTwo.Fill(leptons[1].pt(),wt) 
            hPtThree.Fill(leptons[2].pt(),wt) 
            nOSSF = leptons[0].ossf(leptons[1]) + leptons[1].ossf(leptons[2]) + leptons[2].ossf(leptons[0])
            hnOSSF.Fill(nOSSF,wt)
            charge3l = leptons[0].lcharge() + leptons[1].lcharge() + leptons[2].lcharge()                     
            h3lCharge.Fill(charge3l,wt)
            nel = leptons[0].eflavor() + leptons[1].eflavor() + leptons[2].eflavor() 
            nmu = leptons[0].mflavor() + leptons[1].mflavor() + leptons[2].mflavor()
            hnelmu.Fill(nel,nmu,wt)
#            if charge3l==-3 or charge3l==3:
#                print('SSS event ',event,nparticles,sqrts)
#                print('Lepton 0',leptons[0].pt(),leptons[0].pdgID, leptons[0].px, leptons[0].py, leptons[0].pz) 
#                print('Lepton 1',leptons[1].pt(),leptons[1].pdgID, leptons[1].px, leptons[1].py, leptons[1].pz)
#                print('Lepton 2',leptons[2].pt(),leptons[2].pdgID, leptons[2].px, leptons[2].py, leptons[2].pz)
           
# Code for mTmin lepton index ID (a la ATLAS).
            Wleptons = []             # Keep list of indices of left-over l_W leptons in OSSF cases
            mTlW = []                 # And calculate transverse mass
            all_indices = {0, 1, 2}
            for i, j in itertools.combinations(all_indices, 2):
                if leptons[i].osdil(leptons[j]):
                    # Convert the complementary set to a list and get the first element that is the left-over lepton index
                    k = list(all_indices - {i, j})[0]                    
                    Wleptons.append(k)
                    mTlW.append(leptons[k].mtsimple(fMET))
            mTlW.sort()                            # sort in ascending order

            if sorted_dmll:
                 h3lOnZ.Fill(sorted_dmll[0],wt)
            if nOSSF==1:
                 hmTNew1.Fill(mTlW[0],wt)
            if nOSSF==2:
                 hmTNew2.Fill(mTlW[0],wt)                 
           
# Fix me - would be good if it was easier to generalize this
            if leptons[0].osdiel(leptons[1]): hOSeemass.Fill(leptons[0].mtwo(leptons[1]),wt)
            if leptons[1].osdiel(leptons[2]): hOSeemass.Fill(leptons[1].mtwo(leptons[2]),wt)
            if leptons[2].osdiel(leptons[0]): hOSeemass.Fill(leptons[2].mtwo(leptons[0]),wt)          
            if leptons[0].oselmu(leptons[1]): hOSemmass.Fill(leptons[0].mtwo(leptons[1]),wt)
            if leptons[1].oselmu(leptons[2]): hOSemmass.Fill(leptons[1].mtwo(leptons[2]),wt)
            if leptons[2].oselmu(leptons[0]): hOSemmass.Fill(leptons[2].mtwo(leptons[0]),wt)            
            if leptons[0].osdimu(leptons[1]): hOSmmmass.Fill(leptons[0].mtwo(leptons[1]),wt)
            if leptons[1].osdimu(leptons[2]): hOSmmmass.Fill(leptons[1].mtwo(leptons[2]),wt)
            if leptons[2].osdimu(leptons[0]): hOSmmmass.Fill(leptons[2].mtwo(leptons[0]),wt)
            
            if leptons[0].ssdiel(leptons[1]): hSSeemass.Fill(leptons[0].mtwo(leptons[1]),wt)
            if leptons[1].ssdiel(leptons[2]): hSSeemass.Fill(leptons[1].mtwo(leptons[2]),wt)
            if leptons[2].ssdiel(leptons[0]): hSSeemass.Fill(leptons[2].mtwo(leptons[0]),wt)
            if leptons[0].sselmu(leptons[1]): hSSemmass.Fill(leptons[0].mtwo(leptons[1]),wt)
            if leptons[1].sselmu(leptons[2]): hSSemmass.Fill(leptons[1].mtwo(leptons[2]),wt)
            if leptons[2].sselmu(leptons[0]): hSSemmass.Fill(leptons[2].mtwo(leptons[0]),wt)
            if leptons[0].ssdimu(leptons[1]): hSSmmmass.Fill(leptons[0].mtwo(leptons[1]),wt)
            if leptons[1].ssdimu(leptons[2]): hSSmmmass.Fill(leptons[1].mtwo(leptons[2]),wt)
            if leptons[2].ssdimu(leptons[0]): hSSmmmass.Fill(leptons[2].mtwo(leptons[0]),wt) 
           
            fTriLepton = fTriLeptonnull.addlist(100, leptons)
            hTriLeptonMass.Fill(fTriLepton.mass(),wt)
#            print('Event ',event,' METThree = ',fMET.pt())
            hMETThree.Fill(fMET.pt(),wt)           
            hmT.Fill(fTriLepton.mtp(fMET),wt)
            hTriLRap.Fill(fTriLepton.rapidity(),wt)
            # 3lcode here goes from 1 to 4
            l3code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() - 2
            h3Code.Fill(leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor(), wt)
            
            if OnZFlag is True:
                hTriLeptonMassOnZ.Fill(fTriLepton.mass(),wt)
            else:
                hTriLeptonMassOffZ.Fill(fTriLepton.mass(),wt)  
                
            if charge3l == -3 or charge3l == 3:
                hTriLeptonMassQ3.Fill(fTriLepton.mass(),wt)
                    
            if (charge3l == -1 or charge3l == 1) and nOSSF == 0:
                hTriLeptonMass0Q1.Fill(fTriLepton.mass(),wt)                                                    
            
            ATLAS_presel = True
            if leptons[0].pt() < 28.0:
                 ATLAS_presel = False
            if leptons[1].pt() < 20.0:
                 ATLAS_presel = False                 
            
            if ATLAS_presel and (charge3l==-3 or charge3l==3) and fMET.pt() > 50.0:
                 hSRSS.Fill(l3code, wt)            
            
            if nOSSF==0:
                hTriLeptonMass0.Fill(fTriLepton.mass(),wt)         
            
            nOnZ = 0
            
            if nOSSF==1:
                hOnZ1.Fill(sorted_dmll[0],wt)
                if abs(sorted_dmll[0]) < 10.0:
                    nOnZ = 1
            if nOSSF==2:
                m1 = sorted_dmll[0]
                m2 = sorted_dmll[1]
                hOnZ2.Fill(sorted_dmll[0],sorted_dmll[1],wt)
                if abs(m1) < 10.0:
                    nOnZ += 1
                if abs(m2) < 10.0:
                    nOnZ += 1
                    
            ATLAS_SROSon  = False
            ATLAS_SROSoff = False
            if nOSSF >=1:
                if nOnZ >=1:
                    ATLAS_SROSon = True
                else:
                    ATLAS_SROSoff = True
                    
            m3l = fTriLepton.mass()
            aRegion = False
            bRegion = False
            cRegion = False
            if m3l>30.0 and m3l<200.0: 
                aRegion = True
            if m3l>200.0 and m3l<400.0: 
                bRegion = True
            if m3l>400.0: 
                cRegion = True                                     
            
            if ATLAS_presel and ATLAS_SROSon and fMET.pt() > 150.0 and mTlW[0] > 125.0:
                if aRegion: hSROSon.Fill(l3code, wt)
                if bRegion: hSROSon.Fill(l3code+4, wt)
                if cRegion: hSROSon.Fill(l3code+8, wt)                                
            if ATLAS_presel and ATLAS_SROSoff and fMET.pt() > 150.0 and mTlW[0] > 125.0:
                if aRegion: hSROSoff.Fill(l3code, wt)
                if bRegion: hSROSoff.Fill(l3code+4, wt)
                if cRegion: hSROSoff.Fill(l3code+8, wt)                            
                               
        if len(leptons) == 4:
            if sorted_dmll:
                 h4lOnZ.Fill(sorted_dmll[0],wt)        
            hPtFour.Fill(leptons[3].pt(), wt)
            fQuadLepton = fQuadLeptonnull.addlist(102, leptons)
          
            hMETFour.Fill(fMET.pt(), wt)
            hmTFour.Fill(fQuadLepton.mtp(fMET),wt)
            code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor()
            h4Code.Fill(code, wt)
            charge4l = leptons[0].lcharge() + leptons[1].lcharge() + leptons[2].lcharge() + leptons[3].lcharge()
            h4lCharge.Fill(charge4l,wt)
            
            hQuadLeptonMass.Fill(fQuadLepton.mass(),wt)
            if OnZFlag is True:
                hQuadLeptonMassOnZ.Fill(fQuadLepton.mass(),wt)
            else:
                hQuadLeptonMassOffZ.Fill(fQuadLepton.mass(),wt)            
                                  
            if code==5 or code==7:
                hQuadLeptonMass57.Fill(fQuadLepton.mass(),wt)
                if OnZFlag is True:
                    hQuadLeptonMass57OnZ.Fill(fQuadLepton.mass(),wt)
                else:
                    hQuadLeptonMass57OffZ.Fill(fQuadLepton.mass(),wt) 
                    
            if charge4l == -2 or charge4l == 2:
                hQuadLeptonMassQ2.Fill(fQuadLepton.mass(),wt)
                if OnZFlag is True:
                    hQuadLeptonMassQ2OnZ.Fill(fQuadLepton.mass(),wt)
                else:
                    hQuadLeptonMassQ2OffZ.Fill(fQuadLepton.mass(),wt) 
                    
            if (charge4l == -2 or charge4l == 2) and (code == 5 or code ==7):
                hQuadLeptonMass57Q2.Fill(fQuadLepton.mass(),wt)
                if OnZFlag is True:
                    hQuadLeptonMass57Q2OnZ.Fill(fQuadLepton.mass(),wt)
                else:
                    hQuadLeptonMass57Q2OffZ.Fill(fQuadLepton.mass(),wt)                                 
            
        if len(leptons) == 5:
            if sorted_dmll:
                 h5lOnZ.Fill(sorted_dmll[0],wt)        
            code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor() + leptons[4].lflavor()
            h5Code.Fill(code,wt)
            
        if len(leptons) == 6:
            if sorted_dmll:
                 h6lOnZ.Fill(sorted_dmll[0],wt)            
            code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor() + leptons[4].lflavor() + leptons[5].lflavor()
            h6Code.Fill(code,wt)                        
                      
# Automate the process of forming a new composite 4-vector
#        fpartons = fpartona.add(1000, fpartonb)
#        hmass.Fill(fpartons.mass(),wt)
            
# Do the same for a list of particles like Photons
#        fnull = FourVec(0, 0.0, 0.0, 0.0, 0.0)
#        fparticles = fnull.addlist(2222, Particles)
#        hphmass.Fill(fparticles.mass(),wt)
#        hparticles.Fill(nfinal,wt)         
 
fout.Write()
f.close()
