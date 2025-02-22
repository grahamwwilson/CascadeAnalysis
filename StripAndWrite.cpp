#include "CLI11.hpp"   // See https://github.com/CLIUtils/CLI11    This is v2.4.2 of CLI11.
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

struct Particle {
    int id;           // An index for the particle according to order of appearance
    int pdgCode;      // Particle ID (PDG code)
    int status;       // Status (1 for final state, 3 for intermediate, etc.)
    int mother1, mother2, daughter1, daughter2; // Mother and daughter indices
    double px, py, pz, energy; // Particle momentum and energy
    double mass;       // Mass of the particle
    double charge;     // Charge of the particle
    int colorFlow;     // Color flow (usually 0 for standard processes)
};

bool isNeutrino(int pdgCode) {
    // Neutrinos have PDG codes 12, 14, 16 and their anti-particles -12, -14, -16
    return (pdgCode == 12 || pdgCode == 14 || pdgCode == 16 ||
            pdgCode == -12 || pdgCode == -14 || pdgCode == -16);
}

bool isTauNeutrino(int pdgCode) {
    return ( pdgCode == 16 );
}

bool isDecayNuWithTauNeutrino(int pdgCode) {
    return ( pdgCode == -12 || pdgCode == -14 );
}

bool isTauAntiNeutrino(int pdgCode) {
    return ( pdgCode == -16 );
}

bool isDecayNuWithTauAntiNeutrino(int pdgCode) {
    return ( pdgCode == 12 || pdgCode == 14 );
}


int CountTauNeutrinos(const std::vector<Particle>& particles) {

// Figure out whether we have a leptonically decaying tau- -> l- nu_bar nu_tau
// We only check whether we find the nu_tau and the nu_bar with the same parent

    int neutrinoCount = 0;
    
    for (const auto& p1 : particles) {
        if (isTauNeutrino(p1.pdgCode) && p1.status==1) {
            for (const auto& p2 : particles) {
                 if (p2.id != p1.id && p2.mother1 == p1.mother1){
                     if (isDecayNuWithTauNeutrino(p2.pdgCode) && p2.status==1) {
                         neutrinoCount++;
                     } 
                 }
            }
        }
    }
    return neutrinoCount;
    
}

int CountTauAntiNeutrinos(const std::vector<Particle>& particles) {

// Figure out whether we have a leptonically decaying tau+ -> l+ nu nu_tau_bar
// We only check whether we find the nu_tau_bar and the nu with the same parent

    int neutrinoCount = 0;
    
    for (const auto& p1 : particles) {
        if (isTauAntiNeutrino(p1.pdgCode) && p1.status==1) {
            for (const auto& p2 : particles) {
                 if (p2.id != p1.id && p2.mother1 == p1.mother1){
                     if (isDecayNuWithTauAntiNeutrino(p2.pdgCode) && p2.status==1) {
                         neutrinoCount++;
                     } 
                 }
            }
        }
    }
    return neutrinoCount;
    
}



int countNeutrinos(const std::vector<Particle>& particles) {
    int neutrinoCount = 0;
    for (const auto& p : particles) {
        if (isNeutrino(p.pdgCode) && p.status==1) {
            neutrinoCount++;
        }
    }
    return neutrinoCount;
}

void processLHEFile(const std::string& inputFileName, const std::string& outputFileName, int requestedCount, int extraCount) {
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file: " << inputFileName << std::endl;
        return;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFileName << std::endl;
        return;
    }

    std::string line;
    std::vector<Particle> particles;
    int eventCount = 0;
    int eventsWithNeutrinos = 0;

    // Write the header to the output file (this assumes the header is part of the input LHE)
    while (std::getline(inputFile, line)) {
        if (line.find("<event>") != std::string::npos) {
            // Start of a new event, write it to the output
            // GWW outputFile << line << std::endl;  // Write the opening <event> tag
            break;
        } else if (line.find("<LesHouchesEvents") != std::string::npos) {
            // Write the LHE header to the output file
            outputFile << line << std::endl;
        } else if (line.find("<init>") != std::string::npos) {
            // Write the <init> block (this part contains metadata)
            outputFile << line << std::endl;
            while (std::getline(inputFile, line) && line.find("</init>") == std::string::npos) {
                outputFile << line << std::endl;
            }
            outputFile << line << std::endl;  // Write the closing </init> tag
        }
    }

    // Process each event
    while (std::getline(inputFile, line)) {
        if (line.empty() || line[0] == '#' || line[0] == '<') {
            // Skip comments or empty lines
            continue;
        }

        // First line of the event (the header line) - we do not process it for particles
        std::stringstream ss(line);
        int eventInfo[2];  // To store event-level info (like total energy, number of particles, etc.)
        for (int i = 0; i < 2; i++) {
            ss >> eventInfo[i];
        }
        double deventInfo[4];
        for (int i = 0; i < 4; i++) {
            ss >> deventInfo[i];
        }        
        
        // Prepare to process particle lines
        particles.clear();
        int lines = 0;

        // Read the particle lines for this event
        while (std::getline(inputFile, line) && line.find("<event>") == std::string::npos) {
            if (line.find("</event>") != std::string::npos) {
                // End of this event, check the neutrino count
                eventCount++;
                int tauLeptonicDecayCount = CountTauNeutrinos(particles) + CountTauAntiNeutrinos(particles);
//                int neutrinoCount = countNeutrinos(particles);
//                if (neutrinoCount >= requestedNuCount && neutrinoCount <= requestedNuCount + extraNuCount) {
                if (tauLeptonicDecayCount >= requestedCount && tauLeptonicDecayCount <= requestedCount + extraCount) {                
                    eventsWithNeutrinos++;
                    // Event passes the neutrino count condition, write it to the output file
                    outputFile << "<event>" << std::endl;  // Start of the event block
                    // Write the event-level info line (just as in the input)
                    outputFile << std::setw(3) << eventInfo[0] << " "
                               << std::setw(2) << eventInfo[1] << " "
                               << std::scientific << std::setprecision(10) << std::setw(13) << deventInfo[0] << " "
                               << std::setw(13) << deventInfo[1] << " "
                               << std::setw(13) << deventInfo[2] << " "
                               << std::setw(13) << deventInfo[3] << std::endl;
                    // Write particle lines for this event
                    for (const auto& p : particles) {
                        if (p.pdgCode != 0) { // Check to avoid invalid particles with pdgCode 0
                            outputFile << std::setw(5) << p.pdgCode << " " 
                                       << std::setw(1) << p.status << " " 
                                       << std::setw(1) << p.mother1 << " " 
                                       << std::setw(1) << p.mother2 << " " 
                                       << std::setw(1) << p.daughter1 << " " 
                                       << std::setw(1) << p.daughter2 << " "
                                       << std::scientific << std::setprecision(10) << std::setw(13) << p.px << " "
                                       << std::setw(13) << p.py << " "
                                       << std::setw(13) << p.pz << " "
                                       << std::setw(13) << p.energy << " "
                                       << std::setw(13) << p.mass << " "
                                       << std::setw(13) << p.charge << " "
                                       << std::setw(3) << p.colorFlow << std::endl;
                        }
                    }
                    outputFile << "</event>" << std::endl;  // End of the event block
                }
                break;  // Exit the event loop after processing this event
            }

            // Process each particle line
            std::stringstream particleStream(line);
            Particle particle;
            particleStream >> particle.pdgCode >> particle.status >> particle.mother1 >> particle.mother2 >>
                              particle.daughter1 >> particle.daughter2 >>
                              particle.px >> particle.py >> particle.pz >> particle.energy >>
                              particle.mass >> particle.charge >> particle.colorFlow;

            // Only add valid particles to the event (ignore any with pdgCode 0)
            if (particle.pdgCode != 0) {
                lines++;
                particle.id = lines;
                particles.push_back(particle);
            }
        }
    }

    inputFile.close();
    outputFile.close();
    
    // Print out diagnostics
    std::cout << "Processing complete." << std::endl;
    std::cout << "Total number of events processed: " << eventCount << std::endl;
    std::cout << "Number of events with requested neutrino count: " << eventsWithNeutrinos << std::endl;
}

int main(int argc, char** argv) {

    CLI::App app{"Skim the fully leptonic tau decays specifying the number of final state neutrinos"};  
        
    std::string inputLHEFile = "input.lhe";
    app.add_option("-i,--ifile", inputLHEFile, "Input LHE file (default: input.lhe)"); 
    
    std::string outputLHEFile = "output.lhe";
    app.add_option("-o,--ofile", outputLHEFile, "Output LHE file (default: output.lhe)");
    
    int requestedCount = 2;
    app.add_option("-c,--count", requestedCount, "Requested tau leptonic decay count (default: 2)"); 
    
    int extraCount = 0;
    app.add_option("-e,--extra", extraCount, "Allowed extra tau leptonic decays (default: 0)");    
       
    CLI11_PARSE(app, argc, argv);

    processLHEFile(inputLHEFile, outputLHEFile, requestedCount, extraCount);
    
    return 0;
}

