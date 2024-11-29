/*
 * MainEditor.h
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#ifndef BASIC_MAINEDITOR_H_
#define BASIC_MAINEDITOR_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TObject.h>


class TVirtualPad;

class TGCompositeFrame;

namespace Bober {

  class MainEditor : public TObject {
  protected:
    TGCompositeFrame* fFrame = {nullptr};
    Bool_t fAvoidSignal      = {kTRUE};
    Bool_t fInit             = {kFALSE};
    TVirtualPad* fPad        = {nullptr};


  public:
    MainEditor(TGCompositeFrame* frame, TVirtualPad* pad);
    virtual void Update();
    virtual ~MainEditor();
    ClassDef(MainEditor, 0)
  };

} /* namespace Bober */

#endif /* BASIC_MAINEDITOR_H_ */
