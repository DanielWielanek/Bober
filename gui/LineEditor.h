/*
 * LineEditor.h
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#ifndef BASIC_LINEEDITOR_H_
#define BASIC_LINEEDITOR_H_

#include <GuiTypes.h>
#include <RtypesCore.h>

#include "MainEditor.h"

class TAttLine;
class TGColorSelect;
class TGHSlider;
class TGLineStyleComboBox;
class TGLineWidthComboBox;
class TGNumberEntryField;
class TGWindow;

namespace Bober {

  class LineEditor : public MainEditor {
  protected:
    TAttLine* fAttLine;                ///< line attribute object
    TGLineStyleComboBox* fStyleCombo;  ///< line style combo box
    TGLineWidthComboBox* fWidthCombo;  ///< line width combo box
    TGColorSelect* fColorSelect;       ///< line color widget
    TGHSlider* fAlpha;                 ///< fill opacity
    TGNumberEntryField* fAlphaField;
    Int_t fPriority;

    virtual void ConnectSignals2Slots();

  public:
    LineEditor(TGCompositeFrame* frame, TVirtualPad* pad);
    virtual ~LineEditor();

    virtual void SetModel(TObject* obj);
    virtual void DoLineColor(Pixel_t color);
    virtual void DoLineAlphaColor(ULongptr_t p);
    virtual void DoLineStyle(Int_t style);
    virtual void DoLineWidth(Int_t width);
    virtual void DoAlpha();
    virtual void DoAlphaField();
    virtual void DoLiveAlpha(Int_t a);
    virtual void GetCurAlpha();

    ClassDef(LineEditor, 0)  // GUI for editing line attributes
  };

} /* namespace Bober */

#endif /* BASIC_LINEEDITOR_H_ */
