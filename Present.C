double zBi(double ns, double mub, double syst){
//  Based on 
//  @article{Cousins:2008zz,
//     author = "Cousins, Robert D. and Linnemann, James T. and Tucker, Jordan",
//     title = "{Evaluation of three methods for calculating 
//     statistical significance when incorporating a systematic uncertainty 
//     into a test of the background-only hypothesis for a Poisson process}",
//     eprint = "physics/0702156",
//     archivePrefix = "arXiv",
//     doi = "10.1016/j.nima.2008.07.086",
//     journal = "Nucl. Instrum. Meth. A",
//     volume = "595",
//     number = "2",
//     pages = "480--501",
//     year = "2008"

// ns  = Nsignal events
// mub = Background mean
// syst = Fractional systematic uncertainty on background mean
//
// See Appendix E for essentially this implementation
//
    double sb = syst*mub;
    double tau = mub/(sb*sb);
    double non  = mub + ns;
    double noff = tau*mub;

//    cout << "Calculating pBi for ns = " << ns << " mub= " << mub << " syst = " << syst << endl;
//    cout << "Using auxiliary parameters tau, non, and noff of " << tau << " " << non << " " << noff << endl;

    double pBi = TMath::BetaIncomplete(1.0/(1.0+tau), non, noff+1.0);
// And transform to single-sided z score.
    double zBi = sqrt(2.0)*TMath::ErfInverse(1.0 - 2.0*pBi);
//    cout << "Found pBi, zBi values of " << pBi << " " << zBi << endl;
    return zBi;
}

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

std::pair<double,double> GetIntegral(TH1D* hist) {
 
    double integral, uncertainty;
  
   // Check if the histogram is named "hCutFlow" or "hDiCutFlow" or "hQuadCutFlow"
    if (std::string(hist->GetName()) == "hCutFlow"  || std::string(hist->GetName()) == "hDiCutFlow"  || std::string(hist->GetName()) == "hQuadCutFlow") {
        // Use entries in last bin method for uncertainty
        auto bin = hist->GetNbinsX();
        integral    = hist->GetBinContent(bin);
        uncertainty = hist->GetBinError(bin);
    }
    else {
        // Use default methods
        integral = hist->Integral(0,hist->GetNbinsX() + 1);
        uncertainty = GetIntegralError(hist);
    } 
    return std::make_pair(integral, uncertainty);
}

// Function to add histogram with its integral and uncertainty to the legend
void AddHistogramToLegend(TLegend* legend, TH1D* hist, const std::string& label) {

    // Get the integral and its uncertainty for the histogram
    double integral;
    double uncertainty;

    auto p = GetIntegral(hist);

    integral = p.first;
    uncertainty = p.second;

//    cout << " Found integral and uncertainty of " << integral << " " << uncertainty << endl;
    
    // Add the entry to the legend, formatted with the label, integral, and uncertainty
    legend->AddEntry(hist, Form("%s [%.2f#pm%.2f]", label.c_str(), integral, uncertainty), "l");
}

// Function to draw a TPaveText with PT label and FOM values
void AddFOMBox(const std::string& ptLabel, double S, double FOM1, double FOM2, double FOM3, double FOM4, 
               float x1 = 0.12, float y1 = 0.67, float x2 = 0.37, float y2 = 0.86) {
    
    // Format FOM values into strings
    char buf1[100], buf2[100], buf3[100];
    sprintf(buf1, "S = %.2f, S/B = %.4f", S, FOM1);
    sprintf(buf2, "S/#sqrt{B} = %.2f, Z_{Bi}^{1%%} = %.2f", FOM2, FOM4);
    sprintf(buf3, "med[Z_{0}^{A} |#mu=1] = %.2f", FOM3); 

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

void Present(string htype="hDiCutFlow", float ymax=1.0e6, float ymin=0.1, string MLSP="260", int labelchoice = 1, int which = 0, int mode=0, float xlmin=0.45, float ylmin=0.54){

// Vector of PT labels
//std::vector<std::string> ptLabels = {"p_{T} Cuts (2, 3 GeV)", "p_{T} Cuts (5 GeV)", "p_{T} Cuts (7.5 GeV)", "p_{T} Cuts (10 GeV)"};

std::vector<std::string> ptLabels = {"2l selection", "3l selection", "4l selection"};

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
   hsignal = "SlepSnu-" + MLSP + "_histos.root";
}  

//hsignal="histos_SMNew.root";

cout << "Using signal histogram set " << hsignal << endl;

// Let's rationalize the color choice
//std::vector<int> colors = {kBlue, kRed, kTeal, kMagenta, kBlack, kGreen, kCyan, kGray, kOrange, kViolet, kPink};
std::vector<int> colors = {kAzure, kBlack, kRed, kMagenta,  kTeal+4, kGreen, kAzure+10, kPink-4, kOrange+1, kViolet+5};
for (int color : colors) {
    std::cout << "Color: " << color << std::endl;
}

TFile *f0 = new TFile(hsignal.c_str());
TFile *f1 = new TFile("Bkgd_histos.root");
TFile *f2 = new TFile("WZ_histos.root");
TFile *f3 = new TFile("ZZ_histos.root");
TFile *f4 = new TFile("WW_histos.root");
TFile *f5 = new TFile("VVV_histos.root");
TFile *f6 = new TFile("Hto4L_histos.root");
TFile *f7 = new TFile("ttbar_histos.root");
TFile *f8 = new TFile("DY_histos.root");
TFile *f9 = new TFile("WtoLNu_histos.root");

TH1D * hSignal = (TH1D*)f0->Get(htype.c_str());
TH1D * hBkgd = (TH1D*)f1->Get(htype.c_str());
TH1D * hWZ = (TH1D*)f2->Get(htype.c_str());
TH1D * hZZ = (TH1D*)f3->Get(htype.c_str());
TH1D * hWW = (TH1D*)f4->Get(htype.c_str());
TH1D * hVVV = (TH1D*)f5->Get(htype.c_str());
TH1D * hHiggs = (TH1D*)f6->Get(htype.c_str());
TH1D * htt = (TH1D*)f7->Get(htype.c_str());
TH1D * hDY = (TH1D*)f8->Get(htype.c_str());
TH1D * hW = (TH1D*)f9->Get(htype.c_str());

hSignal->GetYaxis()->SetTitleOffset(1.4);
hSignal->SetMaximum(ymax);
hSignal->SetMinimum(ymin);

c1->SetTicks(1,1);
c1->SetGrid(1,1);
if(mode!=1)c1->SetLogy();

hSignal->SetTitle("Reco Level Study 400 fb^{-1}");
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

hWW->SetLineColor(colors[4]);
hWW->SetLineWidth(3);
hWW->Draw("histsames");
gPad->Update();
hWW->Draw("histsames");

hVVV->SetLineColor(colors[5]);
hVVV->SetLineWidth(3);
hVVV->Draw("histsames");
gPad->Update();
hVVV->Draw("histsames");

hHiggs->SetLineColor(colors[6]);
hHiggs->SetLineWidth(3);
hHiggs->Draw("histsames");
gPad->Update();
hHiggs->Draw("histsames");

htt->SetLineColor(colors[7]);
htt->SetLineWidth(3);
htt->Draw("histsames");
gPad->Update();
htt->Draw("histsames");

hDY->SetLineColor(colors[8]);
hDY->SetLineWidth(3);
hDY->Draw("histsames");
gPad->Update();
hDY->Draw("histsames");

hW->SetLineColor(colors[9]);
hW->SetLineWidth(3);
hW->Draw("histsames");
gPad->Update();
hW->Draw("histsames");

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
if (MLSP=="180")signalstr = "(220/209/200/190/180)";
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

AddHistogramToLegend(leg, hBkgd, "Total Bkgd.");
AddHistogramToLegend(leg, hWZ, "WZ");
AddHistogramToLegend(leg, hZZ, "ZZ");
AddHistogramToLegend(leg, hWW, "WW");
AddHistogramToLegend(leg, hVVV, "VVV");
AddHistogramToLegend(leg, hHiggs, "Hto4l");
AddHistogramToLegend(leg, htt, "ttbar");
AddHistogramToLegend(leg, hDY, "DY");
AddHistogramToLegend(leg, hW, "W");

std::pair<double, double> pSignal = GetIntegral(hSignal);
std::pair<double, double> pBkgd = GetIntegral(hBkgd);
double S = pSignal.first;
double B = pBkgd.first;
double FOM1 = S/B;
double FOM2 = S/std::sqrt(B);
// Cowan, Cranmer, Gross, Vitells, asymptotic discovery significance, Eqn 97 in 1007.1727.
double FOM3 = std::sqrt(2.0*( (S+B)*std::log(1.0 + (S/B)) - S ));
double FOM4 = zBi(S, B, 0.01);   // Use 2% uncertainty for now
std::cout << "Signal      : " << pSignal.first << " +- " << pSignal.second << std::endl;
std::cout << "Bkgd        : " << pBkgd.first << " +- " << pBkgd.second << std:: endl;
std::cout << "S/B         : " << FOM1 << std::endl;
std::cout << "S/sqrt(B)   : " << FOM2 << std::endl;
std::cout << "Z Asimov    : " << FOM3 << std::endl;
std::cout << "Z Bi        : " << FOM4 << std::endl;

// Draw FOM box
AddFOMBox(ptLabel, S, FOM1, FOM2, FOM3, FOM4);

leg->SetBorderSize(1);                          // Include border
leg->Draw();
//c1->Print("Plot1.pdf");
}
