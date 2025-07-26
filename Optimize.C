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
        // Use entries in last bin for uncertainty
        auto bin = hist->GetNbinsX();
        integral    = hist->GetBinContent(bin);
        uncertainty = hist->GetBinError(bin);
    }
    else if (std::string(hist->GetName()) == "hTriTrigger"  || std::string(hist->GetName()) == "hQuadTrigger") {
        // Use entries in first bin for uncertainty
        auto bin = 1;
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
               float x1 = 0.17, float y1 = 0.67, float x2 = 0.42, float y2 = 0.86) {
    
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

// Function to draw a TPaveText with PT label and FOM values
void AddFOMBox2(const std::string& ptLabel, std::string signalstr, 
               float x1 = 0.12, float y1 = 0.67, float x2 = 0.42, float y2 = 0.86) {
    
    // Format FOM values into strings
/*    char buf1[100], buf2[100], buf3[100];
    sprintf(buf1, "S = %.2f, S/B = %.4f", S, FOM1);
    sprintf(buf2, "S/#sqrt{B} = %.2f, Z_{Bi}^{1%%} = %.2f", FOM2, FOM4);
    sprintf(buf3, "med[Z_{0}^{A} |#mu=1] = %.2f", FOM3); 
*/

    // Create and configure TPaveText
    TPaveText* ptbox = new TPaveText(x1, y1, x2, y2, "NDC");
    ptbox->SetFillColor(0);
    ptbox->SetTextAlign(12);  // Left-aligned text
    ptbox->SetTextFont(42);
    ptbox->SetTextSize(0.03);
    ptbox->SetBorderSize(1);

    std::string newstring = "Signal="+signalstr;
    std::string lumistring = "Scaled to 400 inv fb";

    // Add text lines
    ptbox->AddText(ptLabel.c_str());
    ptbox->AddText(lumistring.c_str());
    ptbox->AddText(newstring.c_str());
//    ptbox->AddText(buf1);
//    ptbox->AddText(buf2);
//    ptbox->AddText(buf3);    

    // Draw the box
    ptbox->Draw();
}

void Optimize(string htype="hmaxSIP3D", double ymin=1.9, double ymax=2.4, string MLSP="220", int labelchoice = 3, int which = 0, int mode=1, double xlmin=0.45, double ylmin=0.54){

// Vector of PT labels
//std::vector<std::string> ptLabels = {"p_{T} Cuts (2, 3 GeV)", "p_{T} Cuts (5 GeV)", "p_{T} Cuts (7.5 GeV)", "p_{T} Cuts (10 GeV)"};

std::vector<std::string> ptLabels = {"2l selection", "3l selection (MLL<35 GeV)", "4l selection (MLL<35 GeV)", "3l selection (MLL<65 GeV)", "4l selection (MLL<65 GeV)"};

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

TH1D * hSignal = (TH1D*)f0->Get(htype.c_str());
TH1D * hBkgd = (TH1D*)f1->Get(htype.c_str());

hSignal->GetYaxis()->SetTitleOffset(0.1);
hSignal->SetMaximum(ymax);
hSignal->SetMinimum(ymin);

c1->SetTicks(1,1);
c1->SetGrid(1,1);
if(mode!=1)c1->SetLogy();

hSignal->SetTitle("Reco Level Study 400 fb^{-1}");
hSignal->SetLineColor(colors[0]);
hSignal->SetLineWidth(4);
hSignal->GetXaxis()->SetTitle("Cut on maxSIP3D");
hSignal->GetYaxis()->SetTitle("Figure of Merit (S/#sqrt{B})");
hSignal->Draw("hist");
gPad->Update();
hSignal->Draw("hist");

hBkgd->SetLineColor(colors[1]);
hBkgd->SetLineWidth(3);
hBkgd->Draw("histsames");
gPad->Update();
hBkgd->Draw("histsames");

// Make a copy of the original histogram but with the bins shifted to the right by half a bin width.
// This is to ensure that the middle of the new bin corresponds to the cut value.
int nbins = hSignal->GetNbinsX();
double xmin = hSignal->GetXaxis()->GetXmin();
double xmax = hSignal->GetXaxis()->GetXmax();
double bw = (xmax - xmin)/double(nbins);

TH1D* hFOM = new TH1D("hFOM","Reco level cut optimization; Cut on maxSIP3D; Figure of merit (S/#sqrt{B})",nbins, xmin+0.5*bw, xmax+0.5*bw);

// Loop through a range of cut values and fill the new histogram with the chosen FOM.

for (int icut = 10; icut <= hSignal->GetNbinsX() + 1; ++icut) { // Potentially include underflow (bin 0) and overflow (bin N+1) in calculation
    double errS = 0.0;  double errB = 0.0;
    double S = hSignal->IntegralAndError(0, icut, errS);  
    double B = hBkgd->IntegralAndError(0, icut, errB);
    double FOM = S/std::sqrt(B);
    double errFOM = FOM*std::sqrt( std::pow(errS/S, 2) + std::pow (0.5*errB/B, 2) );
    double bc = hFOM->GetBinCenter(icut);
    std::cout << "Cut bin " << std::setw(3) << icut << std::fixed << std::setprecision(1) << " " << bc << "  " 
                                                    << std::setprecision(3) << S << " +- " << errS << "  " 
                                                    << std::setprecision(2) << B << " +- " << errB << "  " 
                                                    << std::setprecision(4) << FOM << " +- " << errFOM << std::endl;
    errFOM = errFOM/10.0;             // fudge factor
    hFOM->SetBinContent(icut, FOM);
    hFOM->SetBinError(icut, errFOM);       
}

hFOM->GetYaxis()->SetTitleOffset(1.4);
hFOM->SetMinimum(ymin);
hFOM->SetMaximum(ymax);
hFOM->SetLineColor(colors[0]);
hFOM->SetLineWidth(3);
hFOM->Draw("histe");

gPad->Update();
gPad->SetTopMargin(0.075);
gPad->Update();


/*
// Also need a legend ...
const float dx=0.38;
const float dy=0.34;

TLegend* leg = new TLegend(xlmin,ylmin,xlmin+dx,ylmin+dy);
leg->SetTextFont(42);
leg->SetTextSize(0.024);
*/
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

/*
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
*/
AddFOMBox2(ptLabel, signalstr);

//hFOM->Fit("pol2");
//hFOM->Draw("ehist");

/*
leg->SetBorderSize(1);                          // Include border
leg->Draw();
*/
c1->Print("Plot.png");
}
