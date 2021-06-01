{
  TFile *inFile;
  TH1F *spect;
  char inputFile[100]; // input file memory location

  TCanvas *c1 = new TCanvas("c1", "c1", 2400, 1800);
  c1 -> SetLogx(); c1 -> SetLogy();

  sprintf(inputFile, "%s%s%s", "../output/plots/", "final_simulation_soft_full_spectrum", ".root");
  inFile = new TFile(inputFile);
  spect = (TH1F*) inFile -> Get("Full_Spect");
  spect -> SetStats(0);
  spect -> SetLineColor(kBlue);
  spect -> Draw("SAME HIST");

  TF1 *pwrLaw1;
    pwrLaw1 = new TF1("pwrLaw1", "[0]*pow(x/[1], -1*[2])", 1.0, 10.0);
    pwrLaw1 -> SetParName(0, "Amplitude");
    pwrLaw1 -> SetParLimits(0, 1e2, 1e4);
    pwrLaw1 -> SetParName(1, "E_0");
    pwrLaw1 -> SetParLimits(1, 1.0, 10.0);
    pwrLaw1 -> SetParName(2, "#alpha");
    pwrLaw1 -> SetParLimits(2, 0, 5);

  cout << "alpha_e = 0.8" << endl;
  spect -> Fit("pwrLaw1", "0", "", 1.0, 10.0);
  pwrLaw1 -> SetLineColor(kBlue);
  pwrLaw1 -> DrawF1(1.0, 10.0, "CSAME");

  sprintf(inputFile, "%s%s%s", "../output/plots/", "final_simulation_hard_full_spectrum", ".root");
  inFile = new TFile(inputFile);
  spect = (TH1F*) inFile -> Get("Full_Spect");
  spect -> SetStats(0);
  spect -> SetLineColor(kRed);
  spect -> Draw("SAME HIST");

  TF1 *pwrLaw2;
    pwrLaw2 = new TF1("pwrLaw2", "[0]*pow(x/[1], -1*[2])", 1.0, 10.0);
    pwrLaw2 -> SetParName(0, "Amplitude");
    pwrLaw2 -> SetParLimits(0, 1e2, 1e4);
    pwrLaw2 -> SetParName(1, "E_0");
    pwrLaw2 -> SetParLimits(1, 1.0, 10.0);
    pwrLaw2 -> SetParName(2, "#alpha");
    pwrLaw2 -> SetParLimits(2, 0, 5);

  cout << "alpha_e = 1.7" << endl;
  spect -> Fit("pwrLaw2", "0", "", 1.0, 10.0);
  pwrLaw2 -> SetLineColor(kRed);
  pwrLaw2 -> DrawF1(1.0, 10.0, "CSAME");

}
