#include "View.h"

static QCursor* defCursor = nullptr;
static QCursor* handCursor = nullptr;
static QCursor* panCursor = nullptr;
static QCursor* globPanCursor = nullptr;
static QCursor* zoomCursor = nullptr;
static QCursor* rotCursor = nullptr;

View::View(const Handle(AIS_InteractiveContext)& theContext , bool theIs3dView , QWidget* theParent) :
	QWidget(theParent) ,
	myIs3dView(theIs3dView) ,
	_devPx(devicePixelRatio()) ,
	myIsRaytracing(false) ,
	myIsShadowsEnabled(true) ,
	myIsReflectionsEnabled(false) ,
	myIsAntialiasingEnabled(false)
{
	myContext = theContext;
	myCurrentMode = CurrentAction3d_Nothing;

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setBackgroundRole(QPalette::NoRole);
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	init();
	initCursors();
}

void View::init()
{
	if (myV3dView.IsNull())
	{
		myV3dView = myContext->CurrentViewer()->CreateView();
	}

	Handle(OcctWindow) hWnd = new OcctWindow(this);
	// 设置视图窗口
	myV3dView->SetWindow(hWnd);
	if (!hWnd->IsMapped())
	{
		hWnd->Map();
	}

	if (myIs3dView)
	{
		Quantity_Color color1(Quantity_NOC_BLACK);
		Quantity_Color color2(Quantity_NOC_GRAY);
		myV3dView->SetBgGradientColors(color1 , color2 , Aspect_GFM_VER);
	}
	else
	{
		myV3dView->SetBackgroundColor(Quantity_TOC_RGB , 33.0 / 255.0 , 40.0 / 255.0 , 48.0 / 255.0);
		myV3dView->SetProj(V3d_Zpos);
	}

	myV3dView->MustBeResized();

	// 添加viewcube
	aViewCube = new AIS_ViewCube();
	aViewCube->SetBoxTransparency(0.5);
	Handle(Prs3d_Drawer) myDrawer = aViewCube->Attributes();
	Handle(Prs3d_DatumAspect) datumAspect = new Prs3d_DatumAspect();
	datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);
	datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);
	datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE);
	datumAspect->TextAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);
	datumAspect->TextAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);
	datumAspect->TextAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE);
	myDrawer->SetDatumAspect(datumAspect);
	aViewCube->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers ,
		Aspect_TOTP_RIGHT_UPPER ,
		Graphic3d_Vec2i(100 , 100)));
	myContext->Display(aViewCube , false);

	// 添加矩形选择框
	myRubberBand = new AIS_RubberBand();
	myRubberBand->SetLineType(Aspect_TOL_DASH);
	myRubberBand->SetLineColor(Quantity_NOC_WHITE);

	setShading();
}

QPaintEngine* View::paintEngine() const
{
	return nullptr;
}

void View::removeAll()
{
	myContext->RemoveAll(false);
	if (!myContext->IsDisplayed(aViewCube))
	{
		myContext->Display(aViewCube , true);
	}
}

void View::remove()
{
	std::vector<Handle(AIS_InteractiveObject)> selectedObjects;
	myContext->InitSelected();
	while (myContext->MoreSelected())
	{
		Handle(AIS_InteractiveObject) aAis = myContext->SelectedInteractive();
		selectedObjects.push_back(aAis);
		myContext->NextSelected();
	}
	for (auto aAis : selectedObjects)
	{
		myContext->Remove(aAis , false);
	}
	myContext->UpdateCurrentViewer();
}

void View::setProjectionType(Graphic3d_Camera::Projection prj)
{
	myV3dView->Camera()->SetProjectionType(prj);
	myV3dView->Redraw();
}

void View::fitAll()
{
	if (myContext->NbSelected() > 0)
	{
		myContext->FitSelected(myV3dView);
	}
	else
	{
		myV3dView->FitAll();
	}

	myV3dView->ZFitAll();
	myV3dView->Redraw();
}

void View::setAxo()
{
	if (myIs3dView)
	{
		myV3dView->SetProj(V3d_XposYnegZpos);
	}
}

void View::setFront()
{
	myV3dView->SetProj(V3d_Yneg);
}

void View::setBack()
{
	myV3dView->SetProj(V3d_Ypos);
}

void View::setLeft()
{
	myV3dView->SetProj(V3d_Xneg);
}

void View::setRight()
{
	myV3dView->SetProj(V3d_Xpos);
}

void View::setTop()
{
	myV3dView->SetProj(V3d_Zpos);
}

void View::setBottom()
{
	myV3dView->SetProj(V3d_Zneg);
}

void View::setShading()
{
	myContext->InitSelected();
	while (myContext->MoreSelected())
	{
		Handle(AIS_InteractiveObject) aAis = myContext->SelectedInteractive();
		myContext->SetDisplayMode(aAis , 1 , false);
		myContext->NextSelected();
	}

	myContext->UpdateCurrentViewer();
}

void View::setWireframe()
{
	myContext->InitSelected();
	while (myContext->MoreSelected())
	{
		Handle(AIS_InteractiveObject) aAis = myContext->SelectedInteractive();
		myContext->SetDisplayMode(aAis , 0 , false);

		myContext->NextSelected();
	}

	myContext->UpdateCurrentViewer();
}

void View::setAxis()
{
	gp_Ax2 ax2 = gp_Ax2(gp_Pnt(0 , 0 , 0) , gp_Dir(0 , 0 , 100));
	Handle(Geom_Axis2Placement) aGeoAxis2Placement = new Geom_Axis2Placement(ax2);
	Handle(AIS_Trihedron) aisTrihedron = new AIS_Trihedron(aGeoAxis2Placement);

	aisTrihedron->SetDatumPartColor(Prs3d_DP_XArrow , Quantity_NOC_RED2);
	aisTrihedron->SetDatumPartColor(Prs3d_DP_YArrow , Quantity_NOC_GREEN2);
	aisTrihedron->SetDatumPartColor(Prs3d_DP_ZArrow , Quantity_NOC_BLUE2);
	aisTrihedron->SetDatumPartColor(Prs3d_DP_XAxis , Quantity_NOC_RED2);
	aisTrihedron->SetDatumPartColor(Prs3d_DP_YAxis , Quantity_NOC_GREEN2);
	aisTrihedron->SetDatumPartColor(Prs3d_DP_ZAxis , Quantity_NOC_BLUE2);

	//缩放时会有问题
  Handle(Graphic3d_TransformPers) pers =
    new Graphic3d_TransformPers(
      Graphic3d_TMF_ZoomPers ,   // 与视图缩放无关
      gp_Pnt(0 , 0 , 0)           // 参考点
    );

	aisTrihedron->SetTransformPersistence(pers);


	// 显示模式shade在此设置，否则display方法报错
	//aisTrihedron->SetDatumDisplayMode(Prs3d_DM_Shaded);
	aisTrihedron->SetDrawArrows(true);
	myContext->Display(aisTrihedron , 0 , AIS_TrihedronSelectionMode_EntireObject , false);
	fitAll();
}

void View::setHLR(bool theState)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	myV3dView->SetComputedMode(theState);
	myV3dView->Redraw();

	QApplication::restoreOverrideCursor();
}

void View::setRaytracing(bool theState)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if (theState)
	{
		myV3dView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;
	}
	else
	{
		myV3dView->ChangeRenderingParams().Method = Graphic3d_RM_RASTERIZATION;
	}
	myIsRaytracing = theState;
	myContext->UpdateCurrentViewer();
	QApplication::restoreOverrideCursor();
}

void View::SetRaytracedShadows(bool theState)
{
	myV3dView->ChangeRenderingParams().IsShadowEnabled = theState;
	myIsShadowsEnabled = theState;
	myContext->UpdateCurrentViewer();
}

void View::SetRaytracedReflections(bool theState)
{
	myV3dView->ChangeRenderingParams().IsReflectionEnabled = theState;
	myIsReflectionsEnabled = theState;
	myContext->UpdateCurrentViewer();
}

void View::SetRaytracedAntialiasing(bool theState)
{
	myV3dView->ChangeRenderingParams().IsAntialiasingEnabled = theState;
	myIsAntialiasingEnabled = theState;
	myContext->UpdateCurrentViewer();
}

void View::setBackgroundColor(QColor aRetColor)
{
	Standard_Real R1;
	Standard_Real G1;
	Standard_Real B1;
	if (aRetColor.isValid())
	{
		R1 = aRetColor.red() / 255.;
		G1 = aRetColor.green() / 255.;
		B1 = aRetColor.blue() / 255.;
		myV3dView->SetBackgroundColor(Quantity_TOC_RGB , R1 , G1 , B1);
	}
	myV3dView->Redraw();
}

void View::setEnvironmentMap(const QString& fileName)
{
	if (!fileName.isEmpty())
	{
		Handle(Graphic3d_TextureEnv) aTexture = new Graphic3d_TextureEnv(fileName.toStdString().c_str());
		myV3dView->SetTextureEnv(aTexture);
	}
	else
	{
		myV3dView->SetTextureEnv(Handle(Graphic3d_TextureEnv)());
	}

	myV3dView->Redraw();
}

void View::setTransparency(double aTranspValue)
{
	AIS_ListOfInteractive anAisObjectsList;
	myContext->DisplayedObjects(anAisObjectsList);
	if (anAisObjectsList.Extent() == 0)
	{
		return;
	}
	for (AIS_ListOfInteractive::Iterator anIter(anAisObjectsList); anIter.More(); anIter.Next())
	{
		const Handle(AIS_InteractiveObject)& anAisObject = anIter.Value();
		myContext->SetTransparency(anAisObject , aTranspValue , Standard_False);
	}
	myContext->UpdateCurrentViewer();
}

void View::activateCursor(const CurrentAction3d theMode)
{
	QCursor* aCursor = defCursor;
	switch (theMode)
	{
	case CurrentAction3d_DynamicPanning:
		aCursor = panCursor;
		break;
	case CurrentAction3d_DynamicZooming:
		aCursor = zoomCursor;
		break;
	case CurrentAction3d_DynamicRotation:
		aCursor = rotCursor;
		break;
	case CurrentAction3d_GlobalPanning:
		aCursor = globPanCursor;
		break;
	case CurrentAction3d_WindowZooming:
		aCursor = handCursor;
		break;
	case CurrentAction3d_Nothing:
		aCursor = defCursor;
		break;
	default:
		break;
	}
	setCursor(*aCursor);
}

void View::paintEvent(QPaintEvent* event)
{
	myV3dView->Redraw();
}

void View::resizeEvent(QResizeEvent* event)
{
	if (!myV3dView.IsNull())
	{
		myV3dView->MustBeResized();
	}
}

void View::mousePressEvent(QMouseEvent* theEvent)
{
	myClickPos = Graphic3d_Vec2i(_devPx * theEvent->pos().x() , _devPx * theEvent->pos().y());

	if (!myV3dView.IsNull())
	{
		if (theEvent->button() == Qt::MiddleButton)
		{
			myCurrentMode = CurrentAction3d_DynamicPanning;
		}

		else if (theEvent->button() == Qt::RightButton && theEvent->modifiers() & Qt::ShiftModifier)
		{
			if (myIs3dView)
			{
				myCurrentMode = CurrentAction3d_DynamicRotation;
				myV3dView->StartRotation(myClickPos.x() , myClickPos.y());
			}
		}

		activateCursor(myCurrentMode);
		update();
	}
}

void View::mouseReleaseEvent(QMouseEvent* theEvent)
{
	const Graphic3d_Vec2i aNewPos = Graphic3d_Vec2i(_devPx * theEvent->pos().x() , _devPx * theEvent->pos().y());
	if (!myV3dView.IsNull())
	{
		myCurrentMode = CurrentAction3d_Nothing;
		activateCursor(myCurrentMode);
		bool hasRubberBand = myContext->IsDisplayed(myRubberBand);
		if (theEvent->button() == Qt::LeftButton && !hasRubberBand)
		{
			if (theEvent->modifiers() & Qt::ControlModifier)
				myContext->SelectDetected(AIS_SelectionScheme_XOR);
      else if (theEvent->modifiers() & Qt::ShiftModifier)
      {
				myContext->SelectDetected(AIS_SelectionScheme_Add);
			}
			else
			{
				myContext->SelectDetected(); // 此代码没有viewcube无法旋转
			}
		}
		else if (theEvent->button() == Qt::LeftButton && hasRubberBand)
		{
			if (theEvent->modifiers() & Qt::ControlModifier)
			{
				myContext->SelectRectangle(myClickPos , aNewPos , myV3dView , AIS_SelectionScheme_XOR);
			}
			else if (theEvent->modifiers() & Qt::ShiftModifier)
			{
				myContext->SelectRectangle(myClickPos , aNewPos , myV3dView , AIS_SelectionScheme_Add);

			}
			else if (!(theEvent->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier)))
			{
				myContext->SelectRectangle(myClickPos , aNewPos , myV3dView);
			}
			myContext->Remove(myRubberBand , false);
		}

		myClickPos = aNewPos;

		update();
		emit selectionChanged();
	}
}

void View::mouseMoveEvent(QMouseEvent* theEvent)
{
	const Graphic3d_Vec2i aNewPos(_devPx * theEvent->pos().x() , _devPx * theEvent->pos().y());
	if (!myV3dView.IsNull())
	{
		myContext->MoveTo(aNewPos.x() , aNewPos.y() , myV3dView , false);

		if (theEvent->buttons() & Qt::LeftButton)
		{
			myRubberBand->SetRectangle(myClickPos.x() , height() * _devPx - myClickPos.y() ,
				aNewPos.x() , height() * _devPx - aNewPos.y());
			if (myContext->IsDisplayed(myRubberBand))
				myContext->Redisplay(myRubberBand , false);
			else
				myContext->Display(myRubberBand , false);
		}
		else
		{
			switch (myCurrentMode)
			{
			case CurrentAction3d_Nothing:
				break;
			case CurrentAction3d_DynamicZooming:
				myCurrentMode = CurrentAction3d_Nothing;
				break;
			case CurrentAction3d_DynamicPanning:
				myV3dView->Pan(aNewPos.x() - myClickPos.x() , myClickPos.y() - aNewPos.y());
				myClickPos = aNewPos;
				break;
			case CurrentAction3d_DynamicRotation:
				if (myIs3dView)
				{
					myV3dView->Rotation(aNewPos.x() , aNewPos.y());
				}
				break;
			case CurrentAction3d_ObjectDececting:
				break;
			default:
				break;
			}
			activateCursor(myCurrentMode);
		}
		update();
	}
}

void View::wheelEvent(QWheelEvent* theEvent)
{
	myClickPos = Graphic3d_Vec2i(_devPx * theEvent->position().x() , _devPx * theEvent->position().y());
	if (!myV3dView.IsNull())
	{
		myCurrentMode = CurrentAction3d_DynamicZooming;

		myV3dView->StartZoomAtPoint(myClickPos.x() , myClickPos.y());
		myV3dView->ZoomAtPoint(0 , 0 , theEvent->angleDelta().y() / 8 , 0);

		activateCursor(myCurrentMode);
		update();
	}
}

void View::initCursors()
{
	if (!defCursor)
		defCursor = new QCursor(Qt::ArrowCursor);
	if (!handCursor)
		handCursor = new QCursor(Qt::PointingHandCursor);
	if (!panCursor)
		panCursor = new QCursor(Qt::SizeAllCursor);
	if (!globPanCursor)
		globPanCursor = new QCursor(Qt::CrossCursor);
	if (!zoomCursor)
		zoomCursor = new QCursor(QPixmap(":/icons/icons/cursor_zoom.png"));
	if (!rotCursor)
		rotCursor = new QCursor(QPixmap(":/icons/icons/cursor_rotate.png"));
}
