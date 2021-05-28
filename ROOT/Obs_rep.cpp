#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <math.h>
# define PI 3.14159265358979323846
#include "getDate.cpp"

using namespace std;

// generates a spectrum with the same binning as NuSTAR observations of Jupiter

int Obs_rep(const char filename[20])
{
  gROOT -> Reset();

  int const num_particles = 2; // expected number of unique particle species

  TFile *inFile; // adress for input file
  TTree *tree; // adress for root tree stored in file
  TH1D  *NuSTAR_spectrum; // adress for array of histograms (one for each particle species)
  TH1D  *XMM_spectrum;
  TH1D *Joint_spectrum;

  int const NuSTAR_bin_num = 27; // number of bins in a hist
  Double_t NuSTAR_edges[28] = {4.72, 5.04, 6.12, 6.48, 6.84, 6.92, 7.12, 7.32, 7.52,
                               7.92, 8.0, 8.76, 8.96, 9.48, 9.76, 10.24, 11.04, 11.12,
                               11.4, 11.52, 13.04, 14.56, 15.88, 16.36, 17.28, 18.08, 20.32, 21.2};
  int const XMM_bin_num = 9; // number of bins in a hist
  Double_t XMM_edges[10] = {3.07, 3.37, 3.84, 4.495, 5.015, 5.835, 5.955, 6.22, 6.68, 9.975};
  float min, max, binSize; // maximum and minimun energy values and width of each bin

  int const joint_bin_num = 30;
  Double_t Joint_edges[31] = {3.07, 3.37, 3.84, 4.72, 5.04, 6.12, 6.48, 6.84, 6.92, 7.12, 7.32, 7.52,
                               7.92, 8.0, 8.76, 8.96, 9.48, 9.76, 10.24, 11.04, 11.12,
                               11.4, 11.52, 13.04, 14.56, 15.88, 16.36, 17.28, 18.08, 20.32, 21.2};

  NuSTAR_spectrum = new TH1D("NuSTAR_Spect", "NuSTAR Observation Simulation", NuSTAR_bin_num, NuSTAR_edges);
    // assign titles to the histogram axes
	NuSTAR_spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
	NuSTAR_spectrum -> GetYaxis() -> SetTitle("Counts");

  XMM_spectrum = new TH1D("XMM_Spect", "XMM Observation Simulation", XMM_bin_num, XMM_edges);
  XMM_spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
	XMM_spectrum -> GetYaxis() -> SetTitle("Counts");

  Joint_spectrum = new TH1D("Joint_Spect", "Joint Observation Simulation", joint_bin_num, Joint_edges);
  Joint_spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
	Joint_spectrum -> GetYaxis() -> SetTitle("Counts");

  // file paths for input and output files
  const char* dir = "../output/"; // filepath from ROOT directory to output directory

  char outputFile[100]; // output file memory location
  getDate(); // get Date for file name
	sprintf(outputFile, "%s%s%s%s", dir, "plots/", filename, "observation_reproduction.root"); // output file name

	char inputFile[100]; // input file memory location
  sprintf(inputFile, "%s%s%s", dir, filename, ".root"); // input file name and directory

  cout << "********* opening " << inputFile << " *********" << endl; // let me know that we are OPENING this bish

  // open data file
  inFile = (new TFile(inputFile));
  // extract the tree
  tree = (TTree*) inFile -> Get("JupiterData");
  tree -> Print();
  // set variables to hold data
  int eventID, trackID, parentID, nextVolNum, particleID, lastEventID;
  float px, py, pz, x, y, z;
  double energy;

  // setup the variable adresses for branches
  tree -> SetBranchAddress("eventID", &eventID);
  //tree -> SetBranchAddress("trackID", &trackID);
  //tree -> SetBranchAddress("parentID", &parentID);
  tree -> SetBranchAddress("particleID", &particleID);
  tree -> SetBranchAddress("postCopyNb", &nextVolNum);
  tree -> SetBranchAddress("energy", &energy);
  //tree -> SetBranchAddress("px", &px);
  //tree -> SetBranchAddress("py", &py);
  //tree -> SetBranchAddress("pz", &pz);
  //tree -> SetBranchAddress("x", &x);
  //tree -> SetBranchAddress("y", &y);
  //tree -> SetBranchAddress("z", &z);

  // loop through tree and extract data
  int length = tree -> GetEntries(); // get total number of entries
  tree -> GetEntry(length - 1); // sets pointers to the last event
  cout << "last eventID: " << eventID << endl;

  for(int i = 0; i < length; i++){
    tree -> GetEntry(i);
    if(eventID < 10 || eventID%10000 == 0) cout << "event " << eventID << " is loaded" << endl;
    lastEventID = eventID;
    if (particleID == 22) { // if the particle is leaving the cloud
      NuSTAR_spectrum -> Fill(energy);
      XMM_spectrum -> Fill(energy);
      Joint_spectrum -> Fill(energy);
    }
  }

  // save to a file
  TFile *outFile = new TFile(outputFile,"RECREATE");
  // write histograms to file
  NuSTAR_spectrum -> Write("NuSTAR_Spect");
  XMM_spectrum -> Write("XMM_Spect");
  Joint_spectrum -> Write("Joint_Spect");
  delete outFile;
  return 0;
}
