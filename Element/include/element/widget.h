#pragma once

#include "layout.h"
#include "property.h"
#include "event.h"

typedef struct NVGcontext PainterContext;

NAMESPACE_BEGIN
class Painter;
class Window;

PRIV_NAMESPACE_BEGIN
class _Cursor;
NAMESPACE_END



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
	PropertyView< Rect > contentGeometry;
	PropertyView< Size > contentSize;

	Property< float > borderRadiusTopLeft;
	Property< float > borderRadiusTopRight;
	Property< float > borderRadiusBottomLeft;
	Property< float > borderRadiusBottomRight;

	Cursor cursor = Cursor::Arrow;

	bool paintWidget = true;
	bool isDraggable = false;
	bool isResizeable = false;

	// Events

	/**
	 * \brief Left mouse button is pressed
	 */
	PropertyEvent<Point, Window> leftPress;
	/**
	 * \brief Right mouse button is pressed
	 */
	PropertyEvent<Point, Window> rightPress;
	/**
	 * \brief Any mouse button is pressed
	 */
	PropertyEvent<MouseEvent, Window> pressed;

	/**
	* \brief Left mouse button is doublepressed
	*/
	PropertyEvent<Point, Window> leftDoublePress;
	/**
	* \brief Right mouse button is doublepressed
	*/
	PropertyEvent<Point, Window> rightDoublePress;
	/**
	* \brief Any mouse button is doublepressed
	*/
	PropertyEvent<MouseEvent, Window> doublePress;

	/**
	 * \brief Left mouse button is released
	 */
	PropertyEvent<Point, Window> leftRelease;
	/**
	 * \brief Right mouse button is released
	 */
	PropertyEvent<Point, Window> rightRelease;
	/**
	 * \brief Any mouse button is released
	 */
	PropertyEvent<MouseEvent, Window> release;

	/**
	* \brief Left mouse button is clicked
	*/
	PropertyEvent<Point, Window> leftClick;
	/**
	* \brief Right mouse button is clicked
	*/
	PropertyEvent<Point, Window> rightClick;
	/**
	* \brief Any mouse button is clicked
	*/
	PropertyEvent<MouseEvent, Window> click;

	/**
	* \brief Left mouse button is doubleclicked
	*/
	PropertyEvent<Point, Window> leftDoubleClick;
	/**
	* \brief Right mouse button is doubleclicked
	*/
	PropertyEvent<Point, Window> rightDoubleClick;
	/**
	* \brief Any mouse button is doubleclicked
	*/
	PropertyEvent<MouseEvent, Window> doubleClick;

	/**
	 * \brief Mouse moved
	 */
	PropertyEvent<MouseEvent, Window> mouseMoved;

	/**
	 * \brief When the widget is moved this property emits the new position
	 */
	PropertyEvent<Point, Widget> moved;

	/**
	* \brief When the widget is resized this property emits the new size
	*/
	PropertyEvent<Rect, Widget> resized;


	/**
	 * \brief Translates the coordinates p of this widget to a coordinate p in the parent widget
	 * \param p Coordinates to translate
	 * \return The translated coordinate in the parent widget
	 */
	Point mapToParent( Point p ) const;

	/**
	 * \brief Overloaded function
	 * \param p Coordinates to translate
	 * \return The translated coordinate in the parent widget
	 */
	Rect mapToParent( Rect p ) const { return Rect( mapToParent( p.pos() ), p.size() ); }

	/**
	 * \brief Translates the coordinates p of the parent widget to a coordinate p in this widget
	 * \param p Coordinates to translate
	 * \return The translated coordinate in this widget
	 */
	Point mapFromParent( Point p ) const;

	/**
	 * \brief Overloaded function
	 * \param p Coordinates to translate
	 * \return The translated coordinate in this widget
	 */
	Rect mapFromParent( Rect p ) const { return Rect( mapFromParent( p.pos() ), p.size() ); }

	/**
	 * \brief Translates the coordinates p from the containing window
	 * \param p Coordinate to translate
	 * \return The translated coordinate from containing window
	 * \remark If there is no containing window, no translation will be done
	 */
	Point mapFromWindow( Point p );

	/**
	 * \brief Overloaded function
	 * \param p Coordinate to translate
	 * \return The translated coordinate from containing window
	 * \remark If there is no containing window, no translation will be done
	 */
	Rect mapFromWindow( Rect p ) { return Rect( mapFromWindow( p.pos() ), p.size() ); }

	/**
	* \brief Translates the coordinates p to the containing window's
	* \param p Coordinate to translate
	* \return The translated coordinate in containing window
	* \remark If there is no containing window, no translation will be done
	*/
	Point mapToWindow( Point p );

	/**
	* \brief Overloaded function
	* \param p Coordinate to translate
	* \return The translated coordinate in containing window
	* \remark If there is no containing window, no translation will be done
	*/
	Rect mapToWindow( Rect p ) { return Rect( mapToWindow( p.pos() ), p.size() ); }

	/**
	* \brief Translates the coordinates p from the desktop coordinates
	* \param p Coordinate to translate
	* \return The translated coordine from desktop
	*/
	Point mapFromScreen( Point p );

	/**
	* \brief Overloaded function
	* \param p Coordinate to translate
	* \return The translated coordine from desktop
	*/
	Rect mapFromScreen( Rect p ) { return Rect( mapFromScreen( p.pos() ), p.size() ); }

	/**
	* \brief Translates the coordinates p to the desktop coordinates
	* \param p Coordinate to translate
	* \return The translated coordine in desktop
	*/
	Point mapToScreen( Point p );

	/**
	* \brief Overloaded function
	* \param p Coordinate to translate
	* \return The translated coordine in desktop
	*/
	Rect mapToScreen( Rect p ) { return Rect( mapToScreen( p.pos() ), p.size() ); }


private:

	// data members

	Window* parent_window = nullptr;
	Widget* parent_widget;
	PainterContext* this_paint = nullptr;
	Point last_mouse_pos;
	std::unique_ptr<PRIV_NAMESPACE::_Cursor> _cursor = nullptr;
	Point resize_pos;
	Direction resize_dir = Direction::None;

	void setParent( Element* ) override;
	static void handleMove(MouseEvent m_ev); // handle movement
	void windowMovedHelper(Point);

	friend class Painter;
	friend class Layout;
	friend class Window;
};



NAMESPACE_END
