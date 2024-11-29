/*
 * HistogramEditor.cxx
 *
 *  Created on: 20 lis 2024
 *      Author: daniel
 */

#include "HistogramEditor.h"

#include <TAxis.h>
#include <TGClient.h>
#include <TGLayout.h>
#include <TGTab.h>
#include <TH1.h>

#include "AxisEditor.h"
#include "PadEditor.h"
#include "SuperEditor.h"

namespace Bober {
  HistogramEditor::HistogramEditor(const TGWindow* p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h) {}
  HistogramEditor::HistogramEditor(TH1* histo, TVirtualPad* pad) : HistogramEditor(gClient->GetRoot(), 800, 600) {
    TGTab* tabs = new TGTab(this, 200, 500);
    fHisto      = histo;

    TGCompositeFrame* tab1 = tabs->AddTab("Pad");
    PadEditor* editor1     = new PadEditor(tab1, pad);
    editor1->SetModel(pad);
    TGCompositeFrame* tab2 = tabs->AddTab("Xaxis");
    AxisEditor* editor2    = new AxisEditor(tab2, pad);
    editor2->SetModel(fHisto->GetXaxis());
    TGCompositeFrame* tab3 = tabs->AddTab("Yaxis");
    AxisEditor* editor3    = new AxisEditor(tab3, pad);
    editor3->SetModel(fHisto->GetYaxis());

    TGCompositeFrame* tab4 = tabs->AddTab("Super");
    SuperEditor* editor4   = new SuperEditor(tab4, pad);
    editor4->SetAxesEditors(editor2, editor3);
    editor4->SetModel(histo);


    //  TGVerticalFrame* tab1Frame = new TGVerticalFrame(tab1);


    //  TGCompositeFrame* histogramTab = tabs->AddTab("Histogram");
    // fCanvas                        = new TRootEmbeddedCanvas("Canvas", histogramTab, 800, 600);
    //  histogramTab->AddFrame(fCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));


    AddFrame(tabs, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    // Wyświetlenie GUI
    SetWindowName("Histogram Editor");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
  }
  HistogramEditor::~HistogramEditor() {}
}  // namespace Bober
