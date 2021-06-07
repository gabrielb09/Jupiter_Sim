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

int Chandra()
{
  gROOT -> Reset();

 // adress for input file
  TFile *inFile;

 // adress for root tree stored in file
  TTree *tree;

  TH1D  *spectrum;

  // spectra
  spectrum = new TH1D();

  spectrum -> SetName("Full_Spect");

  spectrum -> SetTitle("Simulated X-Ray Spectrum");

  int nbins = 200;
  double e_min = 0.3;
  double e_max = 1014.0;


  spectrum -> SetBins(nbins, e_min, e_max);
  BinLogX(spectrum);


  // assign titles to the histogram axes
  spectrum -> GetXaxis() -> SetTitle("Energy [keV]");
  spectrum -> GetYaxis() -> SetTitle("#frac{dN}{dE} [counts/keV]");

  cout << "********* opening Spectra *********" << endl; // let me know that we are OPENING this bish

  // open data files
  inFile = new TFile("../output/final_simulation_extrapolate_low.root");

  // extract the trees
  tree = (TTree*) inFile -> Get("JupiterData");

  // set variables to hold data
  double energy;

  // setup the variable adresses for branches
  tree -> SetBranchAddress("energy", &energy);

  double max_E = 0.0;
  int length;
  // loop through trees and extract data
  length = tree -> GetEntries(); // get total number of entries

  for(int i = 0; i < length; i++){
    tree -> GetEntry(i);
    spectrum -> Fill(energy);
    if(energy > max_E){
      max_E = energy;
    }
  }

  // rescale
  double bin_height, bin_width, bin_center;

  for(int i = 0; i <= spectrum -> GetNbinsX(); i++){
      bin_height = spectrum -> GetBinContent(i);
      bin_width = spectrum -> GetBinWidth(i);
      spectrum -> SetBinContent(i, bin_height/bin_width);
  }

  cout << "Maximum Energy: " << max_E << endl;

  // create plot
  TCanvas *c1 = new TCanvas("c1", "c1", 2400, 1800);
  c1 -> SetLogx(); c1 -> SetLogy();

  spectrum -> SetStats(0);
  spectrum -> SetLineColor(kRed);
  spectrum -> Draw("SAME HIST");

  return 0;
}
