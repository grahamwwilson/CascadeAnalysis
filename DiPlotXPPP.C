// Function to calculate the uncertainty of the histogram integral
double GetIntegralError(TH1D* hist) {

    double errorsq = 0.0;
    
    for (int i = 0; i <= hist->GetNbinsX() + 1; ++i) { // Include underflow (bin 0) and overflow (bin N+1) in calculation
        double binError = hist->GetBinError(i); // Error for each bin
//        cout << "Bin " << i << " error " << binError << endl;
        errorsq += binError * binError; // Add the squared error
    }
    
    return std::sqrt(errorsq); // Return the total uncertainty
}

// Function to add histogram with its integral and uncertainty to the legend
void AddHistogramToLegend(TLegend* legend, TH1D* hist, const std::string& label) {
    // Get the integral and its uncertainty for the histogram
    double integral = hist->Integral();
    double uncertainty = GetIntegralError(hist);
    
//    cout << " Found integral and uncertainty of " << integral << " " << uncertainty << endl;
    
    // Add the entry to the legend, formatted with the label, integral, and uncertainty
    legend->AddEntry(hist, Form("%s [%.2f#pm%.2f]", label.c_str(), integral, uncertainty), "l");
}



void DiPlotXPPP(int which = 1, string MLSP="350-200-100", string htype="hnOSSF",float ymax=1.0e8, float ymin=0.01, int mode=0, float xlmin=0.15, float ylmin=0.58){

gStyle->SetOptStat(" ");   // 1210
gStyle->SetStatFormat("9.7g");

TCanvas *c1 = new TCanvas("c1","multipads",800,600);

string hsignal = "Dummy.root";
if(which==1){
   hsignal="histos_ATLAS-" + MLSP + ".root";
}
else{
   hsignal = "histos_Sleptons-LSP" + MLSP + ".root";
}  

//hsignal="histos_SMNew.root";

cout << "Using signal histogram set " << hsignal << endl;

// Let's rationalize the color choice
//std::vector<int> colors = {kBlue, kRed, kTeal, kMagenta, kBlack, kGreen, kCyan, kGray, kOrange, kViolet, kPink};
std::vector<int> colors = {kAzure, kBlack, kRed, kMagenta,  kTeal+4, kGreen, kAzure+10, kPink-4, kOrange+1, kViolet+5, kGray+1, kRed-1};
for (int color : colors) {
    std::cout << "Color: " << color << std::endl;
}

TFile *f0 = new TFile(hsignal.c_str());
TFile *f1 = new TFile("histos_SM.root");
TFile *f2 = new TFile("histos_WZ.root");
TFile *f3 = new TFile("histos_ZZ.root");
TFile *f4 = new TFile("histos_WWW.root");
TFile *f5 = new TFile("histos_WWZ.root");
TFile *f6 = new TFile("histos_WZZ.root");
TFile *f7 = new TFile("histos_ZZZ.root");
TFile *f8 = new TFile("histos_Run-60.root");
TFile *f9 = new TFile("histos_Run-61.root");
TFile *f10 = new TFile("histos_SundryTaus.root");
TFile *f11 = new TFile("histos_HadTaus.root");

TH1D * hSignal = (TH1D*)f0->Get(htype.c_str());
TH1D * hBkgd = (TH1D*)f1->Get(htype.c_str());
TH1D * hWZ = (TH1D*)f2->Get(htype.c_str());
TH1D * hZZ = (TH1D*)f3->Get(htype.c_str());
TH1D * hWWW = (TH1D*)f4->Get(htype.c_str());
TH1D * hWWZ = (TH1D*)f5->Get(htype.c_str());
TH1D * hWZZ = (TH1D*)f6->Get(htype.c_str());
TH1D * hZZZ = (TH1D*)f7->Get(htype.c_str());
TH1D * hWZP = (TH1D*)f8->Get(htype.c_str());
TH1D * hLLTT = (TH1D*)f9->Get(htype.c_str());
TH1D * hSundry = (TH1D*)f10->Get(htype.c_str());
TH1D * hLLLT = (TH1D*)f11->Get(htype.c_str());

hSignal->GetYaxis()->SetTitleOffset(1.4);
hSignal->SetMaximum(ymax);
hSignal->SetMinimum(ymin);

c1->SetTicks(1,1);
c1->SetGrid(1,1);
if(mode!=1)c1->SetLogy();

hSignal->SetTitle("Generator Level Study (Whizard) 400 inv fb");
hSignal->SetLineColor(colors[0]);
hSignal->SetLineWidth(4);
hSignal->GetYaxis()->SetTitle("Events per bin");
hSignal->Draw("hist");
gPad->Update();
hSignal->Draw("hist");

hBkgd->SetLineColor(colors[1]);
hBkgd->SetLineWidth(3);
hBkgd->Draw("histsames");
gPad->Update();
hBkgd->Draw("histsames");

hWZ->SetLineColor(colors[2]);
hWZ->SetLineWidth(3);
hWZ->Draw("histsames");
gPad->Update();
hWZ->Draw("histsames");

hZZ->SetLineColor(colors[3]);
hZZ->SetLineWidth(3);
hZZ->Draw("histsames");
gPad->Update();
hZZ->Draw("histsames");

hWZP->SetLineColor(colors[8]);
hWZP->SetLineWidth(3);
hWZP->Draw("histsames");
gPad->Update();
hWZP->Draw("histsames");

hLLTT->SetLineColor(colors[9]);
hLLTT->SetLineWidth(3);
hLLTT->Draw("histsames");
gPad->Update();
hLLTT->Draw("histsames");

hSundry->SetLineColor(colors[10]);
hSundry->SetLineWidth(3);
hSundry->Draw("histsames");
gPad->Update();
hSundry->Draw("histsames");

hLLLT->SetLineColor(colors[11]);
hLLLT->SetLineWidth(3);
hLLLT->Draw("histsames");
gPad->Update();
hLLLT->Draw("histsames");

hWWW->SetLineColor(colors[4]);
hWWW->SetLineWidth(3);
hWWW->Draw("histsames");
gPad->Update();
hWWW->Draw("histsames");

hWWZ->SetLineColor(colors[5]);
hWWZ->SetLineWidth(3);
hWWZ->Draw("histsames");
gPad->Update();
hWWZ->Draw("histsames");

hWZZ->SetLineColor(colors[6]);
hWZZ->SetLineWidth(3);
hWZZ->Draw("histsames");
gPad->Update();
hWZZ->Draw("histsames");

hZZZ->SetLineColor(colors[7]);
hZZZ->SetLineWidth(3);
hZZZ->Draw("histsames");
gPad->Update();
hZZZ->Draw("histsames");

hBkgd->Draw("histsames");
gPad->Update();

// Also need a legend ...
const float dx=0.3;
const float dy=0.26;

TLegend* leg = new TLegend(xlmin,ylmin,xlmin+dx,ylmin+dy);
leg->SetTextFont(42);
leg->SetTextSize(0.024);
string signalstr;
if (MLSP=="90")signalstr = "(130/115/110/100/90)";
if (MLSP=="260")signalstr = "(300/289/280/270/260)";
if (MLSP=="270")signalstr = "(300/289/280/275/270)";
if (MLSP=="220")signalstr = "(300/289/260/240/220)";
if (MLSP=="250-150-100")signalstr = "(ATLAS 250/150/100)";
if (MLSP=="300-200-100")signalstr = "(ATLAS 300/200/100)";
if (MLSP=="350-200-100")signalstr = "(ATLAS 350/200/100)";
if (MLSP=="400-200-100")signalstr = "(ATLAS 400/200/100)";
if (MLSP=="450-200-100")signalstr = "(ATLAS 450/200/100)";
if (MLSP=="550-200-100")signalstr = "(ATLAS 550/200/100)";



// Add histograms to the legend using the helper function
AddHistogramToLegend(leg, hSignal, signalstr);

AddHistogramToLegend(leg, hBkgd, "Leptonic SM:Total");
AddHistogramToLegend(leg, hWZ, "Leptonic SM:WZ");
AddHistogramToLegend(leg, hZZ, "Leptonic SM:ZZ");

AddHistogramToLegend(leg, hWZP,"WZ to l^{#pm}#tau^{+}_{l}#tau^{-}_{l}");
AddHistogramToLegend(leg, hLLTT,"ZZ to l^{+}l^{-}#tau^{+}_{l}#tau^{-}_{l}");
AddHistogramToLegend(leg, hSundry,"Other WZ/ZZ with #tau_{l}");
AddHistogramToLegend(leg, hLLLT,"ZZ with 1 #tau_{h}");

AddHistogramToLegend(leg, hWWW, "Leptonic SM:WWW");
AddHistogramToLegend(leg, hWWZ, "Leptonic SM:WWZ");
AddHistogramToLegend(leg, hWZZ, "Leptonic SM:WZZ");
AddHistogramToLegend(leg, hZZZ, "Leptonic SM:ZZZ");

leg->SetBorderSize(1);                          // Include border
leg->Draw();
//c1->Print("Plot1.pdf");
}
