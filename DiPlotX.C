void DiPlotX(string MLSP="260", string htype="h3Code",float ymax=4.0e6, float ymin=20.0, int mode=0, float xlmin=0.15, float ylmin=0.58){

//gStyle->SetOptStat(111111);
// See https://root.cern/doc/v608/classTPaveStats.html
gStyle->SetOptStat("emi");   // 1210
gStyle->SetStatFormat("9.7g");

TCanvas *c1 = new TCanvas("c1","multipads",800,600);

string hsignal = "histos_Sleptons-LSP" + MLSP + ".root";
cout << "Using signal histogram set " << hsignal << endl;

TFile *f1 = new TFile(hsignal.c_str());
TFile *f2 = new TFile("histos_SM.root");
TFile *f3 = new TFile("histos_ZZ.root");
TFile *f4 = new TFile("histos_WZBoth.root");
TFile *f5 = new TFile("histos_WWW.root");

TH1D * hLRutype = (TH1D*)f1->Get(htype.c_str());
TH1D * hLRdtype = (TH1D*)f2->Get(htype.c_str());
TH1D * hWZ = (TH1D*)f4->Get(htype.c_str());
TH1D * hZZ = (TH1D*)f3->Get(htype.c_str());
TH1D * hWWW = (TH1D*)f5->Get(htype.c_str());

hLRutype->GetYaxis()->SetTitleOffset(1.4);
hLRutype->Sumw2(kFALSE);
hLRutype->SetMaximum(ymax);
hLRutype->SetMinimum(ymin);

c1->SetTicks(1,1);
c1->SetGrid(1,1);
if(mode!=1)c1->SetLogy();

hLRutype->SetTitle("Generator Level Study (Whizard) 400 inv fb");
hLRutype->SetLineColor(kBlue);
hLRutype->SetLineWidth(4);
//hLRutype->GetXaxis()->SetTitle("Beam Particle Energy [GeV]");
hLRutype->Draw("ehist");
gPad->Update();
TPaveStats *st1 = (TPaveStats*)hLRutype->FindObject("stats");
st1->SetLineColor(kBlue);
st1->SetTextColor(kBlue);
hLRutype->Draw("ehist");

hLRdtype->SetLineColor(kRed);
hLRdtype->SetLineWidth(3);
hLRdtype->Draw("sames");
hLRdtype->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st2 = (TPaveStats*)hLRdtype->FindObject("stats");
st2->SetLineColor(kRed);
st2->SetTextColor(kRed);
hLRdtype->Draw("sames");

hWZ->SetLineColor(kTeal);
hWZ->SetLineWidth(3);
hWZ->Draw("sames");
hWZ->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st4 = (TPaveStats*)hWZ->FindObject("stats");
st4->SetLineColor(kTeal);
st4->SetTextColor(kTeal);
hWZ->Draw("sames");

hZZ->SetLineColor(kMagenta);
hZZ->SetLineWidth(3);
hZZ->Draw("sames");
hZZ->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st3 = (TPaveStats*)hZZ->FindObject("stats");
st3->SetLineColor(kMagenta);
st3->SetTextColor(kMagenta);
hZZ->Draw("sames");

hWWW->SetLineColor(kBlack);
hWWW->SetLineWidth(3);
hWWW->Draw("sames");
hWWW->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st5 = (TPaveStats*)hWWW->FindObject("stats");
st5->SetLineColor(kBlack);
st5->SetTextColor(kBlack);
hWWW->Draw("sames");

hLRdtype->Draw("sames");
gPad->Update();

// Also need a legend ...
const float dx=0.42;
const float dy=0.21;

TLegend* leg = new TLegend(xlmin,ylmin,xlmin+dx,ylmin+dy);
leg->SetTextFont(42);
leg->SetTextSize(0.04);
string signalstr;
if (MLSP=="260")signalstr = "(300/289/280/270/260)*10";
if (MLSP=="270")signalstr = "(300/289/280/275/270)*10";
if (MLSP=="220")signalstr = "(300/289/260/240/220)*10";
leg->AddEntry(hLRutype,signalstr.c_str(),"l");
leg->AddEntry(hLRdtype,"Leptonic SM:WZ+ZZ+WWW","l");
leg->AddEntry(hWZ,"Leptonic SM:WZ","l");
leg->AddEntry(hZZ,"Leptonic SM:ZZ","l");
leg->AddEntry(hWWW,"Leptonic SM:WWW","l");

leg->SetBorderSize(1);                          // Include border
leg->Draw();
//c1->Print("Plot1.pdf");
}
