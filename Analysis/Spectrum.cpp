using namespace std;

// generates an X-Ray spectrum
void BinLogX(TH1*h)
{
    TAxis *axis = h -> GetXaxis();
    int bins = axis -> GetNbins();

    Axis_t min = log10(axis -> GetXmin());
    Axis_t max = log10(axis -> GetXmax());
    Axis_t width = (max - min) / bins;
    Axis_t *new_bins = new Axis_t[bins + 1];

    for (int i = 0; i <= bins; i++) {
        new_bins[i] = pow(10, min + i*width);
    }
    axis -> Set(bins, new_bins);
    delete new_bins;
}

int Spectrum()
{
    gROOT -> Reset();

    TFile *inFile = new TFile("../output/final_simulation.root"); // output file
    FILE *out_file = fopen("../../Figures/spectrum.csv", "w");
    TTree *tree = (TTree*) inFile -> Get("X_Rays"); // Root Tree
    TH1D  *spectrum = new TH1D(); // spectrum histogram

    spectrum -> SetName("Spectrum");
    spectrum -> SetTitle("X-Ray Spectrum");
    spectrum -> SetBins(150, 1.0, 1014.0);
    BinLogX(spectrum);
    // assign titles to the histogram axes
	spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
	spectrum -> GetYaxis() -> SetTitle("Counts/keV");

    tree -> Print();
    // create variables to hold necessary data
    double energy;

    // setup the variable adresses for branches
    tree -> SetBranchAddress("energy", &energy);

    // loop through tree and extract data
    int length = tree -> GetEntries(); // get total number of entries

    for(int i = 0; i < length; i++){
        tree -> GetEntry(i); // get the ith entry
        spectrum -> Fill(energy);
    }

    // save spectrum to csv
	fprintf(out_file, "Number of bins:%d\n", spectrum -> GetNbinsX());
	fprintf(out_file, "Bin Center[keV],Bin Width[keV],Counts\n");
	for (int i = 1; i <= spectrum -> GetNbinsX(); i++) {
		fprintf(out_file, "%g,%g,%g\n", spectrum -> GetBinCenter(i), spectrum -> GetBinWidth(i), spectrum -> GetBinContent(i));
	}
	fclose(out_file);

    // plot results for our benefit, but with some normalization
    double bin_height, bin_width, bin_center;

    for(int i = 0; i <= spectrum -> GetNbinsX(); i++){
        bin_height = spectrum -> GetBinContent(i);
        bin_width = spectrum -> GetBinWidth(i);
        spectrum -> SetBinContent(i, bin_height/bin_width);
    }
    TCanvas *c1 = new TCanvas("c1", "c1", 2400, 1800);
    c1 -> SetLogx(); c1 -> SetLogy();
    spectrum -> Draw("SAME HIST");

    return 0;
}
