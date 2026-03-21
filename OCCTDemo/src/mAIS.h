#include <AIS_InteractiveObject.hxx>
#include <Prs3d_Presentation.hxx>
#include <Prs3d_Point.hxx>
#include <Prs3d_Text.hxx>
#include <Prs3d_Arrow.hxx>
#include <StdPrs_WFShape.hxx>
#include <StdPrs_ShadedShape.hxx>
#include <DsgPrs_LengthPresentation.hxx>

#include <SelectMgr_Selection.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <Select3D_SensitivePoint.hxx>
#include <StdSelect_BRepSelectionTool.hxx>

#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <gp_Pnt.hxx>
#include <TCollection_ExtendedString.hxx>

class AIS_DemoPrs : public AIS_InteractiveObject
{
  DEFINE_STANDARD_RTTIEXT(AIS_DemoPrs , AIS_InteractiveObject)
public:
  AIS_DemoPrs()
  {
    // 简单的 B-Rep box
    myShape = BRepPrimAPI_MakeBox(50.0 , 60.0 , 40.0).Shape();

    // 初始化 Drawer（点、文字、箭头的风格）
    Handle(Prs3d_Drawer) drawer = Attributes();
    drawer->SetPointAspect(new Prs3d_PointAspect(Aspect_TOM_PLUS , Quantity_NOC_RED , 3.0));
    drawer->SetTextAspect(new Prs3d_TextAspect());
    drawer->SetArrowAspect(new Prs3d_ArrowAspect());
  }

protected:
  // 显示
  virtual void Compute(const Handle(PrsMgr_PresentationManager3d)& ,
                       const Handle(Prs3d_Presentation)& aPrs ,
                       const Standard_Integer theMode) override
  {
    // 1. 原点：点 + 文字 + 箭头
    gp_Pnt aPoint(0 , 0 , 0);
    Prs3d_Point::Add(aPrs , aPoint , myDrawer);
    Prs3d_Text::Draw(aPrs , myDrawer ,
                     TCollection_ExtendedString("Origin") , aPoint);
    Prs3d_Arrow::Draw(aPrs , aPoint , gp_Dir(1 , 0 , 0) , M_PI / 12 , 20.0);

    // 2. Box 的线框 & 着色
    StdPrs_WFShape::Add(aPrs , myShape , myDrawer);
    StdPrs_ShadedShape::Add(aPrs , myShape , myDrawer);

    // 3. 标注线
    gp_Pnt p1(0 , 0 , 0) , p2(50 , 0 , 0);
    gp_Dir dir(0 , 0 , 1);  
    DsgPrs_LengthPresentation::Add(aPrs , myDrawer , p1 , p2 , dir ,
                                   TCollection_ExtendedString("50.0"));
  }

  // 选择
  virtual void ComputeSelection(const Handle(SelectMgr_Selection)& aSelection ,
                                const Standard_Integer theMode) override
  {
    if (theMode == 0) // 默认模式
    {
      // 1. Box 的选择：让 StdSelect 自动生成面/边的 sensitive entities
      StdSelect_BRepSelectionTool::Load(aSelection , myShape , TopAbs_SHAPE , 0 , 0.01);

      // 2. 原点的选择：加一个点
      Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner(this , 1);
      gp_Pnt aPoint(0 , 0 , 0);
      Handle(Select3D_SensitivePoint) aSens = new Select3D_SensitivePoint(anOwner , aPoint);
      aSelection->Add(aSens);
    }
  }

private:
  TopoDS_Shape myShape;
};
