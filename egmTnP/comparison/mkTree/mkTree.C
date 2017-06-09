#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TTree.h"

void mkTree()
{

//TTree *arTree = new TTree("effTree", "effTree");
//arTree->ReadFile("arun.txt", "etaL:etaR:ptL:ptR:effData:errEffData");
//arTree->SetEstimate(-1);

TTree *dmTree = new TTree("effTree", "effTree");
dmTree->ReadFile("dalmin_my_final.txt", "etaL:etaR:ptL:ptR:effData:errEffData");
dmTree->SetEstimate(-1);

//TFile f_output1("/home/dmpai/test/TnPtest/AR/arTree.root", "recreate");
//arTree->Write();

TFile f_output2("../DM/dmTree_my_final.root", "recreate");
dmTree->Write();

}
