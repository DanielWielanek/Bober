/*
 * create.cpp
 *
 *  Created on: 29 lis 2024
 *      Author: daniel
 */
#include <TApplication.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TRandom.h>
#ifndef __CLING__
#include "HistogramEditor.h"
#endif


int main(int argc, char* argv[]) {
  TH1D* h = new TH1D("a", "a", 100, 0, 1);
  gRandom->SetSeed(512);
  for (int i = 0; i < 100; i++) {
    h->Fill(gRandom->Gaus(0.2, 0.05));
  }

  TApplication* rootapp = new TApplication("My Application", &argc, argv);

  TCanvas* c = new TCanvas();
  h->Draw();
  new Bober::HistogramEditor(h, gPad);
  rootapp->Run();
  return 0;
}
