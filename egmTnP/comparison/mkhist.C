////////////////////////////////////////////////////////////////////////
//								      //
// ** For bin00, you should input efficiency information manually. ** //
//								      //
////////////////////////////////////////////////////////////////////////

#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TTimeStamp.h>
#include <TString.h>
#include <TLegend.h>
#include <THStack.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TAttMarker.h>
#include <TF1.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TApplication.h>
#include <vector>
#include <TMath.h>

void mkhist(TString BinNum = "0") // BinNum:0-5
{
	cout << "Bin Number: " << BinNum << endl;

	//TFile *f = new TFile("EleId_efficiency_" + BinNum + ".root", "RECREATE");
	TFile *f = new TFile("./root/EleId_efficiency_my_final_" + BinNum + ".root", "RECREATE");

	TString BaseLocation = "./";

	vector<TString> ntupleDirectory; vector<TString> Tag;
	ntupleDirectory.push_back( "AR" ); Tag.push_back( "AR" );
	ntupleDirectory.push_back( "DM" ); Tag.push_back( "DM" );

	Float_t ptbin[7];ptbin[0]=10;ptbin[1]=20;ptbin[2]=35;ptbin[3]=50;ptbin[4]=90;ptbin[5]=150;ptbin[6]=500;

	Float_t etabin[11];etabin[0]=-2.5;etabin[1]=-2.0;etabin[2]=-1.566;etabin[3]=-1.444;etabin[4]=-0.8;etabin[5]=0.0;etabin[6]=0.8;etabin[7]=1.444;etabin[8]=1.566;etabin[9]=2.0;etabin[10]=2.5;

	Float_t ee; Float_t pp;
	if( BinNum == "0" ) {ee=0; pp=10;}
	else if( BinNum == "1" ) {ee=0.8; pp=20;}
	else if( BinNum == "2" ) {ee=1.444; pp=35;}
	else if( BinNum == "3" ) {ee=1.566; pp=50;}
	else if( BinNum == "4" ) {ee=2; pp=90;}
	else if( BinNum == "5" ) {ee=2.5; pp=150;}

	Float_t ptL; Float_t ptR; Float_t etaL; Float_t etaR; Float_t effData; Float_t errEffData;

	//Loop for all samples
	const Int_t Ntup = ntupleDirectory.size();
	for(Int_t i_tup = 0; i_tup<Ntup; i_tup++)
	{
		cout << "\t<" << Tag[i_tup] << ">" << endl;

		TChain *chain = new TChain("effTree");
		if( Tag[i_tup] == "DM" ) chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/dmTree_my_final.root");
		else chain->Add(BaseLocation+"/"+ntupleDirectory[i_tup]+"/*.root");

		TH1D *h_pt = new TH1D("h_pt_"+Tag[i_tup], "", 6, ptbin);
		TH1D *h_eta = new TH1D("h_eta_"+Tag[i_tup], "", 10, etabin);

		// -- This is for bin00 -- //
		if(Tag[i_tup] == "AR") {ptL=10; ptR=20; etaL=-2.5; etaR=-2.0; effData = 0.246; errEffData = 0.002;}
		else if(Tag[i_tup] == "DM") {ptL=10; ptR=20; etaL=-2.5; etaR=-2.0; effData = 0.245; errEffData = 0.002;}

		Int_t NEvents = chain->GetEntries();
		cout << "\t[Total Events: " << NEvents << "]" << endl;
		for(Int_t i=0; i<NEvents; i++)
		{
			chain->GetEntry(i);

			chain->SetBranchAddress("ptL",&ptL);
			chain->SetBranchAddress("ptR",&ptR);
			chain->SetBranchAddress("etaL",&etaL);
			chain->SetBranchAddress("etaR",&etaR);
			chain->SetBranchAddress("effData",&effData);
			chain->SetBranchAddress("errEffData",&errEffData);

			if( etaL == ee || etaR == -ee )
			{
				if( ptL == ptbin[0] ) {h_pt->SetBinContent( 1, effData ); h_pt->SetBinError( 1, errEffData );}
				else if( ptL == ptbin[1] ) {h_pt->SetBinContent( 2, effData ); h_pt->SetBinError( 2, errEffData );}
				else if( ptL == ptbin[2] ) {h_pt->SetBinContent( 3, effData ); h_pt->SetBinError( 3, errEffData );}
				else if( ptL == ptbin[3] ) {h_pt->SetBinContent( 4, effData ); h_pt->SetBinError( 4, errEffData );}
				else if( ptL == ptbin[4] ) {h_pt->SetBinContent( 5, effData ); h_pt->SetBinError( 5, errEffData );}
				else if( ptL == ptbin[5] ) {h_pt->SetBinContent( 6, effData ); h_pt->SetBinError( 6, errEffData );}
			}
			if( ptL == pp )
			{
				if( etaL == etabin[0] ) {h_eta->SetBinContent( 1, effData ); h_eta->SetBinError( 1, errEffData );}
				else if( etaL == etabin[1] ) {h_eta->SetBinContent( 2, effData ); h_eta->SetBinError( 2, errEffData );}
				else if( etaL == etabin[2] ) {h_eta->SetBinContent( 3, effData ); h_eta->SetBinError( 3, errEffData );}
				else if( etaL == etabin[3] ) {h_eta->SetBinContent( 4, effData ); h_eta->SetBinError( 4, errEffData );}
				else if( etaL == etabin[4] ) {h_eta->SetBinContent( 5, effData ); h_eta->SetBinError( 5, errEffData );}
				else if( etaL == etabin[5] ) {h_eta->SetBinContent( 6, effData ); h_eta->SetBinError( 6, errEffData );}
				else if( etaL == etabin[6] ) {h_eta->SetBinContent( 7, effData ); h_eta->SetBinError( 7, errEffData );}
				else if( etaL == etabin[7] ) {h_eta->SetBinContent( 8, effData ); h_eta->SetBinError( 8, errEffData );}
				else if( etaL == etabin[8] ) {h_eta->SetBinContent( 9, effData ); h_eta->SetBinError( 9, errEffData );}
				else if( etaL == etabin[9] ) {h_eta->SetBinContent( 10, effData ); h_eta->SetBinError( 10, errEffData );}
			}
		} //End of event iteration

		h_pt->Write();
		h_eta->Write();

	} //end of i_tup iteration
}

