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

int Figure_6()
{
  gROOT -> Reset();

  TFile *inFile_hrd; // adress for input file
  TFile *inFile_avg;
  TFile *inFile_sft;

  TTree *tree_hrd; // adress for root tree stored in file
  TTree *tree_avg;
  TTree *tree_sft;

  TH1D  *spectrum_hrd;
  TH1D  *spectrum_avg;
  TH1D  *spectrum_sft;

  // spectra
  spectrum_hrd = new TH1D();
  spectrum_avg = new TH1D();
  spectrum_sft = new TH1D();

  spectrum_hrd -> SetName("Full_Spect_Hard");
  spectrum_avg -> SetName("Full_Spect_Average");
  spectrum_sft -> SetName("Full_Spect_Soft");

  spectrum_hrd -> SetTitle("Simulated X-Ray Spectrum");
  spectrum_avg -> SetTitle("Simulated X-Ray Spectrum");
  spectrum_sft -> SetTitle("Simulated X-Ray Spectrum");

  int nbins = 200;
  double e_min = 1.0;
  double e_max = 1014.0;

  spectrum_hrd -> SetBins(nbins, e_min, e_max);
  BinLogX(spectrum_hrd);
  spectrum_avg -> SetBins(nbins, e_min, e_max);
  BinLogX(spectrum_avg);
  spectrum_sft -> SetBins(nbins, e_min, e_max);
  BinLogX(spectrum_sft);

  // assign titles to the histogram axes
  spectrum_hrd -> GetXaxis() -> SetTitle("Energy [keV]");
  spectrum_hrd -> GetYaxis() -> SetTitle("#frac{dN}{dE} [counts/keV]");
  spectrum_avg -> GetXaxis() -> SetTitle("Energy [keV]");
  spectrum_avg -> GetYaxis() -> SetTitle("#frac{dN}{dE} [counts/keV]");
  spectrum_sft -> GetXaxis() -> SetTitle("Energy [keV]");
  spectrum_sft -> GetYaxis() -> SetTitle("#frac{dN}{dE} [counts/keV]");

  // file paths for input and output files
  const char* dir = "../output/"; // filepath from ROOT directory to output directory

  char outputFile[100]; // output file memory location
	sprintf(outputFile, "%s%s%s", dir, "plots/", "full_spectrum.root"); // output file name

  cout << "********* opening Spectra *********" << endl; // let me know that we are OPENING this bish

  // open data files
  inFile_hrd = new TFile("../output/final_simulation_hard.root");
  inFile_avg = new TFile("../output/final_simulation.root");
  inFile_sft = new TFile("../output/final_simulation_soft.root");

  // extract the trees
  tree_hrd = (TTree*) inFile_hrd -> Get("JupiterData");
  tree_avg = (TTree*) inFile_avg -> Get("JupiterData");
  tree_sft = (TTree*) inFile_sft -> Get("JupiterData");

  // set variables to hold data
  double energy_hrd;
  double energy_avg;
  double energy_sft;

  // setup the variable adresses for branches
  tree_hrd -> SetBranchAddress("energy", &energy_hrd);
  tree_avg -> SetBranchAddress("energy", &energy_avg);
  tree_sft -> SetBranchAddress("energy", &energy_sft);

  double max_E = 0.0;
  int length;
  // loop through trees and extract data
  length = tree_hrd -> GetEntries(); // get total number of entries

  for(int i = 0; i < length; i++){
    tree_hrd -> GetEntry(i);
    spectrum_hrd -> Fill(energy_hrd);
    if(energy_hrd > max_E){
      max_E = energy_hrd;
    }
  }

  length = tree_avg -> GetEntries();

  for(int i = 0; i < length; i++){
    tree_avg -> GetEntry(i);
    spectrum_avg -> Fill(energy_avg);
    if(energy_avg > max_E){
      max_E = energy_avg;
    }
  }

  length = tree_sft -> GetEntries();

  for(int i = 0; i < length; i++){
    tree_sft -> GetEntry(i);
    spectrum_sft -> Fill(energy_sft);
    if(energy_sft > max_E){
      max_E = energy_sft;
    }
  }

  // rescale
  double bin_height, bin_width, bin_center;

  for(int i = 0; i <= spectrum_avg -> GetNbinsX(); i++){
      bin_height = spectrum_avg -> GetBinContent(i);
      bin_width = spectrum_avg -> GetBinWidth(i);
      spectrum_avg -> SetBinContent(i, bin_height/bin_width);

      bin_height = spectrum_hrd -> GetBinContent(i);
      bin_width = spectrum_hrd -> GetBinWidth(i);
      spectrum_hrd -> SetBinContent(i, bin_height/bin_width);

      bin_height = spectrum_sft -> GetBinContent(i);
      bin_width = spectrum_sft -> GetBinWidth(i);
      spectrum_sft -> SetBinContent(i, bin_height/bin_width);
  }

  // save to a file
  TFile *outFile = new TFile(outputFile,"RECREATE");
  // write histograms to file
  spectrum_hrd -> Write("Spect_Hard");
  spectrum_avg -> Write("Spect_Average");
  spectrum_sft -> Write("Spect_Soft");
  cout << "Maximum Energy: " << max_E << endl;
  delete outFile;

  // create plot
  TCanvas *c1 = new TCanvas("c1", "c1", 2400, 1800);
  c1 -> SetLogx(); c1 -> SetLogy();

  spectrum_hrd -> SetStats(0);
  spectrum_avg -> SetStats(0);
  spectrum_sft -> SetStats(0);

  spectrum_hrd -> SetLineColor(kRed);
  spectrum_hrd -> Draw("SAME HIST");

  spectrum_avg -> SetLineColor(kBlack);
  spectrum_avg -> Draw("SAME HIST");

  spectrum_sft -> SetLineColor(kBlue);
  spectrum_sft -> Draw("SAME HIST");

  return 0;
}
