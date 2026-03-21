#include "Occtwindow.h"

OcctWindow::OcctWindow(QWidget *parent, const Quantity_NameOfColor theBackColor) :
    Aspect_Window(),
    _devPx(parent->devicePixelRatio()),
    _myWidget(parent)
{
    SetBackground(theBackColor);
    _xLeft = _devPx * _myWidget->rect().left();
    _yTop = _devPx * _myWidget->rect().top();
    _xRight = _devPx * _myWidget->rect().right();
    _yBottom = _devPx * _myWidget->rect().bottom();
}


Aspect_Drawable OcctWindow::NativeParentHandle() const
{
    auto parentWidget = _myWidget->parentWidget();
    if (parentWidget)
      return (Aspect_Drawable)parentWidget->winId();
    else
      return 0;
}


Aspect_Drawable OcctWindow::NativeHandle() const
{
    return (Aspect_Drawable)_myWidget->winId();
}


Standard_Boolean OcctWindow::IsMapped() const
{
    return !( _myWidget->isMinimized() || _myWidget->isHidden() );
}


void OcctWindow::Map() const
{
    _myWidget->show();
    _myWidget->update();
}


void OcctWindow::Unmap() const
{
    _myWidget->hide();
    _myWidget->update();
}


Aspect_TypeOfResize OcctWindow::DoResize()
{
    int aMask {0};
    auto aMode = Aspect_TOR_UNKNOWN;

    if (!_myWidget->isMinimized()) {
        if ( Abs(_devPx * _myWidget->rect().left()   - _xLeft   ) > 2 ) aMask |= 1;
        if ( Abs(_devPx * _myWidget->rect().right()  - _xRight  ) > 2 ) aMask |= 2;
        if ( Abs(_devPx * _myWidget->rect().top()    - _yTop    ) > 2 ) aMask |= 4;
        if ( Abs(_devPx * _myWidget->rect().bottom() - _yBottom ) > 2 ) aMask |= 8;

        switch ( aMask )
        {
          case 0:
            aMode = Aspect_TOR_NO_BORDER;
            break;
          case 1:
            aMode = Aspect_TOR_LEFT_BORDER;
            break;
          case 2:
            aMode = Aspect_TOR_RIGHT_BORDER;
            break;
          case 4:
            aMode = Aspect_TOR_TOP_BORDER;
            break;
          case 5:
            aMode = Aspect_TOR_LEFT_AND_TOP_BORDER;
            break;
          case 6:
            aMode = Aspect_TOR_TOP_AND_RIGHT_BORDER;
            break;
          case 8:
            aMode = Aspect_TOR_BOTTOM_BORDER;
            break;
          case 9:
            aMode = Aspect_TOR_BOTTOM_AND_LEFT_BORDER;
            break;
          case 10:
            aMode = Aspect_TOR_RIGHT_AND_BOTTOM_BORDER;
            break;
          default:
            break;
        }  // end switch

        _xLeft  = static_cast<Standard_Integer>(_devPx * _myWidget->rect().left());
        _xRight = static_cast<Standard_Integer>(_devPx * _myWidget->rect().right());
        _yTop   = static_cast<Standard_Integer>(_devPx * _myWidget->rect().top());
        _yBottom = static_cast<Standard_Integer>(_devPx * _myWidget->rect().bottom());
      } // end if

      return aMode;
}

Standard_Real OcctWindow::Ratio() const
{
    auto aRect = _myWidget->rect();
    return Standard_Real(aRect.right() - aRect.left()) / Standard_Real(aRect.bottom() - aRect.top());
}

void OcctWindow::Size(Standard_Integer &theWidth, Standard_Integer &theHeight) const
{
    theWidth = _devPx * _myWidget->rect().right();
    theHeight = _devPx * _myWidget->rect().bottom();
}


void OcctWindow::Position(Standard_Integer &theX1,
                          Standard_Integer &theY1,
                          Standard_Integer &theX2,
                          Standard_Integer &theY2) const
{
    theX1 = _devPx * _myWidget->rect().left();
    theX2 = _devPx * _myWidget->rect().right();
    theY1 = _devPx * _myWidget->rect().top();
    theY2 = _devPx * _myWidget->rect().bottom();
}

IMPLEMENT_STANDARD_RTTIEXT(OcctWindow,Aspect_Window)