/*
 * MarkerEditor.cpp
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#include "MarkerEditor.h"


#include "TAttMarker.h"
#include "TAttMarkerEditor.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TGColorSelect.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGSlider.h"
#include "TGedMarkerSelect.h"
#include "TROOT.h"

namespace Bober {
  enum EMarkerWid { kCOLOR, kMARKER, kMARKER_SIZE, kALPHA, kALPHAFIELD };

  ////////////////////////////////////////////////////////////////////////////////
  /// Constructor of marker attributes GUI.

  MarkerEditor::MarkerEditor(TGCompositeFrame* frame, TVirtualPad* pad) : MainEditor(frame, pad) {
    fAttMarker   = 0;
    fSizeForText = kFALSE;

    TGCompositeFrame* f2 = new TGCompositeFrame(fFrame, 80, 20, kHorizontalFrame);
    fColorSelect         = new TGColorSelect(f2, 0, kCOLOR);
    f2->AddFrame(fColorSelect, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
    fColorSelect->Associate(fFrame);

    fMarkerType = new TGedMarkerSelect(f2, 1, kMARKER);
    f2->AddFrame(fMarkerType, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
    fMarkerType->Associate(fFrame);

    fMarkerSize = new TGNumberEntry(f2,
                                    0.,
                                    4,
                                    kMARKER_SIZE,
                                    TGNumberFormat::kNESRealOne,
                                    TGNumberFormat::kNEANonNegative,
                                    TGNumberFormat::kNELLimitMinMax,
                                    0.2,
                                    5.0);
    fMarkerSize->GetNumberEntry()->SetToolTipText("Set marker size");
    f2->AddFrame(fMarkerSize, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
    fMarkerSize->Associate(fFrame);
    fFrame->AddFrame(f2, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));
#ifdef ALFA__
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
#endif
    ConnectSignals2Slots();
    fInit = kTRUE;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Destructor of marker editor.

  MarkerEditor::~MarkerEditor() {}

  ////////////////////////////////////////////////////////////////////////////////
  /// Connect signals to slots.

  void MarkerEditor::ConnectSignals2Slots() {
    fColorSelect->Connect("ColorSelected(Pixel_t)", "Bober::MarkerEditor", this, "DoMarkerColor(Pixel_t)");
    fColorSelect->Connect("AlphaColorSelected(ULong_t)", "Bober::MarkerEditor", this, "DoMarkerAlphaColor(ULong_t)");
    fMarkerType->Connect("MarkerSelected(Style_t)", "Bober::MarkerEditor", this, "DoMarkerStyle(Style_t)");
    fMarkerSize->Connect("ValueSet(Long_t)", "Bober::MarkerEditor", this, "DoMarkerSize()");
    (fMarkerSize->GetNumberEntry())->Connect("ReturnPressed()", "Bober::MarkerEditor", this, "DoMarkerSize()");
#ifdef ALFA__
    fAlpha->Connect("Released()", "Bober::MarkerEditor", this, "DoAlpha()");
    fAlpha->Connect("PositionChanged(Int_t)", "Bober::MarkerEditor", this, "DoLiveAlpha(Int_t)");
    fAlphaField->Connect("ReturnPressed()", "Bober::MarkerEditor", this, "DoAlphaField()");
    fAlpha->Connect("Pressed()", "Bober::MarkerEditor", this, "GetCurAlpha()");
#endif
    fInit = kFALSE;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Pick up the values of used marker attributes.

  void MarkerEditor::SetModel(TObject* obj) {
    fAvoidSignal = kTRUE;

    fAttMarker = dynamic_cast<TAttMarker*>(obj);
    if (!fAttMarker) return;

    TString str = GetDrawOption();
    str.ToUpper();
    if (obj->InheritsFrom("TH2") && str.Contains("TEXT")) {
      fSizeForText = kTRUE;
    } else {
      fSizeForText = kFALSE;
    }
    Style_t marker = fAttMarker->GetMarkerStyle();
    if ((marker == 1 || marker == 6 || marker == 7) && !fSizeForText) {
      fMarkerSize->SetNumber(1.);
      fMarkerSize->SetState(kFALSE);
    } else {
      Float_t s = fAttMarker->GetMarkerSize();
      fMarkerSize->SetState(kTRUE);
      fMarkerSize->SetNumber(s);
    }
    fMarkerType->SetMarkerStyle(marker);

    Color_t c = fAttMarker->GetMarkerColor();
    Pixel_t p = TColor::Number2Pixel(c);
    fColorSelect->SetColor(p);

    if (fInit) ConnectSignals2Slots();
    fAvoidSignal = kFALSE;
#ifdef ALFA__
    if (TColor* color = gROOT->GetColor(fAttMarker->GetMarkerColor())) {
      fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
      fAlphaField->SetNumber(color->GetAlpha());
    }
#endif
  }


  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the marker color.

  void MarkerEditor::DoMarkerColor(Pixel_t color) {
    if (fAvoidSignal) return;
    fAttMarker->SetMarkerColor(TColor::GetColor(color));
#ifdef ALFA__
    if (TColor* tcolor = gROOT->GetColor(TColor::GetColor(color))) {
      fAlpha->SetPosition((Int_t)(tcolor->GetAlpha() * 1000));
      fAlphaField->SetNumber(tcolor->GetAlpha());
    }
#endif
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the color with alpha.

  void MarkerEditor::DoMarkerAlphaColor(ULongptr_t p) {
    TColor* color = (TColor*) p;

    if (fAvoidSignal) return;
    fAttMarker->SetMarkerColor(color->GetNumber());
#ifdef ALFA__
    fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
    fAlphaField->SetNumber(color->GetAlpha());
#endif
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the marker type.

  void MarkerEditor::DoMarkerStyle(Style_t marker) {
    if (fAvoidSignal) return;
    if ((marker == 1 || marker == 6 || marker == 7) && !fSizeForText) {
      fMarkerSize->SetNumber(1.);
      fMarkerSize->SetState(kFALSE);
    } else
      fMarkerSize->SetState(kTRUE);

    fAttMarker->SetMarkerStyle(marker);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the marker size.

  void MarkerEditor::DoMarkerSize() {
    if (fAvoidSignal) return;
    Style_t marker = fAttMarker->GetMarkerStyle();
    if ((marker == 1 || marker == 6 || marker == 7) && !fSizeForText) {
      fMarkerSize->SetNumber(1.);
      fMarkerSize->SetState(kFALSE);
    } else
      fMarkerSize->SetState(kTRUE);
    Float_t size = fMarkerSize->GetNumber();
    fAttMarker->SetMarkerSize(size);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set the alpha value from the entry field.

  void MarkerEditor::DoAlphaField() {
    if (fAvoidSignal) return;
#ifdef ALFA__
    if (TColor* color = gROOT->GetColor(fAttMarker->GetMarkerColor())) {
      color->SetAlpha((Float_t) fAlphaField->GetNumber());
      fAlpha->SetPosition((Int_t) fAlphaField->GetNumber() * 1000);
    }
#endif
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set the alpha value

  void MarkerEditor::DoAlpha() {
    if (fAvoidSignal) return;
#ifdef ALFA__
    if (TColor* color = gROOT->GetColor(fAttMarker->GetMarkerColor())) {
      color->SetAlpha((Float_t) fAlpha->GetPosition() / 1000);
      fAlphaField->SetNumber((Float_t) fAlpha->GetPosition() / 1000);
    }
#endif
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to set alpha value online.

  void MarkerEditor::DoLiveAlpha(Int_t a) {
    if (fAvoidSignal) return;
#ifdef ALFA__
    fAlphaField->SetNumber((Float_t) a / 1000);
#endif
    if (TColor* color = gROOT->GetColor(fAttMarker->GetMarkerColor())) {
      // In case the color is not transparent a new color is created.
      if (color->GetAlpha() == 1.) {
        fAttMarker->SetMarkerColor(TColor::GetColorTransparent(color->GetNumber(), 0.99));
      } else {
        color->SetAlpha((Float_t) a / 1000);
      }
    }
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot to update alpha value on click on Slider

  void MarkerEditor::GetCurAlpha() {
    if (fAvoidSignal) return;
#ifdef ALFA__
    if (TColor* color = gROOT->GetColor(fAttMarker->GetMarkerColor())) {
      fAlpha->SetPosition((Int_t)(color->GetAlpha() * 1000));
      fAlphaField->SetNumber(color->GetAlpha());
    }
#endif
    Update();
  }
} /* namespace Bober */
