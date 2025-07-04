// Organize cuts for selecting events that are consistent with a prompt 3-lepton signature

enum class TriCuts {
    GSNumber = 0, 
    PtOne, PtTwo, PtThree, PtFourVeto,
    BTagVeto, SIP3DCut, NUMCUTS
};

std::string to_string(TriCuts value) {
    switch (value) {
        case TriCuts::GSNumber:   return "GSNumber";         // eg. >=3 G+S leptons
        case TriCuts::PtOne:      return "PtOne";            // eg. highest pt G+S lepton pT>20 GeV
        case TriCuts::PtTwo:      return "PtTwo";            // 2nd highest pt G+S lepton pT>15 GeV
        case TriCuts::PtThree:    return "PtThree";          // 3rd highest pt G+S lepton pT>10  
        case TriCuts::PtFourVeto: return "PtFourVeto";       // no appropriate lepton with pT > 5 GeV
        case TriCuts::BTagVeto:   return "BTagVeto";         // no b-tagged jets (uses loose criterion)
        case TriCuts::SIP3DCut:   return "SIP3DCut";         // reject events with any of the 3 selected leptons with SIP3D > eg. 3.5
        case TriCuts::NUMCUTS:    return "NUMCUTS";          // Use this to count the number of cuts.
    }
    return "Unknown";
}
