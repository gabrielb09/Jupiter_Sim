#include <math.h>
# define PI 3.14159265358979323846

using namespace std;

// generates a histogram of X-Ray counts by altitude
// also make histogram of X-Ray counts by altitude and energy

int Alt_dist(const char filename[20])
{
  gROOT -> Reset();

  TFile *inFile; // adress for input file
  TTree *tree; // adress for root tree stored in file
  TH1D  *xray_hist; // adress for histogram
  TH1D  *edep_hist;

  int const num_bin = 1000; // number of bins in a hist
  float Rj = 71492.0;
  float min, max, binSize; // maximum and minimun energy values and width of each bin

  xray_hist = new TH1D();
  xray_hist -> SetName("altitude_dist");
  xray_hist -> SetTitle("X-Ray Altitude Distribution");
  xray_hist -> SetBins(num_bin, 0, 1000.0);
  // assign titles to the histogram axes
	xray_hist -> GetXaxis() -> SetTitle("Altitude [km]");
	xray_hist -> GetYaxis() -> SetTitle("Fractional Counts");

  edep_hist = new TH1D();
  edep_hist -> SetName("edep_dist");
  edep_hist -> SetTitle("Deposited Energy Altitude Distribution");
  edep_hist -> SetBins(num_bin, 0, 1000.0);
  // assign titles to the histogram axes
	edep_hist -> GetXaxis() -> SetTitle("Altitude [km]");
	edep_hist -> GetYaxis() -> SetTitle("Fractional Energy Deposition");

  // file paths for input and output files
  const char* dir = "../output/"; // filepath from ROOT directory to output directory

  char outputFile[100]; // output file memory location
	sprintf(outputFile, "%s%s%s%s", dir, "plots/", filename ,"_altitude.root"); // output file name

	char inputFile[100]; // input file memory location
  sprintf(inputFile, "%s%s%s", dir, filename, ".root"); // input file name and directory

  cout << "********* opening " << inputFile << " *********" << endl; // let me know that we are OPENING this bish

  // open data file
  inFile = (new TFile(inputFile));
  // extract the tree
  tree = (TTree*) inFile -> Get("JupiterData");
  tree -> Print();
  // set variables to hold data
  int trackID, parentID, nextVolNum, particleID;
  double x, y, z, vertex_x, vertex_y, vertex_z, energy, edep;

  // setup the variable adresses for branches
  tree -> SetBranchAddress("trackID", &trackID);
  tree -> SetBranchAddress("particleID", &particleID);
  tree -> SetBranchAddress("postCopyNb", &nextVolNum);
  tree -> SetBranchAddress("energy", &energy);
  tree -> SetBranchAddress("eDep", &edep);
  tree -> SetBranchAddress("x", &x);
  tree -> SetBranchAddress("y", &y);
  tree -> SetBranchAddress("z", &z);
  tree -> SetBranchAddress("vertex_x", &vertex_x);
  tree -> SetBranchAddress("vertex_y", &vertex_y);
  tree -> SetBranchAddress("vertex_z", &vertex_z);

  // loop through tree and extract data
  int length = tree -> GetEntries(); // get total number of entries

  int total_counts = 0;
  double total_edep = 0.0;

  double radius;
  string bar;

  for(int i = 0; i < length; i++){
    bar = "";
    for(int j = 0; j < (100*i/(length - 1))/5; j++){
      bar += "#";
    }
    cout << "\r" << setprecision(4) << 100.0*float(i)/float(length - 1) << "%  " << bar << flush;

    tree -> GetEntry(i);
    if (particleID == 22 && nextVolNum == -1 && energy >= 2.0) { // if photon is leaving the atmoshpere
      radius = sqrt((vertex_x*vertex_x) + (vertex_y*vertex_y) + (vertex_z*vertex_z));
      xray_hist -> Fill(radius - Rj);
      total_counts += 1;
    }

    radius = sqrt((x*x) + (y*y) + (z*z));
    edep_hist -> Fill(radius - Rj, edep);

  }

  // save to a file
  TFile *outFile = new TFile(outputFile,"RECREATE");
  // write histograms to file
  xray_hist -> Write("Altitude_Spect");
  edep_hist -> Write("EDep_Spect");
  delete outFile;
  return 0;
}
