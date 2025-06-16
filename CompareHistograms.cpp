#include <TFile.h>
#include <TH1.h>
#include <TKey.h>
#include <TClass.h>
#include <TDirectory.h>
#include <TString.h>
#include <iostream>
#include <map>
#include <memory>

bool CompareHistograms(const TH1* h1, const TH1* h2, double tol = 1e-12) {
    if (!h1 || !h2) return false;
    if (h1->GetDimension() != h2->GetDimension()) return false;
    if (h1->GetNbinsX() != h2->GetNbinsX() || 
        h1->GetNbinsY() != h2->GetNbinsY() || 
        h1->GetNbinsZ() != h2->GetNbinsZ()) return false;

    bool same = true;

    for (int x = 0; x <= h1->GetNbinsX() + 1; ++x) {
        for (int y = 0; y <= (h1->GetDimension() >= 2 ? h1->GetNbinsY() + 1 : 0); ++y) {
            for (int z = 0; z <= (h1->GetDimension() == 3 ? h1->GetNbinsZ() + 1 : 0); ++z) {
                int bin = h1->GetBin(x, y, z);
                double c1 = h1->GetBinContent(bin);
                double c2 = h2->GetBinContent(bin);
                double e1 = h1->GetBinError(bin);
                double e2 = h2->GetBinError(bin);
                if (std::abs(c1 - c2) > tol || std::abs(e1 - e2) > tol) {
                    std::cout << "Difference at bin (" << x << "," << y << "," << z << "): "
                              << "content1 = " << c1 << ", content2 = " << c2
                              << ", error1 = " << e1 << ", error2 = " << e2 << "\n";
                    same = false;
                }
            }
        }
    }

    return same;
}

void ScanDirectory(TDirectory* dir, std::map<std::string, TObject*>& outMap, const std::string& path = "") {
    TIter next(dir->GetListOfKeys());
    TKey* key;
    while ((key = (TKey*)next())) {
        TObject* obj = key->ReadObj();
        std::string fullName = path + "/" + obj->GetName();
        if (obj->InheritsFrom("TDirectory")) {
            ScanDirectory((TDirectory*)obj, outMap, fullName);
        } else if (obj->InheritsFrom("TH1")) {
            outMap[fullName] = obj;
        }
    }
}

int main() {

   // 🔒 Hardcoded ROOT file names
    const std::string file1Name = "Test/histos_SM_python.root";
    const std::string file2Name = "Test/histos_SM_cpp.root";

    std::unique_ptr<TFile> f1(TFile::Open(file1Name.c_str()));
    std::unique_ptr<TFile> f2(TFile::Open(file2Name.c_str()));

    if (!f1 || f1->IsZombie() || !f2 || f2->IsZombie()) {
        std::cerr << "Error opening one of the files.\n";
        return 1;
    }

    std::map<std::string, TObject*> hists1, hists2;
    ScanDirectory(f1.get(), hists1);
    ScanDirectory(f2.get(), hists2);

    std::cout << "Found " << hists1.size() << " histograms in file 1, "
              << hists2.size() << " in file 2.\n";

    bool allMatch = true;
    for (const auto& [name, obj1] : hists1) {
        auto it = hists2.find(name);
        if (it == hists2.end()) {
            std::cout << "Missing in file2: " << name << "\n";
            allMatch = false;
            continue;
        }

// Note. CompareHistograms is now generalized. Works for TH1 and TH2 as TH2 inherits from TH1.

        auto* h1 = dynamic_cast<TH1*>(obj1);
        auto* h2 = dynamic_cast<TH1*>(it->second);
        if (!CompareHistograms(h1, h2)) {
            std::cout << "Mismatch in histogram: " << name << " " << h1->GetEntries() << " " << h2->GetEntries() << "\n";
            allMatch = false;
        }
        else{
            std::cout << "Histograms consistent: " << name << " " << h1->GetEntries() << " " << h2->GetEntries() << "\n";
        }

    }

    for (const auto& [name, _] : hists2) {
        if (hists1.find(name) == hists1.end()) {
            std::cout << "Missing in file1: " << name << "\n";
            allMatch = false;
        }
    }

    if (allMatch) {
        std::cout << "All histograms match.\n";
        return 0;
    } else {
        std::cout << "There were differences in histograms.\n";
        return 2;
    }
}


// g++ CompareHistograms.cpp $(root-config --cflags --libs) -o CompareHistograms
