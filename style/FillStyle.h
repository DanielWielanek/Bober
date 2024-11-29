/*
 * FillStyle.h
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#ifndef BOBER_FILLSTYLE_H_
#define BOBER_FILLSTYLE_H_

#include <RtypesCore.h>

#include "Style.h"

namespace Bober {
  /**
   * contains information about fill styles
   */
  class FillStyle : public Style {
  public:
    static const unsigned short int kColor;
    static const unsigned short int kStyle;
    FillStyle() {};

    void SetColor(Int_t val);

    void SetStyle(Int_t val);

    Int_t GetColor() const;

    Int_t GetStyle() const;

    void ExportToXML(XMLNode* node) const;

    void ImportFromXML(XMLNode* node);

    template<class T>
    void Apply(T& x) const;

    template<class T>
    void Import(const T& x);

    virtual ~FillStyle() {};
    ClassDef(FillStyle, 1);
  };

} /* namespace Bober */

template<class T>
inline void Bober::FillStyle::Apply(T& x) const {
  if (Find(kColor)) x.SetFillColor(GetI(kColor));
  if (Find(kStyle)) x.SetFillStyle(GetI(kStyle));
}

template<class T>
inline void Bober::FillStyle::Import(const T& x) {
  SetColor(x.GetFillColor());
  SetStyle(x.GetFillStyle());
}

#endif /* BOBER_FILLSTYLE_H_ */
