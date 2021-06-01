#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <TMath.h>

int root2csv(){
	gROOT->Reset();
	// open data file
	TFile *inFile = new TFile("../output/final_simulation.root", "READ");
	TTree *tree;
	TH1D* spectrum;

	spectrum = new TH1D();
  spectrum -> SetName("Spectrum");
  spectrum -> SetTitle("X-Ray Spectrum");
  spectrum -> SetBins(192, 1.0, 80.0);
  // assign titles to the histogram axes
	spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
	spectrum -> GetYaxis() -> SetTitle("Counts");

	tree = (TTree*) inFile -> Get("JupiterData");
  tree -> Print();

	double energy;
	tree -> SetBranchAddress("energy", &energy);

	int length = tree -> GetEntries();
	
	for(int i = 0; i < length; i++){
    tree -> GetEntry(i);
		spectrum -> Fill(energy);
	}

	FILE *out_file;
	// open the output file
	out_file = fopen("spectra.csv", "w");
	// print header
	fprintf(out_file, "Number of bins:%d\n", spectrum -> GetNbinsX());
	fprintf(out_file, "Bin Center[keV],Bin Width[keV],Counts\n");
	for (int i = 1; i <= spectrum -> GetNbinsX(); i++) {
		fprintf(out_file, "%g,%g,%g\n", spectrum -> GetBinCenter(i), spectrum -> GetBinWidth(i), spectrum -> GetBinContent(i));
	}
	fclose(out_file);
	cout << "Output complete" << endl;
	return 0;
}
