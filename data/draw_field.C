void draw_field(){
  ifstream data("magfield.table");
  int xbin=251,ybin=201;
  double xmin=-2.,xmax=2.,ymin=-1.5,ymax=1.5;
  double pos[3],mag[3];
  TH2F *h[3];
  h[0] = new TH2F("h0","h0",xbin,xmin,xmax,ybin,ymin,ymax);
  h[1] = new TH2F("h1","h1",xbin,xmin,xmax,ybin,ymin,ymax);
  h[2] = new TH2F("h2","h2",xbin,xmin,xmax,ybin,ymin,ymax);
  for(int i=0;i<251;i++){
    for(int j=0;j<201;j++){
      data>>pos[0]>>pos[1]>>pos[2]>>mag[0]>>mag[1]>>mag[2];
      h[0]->SetBinContent(i+1,j+1,mag[0]);
      h[1]->SetBinContent(i+1,j+1,mag[1]);
      h[2]->SetBinContent(i+1,j+1,mag[2]);
    }
  }
  TCanvas *c1 = new TCanvas("c1","c1");
  c1->Divide(2,2);
  for(int i=0;i<3;i++){
    c1->cd(i+1);
    h[i]->GetXaxis()->SetTitle("X (m)");
    h[i]->GetYaxis()->SetTitle("Y (m)");
    h[i]->Draw("colz");
  }
}
