#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <math.h>
# define PI 3.14159265358979323846
#include "getDate.cpp"

using namespace std;

// generates a photon spectrum spanning the NuSTAR energy range and with its approximate energy resolution

int energy_range(const char filename[20])
{
  gROOT -> Reset();

  int const num_particles = 2; // expected number of unique particle species

  TFile *inFile; // adress for input file
  TTree *tree; // adress for root tree stored in file
  const char* dir = "../output/";
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

  int num_500 = 0;
  int num_300 = 0;
  int num_100 = 0;
  int num_050 = 0;
  int total_num = 0;
  int num_e = 0;
  float e_energy;
  // loop through tree and extract data
  int length = tree -> GetEntries(); // get total number of entries
  tree -> GetEntry(length - 1); // sets pointers to the last event
  cout << "last eventID: " << eventID << endl;

  for(int i = 0; i < length; i++){
    tree -> GetEntry(i);
    if(eventID < 10 || eventID%10000 == 0) cout << "event " << eventID << " is loaded" << endl;
    lastEventID = eventID;
    if (particleID == 11 && x == 0.0) {
      e_energy = energy;
      num_e += 1;
    }
    if (particleID == 22 && nextVolNum == -1){ // if the particle is leaving the cloud
      if (1.0 <= energy  && energy <= 20.0 ){
        total_num += 1;
        if (e_energy > 500.0) {
          num_500 += 1;
          num_300 += 1;
          num_100 += 1;
          num_050 += 1;
        }
        else if (e_energy > 300.0) {
          num_300 += 1;
          num_100 += 1;
          num_050 += 1;
        }
        else if (e_energy > 100.0) {
          num_100 += 1;
          num_050 += 1;
        }
        else if (e_energy > 50.0) {
          num_050 += 1;
        }
      }
    }
  }
  cout << "Total Electrons: " << num_e << endl;
  cout << "Total Photons: " << total_num << endl;
  cout << "Percent from > 500 keV " << 100.0*double(num_500)/double(total_num) << "%" << endl;
  cout << "Percent from > 300 keV " << 100.0*double(num_300)/double(total_num) << "%" << endl;
  cout << "Percent from > 100 keV " << 100.0*double(num_100)/double(total_num) << "%" << endl;
  cout << "Percent from >  50 keV " << 100.0*double(num_050)/double(total_num) << "%" << endl;
  return 0;
}
