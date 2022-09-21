#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

// #include "apv.C"
// #include "evBuilder.C"

using std::string;

bool checkTimeDifference = false;

void mergeMapped(string runVMM = "0832_cut", string runAPV = "423_cut", bool tight = true, bool timefix = false){
  string tightText = tight ? "_tight" : "";
  string fixTimeText = timefix ? "_timefix" : "";
  auto pairFile = TFile::Open(TString("../out/mapped_run_"+runVMM+"_run"+runAPV+tightText+fixTimeText+".root"), "read");
  auto pairTree = static_cast<TTree*>(pairFile->Get("mappedEvents"));
  long long eventNumAPV, eventNumVMM;
  int deltaT;
  pairTree->SetBranchAddress("apv", &eventNumAPV);
  pairTree->SetBranchAddress("vmm", &eventNumVMM);
  pairTree->SetBranchAddress("deltaT", &deltaT);

  auto vmmFile = TFile::Open(TString("../data/run_"+runVMM+".root"), "read");
  auto vmmTree = static_cast<TTree*>(vmmFile->Get("vmm"));
  // auto vmmEntryList = new TEntryList("","");
  // vmmEntryList->SetTree(vmmTree);
  auto apvFile = TFile::Open(TString("../data-apv/run"+runAPV+".root"), "read");
  auto apvTree = static_cast<TTree*>(apvFile->Get("apv_raw"));
  // auto apvEntryList = new TEntryList("","");
  // apvEntryList->SetTree(apvTree);

  auto mergedFile = TFile::Open(TString("../out/runMerged_run_"+runVMM+"_run"+runAPV+tightText+fixTimeText+".root"), "recreate");
  auto pairTreeNew = pairTree->CloneTree(0);
  pairTreeNew->SetDirectory(mergedFile);
  auto vmmTreeNew = vmmTree->CloneTree(0);
  vmmTreeNew->SetDirectory(mergedFile);
  auto apvTreeNew = apvTree->CloneTree(0);
  apvTreeNew->SetDirectory(mergedFile);

  auto nEntries = pairTree->GetEntries();
  printf("N paired entries: %lld\n", nEntries);
  for(auto i = 0; i < nEntries; i++){
    pairTree->GetEntry(i);
    if(checkTimeDifference && abs(deltaT) > 50) continue;
    // printf("%d -- deltaT: %d\n", i, deltaT);
    pairTreeNew->Fill();
    vmmTree->GetEntry(eventNumVMM);
    vmmTreeNew->Fill();
    apvTree->GetEntry(eventNumAPV);
    apvTreeNew->Fill();
  }
  
  pairTreeNew->Print();
  vmmTreeNew->Print();
  apvTreeNew->Print();

  mergedFile->Write();
  mergedFile->Close();

  pairFile->Close();
  apvFile->Close();
  vmmFile->Close();
}