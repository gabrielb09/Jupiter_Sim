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
/*
void BrokenPwrLaw(TF1 *func, Double_t xmin, Double_t xmax, TH1 *h)
{
  func -> SetFunction(TFormula("[0]*pow(x/[1], -1*[2])*pow(0.5*(1+pow(x/[1], 1/[4])), ([2] - [3])*[4])"));
  func -> SetRange(xmin, xmax);
  func -> SetParName(0, "Amplitude");
  h -> GetXaxis() -> SetRangeUser(xmin,xmax);
  func -> SetParLimits(0, h -> GetMinimum(), h -> GetMaximum());
  h -> GetXaxis() -> SetRange(0,0);
  func -> SetParName(1, "E_break");
  func -> SetParLimits(1, xmin, xmax);
  func -> SetParName(2, "alpha_1");
  func -> SetParLimits(2, -5, 5);
  func -> SetParName(3, "alpha_2");
  func -> SetParLimits(3, -5, 5);
  func -> SetParName(4, "delta");
  func -> SetParLimits(4, 1e-4, 10);
}

void PwrLaw(TF1 *func, Double_t xmin, Double_t xmax, TH1 *h)
{
  func -> SetFunction(TFormula("[0]*pow(x/[1], -1*[2])"));
  func -> SetParName(0, "Amplitude");
  h -> GetXaxis() -> SetRangeUser(xmin,xmax);
  func -> SetParLimits(0, h -> GetMinimum(), h -> GetMaximum());
  h -> GetXaxis() -> SetRange(0,0);
  func -> SetParName(1, "x_0");
  func -> SetParLimits(1, xmin, xmax);
  func -> SetParName(2, "alpha_1");
  func -> SetParLimits(2, -5, 5);
}
*/

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

  int nbins = 150;
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

  spectrum_hrd ->SetMinimum(50.);

  spectrum_hrd -> SetLineColor(kBlue);
  spectrum_hrd -> SetLineWidth(1);
  spectrum_hrd -> Draw("SAME E");

  spectrum_avg -> SetLineColor(kBlack);
  spectrum_avg -> SetLineWidth(1);
  spectrum_avg -> Draw("SAME E");

  spectrum_sft -> SetLineColor(kOrange);
  spectrum_sft -> SetLineWidth(1);
  spectrum_sft -> Draw("SAME E");

/*
  TF1 *pwrLawSft = new TF1();
  TF1 *brknPwrLawSft = new TF1();
  PwrLaw(pwrLawSft, 3.0, 20.0, spectrum_sft);
  BrokenPwrLaw(brknPwrLawSft, 3.0, 20.0, spectrum_sft);

  TF1 *pwrLawAvg = new TF1();
  TF1 *brknPwrLawAvg = new TF1();
  PwrLaw(pwrLawAvg, 3.0, 20.0, spectrum_avg);
  BrokenPwrLaw(brknPwrLawAvg, 3.0, 20.0, spectrum_avg);

  TF1 *pwrLawHrd = new TF1();
  TF1 *brknPwrLawHrd = new TF1();
  PwrLaw(pwrLawHrd, 3.0, 20.0, spectrum_hrd);
  BrokenPwrLaw(brknPwrLawHrd, 3.0, 20.0, spectrum_hrd);

  */
  double xmin = 3.0;
  double xmax = 20.0;
  TF1 *brknPwrLaw = new TF1("brknPwrLaw", "[0]*pow(x/[1], -1*[2])*pow(0.5*(1+pow(x/[1], 1/[4])), ([2] - [3])*[4])", xmin, xmax);
  brknPwrLaw -> SetParName(0, "Amplitude");
  spectrum_sft -> GetXaxis() -> SetRangeUser(xmin, xmax);
  spectrum_hrd -> GetXaxis() -> SetRangeUser(xmin, xmax);
  brknPwrLaw -> SetParLimits(0, spectrum_sft -> GetMinimum(), spectrum_hrd -> GetMaximum());
  spectrum_sft -> GetXaxis() -> SetRange(0,0);
  spectrum_hrd -> GetXaxis() -> SetRange(0,0);
  brknPwrLaw -> SetParName(1, "E_break");
  brknPwrLaw -> SetParLimits(1, xmin, xmax);
  brknPwrLaw -> SetParName(2, "alpha_1");
  brknPwrLaw -> SetParLimits(2, -5, 5);
  brknPwrLaw -> SetParName(3, "alpha_2");
  brknPwrLaw -> SetParLimits(3, -5, 5);
  brknPwrLaw -> SetParName(4, "delta");
  brknPwrLaw -> SetParLimits(4, 1e-4, 10);

  TF1 *pwrLaw = new TF1("pwrLaw", "[0]*pow(x/[1], -1*[2])", xmin, xmax);
  pwrLaw -> SetParName(0, "Amplitude");
  spectrum_sft -> GetXaxis() -> SetRangeUser(xmin, xmax);
  spectrum_hrd -> GetXaxis() -> SetRangeUser(xmin, xmax);
  pwrLaw -> SetParLimits(0, spectrum_sft -> GetMinimum(), spectrum_hrd -> GetMaximum());
  spectrum_sft -> GetXaxis() -> SetRange(0,0);
  spectrum_hrd -> GetXaxis() -> SetRange(0,0);
  pwrLaw -> SetParName(1, "E_0");
  pwrLaw -> SetParLimits(1, xmin, xmax);
  pwrLaw -> SetParName(2, "alpha");
  pwrLaw -> SetParLimits(2, -5, 5);

  spectrum_hrd -> Fit(brknPwrLaw, "0", "", 3.0, 20.0);
  spectrum_hrd -> Fit(pwrLaw, "0", "", 3.0, 20.0);
  TF1 *hrd_pl = (TF1*)(pwrLaw -> Clone("hrd_pl"));
  TF1 *hrd_bpl = (TF1*)(brknPwrLaw -> Clone("hrd_bpl"));
  hrd_pl -> SetLineColor(kRed);
  hrd_pl -> DrawF1(3.0, 20.0, "CSAME");
  hrd_bpl -> SetLineColor(kRed);
  hrd_bpl -> DrawF1(3.0, 20.0, "CSAME");

  spectrum_avg -> Fit(brknPwrLaw, "0", "", 3.0, 20.0);
  spectrum_avg -> Fit(pwrLaw, "0", "", 3.0, 20.0);
  TF1 *avg_pl = (TF1*)(pwrLaw -> Clone("avg_pl"));
  TF1 *avg_bpl = (TF1*)(brknPwrLaw -> Clone("avg_bpl"));
  avg_pl -> SetLineColor(kRed);
  avg_pl -> DrawF1(3.0, 20.0, "CSAME");
  avg_bpl -> SetLineColor(kRed);
  avg_bpl -> DrawF1(3.0, 20.0, "CSAME");

  spectrum_sft -> Fit(brknPwrLaw, "0", "", 3.0, 20.0);
  spectrum_sft -> Fit(pwrLaw, "0", "", 3.0, 20.0);
  TF1 *sft_pl = (TF1*)(pwrLaw -> Clone("sft_pl"));
  TF1 *sft_bpl = (TF1*)(brknPwrLaw -> Clone("sft_bpl"));
  sft_pl -> SetLineColor(kRed);
  sft_pl -> DrawF1(3.0, 20.0, "CSAME");
  sft_bpl -> SetLineColor(kRed);
  sft_bpl -> DrawF1(3.0, 20.0, "CSAME");

  // output data
  FILE *out_file;
  // open the output file
  out_file = fopen("../../Figures/spect_sft.csv", "w");
  // print header
  fprintf(out_file, "Number of bins:%d\n", spectrum_sft -> GetNbinsX());
  fprintf(out_file, "Bin Center[km],Bin Width[km],Counts\n");
  for (int i = 1; i <= spectrum_sft -> GetNbinsX(); i++) {
    fprintf(out_file, "%g,%g,%g\n", spectrum_sft -> GetBinCenter(i), spectrum_sft -> GetBinWidth(i), spectrum_sft -> GetBinContent(i));
  }
  fclose(out_file);

  // open the output file
  out_file = fopen("../../Figures/spect_avg.csv", "w");
  // print header
  fprintf(out_file, "Number of bins:%d\n", spectrum_avg -> GetNbinsX());
  fprintf(out_file, "Bin Center[km],Bin Width[km],Counts\n");
  for (int i = 1; i <= spectrum_avg -> GetNbinsX(); i++) {
    fprintf(out_file, "%g,%g,%g\n", spectrum_avg -> GetBinCenter(i), spectrum_avg -> GetBinWidth(i), spectrum_avg -> GetBinContent(i));
  }
  fclose(out_file);

  // open the output file
  out_file = fopen("../../Figures/spect_hrd.csv", "w");
  // print header
  fprintf(out_file, "Number of bins:%d\n", spectrum_hrd -> GetNbinsX());
  fprintf(out_file, "Bin Center[km],Bin Width[km],Counts\n");
  for (int i = 1; i <= spectrum_hrd -> GetNbinsX(); i++) {
    fprintf(out_file, "%g,%g,%g\n", spectrum_hrd -> GetBinCenter(i), spectrum_hrd -> GetBinWidth(i), spectrum_hrd -> GetBinContent(i));
  }
  fclose(out_file);

  return 0;
}
