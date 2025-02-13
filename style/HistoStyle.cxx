/*
 * HistoStyle.cxx
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "HistoStyle.h"

#include <Rtypes.h>
#include <TAttMarker.h>
#include <TGraphErrors.h>
#include <TH1.h>

#include "Style.h"
#include "XMLNode.h"

#include <type_traits>

namespace Bober {
  const unsigned short int HistoStyle::kTitle = 0;
  const unsigned short int HistoStyle::kMin   = 1;
  const unsigned short int HistoStyle::kMax   = 2;
  void HistoStyle::SetTitle(TString title) {
    SetI(kTitle, 0);
    fTitle = title;
  }

  AxisStyle& HistoStyle::GetAxisStyle(Char_t x) {
    if (x == 'y' || x == 'Y') return fYAxis;
    if (x == 'z' || x == 'Z') return fZAxis;
    return fXAxis;
  }

  void HistoStyle::ExportToXML(XMLNode* node) const {
    if (Find(kTitle)) node->AddAttrib(new Bober::XMLAttrib("Title", fTitle));
    if (Find(kMin)) node->AddAttrib(new Bober::XMLAttrib("Min", Form("%4.4f", GetF(kMin))));
    if (Find(kMax)) node->AddAttrib(new Bober::XMLAttrib("Max", Form("%4.4f", GetF(kMax))));
    XMLNode* xAxis  = new XMLNode("XAxis");
    XMLNode* yAxis  = new XMLNode("YAxis");
    XMLNode* zAxis  = new XMLNode("ZAxis");
    XMLNode* fill   = new XMLNode("Fill");
    XMLNode* marker = new XMLNode("Marker");
    XMLNode* line   = new XMLNode("Line");
    fXAxis.ExportToXML(xAxis);
    fYAxis.ExportToXML(yAxis);
    fZAxis.ExportToXML(zAxis);
    fMarker.ExportToXML(marker);
    fLine.ExportToXML(line);
    fFill.ExportToXML(fill);
    node->AddChild(xAxis);
    node->AddChild(yAxis);
    node->AddChild(zAxis);
    node->AddChild(fill);
    node->AddChild(marker);
    node->AddChild(line);
  }

  HistoStyle HistoStyle::GetStyle(TString style) {
    HistoStyle res;
    if (style.Contains("05")) {
      res.SetAxis(0.05, 0.8, 0.05, 0.005, 'x');
      res.SetAxis(0.05, 0.8, 0.05, 0.005, 'y');
    }
    if (style.Contains("color")) res.SetColor(kRed);
    if (style.Contains("circle")) res.GetMarkerStyle().SetStyle(kFullCircle);
    if (style.Contains("center")) {
      res.CenterTitle(kTRUE, 'x');
      res.CenterTitle(kTRUE, 'y');
    }
    return res;
  }

  void HistoStyle::SetMin(Double_t val) { SetF(kMin, val); }

  void HistoStyle::SetMax(Double_t val) { SetF(kMax, val); }

  void HistoStyle::SetMinMax(Double_t minVal, Double_t maxVal) {
    SetMin(minVal);
    SetMax(maxVal);
  }

  void HistoStyle::SetMarkerProperties(Color_t c, Int_t marker, Size_t size) {
    fMarker.SetColor(c);
    fMarker.SetStyle(marker);
    fMarker.SetSize(size);
  }

  void HistoStyle::SetLineProperties(Color_t c, Style_t style, Double_t widgth) {
    fLine.SetColor(c);
    fLine.SetStyle(style);
    fLine.SetWidth(widgth);
  }

  void HistoStyle::SetColor(Color_t c) {
    fLine.SetColor(c);
    fMarker.SetColor(c);
  }

  void HistoStyle::SetRangeUser(Double_t min, Double_t max, Char_t ax) {
    if (ax == 'x') fXAxis.SetRangeUser(min, max);
    if (ax == 'y') fYAxis.SetRangeUser(min, max);
    if (ax == 'z') fZAxis.SetRangeUser(min, max);
  }

  void HistoStyle::SetTitles(TString histo, TString xAxis, TString yAxis, TString zAxis) {
    SetTitle(histo);
    fXAxis.SetTitle(xAxis);
    fYAxis.SetTitle(yAxis);
    fZAxis.SetTitle(zAxis);
  }

  void HistoStyle::SetAxis(Double_t titleSize, Double_t titleOffset, Double_t labelSize, Double_t labelOffset, Char_t x) {
    auto setAx = [](AxisStyle& st, Double_t _titleSize, Double_t _titleOffset, Double_t _labelSize, Double_t _labelOffset) {
      st.SetTitleSize(_titleSize);
      st.SetTitleOffset(_titleOffset);
      st.SetLabelOffset(_labelOffset);
      st.SetLabelSize(_labelSize);
    };
    if (x == 'x') setAx(fXAxis, titleSize, titleOffset, labelSize, labelOffset);
    if (x == 'y') setAx(fYAxis, titleSize, titleOffset, labelSize, labelOffset);
    if (x == 'z') setAx(fZAxis, titleSize, titleOffset, labelSize, labelOffset);
  }

  void HistoStyle::CenterTitle(Bool_t center, Char_t x) {
    if (x == 'x') fXAxis.SetCenterTitle(center);
    if (x == 'y') fYAxis.SetCenterTitle(center);
    if (x == 'z') fZAxis.SetCenterTitle(center);
  }

  void HistoStyle::Apply(TObject& h, Int_t no) const {
    if (h.InheritsFrom("TH1")) {
      ApplyInternal(*dynamic_cast<TH1*>(&h));
      auto zax = static_cast<TH1&>(h).GetZaxis();
      if (zax) fZAxis.Apply(*zax);
    }
    if (h.InheritsFrom("TGraph")) { ApplyInternal(dynamic_cast<TGraph&>(h)); }
  }

  template<class T>
  void Bober::HistoStyle::ApplyInternal(T& object) const {
    if (Find(kTitle)) object.SetTitle(fTitle);
    if (Find(kMin)) object.SetMinimum(GetF(kMin));
    if (Find(kMax)) object.SetMaximum(GetF(kMax));

    fMarker.Apply(object);
    fLine.Apply(object);
    fFill.Apply(object);
    auto xax = object.GetXaxis();
    if (xax) fXAxis.Apply(*xax);
    auto yax = object.GetYaxis();
    if (yax) fYAxis.Apply(*yax);
  }

  void HistoStyle::ImportFromXML(XMLNode* node) {
    if (auto atr = node->GetAttrib("Title")) {
      TString x = atr->GetValue();
      SetTitle(x);
    }
    if (auto atr = node->GetAttrib("Min")) {
      float x = atr->GetValue().Atof();
      SetMin(x);
    }
    if (auto atr = node->GetAttrib("Max")) {
      float x = atr->GetValue().Atof();
      SetMax(x);
    }
    if (auto xAxis = node->GetChild("XAxis")) { fXAxis.ImportFromXML(xAxis); }
    if (auto yAxis = node->GetChild("YAxis")) { fYAxis.ImportFromXML(yAxis); }
    if (auto zAxis = node->GetChild("ZAxis")) { fZAxis.ImportFromXML(zAxis); }
    if (auto xAxis = node->GetChild("Fill")) { fFill.ImportFromXML(xAxis); }
    if (auto xAxis = node->GetChild("Marker")) { fMarker.ImportFromXML(xAxis); }
    if (auto line = node->GetChild("Line")) { fLine.ImportFromXML(line); }
  }

  void HistoStyle::Import(const TObject& obj) {
    if (obj.InheritsFrom("TH1")) {
      auto histo = static_cast<const TH1D&>(obj);
      fTitle     = histo.GetTitle();
      fXAxis.Import(*histo.GetXaxis());
      fYAxis.Import(*histo.GetYaxis());
      if (histo.GetZaxis()) { fZAxis.Import(*histo.GetZaxis()); }
      fMarker.Import(histo);
      fLine.Import(histo);
      fFill.Import(histo);
    }
  }

} /* namespace Bober */
