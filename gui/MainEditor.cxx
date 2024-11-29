/*
 * MainEditor.cpp
 *
 *  Created on: 21 lis 2024
 *      Author: daniel
 */

#include "MainEditor.h"

#include <TGButton.h>
#include <TGMenu.h>
#include <TVirtualPad.h>

namespace Bober {

  MainEditor::MainEditor(TGCompositeFrame* frame, TVirtualPad* pad) : fFrame(frame), fPad(pad) {
    if (!frame || !pad) { throw std::invalid_argument("MainEditor: frame and pad pointers must not be null"); }
  }

  void MainEditor::Update() {
    if (fAvoidSignal) return;
    fPad->Modified(1);
    fPad->Update();
  }

  MainEditor::~MainEditor() {}

} /* namespace Bober */
