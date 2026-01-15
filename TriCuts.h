// Organize cuts for selecting events that are consistent with a prompt 3-lepton signature

enum class TriCuts {
    GSNumber = 0, 
    PtOne, PtTwo, PtThree, Trigger, PtFourVeto,
    BTagVeto, SIP3DCut, MaxEta, MinEta,
    MinMll, MxMinMll, OffZ, Pt3L, M3LZV, FQSB, OSSF, CS, NUMCUTS
};

std::string to_string(TriCuts value) {
    switch (value) {
        case TriCuts::GSNumber:   return "GSNumber";         // eg. >=3 G+S leptons
        case TriCuts::PtOne:      return "PtOne";            // eg. highest pt G+S lepton pT>20 GeV
        case TriCuts::PtTwo:      return "PtTwo";            // 2nd highest pt G+S lepton pT>15 GeV
        case TriCuts::PtThree:    return "PtThree";          // 3rd highest pt G+S lepton pT>10 GeV 
        case TriCuts::Trigger:    return "Trigger";          // Single-lepton or double-lepton trigger 
        case TriCuts::PtFourVeto: return "PtFourVeto";       // no appropriate lepton with pT > 5 GeV
        case TriCuts::BTagVeto:   return "BTagVeto";         // no b-tagged jets (uses loose criterion)
        case TriCuts::SIP3DCut:   return "SIP3DCut";         // reject events with any of the 3 selected leptons with SIP3D > eg. 3.5
        case TriCuts::MaxEta:     return "MaxEta";           // reject events unless all 3 leptons have |eta| < max value (such as 2.1)
        case TriCuts::MinEta:     return "MinEta";           // reject events unless at least one lepton has |eta| < min value (such as 1.3)
        case TriCuts::MinMll:     return "MinMll";           // reject events with the smallest lepton pair mass below 4 GeV
        case TriCuts::MxMinMll:   return "MxMinMll";         // reject events with the smallest lepton pair mass above 65 GeV
        case TriCuts::OffZ:       return "OffZ";             // reject events with the an OSSF lepton pair within eg. 7.5 GeV of mZ.
        case TriCuts::Pt3L:       return "Pt3L";             // reject events with the tri-lepton pT below eg 15 GeV.
        case TriCuts::M3LZV:      return "M3LZV";            // reject events with trilepton mass within eg. 7.5 GeV of mZ.
        case TriCuts::FQSB:       return "FQSB";             // reject trilepton (minPairCode, compLepCode) event categories with low lepton-flavor/charge S/B 
        case TriCuts::OSSF:       return "OSSF";             // reject events with no OSSF lepton pair (makes a lot of sense for OSemu cases selected by FQSB cut)
        case TriCuts::CS:         return "CS";               // reject events with large |costheta*(Collins-Soper)|
        case TriCuts::NUMCUTS:    return "NUMCUTS";          // Use this to count the number of cuts.
    }
    return "Unknown";
}
