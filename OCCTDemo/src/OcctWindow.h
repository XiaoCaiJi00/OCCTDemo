#ifndef OCCTWINDOW_H
#define OCCTWINDOW_H

// Qt headers
#include <QtWidgets/QWidget>

// occ headers
#include <Aspect_Window.hxx>

class OcctWindow : public Aspect_Window
{
    DEFINE_STANDARD_RTTIEXT(OcctWindow, Aspect_Window)

public:

    // constructor
    OcctWindow(QWidget *parent = nullptr, const Quantity_NameOfColor theBackColor = Quantity_NOC_MATRAGRAY);

    // destructor
    virtual ~OcctWindow() { _myWidget = nullptr; }

    // Returns native Window handle
    virtual Aspect_Drawable NativeHandle() const override;

    // Returns parent of native Window handle.
    virtual Aspect_Drawable NativeParentHandle() const override;

    // Applies the resizing to the window <me>
    virtual Aspect_TypeOfResize DoResize() override;

    // Returns True if the window <me> is opened
    // and False if the window is closed.
    virtual Standard_Boolean IsMapped() const override;

    // Apply the mapping change to the window <me>
    // and returns TRUE if the window is mapped at screen.
    virtual Standard_Boolean DoMapping() const override { return Standard_True; }

    // Opens the window <me>.
    virtual void Map() const override;

    //! Closes the window <me>.
    virtual void Unmap() const override;

    virtual void Position( Standard_Integer& theX1,
                           Standard_Integer& theY1,
                           Standard_Integer& theX2,
                           Standard_Integer& theY2
                           ) const override;

    // Returns the window RATIO equal to the physical
    // WIDTH/HEIGHT dimensions.
    virtual Standard_Real Ratio() const override;

    virtual void Size(Standard_Integer& theWidth, Standard_Integer& theHeight ) const override;

    virtual Aspect_FBConfig NativeFBConfig() const Standard_OVERRIDE { return NULL; }

protected:
    Standard_Integer _xLeft;
    Standard_Integer _yTop;
    Standard_Integer _xRight;
    Standard_Integer _yBottom;
    const Standard_Real _devPx;
    QWidget* _myWidget;
};

#endif // OCCTWINDOW_H
