/*
 * Style.cxx
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Style.h"
#include "XMLNode.h"

#include <initializer_list>

#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TVirtualPad.h>

namespace Bober {
  void Style::SetF(Int_t bit, Float_t val) {
    if (bit < 0 || bit >= 64) { throw std::out_of_range("Style::SetF - bit index out of range"); }
    fValues[bit].sFval = val;
    SETBIT(fFlags, bit);
  }

  void Style::SetI(Int_t bit, Int_t val) {
    if (bit < 0 || bit >= 64) { throw std::out_of_range("Style::SetF - bit index out of range"); }
    fValues[bit].sIval = val;
    SETBIT(fFlags, bit);
  }

}  // namespace Bober
