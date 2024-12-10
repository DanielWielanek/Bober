/*
 * SuperEditor.cpp
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#include "SuperEditor.h"

#include <GuiTypes.h>
#include <RtypesCore.h>
#include <TGButton.h>
#include <TGFileDialog.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGLayout.h>
#include <TGTextEntry.h>
#include <iostream>

#include "AxisEditor.h"
#include "FillEditor.h"
#include "HistoStyle.h"
#include "LineEditor.h"
#include "MarkerEditor.h"
#include "PadStyle.h"
#include "XMLNode.h"

namespace Bober {

  SuperEditor::SuperEditor(TGCompositeFrame* frame, TVirtualPad* pad) : MainEditor(frame, pad) {
    TGCompositeFrame* superFrame = new TGCompositeFrame(fFrame, 80, 20, kVerticalFrame);

    TGLabel* label1 = new TGLabel(superFrame, "Marker Settings:");
    superFrame->AddFrame(label1, new TGLayoutHints(kLHintsCenterY | kLHintsTop));
    fMarkerEdit     = new MarkerEditor(superFrame, fPad);
    TGLabel* label2 = new TGLabel(superFrame, "Line Settings:");
    superFrame->AddFrame(label2, new TGLayoutHints(kLHintsCenterY | kLHintsTop));
    fLineEdit       = new LineEditor(superFrame, fPad);
    TGLabel* label3 = new TGLabel(superFrame, "Fill Settings:");
    superFrame->AddFrame(label3, new TGLayoutHints(kLHintsCenterY | kLHintsTop));
    fFillEdit = new FillEditor(superFrame, fPad);

    fFrame->AddFrame(superFrame, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, kLHintsTop));
    fInit           = kTRUE;
    TGLabel* label4 = new TGLabel(superFrame, "Path to save:");
    superFrame->AddFrame(label4, new TGLayoutHints(kLHintsCenterY | kLHintsTop));
    fSaveAs = new TGTextEntry(superFrame, new TGTextBuffer(50));
    fSaveAs->Resize(135, fSaveAs->GetDefaultHeight());
    fSaveAs->SetToolTipText("Set path to saved file");
    superFrame->AddFrame(fSaveAs, new TGLayoutHints(kLHintsLeft, kLHintsTop));

    TGTextButton* draw = new TGTextButton(superFrame, "&SaveAs");
    draw->Connect("Clicked()", "Bober::SuperEditor", this, "DoSave()");
    superFrame->AddFrame(draw, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
    fObject = nullptr;
    fXaxis  = nullptr;
    fYaxis  = nullptr;
  }
  void SuperEditor::SetAxesEditors(AxisEditor* x, AxisEditor* y) {
    fXaxis = x;
    fYaxis = y;
  }

  void SuperEditor::DoSave() {
    HistoStyle style;
    style.Import(*fObject);
    style.GetAxisStyle('x').SetNdivisions(style.GetAxisStyle('x').GetNDivisions(), fXaxis->ForcedNdiv());
    style.GetAxisStyle('y').SetNdivisions(style.GetAxisStyle('y').GetNDivisions(), fYaxis->ForcedNdiv());

    TString path = fSaveAs->GetText();
    XMLFile file(path, "recreate");
    file.CreateRootNode("data");
    auto root      = file.GetRootNode();
    auto histogram = new XMLNode("histogram");
    auto pad       = new XMLNode("pad");
    style.ExportToXML(histogram);
    PadStyle padStyle;
    padStyle.Import(*fPad);
    padStyle.ExportToXML(pad);

    root->AddChild(histogram);
    root->AddChild(pad);
  }

  void SuperEditor::SetModel(TObject* obj) {
    fObject = obj;
    fMarkerEdit->SetModel(obj);
    fLineEdit->SetModel(obj);
    fFillEdit->SetModel(obj);
    fAvoidSignal = kFALSE;
    TString name = fObject->GetName();
    name.ReplaceAll(" ", "");
    name = name + ".xml";
    fSaveAs->SetText(name, kFALSE);
  }

  SuperEditor::~SuperEditor() {}

} /* namespace Bober */
