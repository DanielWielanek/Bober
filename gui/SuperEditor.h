/*
 * SuperEditor.h
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#ifndef BASIC_SUPEREDITOR_H_
#define BASIC_SUPEREDITOR_H_

#include <Rtypes.h>

#include "MainEditor.h"
class TGTextEntry;
namespace Bober {
  class FillEditor;
  class LineEditor;
  class MarkerEditor;
  class AxisEditor;
} /* namespace Bober */
// class TGFileDialog;
namespace Bober {

  class SuperEditor : public MainEditor {
    MarkerEditor* fMarkerEdit;
    LineEditor* fLineEdit;
    FillEditor* fFillEdit;
    TGTextEntry* fSaveAs;
    TGTextEntry* fRedraw;
    TObject* fObject;
    AxisEditor* fXaxis;
    AxisEditor* fYaxis;

  public:
    SuperEditor(TGCompositeFrame* frame, TVirtualPad* pad);
    void DoSave();
    void Redraw();
    void SetAxesEditors(AxisEditor* x, AxisEditor* b);
    virtual void SetModel(TObject* obj);
    virtual ~SuperEditor();
    ClassDef(MainEditor, 0)
  };

} /* namespace Bober */

#endif /* BASIC_SUPEREDITOR_H_ */
