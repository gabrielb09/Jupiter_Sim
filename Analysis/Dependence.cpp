void BinLogX(TH2*h, int ax){

    TAxis *axis;
    if (ax == 0){
    axis = h -> GetXaxis();
    }
    else{
    axis = h -> GetYaxis();
    }
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

void BinLogX(TH1*h){

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

void BinLog2(TH2*h){
    BinLogX(h, 0);
    BinLogX(h, 1);
}

int Dependence(){
    TFile *inFile = new TFile("../output/final_simulation_10MeV.root", "READ");
    TTree *e_tree = (TTree*) inFile -> Get("Primary_Electrons");
    TTree *x_tree = (TTree*) inFile -> Get("X_Rays");
    TH2D* spectrum = new TH2D();
    TH1D* num_E = new TH1D();

    spectrum -> SetName("Spectrum");
    spectrum -> SetTitle("X-Ray Spectral Dependence");
    int nx, ny;
    double xmin, xmax, ymin, ymax;

    int length = e_tree -> GetEntries();
    xmin = e_tree -> GetMinimum("energy");
    xmax = e_tree -> GetMaximum("energy");
    nx = (int)((xmax-xmin)/250.0);

    cout << "Electron energy range: " << xmin << " keV - " << xmax << " keV" << endl;

    spectrum -> SetBins(nx, xmin, xmax, ny = 80, ymin = 1.0, ymax = 30.0);
    BinLog2(spectrum);

    spectrum -> GetXaxis() -> SetTitle("Electron Energy [keV]");
	spectrum -> GetYaxis() -> SetTitle("Photon Energy [keV]");
    spectrum -> GetZaxis() -> SetTitle("#frac{dN}{dE} [photons kev^{-1} electron^{-1}]");

    num_E -> SetName("NumPrimaries");
    num_E -> SetTitle("Electron Spectrum");
    num_E -> SetBins(nx, xmin, xmax);
    BinLogX(num_E);

    int eventID;
    double e_energy, energy;

    x_tree -> SetBranchAddress("eventID", &eventID);
    x_tree -> SetBranchAddress("energy", &energy);
    e_tree -> SetBranchAddress("energy", &e_energy);

    x_tree -> Print();
    length = x_tree -> GetEntries();

	for(int i = 0; i < length; i++){
        x_tree -> GetEntry(i);
        e_tree -> GetEntry(eventID);
        spectrum -> Fill(e_energy, energy);
	}

    length = e_tree -> GetEntries();

    for(int i = 0; i < length; i++){
        e_tree -> GetEntry(i);
        num_E -> Fill(e_energy);
    }

    double bin_width, bin_height;
    for(int i = 0; i <= spectrum -> GetNbinsX(); i++){
        for(int j = 0; j <= spectrum -> GetNbinsY(); j++){

            bin_width = spectrum -> GetYaxis() -> GetBinWidth(j);
            bin_height = spectrum -> GetBinContent(i,j);
            spectrum -> SetBinContent(spectrum -> GetBin(i, j), bin_height/(bin_width*(num_E -> GetBinContent(i))));
        }
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 1000, 750);
    c1 -> SetLogx(); c1 -> SetLogy();

    spectrum -> SetStats(0);
    c1 -> SetLogz();

    spectrum -> Draw("SAME COLZ");

    return 0;
}
