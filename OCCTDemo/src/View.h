#pragma once

#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QApplication>
#include <QtGui/QPainter>

#include "Geom_Axis2Placement.hxx"

#include "OpenGl_GraphicDriver.hxx"
#include "Aspect_DisplayConnection.hxx"
#include "AIS_TrihedronSelectionMode.hxx"
#include "AIS_InteractiveContext.hxx"
#include "AIS_ViewCube.hxx"
#include "AIS_Trihedron.hxx"
#include "AIS_Shape.hxx"
#include "AIS_RubberBand.hxx"
#include "V3d_Viewer.hxx"
#include "V3d_View.hxx"

#include "OcctWindow.h"

enum CurrentAction3d
{
	CurrentAction3d_Nothing,
	CurrentAction3d_DynamicZooming,
	CurrentAction3d_WindowZooming,
	CurrentAction3d_DynamicPanning,
	CurrentAction3d_GlobalPanning,
	CurrentAction3d_DynamicRotation,
	CurrentAction3d_ObjectDececting
};
enum ViewAction
{
	ViewAction_FitAll,
	ViewAction_FitArea,
	ViewAction_Zoom,
	ViewAction_Pan,
	ViewAction_GlobalPan,
	ViewAction_Front,
	ViewAction_Back,
	ViewAction_Top,
	ViewAction_Bottom,
	ViewAction_Left,
	ViewAction_Right,
	ViewAction_Axo,
	ViewAction_Rotation,
	ViewAction_Reset,
	ViewAction_HlrOff,
	ViewAction_HlrOn,
	ViewAction_Shading,
	ViewAction_Wireframe,
	ViewAction_Transparency
};
enum RaytraceAction
{
	RaytraceAction_Raytracing,
	RaytraceAction_Shadows,
	RaytraceAction_Reflections,
	RaytraceAction_Antialiasing
};

class View :public QWidget
{
	Q_OBJECT

signals:
	void selectionChanged();

public:
	View(const Handle(AIS_InteractiveContext)& theContext, bool theIs3dView = true, QWidget* theParent = nullptr);

	virtual void init();
	virtual QPaintEngine* paintEngine() const;

	void removeAll();
	void remove();
	void setProjectionType(Graphic3d_Camera::Projection prj = Graphic3d_Camera::Projection_Orthographic);
	void fitAll();
	void setAxo();
	void setFront();
	void setBack();
	void setLeft();
	void setRight();
	void setTop();
	void setBottom();

	void setShading();
	void setWireframe();
	void setAxis();

	void setHLR(bool theState);
	void setRaytracing(bool theState);
	void SetRaytracedShadows(bool theState);
	void SetRaytracedReflections (bool theState);
	void SetRaytracedAntialiasing (bool theState);

	void setBackgroundColor(QColor aRetColor);
	void setEnvironmentMap(const QString& fileName);
	void setTransparency(double aTranspValue);

	const Handle(V3d_View)& getView() const 
	{ 
		return myV3dView; 
	}
	Handle(AIS_InteractiveContext)& getContext() 
	{
		return myContext;
	}

protected:
	void activateCursor(const CurrentAction3d);
	virtual void paintEvent(QPaintEvent*) override;
	virtual void resizeEvent(QResizeEvent*) override;
	virtual void mousePressEvent(QMouseEvent*) override;
	virtual void mouseReleaseEvent(QMouseEvent*) override;
	virtual void mouseMoveEvent(QMouseEvent*) override;
	virtual void wheelEvent(QWheelEvent*) override;

private:
	void initCursors();

	bool myIs3dView;
	double _devPx;
	Handle(V3d_View) myV3dView;
	Handle(AIS_InteractiveContext)  myContext;
	Handle(AIS_ViewCube)  aViewCube;
	Handle(AIS_RubberBand) myRubberBand;

	CurrentAction3d myCurrentMode;
	Graphic3d_Vec2i myClickPos;

	bool myIsRaytracing;
	bool myIsShadowsEnabled;
	bool myIsReflectionsEnabled;
	bool myIsAntialiasingEnabled;
};
