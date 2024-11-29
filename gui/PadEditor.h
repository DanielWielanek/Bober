/*
 * PadEditor.h
 *
 *  Created on: 20 lis 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_EDITOR_PADEDITOR_H_
#define HAL_FEATURES_HIST_EDITOR_PADEDITOR_H_

#include <GuiTypes.h>
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TGedFrame.h>

#include "MainEditor.h"

class TGButtonGroup;
class TGLineWidthComboBox;
class TGRadioButton;
class TGNumberEntry;

class TGCheckButton;
class TPad;

class TGFrame;

namespace Bober {

  class PadEditor : public MainEditor {

  protected:
    // TGCheckButton* fEditable;     ///< set pad editable
    // TGCheckButton* fCrosshair;    ///< set crosshair
    // TGCheckButton* fFixedAR;      ///< set fixed aspect ratio
    TGCheckButton* fGridX;        ///< set grid on X
    TGCheckButton* fGridY;        ///< set grid on Y
    TGCheckButton* fLogX;         ///< set log scale on X
    TGCheckButton* fLogY;         ///< set log scale on Y
    TGCheckButton* fLogZ;         ///< set log scale on Z
    TGCheckButton* fTickX;        ///< set ticks on X
    TGCheckButton* fTickY;        ///< set ticks on Y
    TGRadioButton* fBmode;        ///< set sinken pad border mode
    TGRadioButton* fBmode0;       ///< set no pad border
    TGRadioButton* fBmode1;       ///< set raised pad border mode
    TGLayoutHints* fBmodelh;      ///< layout hints for border mode buttons
    TGLineWidthComboBox* fBsize;  ///< set pad border size
    TGButtonGroup* fBgroup;       ///< button group of border mode


    TGNumberEntry* fBottomMargin;
    TGNumberEntry* fTopMargin;
    TGNumberEntry* fLeftMargin;
    TGNumberEntry* fRightMargin;
    virtual void ConnectSignals2Slots();

  public:
    PadEditor(TGCompositeFrame* frame, TVirtualPad* pad);
    virtual ~PadEditor();

    virtual void SetModel(TObject* obj);
    virtual void ActivateBaseClassEditors(TClass* cl);

    virtual void DoEditable(Bool_t on);
    virtual void DoCrosshair(Bool_t on);
    virtual void DoFixedAspectRatio(Bool_t on);
    virtual void DoGridX(Bool_t on);
    virtual void DoGridY(Bool_t on);
    virtual void DoLogX(Bool_t on);
    virtual void DoLogY(Bool_t on);
    virtual void DoLogZ(Bool_t on);
    virtual void DoTickX(Bool_t on);
    virtual void DoTickY(Bool_t on);
    virtual void DoBorderMode();
    virtual void DoBorderSize(Int_t size);
    virtual void ScaleMargin(Bool_t on);

    ClassDef(PadEditor, 0)  // editor of TPad objects
  };


}  // namespace Bober

#endif /* HAL_FEATURES_HIST_EDITOR_PADEDITOR_H_ */
