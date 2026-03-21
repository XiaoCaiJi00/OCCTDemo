#include "myAisOwner.h"
#include "Prs3d_PresentationShadow.hxx"
#include "StdSelect_ViewerSelector3d.hxx"
#include "AIS_InteractiveContext.hxx"
#include "Prs3d_Arrow.hxx"
#include "Prs3d_ArrowAspect.hxx"
#include "SelectMgr_SelectableObject.hxx"

MyAisOwner::MyAisOwner(const Handle(MyAisObject)& theObj, int thePriority) : SelectMgr_EntityOwner(theObj , thePriority)
{
}

void MyAisOwner::HilightWithColor(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                                  const Handle(Prs3d_Drawer)& theStyle ,
                                  const Standard_Integer theMode)
{
  //base_type::HilightWithColor(thePrsMgr , theStyle , theMode);
  if (myPrs.IsNull())
  {
    myPrs = new Prs3d_Presentation(thePrsMgr->StructureManager());
    MyAisObject* anObj = dynamic_cast<MyAisObject*> (mySelectable);
    anObj->Compute(thePrsMgr , myPrs , MyAisObject::MyDispMode_Highlight);
  }
  if (thePrsMgr->IsImmediateModeOn())
  {
    Handle(Prs3d_PresentationShadow) aShadow =
      new Prs3d_PresentationShadow(thePrsMgr->StructureManager() , myPrs);
    aShadow->SetZLayer(Graphic3d_ZLayerId_Top);
    aShadow->Highlight(theStyle);
    thePrsMgr->AddToImmediateList(aShadow);
  }
  else
  {
    myPrs->Display();
  }
}

void MyAisOwner::Unhilight(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Standard_Integer theMode)
{
  //base_type::Unhilight(thePrsMgr , theMode);
  if (!myPrs.IsNull()) 
  { 
    myPrs->Erase(); 
  }
}


MyAisOwner1::MyAisOwner1(const Handle(MyAisObject)& theObj , int thePriority) : SelectMgr_EntityOwner(theObj , thePriority)
{
}

void MyAisOwner1::HilightWithColor(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                                  const Handle(Prs3d_Drawer)& theStyle ,
                                  const Standard_Integer theMode)
{
  // 基本参数检查
  if (thePrsMgr.IsNull() || theStyle.IsNull() || mySelectable == nullptr)
    return;

  MyAisObject* anObj = dynamic_cast<MyAisObject*>(mySelectable);
  if (!anObj || !anObj->InteractiveContext())
    return;

  if (!thePrsMgr->IsImmediateModeOn())
    return;

  Handle(StdSelect_ViewerSelector3d) aSelector = anObj->InteractiveContext()->MainSelector();
  if (aSelector.IsNull())
    return;

  // 查找选择点
  SelectMgr_SortCriterion aPickPnt;
  bool found = false;
  for (int aPickIter = 1; aPickIter <= aSelector->NbPicked(); ++aPickIter)
  {
    if (aSelector->Picked(aPickIter) == this)
    {
      aPickPnt = aSelector->PickedData(aPickIter);
      found = true;
      break;
    }
  }

  if (!found)
    return;

  // 创建和设置演示
  Handle(Prs3d_Presentation) aPrs = mySelectable->GetHilightPresentation(thePrsMgr);
  if (aPrs.IsNull())
    return;

  aPrs->SetZLayer(Graphic3d_ZLayerId_Top);
  aPrs->Clear();

  // 创建箭头
  Handle(Graphic3d_Group) aGroupPnt = aPrs->NewGroup();
  if (aGroupPnt.IsNull() || theStyle->ArrowAspect().IsNull())
    return;

  aGroupPnt->SetGroupPrimitivesAspect(theStyle->ArrowAspect()->Aspect());

  try {
    gp_Trsf aTrsfInv = mySelectable->LocalTransformation().Inverted();
    gp_Dir aNorm(aPickPnt.Normal.x() , aPickPnt.Normal.y() , aPickPnt.Normal.z());

    Handle(Graphic3d_ArrayOfTriangles) aTris = Prs3d_Arrow::DrawShaded(
        gp_Ax1(aPickPnt.Point , aNorm).Transformed(aTrsfInv) ,
        1.0 , 15.0 , 3.0 , 4.0 , 10);

    if (!aTris.IsNull())
    {
      aGroupPnt->AddPrimitiveArray(aTris);
      thePrsMgr->AddToImmediateList(aPrs);
    }
  }
  catch(const Standard_Failure & e)
  {
    // 处理可能的异常
    return;
  }
}

void MyAisOwner1::Unhilight(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Standard_Integer theMode)
{
  //base_type::Unhilight(thePrsMgr , theMode);
  if (!myPrs.IsNull())
  {
    myPrs->Erase();
  }
}