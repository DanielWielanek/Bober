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

  MainEditor::MainEditor(TGCompositeFrame* frame, TVirtualPad* pad) : fFrame(frame), fPad(pad) {}

  void MainEditor::Update() {
    if (fAvoidSignal) return;
    fPad->Modified(1);
    fPad->Update();
  }

  MainEditor::~MainEditor() {}

} /* namespace Bober */
