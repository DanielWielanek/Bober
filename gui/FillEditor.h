/*
 * FillEditor.h
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#ifndef BASIC_FILLEDITOR_H_
#define BASIC_FILLEDITOR_H_

#include <GuiTypes.h>
#include <Rtypes.h>
#include <RtypesCore.h>

#include "MainEditor.h"

class TAttFill;
class TGColorSelect;
class TGedPatternSelect;
class TGHSlider;
class TGNumberEntryField;

namespace Bober {

  class FillEditor : public MainEditor {
  protected:
    TAttFill* fAttFill;                 ///< fill attribute object
    TGColorSelect* fColorSelect;        ///< fill color widget
    TGedPatternSelect* fPatternSelect;  ///< fill pattern widget
    TGHSlider* fAlpha;                  ///< fill opacity
    TGNumberEntryField* fAlphaField;
    Int_t fPriority;
    virtual void ConnectSignals2Slots();

  public:
    FillEditor(TGCompositeFrame* frame, TVirtualPad* pad);
    virtual ~FillEditor();

    virtual void SetModel(TObject* obj);
    virtual void DoFillColor(Pixel_t color);
    virtual void DoFillAlphaColor(ULongptr_t p);
    virtual void DoFillPattern(Style_t color);
    virtual void DoAlpha();
    virtual void DoAlphaField();
    virtual void DoLiveAlpha(Int_t a);
    virtual void GetCurAlpha();

    ClassDef(FillEditor, 0)  // GUI for editing fill attributes
  };

} /* namespace Bober */

#endif /* BASIC_FILLEDITOR_H_ */
