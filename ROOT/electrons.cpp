#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <math.h>
# define PI 3.14159265358979323846
#include "getDate.cpp"

using namespace std;

// generates a photon spectrum spanning the NuSTAR energy range and with its approximate energy resolution

int electrons(const char filename[20])
{
  gROOT -> Reset();

  TFile *inFile; // adress for input file
  TTree *tree; // adress for root tree stored in file
  TH1D  *spectrum; // adress for array of histograms (one for each particle species)

  int const spectral_bin = 192; // number of bins in a hist
  float min, max, binSize; // maximum and minimun energy values and width of each bin

  // Juno Bins
  Double_t Juno_E[42] = {3.144832, 3.508336, 3.884463, 4.4365850, 4.9552660, 5.6309380, 6.3230710,
                         7.141655, 8.039168, 9.073550, 10.211496, 11.549773, 13.000467, 14.638207,
                         16.515199, 18.636033, 21.056811, 23.692213, 26.747727, 30.188629, 34.066445,
                         38.421746, 43.387512, 48.922305, 55.331812, 62.432383, 70.553359, 79.752859,
                         90.211672, 101.894273, 130.1400, 156.2800, 188.6100, 226.7200, 271.6800,
                         325.0800, 388.5900, 464.9700, 557.7600, 672.0600, 815.9700, 1185.4600};

  // spectrum
  spectrum = new TH1D("Electron_Spect", "Source Electron Spectrum", 41, Juno_E);
  // assign titles to the histogram axes
	spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
	spectrum -> GetYaxis() -> SetTitle("Counts");

  // file paths for input and output files
  const char* dir = "../output/"; // filepath from ROOT directory to output directory

  char outputFile[100]; // output file memory location
  getDate(); // get Date for file name
	sprintf(outputFile, "%s%s%s%s", dir, "plots/", filename, "_electrons.root"); // output file name

	char inputFile[100]; // input file memory location
  sprintf(inputFile, "%s%s%s", dir, filename, ".root"); // input file name and directory

  cout << "********* opening " << inputFile << " *********" << endl; // let me know that we are OPENING this bish

  // open data file
  inFile = (new TFile(inputFile));
  // extract the tree
  tree = (TTree*) inFile -> Get("JupiterData");
  tree -> Print();
  // set variables to hold data
  char creatorProcess[5];
  int eventID, trackID, particleID, nextVolNum, lastEventID;
  double energy;
  float px, py, pz, x, y, z;

  // setup the variable adresses for branches
  tree -> SetBranchAddress("eventID", &eventID);
  tree -> SetBranchAddress("particleID", &particleID);
  tree -> SetBranchAddress("postCopyNb", &nextVolNum);
  tree -> SetBranchAddress("energy", &energy);
  tree -> SetBranchAddress("px", &px);
  tree -> SetBranchAddress("py", &py);
  tree -> SetBranchAddress("pz", &pz);
  tree -> SetBranchAddress("x", &x);
  tree -> SetBranchAddress("y", &y);
  tree -> SetBranchAddress("z", &z);

  // loop through tree and extract data
  int length = tree -> GetEntries(); // get total number of entries
  tree -> GetEntry(length - 1); // sets pointers to the last event
  cout << "last eventID: " << eventID << endl;

  for(int i = 0; i < length; i++){
    tree -> GetEntry(i);
    if(eventID < 10 || eventID%10000 == 0) cout << "event " << eventID << " is loaded" << endl;
    lastEventID = eventID;
    if (particleID == 11 && nextVolNum == 1) { // if the particle is leaving the cloud
      spectrum -> Fill(energy);
    }
  }

  // save to a file
  TFile *outFile = new TFile(outputFile,"RECREATE");
  // write histograms to file
  spectrum -> Write("Electron_Spect");
  delete outFile;
  return 0;
}
