#pragma once
#include "myShape.h"

class MyAisObject : public AIS_InteractiveObject
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisObject, AIS_InteractiveObject)
public:
  enum MyDispMode { MyDispMode_Main = 0 , MyDispMode_Highlight = 1 };
  MyAisObject(){}
  virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Handle(Prs3d_Presentation)& thePrs ,
                        const Standard_Integer theMode) override
  {
    //AIS_InteractiveObject::Compute(thePrsMgr , thePrs , theMode);
  }

  virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel ,
                                 const Standard_Integer theMode) override
  {
    //AIS_InteractiveObject::ComputeSelection(theSel , theMode);
  }
  virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
  {
    //return AIS_InteractiveObject::AcceptDisplayMode(theMode);
    return true;
  }
private:
  TopoDS_Shape m_shape;
};