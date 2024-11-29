/*
 * PadEditor.cxx
 *
 *  Created on: 20 lis 2024
 *      Author: daniel
 */

#include "PadEditor.h"

#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGComboBox.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGLayout.h>
#include <TGNumberEntry.h>
#include <TGWidget.h>
#include <TGedEditor.h>
#include <TPad.h>

#include <iostream>

namespace Bober {
  enum EPadWid {
    kCOLOR,
    kPAD_FAR,
    kPAD_EDIT,
    kPAD_CROSS,
    kPAD_GRIDX,
    kPAD_GRIDY,
    kPAD_LOGX,
    kPAD_LOGY,
    kPAD_LOGZ,
    kPAD_TICKX,
    kPAD_TICKY,
    kPAD_BSIZE,
    kPAD_BMODE
  };


  ////////////////////////////////////////////////////////////////////////////////
  /// Constructor of TPad editor GUI.

  PadEditor::PadEditor(TGCompositeFrame* frame, TVirtualPad* pad) : MainEditor(frame, pad) {

    // fFixedAR = new TGCheckButton(frame, "Fixed aspect ratio", kPAD_FAR);
    /// fFixedAR->SetToolTipText("Set fixed aspect ratio");
    // fFrame->AddFrame(fFixedAR, new TGLayoutHints(kLHintsTop, 4, 1, 2, 1));

    TGCompositeFrame* f2 = new TGCompositeFrame(fFrame, 80, 20, kHorizontalFrame);
    TGCompositeFrame* f3 = new TGCompositeFrame(f2, 40, 20, kVerticalFrame);
    // fCrosshair           = new TGCheckButton(f3, "Crosshair", kPAD_CROSS);
    // fCrosshair->SetToolTipText("Set crosshair");
    // f3->AddFrame(fCrosshair, new TGLayoutHints(kLHintsTop, 3, 1, 1, 1));
    fGridX = new TGCheckButton(f3, "GridX", kPAD_GRIDX);
    fGridX->SetToolTipText("Set grid along X");
    f3->AddFrame(fGridX, new TGLayoutHints(kLHintsTop, 3, 1, 1, 1));
    fTickX = new TGCheckButton(f3, "TickX", kPAD_TICKX);
    fTickX->SetToolTipText("Set tick marks along X");
    f3->AddFrame(fTickX, new TGLayoutHints(kLHintsTop, 3, 1, 1, 1));
    f2->AddFrame(f3, new TGLayoutHints(kLHintsTop, 0, 1, 0, 0));

    TGCompositeFrame* f4 = new TGCompositeFrame(f2, 40, 20, kVerticalFrame);
    // fEditable            = new TGCheckButton(f4, "Edit", kPAD_EDIT);
    // fEditable->SetToolTipText("Set editable mode");
    // f4->AddFrame(fEditable, new TGLayoutHints(kLHintsTop, 3, 1, 1, 1));
    fGridY = new TGCheckButton(f4, "GridY", kPAD_GRIDY);
    fGridY->SetToolTipText("Set grid along Y");
    f4->AddFrame(fGridY, new TGLayoutHints(kLHintsTop, 3, 1, 1, 1));
    fTickY = new TGCheckButton(f4, "TickY", kPAD_TICKY);
    fTickY->SetToolTipText("Set tick marks along Y");
    f4->AddFrame(fTickY, new TGLayoutHints(kLHintsTop, 3, 1, 1, 1));
    f2->AddFrame(f4, new TGLayoutHints(kLHintsTop, 0, 1, 0, 0));

    fFrame->AddFrame(f2, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

    // fFrame->MakeTitle("Log Scale");

    TGCompositeFrame* f5 = new TGCompositeFrame(fFrame, 80, 20, kVerticalFrame);
    TGLabel* logLabel    = new TGLabel(f5, "Log");
    f5->AddFrame(logLabel, new TGLayoutHints(kLHintsTop, 4, 1, 1, 1));
    TGCompositeFrame* f5b = new TGCompositeFrame(f5, 80, 20, kHorizontalFrame);

    fLogX = new TGCheckButton(f5b, ":X", kPAD_LOGX);
    fLogX->SetToolTipText("Set logarithmic scale along X");
    f5b->AddFrame(fLogX, new TGLayoutHints(kLHintsTop, 4, 1, 1, 1));
    fLogY = new TGCheckButton(f5b, ":Y", kPAD_LOGY);
    fLogY->SetToolTipText("Set logarithmic scale along Y");
    f5b->AddFrame(fLogY, new TGLayoutHints(kLHintsTop, 15, 1, 1, 1));
    fLogZ = new TGCheckButton(f5b, ":Z", kPAD_LOGZ);
    fLogZ->SetToolTipText("Set logarithmic scale along Z");
    f5b->AddFrame(fLogZ, new TGLayoutHints(kLHintsTop, 15, 1, 1, 1));
    f5->AddFrame(f5b, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));
    fFrame->AddFrame(f5, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

    TGCompositeFrame* f6 = new TGCompositeFrame(fFrame, 80, 20, kHorizontalFrame);
    fBgroup              = new TGButtonGroup(f6, 3, 1, 3, 0, "Border Mode");
    // fBgroup->SetRadioButtonExclusive(kTRUE);
    fBmode = new TGRadioButton(fBgroup, " Sunken border", 77);
    fBmode->SetToolTipText("Set a sinken border of the pad/canvas");
    fBmode0 = new TGRadioButton(fBgroup, " No border", 78);
    fBmode0->SetToolTipText("Set no border of the pad/canvas");
    fBmode1 = new TGRadioButton(fBgroup, " Raised border", 79);
    fBmode1->SetToolTipText("Set a raised border of the pad/canvas");
    fBmodelh = new TGLayoutHints(kLHintsLeft, 0, 0, 3, 0);
    // fBgroup->SetLayoutHints(fBmodelh, fBmode);
    // fBgroup->ChangeOptions(kFitWidth | kChildFrame | kVerticalFrame);
    // f6->AddFrame(fBgroup, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 4, 1, 0, 0));
    fFrame->AddFrame(f6, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

    TGCompositeFrame* f7 = new TGCompositeFrame(fFrame, 80, 20, kHorizontalFrame);
    TGLabel* fSizeLbl    = new TGLabel(f7, "Size:");
    f7->AddFrame(fSizeLbl, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));
    fBsize = new TGLineWidthComboBox(f7, kPAD_BSIZE);
    fBsize->Resize(92, 20);
    f7->AddFrame(fBsize, new TGLayoutHints(kLHintsLeft, 13, 1, 0, 0));
    fBsize->Associate(fFrame);
    fFrame->AddFrame(f7, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));

    TGCompositeFrame* f8 = new TGCompositeFrame(fFrame, 80, 20, kVerticalFrame);
    TGLabel* marginLabel = new TGLabel(f8, "Bottom Margin:");
    f8->AddFrame(marginLabel, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));
    fBottomMargin = new TGNumberEntry(f8,
                                      0,
                                      5,
                                      -1,
                                      TGNumberFormat::EStyle::kNESRealTwo,
                                      TGNumberFormat::EAttribute::kNEANonNegative,
                                      TGNumberFormat::kNELLimitMinMax,
                                      0,
                                      1);
    f8->AddFrame(fBottomMargin, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));

    marginLabel = new TGLabel(f8, "Top Margin:");
    f8->AddFrame(marginLabel, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));
    fTopMargin = new TGNumberEntry(f8,
                                   0,
                                   5,
                                   -1,
                                   TGNumberFormat::EStyle::kNESRealTwo,
                                   TGNumberFormat::EAttribute::kNEANonNegative,
                                   TGNumberFormat::kNELLimitMinMax,
                                   0,
                                   1);
    f8->AddFrame(fTopMargin, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));

    marginLabel = new TGLabel(f8, "Left Margin:");
    f8->AddFrame(marginLabel, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));
    fLeftMargin = new TGNumberEntry(f8,
                                    0,
                                    5,
                                    -1,
                                    TGNumberFormat::EStyle::kNESRealTwo,
                                    TGNumberFormat::EAttribute::kNEANonNegative,
                                    TGNumberFormat::kNELLimitMinMax,
                                    0,
                                    1);
    f8->AddFrame(fLeftMargin, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));

    marginLabel = new TGLabel(f8, "Right Margin:");
    f8->AddFrame(marginLabel, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));
    fRightMargin = new TGNumberEntry(f8,
                                     0,
                                     5,
                                     -1,
                                     TGNumberFormat::EStyle::kNESRealTwo,
                                     TGNumberFormat::EAttribute::kNEANonNegative,
                                     TGNumberFormat::kNELLimitMinMax,
                                     0,
                                     1);
    f8->AddFrame(fRightMargin, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 6, 1, 0, 0));
    fFrame->AddFrame(f8, new TGLayoutHints(kLHintsTop, 1, 1, 0, 0));
    ConnectSignals2Slots();
    fInit        = kTRUE;
    fAvoidSignal = kFALSE;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Destructor of fill editor.

  PadEditor::~PadEditor() {
    // children of TGButonGroup are not deleted
    delete fBmode;
    delete fBmode0;
    delete fBmode1;
    delete fBmodelh;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Connect signals to slots.

  void PadEditor::ConnectSignals2Slots() {
    // fFixedAR->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoFixedAspectRatio(Bool_t)");
    // fCrosshair->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoCrosshair(Bool_t)");
    // fEditable->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoEditable(Bool_t)");
    fGridX->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoGridX(Bool_t)");
    fGridY->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoGridY(Bool_t)");
    fTickX->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoTickX(Bool_t)");
    fTickY->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoTickY(Bool_t)");
    fLogX->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoLogX(Bool_t)");
    fLogY->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoLogY(Bool_t)");
    fLogZ->Connect("Toggled(Bool_t)", "Bober::PadEditor", this, "DoLogZ(Bool_t)");
    //  fBgroup->Connect("Clicked(Int_t)", "PadEditor", this, "DoBorderMode()");
    fBsize->Connect("Selected(Int_t)", "Bober::PadEditor", this, "DoBorderSize(Int_t)");
    fBottomMargin->Connect("ValueSet(Long_t)", "Bober::PadEditor", this, "ScaleMargin(Bool_t)");
    fTopMargin->Connect("ValueSet(Long_t)", "Bober::PadEditor", this, "ScaleMargin(Bool_t)");
    fLeftMargin->Connect("ValueSet(Long_t)", "Bober::PadEditor", this, "ScaleMargin(Bool_t)");
    fRightMargin->Connect("ValueSet(Long_t)", "Bober::PadEditor", this, "ScaleMargin(Bool_t)");
    // fInit = kFALSE;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Pick up the used fill attributes.

  void PadEditor::SetModel(TObject* obj) {
    if (!obj || !obj->InheritsFrom("TPad")) return;

    //  fAvoidSignal = kTRUE;
    Bool_t on;

    on = fPad->HasFixedAspectRatio();
    /*  if (on)
        fFixedAR->SetState(kButtonDown);
      else
        fFixedAR->SetState(kButtonUp);
  */
    on = fPad->HasCrosshair();
    /*  if (on)
        fCrosshair->SetState(kButtonDown);
      else
        fCrosshair->SetState(kButtonUp);
  */
    on = fPad->IsEditable();
    /*    if (on)
          fEditable->SetState(kButtonDown);
        else
          fEditable->SetState(kButtonUp);
    */
    on = fPad->GetGridx();
    if (on)
      fGridX->SetState(kButtonDown);
    else
      fGridX->SetState(kButtonUp);

    on = fPad->GetGridy();
    if (on)
      fGridY->SetState(kButtonDown);
    else
      fGridY->SetState(kButtonUp);

    Int_t par;
    par = fPad->GetLogx();
    if (par)
      fLogX->SetState(kButtonDown);
    else
      fLogX->SetState(kButtonUp);

    par = fPad->GetLogy();
    if (par)
      fLogY->SetState(kButtonDown);
    else
      fLogY->SetState(kButtonUp);

    par = fPad->GetLogz();
    if (par)
      fLogZ->SetState(kButtonDown);
    else
      fLogZ->SetState(kButtonUp);

    par = fPad->GetTickx();
    if (par)
      fTickX->SetState(kButtonDown);
    else
      fTickX->SetState(kButtonUp);

    par = fPad->GetTicky();
    if (par)
      fTickY->SetState(kButtonDown);
    else
      fTickY->SetState(kButtonUp);

    par = fPad->GetBorderMode();
    if (par == -1) {
      // fBgroup->SetButton(77, kTRUE);
      fBsize->SetEnabled(kTRUE);
    } else if (par == 1) {
      // fBgroup->SetButton(79, kTRUE);
      fBsize->SetEnabled(kTRUE);
    } else {
      // fBgroup->SetButton(78, kTRUE);
      fBsize->SetEnabled(kFALSE);
    }
    par = fPad->GetBorderSize();
    if (par < 1) par = 1;
    if (par > 16) par = 16;
    fBsize->Select(par);
    fBottomMargin->SetNumber(fPad->GetBottomMargin(), kFALSE);
    fTopMargin->SetNumber(fPad->GetTopMargin(), kFALSE);
    fLeftMargin->SetNumber(fPad->GetLeftMargin(), kFALSE);
    fRightMargin->SetNumber(fPad->GetRightMargin(), kFALSE);

    if (fInit) ConnectSignals2Slots();

    // fAvoidSignal = kFALSE;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Exclude TAttLineEditor from this interface.

  void PadEditor::ActivateBaseClassEditors(TClass* cl) {
    //  fGedEditor->ExcludeClassEditor(TAttLine::Class());
    //  TGedFrame::ActivateBaseClassEditors(cl);
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'Editable'.

  void PadEditor::DoEditable(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetEditable(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'Crosshair'.

  void PadEditor::DoCrosshair(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetCrosshair(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'Fixed aspect ratio'.

  void PadEditor::DoFixedAspectRatio(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetFixedAspectRatio(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'GridX'.

  void PadEditor::DoGridX(Bool_t on) {
    if (fAvoidSignal) return;
    std::cout << "SET GRID" << std::endl;
    fPad->SetGridx(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'GridY'.

  void PadEditor::DoGridY(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetGridy(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'LogX'.

  void PadEditor::DoLogX(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetLogx(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'LogY'.

  void PadEditor::DoLogY(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetLogy(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'LogZ'.

  void PadEditor::DoLogZ(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetLogz(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'TickX'.

  void PadEditor::DoTickX(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetTickx(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the check box 'TickY'.

  void PadEditor::DoTickY(Bool_t on) {
    if (fAvoidSignal) return;
    fPad->SetTicky(on);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the border mode settings.

  void PadEditor::DoBorderMode() {
    if (fAvoidSignal) return;
    Int_t mode = 0;
    if (fBmode->GetState() == kButtonDown)
      mode = -1;
    else if (fBmode0->GetState() == kButtonDown)
      mode = 0;
    else
      mode = 1;

    if (!mode) {
      fBsize->SetEnabled(kFALSE);
    } else {
      fBsize->SetEnabled(kTRUE);
    }
    fPad->SetBorderMode(mode);
    Update();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Slot connected to the border size settings.

  void PadEditor::DoBorderSize(Int_t size) {
    if (fAvoidSignal) return;
    fPad->SetBorderSize(size);
    Update();
  }

  void PadEditor::ScaleMargin(Bool_t on) {
    fPad->SetBottomMargin(fBottomMargin->GetNumber());
    fPad->SetTopMargin(fTopMargin->GetNumber());
    fPad->SetLeftMargin(fLeftMargin->GetNumber());
    fPad->SetRightMargin(fRightMargin->GetNumber());
    Update();
    fPad->UpdateAsync();
  }

}  // namespace Bober
