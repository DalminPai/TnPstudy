#include <TF1.h>
#include <TFile.h>
#include <TString.h>
#include <TGraphAsymmErrors.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TPad.h>

#include "./tdrstyle.C"
#include <vector>
#include <iostream>


void comparison(TString BinNum="0", TString axisType="eta") // BinNum : 0 ~ 4 ( pT range is limited to 150GeV. )
{
double ts = 0.03;

TString inputfile1 = "./egammaEffi.txt_EGM2D_my.root";
TString inputfile2 = "./egammaEffi.txt_EGM2D.root";

TFile f_input1(inputfile1, "read");
TGraphErrors *g_Type1 = (TGraphErrors*)f_input1.Get("grSF1D_"+BinNum);

TFile f_input2(inputfile2, "read");
TGraphErrors *g_Type2 = (TGraphErrors*)f_input2.Get("grSF1D_"+BinNum);


        ////////////////////////////////////
        // -- Make Type1 / Type2 Graph -- //
        ////////////////////////////////////
        Int_t nPoint_Type1 = g_Type1->GetN();
        Int_t nPoint_Type2 = g_Type2->GetN();
/*        if( nPoint_Type1 != nPoint_Type2)
        {
                printf("[# points in the graphs are different! ... ratio of the graph may not be calculated properly!]\n");
                printf("\t[Graph name] %s\n", GraphName_Type1.Data() );
                printf("\t[# points] (%s, %s) = (%d, %d)\n", Type1.Data(), Type2.Data(), nPoint_Type1, nPoint_Type2);
        }
*/

        TGraphErrors *g_ratio = (TGraphErrors*)g_Type1->Clone();
//        g_ratio->SetName( "SF_" + EffName );




        g_ratio->Set(0); //Remove all points (reset)

        Int_t NPoints = g_Type1->GetN();
        Int_t NPoints_2 = g_Type2->GetN();
        if( NPoints != NPoints_2 )
                printf("# points is different bewteen two graph...be careful for the ratio plot\n");

        for(Int_t i_p=0; i_p<NPoints; i_p++)
        {
                //Get Type1 point
                Double_t x_Type1, y_Type1, error_Type1;
                g_Type1->GetPoint(i_p, x_Type1, y_Type1);
                if( g_Type1->GetErrorYhigh(i_p) > g_Type1->GetErrorYlow(i_p) ) error_Type1 = g_Type1->GetErrorYhigh(i_p);
		else error_Type1 = g_Type1->GetErrorYlow(i_p);

                //Get Type2 point
                Double_t x_Type2, y_Type2, error_Type2;
                g_Type2->GetPoint(i_p, x_Type2, y_Type2);
                if( g_Type2->GetErrorYhigh(i_p) > g_Type2->GetErrorYlow(i_p) ) error_Type2 = g_Type2->GetErrorYhigh(i_p);
		else error_Type2 = g_Type2->GetErrorYlow(i_p);

                Double_t ratio;
                Double_t ratio_error;
		Double_t ratio_A, ratio_B, errorSquare; 
                if( (NPoints != NPoints_2) && i_p >= NPoints_2)
                {
                        ratio = 0;
                        ratio_error = 0;
                }
                else if(y_Type1 != 0 && error_Type1 != 0 && y_Type2 != 0 && error_Type2 != 0)
                {
                        ratio = y_Type1 / y_Type2;

		        ratio_A = (error_Type1) / y_Type1;
       			ratio_B = (error_Type2) / y_Type2;

		        errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;
	 	        ratio_error = (y_Type1/y_Type2) * sqrt(errorSquare);
                }
                else
                {
                        ratio = 0;
                        ratio_error = 0;
                }

                //Set Central value
                g_ratio->SetPoint(i_p, x_Type1, ratio);

                //Set the error
                Double_t error_X = g_Type1->GetErrorX(i_p);
                //Double_t error_XLow = g_Type1->GetErrorXlow(i_p);
                //Double_t error_Xhigh = g_Type1->GetErrorXhigh(i_p);
                g_ratio->SetPointError(i_p, error_X, ratio_error);

                // cout << endl;
        }




        ///////////////////////////
        // -- Make the Canvas -- //
        ///////////////////////////
        setTDRStyle();

        TLegend *legend = new TLegend(0.65, 0.30, 0.99, 0.42);
        TString CanvasName = "c_SF_eta";
        TCanvas *c = new TCanvas(CanvasName, "", 600, 600); c->cd();

        TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.01,0.99,0.99);
        c1_1->Draw();
        c1_1->cd();

        //Draw Plots
        g_Type2->Draw("APSAME");
        g_Type1->Draw("PSAME");

        //Apply tdrStyle
        c1_1->UseCurrentStyle();

        //ensure additional space at the bottom side for ratio plot
        c1_1->SetBottomMargin(0.3);

        //Attributes on MC plot
        g_Type2->SetLineColor( kGreen+2 );
        g_Type2->SetMarkerColor( kGreen+2 );
        g_Type2->SetMarkerStyle( 20 );
        g_Type2->SetFillColorAlpha( kWhite, 0 );

//        g_Type2->GetXaxis()->SetTitle( XTitle );
        g_Type2->GetXaxis()->SetLabelSize(0);
        g_Type2->GetXaxis()->SetNoExponent();
        g_Type2->GetXaxis()->SetMoreLogLabels();
        g_Type2->GetXaxis()->SetRangeUser(-2.6, 2.6);

//        this->SetYTitle_fromEffName( EffName );
        g_Type2->GetYaxis()->SetTitle( "Efficieny SF" );
        g_Type2->GetYaxis()->SetLabelSize(0.04);

        Double_t yMin = 0.63;
        Double_t yMax = 1.07;

//        if( Variable == "pt" )
//        {
//                yMin = -0.03;
//                yMax = 1.25;
//        }

        g_Type2->SetMinimum(yMin);
        g_Type2->SetMaximum(yMax);
        g_Type2->SetTitle("");

        //Attributes on data plot
        g_Type1->SetLineColor( kBlue );
        g_Type1->SetMarkerColor( kBlue );
        g_Type1->SetMarkerStyle( kFullSquare );
        g_Type1->SetFillColorAlpha( kWhite, 0 );

        //Draw legend
        legend->AddEntry( g_Type1, "my_runGH" );
        legend->AddEntry( g_Type2, "official_run2016" );
        legend->SetFillStyle(0);
        legend->SetBorderSize(0);
        legend->Draw();

        //Draw latex
//        TLatex latex;
//        this->InsertLatex_1D(&latex, EffName, Variable);

   TLatex *   tex = new TLatex(0.74,0.96,"#font[42]{2016, 13TeV}");
   tex->SetNDC();
   tex->SetLineWidth(2);
   tex->SetTextSize(ts);
   tex->Draw();

   tex = new TLatex(0.16,0.98,"#font[62]{#scale[0.8]{CMS}}");
   tex->SetNDC();
   tex->SetLineWidth(2);
   tex->SetTextSize(ts);
   tex->Draw();

   tex = new TLatex(0.16,0.96,"#font[42]{#it{#scale[0.8]{Preliminary}}}");
   tex->SetNDC();
   tex->SetLineWidth(2);
   tex->SetTextSize(ts);
   tex->Draw();

/*if( axisType == "pt" && BinNum == "0" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{0.000 #leq |#eta| #leq 0.800}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "pt" && BinNum == "1" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{0.800 #leq |#eta| #leq 1.444}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "pt" && BinNum == "2" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{1.444 #leq |#eta| #leq 1.566}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "pt" && BinNum == "3" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{1.566 #leq |#eta| #leq 2.000}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}
else if( axisType == "pt" && BinNum == "4" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{2.000 #leq |#eta| #leq 2.500}}");tex->SetNDC();tex->SetLineWidth(2);tex->Draw();}*/
if( axisType == "eta" && BinNum == "0" ){tex = new TLatex(0.3,0.96,"#font[42]{#scale[0.7]{10 #leq P_{T} #leq 20 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->SetTextSize(ts);tex->Draw();}
else if( axisType == "eta" && BinNum == "1" ){tex = new TLatex(0.3,0.96,"#font[42]{#scale[0.7]{20 #leq P_{T} #leq 35 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->SetTextSize(ts);tex->Draw();}
else if( axisType == "eta" && BinNum == "2" ){tex = new TLatex(0.3,0.96,"#font[42]{#scale[0.7]{35 #leq P_{T} #leq 50 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->SetTextSize(ts);tex->Draw();}
else if( axisType == "eta" && BinNum == "3" ){tex = new TLatex(0.3,0.96,"#font[42]{#scale[0.7]{50 #leq P_{T} #leq 90 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->SetTextSize(ts);tex->Draw();}
else if( axisType == "eta" && BinNum == "4" ){tex = new TLatex(0.3,0.96,"#font[42]{#scale[0.7]{90 #leq P_{T} #leq 150 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->SetTextSize(ts);tex->Draw();}
else if( axisType == "eta" && BinNum == "5" ){tex = new TLatex(0.3,0.91,"#font[42]{#scale[0.7]{150 #leq P_{T} #leq 500 GeV}}");tex->SetNDC();tex->SetLineWidth(2);tex->SetTextSize(ts);tex->Draw();}



        //////////////
        //Bottom Pad//
        //////////////
        TPad *c1_2 = new TPad("padc1_2","padc1_2",0.01,0.01,0.99,0.3);
        c1_2->Draw();
        c1_2->cd();

        g_ratio->Draw("APSAME");

        //Set the margins
        c1_2->SetBottomMargin(0.4);
        c1_2->SetRightMargin(0.01);
        c1_2->SetLeftMargin(0.15);

        //Attribtues on ratio plot
        g_ratio->SetTitle("");
//        g_ratio->GetXaxis()->SetTitle( XTitle );
        g_ratio->GetXaxis()->SetTitleOffset( 0.9 );
        g_ratio->GetXaxis()->SetTitleSize( 0.2 );

        g_ratio->GetYaxis()->SetTitle("my/official");
        g_ratio->GetYaxis()->SetTitleOffset( 0.4 );
        g_ratio->GetYaxis()->SetTitleSize( 0.17);
        g_ratio->GetYaxis()->SetLabelSize(0.065);

        g_ratio->GetXaxis()->SetLabelColor(1);
        g_ratio->GetXaxis()->SetLabelFont(42);
        g_ratio->GetXaxis()->SetLabelOffset(0.007);
        g_ratio->GetXaxis()->SetLabelSize(0.15);
        g_ratio->GetXaxis()->SetRangeUser(-2.6, 2.6);

        g_ratio->SetMinimum( 0.81 );
        g_ratio->SetMaximum( 1.19 );
        g_ratio->SetMarkerSize(1);

        //ratio=1 line
        TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
        f_line->SetLineColor(kAzure+1);
        f_line->SetLineWidth(1);
        f_line->Draw("SAME");

        g_ratio->Draw("PSAME");
        c->SaveAs("sf_"+BinNum+".pdf");
}






/*
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



//TString Baselocation = "./root/";
//TString inputfile = Baselocation+"EleId_efficiency_"+BinNum+".root";
TString inputfile1 = "./egammaEffi.txt_EGM2D.root";
TString inputfile2 = "./egammaEffi.txt_EGM2D_my.root";

TFile f_input1(inputfile1, "read");
TGraphErrors *SF1_0 = (TGraphErrors*)f_input1.Get("grSF1D_0");
//SF1_0->Draw();
TH1F *h1_0 = SF1_0->GetHistogram();
h1_0->Draw("p");

TFile f_input2(inputfile2, "read");
TGraphErrors *SF2_0 = (TGraphErrors*)f_input2.Get("grSF1D_0");






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

}*/
