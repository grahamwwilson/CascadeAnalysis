// Organize cuts for selecting events that are consistent with a prompt 2-lepton signature

enum class DiCuts {
    GSNumber = 0, 
    PtOne, PtTwo, PtThreeVeto,
    BTagVeto, SIP3DCut, NUMCUTS
};

std::string to_string(DiCuts value) {
    switch (value) {
        case DiCuts::GSNumber:    return "GSNumber";         // eg. >=2 G+S leptons
        case DiCuts::PtOne:       return "PtOne";            // eg. highest pt G+S lepton pT>25 GeV
        case DiCuts::PtTwo:       return "PtTwo";            // 2nd highest pt G+S lepton pT>20 GeV  
        case DiCuts::PtThreeVeto: return "PtThreeVeto";     // no appropriate lepton with pT > 5 GeV
        case DiCuts::BTagVeto:    return "BTagVeto";         // no b-tagged jets (uses loose criterion)
        case DiCuts::SIP3DCut:    return "SIP3DCut";         // reject events with either of the 2 selected leptons with SIP3D > eg. 3.0
        case DiCuts::NUMCUTS:     return "NUMCUTS";          // Use this to count the number of cuts.
    }
    return "Unknown";
}
