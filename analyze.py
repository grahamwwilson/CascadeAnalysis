# This does 1M events in 5 minutes on my laptop.
import random
import ROOT
import math
import analyzeArgs
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
hOSeemass = ROOT.TH1D("hOSeemass","; OS Di-electron mass [GeV]",125,0.0,250.0)
hOSemmass = ROOT.TH1D("hOSemmass","; OS Electron-muon mass [GeV]",125,0.0,250.0)
hOSmmmass = ROOT.TH1D("hOSmmmass","; OS Di-muon mass [GeV]",125,0.0,250.0)
hSSeemass = ROOT.TH1D("hSSeemass","; SS Di-electron mass [GeV]",125,0.0,250.0)
hSSemmass = ROOT.TH1D("hSSemmass","; SS Electron-muon mass [GeV]",125,0.0,250.0)
hSSmmmass = ROOT.TH1D("hSSmmmass","; SS Di-muon mass [GeV]",125,0.0,250.0)
hPosMumass = ROOT.TH1D("hPosMumass","; Positron-Muon mass [GeV]",500,0.0,250.0)
hEleMumass = ROOT.TH1D("hEleMumass","; Electron-Muon mass [GeV]",500,0.0,250.0)
hWmass = ROOT.TH1D("hWmass","; Muon-Neutrino mass [GeV]",500,0.0,250.0)
h4lCharge = ROOT.TH1D("h4lCharge","; 4l charge",9,-4.5,4.5)
hTriLeptonMass = ROOT.TH1D("hTriLeptonMass","; Trilepton mass [GeV]",375,0.0,750.0)
hTriLeptonMass0 = ROOT.TH1D("hTriLeptonMass0","; Trilepton mass [GeV] in 0 OSSF events",75,0.0,750.0)
hQuadLeptonMass57 = ROOT.TH1D("hQuadLeptonMass57","; Four-lepton mass [GeV] in 3e-1mu and 1e-3mu events",75,0.0,750.0)
hQuadLeptonMass = ROOT.TH1D("hQuadLeptonMass","; Four-lepton mass [GeV]",375,0.0,750.0)
#hQuadLeptonMass = ROOT.TH1D("hQuadLeptonMass","; Four-lepton mass [GeV]",3750,-0.1,749.9)
hnupT = ROOT.TH1D("hnupT","; Neutrino pT [GeV]",100,0.0,200.0)
hZpT = ROOT.TH1D("hZpT","; Di-electron pT [GeV]",100,0.0,200.0)
hWmT = ROOT.TH1D("hWmT","; Muon-neutrino transverse mass [GeV]",200,0.0,200.0)
hmT = ROOT.TH1D("hmT","; Transverse mass [GeV]",125,0.0,500.0)
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

hmZvsyZ = ROOT.TH2D("hmZvsyZ","; Di-electron mass [GeV] vs Z |rapidity|",50,0.0,7.5,100,0.0,250.0)

hwhich = ROOT.TH2D("hwhich","; Process ID; NParticles",12,0.5,12.5,12,5.5,17.5)
hwhich2 = ROOT.TH2D("hwhich2","; Process ID; N Accepted Leptons",12,0.5,12.5,7,-0.5,6.5)
hprocess= ROOT.TH1D("hprocess","; Process ID; ",12,0.5,12.5)

event = 0

eventWT = 1.0
print("eventWT ",eventWT)

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
            print("Stopping event reading after ",event-1,"events")
            break

        partona = (f.readline()).split()
        partonb = (f.readline()).split() 
    
        if event%100000 ==0:
            print("event ",event)
       
# Initial state partons     
        fpartona = FourVec( int(partona[0]), float(partona[6]), float(partona[7]), float(partona[8]), float(partona[9]) )    
        fpartonb = FourVec( int(partonb[0]), float(partonb[6]), float(partonb[7]), float(partonb[8]), float(partonb[9]) ) 
    
        Particles = []
        Jets = []
        fZnull = FourVec(0, 0.0, 0.0, 0.0, 0.0)
        fWnull = FourVec(0, 0.0, 0.0, 0.0, 0.0)
        fJetsnull = FourVec(0, 0.0, 0.0, 0.0, 0.0)
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
            if abs(particle.pdgID) == 14:
                Wlist.append(particle)
            if abs(particle.pdgID) == 12 or abs(particle.pdgID) == 14 or abs(particle.pdgID) == 16:
                Nulist.append(particle)
                hnupT.Fill(particle.pt(),wt)
                METlist.append(particle)                
            if particle.pdgID == 21 or abs(particle.pdgID) < 7:
                Jets.append(particle)
            if particle.pdgID == 1000022:
                METlist.append(particle)
                
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
       
        if len(leptons) == 2:
            h2LPtOne.Fill(leptons[0].pt(),wt) 
            h2LPtTwo.Fill(leptons[1].pt(),wt)
           
            code = leptons[0].lflavor() + leptons[1].lflavor()
            q1 = leptons[0].lcharge()
            q2 = leptons[1].lcharge()
            if q1!=q2:
                h2Code.Fill(code, wt)
            else:
                h2Code.Fill(-code, wt)
       
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
            if charge3l==-3 or charge3l==3:
                print('SSS event ',event,nparticles,sqrts)
                print('Lepton 0',leptons[0].pt(),leptons[0].pdgID, leptons[0].px, leptons[0].py, leptons[0].pz) 
                print('Lepton 1',leptons[1].pt(),leptons[1].pdgID, leptons[1].px, leptons[1].py, leptons[1].pz)
                print('Lepton 2',leptons[2].pt(),leptons[2].pdgID, leptons[2].px, leptons[2].py, leptons[2].pz)
           
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
            hMETThree.Fill(fMET.pt(),wt)           
            hmT.Fill(fTriLepton.mtp(fMET),wt)
            hTriLRap.Fill(fTriLepton.rapidity(),wt)
            h3Code.Fill(leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor(), wt)
            if nOSSF==0:
                hTriLeptonMass0.Fill(fTriLepton.mass(),wt)         
                               
        if len(leptons) == 4:
            hPtFour.Fill(leptons[3].pt(), wt)
            fQuadLepton = fQuadLeptonnull.addlist(102, leptons)
            hQuadLeptonMass.Fill(fQuadLepton.mass(),wt)           
            hMETFour.Fill(fMET.pt(), wt)
            hmTFour.Fill(fQuadLepton.mtp(fMET),wt)
            code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor()
            h4Code.Fill(code, wt)
            charge4l = leptons[0].lcharge() + leptons[1].lcharge() + leptons[2].lcharge() + leptons[3].lcharge()
            h4lCharge.Fill(charge4l,wt)
            if code==5 or code==7:
                hQuadLeptonMass57.Fill(fQuadLepton.mass(),wt)            
            
        if len(leptons) == 5:
            code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor() + leptons[4].lflavor()
            h5Code.Fill(code,wt)
            
        if len(leptons) == 6:
            code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor() + leptons[4].lflavor() + leptons[5].lflavor()
            h6Code.Fill(code,wt)                        
                      
# Automate the process of forming a new composite 4-vector
        fpartons = fpartona.add(1000, fpartonb)
        hmass.Fill(fpartons.mass(),wt)
            
# Do the same for a list of particles like Photons
        fnull = FourVec(0, 0.0, 0.0, 0.0, 0.0)
        fparticles = fnull.addlist(2222, Particles)
        hphmass.Fill(fparticles.mass(),wt)
        hparticles.Fill(nfinal,wt)         
 
fout.Write()
f.close()
