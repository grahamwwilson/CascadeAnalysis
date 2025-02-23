void DiPlotXP(string MLSP="90", string htype="hnOSSF",float ymax=1.0e8, float ymin=0.01, int mode=0, float xlmin=0.15, float ylmin=0.58){

gStyle->SetOptStat("i");   // 1210
gStyle->SetStatFormat("9.7g");

TCanvas *c1 = new TCanvas("c1","multipads",800,600);

string hsignal = "histos_Sleptons-LSP" + MLSP + ".root";
cout << "Using signal histogram set " << hsignal << endl;

// Let's rationalize the color choice
//std::vector<int> colors = {kBlue, kRed, kTeal, kMagenta, kBlack, kGreen, kCyan, kGray, kOrange, kViolet, kPink};
std::vector<int> colors = {kAzure, kBlack, kRed+1, kMagenta,  kTeal+4, kGreen, kAzure+10, kPink-4, kOrange+1, kViolet+5, kGray+1};
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

hSignal->GetYaxis()->SetTitleOffset(1.4);
hSignal->Sumw2(kFALSE);
hSignal->SetMaximum(ymax);
hSignal->SetMinimum(ymin);

c1->SetTicks(1,1);
c1->SetGrid(1,1);
if(mode!=1)c1->SetLogy();

hSignal->SetTitle("Generator Level Study (Whizard) 400 inv fb");
hSignal->SetLineColor(colors[0]);
hSignal->SetLineWidth(4);
hSignal->GetYaxis()->SetTitle("Events per bin");
hSignal->Draw("ehist");
gPad->Update();
TPaveStats *st0 = (TPaveStats*)hSignal->FindObject("stats");
st0->SetLineColor(colors[0]);
st0->SetTextColor(colors[0]);
hSignal->Draw("hist");

hBkgd->SetLineColor(colors[1]);
hBkgd->SetLineWidth(3);
hBkgd->Draw("sames");
hBkgd->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st1 = (TPaveStats*)hBkgd->FindObject("stats");
st1->SetLineColor(colors[1]);
st1->SetTextColor(colors[1]);
hBkgd->Draw("sames");

hWZ->SetLineColor(colors[2]);
hWZ->SetLineWidth(3);
hWZ->Draw("sames");
hWZ->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st2 = (TPaveStats*)hWZ->FindObject("stats");
st2->SetLineColor(colors[2]);
st2->SetTextColor(colors[2]);
hWZ->Draw("sames");

hZZ->SetLineColor(colors[3]);
hZZ->SetLineWidth(3);
hZZ->Draw("sames");
hZZ->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st3 = (TPaveStats*)hZZ->FindObject("stats");
st3->SetLineColor(colors[3]);
st3->SetTextColor(colors[3]);
hZZ->Draw("sames");

hWZP->SetLineColor(colors[8]);
hWZP->SetLineWidth(3);
hWZP->Draw("sames");
hWZP->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st8 = (TPaveStats*)hWZP->FindObject("stats");
st8->SetLineColor(colors[8]);
st8->SetTextColor(colors[8]);
hWZP->Draw("sames");

hLLTT->SetLineColor(colors[9]);
hLLTT->SetLineWidth(3);
hLLTT->Draw("sames");
hLLTT->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st9 = (TPaveStats*)hLLTT->FindObject("stats");
st9->SetLineColor(colors[9]);
st9->SetTextColor(colors[9]);
hLLTT->Draw("sames");

hSundry->SetLineColor(colors[10]);
hSundry->SetLineWidth(3);
hSundry->Draw("sames");
hSundry->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st10 = (TPaveStats*)hSundry->FindObject("stats");
st10->SetLineColor(colors[10]);
st10->SetTextColor(colors[10]);
hSundry->Draw("sames");

hWWW->SetLineColor(colors[4]);
hWWW->SetLineWidth(3);
hWWW->Draw("sames");
hWWW->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st4 = (TPaveStats*)hWWW->FindObject("stats");
st4->SetLineColor(colors[4]);
st4->SetTextColor(colors[4]);
hWWW->Draw("sames");

hWWZ->SetLineColor(colors[5]);
hWWZ->SetLineWidth(3);
hWWZ->Draw("sames");
hWWZ->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st5 = (TPaveStats*)hWWZ->FindObject("stats");
st5->SetLineColor(colors[5]);
st5->SetTextColor(colors[5]);
hWWZ->Draw("sames");

hWZZ->SetLineColor(colors[6]);
hWZZ->SetLineWidth(3);
hWZZ->Draw("sames");
hWZZ->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st6 = (TPaveStats*)hWZZ->FindObject("stats");
st6->SetLineColor(colors[6]);
st6->SetTextColor(colors[6]);
hWZZ->Draw("sames");

hZZZ->SetLineColor(colors[7]);
hZZZ->SetLineWidth(3);
hZZZ->Draw("sames");
hZZZ->Sumw2(kFALSE);
gPad->Update();
TPaveStats *st7 = (TPaveStats*)hZZZ->FindObject("stats");
st7->SetLineColor(colors[7]);
st7->SetTextColor(colors[7]);
hZZZ->Draw("sames");



hBkgd->Draw("sames");
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
leg->AddEntry(hSignal,signalstr.c_str(),"l");
leg->AddEntry(hBkgd,"Leptonic SM:Total","l");
leg->AddEntry(hWZ,"Leptonic SM:WZ","l");
leg->AddEntry(hZZ,"Leptonic SM:ZZ","l");
leg->AddEntry(hWZP,"WZ to l^{#pm}#tau^{+}_{l}#tau^{-}_{l}","l");
leg->AddEntry(hLLTT,"ZZ to l^{+}l^{-}#tau^{+}_{l}#tau^{-}_{l}","l");
leg->AddEntry(hSundry,"Other WZ/ZZ with #tau_{l}","l");
leg->AddEntry(hWWW,"Leptonic SM:WWW","l");
leg->AddEntry(hWWZ,"Leptonic SM:WWZ","l");
leg->AddEntry(hWZZ,"Leptonic SM:WZZ","l");
leg->AddEntry(hZZZ,"Leptonic SM:ZZZ","l");

leg->SetBorderSize(1);                          // Include border
leg->Draw();
}
