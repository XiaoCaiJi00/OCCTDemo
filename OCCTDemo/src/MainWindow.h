#pragma once

#include "View.h"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QLabel>
#include <QtGui/QActionGroup>
#include <QtWidgets/QToolBar>

#include "myShape.h"

class MainWindow: public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	void initMenuToolbar();
	void initOCCTView();
	void setViewer(const Handle(V3d_Viewer)& theViewer) {
		myViewer = theViewer;
	}
	void initSignals();
	void test();
	void setSelectMode();
	void setWorkArea(int index);
	void runAiscCustom();
	void runModelingData();
	void runTest();

protected:
	virtual bool eventFilter(QObject* watched , QEvent* event) override;

private:
	Handle(V3d_Viewer) Viewer(const V3d_TypeOfOrientation theViewProj);

	void updateStatusBar(const std::vector<double>& aNewPos);

	double _devPx;
	Handle(V3d_Viewer) myViewer;
	Handle(AIS_InteractiveContext) myContext;
	std::vector<double> myClickPos;

	View* view_occt;
	QStatusBar* stb;
	QLabel* lb_positionInfo;

	QToolBar* tb_view;
	QAction* act_hideAll;
	QAction* act_hide;
	QAction* act_fitAll;
	QAction* act_axo;
	QAction* act_top;
	QAction* act_buttom;
	QAction* act_left;
	QAction* act_right;
	QAction* act_front;
	QAction* act_back;
	QAction* act_shade;
	QAction* act_wireframe;
	QAction* act_axis;
	QAction* act_test;

	QToolBar* tb_style;
	QAction* act_color;
	QAction* act_material;
	QActionGroup* actgp_selectMode;
	QAction* act_selectVertex;
	QAction* act_selectEdge;
	QAction* act_selectFace;

	std::vector<QToolBar*> tbs;
	QLabel* lb_workArea;
	QComboBox* cbb_workAreas;

  QToolBar* tb_aisCustom;
  QComboBox* cbb_aisCustom;
	QAction* act_aisCustom;

  QToolBar* tb_modelingData;
  QComboBox* cbb_modelingData;
  QAction* act_modelingData;

  QToolBar* test_bar;
  QComboBox* test_cbb;
  QAction* test_act;



};
