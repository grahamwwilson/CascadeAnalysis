// Organize cuts for selecting events that are consistent with a prompt 3-lepton signature

enum class QuadCuts {
    GSNumber = 0, 
    PtOne, PtTwo, PtThree, PtFour, Trigger, PtFiveVeto,
    BTagVeto, SIP3DCut, 
    MinMll, MxMinMll, OffZ, Pt4L, M4LZV, MxCompMll, NUMCUTS
};

// Note: actual cut values are for now mostly hard-coded in the code, and these comments are indicative of cut values (and out-of-date)

std::string to_string(QuadCuts value) {
    switch (value) {
        case QuadCuts::GSNumber:   return "GSNumber";         // eg. >=3 G+S leptons
        case QuadCuts::PtOne:      return "PtOne";            // eg. highest pt G+S lepton pT>20 GeV
        case QuadCuts::PtTwo:      return "PtTwo";            // eg. 2nd highest pt G+S lepton pT>15 GeV
        case QuadCuts::PtThree:    return "PtThree";          // eg. 3rd highest pt G+S lepton pT>10  
        case QuadCuts::PtFour:     return "PtFour";           // eg. 4th highest pt G+S lepton pT>5 GeV
        case QuadCuts::Trigger:    return "Trigger";          // eg. Single or double lepton trigger 
        case QuadCuts::PtFiveVeto: return "PtFiveVeto";       // eg. No appropriate lepton with pT > 3 GeV
        case QuadCuts::BTagVeto:   return "BTagVeto";         // eg. no b-tagged jets (uses loose criterion)
        case QuadCuts::SIP3DCut:   return "SIP3DCut";         // reject events with any of the 3 selected leptons with SIP3D > eg. 3.5
        case QuadCuts::MinMll:     return "MinMll";           // reject events with the smallest lepton pair mass below eg 4 GeV
        case QuadCuts::MxMinMll:   return "MxMinMll";         // reject events with the smallest lepton pair mass above eg 21 GeV
        case QuadCuts::OffZ:       return "OffZ";             // reject events with the an OSSF lepton pair within eg. 7.5 GeV of mZ.
        case QuadCuts::Pt4L:       return "Pt4L";             // reject events with the four-lepton pT below eg 15 GeV.
        case QuadCuts::M4LZV:      return "M4LZV";            // reject events with four-lepton mass within eg. 7.5 GeV of mZ.
        case QuadCuts::MxCompMll:  return "MxCompMll";        // reject events with the complementary pair mass above eg 60 GeV
        case QuadCuts::NUMCUTS:    return "NUMCUTS";          // Use this to count the number of cuts.
    }
    return "Unknown";
}
