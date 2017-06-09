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

#include "/Users/KyeongPil_Lee/Codes/DYAnalysisCodes/tdrstyle.C"
#include <vector>
#include <iostream>



class TagProbeTool
{
public:
	TFile *f_input1;
	TFile *f_input2;

	TString YTitle_1D;

	vector< TString > EffNames;
	vector< TString > Variables;

	TString Type1;
	TString Type2;

	TString LegendName1;
	TString LegendName2;

	TFile *f_output;

	TagProbeTool();

	void LoopForAllEffPlots_1D();
	void SaveComparisonPlots_1D(TString EffName, TString Variable);
	void InsertLatex_1D(TLatex *latex, TString EffName, TString Variable);
	void MakeRatioGraph(TGraphAsymmErrors *g_ratio, TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2);
	Double_t ReturnLargerValue(Double_t a, Double_t b);
	Double_t Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B);
	void RemoveSF_BelowPtThreshold( TString EffName, TGraphAsymmErrors* g_ratio );
	TGraphAsymmErrors* HotFix(TString EffName, TString Variable, TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2);

	// void Save_2DHistogram( TString Type, TString EffName, vector<TGraphAsymmErrors*> Graphs, TFile *f_output );
	// void Store_Canvas_2DHisto( TH2D *h_2D, TFile *f_output );
	void LoopForAllEffPlots_2D();
	void SaveComparisonPlots_2D(TString EffName, Int_t nEtaBin);
	void InsertLatex_2D(TLatex *latex, TString EffName, Double_t i_etabin);

	void SetInputFile( TFile* _f_input1, TFile * _f_input2 );
	void SetOutputFile( TFile *_f_output );
	void SetType( TString _Type1, TString _Type2 );
	void SetYTitle_fromEffName( TString EffName );
	void AddVariable( TString _Variable );
	void AddEffName( TString _EffName );
	void SetLegendName( TString _LegendName1, TString _LegendName2 );
};

TagProbeTool::TagProbeTool()
{
	this->f_input1 = NULL;
	this->f_input2 = NULL;

	this->YTitle_1D = "";

	this->Type1 = "";
	this->Type2 = "";
	this->LegendName1 = "";
	this->LegendName2 = "";
}

void TagProbeTool::LoopForAllEffPlots_1D()
{
	Int_t nEff = (Int_t)EffNames.size();
	Int_t nVar = (Int_t)Variables.size();

	for(Int_t i_eff=0; i_eff<nEff; i_eff++ )
	{
		for(Int_t i_var=0; i_var<nVar; i_var++)
		{
			this->SaveComparisonPlots_1D( EffNames[i_eff], Variables[i_var] );
		}
	}
}

void TagProbeTool::AddEffName( TString _EffName )
{
	EffNames.push_back( _EffName );
}

void TagProbeTool::AddVariable( TString _Variable )
{
	Variables.push_back( _Variable );
}

void TagProbeTool::SetType( TString _Type1, TString _Type2 )
{
	this->Type1 = _Type1;
	this->Type2 = _Type2;
}

void TagProbeTool::SetLegendName( TString _LegendName1, TString _LegendName2 )
{
	this->LegendName1 = _LegendName1;
	this->LegendName2 = _LegendName2;
}

void TagProbeTool::SetInputFile( TFile* _f_input1, TFile * _f_input2 )
{
	this->f_input1 = _f_input1;
	this->f_input2 = _f_input2;
}

void TagProbeTool::SetOutputFile( TFile *_f_output )
{
	this->f_output = _f_output;
}

void TagProbeTool::SetYTitle_fromEffName( TString EffName )
{
	if( EffName == "IsoMu20_OR_IsoTkMu20_from_Tight2012_and_dBeta_015" ||
		EffName == "Mu45_eta2p1_from_HighPt_and_RelTrkIso_010" ||
		EffName == "Mu50_from_HighPt_and_RelTrkIso_010" ||
		EffName == "IsoMu20_from_Tight2012_and_dBeta_015" ||
		EffName == "IsoMu22_OR_IsoTkMu22_from_Tight2012_and_dBeta_015" ||
		EffName == "Mu50_OR_TkMu50_from_HighPt_and_RelTrkIso_010" ||
		EffName == "IsoTkMu20_from_Tight2012_and_dBeta_015" ||
		EffName == "IsoMu27_OR_IsoTkMu27_from_Tight2012_and_dBeta_015" )
		// YTitle_1D = "Overall Efficiency";
		this->YTitle_1D = "Efficiency";

	else if( EffName == "L1_IsoMu20_from_Tight2012_and_dBeta_015" ||
	 		 EffName == "L1_Mu45_eta2p1_from_HighPt_and_RelTrkIso_010" ||
	 		 EffName == "L1_Mu50_from_HighPt_and_RelTrkIso_010" )
		this->YTitle_1D = "L1 Efficiency";

	else if( EffName == "L2_IsoMu20_from_L1_IsoMu20" ||
			 EffName == "L2_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" ||
	 		 EffName == "L2_Mu45_eta2p1_from_L1_Mu45_eta2p1" ||
	 		 EffName == "L2_Mu50_from_L1_Mu50" )
		this->YTitle_1D = "L2 Eff. over L1";

	else if( EffName == "L3_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
		this->YTitle_1D = "L3 Eff. over L1";

	else if( EffName == "L3_IsoMu20_from_L2_IsoMu20" ||
			 EffName == "L3_IsoMu24_from_L2_IsoMu24_and_Tight2012_and_dBeta_015" ||
	 		 EffName == "Mu45_eta2p1_from_L2_Mu45_eta2p1" ||
	 		 EffName == "Mu50_from_L2_Mu50" )
		this->YTitle_1D = "L3 Eff. over L2";

	else if( EffName == "IsoF_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
		this->YTitle_1D = "IsoF Eff. over L1";

	else if( EffName == "IsoF_IsoMu20_from_L3_IsoMu20" ||
			 EffName == "IsoF_IsoMu24_from_L3_IsoMu24_and_Tight2012_and_dBeta_015" )
		this->YTitle_1D = "IsoF Eff. over L3";

	else if( EffName == "TkMuF_IsoTkMu20_from_L1_IsoMu20" ||
			 EffName == "TkMuF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015")
		this->YTitle_1D = "TkMuF Eff. over L1";

	else if( EffName == "IsoF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
		this->YTitle_1D = "IsoF Eff. over L1";

	else if( EffName == "IsoF_IsoTkMu20_from_TkMuF_IsoTkMu20" )
		this->YTitle_1D = "IsoF Eff. over TkMuF";

	else if( EffName == "IsoMu20_OR_IsoTkMu20_from_L1_IsoMu20_and_Tight2012_and_dBeta_015" )
		this->YTitle_1D = "HLT Eff. over L1";

	else if( EffName == "L3_IsoMu24_OR_TkMuF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
		this->YTitle_1D = "L3 or TkMu Eff. over L1";

	else if( EffName == "IsoF_IsoMu24_OR_IsoF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
		this->YTitle_1D = "Iso or IsoTk Eff. over L1";
	
	else
		this->YTitle_1D = "Efficiency";
}

TGraphAsymmErrors* TagProbeTool::HotFix(TString EffName, TString Variable, TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2)
{
	TGraphAsymmErrors *g_Fix;
	if( Variable == "vtx" )
	{
		g_Fix = new TGraphAsymmErrors();
		Int_t nPoint = g_Type2->GetN();
		for(Int_t i=0; i<nPoint; i++)
		{
			printf("[(i, nPoint_Fix) = (%d, %d)]\n", i, g_Fix->GetN() );

			Double_t x, y;
			g_Type2->GetPoint(i, x, y);
			Double_t xErrorHigh = g_Type2->GetErrorXhigh(i);
			Double_t xErrorLow = g_Type2->GetErrorXlow(i);
			Double_t yErrorHigh = g_Type2->GetErrorYhigh(i);
			Double_t yErrorLow = g_Type2->GetErrorYlow(i);

			if( i == 0 )
			{
				printf("[Add additional points]\n");
				Int_t i_Type1 = 0;

				Double_t x_Type1, y_Type1;
				g_Type1->GetPoint(i_Type1, x_Type1, y_Type1);
				Double_t xErrorHigh_Type1 = g_Type1->GetErrorXhigh(i_Type1);
				Double_t xErrorLow_Type1 = g_Type1->GetErrorXlow(i_Type1);

				g_Fix->SetPoint(g_Fix->GetN(), x_Type1, 0);
				g_Fix->SetPointError(g_Fix->GetN()-1, xErrorLow_Type1, xErrorHigh_Type1, 0, 0);

				i_Type1 = 1;
				g_Type1->GetPoint(i_Type1, x_Type1, y_Type1);
				xErrorHigh_Type1 = g_Type1->GetErrorXhigh(i_Type1);
				xErrorLow_Type1 = g_Type1->GetErrorXlow(i_Type1);

				g_Fix->SetPoint(g_Fix->GetN(), x_Type1, 0);
				g_Fix->SetPointError(g_Fix->GetN()-1, xErrorLow_Type1, xErrorHigh_Type1, 0, 0);

				i_Type1 = 2;
				g_Type1->GetPoint(i_Type1, x_Type1, y_Type1);
				xErrorHigh_Type1 = g_Type1->GetErrorXhigh(i_Type1);
				xErrorLow_Type1 = g_Type1->GetErrorXlow(i_Type1);

				g_Fix->SetPoint(g_Fix->GetN(), x_Type1, 0);
				g_Fix->SetPointError(g_Fix->GetN()-1, xErrorLow_Type1, xErrorHigh_Type1, 0, 0);

				i_Type1 = 3;
				g_Type1->GetPoint(i_Type1, x_Type1, y_Type1);
				xErrorHigh_Type1 = g_Type1->GetErrorXhigh(i_Type1);
				xErrorLow_Type1 = g_Type1->GetErrorXlow(i_Type1);

				g_Fix->SetPoint(g_Fix->GetN(), x_Type1, 0);
				g_Fix->SetPointError(g_Fix->GetN()-1, xErrorLow_Type1, xErrorHigh_Type1, 0, 0);
			}

			g_Fix->SetPoint(g_Fix->GetN(), x, y);
			g_Fix->SetPointError(g_Fix->GetN()-1, xErrorLow, xErrorHigh, yErrorLow, yErrorHigh);
		}
	}
	else if( EffName == "IsoF_IsoMu24_from_L3_IsoMu24_and_Tight2012_and_dBeta_015" ) // -- add missing first 3 points in MC plot -- //
	{
		g_Fix = new TGraphAsymmErrors();
		Int_t nPoint = g_Type2->GetN();
		for(Int_t i=0; i<nPoint; i++)
		{
			printf("[(i, nPoint_Fix) = (%d, %d)]\n", i, g_Fix->GetN() );

			Double_t x, y;
			g_Type2->GetPoint(i, x, y);
			Double_t xErrorHigh = g_Type2->GetErrorXhigh(i);
			Double_t xErrorLow = g_Type2->GetErrorXlow(i);
			Double_t yErrorHigh = g_Type2->GetErrorYhigh(i);
			Double_t yErrorLow = g_Type2->GetErrorYlow(i);

			if( i == 0 )
			{
				printf("[Add additional points]\n");
				Int_t i_Type1 = 0;

				Double_t x_Type1, y_Type1;
				g_Type1->GetPoint(i_Type1, x_Type1, y_Type1);
				Double_t xErrorHigh_Type1 = g_Type1->GetErrorXhigh(i_Type1);
				Double_t xErrorLow_Type1 = g_Type1->GetErrorXlow(i_Type1);

				g_Fix->SetPoint(g_Fix->GetN(), x_Type1, 0);
				g_Fix->SetPointError(g_Fix->GetN()-1, xErrorLow_Type1, xErrorHigh_Type1, 0, 0);

				i_Type1 = 1;
				g_Type1->GetPoint(i_Type1, x_Type1, y_Type1);
				xErrorHigh_Type1 = g_Type1->GetErrorXhigh(i_Type1);
				xErrorLow_Type1 = g_Type1->GetErrorXlow(i_Type1);

				g_Fix->SetPoint(g_Fix->GetN(), x_Type1, 0);
				g_Fix->SetPointError(g_Fix->GetN()-1, xErrorLow_Type1, xErrorHigh_Type1, 0, 0);
			}

			g_Fix->SetPoint(g_Fix->GetN(), x, y);
			g_Fix->SetPointError(g_Fix->GetN()-1, xErrorLow, xErrorHigh, yErrorLow, yErrorHigh);
		}
	}

	return g_Fix;
}

void TagProbeTool::SaveComparisonPlots_1D(TString EffName, TString Variable)
{
	TString GraphName_Type1 = Type1 + "_" + EffName + "_" + Variable;
	TString GraphName_Type2 = Type2 + "_" + EffName + "_" + Variable;

	this->f_input1->cd();	
	TGraphAsymmErrors *g_Type1 = (TGraphAsymmErrors*)f_input1->Get( GraphName_Type1 )->Clone();

	this->f_input2->cd();
	TGraphAsymmErrors *g_Type2 = (TGraphAsymmErrors*)f_input2->Get( GraphName_Type2 )->Clone();

	// if( (EffName == "IsoF_IsoMu24_from_L3_IsoMu24_and_Tight2012_and_dBeta_015" && Variable == "pt" && Type2 == "MC_weight") ||
	// 	(Variable == "vtx" && Type2 == "MC_weight") )
	// 	g_Type2 = this->HotFix( EffName, Variable, g_Type1, g_Type2 );

	////////////////////////////////////
	// -- Make Type1 / Type2 Graph -- //
	////////////////////////////////////
	Int_t nPoint_Type1 = g_Type1->GetN();
	Int_t nPoint_Type2 = g_Type2->GetN();
	if( nPoint_Type1 != nPoint_Type2)
	{
		printf("[# points in the graphs are different! ... ratio of the graph may not be calculated properly!]\n");
		printf("\t[Graph name] %s\n", GraphName_Type1.Data() );
		printf("\t[# points] (%s, %s) = (%d, %d)\n", Type1.Data(), Type2.Data(), nPoint_Type1, nPoint_Type2);
	}


	TGraphAsymmErrors *g_ratio = (TGraphAsymmErrors*)g_Type1->Clone();
	g_ratio->SetName( "SF_" + EffName );
	this->MakeRatioGraph(g_ratio, g_Type1, g_Type2);

	if( Variable == "pt" )
		this->RemoveSF_BelowPtThreshold( EffName, g_ratio );

	///////////////////////////
	// -- Make the Canvas -- //
	///////////////////////////
	setTDRStyle();

	TLegend *legend = new TLegend(0.65, 0.30, 0.99, 0.42);
	TString CanvasName = "c_Eff_" + EffName + "_" + Variable;
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
	
	TString XTitle = "";
	Double_t xMin = 0;
	Double_t xMax = 0;
	if( Variable == "pt" )
	{
		XTitle = "Muon P_{T} [GeV]";
		xMin = 0;
		xMax = 740;
	}
	else if( Variable == "eta" )
	{
		XTitle = "Muon #eta";
		xMin = -2.4;
		xMax = 2.4;
	}
	else if( Variable == "vtx" )
	{
		XTitle = "# Vertices";
		xMin = 2.5;
		xMax = 28.5;
	}
	else if( Variable == "phi" )
	{
		XTitle = "Muon #phi";
		xMin = -3.2;
		xMax = 3.2;
	}
	else if( Variable == "InstLumi" )
	{
		XTitle = "Instantaneous lumi.";
		xMin = 0;
		xMax = 5500;
	} 

	g_Type2->GetXaxis()->SetTitle( XTitle );
	g_Type2->GetXaxis()->SetLabelSize(0);
	g_Type2->GetXaxis()->SetNoExponent();
	g_Type2->GetXaxis()->SetMoreLogLabels();
	g_Type2->GetXaxis()->SetRangeUser(xMin, xMax);

	this->SetYTitle_fromEffName( EffName );
	g_Type2->GetYaxis()->SetTitle( YTitle_1D );
	g_Type2->GetYaxis()->SetLabelSize(0.04);

	Double_t yMin = 0.63;
	Double_t yMax = 1.07;

	if( Variable == "pt" )
	{
		yMin = -0.03;
		yMax = 1.25;
	}

	g_Type2->SetMinimum(yMin);
	g_Type2->SetMaximum(yMax);
	g_Type2->SetTitle("");

	//Attributes on data plot
	g_Type1->SetLineColor( kBlue );
	g_Type1->SetMarkerColor( kBlue );
	g_Type1->SetMarkerStyle( kFullSquare );
	g_Type1->SetFillColorAlpha( kWhite, 0 );

	//Draw legend
	legend->AddEntry( g_Type1, LegendName1 );
	legend->AddEntry( g_Type2, LegendName2 );
	legend->SetFillStyle(0);
	legend->SetBorderSize(0);
	legend->Draw();

	//Draw latex
	TLatex latex;
	this->InsertLatex_1D(&latex, EffName, Variable);

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
	g_ratio->GetXaxis()->SetTitle( XTitle );
	g_ratio->GetXaxis()->SetTitleOffset( 0.9 );
	g_ratio->GetXaxis()->SetTitleSize( 0.2 );

	g_ratio->GetYaxis()->SetTitle("2016/2015");
	g_ratio->GetYaxis()->SetTitleOffset( 0.4 );
	g_ratio->GetYaxis()->SetTitleSize( 0.17);
	g_ratio->GetYaxis()->SetLabelSize(0.065);

	g_ratio->GetXaxis()->SetLabelColor(1);
	g_ratio->GetXaxis()->SetLabelFont(42);
	g_ratio->GetXaxis()->SetLabelOffset(0.007);
	g_ratio->GetXaxis()->SetLabelSize(0.15);
	g_ratio->GetXaxis()->SetRangeUser(xMin, xMax);

	g_ratio->SetMinimum( 0.81 );
	g_ratio->SetMaximum( 1.19 );
	g_ratio->SetMarkerSize(1);

	//ratio=1 line
	TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
	f_line->SetLineColor(kAzure+1);
	f_line->SetLineWidth(1);
	f_line->Draw("SAME");

	g_ratio->Draw("PSAME");
	c->SaveAs(CanvasName+".pdf");

	f_output->cd();
	g_Type1->Write();
	g_Type2->Write();
	g_ratio->Write();
	c->Write();

}

void TagProbeTool::RemoveSF_BelowPtThreshold( TString EffName, TGraphAsymmErrors* g_ratio )
{
	Double_t PtCut = 0;

	if( EffName == "IsoMu22_OR_IsoTkMu22_from_Tight2012_and_dBeta_015" )
		PtCut = 22;
	
	else if( EffName == "L2_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" ||
			 EffName == "L3_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" ||
			 EffName == "IsoF_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" ||
			 EffName == "TkMuF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" || 
			 EffName == "IsoF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" ||
			 EffName == "L3_IsoMu24_OR_TkMuF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" || 
			 EffName == "IsoF_IsoMu24_OR_IsoF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" ||
			 EffName == "L3_IsoMu24_from_L2_IsoMu24_and_Tight2012_and_dBeta_015" ||
			 EffName == "IsoF_IsoMu24_from_L3_IsoMu24_and_Tight2012_and_dBeta_015" )
		PtCut = 24;

	else if( EffName == "IsoMu27_OR_IsoTkMu27_from_Tight2012_and_dBeta_015" )
		PtCut = 27;

	else if( EffName == "Mu45_eta2p1_from_L2_Mu45_eta2p1" )
		PtCut = 45;
	
	else if( EffName == "Mu50_from_HighPt_and_RelTrkIso_010" ||
			 EffName == "Mu50_OR_TkMu50_from_HighPt_and_RelTrkIso_010" )
		PtCut = 50;

	Double_t x, y;
	Int_t nPoint = g_ratio->GetN();
	cout << "===================" << endl;
	cout << g_ratio->GetName() << endl;
	cout << "===================" << endl;
	for(Int_t i=0; i<nPoint; i++)
	{
		g_ratio->GetPoint(i, x, y);
		printf("[%d point] (%.3lf, %.3lf)\n", i, x, y);
		if( x < PtCut )
		{
			g_ratio->SetPoint(i, x, 0); // -- doesn't show the ratio value below pt threshold of the trigger (meaningless) -- //
			g_ratio->SetPointEYhigh(i, 0);
			g_ratio->SetPointEYlow(i, 0);
			printf("\t[this point is removed]\n");
		}
	}
}

Double_t TagProbeTool::Error_PropagatedAoverB(Double_t A, Double_t sigma_A, Double_t B, Double_t sigma_B)
{
	Double_t ratio_A = (sigma_A) / A;
	Double_t ratio_B = (sigma_B) / B;

	Double_t errorSquare = ratio_A * ratio_A + ratio_B * ratio_B;

	return (A/B) * sqrt(errorSquare);
}

Double_t TagProbeTool::ReturnLargerValue(Double_t a, Double_t b)
{
	if( a > b )
		return a;
	else
		return b;
}

void TagProbeTool::MakeRatioGraph(TGraphAsymmErrors *g_ratio, TGraphAsymmErrors *g_Type1, TGraphAsymmErrors *g_Type2)
{
	// g_ratio = (TGraphAsymmErrors*)g_Type2->Clone();
	g_ratio->Set(0); //Remove all points (reset)

	Int_t NPoints = g_Type1->GetN();
	Int_t NPoints_2 = g_Type2->GetN();
	if( NPoints != NPoints_2 )
		printf("# points is different bewteen two graph...be careful for the ratio plot\n");

	for(Int_t i_p=0; i_p<NPoints; i_p++)
	{
		// cout << i_p << "th Point" << endl;
		//Get Type1 point
		Double_t x_Type1, y_Type1;
		g_Type1->GetPoint(i_p, x_Type1, y_Type1);
		Double_t error_Type1 = ReturnLargerValue( g_Type1->GetErrorYhigh(i_p), g_Type1->GetErrorYlow(i_p) );
		// cout << "x_Type1: " << x_Type1 << " y_Type1: " << y_Type1 << " error_Type1: " << error_Type1 << " g_Type1->GetErrorYhigh: " << g_Type1->GetErrorYhigh(i_p) << " g_Type1->GetErrorYlow: " << g_Type1->GetErrorYlow(i_p) << endl;

		//Get Type2 point
		Double_t x_Type2, y_Type2;
		g_Type2->GetPoint(i_p, x_Type2, y_Type2);
		Double_t error_Type2 = ReturnLargerValue( g_Type2->GetErrorYhigh(i_p), g_Type2->GetErrorYlow(i_p) );
		// cout << "x_Type2: " << x_Type2 << " y_Type2: " << y_Type2 << " error_Type2: " << error_Type2 << " g_Type2->GetErrorYhigh: " << g_Type2->GetErrorYhigh(i_p) << " g_Type2->GetErrorYlow: " << g_Type2->GetErrorYlow(i_p) << endl;

		Double_t ratio;
		Double_t ratio_error;
		if( (NPoints != NPoints_2) && i_p >= NPoints_2)
		{
			ratio = 0;
			ratio_error = 0;
		}
		else if(y_Type1 != 0 && error_Type1 != 0 && y_Type2 != 0 && error_Type2 != 0)
		{
			ratio = y_Type1 / y_Type2;
			ratio_error = Error_PropagatedAoverB(y_Type1, error_Type1, y_Type2, error_Type2);
			//calculate Scale Factor(Type1/Type2) & error

			// cout << "ratio: " << ratio << " ratio_error: " << ratio_error << endl;
		}
		else
		{
			ratio = 0;
			ratio_error = 0;
		}

		//Set Central value
		g_ratio->SetPoint(i_p, x_Type1, ratio);

		//Set the error
		Double_t error_XLow = g_Type1->GetErrorXlow(i_p);
		Double_t error_Xhigh = g_Type1->GetErrorXhigh(i_p);
		g_ratio->SetPointError(i_p, error_XLow, error_Xhigh, ratio_error, ratio_error);

		// cout << endl;
	}
}

void TagProbeTool::InsertLatex_1D(TLatex *latex, TString EffName, TString Variable)
{
	TString Type = "";
	Double_t pTMin = 0;
	Double_t EtaMax = 2.4;
	if( EffName == "IsoMu20_OR_IsoTkMu20_from_Tight2012_and_dBeta_015" )
	{
		Type = "IsoMu20_OR_IsoTkMu20";
		pTMin = 22;
	}
	else if( EffName == "IsoMu22_OR_IsoTkMu22_from_Tight2012_and_dBeta_015" )
	{
		Type = "IsoMu22_OR_IsoTkMu22";
		pTMin = 24;
	}
	else if( EffName == "Mu45_eta2p1_from_HighPt_and_RelTrkIso_010" )
	{
		Type = "Mu45_eta2p1";
		pTMin = 47;
		EtaMax = 2.1;
	}
	else if( EffName == "Mu50_from_HighPt_and_RelTrkIso_010" )
	{
		Type = "Mu50";
		pTMin = 52;
	}
	else if( EffName == "Mu50_OR_TkMu50_from_HighPt_and_RelTrkIso_010" )
	{
		Type = "Mu50_OR_TkMu50";
		pTMin = 52;
	}
	else if( EffName == "IsoMu27_OR_IsoTkMu27_from_Tight2012_and_dBeta_015" )
	{
		Type = "IsoMu27_OR_IsoTkMu27";
		pTMin = 29;
	}
	else if( EffName == "IsoMu20_from_Tight2012_and_dBeta_015" )
	{
		Type = "IsoMu20";
		pTMin = 22;
	}
	else if( EffName == "IsoTkMu20_from_Tight2012_and_dBeta_015" )
	{
		Type = "IsoTkMu20";
		pTMin = 22;
	}
	else if( EffName == "L1_IsoMu20_from_Tight2012_and_dBeta_015" )
	{
		Type = "L1_IsoMu20";
		pTMin = 22;
	}
	else if( EffName == "L2_IsoMu20_from_L1_IsoMu20" )
	{
		Type = "L2_IsoMu20";
		pTMin = 22;
	}
	else if( EffName == "L3_IsoMu20_from_L2_IsoMu20" )
	{
		Type = "L3_IsoMu20";
		pTMin = 22;
	}
	else if( EffName == "IsoF_IsoMu20_from_L3_IsoMu20" )
	{
		Type = "IsoF_IsoMu20";
		pTMin = 22;
	}
	else if( EffName == "TkMuF_IsoTkMu20_from_L1_IsoMu20" )
	{
		Type = "TkMuF_IsoTkMu20";
		pTMin = 22;
	}
	else if( EffName == "IsoF_IsoTkMu20_from_TkMuF_IsoTkMu20" )
	{
		Type = "IsoF_IsoTkMu20";
		pTMin = 22;
	}
	else if( EffName == "L1_Mu45_eta2p1_from_HighPt_and_RelTrkIso_010" )
	{
		Type = "L1_Mu45_eta2p1";
		pTMin = 47;
		EtaMax = 2.1;
	}
	else if( EffName == "L2_Mu45_eta2p1_from_L1_Mu45_eta2p1" )
	{
		Type = "L2_Mu45_eta2p1";
		pTMin = 47;
		EtaMax = 2.1;
	}
	else if( EffName == "Mu45_eta2p1_from_L2_Mu45_eta2p1" )
	{
		Type = "L3_Mu45_eta2p1";
		pTMin = 47;
		EtaMax = 2.1;
	}
	else if( EffName == "L1_Mu50_from_HighPt_and_RelTrkIso_010" )
	{
		Type = "L1_Mu50";
		pTMin = 52;
	}
	else if( EffName == "L2_Mu50_from_L1_Mu50" )
	{
		Type = "L2_Mu50";
		pTMin = 52;
	}
	else if( EffName == "Mu50_from_L2_Mu50" )
	{
		Type = "L3_Mu50";
		pTMin = 52;
	}
	else if( EffName == "IsoMu20_OR_IsoTkMu20_from_L1_IsoMu20_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoMu20_OR_IsoTkMu20";
		pTMin = 22;
	}
	else if( EffName == "L2_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "L2_IsoMu24 over L1";
		pTMin = 26;
	}
	else if( EffName == "L3_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "L3_IsoMu24 over L1";
		pTMin = 26;
	}
	else if( EffName == "IsoF_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoF_IsoMu24 over L1";
		pTMin = 26;
	}
	else if( EffName == "TkMuF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "TkMuF_IsoTkMu24 over L1";
		pTMin = 26;
	}
	else if( EffName == "IsoF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoF_IsoTkMu24 over L1";
		pTMin = 26;
	}
	else if( EffName == "L3_IsoMu24_OR_TkMuF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "L3 or TkMuF over L1";
		pTMin = 26;
	}
	else if( EffName == "IsoF_IsoMu24_OR_IsoF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoF or IsoTkF over L1";
		pTMin = 26;
	}
	else if( EffName == "L3_IsoMu24_from_L2_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "L3_IsoMu24 over L2";
		pTMin = 26;
	}
	else if( EffName == "IsoF_IsoMu24_from_L3_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoF_IsoMu24 over L3";
		pTMin = 26;
	}

	TString str_pTMin = TString::Format("%.lf", pTMin);
	TString str_EtaMax = TString::Format("%.1lf", EtaMax);

	latex->DrawLatexNDC(0.74, 0.96, "#font[42]{2016, 13TeV}");
	latex->DrawLatexNDC(0.19, 0.895, "#font[62]{CMS}");
	latex->DrawLatexNDC(0.19, 0.85, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
	latex->DrawLatexNDC(0.50, 0.895, "#font[42]{#scale[0.7]{"+Type+"}}");

	if( Variable == "pt" )
		latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{|#eta| < "+str_EtaMax+"}}");

	else if( Variable == "eta" )
		latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{P_{T} > "+str_pTMin+" GeV}}");

	else if( Variable == "vtx" || Variable == "phi" || Variable == "InstLumi" )
		latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{P_{T} > "+str_pTMin+" GeV, |#eta| < "+str_EtaMax+"}}");
}


void TagProbeTool::LoopForAllEffPlots_2D()
{
	Int_t nEff = (Int_t)EffNames.size();
	
	for(Int_t i_eff=0; i_eff<nEff; i_eff++ )
	{
		Int_t nEtaBin = 4;

		// if( EffNames[i_eff] == "Mu45_eta2p1_from_HighPt_and_RelTrkIso_010" )
		// 	nEtaBin = 3;

		this->SaveComparisonPlots_2D( EffNames[i_eff], nEtaBin );
	}
}

void TagProbeTool::InsertLatex_2D(TLatex *latex, TString EffName, Double_t i_etabin)
{
	TString Type = "";
	if( EffName == "IsoMu22_OR_IsoTkMu22_from_Tight2012_and_dBeta_015" )
	{
		Type = "IsoMu22_OR_IsoTkMu22";
	}
	else if( EffName == "Mu45_eta2p1_from_HighPt_and_RelTrkIso_010" )
	{
		Type = "Mu45_eta2p1";
	}
	else if( EffName == "Mu50_from_HighPt_and_RelTrkIso_010" )
	{
		Type = "Mu50";
	}
	else if( EffName == "Mu50_OR_TkMu50_from_HighPt_and_RelTrkIso_010" )
	{
		Type = "Mu50_OR_TkMu50";
	}
	else if( EffName == "IsoMu27_OR_IsoTkMu27_from_Tight2012_and_dBeta_015" )
	{
		Type = "IsoMu27_OR_IsoTkMu27";
	}
	else if( EffName == "L2_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "L2_IsoMu24 over L1";
	}
	else if( EffName == "L3_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "L3_IsoMu24 over L1";
	}
	else if( EffName == "IsoF_IsoMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoF_IsoMu24 over L1";
	}
	else if( EffName == "TkMuF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "TkMuF_IsoTkMu24 over L1";
	}
	else if( EffName == "IsoF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoF_IsoTkMu24 over L1";
	}
	else if( EffName == "L3_IsoMu24_OR_TkMuF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "L3 or TkMuF over L1";
	}
	else if( EffName == "IsoF_IsoMu24_OR_IsoF_IsoTkMu24_from_L1_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoF or IsoTkF over L1";
	}
	else if( EffName == "L3_IsoMu24_from_L2_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "L3_IsoMu24 over L1";
	}
	else if( EffName == "IsoF_IsoMu24_from_L3_IsoMu24_and_Tight2012_and_dBeta_015" )
	{
		Type = "IsoF_IsoMu24 over L3";
	}

	latex->DrawLatexNDC(0.74, 0.96, "#font[42]{2016, 13TeV}");
	latex->DrawLatexNDC(0.19, 0.895, "#font[62]{CMS}");
	latex->DrawLatexNDC(0.19, 0.85, "#font[42]{#it{#scale[0.8]{Preliminary}}}");
	latex->DrawLatexNDC(0.50, 0.895, "#font[42]{#scale[0.7]{"+Type+"}}");

	if( i_etabin == 0 )
		latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{|#eta| < 0.9}}");
	else if( i_etabin == 1 )
		latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{0.9 < |#eta| < 1.2}}");
	else if( i_etabin == 2 )
		latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{1.2 < |#eta| < 2.1}}");
	else if( i_etabin == 3 )
		latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{2.1 < |#eta| < 2.4}}");

	// if( i_etabin == 0 )
	// 	latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{|#eta| < 0.2}}");
	// else if( i_etabin == 1 )
	// 	latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{0.2 < |#eta| < 0.3}}");
	// else if( i_etabin == 2 )
	// 	latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{0.3 < |#eta| < 0.9}}");
	// else if( i_etabin == 3 )
	// 	latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{0.9 < |#eta| < 1.2}}");
	// else if( i_etabin == 4 )
	// 	latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{1.2 < |#eta| < 1.6}}");
	// else if( i_etabin == 5 )
	// 	latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{1.6 < |#eta| < 2.1}}");
	// else if( i_etabin == 6 )
	// 	latex->DrawLatexNDC(0.50, 0.85, "#font[42]{#scale[0.7]{2.1 < |#eta| < 2.4}}");
}

void TagProbeTool::SaveComparisonPlots_2D(TString EffName, Int_t nEtaBin)
{
	vector< TGraphAsymmErrors* > Graphs_Type1;
	vector< TGraphAsymmErrors* > Graphs_Type2;
	vector< TGraphAsymmErrors* > Graphs_SF;

	for(Int_t i_etabin=0; i_etabin<nEtaBin; i_etabin++)
	{
		TString EtaBinName = TString::Format("_pteta_abseta%d", i_etabin);
		TString GraphName_Type1 = Type1 + "_" + EffName + EtaBinName;
		TString GraphName_Type2 = Type2 + "_" + EffName + EtaBinName;

		// printf("[Graph1, Graph2 = (%s, %s)]\n", GraphName_Type1.Data(), GraphName_Type2.Data() );

		f_input1->cd();	
		TGraphAsymmErrors *g_Type1 = (TGraphAsymmErrors*)f_input1->Get( GraphName_Type1 )->Clone();

		f_input2->cd();
		TGraphAsymmErrors *g_Type2 = (TGraphAsymmErrors*)f_input2->Get( GraphName_Type2 )->Clone();

		////////////////////////////////////
		// -- Make Type1 / Type2 Graph -- //
		////////////////////////////////////
		TGraphAsymmErrors *g_ratio = (TGraphAsymmErrors*)g_Type1->Clone();
		g_ratio->SetName( "SF_" + EffName + EtaBinName );
		MakeRatioGraph(g_ratio, g_Type1, g_Type2);

		Graphs_Type1.push_back( g_Type1 );
		Graphs_Type2.push_back( g_Type2 );
		Graphs_SF.push_back( g_ratio	);

		///////////////////////////
		// -- Make the Canvas -- //
		///////////////////////////
		setTDRStyle();

		TLegend *legend = new TLegend(0.60, 0.30, 0.99, 0.45);
		TString CanvasName = "c_Eff_" + EffName + EtaBinName;
		TCanvas *c = new TCanvas(CanvasName, "", 600, 600); c->cd();

		TPad *c1_1 = new TPad("padc1_1","padc1_1",0.01,0.01,0.99,0.99);
		c1_1->Draw();
		c1_1->cd();

		//Draw Plots
		g_Type1->Draw("APSAME");
		g_Type2->Draw("PSAME");

		//Apply tdrStyle
		c1_1->UseCurrentStyle();

		//ensure additional space at the bottom side for ratio plot
		c1_1->SetBottomMargin(0.3);

		//Attributes on MC plot
		g_Type2->SetLineColor( kRed );
		g_Type2->SetMarkerColor( kRed );
		g_Type2->SetMarkerStyle( 20 );
		g_Type2->SetFillColorAlpha( kWhite, 0 );
		
		TString XTitle = "Muon P_{T} [GeV]";
		// Double_t xMin = 20;
		// Double_t xMax = 123;

		g_Type1->GetXaxis()->SetTitle( XTitle );
		g_Type1->GetXaxis()->SetLabelSize(0);
		g_Type1->GetXaxis()->SetNoExponent();
		g_Type1->GetXaxis()->SetMoreLogLabels();
		// g_Type1->GetXaxis()->SetRangeUser(xMin, xMax);

		// TString YTitle_1D = "";
		// if( EffName.Contains("RelMiniIso_020") )
		// 	YTitle_1D = "Iso. Efficiency";
		// else
		// 	YTitle_1D = "Reco+ID Efficiency";
		this->SetYTitle_fromEffName( EffName );
		g_Type1->GetYaxis()->SetTitle( YTitle_1D );

		Double_t yMin = 0.63;
		Double_t yMax = 1.16;
		g_Type1->SetMinimum(yMin);
		g_Type1->SetMaximum(yMax);
		g_Type1->SetTitle("");

		//Attributes on data plot
		g_Type1->SetLineColor( kBlack );
		g_Type1->SetMarkerColor( kBlack );
		g_Type1->SetMarkerStyle( kFullSquare );
		g_Type1->SetFillColorAlpha( kWhite, 0 );

		//Draw legend
		legend->AddEntry( g_Type1, LegendName1 );
		legend->AddEntry( g_Type2, LegendName2 );
		
		legend->SetFillStyle(0);
		legend->SetBorderSize(0);
		legend->Draw();

		//Draw latex
		TLatex latex;
		InsertLatex_2D(&latex, EffName, i_etabin);

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
		g_ratio->GetXaxis()->SetTitle( XTitle );
		g_ratio->GetXaxis()->SetTitleOffset( 0.9 );
		g_ratio->GetXaxis()->SetTitleSize( 0.2 );

		g_ratio->GetYaxis()->SetTitle("Data/MC");
		g_ratio->GetYaxis()->SetTitleOffset( 0.4 );
		g_ratio->GetYaxis()->SetTitleSize( 0.17);
		g_ratio->GetYaxis()->SetLabelSize(0.065);

		g_ratio->GetXaxis()->SetLabelColor(1);
		g_ratio->GetXaxis()->SetLabelFont(42);
		g_ratio->GetXaxis()->SetLabelOffset(0.007);
		g_ratio->GetXaxis()->SetLabelSize(0.15);
		// g_ratio->GetXaxis()->SetRangeUser(xMin, xMax);

		g_ratio->SetMinimum( 0.91 );
		g_ratio->SetMaximum( 1.09 );
		g_ratio->SetMarkerSize(1);

		//ratio=1 line
		TF1 *f_line = new TF1("f_line", "1", -10000, 10000);
		f_line->SetLineColor(kAzure+1);
		f_line->SetLineWidth(1);
		f_line->Draw("SAME");

		g_ratio->Draw("PSAME");
		c->SaveAs(CanvasName+".pdf");

		f_output->cd();
		g_Type1->Write();
		g_Type2->Write();
		g_ratio->Write();
		c->Write();
	}

	// Save_2DHistogram( Type1, EffName, Graphs_Type1, f_output );
	// Save_2DHistogram( Type2, EffName, Graphs_Type2, f_output );
	// Save_2DHistogram( "SF", EffName, Graphs_SF, f_output );

}

// void Save_2DHistogram( TString Type, TString EffName, vector<TGraphAsymmErrors*> Graphs, TFile *f_output )
// {
// 	const Int_t nPtBin = 6;
// 	Double_t PtBinEdges[nPtBin+1] = {20, 25, 30, 40, 50, 60, 120};

// 	const Int_t nEtaBin = 4;
// 	Double_t EtaBinEdges[nPtBin+1] = {0, 0.9, 1.2, 2.1, 2.4};

// 	TString name_histo = "h_2D_" + Type + "_" + EffName;
// 	TH2D *h_2D = new TH2D(name_histo, name_histo, nEtaBin, EtaBinEdges, nPtBin, PtBinEdges);

// 	h_2D->SetStats(kFALSE);
// 	h_2D->SetXTitle( "Muon |#eta|" );
// 	h_2D->SetYTitle( "Muon P_{T} [GeV]" );

// 	//////////////////////////////////////////////////////////////////
// 	// -- Fill the histogram using the values from the SF graphs -- //
// 	//////////////////////////////////////////////////////////////////
// 	Int_t nGraphs = (Int_t)Graphs.size();
// 	for(Int_t i_graph=0; i_graph < nGraphs; i_graph++)
// 	{
// 		TGraphAsymmErrors *g = Graphs[i_graph];

// 		Int_t nPoint = g->GetN();
// 		if( nPoint != nPtBin )
// 		{
// 			printf( "ERROR! # Points != nPtBin ... (# Points, nPtBin) = (%d, %d)\n", nPoint, nPtBin );
// 			return;
// 		}

// 		for(Int_t i_p=0; i_p < nPoint; i_p++)
// 		{
// 			Double_t x, y, error;
// 			g->GetPoint(i_p, x, y);
// 			error = ReturnLargerValue( g->GetErrorYhigh(i_p), g->GetErrorYlow(i_p) );

// 			Int_t i_binX = i_graph +1; // -- graph = eta -- //
// 			Int_t i_binY = i_p + 1; // -- point = pt -- //
			
// 			h_2D->SetBinError(i_binX, i_binY, error);
// 			h_2D->SetBinContent(i_binX, i_binY, y);
// 		}
// 	}

// 	f_output->cd();
// 	h_2D->Write();

// 	Store_Canvas_2DHisto( h_2D, f_output );
// }

// void Store_Canvas_2DHisto( TH2D *h_2D, TFile *f_output )
// {
// 	TString CanvasName = h_2D->GetName();
// 	CanvasName.ReplaceAll("h_", "c_");

// 	TCanvas *c = new TCanvas(CanvasName, "", 800, 800);
// 	c->cd();
// 	gPad->SetTopMargin(0.03);
// 	gPad->SetLeftMargin(0.12);
// 	gPad->SetRightMargin(0.15);
// 	gPad->SetLogy();

// 	h_2D->Draw("COLZTEXT");

// 	h_2D->SetStats(kFALSE);
// 	h_2D->SetXTitle("Muon |#eta|");
// 	h_2D->SetYTitle("Muon P_{T} [GeV]");

// 	h_2D->GetYaxis()->SetNoExponent();
// 	h_2D->GetYaxis()->SetMoreLogLabels();
// 	h_2D->GetYaxis()->SetTitleOffset(1.5);
// 	h_2D->GetZaxis()->SetRangeUser(0.9, 1.1);

// 	f_output->cd();
// 	c->Write();
// 	c->SaveAs(CanvasName+".pdf");
// }


void DrawPlots_Triggers()
{
	TFile *f_input1 = TFile::Open("../ROOTFile_EfficiencyGraphs_Run2016G.root");
	TFile *f_input2 = TFile::Open("../ROOTFile_EfficiencyGraphs_Run2015D.root");
	TFile *f_output = TFile::Open("ROOTFile_EfficiencyCanvases.root", "RECREATE");

	// const Int_t nEff = 5;
	// TString EffNames[nEff] = {"IsoMu20_OR_IsoTkMu20_from_Tight2012_and_dBeta_015", 
	// "Mu45_eta2p1_from_HighPt_and_RelTrkIso_010", "Mu50_from_HighPt_and_RelTrkIso_010", 
	// "IsoMu20_from_Tight2012_and_dBeta_015", "IsoTkMu20_from_Tight2012_and_dBeta_015"};

	// const Int_t nEff = 12;
	// TString EffNames[nEff] = {"L1_IsoMu20_from_Tight2012_and_dBeta_015", "L2_IsoMu20_from_L1_IsoMu20", 
	// "L3_IsoMu20_from_L2_IsoMu20", "IsoF_IsoMu20_from_L3_IsoMu20",
	// "TkMuF_IsoTkMu20_from_L1_IsoMu20", "IsoF_IsoTkMu20_from_TkMuF_IsoTkMu20",
	// "L1_Mu45_eta2p1_from_HighPt_and_RelTrkIso_010", "L2_Mu45_eta2p1_from_L1_Mu45_eta2p1", "Mu45_eta2p1_from_L2_Mu45_eta2p1",
	// "L1_Mu50_from_HighPt_and_RelTrkIso_010", "L2_Mu50_from_L1_Mu50", "Mu50_from_L2_Mu50" };

	// const Int_t nEff = 2;
	// TString EffNames[nEff] = {"Loose", "Tight2012"};

	TagProbeTool *tool = new TagProbeTool();

	tool->SetInputFile( f_input1, f_input2 );
	tool->SetType( "Data", "Data" ); // -- set the type used to find graph name -- //
	tool->SetLegendName( "Data (2016G)", "Data (2015D)"); // -- set the legend name shown in the plot -- //

	tool->AddEffName( "IsoMu27_OR_IsoTkMu27_from_Tight2012_and_dBeta_015" );
	tool->AddEffName( "Mu50_from_HighPt_and_RelTrkIso_010" );

	tool->AddVariable( "pt" );
	tool->AddVariable( "eta" );
	tool->AddVariable( "vtx" );
	tool->AddVariable( "InstLumi" );

	tool->SetOutputFile( f_output );

	tool->LoopForAllEffPlots_1D();
	// tool->LoopForAllEffPlots_2D();
}