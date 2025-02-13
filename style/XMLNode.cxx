/*
 * XMLNode.cxx
 *
 *  Created on: 01-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "XMLNode.h"

#include <TDOMParser.h>
#include <TXMLAttr.h>      // for TXMLAttr
#include <TXMLDocument.h>  // for TXMLDocument
#include <TXMLNode.h>
#include <iostream>

namespace Bober {
  XMLNode::XMLNode(TString name, TString value) : TNamed(name, value) {
    fChildren.SetOwner(kTRUE);
    fAttrib.SetOwner(kTRUE);
  }

  XMLNode::XMLNode(const XMLNode& other) : XMLNode(other.GetName(), other.GetValue()) {
    for (int i = 0; i < other.fChildren.GetEntries(); i++) {
      fChildren.Add(new XMLNode(*other.GetChild(i)));
    }
    for (int i = 0; i < other.fAttrib.GetEntries(); i++) {
      fAttrib.Add(new XMLAttrib(*other.GetAttrib(i)));
    }
  }

  XMLNode& XMLNode::operator=(const XMLNode& other) {
    if (&other == this) return *this;
    SetName(other.GetName());
    SetValue(other.GetValue());
    fChildren.Clear();
    fAttrib.Clear();
    for (int i = 0; i < other.fChildren.GetEntries(); i++) {
      fChildren.Add(new XMLNode(*other.GetChild(i)));
    }
    for (int i = 0; i < other.fAttrib.GetEntries(); i++) {
      fAttrib.Add(new XMLAttrib(*other.GetAttrib(i)));
    }
    return *this;
  }

  void XMLNode::Copy(TXMLNode* node) {
    fChildren.Clear();
    fAttrib.Clear();
    SetName(node->GetNodeName());
    SetTitle(node->GetText());
    if (node->HasChildren()) {
      TXMLNode* child = node->GetChildren();
      do {
        if (child == nullptr) break;
        TString name = child->GetNodeName();
        if (name != "text") {  // skip "text" nodes
          XMLNode* tempnode = new XMLNode();
          tempnode->Copy(child);
          fChildren.Add(tempnode);
        }
        if (child->HasNextNode()) child = child->GetNextNode();
      } while (child->HasNextNode());
    }
    if (node->HasAttributes()) {
      TList* atr_list = node->GetAttributes();
      for (int i = 0; i < atr_list->GetEntries(); i++) {
        TXMLAttr* atrib = (TXMLAttr*) atr_list->At(i);
        fAttrib.Add(new XMLAttrib(atrib->GetName(), atrib->GetValue()));
      }
    }
  }

  void XMLNode::AddAttrib(XMLAttrib* attrib) {
    TString new_atr = attrib->GetName();
    if (GetAttrib(new_atr) != nullptr) {
      std::cout << "XMLNode::AddAttrib Can't have two attributes with the same name!" << std::endl;
      return;
    }
    fAttrib.AddLast(attrib);
  }

  Int_t XMLNode::GetNChildren(TString name) const {
    Int_t counter = 0;
    for (int i = 0; i < GetNChildren(); i++) {
      TString name_temp = GetChild(i)->GetName();
      if (name_temp == name) { counter++; }
    }
    return counter;
  }

  XMLNode* XMLNode::GetChild(TString name, Int_t count) const {
    Int_t control_index = 0;
    for (int i = 0; i < fChildren.GetEntries(); i++) {
      XMLNode* node = GetChild(i);
      TString temp  = node->GetName();
      if (temp == name) { control_index++; }
      if (control_index > count) return node;
    }
    return nullptr;
  }

  XMLAttrib* XMLNode::GetAttrib(TString name) const { return static_cast<XMLAttrib*>(fAttrib.FindObject(name)); }

  XMLNode* XMLNode::GetChild(Int_t index) const { return static_cast<XMLNode*>(fChildren.At(index)); }

  XMLAttrib* XMLNode::GetAttrib(Int_t index) const { return static_cast<XMLAttrib*>(fAttrib.At(index)); }

  XMLNode::~XMLNode() {}

  //---------- BoberXMLFile ------------------------------------------------------------------------------------

  XMLFile::XMLFile(TString name, TString mode) : fName(name) {
    if (mode == "read" || mode == "READ") {
      fOverwrite = kFALSE;
      TDOMParser Parser;
      Parser.SetValidate(kFALSE);
      Parser.ParseFile(name);
      TXMLNode* MainNode = Parser.GetXMLDocument()->GetRootNode();
      fRootNode.reset(new XMLNode());
      fRootNode->Copy(MainNode);
    } else {
      fOverwrite = kTRUE;
    }
  }

  void XMLFile::CreateRootNode(TString name) { fRootNode.reset(new XMLNode(name)); }

  void XMLFile::SetRootNode(XMLNode* node) { fRootNode.reset(node); }

  void XMLFile::Close() {
    if (fOverwrite) {
      if (!fRootNode) {
        std::cout << "BoberXMLFile::Close() No root node!" << std::endl;
        return;
      }
      TXMLEngine engine;
      XMLNodePointer_t mainnode = engine.NewChild(0, 0, fRootNode->GetName());
      ExportNode(mainnode, engine, *fRootNode.get());
      XMLDocPointer_t xmldoc = engine.NewDoc();
      engine.DocSetRootElement(xmldoc, mainnode);
      engine.SaveDoc(xmldoc, fName);
      engine.FreeDoc(xmldoc);
    }
  }

  void XMLFile::ExportNode(XMLNodePointer_t& nodePointer, TXMLEngine& engine, const XMLNode& node) const {
    for (int i = 0; i < node.GetNChildren(); i++) {
      XMLNodePointer_t child = engine.NewChild(nodePointer, 0, node.GetChild(i)->GetName(), node.GetChild(i)->GetValue());
      for (int j = 0; j < node.GetChild(i)->GetNAttributes(); j++) {
        engine.NewAttr(child, 0, node.GetChild(i)->GetAttrib(j)->GetName(), node.GetChild(i)->GetAttrib(j)->GetValue());
      }
      ExportNode(child, engine, *node.GetChild(i));
    }
  }

  XMLFile::~XMLFile() {
    if (fRootNode && fOverwrite) Close();
  }
}  // namespace Bober
