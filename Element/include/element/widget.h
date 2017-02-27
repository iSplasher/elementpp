#pragma once

#include "layout.h"
#include "property.h"
#include "event.h"

typedef struct NVGcontext PainterContext;

NAMESPACE_BEGIN
class Painter;
class Window;


/**
 * \brief Base widget. Should be used to compose more specific widgets.
 */
class ELEMENT_API Widget : public PRIV_NAMESPACE::Layoutable {
public:

	enum MoveState {
		Normal = 0,
		Moving = 1
	};


	enum State {
		Acivated = 0,
		Disabled = 1,
	};


	// *structers
	explicit Widget( Widget* parent = nullptr );

	//Widget(const Widget&);
	//explicit Widget(const gWidgetProperties s, Widget *parent = nullptr);
	virtual ~Widget();

	// member methods
	virtual void paint( Painter& painter );

	void update() override;

	Property< bool, Widget > underMouse;
	Property< Font > font;
	Property< Color > borderColor;
	Property< Color > backgroundColor;
	Property< Color > foregroundColor;
	PropertyView< RectF > contentGeometry;
	PropertyView< SizeF > contentSize;

	Property< float > borderRadiusTopLeft;
	Property< float > borderRadiusTopRight;
	Property< float > borderRadiusBottomLeft;
	Property< float > borderRadiusBottomRight;

	bool paintWidget = true;

	// Events

	/**
	 * \brief Left mouse button is pressed
	 */
	PropertyEvent<PointF, Window> leftPress;
	/**
	 * \brief Right mouse button is pressed
	 */
	PropertyEvent<PointF, Window> rightPress;
	/**
	 * \brief Any mouse button is pressed
	 */
	PropertyEvent<MouseEvent, Window> pressed;

	/**
	 * \brief Left mouse button is released
	 */
	PropertyEvent<PointF, Window> leftReleased;
	/**
	 * \brief Right mouse button is released
	 */
	PropertyEvent<PointF, Window> rightReleased;
	/**
	 * \brief Any mouse button is released
	 */
	PropertyEvent<MouseEvent, Window> released;

	/**
	* \brief Left mouse button is clicked
	*/
	PropertyEvent<PointF, Window> leftClick;
	/**
	* \brief Right mouse button is clicked
	*/
	PropertyEvent<PointF, Window> rightClick;
	/**
	* \brief Any mouse button is clicked
	*/
	PropertyEvent<MouseEvent, Window> clicked;

	/**
	 * \brief Mouse moved
	 */
	PropertyEvent<MouseEvent, Window> mouseMoved;

	/**
	 * \brief Widget moved
	 */
	PropertyEvent<MouseEvent, Window> moved;


	/**
	 * \brief Translates the coordinates p of this widget to a coordinate p in the parent widget
	 * \param p Coordinates to translate
	 * \return The translated coordinate in the parent widget
	 */
	PointF mapToParent( PointF p ) const;

	/**
	 * \brief Overloaded function
	 * \param p Coordinates to translate
	 * \return The translated coordinate in the parent widget
	 */
	RectF mapToParent( RectF p ) const { return RectF( mapToParent( p.pos() ), p.size() ); }

	/**
	 * \brief Translates the coordinates p of the parent widget to a coordinate p in this widget
	 * \param p Coordinates to translate
	 * \return The translated coordinate in this widget
	 */
	PointF mapFromParent( PointF p ) const;

	/**
	 * \brief Overloaded function
	 * \param p Coordinates to translate
	 * \return The translated coordinate in this widget
	 */
	RectF mapFromParent( RectF p ) const { return RectF( mapFromParent( p.pos() ), p.size() ); }

	/**
	 * \brief Translates the coordinates p from the containing window
	 * \param p Coordinate to translate
	 * \return The translated coordinate from containing window
	 * \remark If there is no containing window, no translation will be done
	 */
	PointF mapFromWindow( PointF p );

	/**
	 * \brief Overloaded function
	 * \param p Coordinate to translate
	 * \return The translated coordinate from containing window
	 * \remark If there is no containing window, no translation will be done
	 */
	RectF mapFromWindow( RectF p ) { return RectF( mapFromWindow( p.pos() ), p.size() ); }

	/**
	* \brief Translates the coordinates p to the containing window's
	* \param p Coordinate to translate
	* \return The translated coordinate in containing window
	* \remark If there is no containing window, no translation will be done
	*/
	PointF mapToWindow( PointF p );

	/**
	* \brief Overloaded function
	* \param p Coordinate to translate
	* \return The translated coordinate in containing window
	* \remark If there is no containing window, no translation will be done
	*/
	RectF mapToWindow( RectF p ) { return RectF( mapToWindow( p.pos() ), p.size() ); }

	/**
	* \brief Translates the coordinates p from the desktop coordinates
	* \param p Coordinate to translate
	* \return The translated coordine from desktop
	*/
	PointF mapFromScreen( PointF p );

	/**
	* \brief Overloaded function
	* \param p Coordinate to translate
	* \return The translated coordine from desktop
	*/
	RectF mapFromScreen( RectF p ) { return RectF( mapFromScreen( p.pos() ), p.size() ); }

	/**
	* \brief Translates the coordinates p to the desktop coordinates
	* \param p Coordinate to translate
	* \return The translated coordine in desktop
	*/
	PointF mapToScreen( PointF p );

	/**
	* \brief Overloaded function
	* \param p Coordinate to translate
	* \return The translated coordine in desktop
	*/
	RectF mapToScreen( RectF p ) { return RectF( mapToScreen( p.pos() ), p.size() ); }


protected:

	struct Drag {
		bool is_draggable = true;
		PointF start_mouse_pos;
		PointF start_pos;
	};


	// data members
	Window* parent_window = nullptr;
	Widget* parent_widget;
	MoveState move_state = Normal;
	PainterContext* this_paint = nullptr;
	Drag drag;

private:

	void setParent( Element* ) override;

	friend class Painter;
	friend class Layout;
};



NAMESPACE_END
