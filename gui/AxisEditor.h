/*
 * AxisEditor.h
 *
 *  Created on: 20 lis 2024
 *      Author: daniel
 */

#ifndef BASIC_AXISEDITOR_H_
#define BASIC_AXISEDITOR_H_

#include <GuiTypes.h>
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TObject.h>

#include "MainEditor.h"

class TAxis;
class TGCheckButton;
class TGFontTypeComboBox;
class TGTextEntry;
class TGNumberEntry;
class TGColorSelect;
class TGCompositeFrame;
class TVirtualPad;

namespace Bober {

  class AxisEditor : public MainEditor {
  protected:
    TAxis* fAxis;                    ///< axis object
    TGColorSelect* fAxisColor;       ///< color selection widget
                                     //  TGCheckButton* fLogAxis;         ///< logarithmic check box
    TGNumberEntry* fTickLength;      ///< tick length number entry
    TGNumberEntry* fDiv1;            ///< primary axis division number entry
    TGNumberEntry* fDiv2;            ///< secondary axis division number entry
    TGNumberEntry* fDiv3;            ///< tertiary axis division number entry
    TGCheckButton* fOptimize;        ///< tick optimization check box
    TGCheckButton* fTicksBoth;       ///< check box setting ticks on both axis sides
    TGCheckButton* fMoreLog;         ///< more logarithmic labels check box
    Int_t fTicksFlag;                ///< positive/negative ticks' flag
    TGTextEntry* fTitle;             ///< axis title input field
    TGColorSelect* fTitleColor;      ///< color selection widget
    TGFontTypeComboBox* fTitleFont;  ///< title font combo box
    Int_t fTitlePrec;                ///< font precision level
    TGNumberEntry* fTitleSize;       ///< title size number entry
    TGNumberEntry* fTitleOffset;     ///< title offset number entry
    TGCheckButton* fCentered;        ///< check button for centered title
    TGCheckButton* fRotated;         ///< check button for rotated title
    TGColorSelect* fLabelColor;      ///< color selection widget
    TGFontTypeComboBox* fLabelFont;  ///< label font combo box
    Int_t fLabelPrec;                ///< font precision level
    TGNumberEntry* fLabelSize;       ///< label size number entry
    TGNumberEntry* fLabelOffset;     ///< label offset number entry
    TGCheckButton* fNoExponent;      ///< check box for No exponent choice
    TGCheckButton* fDecimal;         ///< decimal part check box
    virtual void ConnectSignals2Slots();
    void BuildTickEditor();
    void BuildTitleEditor();
    void BuildLabelEditor();

  public:
    AxisEditor(TGCompositeFrame* frame, TVirtualPad* pad);
    virtual ~AxisEditor();
    virtual void SetModel(TObject* obj);
    // slots related to axis attributes
    virtual void DoTickLength();
    virtual void DoAxisColor(Pixel_t color);
    virtual void DoTicks();
    virtual void DoDivisions();
    virtual void DoLogAxis();
    virtual void DoMoreLog();
    // slots related to axis title attributes
    virtual void DoTitleColor(Pixel_t color);
    virtual void DoTitle(const char* text);
    virtual void DoTitleSize();
    virtual void DoTitleFont(Int_t font);
    virtual void DoTitleOffset();
    virtual void DoTitleCentered();
    virtual void DoTitleRotated();
    // slots related to axis labels attributes
    virtual void DoLabelColor(Pixel_t color);
    virtual void DoLabelSize();
    virtual void DoLabelFont(Int_t font);
    virtual void DoLabelOffset();
    virtual void DoNoExponent();
    virtual void DoDecimal(Bool_t on);

    Bool_t ForcedNdiv() const;

    ClassDef(AxisEditor, 0)  // axis editor
  };

}  // namespace Bober

#endif /* BASIC_AXISEDITOR_H_ */
