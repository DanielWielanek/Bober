/*
 * FillEditor.cpp
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#include "FillEditor.h"

#include <TAttFill.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TGColorSelect.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGLayout.h>
#include <TGNumberEntry.h>
#include <TGSlider.h>
#include <TGTextEntry.h>
#include <TGWidget.h>
#include <TGedPatternSelect.h>
#include <TROOT.h>

namespace Bober {
  enum EFillWid { kCOLOR, kPATTERN, kALPHA, kALPHAFIELD };


  ////////////////////////////////////////////////////////////////////////////////
  /// Constructor of fill attributes GUI.

  FillEditor::FillEditor(TGCompositeFrame* frame, TVirtualPad* pad) : MainEditor(frame, pad) {
    fPriority = 2;

    fAttFill = 0;


    TGCompositeFrame* f2 = new TGCompositeFrame(fFrame, 80, 20, kHorizontalFrame);
    fColorSelect         = new TGColorSelect(f2, 0, kCOLOR);
    f2->AddFrame(fColorSelect, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
    fColorSelect->Associate(fFrame);
    fPatternSelect = new TGedPatternSelect(f2, 1, kPATTERN);
    f2->AddFrame(fPatternSelect, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
    fPatternSelect->Associate(fFrame);
    fFrame->AddFrame(f2, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

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
    fAvoidSignal = kFALSE;
    fInit        = kTRUE;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Destructor of fill editor.

  FillEditor::~FillEditor() {}

  ////////////////////////////////////////////////////////////////////////////////
  /// Connect signals to slots.

  void FillEditor::ConnectSignals2Slots() {
    fColorSelect->Connect("ColorSelected(Pixel_t)", "Bober::FillEditor", this, "DoFillColor(Pixel_t)");
    fColorSelect->Connect("AlphaColorSelected(ULong_t)", "Bober::FillEditor", this, "DoFillAlphaColor(ULong_t)");
    fPatternSelect->Connect("PatternSelected(Style_t)", "Bober::FillEditor", this, "DoFillPattern(Style_t)");
    fAlpha->Connect("Released()", "Bober::FillEditor", this, "DoAlpha()");
    fAlpha->Connect("PositionChanged(Int_t)", "Bober::FillEditor", this, "DoLiveAlpha(Int_t)");
    fAlphaField->Connect("ReturnPressed()", "Bober::FillEditor", this, "DoAlphaField()");
    fAlpha->Connect("Pressed()", "Bober::FillEditor", this, "GetCurAlpha()");
    fInit = kFALSE;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Pick up the used fill attributes.

  void FillEditor::SetModel(TObject* obj) {
    TAttFill* attfill = dynamic_cast<TAttFill*>(obj);
    if (!attfill) return;

    fAttFill     = attfill;
    fAvoidSignal = kTRUE;

    Color_t c = fAttFill->GetFillColor();
    Pixel_t p = TColor::Number2Pixel(c);
    fColorSelect->SetColor(p, kFALSE);

    Style_t s = fAttFill->GetFillStyle();
    fPatternSelect->SetPattern(s, kFALSE);

    if (fInit) ConnectSignals2Slots();
    fAvoidSignal = kFALSE;

    if (TColor* color = gROOT->GetColor(fAttFill->GetFillColor())) {
      fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
      fAlphaField->SetNumber(color->GetAlpha());
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the fill area color with alpha.

  void FillEditor::DoFillAlphaColor(ULongptr_t p) {
    TColor* color = (TColor*) p;

    if (fAvoidSignal) return;
    fAttFill->SetFillColor(color->GetNumber());
    fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
    fAlphaField->SetNumber(color->GetAlpha());

    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the fill area color.

  void FillEditor::DoFillColor(Pixel_t color) {
    if (fAvoidSignal) return;
    fAttFill->SetFillColor(TColor::GetColor(color));

    if (TColor* tcolor = gROOT->GetColor(TColor::GetColor(color))) {
      fAlpha->SetPosition((Int_t)(tcolor->GetAlpha() * 1000));
      fAlphaField->SetNumber(tcolor->GetAlpha());
    }

    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the fill area pattern.

  void FillEditor::DoFillPattern(Style_t pattern) {
    if (fAvoidSignal) return;
    fAttFill->SetFillStyle(pattern);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set the alpha value from the entry field.

  void FillEditor::DoAlphaField() {
    if (fAvoidSignal) return;

    if (TColor* color = gROOT->GetColor(fAttFill->GetFillColor())) {
      color->SetAlpha((Float_t) fAlphaField->GetNumber());
      fAlpha->SetPosition((Int_t) fAlphaField->GetNumber() * 1000);
    }
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set the alpha value

  void FillEditor::DoAlpha() {
    if (fAvoidSignal) return;

    if (TColor* color = gROOT->GetColor(fAttFill->GetFillColor())) {
      color->SetAlpha((Float_t) fAlpha->GetPosition() / 1000);
      fAlphaField->SetNumber((Float_t) fAlpha->GetPosition() / 1000);
    }
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set alpha value online.

  void FillEditor::DoLiveAlpha(Int_t a) {
    if (fAvoidSignal) return;
    fAlphaField->SetNumber((Float_t) a / 1000);

    if (TColor* color = gROOT->GetColor(fAttFill->GetFillColor())) {
      // In case the color is not transparent a new color is created.
      if (color->GetAlpha() == 1.) {
        fAttFill->SetFillColor(TColor::GetColorTransparent(color->GetNumber(), 0.99));
      } else {
        color->SetAlpha((Float_t) a / 1000);
      }
    }
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to update alpha value on click on Slider

  void FillEditor::GetCurAlpha() {
    if (fAvoidSignal) return;

    if (TColor* color = gROOT->GetColor(fAttFill->GetFillColor())) {
      fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
      fAlphaField->SetNumber(color->GetAlpha());
    }
    Update();
  }
} /* namespace Bober */
