#include "MainWindow.h"
#include "myAISObj.h"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "GeomAPI_Interpolate.hxx"
#include "BrepBuilderAPI_MakeEdge.hxx"
#include "TopoDS_Edge.hxx"
#include "GeomAPI_PointsToBSpline.hxx"
#include "gce_MakeCirc.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "Geom2d_Line.hxx"
#include "GCE2d_MakeLine.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "BRepLib.hxx"
#include "BRepOffsetAPI_MakePipe.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "GProp_GProps.hxx"
#include "BRepGProp.hxx"


static Handle(Geom_BSplineCurve) createBSpline(const std::vector<gp_Pnt>& pts)
{
  Handle(TColgp_HArray1OfPnt) ps = new TColgp_HArray1OfPnt(1 , pts.size());
  for (int i = 0; i < pts.size(); ++i)
  {
    ps->SetValue(i + 1 , pts[i]);
  }
  //GeomAPI_Interpolate interp(ps , Standard_False , Precision::Confusion());
  GeomAPI_PointsToBSpline interp(ps->Array1());
  //interp.Perform();
  Handle(Geom_BSplineCurve) C = interp.Curve();
	return C;
}
MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent) ,
	_devPx(devicePixelRatio())
{
	initMenuToolbar();
	initOCCTView();
	initSignals();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initMenuToolbar()
{
	setWindowTitle("OCCTTutorial");
	setWindowIcon(QIcon(":/icons/icons/3dModel.png"));

	tb_view = new QToolBar(this);
	//tb_view->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	addToolBar(tb_view);

	act_hideAll = new QAction(QIcon(":/icons/icons/hideAll.png") , "hideAll" , this);
	act_hide = new QAction(QIcon(":/icons/icons/hide.png") , "hide" , this);
	act_fitAll = new QAction(QIcon(":/icons/icons/view-fullscreen.svg") , "fitAll" , this);
	act_axo = new QAction(QIcon(":/icons/icons/view-axonometric.svg") , "axo" , this);
	act_top = new QAction(QIcon(":/icons/icons/view-top.svg") , "top" , this);
	act_buttom = new QAction(QIcon(":/icons/icons/view-bottom.svg") , "buttom" , this);
	act_left = new QAction(QIcon(":/icons/icons/view-left.svg") , "left" , this);
	act_right = new QAction(QIcon(":/icons/icons/view-right.svg") , "right" , this);
	act_front = new QAction(QIcon(":/icons/icons/view-front.svg") , "front" , this);
	act_back = new QAction(QIcon(":/icons/icons/view-rear.svg") , "back" , this);
	act_shade = new QAction(QIcon(":/icons/icons/DrawStyleShaded.svg") , "shade" , this);
	act_wireframe = new QAction(QIcon(":/icons/icons/DrawStyleWireFrame.svg") , "wireframe" , this);
	act_axis = new QAction(QIcon(":/icons/icons/axis.png") , "axis" , this);
	act_test = new QAction(QIcon(":/icons/icons/utilities-terminal.svg") , "test" , this);

	tb_view->addActions({ act_hideAll,act_hide,act_fitAll,act_axo,
	act_top,act_buttom,act_left,act_right,act_front,act_back });
	tb_view->addSeparator();
	tb_view->addActions({ act_shade,act_wireframe,act_axis,act_test });

	tb_style = new QToolBar(this);
	addToolBar(tb_style);
	addToolBarBreak();

	act_color = new QAction(QIcon(":/icons/icons/colors.svg") , "color" , this);
	act_material = new QAction(QIcon(":/icons/icons/Material.svg") , "material" , this);

	actgp_selectMode = new QActionGroup(this);
	actgp_selectMode->setExclusionPolicy(QActionGroup::ExclusionPolicy::ExclusiveOptional);
	act_selectVertex = new QAction(QIcon(":/icons/icons/vertex-selection.svg") , "selectVertex" , this);
	act_selectVertex->setCheckable(true);
	act_selectEdge = new QAction(QIcon(":/icons/icons/edge-selection.svg") , "selectEdge" , this);
	act_selectEdge->setCheckable(true);
	act_selectFace = new QAction(QIcon(":/icons/icons/face-selection.svg") , "selectFace" , this);
	act_selectFace->setCheckable(true);

  lb_workArea = new QLabel(this);
  lb_workArea->setText("workArea: ");
  cbb_workAreas = new QComboBox(this);
  cbb_workAreas->addItems({ "None", "AISCustomPresentation", "Interpolations and Approximations", "test"});

	//AisObject
  tb_aisCustom = new QToolBar(this);
	tb_aisCustom->setVisible(false);
	addToolBar(tb_aisCustom);
	tbs.push_back(tb_aisCustom);
	cbb_aisCustom = new QComboBox(this);
	cbb_aisCustom->addItems({ "Presentation builders", "Primitive arrays", "Quadric builders", "Computing selection", "Highlighting selection owner"});
	tb_aisCustom->addWidget(cbb_aisCustom);
	act_aisCustom = new QAction(QIcon(":/icons/icons/run.png") , "run" , this);
	tb_aisCustom->addAction(act_aisCustom);

	//Modeling Data
  tb_modelingData = new QToolBar(this);
	tb_modelingData->setVisible(false);
  addToolBar(tb_modelingData);
  tbs.push_back(tb_modelingData);
  cbb_modelingData = new QComboBox(this);
	cbb_modelingData->addItems({ "Interpolations and Approximations", "Direct Construction", "2D Geometry", "Points on Curves", "Properties", "Bounding Box"});
	tb_modelingData->addWidget(cbb_modelingData);
  act_modelingData = new QAction(QIcon(":/icons/icons/run.png") , "run" , this);
	tb_modelingData->addAction(act_modelingData);

	//test my 
  test_bar = new QToolBar(this);
	test_bar->setVisible(false);
  addToolBar(test_bar);
  tbs.push_back(test_bar);
  test_cbb = new QComboBox(this);
	test_cbb->addItems({ "test1" });
  test_bar->addWidget(test_cbb);
  test_act = new QAction(QIcon(":/icons/icons/run.png"), "run", this);
	test_bar->addAction(test_act);


	tb_style->addActions({ act_color,act_material});
	tb_style->addSeparator();
	tb_style->addActions({ act_selectVertex, act_selectEdge, act_selectFace });
	tb_style->addSeparator();
	tb_style->addWidget(lb_workArea);
	tb_style->addWidget(cbb_workAreas);

	stb = new QStatusBar(this);
	this->setStatusBar(stb);
	lb_positionInfo = new QLabel(this);
	stb->addWidget(lb_positionInfo);
}

void MainWindow::initOCCTView()
{
	myViewer = Viewer(V3d_XposYnegZpos);
	myContext = new AIS_InteractiveContext(myViewer);
	view_occt = new View(myContext , this);
	view_occt->installEventFilter(this);
	setCentralWidget(view_occt);
}

void MainWindow::initSignals()
{
	connect(act_hideAll , &QAction::triggered , view_occt , &View::removeAll);
	connect(act_hide , &QAction::triggered , view_occt , &View::remove);
	connect(act_fitAll , &QAction::triggered , view_occt , &View::fitAll);
	connect(act_axo , &QAction::triggered , view_occt , &View::setAxo);
	connect(act_top , &QAction::triggered , view_occt , &View::setTop);
	connect(act_buttom , &QAction::triggered , view_occt , &View::setBottom);
	connect(act_left , &QAction::triggered , view_occt , &View::setLeft);
	connect(act_right , &QAction::triggered , view_occt , &View::setRight);
	connect(act_front , &QAction::triggered , view_occt , &View::setFront);
	connect(act_back , &QAction::triggered , view_occt , &View::setBack);

	connect(act_shade , &QAction::triggered , view_occt , &View::setShading);
	connect(act_wireframe , &QAction::triggered , view_occt , &View::setWireframe);
	connect(act_axis , &QAction::triggered , view_occt , &View::setAxis);
	connect(act_test , &QAction::triggered , this , &MainWindow::test);

	connect(act_selectVertex , &QAction::triggered , this , &MainWindow::setSelectMode);
	connect(act_selectEdge , &QAction::triggered , this , &MainWindow::setSelectMode);
	connect(act_selectFace , &QAction::triggered , this , &MainWindow::setSelectMode);
	connect(cbb_workAreas , &QComboBox::currentIndexChanged, this , &MainWindow::setWorkArea);
	connect(act_aisCustom , &QAction::triggered, this , &MainWindow::runAiscCustom);
	connect(act_modelingData , &QAction::triggered, this , &MainWindow::runModelingData);
	connect(test_act , &QAction::triggered, this , &MainWindow::runTest);
}

void MainWindow::test()
{
	TopoDS_Shape aShape = BRepPrimAPI_MakeBox(100 , 200 , 400);
	Handle(AIS_InteractiveObject) ais = new AIS_Shape(aShape);
	//Handle(AIS_InteractiveObject) ais = new AIS_DemoPrs;
	myContext->Display(ais , 1 , 0 , false);

	view_occt->fitAll(); 
}

void MainWindow::setSelectMode()
{
	bool checkVertex = act_selectVertex->isChecked();
	bool checkEdge = act_selectEdge->isChecked();
	bool checkFace = act_selectFace->isChecked();

	myContext->Deactivate();
	if (checkVertex && !checkEdge && !checkFace)
	{
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
	}
	else if (!checkVertex && checkEdge && !checkFace)
	{
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
	}
	else if (!checkVertex && !checkEdge && checkFace)
	{
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
	}
	else if (checkVertex && checkEdge && !checkFace)
	{
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
	}
	else if (checkVertex && !checkEdge && checkFace)
	{
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
	}
	else if (!checkVertex && checkEdge && checkFace)
	{
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
	}
	else if (checkVertex && checkEdge && checkFace)
	{
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
		myContext->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
	}
	else
	{
		myContext->Activate(0);
	}
}

void MainWindow::setWorkArea(int index)
{
	if (index <= 0)
	{
		for (auto tb : tbs)
		{
			tb->setVisible(false);
		}
		return;
	}
	for (int i = 0; i < tbs.size(); ++i)
	{
		if (i == index - 1)
		{
			tbs[i]->setVisible(true);
		}
		else
		{
			tbs[i]->setVisible(false);
		}

	}
}

void MainWindow::runAiscCustom()
{
	if (cbb_aisCustom->currentText() == "Presentation builders")
	{
		TopoDS_Shape aShape = BRepPrimAPI_MakeCylinder(100.0 , 100.0);
		Handle(MyAisObject0) aPrs = new MyAisObject0(aShape);
		myContext->Display(aPrs , MyAisObject0::MyDispMode_Main , 0 , false);
		myContext->HilightWithColor(aPrs , aPrs->HilightAttributes() , false);
	}
	else if (cbb_aisCustom->currentText() == "Primitive arrays")
	{
    const double aRadius = 100.0 , aHeight = 100.0;
    TopoDS_Shape aShape = BRepPrimAPI_MakeCylinder(aRadius , aHeight);
    Handle(MyAisObject1) aPrs = new MyAisObject1(aShape);
    myContext->Display(aPrs , MyAisObject1::MyDispMode_Main , 0 , false);
	}
	else if (cbb_aisCustom->currentText() == "Quadric builders")
	{
    const double aRadius = 100.0 , aHeight = 100.0;
    TopoDS_Shape aShape = BRepPrimAPI_MakeCylinder(aRadius , aHeight);
    Handle(MyAisObject2) aPrs = new MyAisObject2(aShape);
    myContext->Display(aPrs , MyAisObject2::MyDispMode_Main , 0 , false);
	}
  else if (cbb_aisCustom->currentText() == "Computing selection")
  {
    const double aRadius = 100.0 , aHeight = 100.0;
    TopoDS_Shape aShape = BRepPrimAPI_MakeCylinder(aRadius , aHeight);
    Handle(MyAisObject3) aPrs = new MyAisObject3(aShape);
    myContext->Display(aPrs , MyAisObject3::MyDispMode_Main , 0 , false);
  }
  else if (cbb_aisCustom->currentText() == "Highlighting selection owner")
  {
    const double aRadius = 100.0 , aHeight = 100.0;
    TopoDS_Shape aShape = BRepPrimAPI_MakeCylinder(aRadius , aHeight);
    Handle(MyAisObject4) aPrs = new MyAisObject4(aShape);
    myContext->Display(aPrs , MyAisObject4::MyDispMode_Main , 0 , false);
  }

  view_occt->fitAll();
}

void MainWindow::runModelingData()
{
	if (cbb_modelingData->currentText() == "Interpolations and Approximations")
	{
		std::vector<gp_Pnt> pts = { gp_Pnt(0, 0, 0), gp_Pnt(0, 100, 0),  gp_Pnt(50, 50, 0), gp_Pnt(100, 100, 0) };
		Handle(TColgp_HArray1OfPnt) ps = new TColgp_HArray1OfPnt(1 , 4);
		for (int i = 0; i < pts.size(); ++i)
		{
			TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(pts[i]);
			Handle(AIS_InteractiveObject) aisVertex = new AIS_Shape(aVertex);
			myContext->Display(aisVertex , false);
			ps->SetValue(i + 1 , pts[i]);
		}
    GeomAPI_Interpolate interp(ps , Standard_False , Precision::Confusion());
    interp.Perform();
    Handle(Geom_BSplineCurve) C = interp.Curve();
    TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(C);
    Handle(AIS_InteractiveObject) aisEdge = new AIS_Shape(aEdge);
    myContext->Display(aisEdge , false);

    auto ptArr = ps->Array1();
    GeomAPI_PointsToBSpline Approx(ptArr);
    Handle(Geom_BSplineCurve) cApprox = Approx.Curve();
    cApprox->Translate(gp_Vec(0 , 0 , 100));
    TopoDS_Edge aApproxEdge = BRepBuilderAPI_MakeEdge(cApprox);
    Handle(AIS_InteractiveObject) aisApproxEdge = new AIS_Shape(aApproxEdge);
    myContext->Display(aisApproxEdge , false);

	}
	else if (cbb_modelingData->currentText() == "Direct Construction")
	{
    gp_Pnt P1(0. , 0. , 0.);
    gp_Pnt P2(0. , 10. , 0.);
    gp_Pnt P3(10. , 0. , 0.);
    gce_MakeCirc MC(P1 , P2 , P3);
    if (MC.IsDone())
    {
      const gp_Circ& C = MC.Value();
      TopoDS_Edge aCircle = BRepBuilderAPI_MakeEdge(C);
      Handle(AIS_InteractiveObject) aisCircle = new AIS_Shape(aCircle);
			myContext->Display(aisCircle , false);
    }
	}
	else if (cbb_modelingData->currentText() == "2D Geometry")
	{
		gp_Pnt start(0 , 0 , 0);
		gp_Pnt end(100 , 100 , 100);
		TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(start , end);
		Handle(AIS_InteractiveObject) aisEdge = new AIS_Shape(aEdge);
		myContext->Display(aisEdge , false);

		gp_Vec vec(start , end);

		gp_Vec vecX = vec.Crossed(gp_Vec(0 , 0 , 1));
		gp_Ax3 ax3(start , vec , vecX);
		Handle(Geom_Surface) surf = new Geom_Plane(ax3);
		TopoDS_Face aFace = BRepBuilderAPI_MakeFace(surf , -200 , 200 , -100 , 100 , 0);
		Handle(AIS_InteractiveObject) aisFace = new MyAisObject0(aFace);
		aisFace->SetTransparency(0.5); 
		myContext->Display(aisFace , false);

		double width = 100.0;
		double height = 50.0;

		gp_Pnt2d pnt1(width / 2.0 , height / 2.0);
		gp_Pnt2d pnt2(width / 2.0 , -height / 2.0);
		gp_Pnt2d pnt3(-width / 2.0 , -height / 2.0);
		gp_Pnt2d pnt4(-width / 2.0 , height / 2.0);
		Handle(Geom2d_Line) geom2dLine1 = GCE2d_MakeLine(pnt1 , pnt2).Value();
		Handle(Geom2d_Line) geom2dLine2 = GCE2d_MakeLine(pnt2 , pnt3).Value();
		Handle(Geom2d_Line) geom2dLine3 = GCE2d_MakeLine(pnt3 , pnt4).Value();
		Handle(Geom2d_Line) geom2dLine4 = GCE2d_MakeLine(pnt4 , pnt1).Value();

		BRepBuilderAPI_MakeWire mw;
		mw.Add(BRepBuilderAPI_MakeEdge(geom2dLine1 , surf , 0 , height));
		mw.Add(BRepBuilderAPI_MakeEdge(geom2dLine2 , surf , 0 , width));
		mw.Add(BRepBuilderAPI_MakeEdge(geom2dLine3 , surf , 0 , height));
		mw.Add(BRepBuilderAPI_MakeEdge(geom2dLine4 , surf , 0 , width));

		TopoDS_Wire profile = mw;
		BRepLib::BuildCurves3d(profile);
		Handle(AIS_InteractiveObject) aisProfile = new AIS_Shape(profile);
		myContext->Display(aisProfile , false);

    TopoDS_Wire path = BRepBuilderAPI_MakeWire(aEdge);
    TopoDS_Shape myShape = BRepOffsetAPI_MakePipe(path , profile);

    Handle(AIS_InteractiveObject) aisPipe = new AIS_Shape(myShape);
    myContext->Display(aisPipe , false);
		//gp_Pnt start(0 , 0 , 0);
		//gp_Pnt end(100 , 100 , 100);

		//TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(start , end);
		//TopoDS_Wire path = BRepBuilderAPI_MakeWire(aEdge);

		//Handle(AIS_InteractiveObject) aisEdge = new AIS_Shape(aEdge);
		//myContext->Display(aisEdge , false);

		///* =========================================================
		// * 2. 计算 path 起点切向
		// * ========================================================= */
		//gp_Dir pathDir(gp_Vec(start , end));

		///* =========================================================
		// * 3. 构造稳定的局部坐标系（关键）
		// * ========================================================= */
		//gp_Dir refDir = pathDir.IsParallel(gp::DZ() , Precision::Angular())
		//	? gp::DX()
		//	: gp::DZ();

		//gp_Dir xDir(pathDir.Crossed(refDir));
		//gp_Dir yDir = pathDir ^ xDir;

		//gp_Ax3 ax3(start , pathDir , xDir);

		///* =========================================================
		// * 4. 构造 profile（严格位于 path 起点法向平面）
		// * ========================================================= */
		//double width = 100.0;
		//double height = 50.0;

		//gp_Pnt p1 = start.Translated(0.5 * width * gp_Vec(xDir)
		//														+ 0.5 * height * gp_Vec(yDir));
		//gp_Pnt p2 = start.Translated(0.5 * width * gp_Vec(xDir)
		//														- 0.5 * height * gp_Vec(yDir));
		//gp_Pnt p3 = start.Translated(-0.5 * width * gp_Vec(xDir)
		//														- 0.5 * height * gp_Vec(yDir));
		//gp_Pnt p4 = start.Translated(-0.5 * width * gp_Vec(xDir)
		//														+ 0.5 * height * gp_Vec(yDir));

		//BRepBuilderAPI_MakeWire mw;
		//mw.Add(BRepBuilderAPI_MakeEdge(p1 , p2));
		//mw.Add(BRepBuilderAPI_MakeEdge(p2 , p3));
		//mw.Add(BRepBuilderAPI_MakeEdge(p3 , p4));
		//mw.Add(BRepBuilderAPI_MakeEdge(p4 , p1));

		//TopoDS_Wire profile = mw.Wire();

		//Handle(AIS_InteractiveObject) aisProfile = new AIS_Shape(profile);
		//myContext->Display(aisProfile , false);

		///* =========================================================
		// * 5. Pipe（严格使用你给的接口）
		// * ========================================================= */
		//TopoDS_Shape myShape = BRepOffsetAPI_MakePipe(path , profile);

		///* =========================================================
		// * 6. 基本有效性检查（强烈建议）
		// * ========================================================= */
	
		//Handle(AIS_InteractiveObject) aisPipe = new AIS_Shape(myShape);
		//myContext->Display(aisPipe , false);



	}
  else if (cbb_modelingData->currentText() == "Points on Curves")
  {
		std::vector<gp_Pnt> pnts = { gp_Pnt(0, 0, 0), gp_Pnt(10, 100, 0),  gp_Pnt(50, 50, 0), gp_Pnt(100, 100, 0) };
		for (const auto& p : pnts)
		{
			TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(p);
			Handle(AIS_InteractiveObject) aisVertex = new AIS_Shape(aVertex);
			myContext->Display(aisVertex , false);
    }
		Handle(Geom_BSplineCurve) c = createBSpline(pnts);
    TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(c);
		Handle(AIS_InteractiveObject) aisEdge = new AIS_Shape(aEdge);
    myContext->Display(aisEdge , false);

		Standard_Real startParam =c->FirstParameter();
		Standard_Real endParam =c->LastParameter();
		std::cout << startParam << " " << endParam << std::endl;

    GeomAPI_ProjectPointOnCurve ptOnCurve(gp_Pnt(50 , 50 , 50) , c);
    Standard_Real dist = ptOnCurve.LowerDistance();
    std::cout << "dist: " << dist << std::endl;

  }
	else if (cbb_modelingData->currentText() == "Properties")
	{
		TopoDS_Shape aEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(0 , 0 , 400) , gp_Pnt(0 , 0 , 500));
		Handle(AIS_InteractiveObject) aisEdge = new AIS_Shape(aEdge);
    myContext->Display(aisEdge , false);

		TopoDS_Shape aBox = BRepPrimAPI_MakeBox(100 , 200 , 300);
		Handle(AIS_InteractiveObject) aisBox = new MyAisObject0(aBox);
		myContext->Display(aisBox, 0, 0, false);

		GProp_GProps props;
		BRepGProp::LinearProperties(aBox , props);
		double mass = props.Mass();
		std::cout << "edge mass: " << mass << std::endl;

		gp_Pnt massCenter = props.CentreOfMass();
		std::cout << "edge massCenter: (" << massCenter.X() << " ," << massCenter.Y() << " , " << massCenter.Z() << ")" << std::endl;

		gp_Circ cir(gp_Ax2(gp_Pnt(0 , 600 , 0) , gp_Dir(0 , 0 , 1)) , 100);
		TopoDS_Edge aCir = BRepBuilderAPI_MakeEdge(cir , 0 , M_PI / 2);
		Handle(AIS_InteractiveObject) aisCir = new MyAisObject0(aCir);
		myContext->Display(aisCir , 0 , 0 , false);

    GProp_GProps propsCir;
    BRepGProp::LinearProperties(aCir , propsCir);
    double massCir = propsCir.Mass();
    std::cout << "propsCir mass: " << massCir << std::endl;

    gp_Pnt massCirCenter = propsCir.CentreOfMass();
    std::cout << "massCir massCenter: (" << massCirCenter.X() << " ," << massCirCenter.Y() << " , " << massCirCenter.Z() << ")" << std::endl;

		TopoDS_Vertex aVert = BRepBuilderAPI_MakeVertex(massCirCenter);
		Handle(AIS_InteractiveObject) aisVert = new AIS_Shape(aVert);
		myContext->Display(aisVert , 0 , 0 , false);

	}
	else if (cbb_modelingData->currentText() == "Bounding Box")
	{
    TopoDS_Shape aBox = BRepPrimAPI_MakeBox(100 , 200 , 300);
    Handle(AIS_InteractiveObject) aisBox = new MyAisObject5(aBox);
    myContext->Display(aisBox , 0 , 0 , false);

    gp_Circ cir(gp_Ax2(gp_Pnt(0 , 600 , 0) , gp_Dir(0 , 0 , 1)) , 100);
    TopoDS_Edge aCir = BRepBuilderAPI_MakeEdge(cir , 0 , M_PI / 2);
    Handle(AIS_InteractiveObject) aisCir = new MyAisObject5(aCir);
    myContext->Display(aisCir , 1 , 0 , false);

	}
	view_occt->fitAll();
}

void MainWindow::runTest()
{


}

bool MainWindow::eventFilter(QObject* watched , QEvent* event)
{
	if (event->type() == QEvent::MouseMove)
	{
		QMouseEvent* theEvent = static_cast<QMouseEvent*>(event);
		Qt::MouseButtons aMouseButtons = theEvent->buttons();
		myClickPos = { _devPx * theEvent->pos().x() , _devPx * theEvent->pos().y() };
		updateStatusBar(myClickPos);
	}

	return QMainWindow::eventFilter(watched , event);
}

Handle(V3d_Viewer) MainWindow::Viewer(const V3d_TypeOfOrientation theViewProj)
{
	static Handle(OpenGl_GraphicDriver) aGraphicDriver;
	Handle(Aspect_DisplayConnection) aDisplayConnection;
	aGraphicDriver = new OpenGl_GraphicDriver(aDisplayConnection);
	Handle(V3d_Viewer) aViewer = new V3d_Viewer(aGraphicDriver);
	aViewer->SetDefaultViewProj(theViewProj);

	aViewer->SetDefaultLights();
	aViewer->SetLightOn();
	return aViewer;
}

void MainWindow::updateStatusBar(const std::vector<double>& aNewPos)
{
	double XScreen = aNewPos[0];
	double YScreen = aNewPos[1];

	double x , y , z;
	view_occt->getView()->Convert(XScreen , YScreen , x , y , z);
	gp_Pnt worldPnt = gp_Pnt(x , y , z);

  double xEye , yEye , zEye , xAt , yAt , zAt;
  //view_occt->getView()->Eye(xEye , yEye , zEye);
  view_occt->getView()->At(xAt , yAt , zAt);
  //gp_Pnt eyePoint(xEye , yEye , zEye);
  gp_Pnt atPoint(xAt , yAt , zAt);

  //gp_Dir eyeDir(gp_Vec(eyePoint , atPoint));
  Handle(Graphic3d_Camera) cam = view_occt->getView()->Camera(); 
  gp_Dir eyeDir = cam->Direction();   // 视线方向（从 Eye 指向 At）
	gp_Pln plnAt = gp_Pln(atPoint , eyeDir);

	Handle(Geom_Plane) geoPlane = new Geom_Plane(plnAt);
	GeomAPI_ProjectPointOnSurf project(worldPnt , geoPlane);
	gp_Pnt position = project.NearestPoint();


	// 同上
	//gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(plnAt , worldPnt);
	//gp_Pnt position1 = ElSLib::Value(ConvertedPointOnPlane.X() , ConvertedPointOnPlane.Y() , plnAt);
	//QString positionInfo1 = QString("Position: %0, %1, %2").arg(position1.X()).arg(position1.Y()).arg(position1.Z());

	//QString screenPntInfo = QString("Screen: %0, %1").arg(XScreen).arg(YScreen);
	//QString worldPntInfo = QString("World: %0, %1, %2").arg(x).arg(y).arg(z);
	//QString eyePntInfo = QString("Eye: %0, %1, %2").arg(xEye).arg(yEye).arg(zEye);
	//QString atPntInfo = QString("At: %0, %1, %2").arg(xAt).arg(yAt).arg(zAt);
	QString positionInfo = QString("WCS: %0, %1, %2").arg(position.X()).arg(position.Y()).arg(position.Z());
	//QString info = screenPntInfo + "  "  + positionInfo;
	lb_positionInfo->setText(positionInfo);
}
