#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <math.h>
# define PI 3.14159265358979323846
#include "getDate.cpp"

using namespace std;

// generates a photon spectrum spanning the NuSTAR energy range and with its approximate energy resolution

int NuSTAR_plot(const char filename[20])
{
  gROOT -> Reset();

  int const num_particles = 2; // expected number of unique particle species

  TFile *inFile; // adress for input file
  TTree *tree; // adress for root tree stored in file
  TH1F  *spectrum; // adress for array of histograms (one for each particle species)
  TH2F *angular_dist;

  int const spectral_bin = 192; // number of bins in a hist
  float min, max, binSize; // maximum and minimun energy values and width of each bin

  // spectrum
  spectrum = new TH1F();
  spectrum -> SetName("NuSTAR_Spect");
  spectrum -> SetTitle("NuSTAR X-Ray Spectrum");
  spectrum -> SetBins(192, 1.0, 80.0);
  // assign titles to the histogram axes
	spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
	spectrum -> GetYaxis() -> SetTitle("Counts");

  // angular distribution
  angular_dist = new TH2F("AngDist", "Angular Distribution 3 - 20 keV", 10000, -100000, 100000, 10000, -100000, 100000);
  angular_dist -> GetXaxis() -> SetTitle("X [km]");
	angular_dist -> GetYaxis() -> SetTitle("Y [km]");

  // file paths for input and output files
  const char* dir = "../output/"; // filepath from ROOT directory to output directory

  char outputFile[100]; // output file memory location
  getDate(); // get Date for file name
	sprintf(outputFile, "%s%s%s%s", dir, "plots/", filename, "_NuSTAR_full_res.root"); // output file name

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
  string process = "eBrem";
  string testProc;
  int eventID, trackID, particleID, nextVolNum, lastEventID;
  double energy;
  float px, py, pz, x, y, z;

  // setup the variable adresses for branches
  tree -> SetBranchAddress("eventID", &eventID);
  tree -> SetBranchAddress("trackID", &trackID);
  tree -> SetBranchAddress("particleID", &particleID);
  tree -> SetBranchAddress("postCopyNb", &nextVolNum);
  tree -> SetBranchAddress("energy", &energy);
  tree -> SetBranchAddress("px", &px);
  tree -> SetBranchAddress("py", &py);
  tree -> SetBranchAddress("pz", &pz);
  tree -> SetBranchAddress("x", &x);
  tree -> SetBranchAddress("y", &y);
  tree -> SetBranchAddress("z", &z);
  tree -> SetBranchAddress("creatorProcess", &creatorProcess);

  // loop through tree and extract data
  int num_NuSTAR = 0;
  int num_Ulysseys = 0;
  int lastTrack = -1;
  double electron_energy;

  int length = tree -> GetEntries(); // get total number of entries
  tree -> GetEntry(length - 1); // sets pointers to the last event
  cout << "last eventID: " << eventID << endl;

  for(int i = 0; i < length; i++){
    tree -> GetEntry(i);
    if(eventID < 10 || eventID%10000 == 0) cout << "event " << eventID << " is loaded" << endl;
    lastEventID = eventID;

    if (trackID != lastTrack){
      if (particleID == 11){
        electron_energy = energy;
      }
    }

    if (particleID == 22 && nextVolNum == -1) { // if the particle is leaving the cloud
      spectrum -> Fill(energy);
      angular_dist -> Fill(x, y);
      if((energy <= 21.2) && (energy >= 2.0)){
        num_NuSTAR += 1;
      }
      if((energy <= 48.0) && (energy >= 27.0)){
        num_Ulysseys += 1;

      }
    }
  }

  // save to a file
  TFile *outFile = new TFile(outputFile,"RECREATE");
  // write histograms to file
  spectrum -> Write("NuSTAR_Spect");
  angular_dist -> Write("Angular_Dist");
  cout << "Total number of X-Rays 4.72 - 21.2 keV: " << num_NuSTAR << endl;
  cout << "Total number of X-Rays 27.0 - 48.0 keV: " << num_Ulysseys << endl;
  delete outFile;
  return 0;
}
