int Spatial()
{
  gROOT -> Reset();

 // adress for input file
  TFile *inFile;

 // adress for root tree stored in file
  TTree *tree;

  TH2D *spatial_dist;
  TH2D *angular_dist;

  // spectra
  spatial_dist = new TH2D();

  spatial_dist -> SetName("Spatial_Dist");
  spatial_dist -> SetTitle("Spatial Distribution");

  int nbins_r = 100;
  double r_min = 0.0;
  double r_max = 500.0;

  int nbins_e = 7;
  double e_min = 3.0;
  double e_max = 70.0;

  spatial_dist -> SetBins(nbins_r, r_min, r_max, nbins_e, e_min, e_max);

  // assign titles to the histogram axes
  spatial_dist -> GetXaxis() -> SetTitle("Distance from beam axis [km]");
  spatial_dist -> GetYaxis() -> SetTitle("Energy [keV]");


  angular_dist = new TH2D();

  angular_dist -> SetName("Angular_Dist");
  angular_dist -> SetTitle("Angular Distribution");

  int nbins_theta = 90;
  double theta_min = 0.0;
  double theta_max = 90.0;

  int nbins_E = 7;
  double E_min = 3.0;
  double E_max = 70.0;

  angular_dist -> SetBins(nbins_theta, theta_min, theta_max, nbins_E, E_min, E_max);

  // assign titles to the histogram axes
  angular_dist -> GetXaxis() -> SetTitle("Angle from beam axis [degrees]");
  angular_dist -> GetYaxis() -> SetTitle("Energy [keV]");

  cout << "********* opening Spectra *********" << endl; // let me know that we are OPENING this bish

  // open data files
  inFile = new TFile("../output/final_simulation.root");

  // extract the trees
  tree = (TTree*) inFile -> Get("JupiterData");

  // set variables to hold data
  double energy, x, y, z, vertex_x, vertex_y, vertex_z;

  // setup the variable adresses for branches
  tree -> SetBranchAddress("energy", &energy);
  tree -> SetBranchAddress("x", &x);
  tree -> SetBranchAddress("y", &y);
  tree -> SetBranchAddress("z", &z);
  tree -> SetBranchAddress("vertex_x", &vertex_x);
  tree -> SetBranchAddress("vertex_y", &vertex_y);
  tree -> SetBranchAddress("vertex_z", &vertex_z);

  int length;
  double r;
  double theta;
  double pos_mag;
  double vert_mag;
  // loop through trees and extract data
  length = tree -> GetEntries(); // get total number of entries

  for(int i = 0; i < length; i++){
    tree -> GetEntry(i);

    // calculate radial position
    r = TMath::Sqrt(x*x + y*y);
    spatial_dist -> Fill(r, energy);

    // calculate angular position
    pos_mag = TMath::Sqrt(x*x + y*y + z*z);
    vert_mag = TMath::Sqrt(vertex_x*vertex_x + vertex_y*vertex_y + vertex_z*vertex_z);
    theta = TMath::RadToDeg()*TMath::ACos((x*vertex_x + y*vertex_y + z*vertex_z)/(pos_mag*vert_mag));
    angular_dist -> Fill(theta, energy);

  }

  TH1D *dist;
  char name[1];

  // create plots
  TCanvas *c1 = new TCanvas("c1", "c1", 2400, 1800);

  for(int i = 0; i < 7; i++){
    sprintf(name, "%i", i);
    dist =  spatial_dist -> ProjectionX(name, i, i);
    dist -> Scale(1/dist -> Integral());
    dist -> SetStats(0);
    dist -> SetLineColor(i);
    dist -> SetLineWidth(2);
    dist -> Draw("SAME HIST");
  }

  TCanvas *c2 = new TCanvas("c2", "c2", 2400, 1800);

  for(int i = 0; i < 7; i++){
    sprintf(name, "%i", i + 7);
    dist =  angular_dist -> ProjectionX(name, i, i);
    dist -> Scale(1/dist -> Integral());
    dist -> SetStats(0);
    dist -> SetLineColor(i);
    dist -> Draw("SAME HIST");
  }
  return 0;
}
