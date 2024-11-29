/*
 * HistogramEditor.h
 *
 *  Created on: 20 lis 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_EDITOR_HISTOGRAMEDITOR_H_
#define HAL_FEATURES_HIST_EDITOR_HISTOGRAMEDITOR_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TGFrame.h>
#include <TVirtualPad.h>


class TRootEmbeddedCanvas;

class TH1;
class TVirtualPad;

namespace Bober {

  class HistogramEditor : public TGMainFrame {
    TH1* fHisto                  = {nullptr};
    TRootEmbeddedCanvas* fCanvas = {nullptr};

  public:
    HistogramEditor(const TGWindow* p, UInt_t w, UInt_t h);
    HistogramEditor(TH1* histo = nullptr, TVirtualPad* pad = gPad);
    virtual ~HistogramEditor();
    ClassDef(HistogramEditor, 0)
  };


}  // namespace Bober

#endif /* HAL_FEATURES_HIST_EDITOR_HISTOGRAMEDITOR_H_ */
