#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <math.h>
# define PI 3.14159265358979323846
#include "getDate.cpp"

using namespace std;

// generates a photon spectrum spanning the NuSTAR energy range and with its approximate energy resolution

int Plot_Full(const char filename[20])
{
  gROOT -> Reset();

  int const num_particles = 2; // expected number of unique particle species

  TFile *inFile; // adress for input file
  TTree *tree; // adress for root tree stored in file
  TH1F  *spectrum; // adress for array of histograms (one for each particle species)

  int const spectral_bin = 192; // number of bins in a hist
  float min, max, binSize; // maximum and minimun energy values and width of each bin

  // spectrum
  spectrum = new TH1F();
  spectrum -> SetName("Full_Spect");
  spectrum -> SetTitle("#gamma Spectrum");
  spectrum -> SetBins(1000, 1.0, 585.0);
  // assign titles to the histogram axes
	spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
	spectrum -> GetYaxis() -> SetTitle("Counts");

  // file paths for input and output files
  const char* dir = "../output/"; // filepath from ROOT directory to output directory

  char outputFile[100]; // output file memory location
  getDate(); // get Date for file name
	sprintf(outputFile, "%s%s%s%s", dir, "plots/", filename, "_full_spectrum.root"); // output file name

	char inputFile[100]; // input file memory location
  sprintf(inputFile, "%s%s%s", dir, filename, ".root"); // input file name and directory

  cout << "********* opening " << inputFile << " *********" << endl; // let me know that we are OPENING this bish

  // open data file
  inFile = (new TFile(inputFile));
  // extract the tree
  tree = (TTree*) inFile -> Get("JupiterData");
  tree -> Print();
  // set variables to hold data
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

  double max_E = 0.0;
  int num_uly = 0;
  // loop through tree and extract data
  int length = tree -> GetEntries(); // get total number of entries
  tree -> GetEntry(length - 1); // sets pointers to the last event
  cout << "last eventID: " << eventID << endl;

  for(int i = 0; i < length; i++){
    tree -> GetEntry(i);
    if(eventID < 10 || eventID%10000 == 0) cout << "event " << eventID << " is loaded" << endl;
    lastEventID = eventID;
    if (particleID == 22 && nextVolNum == -1) { // if the particle is leaving the cloud
      spectrum -> Fill(energy);
      if(energy > max_E){
        max_E = energy;
      }
      if ((energy > 27.0) && (energy < 48.0)){
        num_uly += 1;
      }
    }
  }

  // save to a file
  TFile *outFile = new TFile(outputFile,"RECREATE");
  // write histograms to file
  spectrum -> Write("Full_Spect");
  cout << "Maximum Energy: " << max_E << endl;
  cout << "Number of photons in Ulysseys range: " << num_uly << endl;
  delete outFile;
  return 0;
}
