#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <math.h>
# define PI 3.14159265358979323846
#include "getDate.cpp"

using namespace std;

// generates a photon spectrum spanning the NuSTAR energy range and with its approximate energy resolution

int flux_ratio(const char filename[20], const double energy_min)
{
  gROOT -> Reset();

  int const num_particles = 2; // expected number of unique particle species

  TFile *inFile; // adress for input file
  TTree *tree; // adress for root tree stored in file
  TH1F  *spectrum; // adress for array of histograms (one for each particle species)
  TH2F *angular_dist;

  // file paths for input and output files
  const char* dir = "../output/"; // filepath from ROOT directory to output directory

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
  tree -> SetBranchAddress("particleID", &particleID);
  tree -> SetBranchAddress("postCopyNb", &nextVolNum);
  tree -> SetBranchAddress("energy", &energy);
  tree -> SetBranchAddress("px", &px);
  tree -> SetBranchAddress("py", &py);
  tree -> SetBranchAddress("pz", &pz);
  tree -> SetBranchAddress("x", &x);
  tree -> SetBranchAddress("y", &y);
  tree -> SetBranchAddress("z", &z);
  //tree -> SetBranchAddress("creatorProcess", &creatorProcess);

  // loop through tree and extract data
  int num_gamma = 0;
  int num_primaries = 0;
  bool inRange = false;
  int length = tree -> GetEntries(); // get total number of entries
  tree -> GetEntry(length - 1); // sets pointers to the last event
  cout << "last eventID: " << eventID << endl;
  lastEventID = -1;
  int num_events = 0;

  for(int i = 0; i < length; i++){
    tree -> GetEntry(i);
    if((eventID < 10 || (eventID < 100 &&  eventID%10 == 0) ||
       (eventID < 1000 &&  eventID%100 == 0) || (eventID < 10000 &&  eventID%1000 == 0) ||
       (eventID < 100000 &&  eventID%10000 == 0) || (eventID < 500000 &&  eventID%100000 == 0) ||
       eventID%500000 == 0)
      && (lastEventID != eventID)) cout << "event " << eventID << " is loaded" << endl;
    if (lastEventID != eventID){
      num_events += 1;
      inRange = false;
      if (particleID == 11 && energy >= energy_min){
        inRange = true;
        num_primaries += 1;
      }
    }
    lastEventID = eventID;


    if (particleID == 22 && nextVolNum == -1 && inRange) { // if the particle is leaving the cloud
      if((energy <= 21.2) && (energy >= 2.0)){
        num_gamma += 1;
      }
    }
  }

  cout << num_events << endl;
  cout << "Total number of primaries with E >= " << energy_min << " keV: " << num_primaries << endl;
  cout << "Total number of X-Rays 4.72 - 21.2 keV: " << num_gamma << endl;
  cout << "Ratio: " << (float)num_gamma/(float)num_primaries << endl;
  return 0;
}
