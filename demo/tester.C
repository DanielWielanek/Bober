/*
 * tester.C
 *
 *  Created on: 28 lis 2024
 *      Author: daniel
 */

/*
 * creator.C
 *
 *  Created on: 28 lis 2024
 *      Author: daniel
 */

#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TRandom.h>
#ifndef __CLING__
#include "HistoStyle.h"
#include "HistogramEditor.h"
#endif


void tester() {
  TH1D* h = new TH1D("a", "b", 100, 0, 1);
  gRandom->SetSeed(512);
  for (int i = 0; i < 100; i++) {
    h->Fill(gRandom->Gaus(0.2, 0.05));
  }
  TCanvas* c = new TCanvas();
  h->Draw();
  Bober::HistoStyle style;
  Bober::XMLFile file("t.xml");
  auto root = file.GetRootNode();
  style.ImportFromXML(root->GetChild("histogram"));
  style.Apply(*h);
  Bober::PadStyle padStyle;
  padStyle.ImportFromXML(root->GetChild("pad"));
  padStyle.Apply(gPad);

  h->Draw();
}
