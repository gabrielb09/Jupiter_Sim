#include <string>
#include <cstring>
#include <map>
#include <iostream>
#include <math.h>
# define PI 3.14159265358979323846
#include "getDate.cpp"

using namespace std;

// generates a spectrum with the same binning as NuSTAR observations of Jupiter

int Observation()
{
    gROOT -> Reset();

    TFile *inFile = new TFile("../output/5MeV_simulation_soft.root", "READ");
    FILE *ns_out_file = fopen("../../Figures/Spectra/5MeV_soft_ns.csv", "w");
    FILE *xmm_out_file = fopen("../../Figures/Spectra/5MeV_soft_xmm.csv", "w");
	TTree *tree;
    TH1D  *NuSTAR_spectrum; // adress for array of histograms (one for each particle species)
    TH1D  *XMM_spectrum;

    int const NuSTAR_bin_num = 27; // number of bins in a hist
    Double_t NuSTAR_edges[28] = {4.72, 5.04, 6.12, 6.48, 6.84, 6.92, 7.12, 7.32, 7.52,
                               7.92, 8.0, 8.76, 8.96, 9.48, 9.76, 10.24, 11.04, 11.12,
                               11.4, 11.52, 13.04, 14.56, 15.88, 16.36, 17.28, 18.08, 20.32, 21.2};
    int const XMM_bin_num = 9; // number of bins in a hist
    Double_t XMM_edges[10] = {3.07, 3.37, 3.84, 4.495, 5.015, 5.835, 5.955, 6.22, 6.68, 9.975};

    NuSTAR_spectrum = new TH1D("NuSTAR_Spect", "NuSTAR Observation Simulation", NuSTAR_bin_num, NuSTAR_edges);
    // assign titles to the histogram axes
    NuSTAR_spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
    NuSTAR_spectrum -> GetYaxis() -> SetTitle("Counts");

    XMM_spectrum = new TH1D("XMM_Spect", "XMM Observation Simulation", XMM_bin_num, XMM_edges);
    XMM_spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
    XMM_spectrum -> GetYaxis() -> SetTitle("Counts");

    // extract the tree
    tree = (TTree*) inFile -> Get("X_Rays");
    tree -> Print();
    // set variables to hold data
    double energy;
	tree -> SetBranchAddress("energy", &energy);

    // loop through tree and extract data
    int length = tree -> GetEntries(); // get total number of entries

    for(int i = 0; i < length; i++){
        tree -> GetEntry(i);
        NuSTAR_spectrum -> Fill(energy);
        XMM_spectrum -> Fill(energy);
    }

    fprintf(ns_out_file, "Number of bins:%d\n", NuSTAR_spectrum -> GetNbinsX());
	fprintf(ns_out_file, "Bin Center[keV],Bin Width[keV],Counts\n");
    fprintf(xmm_out_file, "Number of bins:%d\n", XMM_spectrum -> GetNbinsX());
	fprintf(xmm_out_file, "Bin Center[keV],Bin Width[keV],Counts\n");
	for (int i = 1; i <= NuSTAR_spectrum -> GetNbinsX(); i++) {
		fprintf(ns_out_file, "%g,%g,%g\n", NuSTAR_spectrum -> GetBinCenter(i), NuSTAR_spectrum -> GetBinWidth(i), NuSTAR_spectrum -> GetBinContent(i));
	}
    for (int i = 1; i <= XMM_spectrum -> GetNbinsX(); i++) {
		fprintf(xmm_out_file, "%g,%g,%g\n", XMM_spectrum -> GetBinCenter(i), XMM_spectrum -> GetBinWidth(i), XMM_spectrum -> GetBinContent(i));
	}
	fclose(ns_out_file);
    fclose(xmm_out_file);
	cout << "Output complete" << endl;
	return 0;
}
