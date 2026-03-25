#include "myAISObj.h"
#include "PrsMgr_PresentableObject.hxx"
#include "Prs3d_ShadingAspect.hxx"
#include "Prs3d_ToolCylinder.hxx"
#include "Select3D_SensitiveBox.hxx"
#include "SelectMgr_EntityOwner.hxx"
#include "StdPrs_ToolTriangulatedShape.hxx"
#include "StdSelect_BRepSelectionTool.hxx"
#include "Select3D_SensitivePrimitiveArray.hxx"
#include "myAisOwner.h"
#include "Prs3d_ToolDisk.hxx"


MyAisObject0::MyAisObject0()
{
  SetDisplayMode(MyDispMode_Main);      // main (active) display mode
  SetHilightMode(MyDispMode_Highlight); // auxiliary (highlighting) mode
}
MyAisObject0::MyAisObject0(const TopoDS_Shape& aShape)
{
  m_shape = aShape;
}
void MyAisObject0::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr , const Handle(Prs3d_Presentation)& thePrs , const Standard_Integer theMode)
{
  if (theMode == 0)
  {
    StdPrs_ShadedShape::Add(thePrs , m_shape , myDrawer);
    StdPrs_WFShape::Add(thePrs , m_shape , myDrawer); // add wireframe
  }
  else if (theMode == 1)
  {
    Bnd_Box aBox;
    BRepBndLib::Add(m_shape , aBox);
    Prs3d_BndBox::Add(thePrs , aBox , myDrawer); 
  }
}

void MyAisObject0::ComputeSelection(const Handle(SelectMgr_Selection)& theSel , const Standard_Integer theMode)
{
}

bool MyAisObject0::AcceptDisplayMode(const Standard_Integer theMode) const
{
  return theMode == MyDispMode_Main || theMode == MyDispMode_Highlight;
}


MyAisObject1::MyAisObject1()
{
  SetHilightMode(MyDispMode_Highlight);
  myDrawer->SetupOwnShadingAspect();  
  myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
  myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN , Aspect_TOL_SOLID , 2.0));
}
MyAisObject1::MyAisObject1(const TopoDS_Shape& aShape)
{
  m_shape = aShape;
}
void MyAisObject1::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr , const Handle(Prs3d_Presentation)& thePrs , const Standard_Integer theMode)
{
  if (theMode == MyDispMode_Main)
  {
    const double aRadius = 100.0 , aHeight = 100.0;
    StdPrs_ShadedShape::Add(thePrs , m_shape , myDrawer);
    //StdPrs_WFShape::Add (thePrs, aShape, myDrawer);
   /* Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments(4 , 4 * 2 , Graphic3d_ArrayFlags_None);
    aSegs->AddVertex(gp_Pnt(0.0 , -aRadius , 0.0));
    aSegs->AddVertex(gp_Pnt(0.0 , -aRadius , aHeight));
    aSegs->AddVertex(gp_Pnt(0.0 , aRadius , aHeight));
    aSegs->AddVertex(gp_Pnt(0.0 , aRadius , 0.0));
    aSegs->AddEdges(1 , 2);
    aSegs->AddEdges(2 , 3);
    aSegs->AddEdges(3 , 4);
    aSegs->AddEdges(4 , 1);
    Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
    aGroupSegs->SetGroupPrimitivesAspect(myDrawer->WireAspect()->Aspect());
    aGroupSegs->AddPrimitiveArray(aSegs);*/
  }
  else if (theMode == 1)
  {
    Bnd_Box aBox;
    BRepBndLib::Add(m_shape , aBox);
    Prs3d_BndBox::Add(thePrs , aBox , myDrawer);
  }
}

void MyAisObject1::ComputeSelection(const Handle(SelectMgr_Selection)& theSel , const Standard_Integer theMode)
{
  Standard_Real aDefl = StdPrs_ToolTriangulatedShape::GetDeflection(m_shape, myDrawer);
  StdSelect_BRepSelectionTool::Load(theSel, this, m_shape, TopAbs_SHAPE, aDefl,
    myDrawer->DeviationAngle(),
    myDrawer->IsAutoTriangulation());

  /*const double aRadius = 100.0, aHeight = 100.0;
  Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner(this);
  Handle(Graphic3d_ArrayOfTriangles) aTris =
    Prs3d_ToolCylinder::Create(aRadius, aRadius, aHeight, 25, 25, gp_Trsf());
  Handle(Select3D_SensitivePrimitiveArray) aSensTri =
    new Select3D_SensitivePrimitiveArray(anOwner);
  aSensTri->InitTriangulation(aTris->Attributes(), aTris->Indices(),
    TopLoc_Location());
  theSel->Add(aSensTri);*/

}

bool MyAisObject1::AcceptDisplayMode(const Standard_Integer theMode) const
{
  return theMode == MyDispMode_Main || theMode == MyDispMode_Highlight;
}


MyAisObject2::MyAisObject2()
{
  SetHilightMode(MyDispMode_Highlight);
  myDrawer->SetupOwnShadingAspect();
  myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
  myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN , Aspect_TOL_SOLID , 2.0));
}
MyAisObject2::MyAisObject2(const TopoDS_Shape& aShape)
{
  m_shape = aShape;
}
void MyAisObject2::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr , const Handle(Prs3d_Presentation)& thePrs , const Standard_Integer theMode)
{
  if (theMode == MyDispMode_Main)
  {
    const double aRadius = 100.0 , aHeight = 100.0;
    //StdPrs_ShadedShape::Add (thePrs, aShape, myDrawer); // add shading
    //StdPrs_WFShape::Add (thePrs, aShape, myDrawer); // add wireframe
    Handle(Graphic3d_ArrayOfTriangles) aTris =
      Prs3d_ToolCylinder::Create(aRadius , aRadius , aHeight , 25 , 25 , gp_Trsf());
    Handle(Graphic3d_Group) aGroupTris = thePrs->NewGroup();
    aGroupTris->SetGroupPrimitivesAspect(myDrawer->ShadingAspect()->Aspect());
    aGroupTris->AddPrimitiveArray(aTris);
  }
  else if (theMode == 1)
  {
    Bnd_Box aBox;
    BRepBndLib::Add(m_shape , aBox);
    Prs3d_BndBox::Add(thePrs , aBox , myDrawer);
  }
}

void MyAisObject2::ComputeSelection(const Handle(SelectMgr_Selection)& theSel , const Standard_Integer theMode)
{
}

bool MyAisObject2::AcceptDisplayMode(const Standard_Integer theMode) const
{
  return theMode == MyDispMode_Main || theMode == MyDispMode_Highlight;
}


MyAisObject3::MyAisObject3()
{
  SetHilightMode(MyDispMode_Highlight);
  myDrawer->SetupOwnShadingAspect();
  myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
  myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN , Aspect_TOL_SOLID , 2.0));
}
MyAisObject3::MyAisObject3(const TopoDS_Shape& aShape)
{
  SetHilightMode(MyDispMode_Highlight);
  myDrawer->SetupOwnShadingAspect();
  myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
  myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN , Aspect_TOL_SOLID , 2.0));
  m_shape = aShape;
}
void MyAisObject3::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr , const Handle(Prs3d_Presentation)& thePrs , const Standard_Integer theMode)
{
  if (theMode == 0)
  {
    StdPrs_ShadedShape::Add(thePrs , m_shape , myDrawer);
    StdPrs_WFShape::Add(thePrs , m_shape , myDrawer); // add wireframe
  }
  else if (theMode == 1)
  {
    Bnd_Box aBox;
    BRepBndLib::Add(m_shape , aBox);
    Prs3d_BndBox::Add(thePrs , aBox , myDrawer);
  }
}

void MyAisObject3::ComputeSelection(const Handle(SelectMgr_Selection)& theSel , const Standard_Integer theMode)
{
  /*Bnd_Box aBox;
  BRepBndLib::Add(m_shape , aBox);
  Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner(this);
  Handle(Select3D_SensitiveBox) aSensBox = new Select3D_SensitiveBox(anOwner , aBox);
  theSel->Add(aSensBox);*/
  Standard_Real aDefl = StdPrs_ToolTriangulatedShape::GetDeflection(m_shape , myDrawer);
  StdSelect_BRepSelectionTool::Load(theSel , this , m_shape , TopAbs_SHAPE , aDefl ,
                                     myDrawer->DeviationAngle() ,
                                     myDrawer->IsAutoTriangulation());

  const double aRadius = 100.0 , aHeight = 100.0;
  Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner(this);  
  Handle(Graphic3d_ArrayOfTriangles) aTris =
    Prs3d_ToolCylinder::Create(aRadius , aRadius , aHeight , 25 , 25 , gp_Trsf());
  Handle(Select3D_SensitivePrimitiveArray) aSensTri =
    new Select3D_SensitivePrimitiveArray(anOwner);
  aSensTri->InitTriangulation(aTris->Attributes() , aTris->Indices() ,
                               TopLoc_Location());
  theSel->Add(aSensTri);
}

bool MyAisObject3::AcceptDisplayMode(const Standard_Integer theMode) const
{
  return theMode == MyDispMode_Main || theMode == MyDispMode_Highlight;
}

MyAisObject4::MyAisObject4()
{
  myDrawer->SetupOwnShadingAspect();
}
MyAisObject4::MyAisObject4(const TopoDS_Shape& aShape)
{
  myDrawer->SetupOwnShadingAspect();
  myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
  myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN , Aspect_TOL_SOLID , 2.0));
  m_shape = aShape;
}
void MyAisObject4::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr , const Handle(Prs3d_Presentation)& thePrs , const Standard_Integer theMode)
{
  if (theMode == MyDispMode_Main)
  {
    const double aRadius = 100.0 , aHeight = 100.0;
    Prs3d_ToolCylinder aCyl(aRadius , aRadius , aHeight , 25 , 25);
    Prs3d_ToolDisk aDisk(0.0 , aRadius , 25 , 1);

    Handle(Graphic3d_ArrayOfTriangles) aTris =
      new Graphic3d_ArrayOfTriangles(aCyl.VerticesNb() + 2 * aDisk.VerticesNb() ,
                                      3 * (aCyl.TrianglesNb() + 2 * aDisk.TrianglesNb()) ,
                                      Graphic3d_ArrayFlags_VertexNormal);
    aCyl.FillArray(aTris , gp_Trsf());
    aDisk.FillArray(aTris , gp_Trsf());

    gp_Trsf aDisk2Trsf;
    aDisk2Trsf.SetTransformation(gp_Ax3(gp_Pnt(0.0 , 0.0 , aHeight) , -gp::DZ() , gp::DX()) , gp::XOY());
    aDisk.FillArray(aTris , aDisk2Trsf);

    Handle(Graphic3d_Group) aGroupTris = thePrs->NewGroup();
    aGroupTris->SetGroupPrimitivesAspect(myDrawer->ShadingAspect()->Aspect());
    aGroupTris->AddPrimitiveArray(aTris);
    aGroupTris->SetClosed(true);
    //StdPrs_WFShape::Add (thePrs, aShape, myDrawer);
    Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments(4 , 4 * 2 , Graphic3d_ArrayFlags_None);
    aSegs->AddVertex(gp_Pnt(0.0 , -aRadius , 0.0));
    aSegs->AddVertex(gp_Pnt(0.0 , -aRadius , aHeight));
    aSegs->AddVertex(gp_Pnt(0.0 , aRadius , aHeight));
    aSegs->AddVertex(gp_Pnt(0.0 , aRadius , 0.0));
    aSegs->AddEdges(1 , 2);
    aSegs->AddEdges(2 , 3);
    aSegs->AddEdges(3 , 4);
    aSegs->AddEdges(4 , 1);
    Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
    aGroupSegs->SetGroupPrimitivesAspect(myDrawer->WireAspect()->Aspect());
    aGroupSegs->AddPrimitiveArray(aSegs);
  }
  else if (theMode == 1)
  {
    Bnd_Box aBox;
    BRepBndLib::Add(m_shape , aBox);
    Prs3d_BndBox::Add(thePrs , aBox , myDrawer);
  }
}

void MyAisObject4::ComputeSelection(const Handle(SelectMgr_Selection)& theSel , const Standard_Integer theMode)
{
  if (theMode != 0)
  {
    return;
  }
  const double aRadius = 100.0 , aHeight = 100.0;
  Handle(MyAisOwner1) anOwner = new MyAisOwner1(this);
  Handle(Graphic3d_ArrayOfTriangles) aTris =
    Prs3d_ToolCylinder::Create(aRadius , aRadius , aHeight , 10 , 10 , gp_Trsf());
  Handle(Select3D_SensitivePrimitiveArray) aSensTri =
    new Select3D_SensitivePrimitiveArray(anOwner);
  aSensTri->InitTriangulation(aTris->Attributes() , aTris->Indices() ,
                               TopLoc_Location());
  theSel->Add(aSensTri);

}

bool MyAisObject4::AcceptDisplayMode(const Standard_Integer theMode) const
{
  return theMode == MyDispMode_Main || theMode == MyDispMode_Highlight;
}

MyAisObject5::MyAisObject5()
{
  SetDisplayMode(MyDispMode_Main);      // main (active) display mode
  SetHilightMode(MyDispMode_Highlight); // auxiliary (highlighting) mode
}

MyAisObject5::MyAisObject5(const TopoDS_Shape& aShape)
{
  m_shape = aShape;
}

void MyAisObject5::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr , const Handle(Prs3d_Presentation)& thePrs , const Standard_Integer theMode)
{
  if (theMode == MyDispMode_Main)
  {
    StdPrs_ShadedShape::Add(thePrs , m_shape , myDrawer);
    Bnd_OBB  aBox;
    BRepBndLib::AddOBB(m_shape , aBox);
    Prs3d_BndBox::Add(thePrs , aBox , myDrawer);
  }
  else if (theMode == 1)
  {
    //StdPrs_ShadedShape::Add(thePrs , m_shape , myDrawer);
    Bnd_Box aBox;
    BRepBndLib::Add(m_shape , aBox);
    Prs3d_BndBox::Add(thePrs , aBox , myDrawer);
  }
}

void MyAisObject5::ComputeSelection(const Handle(SelectMgr_Selection)& theSel , const Standard_Integer theMode)
{
}

bool MyAisObject5::AcceptDisplayMode(const Standard_Integer theMode) const
{
  return false;
}
