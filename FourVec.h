#include <cmath>
#include <iostream>
#include <vector>
#include <limits>
#include "Math/Boost.h"
#include "Math/Vector3D.h"
#include "Math/LorentzVector.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"

using LorentzVector = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double>>;

class FourVec {
public:
    int pdgID;
    int flag;
    LorentzVector v;

    // Basic (px, py, pz, E) construction
    explicit FourVec(int id, double px, double py, double pz, double E, int f=1)
      : pdgID(id), flag(f), v(px,py,pz,E) {}

    // Static factory method for (pt, eta, phi, m) construction
    static FourVec FromPtEtaPhiM(int id, double pt, double eta, double phi, double m, int f = 1) {
        double px = pt * std::cos(phi);
        double py = pt * std::sin(phi);
        double pz = pt * std::sinh(eta);
        double E  = std::sqrt(px * px + py * py + pz * pz + m * m);
        return FourVec(id, px, py, pz, E, f);
    }

    FourVec& operator+=(const FourVec& other) {
        this->v += other.v;
        return *this;
    }

    // You may also want to define operator+
    FourVec operator+(const FourVec& other) const {
        FourVec result(*this);  // Copy current FourVec
        result += other;
        return result;
    }

    void Print() const {
        std::cout << "4-vector has pdgID " << pdgID
                  << " (" << v.Px()<<", "<<v.Py()<<", "<<v.Pz()<<", "<<v.E()<<")"
                  << " flag="<<flag
                  << " m="<<Mass()<<" pt="<<Pt()
                  << " costh="<<CosTh()<<" qsinth="<<QSinth()
                  << "\n";
    }

    double Theta() const {
        return v.Theta();
    }

    double eta() const {
        constexpr double epsilon = 1e-10;  // Small cutoff to avoid singularities
        double theta_val = Theta();

        // Avoid division by zero or log(0)
        if (theta_val < epsilon) {
            // theta ~ 0 → eta → +∞
            return std::numeric_limits<double>::infinity();
        } else if (M_PI - theta_val < epsilon) {
            // theta ~ π → eta → -∞
            return -std::numeric_limits<double>::infinity();
        }

        return -std::log(std::tan(0.5 * theta_val));
    }

    double Charge() const {
        if (pdgID==11)  return -1.;
        if (pdgID==-11) return +1.;
        return 0.;
    }

    double mtwo(const FourVec& other) const {
    // Add other 4-vector to self 4-vector and return mass
        auto sumVec = *this + other;  // assuming operator+ is defined for FourVec
        return sumVec.Mass();            // M() returns the invariant mass
    }

    int lcharge() const {
        if (abs(pdgID)==11 || abs(pdgID)==13)
            return pdgID<0 ? +1 : -1;
        return 0;
    }

    int lflavor() const {
        if (abs(pdgID)==11) return 1;
        if (abs(pdgID)==13) return 2;
        return 0;
    }

   // Determine if two leptons are OSSF (opposite-sign same-flavor)
    int ossf(const FourVec& o) const {
        int flav1 = this->lflavor();
        int flav2 = o.lflavor();
        int charge1 = this->lcharge();
        int charge2 = o.lcharge();
        int ossf = 0;
        if (charge1 * charge2 == -1 && flav1 == flav2 && flav1 != 0) {
            ossf = 1;
        }
        return ossf;
    }

// Electron flavor only
    int eflavor() const {
        int flavor = 0;
        if (abs(pdgID) == 11) {
            flavor = 1;
        }
        return flavor;
    }

    // Muon flavor only
    int mflavor() const {
        int flavor = 0;
        if (abs(pdgID) == 13) {
            flavor = 1;
        }
        return flavor;
    }

    bool osdil(const FourVec& o) const {
        return lflavor()==o.lflavor()
            && lcharge()*o.lcharge()==-1;
    }
    // ... (repeat OS/SS methods similarly) ...

    bool osdiel(const FourVec& other) const {
        // Boolean function on whether the di-lepton candidate is an OS di-electron
        bool osdiel = false;
        int flav1 = this->lflavor();
        int flav2 = other.lflavor();
        int charge1 = this->lcharge();
        int charge2 = other.lcharge();
        if (flav1 == flav2 && flav1 == 1) {
            if (charge1 * charge2 == -1) {
                osdiel = true;
            }
        }
        return osdiel;
    }

    bool ssdiel(const FourVec& other) const {
        // Boolean function on whether the di-lepton candidate is a SS di-electron
        bool ssdiel = false;
        int flav1 = this->lflavor();
        int flav2 = other.lflavor();
        int charge1 = this->lcharge();
        int charge2 = other.lcharge();
        if (flav1 == flav2 && flav1 == 1) {
            if (charge1 * charge2 == 1) {
                ssdiel = true;
            }
        }
        return ssdiel;
    }

    bool osdimu(const FourVec& other) const {
        // Boolean function on whether the di-lepton candidate is an OS di-muon
        bool osdimu = false;
        int flav1 = this->lflavor();
        int flav2 = other.lflavor();
        int charge1 = this->lcharge();
        int charge2 = other.lcharge();
        if (flav1 == flav2 && flav1 == 2) {
            if (charge1 * charge2 == -1) {
                osdimu = true;
            }
        }
        return osdimu;
    }

    bool ssdimu(const FourVec& other) const {
        // Boolean function on whether the di-lepton candidate is a SS di-muon
        bool ssdimu = false;
        int flav1 = this->lflavor();
        int flav2 = other.lflavor();
        int charge1 = this->lcharge();
        int charge2 = other.lcharge();
        if (flav1 == flav2 && flav1 == 2) {
            if (charge1 * charge2 == 1) {
                ssdimu = true;
            }
        }
        return ssdimu;
    }

    bool oselmu(const FourVec& other) const {
        // Boolean function on whether the di-lepton candidate is an OS electron-muon pair
        bool oselmu = false;
        int flav1 = this->lflavor();
        int flav2 = other.lflavor();
        int charge1 = this->lcharge();
        int charge2 = other.lcharge();
        if (flav1 != flav2) {
            if (charge1 * charge2 == -1) {
                oselmu = true;
            }
        }
        return oselmu;
    }

    bool sselmu(const FourVec& other) const {
        // Boolean function on whether the di-lepton candidate is a SS electron-muon pair
        bool sselmu = false;
        int flav1 = this->lflavor();
        int flav2 = other.lflavor();
        int charge1 = this->lcharge();
        int charge2 = other.lcharge();
        if (flav1 != flav2) {
            if (charge1 * charge2 == 1) {
                sselmu = true;
            }
        }
        return sselmu;
    }

    double CosTh() const {
        auto p = v.P();
        return (p>1e-6 ? v.Pz()/p : 0);
    }

    double Pt() const { return v.Pt(); }
    double Mass() const { return v.M(); }

    double E() const { return v.E(); }
    double P() const { return v.P(); }
    double Px() const { return v.Px(); }
    double Py() const { return v.Py(); }
    double Pz() const { return v.Pz(); }        
    double Beta() const { return v.P()/v.E(); }
    double Betaz() const { return v.Pz()/v.E(); }

    double QSinth() const {
        double s = Pt()/P();
        return (v.Pz()<0 ? -s : s);
    }

    double Phi() const { return v.Phi(); }

    double DPhiR(const FourVec& o) const {
        double d = o.Phi() - Phi();
        if (d<-M_PI) d+=2*M_PI;
        if (d> M_PI) d-=2*M_PI;
        return d;
    }
    double DEta(const FourVec& o) const {
        double e1 = -std::log(std::tan(0.5*v.Theta()));
        double e2 = -std::log(std::tan(0.5*o.v.Theta()));
        return e2 - e1;
    }
    double DeltaR(const FourVec& o) const {
        return std::hypot(DPhiR(o), DEta(o));
    }

/////FIXTHIS 
    double CosthStar(const FourVec& o) const {
        double dotp = v.Px()*o.Px() + v.Py()*o.Py() + v.Pz()*o.Pz(); 
        double val = dotp/(v.P()*o.P());
        return val;
    }

    FourVec Add(int newID, const FourVec& o) const {
        auto w = v + o.v;
        return FourVec(newID, w.Px(), w.Py(), w.Pz(), w.E());
    }
    
// Accept electron if |eta| < 2.5 and pt > ptMin
    bool accEl(double ptMin = 2.0) const {
        return std::abs(v.eta()) < 2.5 && v.Pt() > ptMin;
    }

// Accept muon if |eta| < 2.4 and pt > ptMin
    bool accMu(double ptMin = 3.0) const {
        return std::abs(v.eta()) < 2.4 && v.Pt() > ptMin;
    }

    double mtp(const FourVec& o) const {
        double m1 = Mass();
        double m2 = 0.0;     // Assign NO mass to the MET 4-vector in this calculation
        double ET1 = std::sqrt(m1*m1 + Pt()*Pt());
        double ET2 = o.Pt();
        double dpt = Px()*o.Px() + Py()*o.Py();
        return std::sqrt(m1*m1 + 2.0*(ET1*ET2 - dpt));
    } 

    double MTp(const FourVec& o) const {
        double m1 = Mass(), m2 = o.Mass();
        double ET1 = std::sqrt(m1*m1 + Pt()*Pt());
        double ET2 = std::sqrt(m2*m2 + o.Pt()*o.Pt());
        double dpt = Px()*o.Px() + Py()*o.Py();
        return std::sqrt(m1*m1 + m2*m2 + 2.0*(ET1*ET2 - dpt));
    }

    // Compute transverse mass (using eqn from arXiv:2503.13135)
    double mtsimple(const FourVec& o) const {
        double px1 = Px();
        double py1 = Py();
        double pT1 = std::sqrt(px1 * px1 + py1 * py1);

        double px2 = o.Px();
        double py2 = o.Py();

        const double mfloor = 0.511e-3;
        double ET2 = std::sqrt(mfloor * mfloor + px2 * px2 + py2 * py2);

        double cos_term = (px1 * px2 + py1 * py2) / (pT1 * ET2);
        double mtsq = 2.0 * pT1 * ET2 * (1.0 - cos_term);

        return std::sqrt(mtsq);
    }

    // Rapidity
    double rapidity() const {
        const double DEFAULT = (Pz() < 0.0) ? -7.0 : 7.0;

        if (std::abs(E() - Pz()) < 1.0e-6) {
            return DEFAULT;
        } else {
            return 0.5 * std::log((E() + Pz()) / (E() - Pz()));
        }
    }

    FourVec Boost(const ROOT::Math::XYZVector& beta) const {
    // Construct Lorentz boost from the beta vector
        ROOT::Math::Boost boost(beta);

    // Apply the boost to the internal 4-vector
        LorentzVector boostedVec = v;
        boostedVec = boost(boostedVec); 

    // Return a new FourVec with the same pdgID and flag, but boosted 4-vector
        return FourVec(pdgID, boostedVec.Px(), boostedVec.Py(), boostedVec.Pz(), boostedVec.E(), flag);
    }

};
