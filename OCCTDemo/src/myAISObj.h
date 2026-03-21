#pragma once
#include "myShape.h"
#include "AIS_InteractiveObject.hxx"
#include "StdPrs_WFShape.hxx"
#include "BRepBndLib.hxx"
#include "Prs3d_BndBox.hxx"
#include "myAisComman.h"


class MyAisObject0 : public MyAisObject
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisObject0 , MyAisObject)
public:
  enum MyDispMode { MyDispMode_Main = 0 , MyDispMode_Highlight = 1 };
  MyAisObject0();
  MyAisObject0(const TopoDS_Shape& aShape);
  virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Handle(Prs3d_Presentation)& thePrs ,
                        const Standard_Integer theMode) override;

  virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel ,
                                 const Standard_Integer theMode) override;
  virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override;
private:
  TopoDS_Shape m_shape;
};

class MyAisObject1 : public MyAisObject
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisObject1 , MyAisObject)
public:
  enum MyDispMode { MyDispMode_Main = 0 , MyDispMode_Highlight = 1 };
  MyAisObject1();
  MyAisObject1(const TopoDS_Shape& aShape);
  virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Handle(Prs3d_Presentation)& thePrs ,
                        const Standard_Integer theMode) override;

  virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel ,
                                 const Standard_Integer theMode) override;
  virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override;
private:
  TopoDS_Shape m_shape;
};

class MyAisObject2 : public MyAisObject
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisObject2 , MyAisObject)
public:
  enum MyDispMode { MyDispMode_Main = 0 , MyDispMode_Highlight = 1 };
  MyAisObject2();
  MyAisObject2(const TopoDS_Shape& aShape);
  virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Handle(Prs3d_Presentation)& thePrs ,
                        const Standard_Integer theMode) override;

  virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel ,
                                 const Standard_Integer theMode) override;
  virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override;
private:
  TopoDS_Shape m_shape;
};

class MyAisObject3 : public MyAisObject
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisObject3 , MyAisObject)
public:
  enum MyDispMode { MyDispMode_Main = 0 , MyDispMode_Highlight = 1 };
  MyAisObject3();
  MyAisObject3(const TopoDS_Shape& aShape);
  virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Handle(Prs3d_Presentation)& thePrs ,
                        const Standard_Integer theMode) override;

  virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel ,
                                 const Standard_Integer theMode) override;
  virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override;
private:
  TopoDS_Shape m_shape;
};

class MyAisObject4 : public MyAisObject
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisObject4 , MyAisObject)
public:
  enum MyDispMode { MyDispMode_Main = 0 , MyDispMode_Highlight = 1 };
  MyAisObject4();
  MyAisObject4(const TopoDS_Shape& aShape);
  virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Handle(Prs3d_Presentation)& thePrs ,
                        const Standard_Integer theMode) override;

  virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel ,
                                 const Standard_Integer theMode) override;
  virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override;
private:
  TopoDS_Shape m_shape;
};

class MyAisObject5 : public MyAisObject
{
  DEFINE_STANDARD_RTTI_INLINE(MyAisObject5 , MyAisObject)
public:
  enum MyDispMode { MyDispMode_Main = 0 , MyDispMode_Highlight = 1 };
  MyAisObject5();
  MyAisObject5(const TopoDS_Shape& aShape);
  virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr ,
                        const Handle(Prs3d_Presentation)& thePrs ,
                        const Standard_Integer theMode) override;

  virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel ,
                                 const Standard_Integer theMode) override;
  virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override;
private:
  TopoDS_Shape m_shape;
};