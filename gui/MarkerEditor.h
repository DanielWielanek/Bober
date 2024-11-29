/*
 * MarkerEditor.h
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#ifndef BASIC_MARKEREDITOR_H_
#define BASIC_MARKEREDITOR_H_

#include <GuiTypes.h>
#include <Rtypes.h>
#include <RtypesCore.h>
#include <TObject.h>

#include "MainEditor.h"

class TGCompositeFrame;

class TAttMarker;
class TGColorSelect;
class TGHSlider;
class TGNumberEntry;
class TGNumberEntryField;
class TGedMarkerSelect;


namespace Bober {

  class MarkerEditor : public MainEditor {
    TAttMarker* fAttMarker;         ///< marker attribute object
    TGNumberEntry* fMarkerSize;     ///< marker size combo box
    TGColorSelect* fColorSelect;    ///< marker color
    TGedMarkerSelect* fMarkerType;  ///< marker type
    Bool_t fSizeForText;            ///< true if "text" draw option uses marker size
#ifdef ALFA__
    TGHSlider* fAlpha;  ///< fill opacity
    TGNumberEntryField* fAlphaField;
#endif

    virtual void ConnectSignals2Slots();

  public:
    MarkerEditor(TGCompositeFrame* frame, TVirtualPad* pad);
    virtual ~MarkerEditor();

    virtual void SetModel(TObject* obj);
    virtual void DoMarkerColor(Pixel_t color);
    virtual void DoMarkerAlphaColor(ULongptr_t p);
    virtual void DoMarkerSize();
    virtual void DoMarkerStyle(Style_t style);
    virtual void DoAlpha();
    virtual void DoAlphaField();
    virtual void DoLiveAlpha(Int_t a);
    virtual void GetCurAlpha();

    ClassDef(MarkerEditor, 0)  // GUI for editing marker attributes
  };
} /* namespace Bober */

#endif /* BASIC_MARKEREDITOR_H_ */
