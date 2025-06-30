#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <functional>
#include <vector>
#include <cmath>
#include <algorithm>
#include "FourVec.h"
#include "CLI11.hpp"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

int Analyze(int nevsToRead, std::string which, double target, double lumi, std::string prefix, double elPtMin, double muPtMin){

// Turn-off copyright notice
    asymm_mt2_lester_bisect::disableCopyrightMessage();    

    double wt = target / lumi;
    std::cout << "Using event weight of " << wt << std::endl;

    // Build input LHE file path
    std::string lhefile = prefix + "/" + which + ".lhe";
    std::ifstream infile(lhefile);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file: " << lhefile << std::endl;
        return 1;
    }

    // Output ROOT file
    std::string rootfile = "histos_" + which + ".root";
    TFile* fout = new TFile(rootfile.c_str(), "RECREATE");

#include "MyHistos.h"   // Put all the histogram definitions in a separate include file.
    
    int event = 0;
    std::string line;
    std::cout << "eventWT 1.0" << std::endl;

// Continuing inside main() from previous part

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;
        if (word != "<event>") continue;  // Skip until event block
        
        // Read event header line (nparticles, processID, etc)
        std::getline(infile, line);
        std::istringstream headerIss(line);

        int nparticles, processID;
        double sqrts;
        // The LHE event header format: nparticles processID xsec xsec_err scale pdfid1 pdfid2 ...
        // Your Python reads nextline[0], nextline[1], nextline[3]
        // So let's parse first four fields accordingly:
        headerIss >> nparticles >> processID;
        std::string dummy; // We'll read and discard the next two fields to get sqrts (4th field)
        double xsec, xsec_err;
        headerIss >> xsec >> xsec_err;
        sqrts = xsec_err; // This is just placeholder; replace with actual sqrts if needed

        int nfinal = nparticles - 2;
        // Fill your process histograms here if needed
        hprocess->Fill(processID, wt);
        hwhich->Fill(processID, nparticles, wt);

        event++;
        if (nevsToRead > -1 && event > nevsToRead) {
            std::cout << "Stopping event reading after " << event - 1 << " events\n";
            break;
        }

        if (event%100000 ==0 || event <= 10) std::cout << "event " << event << std::endl;

        // Read the two initial partons lines
        std::string partonALine, partonBLine;
        std::getline(infile, partonALine);
        std::getline(infile, partonBLine);

        std::istringstream partonAStream(partonALine);
        std::istringstream partonBStream(partonBLine);

        int pdgIDA, statusA;
        double pxA, pyA, pzA, eA;
        partonAStream >> pdgIDA >> statusA; // skip up to status (index 1)
        for (int i = 2; i < 6; ++i) partonAStream >> dummy; // skip fields 2-5
        partonAStream >> pxA >> pyA >> pzA >> eA;

        int pdgIDB, statusB;
        double pxB, pyB, pzB, eB;
        partonBStream >> pdgIDB >> statusB;
        for (int i = 2; i < 6; ++i) partonBStream >> dummy;
        partonBStream >> pxB >> pyB >> pzB >> eB;

        FourVec fpartona(pdgIDA, pxA, pyA, pzA, eA);
        FourVec fpartonb(pdgIDB, pxB, pyB, pzB, eB);

        // Containers for particles
        std::vector<FourVec> Particles;
        std::vector<FourVec> Jets;
        std::vector<FourVec> Zlist, Wlist, Trilist, METlist, Nulist, Mulist, leptons, genleptons;

        // Null vectors for initialization (if needed)
        FourVec fZnull(0, 0.0, 0.0, 0.0, 0.0);
        FourVec fWnull(0, 0.0, 0.0, 0.0, 0.0);
        FourVec fJetsnull(0, 0.0, 0.0, 0.0, 0.0);
        FourVec fTriLeptonnull(0, 0.0, 0.0, 0.0, 0.0, 0.0);
        FourVec fQuadLeptonnull(0, 0.0, 0.0, 0.0, 0.0, 0.0);
        FourVec fMETnull(0, 0.0, 0.0, 0.0, 0.0, 0.0);

        // Now loop over final state particles
        for (int i = 0; i < nfinal; ++i) {
            std::getline(infile, line);
            std::istringstream pstream(line);

            int pdgID, status;
            double px, py, pz, e, mass;
            // LHE format line has many fields; the python code uses indices 0 for pdgID,
            // 1 for status, and 6-10 for px,py,pz,e,m:
            // indices: 0-pdgID,1-status,6-px,7-py,8-pz,9-e, 10-m
            // so read fields accordingly:
            std::vector<std::string> tokens;
            std::string token;
            while (pstream >> token) tokens.push_back(token);
            if (tokens.size() < 11) continue; // malformed line, skip

            pdgID = std::stoi(tokens[0]);
            status = std::stoi(tokens[1]);
            px = std::stod(tokens[6]);
            py = std::stod(tokens[7]);
            pz = std::stod(tokens[8]);
            // e = std::stod(tokens[9]);
            mass = std::stod(tokens[10]);

            if (status != 1) continue; // Only final state particles

            FourVec particle(pdgID, px, py, pz, mass);
            Particles.push_back(particle);

            int absId = std::abs(pdgID);

            // Categorize particles as in python
            if (absId == 11) { // electrons
                genleptons.push_back(particle);
                if (particle.accEl(elPtMin)) {
                    leptons.push_back(particle);
                    Zlist.push_back(particle);
                    Trilist.push_back(particle);
                    // Fill histograms for electrons by charge here (optional)
                    if (pdgID == 11){
                        heleEta->Fill(particle.eta(),wt);
                        heleRap->Fill(particle.rapidity(),wt);
                        helepT->Fill(particle.Pt(),wt);
                    }
                    else{                                     
                        hposEta->Fill(particle.eta(),wt);
                        hposRap->Fill(particle.rapidity(),wt);
                        hpospT->Fill(particle.Pt(),wt);
                    }
                }
            }
            else if (absId == 13) { // muons
                genleptons.push_back(particle);
                if (particle.accMu(muPtMin)) {
                    leptons.push_back(particle);
                    Mulist.push_back(particle);
                    Wlist.push_back(particle);
                    Trilist.push_back(particle);
                    // Fill histograms for muons here
                    hmuEta->Fill(particle.eta(),wt);
                    hmuRap->Fill(particle.rapidity(),wt);
                    hmupT->Fill(particle.Pt(),wt);   
                }
            }
            else if (absId == 12 || absId == 14 || absId == 16) {
                Nulist.push_back(particle);
                hnupT->Fill(particle.Pt(), wt);
                METlist.push_back(particle);
            }
            else if (pdgID == 21 || absId < 7) {
                Jets.push_back(particle);
            }
            else if (pdgID == 1000022) {
                METlist.push_back(particle);
            }

            // Fill costh and sinth histograms
            hcosth->Fill(std::abs(std::cos(particle.Theta())), wt);
            hsinth->Fill(std::sin(particle.Theta()), wt);
        }

        // Sort leptons by pT descending
        std::sort(leptons.begin(), leptons.end(),
            [](const FourVec& a, const FourVec& b) { return a.Pt() > b.Pt(); });

        // Fill histograms with number of leptons, processID, etc (expand as needed)
        hnleptons->Fill(leptons.size(), wt);
        hwhich2->Fill(processID, leptons.size(), wt);

        unsigned int nel = 0;
        unsigned int nmu = 0;

        // Continue with MET and composite calculations as in your python

// Continuing inside main() after sorting leptons

        // Combine all MET components into one vector:
        FourVec fMET(0, 0.0, 0.0, 0.0, 0.0);
        for (const auto& metp : METlist) {
//           metp.Print();
            fMET += metp;
        }

        // Combine jets into fJets vector:
        FourVec fJets(0, 0.0, 0.0, 0.0, 0.0);
        for (const auto& jet : Jets) {
            fJets += jet;
        }

        // Combine neutrinos (Nulist) into fNeu vector:
        FourVec fNeu(0, 0.0, 0.0, 0.0, 0.0);
        for (const auto& nu : Nulist) {
            fNeu += nu;
        }

        // Combine Z candidates:
        FourVec fZ(0, 0.0, 0.0, 0.0, 0.0);
        for (const auto& z : Zlist) {
            fZ += z;
        }

        // Combine W candidates:
        FourVec fW(0, 0.0, 0.0, 0.0, 0.0);
        for (const auto& w : Wlist) {
            fW += w;
        }

        // Combine leptons:
        FourVec fLeptons(0, 0.0, 0.0, 0.0, 0.0);
        for (const auto& l : leptons) {
            fLeptons += l;
        }

// Assume FourVec is already defined, with methods:
// bool osdil(const FourVec& other) const;
// double mtwo(const FourVec& other) const;

        std::vector<double> dmll;
        if (leptons.size() >= 2) {
            for (size_t i = 0; i < leptons.size(); ++i) {
                for (size_t j = i + 1; j < leptons.size(); ++j) {
                    if (leptons[i].osdil(leptons[j])) {
                         dmll.push_back(leptons[i].mtwo(leptons[j]) - 91.2);
                    }
                }
            }
        }
        std::sort(dmll.begin(), dmll.end(), [](double a, double b) {
            return std::abs(a) < std::abs(b);
        });

        const double ZCUT = 12.5;
        bool OnZFlag = false;
        if (!dmll.empty()) {
            if (dmll[0] > -ZCUT && dmll[0] < ZCUT) {
                OnZFlag = true;
            }
        }

        if (leptons.size() == 2) {
            if (!dmll.empty()) {
                h2lOnZ->Fill(dmll[0], wt);
            }
            h2LPtOne->Fill(leptons[0].Pt(), wt);
            h2LPtTwo->Fill(leptons[1].Pt(), wt);
            hMETTwo->Fill(fMET.Pt(), wt);

            int code = leptons[0].lflavor() + leptons[1].lflavor();
            int q1 = leptons[0].lcharge();
            int q2 = leptons[1].lcharge();
            if (q1 != q2) {
                h2Code->Fill(code, wt);
            } else {
                h2Code->Fill(-code, wt);
            }
            if (leptons[0].osdiel(leptons[1])) {
                h2lOSeemass->Fill(leptons[0].mtwo(leptons[1]), wt);
        //        std::cout << event << " osee mass " << leptons[0].mtwo(leptons[1]) << std::endl;
            }
            if (leptons[0].oselmu(leptons[1])) h2lOSemmass->Fill(leptons[0].mtwo(leptons[1]), wt);
            if (leptons[0].osdimu(leptons[1])) h2lOSmmmass->Fill(leptons[0].mtwo(leptons[1]), wt);
            if (leptons[0].ssdiel(leptons[1])) h2lSSeemass->Fill(leptons[0].mtwo(leptons[1]), wt);
            if (leptons[0].sselmu(leptons[1])) h2lSSemmass->Fill(leptons[0].mtwo(leptons[1]), wt);
            if (leptons[0].ssdimu(leptons[1])) h2lSSmmmass->Fill(leptons[0].mtwo(leptons[1]), wt);

// Add the betaz and costh* variables
            hbetaz->Fill(fLeptons.Betaz(),wt);
            habetaz->Fill(std::abs(fLeptons.Betaz()),wt);

//FourVec muon(13, 10.0, 0.0, 30.0, 32.0);
//ROOT::Math::XYZVector beta(0.0, 0.0, 0.6);  // boost along z with beta = 0.6
//FourVec boostedMuon = muon.Boost(beta);
//boostedMuon.Print();

//            fLeptons.Print();
// Boost decay leptons back to the di-lepton rest-frame
            ROOT::Math::XYZVector beta(-fLeptons.Px()/fLeptons.E(), -fLeptons.Py()/fLeptons.E(), -fLeptons.Pz()/fLeptons.E() );
            FourVec lepton0CM = leptons[0].Boost(beta);

//            FourVec lepton1CM = leptons[1].Boost(beta);
//            std::cout << "M = " << fLeptons.Mass() << " Boost beta = (" << beta.X() << ", " << beta.Y() << ", " << beta.Z() << ")\n";
//            leptons[0].Print();
//            lepton0CM.Print();
//            leptons[1].Print();
//            lepton1CM.Print();
//            std::cout << "Lepton0 costh* " << lepton0CM.CosthStar(fLeptons) << std::endl;
//            std::cout << "Lepton1 costh* " << lepton1CM.CosthStar(fLeptons) << std::endl;
// As the two leptons are back-to-back in the rest-frame can simply take the absolute value. 
// It is not like we have an e+e- or p pbar collider with a preferred direction

            double acosthstar = std::abs(lepton0CM.CosthStar(fLeptons));
            hacosthstar->Fill(acosthstar,wt);

// Calculate MT2 using the Lester-Nachman implementation for various assumptions on LSP mass
            double MT2LN0   = leptons[0].MT2(leptons[1], fMET,   0.0,   0.0,  0.0);
            double MT2LN90  = leptons[0].MT2(leptons[1], fMET,  90.0,  90.0);
            double MT2LN150 = leptons[0].MT2(leptons[1], fMET, 150.0, 150.0);
            double MT2LN200 = leptons[0].MT2(leptons[1], fMET, 200.0, 200.0);
            double MT2LN220 = leptons[0].MT2(leptons[1], fMET, 220.0, 220.0);
            double MT2LN260 = leptons[0].MT2(leptons[1], fMET, 260.0, 260.0);
            double MT2LN270 = leptons[0].MT2(leptons[1], fMET, 270.0, 270.0);

            hMT2LNTINY->Fill(MT2LN0, wt);
            hMT2LNSMALL->Fill(MT2LN0, wt);
            hMT2LN0->Fill(MT2LN0, wt);
            hMT2LN90->Fill(MT2LN90, wt);
            hMT2LN150->Fill(MT2LN150, wt);
            hMT2LN200->Fill(MT2LN200, wt);
            hMT2LN220->Fill(MT2LN220, wt);
            hMT2LN260->Fill(MT2LN260, wt);
            hMT2LN270->Fill(MT2LN270, wt);

        }

        else if (leptons.size() == 3) {

            hPtOne->Fill(leptons[0].Pt(), wt);
            hPtTwo->Fill(leptons[1].Pt(), wt);
            hPtThree->Fill(leptons[2].Pt(), wt);

            int nOSSF = leptons[0].ossf(leptons[1]) + leptons[1].ossf(leptons[2]) + leptons[2].ossf(leptons[0]);
            hnOSSF->Fill(nOSSF, wt);

            int charge3l = leptons[0].lcharge() + leptons[1].lcharge() + leptons[2].lcharge();
            h3lCharge->Fill(charge3l, wt);

            int nel = leptons[0].eflavor() + leptons[1].eflavor() + leptons[2].eflavor();
            int nmu = leptons[0].mflavor() + leptons[1].mflavor() + leptons[2].mflavor();
            hnelmu->Fill(nel, nmu, wt);

            std::vector<int> Wleptons;
            std::vector<double> mTlW;
            std::set<int> all_indices = {0, 1, 2};
            for (int i = 0; i < 3; ++i) {
                for (int j = i + 1; j < 3; ++j) {
                    if (leptons[i].osdil(leptons[j])) {
                        for (int k : all_indices) {
                            if (k != i && k != j) {
                                Wleptons.push_back(k);
                                mTlW.push_back(leptons[k].mtsimple(fMET));
                            }
                        }
                    }
                }
            }

            std::sort(mTlW.begin(), mTlW.end());
            if (!dmll.empty()) {
                h3lOnZ->Fill(dmll[0], wt);
            }

            if (nOSSF == 1 && !mTlW.empty()) {
                hmTNew1->Fill(mTlW[0], wt);
            }

            if (nOSSF == 2 && !mTlW.empty()) {
                hmTNew2->Fill(mTlW[0], wt);
            }

            using Predicate = std::function<bool(const FourVec&, const FourVec&)>;

            auto fillMassHistos = [&](const FourVec& a, const FourVec& b, Predicate pred, TH1D* hist) {
                if (pred(a, b)) hist->Fill(a.mtwo(b), wt);
            };

            // Vector of (predicate function, target histogram pointer)
            std::vector<std::tuple<Predicate, TH1D*>> fillFuncs = {
                {[](const FourVec& a, const FourVec& b){ return a.osdiel(b); }, hOSeemass},
                {[](const FourVec& a, const FourVec& b){ return a.oselmu(b); }, hOSemmass},
                {[](const FourVec& a, const FourVec& b){ return a.osdimu(b); }, hOSmmmass},
                {[](const FourVec& a, const FourVec& b){ return a.ssdiel(b); }, hSSeemass},
                {[](const FourVec& a, const FourVec& b){ return a.sselmu(b); }, hSSemmass},
                {[](const FourVec& a, const FourVec& b){ return a.ssdimu(b); }, hSSmmmass},
            };

            // All pairings (0-1, 1-2, 2-0)
            std::vector<std::pair<int, int>> pairs = { {0,1}, {1,2}, {2,0} };

            for (const auto& [pred, hist] : fillFuncs) {
                for (const auto& [i, j] : pairs) {
                    fillMassHistos(leptons[i], leptons[j], pred, hist);
                }
            }
                        
            hTriLeptonMass->Fill(fLeptons.Mass(), wt);
//            std::cout << "Event " <<  event << " METThree = " << fMET.Pt() << std::endl;
//            fMET.Print();
            hMETThree->Fill(fMET.Pt(), wt);
            hmT->Fill(fLeptons.mtp(fMET), wt);
            hTriLRap->Fill(fLeptons.rapidity(), wt);

            int l3code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() - 2;
            h3Code->Fill(l3code + 2, wt);

            if (OnZFlag) {
                hTriLeptonMassOnZ->Fill(fLeptons.Mass(), wt);
            } else {
                hTriLeptonMassOffZ->Fill(fLeptons.Mass(), wt);
            }

            if (charge3l == -3 || charge3l == 3) {
                hTriLeptonMassQ3->Fill(fLeptons.Mass(), wt);
            }

            if ((charge3l == -1 || charge3l == 1) && nOSSF == 0) {
                hTriLeptonMass0Q1->Fill(fLeptons.Mass(), wt);
            }

            bool ATLAS_presel = (leptons[0].Pt() >= 28.0 && leptons[1].Pt() >= 20.0);

            if (ATLAS_presel && (charge3l == -3 || charge3l == 3) && fMET.Pt() > 50.0) {
                hSRSS->Fill(l3code, wt);
            }

            if (nOSSF == 0) {
                hTriLeptonMass0->Fill(fLeptons.Mass(), wt);
            }

            int nOnZ = 0;
            if (nOSSF == 1 && !dmll.empty()) {
                hOnZ1->Fill(dmll[0], wt);
                if (std::abs(dmll[0]) < 10.0) nOnZ = 1;
            }

            if (nOSSF == 2 && dmll.size() >= 2) {
                hOnZ2->Fill(dmll[0], dmll[1], wt);
                if (std::abs(dmll[0]) < 10.0) ++nOnZ;
                if (std::abs(dmll[1]) < 10.0) ++nOnZ;
            }


            bool ATLAS_SROSon = false, ATLAS_SROSoff = false;
            if (nOSSF >= 1) {
                if (nOnZ >= 1){
                    ATLAS_SROSon = true;
                }
                else{
                    ATLAS_SROSoff = true;
                }
            }

            double m3l = fLeptons.Mass();
            bool aRegion = (m3l > 30.0 && m3l < 200.0);
            bool bRegion = (m3l > 200.0 && m3l < 400.0);
            bool cRegion = (m3l > 400.0);

            if (ATLAS_presel && ATLAS_SROSon && fMET.Pt() > 150.0 && !mTlW.empty() && mTlW[0] > 125.0) {
                if (aRegion) hSROSon->Fill(l3code, wt);
                if (bRegion) hSROSon->Fill(l3code + 4, wt);
                if (cRegion) hSROSon->Fill(l3code + 8, wt);
            }

            if (ATLAS_presel && ATLAS_SROSoff && fMET.Pt() > 150.0 && !mTlW.empty() && mTlW[0] > 125.0) {
                if (aRegion) hSROSoff->Fill(l3code, wt);
                if (bRegion) hSROSoff->Fill(l3code + 4, wt);
                if (cRegion) hSROSoff->Fill(l3code + 8, wt);
            }
        }

        else if (leptons.size() == 4) {
            if (!dmll.empty()) {
                h4lOnZ->Fill(dmll[0], wt);
            }

            hPtFour->Fill(leptons[3].Pt(), wt);

            hMETFour->Fill(fMET.Pt(), wt);
            hmTFour->Fill(fLeptons.mtp(fMET), wt);

            int code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor();
            h4Code->Fill(code, wt);

            int charge4l = leptons[0].lcharge() + leptons[1].lcharge() + leptons[2].lcharge() + leptons[3].lcharge();
            h4lCharge->Fill(charge4l, wt);

            hQuadLeptonMass->Fill(fLeptons.Mass(), wt);

            if (OnZFlag) {
                hQuadLeptonMassOnZ->Fill(fLeptons.Mass(), wt);
            } else {
                hQuadLeptonMassOffZ->Fill(fLeptons.Mass(), wt);
            }

            if (code == 5 || code == 7) {
                hQuadLeptonMass57->Fill(fLeptons.Mass(), wt);
               if (OnZFlag) {
                    hQuadLeptonMass57OnZ->Fill(fLeptons.Mass(), wt);
               } else {
                    hQuadLeptonMass57OffZ->Fill(fLeptons.Mass(), wt);
               }
            }

            if (charge4l == -2 || charge4l == 2) {
                hQuadLeptonMassQ2->Fill(fLeptons.Mass(), wt);
                if (OnZFlag) {
                    hQuadLeptonMassQ2OnZ->Fill(fLeptons.Mass(), wt);
                } else {
                    hQuadLeptonMassQ2OffZ->Fill(fLeptons.Mass(), wt);
                }
            }

            if ((charge4l == -2 || charge4l == 2) && (code == 5 || code == 7)) {
                hQuadLeptonMass57Q2->Fill(fLeptons.Mass(), wt);
                if (OnZFlag) {
                    hQuadLeptonMass57Q2OnZ->Fill(fLeptons.Mass(), wt);
                } else {
                    hQuadLeptonMass57Q2OffZ->Fill(fLeptons.Mass(), wt);
                }
            }
        }
        else if (leptons.size() == 5) {
            if (!dmll.empty()) {
                h5lOnZ->Fill(dmll[0], wt);
            }

            int code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor() + leptons[4].lflavor();
            h5Code->Fill(code, wt);
        }
        else if (leptons.size() == 6) {
            if (!dmll.empty()) {
                h6lOnZ->Fill(dmll[0], wt);
            }

            int code = leptons[0].lflavor() + leptons[1].lflavor() + leptons[2].lflavor() + leptons[3].lflavor() + leptons[4].lflavor() + leptons[5].lflavor();
            h6Code->Fill(code, wt);
        }

    }  // end while getline

    infile.close();
    std::cout << "Finished reading events." << std::endl;

    // Save histograms or other output here
    fout->Write();
    fout->Close();
 
    return 0;

}

int main(int argc, char** argv) {
    CLI::App app{"Run analyze program"};

    int nevsToRead = -1;
    std::string which = "Sleptons-LSP260";
    double target = 1.0;
    double lumi = 1.0;
    std::string prefix = ".";
    double elPtMin = 2.0;
    double muPtMin = 3.0;

    app.add_option("-n,--nevsToRead", nevsToRead, "Number of events to read - defaults to all");
    app.add_option("-w,--which", which, "LHE data-set filename (no .lhe)");
    app.add_option("-t,--target", target, "Target integrated luminosity [inv fb]");
    app.add_option("-l,--lumi", lumi, "Actual simulated integrated luminosity [inv fb]");
    app.add_option("-p,--prefix", prefix, "LHE file path prefix");
    app.add_option("-e,--elPtMin", elPtMin, "PtMin (Electron) [GeV]");
    app.add_option("-m,--muPtMin", muPtMin, "PtMin (Muon) [GeV]");

    CLI11_PARSE(app, argc, argv);

    std::cout << "(analyzeArgs.ShowArgs) Program has set\n";
    std::cout << "which:            " << which << "\n";
    std::cout << "target lumi [/fb] " << target << "\n";
    std::cout << "actual lumi [/fb] " << lumi << "\n";
    std::cout << "# of events to read " << nevsToRead << "\n";
    std::cout << "prefix " << prefix << "\n";
    std::cout << "elPtMin " << elPtMin << "\n";
    std::cout << "muPtMin " << muPtMin << "\n";
    std::cout << "Note actual lumi may need to be corrected if not all events are to be read ...\n";

    Analyze(nevsToRead, which, target, lumi, prefix, elPtMin, muPtMin);

    // Use these variables in your analysis below
    return 0;
}
