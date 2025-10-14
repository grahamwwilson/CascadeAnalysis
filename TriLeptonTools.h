template <size_t N>
int QPairCode(const std::pair<int,int>& lepPair, const std::array<int,N>& qfs) {

    // pair indices are 1-based in TriLeptonInfo, so subtract 1

    int i = lepPair.first  - 1;
    int j = lepPair.second - 1;

    int qfi = qfs[i];
    int qfj = qfs[j];

    int code = 0;

    if (qfi * qfj < 0) {
        // opposite sign
        if (std::abs(qfi) == std::abs(qfj)) {
            if(std::abs(qfi) == 1){
                code = 1; // OSSF-ee
            }
            else {
                code = 2; // OSSF-mm
            }
        } 
        else {  // different flavor
            code = 3;     // OSDF-em
        }
    } 
    else {   // same sign
        if (std::abs(qfi) == std::abs(qfj)) {   // same flavor
            if( qfi == -1){
                code = 4; // NSS-ee
            }
            else if ( qfi == -2 ){
                code = 5; // NSS-mm
            }
            else if ( qfi ==  1 ){
                code = 7; // PSS-ee
            }
            else if ( qfi ==  2 ){
                code = 8; // PSS-mm
            }
            else{
                std::cout << "Shouldn't be here in QPairCode " << qfi << " " << qfj << std::endl;
            }
        } 
        else {  // different flavor
            if( (qfi == -1 && qfj == -2) || (qfi == -2 && qfj == -1) ){
                code = 6;  // NSS-em
            }
            else if ( (qfi ==  1 && qfj ==  2) || (qfi ==  2 && qfj ==  1) ){
                code = 9;  // PSS-em
            }
            else{
                std::cout << "Shouldn't be here either in QPairCode " << qfi << " " << qfj << std::endl; 
            }
        }
    }
    return code;
}

int CompLepCode(int compLep, const std::array<int,3>& qfs) {

    // pair indices are 1-based in TriLeptonInfo, so subtract 1

    int i = compLep - 1;
    int qf = qfs[i];

    int code = 0;
    if(qf == 1)code = 1;
    if(qf == 2)code = 2;
    if(qf == -1)code = 3;
    if(qf == -2)code = 4;

    return code;
}



std::tuple<double, double, int, double, int, int> TriLeptonInfo(double m12, double m13, double m23, bool ossf12, bool ossf13, bool ossf23, int qf1, int qf2, int qf3){

// Calculate a number of quantities related to trilepton topologies and return in a tuple.

// 1. The min dilepton mass independent of flavor/charge
//     double minmll = std::min( { m12, m13, m23 });
// 2. The max dilepton mass independent of flavor/charge
     double maxmll = std::max( { m12, m13, m23 });
     const double MZ = 91.2;
// 3. The number of opposite-sign same flavor (OSSF) dileptons
     int nossf = 0;
// 4. For OSSF dileptons the smallest SIGNED deviation with respect to the nominal Z mass. If no 
     double devZ = -95.0;   // default value
     if( ossf12 ){
         nossf++;
         double thisdevZ = m12 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf13 ){
         nossf++;
         double thisdevZ = m13 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }
     if( ossf23 ){
         nossf++;
         double thisdevZ = m23 - MZ;
         if(std::abs(thisdevZ) < std::abs(devZ)) devZ = thisdevZ;
     }

   // 3. Find min pair and complementary lepton
     struct PairEntry {
         double mass;
         std::pair<int,int> pair;
         int comp; // complementary lepton
     };

     std::array<PairEntry,3> pairs{{
         { m12, {1,2}, 3 },
         { m13, {1,3}, 2 },
         { m23, {2,3}, 1 }
     }};

     auto min_it = std::min_element(pairs.begin(), pairs.end(),
                                   [](auto &a, auto &b){ return a.mass < b.mass; });

     auto minmll   = min_it->mass;
     auto min_pair = min_it->pair;
     int comp_lep  = min_it->comp;

     std::array<int,3> qfs = { qf1, qf2, qf3};

     int minPairCode  = QPairCode(min_pair, qfs);
     int compLepCode  = CompLepCode(comp_lep, qfs);
 
     return std::make_tuple(minmll, maxmll, nossf, devZ, minPairCode, compLepCode);
 
}
