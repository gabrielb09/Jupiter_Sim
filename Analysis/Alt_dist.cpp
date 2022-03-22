#include <math.h>
# define PI 3.14159265358979323846

using namespace std;

// generates a histogram of X-Ray counts by altitude

int Alt_dist()
{
    gROOT -> Reset();

    float Rj = 71492.0;

    TFile *inFile = new TFile("../output/final_simulation.root"); // output file
    FILE *out_file = fopen("../../Figures/altitudes.csv", "w");
    TTree *tree = (TTree*) inFile -> Get("X_Rays"); // Root Tree
    TH1D  *xray_hist = new TH1D(); // Altitude histogram

    int const num_bin = 100; // number of bins in a hist
    xray_hist -> SetName("altitude_dist");
    xray_hist -> SetTitle("X-Ray Altitude Distribution");
    xray_hist -> SetBins(num_bin, 0, 1000.0);
    // assign titles to the histogram axes
	xray_hist -> GetXaxis() -> SetTitle("Altitude [km]");
	xray_hist -> GetYaxis() -> SetTitle("Fractional Counts");

    tree -> Print();
    // create variables to hold necessary data
    double vertex_x, vertex_y, vertex_z, energy;

    // setup the variable adresses for branches
    tree -> SetBranchAddress("energy", &energy);
    tree -> SetBranchAddress("vertex_x", &vertex_x);
    tree -> SetBranchAddress("vertex_y", &vertex_y);
    tree -> SetBranchAddress("vertex_z", &vertex_z);

    // loop through tree and extract data
    int length = tree -> GetEntries(); // get total number of entries
    int total_counts = 0;
    double radius;

    for(int i = 0; i < length; i++){
        tree -> GetEntry(i); // get the ith entry
        if (energy >= 2.0) { // only care about photons in our energy range
          radius = sqrt((vertex_x*vertex_x) + (vertex_y*vertex_y) + (vertex_z*vertex_z));
          xray_hist -> Fill(radius - Rj);
          total_counts += 1;
        }
    }

    // renormalize the distribution
    double bin_height;
    for(int i = 0; i <= xray_hist -> GetNbinsX(); i++){
        bin_height = xray_hist -> GetBinContent(i);
        xray_hist -> SetBinContent(i, bin_height/total_counts);
    }

    // print header
    fprintf(out_file, "Number of bins:%d\n", xray_hist -> GetNbinsX());
    fprintf(out_file, "Bin Center[km],Bin Width[km],Counts\n");
    for (int i = 1; i <= xray_hist -> GetNbinsX(); i++) {
    	fprintf(out_file, "%g,%g,%g\n", xray_hist -> GetBinCenter(i), xray_hist -> GetBinWidth(i), xray_hist -> GetBinContent(i));
    }
    fclose(out_file);

    // plot results for our benefit
    TCanvas *c1 = new TCanvas("c1", "c1", 2400, 1800);
    xray_hist -> Draw("SAME HIST");

    return 0;
}
