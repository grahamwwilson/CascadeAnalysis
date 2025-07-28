// Organize cuts for selecting events that are consistent with a prompt 3-lepton signature

enum class QuadCuts {
    GSNumber = 0, 
    PtOne, PtTwo, PtThree, PtFour, Trigger, PtFiveVeto,
    BTagVeto, SIP3DCut, 
    MinMll, MxMinMll, OffZ, NUMCUTS
};

std::string to_string(QuadCuts value) {
    switch (value) {
        case QuadCuts::GSNumber:   return "GSNumber";         // eg. >=3 G+S leptons
        case QuadCuts::PtOne:      return "PtOne";            // eg. highest pt G+S lepton pT>20 GeV
        case QuadCuts::PtTwo:      return "PtTwo";            // 2nd highest pt G+S lepton pT>15 GeV
        case QuadCuts::PtThree:    return "PtThree";          // 3rd highest pt G+S lepton pT>10  
        case QuadCuts::PtFour:     return "PtFour";           // 4th highest pt G+S lepton pT>5 GeV
        case QuadCuts::Trigger:    return "Trigger";          // Single or double lepton trigger 
        case QuadCuts::PtFiveVeto: return "PtFiveVeto";           // No appropriate lepton with pT > 3 GeV
        case QuadCuts::BTagVeto:   return "BTagVeto";         // no b-tagged jets (uses loose criterion)
        case QuadCuts::SIP3DCut:   return "SIP3DCut";         // reject events with any of the 3 selected leptons with SIP3D > eg. 3.5
        case QuadCuts::MinMll:     return "MinMll";           // reject events with the smallest lepton pair mass below 4 GeV
        case QuadCuts::MxMinMll:   return "MxMinMll";         // reject events with the smallest lepton pair mass above 65 GeV
        case QuadCuts::OffZ:       return "OffZ";             // reject events with the an OSSF lepton pair within eg. 7.5 GeV of mZ.
        case QuadCuts::NUMCUTS:    return "NUMCUTS";          // Use this to count the number of cuts.
    }
    return "Unknown";
}
