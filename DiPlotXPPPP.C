
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
    double integral = hist->Integral(0,hist->GetNbinsX() + 1);
    double uncertainty = GetIntegralError(hist);
    
//    cout << " Found integral and uncertainty of " << integral << " " << uncertainty << endl;
    
    // Add the entry to the legend, formatted with the label, integral, and uncertainty
    legend->AddEntry(hist, Form("%s [%.2f#pm%.2f]", label.c_str(), integral, uncertainty), "l");
}

std::pair<double,double> GetIntegral(TH1D* hist) {
 
    double integral = hist->Integral(0,hist->GetNbinsX() + 1);
    double uncertainty = GetIntegralError(hist);      
   
    return std::make_pair(integral, uncertainty);
}

// Function to draw a TPaveText with PT label and FOM values
void AddFOMBox(const std::string& ptLabel, double S, double FOM1, double FOM2, double FOM3, 
               float x1 = 0.15, float y1 = 0.67, float x2 = 0.37, float y2 = 0.86) {
    
    // Format FOM values into strings
    char buf1[100], buf2[100], buf3[100];
    sprintf(buf1, "S = %.2f, S/B = %.2f", S, FOM1);
    sprintf(buf2, "S/#sqrt{B} = %.2f", FOM2);
    sprintf(buf3, "med[Z_{0}^{A} |#mu=1] = %.2f#sigma", FOM3); 

    // Create and configure TPaveText
    TPaveText* ptbox = new TPaveText(x1, y1, x2, y2, "NDC");
    ptbox->SetFillColor(0);
    ptbox->SetTextAlign(12);  // Left-aligned text
    ptbox->SetTextFont(42);
    ptbox->SetTextSize(0.03);
    ptbox->SetBorderSize(1);

    // Add text lines
    ptbox->AddText(ptLabel.c_str());
    ptbox->AddText(buf1);
    ptbox->AddText(buf2);
    ptbox->AddText(buf3);    

    // Draw the box
    ptbox->Draw();
}

void DiPlotXPPPP(int labelchoice = 0, int which = 1, string MLSP="350-200-100", string htype="hnOSSF",float ymax=1.0e10, float ymin=0.001, int mode=0, float xlmin=0.45, float ylmin=0.50){

// Vector of PT labels
std::vector<std::string> ptLabels = {"p_{T} Cuts (2, 3 GeV)", "p_{T} Cuts (5 GeV)", "p_{T} Cuts (7.5 GeV)", "p_{T} Cuts (10 GeV)"};

// Ensure labelchoice is in bounds
std::string ptLabel = (labelchoice >= 0 && labelchoice < ptLabels.size()) ? ptLabels[labelchoice] : "UNKNOWN";

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
std::vector<int> colors = {kAzure, kBlack, kRed, kMagenta,  kTeal+4, kGreen, kAzure+10, kPink-4, kOrange+1, kViolet+5, kGray+1, kRed-1, kTeal};
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
TFile *f12 = new TFile("histos_New4l.root");

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
TH1D * hOther = (TH1D*)f12->Get(htype.c_str());

hSignal->GetYaxis()->SetTitleOffset(1.4);
hSignal->SetMaximum(ymax);
hSignal->SetMinimum(ymin);

c1->SetTicks(1,1);
c1->SetGrid(1,1);
if(mode!=1)c1->SetLogy();

hSignal->SetTitle("Generator Level Study (Whizard) 400 fb^{-1}");
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

hOther->SetLineColor(colors[12]);
hOther->SetLineWidth(3);
hOther->Draw("histsames");
gPad->Update();
hOther->Draw("histsames");

hBkgd->Draw("histsames");
gPad->Update();

// Also need a legend ...
const float dx=0.38;
const float dy=0.34;

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
AddHistogramToLegend(leg, hOther, "Leptonic SM:WW/DY");
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

std::pair<double, double> pSignal = GetIntegral(hSignal);
std::pair<double, double> pBkgd = GetIntegral(hBkgd);
double S = pSignal.first;
double B = pBkgd.first;
double FOM1 = S/B;
double FOM2 = S/std::sqrt(B);
// Cowan, Cranmer, Gross, Vitells, asymptotic discovery significance, Eqn 97 in 1007.1727.
double FOM3 = std::sqrt(2.0*( (S+B)*std::log(1.0 + (S/B)) - S ));
std::cout << "Signal      : " << pSignal.first << " +- " << pSignal.second << std::endl;
std::cout << "Bkgd        : " << pBkgd.first << " +- " << pBkgd.second << std:: endl;
std::cout << "S/B         : " << FOM1 << std::endl;
std::cout << "S/sqrt(B)   : " << FOM2 << std::endl;
std::cout << "Z Asimov    : " << FOM3 << std::endl;

// Draw FOM box
AddFOMBox(ptLabel, S, FOM1, FOM2, FOM3);

leg->SetBorderSize(1);                          // Include border
leg->Draw();
//c1->Print("Plot1.pdf");
}
