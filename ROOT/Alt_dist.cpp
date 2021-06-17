#include <math.h>
# define PI 3.14159265358979323846

using namespace std;

// generates a histogram of X-Ray counts by altitude
// also make histogram of X-Ray counts by altitude and energy

int Alt_dist()
{
  gROOT -> Reset();

  TFile *inFile; // adress for input file
  TTree *tree; // adress for root tree stored in file
  TH1D  *xray_hist; // adress for histogram

  int const num_bin = 100; // number of bins in a hist
  float Rj = 71492.0;
  float min, max, binSize; // maximum and minimun energy values and width of each bin

  xray_hist = new TH1D();
  xray_hist -> SetName("altitude_dist");
  xray_hist -> SetTitle("X-Ray Altitude Distribution");
  xray_hist -> SetBins(num_bin, 0, 1000.0);
  // assign titles to the histogram axes
	xray_hist -> GetXaxis() -> SetTitle("Altitude [km]");
	xray_hist -> GetYaxis() -> SetTitle("Fractional Counts");

  // file paths for input and output files
  const char* dir = "../output/"; // filepath from ROOT directory to output directory

  char outputFile[100]; // output file memory location
	sprintf(outputFile, "%s%s%s%s", dir, "plots/", "final_simulation" ,"_altitude.root"); // output file name

	char inputFile[100]; // input file memory location
  sprintf(inputFile, "%s%s%s", dir, "final_simulation", ".root"); // input file name and directory

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
  }

  double bin_height;

  for(int i = 0; i <= xray_hist -> GetNbinsX(); i++){
      bin_height = xray_hist -> GetBinContent(i);
      xray_hist -> SetBinContent(i, bin_height/total_counts);
  }

  FILE *out_file;
	// open the output file
	out_file = fopen("../../Figures/altitudes.csv", "w");
	// print header
	fprintf(out_file, "Number of bins:%d\n", xray_hist -> GetNbinsX());
	fprintf(out_file, "Bin Center[km],Bin Width[km],Counts\n");
	for (int i = 1; i <= xray_hist -> GetNbinsX(); i++) {
		fprintf(out_file, "%g,%g,%g\n", xray_hist -> GetBinCenter(i), xray_hist -> GetBinWidth(i), xray_hist -> GetBinContent(i));
	}
	fclose(out_file);

  return 0;
}
