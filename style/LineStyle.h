/*
 * LineStyle.h
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#ifndef BOBER_LINESTYLE_H_
#define BOBER_LINESTYLE_H_

#include "Style.h"

namespace Bober {
  /**
   * contains information about line style
   */
  class LineStyle : public Style {


  public:
    static const unsigned short int kColor;
    static const unsigned short int kWidth;
    static const unsigned short int kStyle;
    LineStyle() {};

    void SetColor(Int_t val);

    void SetWidth(Int_t val);

    void SetStyle(Int_t val);

    Int_t GetColor() const;

    Int_t GetWidth() const;

    Int_t GetStyle() const;

    void ExportToXML(XMLNode* node) const;

    void ImportFromXML(XMLNode* node);


    template<class T>
    void Apply(T& obj) const;

    template<class T>
    void Import(const T& obj);
    virtual ~LineStyle() {};
    ClassDef(LineStyle, 1);
  };

} /* namespace Bober */

template<class T>
inline void Bober::LineStyle::Apply(T& obj) const {
  if (Find(kWidth)) obj.SetLineWidth(GetI(kWidth));
  if (Find(kColor)) obj.SetLineColor(GetI(kColor));
  if (Find(kStyle)) obj.SetLineStyle(GetI(kStyle));
}
template<class T>
inline void Bober::LineStyle::Import(const T& obj) {
  SetWidth(obj.GetLineWidth());
  SetColor(obj.GetLineColor());
  SetStyle(obj.GetLineStyle());
}

#endif /* BOBER_LINESTYLE_H_ */
