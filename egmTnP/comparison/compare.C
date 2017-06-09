void compare(TString axisType="pt", TString BinNum="0", TString DataType="B2H",TString region="all") // pt => BinNum:0-4 / eta => BinNum:0-5
{

double L = 0.0913; // for left margin
double R = 0.09188888888; // for right margin
int N = 1001; // for fill style
TString Lumi = "";
double lumi=0;
double effiMax=1.27;
double effiMin=0.18;
double sfMax=1.02;
double sfMin=0.98;
int MS=1;


// Setting Luminoity //
if( DataType == "B"){ Lumi = "5.788"; lumi = 5788.0; }
else if( DataType == "C"){ Lumi = "2.573"; lumi = 2573.0; }
else if( DataType == "D"){ Lumi = "4.248"; lumi = 4248.0; }
else if( DataType == "E"){ Lumi = "4.009"; lumi = 4009.0; }
else if( DataType == "F"){ Lumi = "3.102"; lumi = 3102.0; }
else if( DataType == "G"){ Lumi = "7.540"; lumi = 7540.0; }
else if( DataType == "H"){ Lumi = "8.606"; lumi = 8606.0; }
else if( DataType == "B2H"){ Lumi = "35.867"; lumi = 35867.0; }
else if( DataType == "B2F"){ Lumi = "19.721"; lumi = 19721.0; }
else if( DataType == "G2H"){ Lumi = "16.146"; lumi = 16146.0; }
else cout << "ERROR: DataType is wrong!" << endl;

        Double_t ptbin[7];
        ptbin[0] = 10;
        ptbin[1] = 20;
        ptbin[2] = 35;
        ptbin[3] = 50;
        ptbin[4] = 90;
        ptbin[5] = 150;
        ptbin[6] = 500;

        Double_t etabin[11];
        etabin[0] = -2.5;
        etabin[1] = -2.0;
        etabin[2] = -1.566;
        etabin[3] = -1.4442;
        etabin[4] = -0.8;
        etabin[5] = 0.0;
        etabin[6] = 0.8;
        etabin[7] = 1.4442;
        etabin[8] = 1.566;
        etabin[9] = 2.0;
        etabin[10] = 2.5;

///////////////////////////////////////////////////////////////////

TString Baselocation = "./root/";

//TString inputfile = Baselocation+"EleId_efficiency_"+BinNum+".root";
TString inputfile = Baselocation+"EleId_efficiency_my_final_"+BinNum+".root";

TFile f_input(inputfile, "read");
TH1D *h_AR = (TH1D*)f_input.Get("h_"+axisType+"_AR");
TH1D *h_DM = (TH1D*)f_input.Get("h_"+axisType+"_DM");

///////////////////////////////////////////////////////////////////

   TCanvas *Mass1 = new TCanvas("Mass1", "",65,24,1855,1056);
   Mass1->Range(-358.125,-1466709,3373.125,1.320038e+07);
   Mass1->SetFillColor(0);
   Mass1->SetBorderMode(0);
   Mass1->SetBorderSize(2);
   Mass1->SetFrameBorderMode(0);

//////////-- Pad1 --///////////////////////////////////////////////////////////////////////////////////////////

// ------------>Primitives in pad: InvariantMass
   TPad *InvariantMass = new TPad("InvariantMass", "",0.01,0.01,0.99,0.99);
   InvariantMass->Draw();
   InvariantMass->cd();
   InvariantMass->Range(-358.125,-2.353834,3373.125,8.453341);
   InvariantMass->SetFillColor(0);
   InvariantMass->SetBorderMode(0);
   InvariantMass->SetBorderSize(2);
//   InvariantMass->SetLogy();
   InvariantMass->SetGridx();
   InvariantMass->SetGridy();
   InvariantMass->SetBottomMargin(0.3130424);
   InvariantMass->SetFrameBorderMode(0);
   if( axisType == "pt" ) InvariantMass->SetLogx();

////////////////////////////////////////////////////////////////////////////////////////////

   h_AR->SetDirectory(0);
   h_AR->SetStats(0);

   h_AR->GetXaxis()->SetLabelFont(42);
   h_AR->GetXaxis()->SetLabelSize(0);
   h_AR->GetXaxis()->SetTitleSize(0.035);
   h_AR->GetXaxis()->SetTitleFont(42);
   h_AR->GetYaxis()->SetLabelFont(42);
   h_AR->GetYaxis()->SetLabelSize(0.035);
   h_AR->GetYaxis()->SetTitleSize(0.045);
   h_AR->GetYaxis()->SetTitleFont(42);
   h_AR->GetYaxis()->SetTitle("Data efficiency");
   h_AR->GetYaxis()->SetTitleOffset(0.8);
//   h_AR->GetXaxis()->SetRangeUser(50,3000);

   h_AR->SetTitle("");
   h_AR->SetMaximum(effiMax);
   h_AR->SetMinimum(effiMin);
   h_AR->SetMarkerStyle(21);
   h_AR->SetMarkerColor(1);
   h_AR->SetMarkerSize(MS);
   h_AR->SetLineColor(1);
   h_AR->SetLineWidth(1);

   h_AR->Draw("e p");

//   h_data->GetXaxis()->SetRangeUser(50,3000);
   h_DM->SetTitle("");
   h_DM->SetMaximum(effiMax);
   h_DM->SetMinimum(effiMin);
   h_DM->SetMarkerStyle(20);
   h_DM->SetMarkerColor(2);
   h_DM->SetMarkerSize(MS);
   h_DM->SetLineColor(2);
   h_DM->SetLineWidth(1);

   h_DM->Draw("e same p");

////////////////////////////////////////////////////////////////////////

   TLegend *leg = new TLegend(0.77,0.75,0.9,0.9,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(N);

   TLegendEntry *entry=leg->AddEntry("","AR","lpf");
   entry->SetFillStyle(N);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(2);
   entry->SetTextFont(42);

   entry=leg->AddEntry("","DM","lpf");
   entry->SetFillStyle(N);
   entry->SetMarkerColor(2);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(2);
   entry->SetTextFont(42);

   leg->Draw();

///////////////////////////////////////////////////////////////////////////

   TLatex *   tex = new TLatex(0.74,0.91,"#font[42]{2016, 13TeV}");
   tex->SetNDC();
   tex->SetLineWidth(2);
   tex->Draw();

   tex = new TLatex(0.1,0.95,"#font[62]{CMS}");
   tex->SetNDC();
   tex->SetLineWidth(2);
   tex->Draw();

   tex = new TLatex(0.1,0.91,"#font[42]{#it{#scale[0.8]{Preliminary}}}");
   tex->SetNDC();
   tex->SetLineWidth(2);
   tex->Draw();

   tex = new TLatex(0.5,0.95,"#font[42]{#scale[0.7]{Run2016"+DataType+"}}");
   tex->SetNDC();
   tex->SetLineWidth(2);
   tex->Draw();

   tex = new TLatex(0.5,0.91,"#font[42]{#scale[0.7]{Luminosity = "+Lumi+" fb^{-1}}}");
   tex->SetNDC();
   tex->SetLineWidth(2);
   tex->Draw();

if( axisType == "pt" && BinNum == "0" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{0.000 #leq |#eta| #leq 0.800}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "pt" && BinNum == "1" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{0.800 #leq |#eta| #leq 1.444}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "pt" && BinNum == "2" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{1.444 #leq |#eta| #leq 1.566}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "pt" && BinNum == "3" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{1.566 #leq |#eta| #leq 2.000}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "pt" && BinNum == "4" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{2.000 #leq |#eta| #leq 2.500}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "eta" && BinNum == "0" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{10 #leq P_{T} #leq 20 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "eta" && BinNum == "1" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{20 #leq P_{T} #leq 35 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "eta" && BinNum == "2" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{35 #leq P_{T} #leq 50 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "eta" && BinNum == "3" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{50 #leq P_{T} #leq 90 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "eta" && BinNum == "4" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{90 #leq P_{T} #leq 150 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "eta" && BinNum == "5" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{150 #leq P_{T} #leq 500 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}

//////-- Pad2 --////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ------------>Primitives in pad: padc1_2
   TPad *padc1_2 = new TPad("padc1_2", "padc1_2",0.01,0.01,0.99,0.3);
   padc1_2->Draw();
   padc1_2->cd();
   padc1_2->Range(-318.6518,0.4645501,3335.804,1.256857);
   padc1_2->SetFillColor(0);
   padc1_2->SetBorderMode(0);
   padc1_2->SetBorderSize(2);
   padc1_2->SetGridx();
   padc1_2->SetGridy();
   padc1_2->SetTopMargin(0.07176078);
   padc1_2->SetBottomMargin(0.4233841);
   padc1_2->SetLeftMargin(L);
   padc1_2->SetRightMargin(R);
   padc1_2->SetFrameBorderMode(0);
   if( axisType == "pt" ) padc1_2->SetLogx();

//////-- raio plot --//////////////////////////////////////////////////////////////////////////////

   TH1D *ratio_ptbin = new TH1D("ratio_ptbin", "", 6, ptbin);
   TH1D *ratio_etabin = new TH1D("ratio_etabin", "", 10, etabin);

if( axisType == "pt" )
{
   ratio_ptbin->Divide(h_AR, h_DM, 1, 1);
   ratio_ptbin->SetMarkerStyle(20);
   ratio_ptbin->SetLineWidth(1);
   ratio_ptbin->GetXaxis()->SetLabelFont(42);
   ratio_ptbin->GetXaxis()->SetLabelOffset(0.007);
   ratio_ptbin->GetXaxis()->SetLabelSize(0.12);
   ratio_ptbin->GetXaxis()->SetMoreLogLabels();
   ratio_ptbin->GetXaxis()->SetTitleSize(0.15);
   ratio_ptbin->GetXaxis()->SetTitleOffset(1.04);
   ratio_ptbin->GetXaxis()->SetTitleFont(42);
   ratio_ptbin->GetXaxis()->SetTitle("p_{T}  [GeV]");
   ratio_ptbin->GetYaxis()->SetTitle("AR/DM");
//   ratio_ptbin->GetYaxis()->SetRangeUser(0.7,1.3);
   ratio_ptbin->GetYaxis()->SetNdivisions(508);
   ratio_ptbin->GetYaxis()->SetLabelFont(42);
   ratio_ptbin->GetYaxis()->SetLabelSize(0.07);
   ratio_ptbin->GetYaxis()->SetTitleSize(0.15);
   ratio_ptbin->GetYaxis()->SetTitleOffset(0.2);
   ratio_ptbin->GetYaxis()->SetTitleFont(42);
   ratio_ptbin->SetStats(0);
   ratio_ptbin->SetMaximum(sfMax);
   ratio_ptbin->SetMinimum(sfMin);

   ratio_ptbin->Draw("e p");
}
else if( axisType == "eta" )
{
   ratio_etabin->Divide(h_AR, h_DM, 1, 1);
   ratio_etabin->SetMarkerStyle(20);
   ratio_etabin->SetLineWidth(1);
   ratio_etabin->GetXaxis()->SetLabelFont(42);
   ratio_etabin->GetXaxis()->SetLabelOffset(0.007);
   ratio_etabin->GetXaxis()->SetLabelSize(0.12);
   ratio_etabin->GetXaxis()->SetTitleSize(0.15);
   ratio_etabin->GetXaxis()->SetTitleOffset(1.04);
   ratio_etabin->GetXaxis()->SetTitleFont(42);
   ratio_etabin->GetXaxis()->SetTitle("SuperCluster #eta");
   ratio_etabin->GetYaxis()->SetTitle("AR/DM");
//   ratio_etabin->GetYaxis()->SetRangeUser(0.7,1.3);
   ratio_etabin->GetYaxis()->SetNdivisions(508);
   ratio_etabin->GetYaxis()->SetLabelFont(42);
   ratio_etabin->GetYaxis()->SetLabelSize(0.07);
   ratio_etabin->GetYaxis()->SetTitleSize(0.15);
   ratio_etabin->GetYaxis()->SetTitleOffset(0.2);
   ratio_etabin->GetYaxis()->SetTitleFont(42);
   ratio_etabin->SetStats(0);
   ratio_etabin->SetMaximum(sfMax);
   ratio_etabin->SetMinimum(sfMin);

   ratio_etabin->Draw("e p");
}


//////-- line in Pad2 --///////////////////////////////////////////////////////////////////////////////

   TF1 *f_line1 = new TF1("f_line","1",-10000,10000);
   f_line1->SetFillColor(19);
   f_line1->SetFillStyle(0);
   f_line1->SetLineColor(4);
   f_line1->SetLineWidth(1);
   f_line1->GetXaxis()->SetLabelFont(42);
   f_line1->GetXaxis()->SetLabelSize(0.035);
   f_line1->GetXaxis()->SetTitleSize(0.035);
   f_line1->GetXaxis()->SetTitleFont(42);
   f_line1->GetYaxis()->SetLabelFont(42);
   f_line1->GetYaxis()->SetLabelSize(0.035);
   f_line1->GetYaxis()->SetTitleSize(0.035);
   f_line1->GetYaxis()->SetTitleFont(42);
   f_line1->Draw("SAME");

//////-- Save modification --////////////////////////////////////////////////////////////////////////////////////////////////////////

   padc1_2->Modified();
   InvariantMass->cd();
   InvariantMass->Modified();
   Mass1->cd();
   Mass1->Modified();
   Mass1->cd();
   Mass1->SetSelected(Mass1);

/////////////////////////////////////////////////////////////////

//   TFile f_output("ROOTFile_EleId_comparison_"+axisType+"_"+BinNum+".root", "recreate");
//   Mass1->Write();
   Mass1->SaveAs("./plot/EleId_comparison_final_"+axisType+"_"+BinNum+".pdf");

}
