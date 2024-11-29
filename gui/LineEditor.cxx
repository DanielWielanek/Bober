/*
 * LineEditor.cxx
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#include "LineEditor.h"

#include <TAttLine.h>
#include <TCanvas.h>
#include <TClass.h>
#include <TColor.h>
#include <TGColorSelect.h>
#include <TGComboBox.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGLayout.h>
#include <TGNumberEntry.h>
#include <TGSlider.h>
#include <TGTextEntry.h>
#include <TGWidget.h>
#include <TGedFrame.h>
#include <TGraph.h>
#include <TMathBase.h>
#include <TROOT.h>

namespace Bober {
  LineEditor::LineEditor(TGCompositeFrame* frame, TVirtualPad* pad) : MainEditor(frame, pad), fAttLine(nullptr), fPriority(1) {
    enum ELineWid { kCOLOR, kLINE_WIDTH, kLINE_STYLE, kALPHA, kALPHAFIELD };

    TGCompositeFrame* f2 = new TGCompositeFrame(fFrame, 80, 20, kHorizontalFrame);
    fFrame->AddFrame(f2, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

    fColorSelect = new TGColorSelect(f2, 0, kCOLOR);
    f2->AddFrame(fColorSelect, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
    fColorSelect->Associate(fFrame);

    fStyleCombo = new TGLineStyleComboBox(fFrame, kLINE_STYLE);
    fStyleCombo->Resize(137, 20);
    fFrame->AddFrame(fStyleCombo, new TGLayoutHints(kLHintsLeft, 3, 1, 1, 1));
    fStyleCombo->Associate(fFrame);

    fWidthCombo = new TGLineWidthComboBox(f2, kLINE_WIDTH);
    fWidthCombo->Resize(90, 20);
    f2->AddFrame(fWidthCombo, new TGLayoutHints(kLHintsLeft, 3, 1, 1, 1));
    fWidthCombo->Associate(fFrame);

    TGLabel* AlphaLabel = new TGLabel(fFrame, "Opacity");
    fFrame->AddFrame(AlphaLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    TGHorizontalFrame* f2a = new TGHorizontalFrame(fFrame);
    fAlpha                 = new TGHSlider(f2a, 100, kSlider2 | kScaleNo, kALPHA);
    fAlpha->SetRange(0, 1000);
    f2a->AddFrame(fAlpha, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    fAlphaField = new TGNumberEntryField(f2a, kALPHAFIELD, 0, TGNumberFormat::kNESReal, TGNumberFormat::kNEANonNegative);
    fAlphaField->Resize(40, 20);
    if (!TCanvas::SupportAlpha()) {
      fAlpha->SetEnabled(kFALSE);
      AlphaLabel->Disable(kTRUE);
      fAlphaField->SetEnabled(kFALSE);
    }
    f2a->AddFrame(fAlphaField, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    fFrame->AddFrame(f2a, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    ConnectSignals2Slots();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Destructor of line editor.

  LineEditor::~LineEditor() {}

  ////////////////////////////////////////////////////////////////////////////////
  /// Connect signals to slots.

  void LineEditor::ConnectSignals2Slots() {
    fColorSelect->Connect("ColorSelected(Pixel_t)", "Bober::LineEditor", this, "DoLineColor(Pixel_t)");
    fColorSelect->Connect("AlphaColorSelected(ULong_t)", "Bober::LineEditor", this, "DoLineAlphaColor(ULong_t)");
    fStyleCombo->Connect("Selected(Int_t)", "Bober::LineEditor", this, "DoLineStyle(Int_t)");
    fWidthCombo->Connect("Selected(Int_t)", "Bober::LineEditor", this, "DoLineWidth(Int_t)");
    fAlpha->Connect("Released()", "Bober::LineEditor", this, "DoAlpha()");
    fAlpha->Connect("PositionChanged(Int_t)", "Bober::LineEditor", this, "DoLiveAlpha(Int_t)");
    fAlphaField->Connect("ReturnPressed()", "Bober::LineEditor", this, "DoAlphaField()");
    fAlpha->Connect("Pressed()", "Bober::LineEditor", this, "GetCurAlpha()");

    fInit = kFALSE;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Pick up the used line attributes.

  void LineEditor::SetModel(TObject* obj) {
    TAttLine* attline = dynamic_cast<TAttLine*>(obj);
    if (!attline) return;

    fAttLine     = attline;
    fAvoidSignal = kTRUE;

    fStyleCombo->Select(fAttLine->GetLineStyle());

    if (obj->InheritsFrom(TGraph::Class())) {
      fWidthCombo->Select(TMath::Abs(fAttLine->GetLineWidth() % 100));
    } else {
      fWidthCombo->Select(fAttLine->GetLineWidth());
    }

    Color_t c = fAttLine->GetLineColor();
    Pixel_t p = TColor::Number2Pixel(c);
    fColorSelect->SetColor(p);

    if (fInit) ConnectSignals2Slots();

    fAvoidSignal = kFALSE;

    if (TColor* color = gROOT->GetColor(fAttLine->GetLineColor())) {
      fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
      fAlphaField->SetNumber(color->GetAlpha());
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the line color.

  void LineEditor::DoLineColor(Pixel_t color) {
    if (fAvoidSignal) return;
    fAttLine->SetLineColor(TColor::GetColor(color));

    if (TColor* tcolor = gROOT->GetColor(TColor::GetColor(color))) {
      fAlpha->SetPosition((Int_t)(tcolor->GetAlpha() * 1000));
      fAlphaField->SetNumber(tcolor->GetAlpha());
    }

    Update();
  }


  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the color with alpha.

  void LineEditor::DoLineAlphaColor(ULongptr_t p) {
    TColor* color = (TColor*) p;

    if (fAvoidSignal) return;
    fAttLine->SetLineColor(color->GetNumber());
    fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
    fAlphaField->SetNumber(color->GetAlpha());

    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the line style.

  void LineEditor::DoLineStyle(Int_t style) {
    if (fAvoidSignal) return;
    fAttLine->SetLineStyle(style);
    Update();
  }


  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the line width.

  void LineEditor::DoLineWidth(Int_t width) {
    if (fAvoidSignal) return;
    if (dynamic_cast<TGraph*>(fAttLine)) {
      Int_t graphLineWidth = 100 * Int_t(fAttLine->GetLineWidth() / 100);
      if (graphLineWidth >= 0) {
        fAttLine->SetLineWidth(graphLineWidth + width);
      } else {
        fAttLine->SetLineWidth(-(TMath::Abs(graphLineWidth) + width));
      }
    } else if (fAttLine) {
      fAttLine->SetLineWidth(width);
    }
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set the alpha value from the entry field.

  void LineEditor::DoAlphaField() {
    if (fAvoidSignal) return;

    if (TColor* color = gROOT->GetColor(fAttLine->GetLineColor())) {
      color->SetAlpha((Float_t) fAlphaField->GetNumber());
      fAlpha->SetPosition((Int_t) fAlphaField->GetNumber() * 1000);
    }
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set the alpha value

  void LineEditor::DoAlpha() {
    if (fAvoidSignal) return;

    if (TColor* color = gROOT->GetColor(fAttLine->GetLineColor())) {
      color->SetAlpha((Float_t) fAlpha->GetPosition() / 1000);
      fAlphaField->SetNumber((Float_t) fAlpha->GetPosition() / 1000);
    }
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set alpha value online.

  void LineEditor::DoLiveAlpha(Int_t a) {
    if (fAvoidSignal) return;
    fAlphaField->SetNumber((Float_t) a / 1000);

    if (TColor* color = gROOT->GetColor(fAttLine->GetLineColor())) {
      // In case the color is not transparent a new color is created.
      if (color->GetAlpha() == 1.) {
        fAttLine->SetLineColor(TColor::GetColorTransparent(color->GetNumber(), 0.99));
      } else {
        color->SetAlpha((Float_t) a / 1000);
      }
    }
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to update alpha value on click on Slider

  void LineEditor::GetCurAlpha() {
    if (fAvoidSignal) return;

    if (TColor* color = gROOT->GetColor(fAttLine->GetLineColor())) {
      fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
      fAlphaField->SetNumber(color->GetAlpha());
    }
    Update();
  }
} /* namespace Bober */
