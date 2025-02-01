from dataclasses import dataclass
import math

@dataclass
class FourVec:
    " FourVec class with a number of helper methods "
    pdgID: int
    px: float
    py: float
    pz: float
    E: float
    flag: int = 1    #Add flag value to denote special cases
    
    def print(self):
        " Print 4-vector attributes "
        print("4-vector has pdgID {pdgID} and elements ({px}, {py}, {pz}, {E}) with flag={flag}, m={m}, pt = {pt}, costh = {costh}, qsinth = {qsinth}".
        format(pdgID = self.pdgID, px = self.px, py =self.py, pz = self.pz, E = self.E, flag=self.flag, m = self.mass(), pt = self.pt(), costh=self.costh(), qsinth = self.qsinth() ))
        
    def theta(self):
        " Polar angle (rad) "
        p = math.sqrt( (self.px)**2 + (self.py)**2 + (self.pz)**2 )
        costh = self.pz/p
        return math.acos(costh)
        
    def charge(self):
        " Charge of 4-vector FIXME - generalize"
        charge = 0.0
        if self.pdgID == 11:
            charge = -1.0
        if self.pdgID == -11:
            charge = 1.0
        return charge 
        
    def lcharge(self):
        " Lepton charge "
        charge = 0
        if self.pdgID == 11 or self.pdgID == 13:
            charge = -1
        if self.pdgID == -11 or self.pdgID == -13:
            charge = 1
        return int(charge)
        
    def lflavor(self):
        " Lepton flavor "
        flavor = 0
        if abs(self.pdgID) == 11:
            flavor = 1
        if abs(self.pdgID) == 13:
            flavor = 2
        return flavor
        
    def eflavor(self):
        " Lepton of electron flavor "
        flavor = 0
        if abs(self.pdgID) == 11:
            flavor = 1
        return flavor
        
    def mflavor(self):
        " Lepton of muon flavor "
        flavor = 0
        if abs(self.pdgID) == 13:
            flavor = 1
        return flavor
        
    def osdiel(self,other):
        " Boolean function on whether the di-lepton candidate is an OS di-electron "
        osdiel = False
        flav1 = self.lflavor()
        flav2 = other.lflavor()
        charge1 = self.lcharge()
        charge2 = other.lcharge()
        if flav1 == flav2 and flav1 == 1:
            if charge1*charge2 == -1:
                osdiel = True
        return osdiel
        
    def ssdiel(self,other):
        " Boolean function on whether the di-lepton candidate is a SS di-electron "
        ssdiel = False
        flav1 = self.lflavor()
        flav2 = other.lflavor()
        charge1 = self.lcharge()
        charge2 = other.lcharge()
        if flav1 == flav2 and flav1 == 1:
            if charge1*charge2 == 1:
                ssdiel = True
        return ssdiel
        
    def osdimu(self,other):
        " Boolean function on whether the di-lepton candidate is an OS di-muon "
        osdimu = False
        flav1 = self.lflavor()
        flav2 = other.lflavor()
        charge1 = self.lcharge()
        charge2 = other.lcharge()
        if flav1 == flav2 and flav1 == 2:
            if charge1*charge2 == -1:
                osdimu = True
        return osdimu
        
    def ssdimu(self,other):
        " Boolean function on whether the di-lepton candidate is a SS di-muon "
        ssdimu = False
        flav1 = self.lflavor()
        flav2 = other.lflavor()
        charge1 = self.lcharge()
        charge2 = other.lcharge()
        if flav1 == flav2 and flav1 == 2:
            if charge1*charge2 == 1:
                ssdimu = True
        return ssdimu
        
    def oselmu(self,other):
        " Boolean function on whether the di-lepton candidate is an OS electron-muon pair"
        oselmu = False
        flav1 = self.lflavor()
        flav2 = other.lflavor()
        charge1 = self.lcharge()
        charge2 = other.lcharge()
        if flav1 != flav2:
            if charge1*charge2 == -1:
                oselmu = True
        return oselmu 
        
    def accEl(self):
        " Boolean function on whether the electron is accepted "
        eta = self.eta()
        pt  = self.pt()
        accEl = False
        if abs(eta) < 2.5 and pt > 2.0: accEl = True
        return accEl
        
    def accMu(self):
        " Boolean function on whether the muon is accepted "
        eta = self.eta()
        pt  = self.pt()
        accMu = False
        if abs(eta) < 2.4 and pt > 3.0: accMu = True
        return accMu        
 
    def sselmu(self,other):
        " Boolean function on whether the di-lepton candidate is a SS electron-muon pair"
        sselmu = False
        flav1 = self.lflavor()
        flav2 = other.lflavor()
        charge1 = self.lcharge()
        charge2 = other.lcharge()
        if flav1 != flav2:
            if charge1*charge2 == 1:
                sselmu = True
        return sselmu                                        
        
    def ossf(self,other):
        " Work out whether two leptons are OSSF "                 
        flav1 = self.lflavor()
        flav2 = other.lflavor()
        charge1 = self.lcharge()
        charge2 = other.lcharge()
        ossf = 0
        if charge1*charge2 == -1 and flav1 == flav2 and flav1 != 0:
            ossf = 1
        return ossf
            
    def mqcosth(self):
        " - q cos(th) of 4-vector "
        value = - self.charge()*self.costh()
        return value            
        
    def costh(self):
        " Cosine of the polar angle "
        p = math.sqrt( (self.px)**2 + (self.py)**2 + (self.pz)**2 )
        value = -2.0
        if abs(p) > 1.0e-6:
            value = self.pz/p
        else:
             print("Funny p ",p,self.pdgID,self.px,self.py,self.pz,self.E)
        return value
        
    def radius(self):
        " Radius of ECAL cluster at LCAL calorimeter "
        zLCAL = 2.46
        acosth = self.acosth()
        atheta = math.acos(acosth)
        r = zLCAL*math.tan(atheta)
        return r
        
    def sinth(self):
        " sine of the polar angle "
        p = math.sqrt( (self.px)**2 + (self.py)**2 + (self.pz)**2 )
        sinth = 0.0
        if p > 1.0e-10:
            sinth = self.pt()/p
        return sinth 
        
    def atanth(self):
        " absolute tan of the polar angle "
        sinth = self.sinth()
        atanth = abs(math.tan(math.asin(sinth)))
        return atanth                 

    def qsinth(self):
        " Signed sine of the polar angle "
        p = math.sqrt( (self.px)**2 + (self.py)**2 + (self.pz)**2 )
        sinth = 0.0
        if p > 1.0e-10:
            sinth = self.pt()/p
        if self.pz < 0.0:
            sinth = - sinth
        return sinth
        
    def beta(self):
        " beta value for the 4-vector "
        p = math.sqrt( (self.px)**2 + (self.py)**2 + (self.pz)**2 )
        b = p/self.E
        return b        
        
    def momentum(self):
        " Momentum [GeV]"
        p = math.sqrt( (self.px)**2 + (self.py)**2 + (self.pz)**2 )
        return p               
        
    def acosth(self):
        " Absolute value of the cosine of the polar angle "
        p = math.sqrt( (self.px)**2 + (self.py)**2 + (self.pz)**2 )
        return abs(self.pz/p)           
        
    def phi(self):
        " Azimuthal angle (rad) "
        phi = math.atan2(self.py, self.px)
        if phi < 0.0:
            phi += 2.0*math.pi
        return phi
        
    def bphi(self):
        " Azimuthal angle (rad) reconstructed from calorimeter coordinates after curvature in magnetic field "
# TODO Make this work for the barrel TOO!
# Need to figure out how to specify this.
# Use code in helix.f
        bfield = 3.5
        acosth = self.acosth()
        zLCAL = 2.46
        RECAL = 1.85
        zDET = zLCAL
        if acosth < 0.8:
            theta = math.acos(acosth)
            zDET = RECAL/math.tan(theta)

        alpha = 1.0/(0.2998*bfield)
        pt = self.pt()
        charge = 0.0
        if self.pdgID==11:
            charge= -1.0
        if self.pdgID==-11:
            charge = 1.0
        
# default to no curvature
        bphi = self.phi()
        
        dphi = 0.0
        if charge != 0.0:
# Need to check sign convention!        
            kappa = charge/pt
            rho = alpha/kappa
            womega = 0.2998*math.sin(self.theta())/rho        
# Need to protect against -ve z! (So put an absolute for vz)
            vz = 0.2998*abs(math.cos(self.theta()))
            tofz = zDET/vz
            dphi = womega*tofz
            dphi = 0.5*dphi      # track turns by dphi, but actual impact point is only half based on the chord
            bphi = self.phi() + dphi
            if bphi < 0.0:
                bphi += 2.0*math.pi
            if bphi > 2.0*math.pi:
                bphi -= 2.0*math.pi
        return bphi
            
    def pt(self):
        " Transverse momentum (GeV) "
        return math.sqrt( (self.px)**2 + (self.py)**2 )
        
    def mass(self):
        " Invariant mass (GeV) "
        msq = (self.E)**2 - (self.px)**2 - (self.py)**2 - (self.pz)**2
        value = 0.0
        if msq>0.0:
            value = math.sqrt(msq)
        elif msq<0.0:
            value = -math.sqrt(abs(msq))
        return value
              
    def boost(self, b):
        " Lorentz boost the four vector along z by relative velocity of b in units of c "
        g = 1.0/math.sqrt(1.0-b**2)
        pxp = self.px
        pyp = self.py
        pzp = g*b*self.E + g*self.pz
        Ep  = g*self.E + g*b*self.pz
        pvec = FourVec(self.pdgID, pxp, pyp, pzp, Ep)
        return pvec
        
    def dphi(self, other):
        " Measure the signed azimuthal acoplanarity between two clusters [(self.phi - other.phi) mod 2pi] - pi. This is centered on zero for back-to-back"
        phi1 = self.phi()
        phi2 = other.phi()
        dphi = phi2 - phi1
        if dphi < 0.0:
           dphi += 2.0*math.pi
        if dphi > 2.0*math.pi:
           dphi -= 2.0*math.pi
        return dphi - math.pi 
        
    def dphiR(self, other):
        " Measure the azimuthal distance between two 4-vectors (self.phi - other.phi). This is defined to be in the range [-pi, pi] "
        phi1 = self.phi()
        phi2 = other.phi()
        dphi = phi2 - phi1
        if dphi < -math.pi:
           dphi += 2.0*math.pi
        if dphi >  math.pi:
           dphi -= 2.0*math.pi
        return dphi         
        
    def deta(self, other):
        " Measure the delta-eta between two 4-vectors (self.eta - other.eta)"
        eta1 = self.eta()
        eta2 = other.eta()
        result = eta2 - eta1
        return result
        
    def deltaR(self, other):
        " Measure the delta-R between two 4-vectors"
        dphi = self.dphiR(other)
        deta = self.deta(other)
        result = math.sqrt(deta**2 + dphi**2)
        return result     
        
    def dbphi(self, other):
        " Measure the signed curvature modified delta-phi between two clusters (self.bphi - other.bphi) "
        phi1 = self.bphi()
        phi2 = other.bphi()
        dphi = phi2 - phi1
        if dphi < 0.0:
           dphi += 2.0*math.pi
        if dphi > 2.0*math.pi:
           dphi -= 2.0*math.pi
        return dphi - math.pi                 

    def bacop(self, other):
        " Measure bending acoplanarity between two clusters "
# TODO check this works for all cases        
        dphi = self.bphi() - other.bphi()
        cdphi = math.cos(dphi)
        dphi2 = math.acos(cdphi)
        bacop = math.pi - dphi2
        return bacop

# TODO. Add quantity that works for phiL, phiR a la OPAL       
    def acop(self, other):
        " Measure acoplanarity between two 4-vectors "
        px1 = self.px
        py1 = self.py
        px2 = other.px
        py2 = other.py        
# dot product
        dotp = px1*px2 + py1*py2
        pt1 = math.sqrt(px1**2 + py1**2)
        pt2 = math.sqrt(px2**2 + py2**2)
        cosphi = dotp/(pt1*pt2)
        if cosphi < -1.0:
            cosphi = -1.0
        acop = math.pi - math.acos(cosphi)
        print("cosphi, acop ",cosphi,acop)        
# Should check range
        if acop < 0.0:
            print("Bad acop range ",cosphi,acop)        
        return acop
        
    def bperp(self, other):
        " (theta, phi) dependent part of eqn 9 in OPAL multiphoton paper "
        theta1 = self.theta()
        phi1 = self.phi()
        theta2 = other.theta()
        phi2 = other.phi()
        num = math.sin(theta1) + math.sin(theta2)
        value = num*abs(math.cos(0.5*(phi1-phi2)))
        return value
        
    def flost(self, other):
        " (theta dependent part of eqn 10 in OPAL multiphoton paper "
        theta1 = self.theta()
        theta2 = other.theta()
        value = 1.0 + ( math.sin(theta1) + math.sin(theta2) )/ abs(math.sin(theta1 + theta2))
        return 1.0/value        
        
    def openingangle(self, other):
        " Measure opening angle between two 4-vectors "
        px1 = self.px
        py1 = self.py
        pz1 = self.pz
        px2 = other.px
        py2 = other.py
        pz2 = other.pz       
# dot product
        dotp = px1*px2 + py1*py2 + pz1*pz2
        p1 = math.sqrt(px1**2 + py1**2 + pz1**2)
        p2 = math.sqrt(px2**2 + py2**2 + pz2**2)
        cospsi = dotp/(p1*p2)
        if cospsi < -1.0:
            cospsi = -1.0
        if cospsi > 1.0:
            cospsi = 1.0
#        print("cospsi ",cospsi)
        psi = math.acos(cospsi)
        print("cospsi, psi",cospsi,psi,psi*180.0/math.pi)         
        return psi     
        
    def add(self, number, other):
        " Add other 4-vector to self 4-vector "
        px = self.px + other.px
        py = self.py + other.py
        pz = self.pz + other.pz
        E  = self.E + other.E
        pvec = FourVec(number, px, py, pz, E)
        return pvec
        
    def madd(self, number, other):
        " Add other 4-vector to self 4-vector and return mass"
        px = self.px + other.px
        py = self.py + other.py
        pz = self.pz + other.pz
        E  = self.E + other.E
        pvec = FourVec(number, px, py, pz, E)
        return pvec.mass() 
        
    def mtwo(self, other):
        " Add other 4-vector to self 4-vector and return mass"
        px = self.px + other.px
        py = self.py + other.py
        pz = self.pz + other.pz
        E  = self.E + other.E
        pvec = FourVec(9999, px, py, pz, E)
        return pvec.mass()                
        
    def addlist(self, number, fvlist):
        " Add each 4-vector in list to self 4-vector "
        px = self.px
        py = self.py
        pz = self.pz
        E  = self.E
        for fv in fvlist:
            px += fv.px
            py += fv.py
            pz += fv.pz
            E  += fv.E
        pvec = FourVec(number, px, py, pz, E)
        return pvec
        
    def mt(self,other):
        " Compute transverse mass of the self and other 4-vector."
        px1 = self.px
        py1 = self.py
        px2 = other.px
        py2 = other.py
        m1 = self.mass()
        m2 = other.mass()
        ET1 =    math.sqrt(px1**2 + py1**2)/self.beta()
        ET2 =    math.sqrt(px2**2 + py2**2)/other.beta()
        mtsq = m1**2 + m2**2 + 2.0*(ET1*ET2 - (px1*px2 + py1*py2)) 
        return math.sqrt(mtsq)
        
    def mtp(self,other):
        " Compute transverse mass of the self and other 4-vector. Use eqn 3 from RJR paper"
        px1 = self.px
        py1 = self.py
        px2 = other.px
        py2 = other.py
        m1 = self.mass()
        m2 = 0.0           # Could potentially use something else here
        ET1 = math.sqrt(m1**2 + px1**2 + py1**2)
        ET2 = math.sqrt(m2**2 + px2**2 + py2**2)
        mtsq = m1**2 + m2**2 + 2.0*(ET1*ET2 - (px1*px2 + py1*py2)) 
        return math.sqrt(mtsq)        
        
    def tdotp(self,other):
        " Compute transverse momentum dot product of the self and other 4-vector "
        px1 = self.px
        py1 = self.py
        px2 = other.px
        py2 = other.py
        dotp = px1*px2 + py1*py2
        return dotp
        
    def eta(self):
        " Compute pseudo-rapidity for this 4-vector "
        theta = self.theta()
        result = -math.log(math.tan(0.5*theta))
        return result
        
    def rapidity(self):
        " Compute rapidity for this 4-vector "
        E = self.E
        pz = self.pz
        DEFAULT = 7.0
        if pz < 0.0:
             DEFAULT = -7.0
             
        if abs(E-pz) < 1.0e-6:
            result = DEFAULT
        else:      
            result = 0.5*math.log( (E+pz)/(E-pz) )
            
        return result             
        
              
                  
        
