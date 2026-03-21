#pragma once
#include "SelectMgr_EntityOwner.hxx"
#include "AIS_InteractiveObject.hxx"
#include "PrsMgr_PresentationManager.hxx"
#include "myAisComman.h"

class MyAisOwner : public SelectMgr_EntityOwner
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisOwner , SelectMgr_EntityOwner)
public:
  MyAisOwner(const Handle(MyAisObject)& theObj, int thePriority = 0);
  virtual void HilightWithColor(const Handle(PrsMgr_PresentationManager)& thePrsMgr, 
                                const Handle(Prs3d_Drawer)& theStyle, 
                                const Standard_Integer theMode) override;
  virtual void Unhilight(const Handle(PrsMgr_PresentationManager)& thePrsMgr, 
                         const Standard_Integer theMode) override;
protected:
  Handle(Prs3d_Presentation) myPrs;
};

class MyAisOwner1 : public SelectMgr_EntityOwner
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisOwner1 , SelectMgr_EntityOwner)
public:
  MyAisOwner1(const Handle(MyAisObject)& theObj , int thePriority = 0);
  virtual void HilightWithColor(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                                const Handle(Prs3d_Drawer)& theStyle ,
                                const Standard_Integer theMode) override;
  virtual void Unhilight(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                         const Standard_Integer theMode) override;
  virtual bool IsForcedHilight() const override { return true; }
protected:
  Handle(Prs3d_Presentation) myPrs;
};

