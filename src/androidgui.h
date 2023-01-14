/* Android window system support.
   Copyright (C) 2023 Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef _ANDROID_GUI_H_
#define _ANDROID_GUI_H_

struct android_char_struct
{
  int rbearing;
  int lbearing;
  int width;
  int ascent;
  int descent;
};

typedef struct android_char_struct XCharStruct;

typedef unsigned short android_handle;

typedef android_handle android_pixmap, Emacs_Pixmap;
typedef android_handle android_window, Emacs_Window;
typedef android_handle android_gcontext, GContext;
typedef android_handle android_drawable, Drawable;

typedef unsigned int android_time;

struct android_rectangle
{
  int x, y;
  unsigned width, height;
};

struct android_point
{
  int x, y;
};

/* Keep this in sync with EmacsGC.java! */

enum android_gc_function
  {
    ANDROID_GC_COPY	= 0,
    ANDROID_GC_XOR	= 1,
  };

enum android_gc_value_mask
  {
    ANDROID_GC_FOREGROUND	  = (1 << 0),
    ANDROID_GC_BACKGROUND	  = (1 << 1),
    ANDROID_GC_FUNCTION		  = (1 << 2),
    ANDROID_GC_CLIP_X_ORIGIN	  = (1 << 3),
    ANDROID_GC_CLIP_Y_ORIGIN	  = (1 << 4),
    ANDROID_GC_CLIP_MASK	  = (1 << 5),
    ANDROID_GC_STIPPLE		  = (1 << 6),
    ANDROID_GC_FILL_STYLE	  = (1 << 7),
    ANDROID_GC_TILE_STIP_X_ORIGIN = (1 << 8),
    ANDROID_GC_TILE_STIP_Y_ORIGIN = (1 << 9),
  };

enum android_fill_style
  {
    ANDROID_FILL_SOLID		 = 0,
    ANDROID_FILL_OPAQUE_STIPPLED = 1,
  };

enum android_window_value_mask
  {
    ANDROID_CW_BACK_PIXEL	 = (1 << 1),
    ANDROID_CW_OVERRIDE_REDIRECT = (1 << 2),
  };

struct android_set_window_attributes
{
  /* The background pixel.  */
  unsigned long background_pixel;

  /* Whether or not the window is override redirect.  This cannot be
     set after creation on Android.  */
  bool override_redirect;
};

struct android_gc_values
{
  /* The foreground and background.  */
  unsigned long foreground, background;

  /* The function.  */
  enum android_gc_function function;

  /* The fill style.  */
  enum android_fill_style fill_style;

  /* The clip X and Y origin.  */
  int clip_x_origin, clip_y_origin;

  /* The clip mask image and stipple.  */
  android_pixmap clip_mask, stipple;

  /* The tile-stipple X and Y origins.  */
  int ts_x_origin, ts_y_origin;
};

/* X-like graphics context structure.  This is implemented in
   EmacsGC.java, but a copy is kept here to avoid sending changes all
   the time.  */

struct android_gc
{
  /* Array of clip rectangles.  */
  struct android_rectangle *clip_rects;

  /* Number of clip rectangles.  When -1, it means clipping should not
     be applied.  */
  int num_clip_rects;

  /* The Java-side handle.  */
  android_gcontext gcontext;

  /* Current foreground color.  */
  unsigned long foreground;

  /* Current background color.  */
  unsigned long background;
};

enum android_swap_action
  {
    ANDROID_COPIED,
  };

enum android_shape
  {
    ANDROID_CONVEX,
  };

enum android_coord_mode
  {
    ANDROID_COORD_MODE_ORIGIN,
  };

struct android_swap_info
{
  /* The window to swap.  */
  android_window swap_window;

  /* Unused field present only for consistency with X.  */
  enum android_swap_action swap_action;
};

/* Android doesn't support cursors, so define this to something
   unused.  */
typedef char Emacs_Cursor;

#define NativeRectangle			Emacs_Rectangle
#define CONVERT_TO_NATIVE_RECT(xr, nr)	((xr) = (nr))
#define CONVERT_FROM_EMACS_RECT(xr, nr) ((nr) = (xr))

#define STORE_NATIVE_RECT(nr, rx, ry, rwidth, rheight)	\
  ((nr).x = (rx), (nr).y = (ry),			\
   (nr).width = (rwidth), (nr).height = (rheight))	\

#define ForgetGravity		0
#define NorthWestGravity	1
#define NorthGravity		2
#define NorthEastGravity	3
#define WestGravity		4
#define CenterGravity		5
#define EastGravity		6
#define SouthWestGravity	7
#define SouthGravity		8
#define SouthEastGravity	9
#define StaticGravity		10

#define NoValue		0x0000
#define XValue  	0x0001
#define YValue		0x0002
#define WidthValue  	0x0004
#define HeightValue  	0x0008
#define AllValues 	0x000F
#define XNegative 	0x0010
#define YNegative 	0x0020

#define USPosition	(1L << 0) /* user specified x, y */
#define USSize		(1L << 1) /* user specified width, height */
#define PPosition	(1L << 2) /* program specified position */
#define PSize		(1L << 3) /* program specified size */
#define PMinSize	(1L << 4) /* program specified minimum size */
#define PMaxSize	(1L << 5) /* program specified maximum size */
#define PResizeInc	(1L << 6) /* program specified resize increments */
#define PAspect		(1L << 7) /* program specified min, max aspect ratios */
#define PBaseSize	(1L << 8) /* program specified base for incrementing */
#define PWinGravity	(1L << 9) /* program specified window gravity */

#ifndef ANDROID_STUBIFY

/* Universal NULL handle.  */
static const int ANDROID_NONE;

/* Keep these as conceptually close to X as possible: that makes
   synchronizing code between the ports much easier.  */

enum android_event_type
  {
    ANDROID_KEY_PRESS,
    ANDROID_KEY_RELEASE,
    ANDROID_CONFIGURE_NOTIFY,
    ANDROID_FOCUS_IN,
    ANDROID_FOCUS_OUT,
    ANDROID_WINDOW_ACTION,
    ANDROID_ENTER_NOTIFY,
    ANDROID_LEAVE_NOTIFY,
    ANDROID_MOTION_NOTIFY,
    ANDROID_BUTTON_PRESS,
    ANDROID_BUTTON_RELEASE,
    ANDROID_TOUCH_DOWN,
    ANDROID_TOUCH_UP,
    ANDROID_TOUCH_MOVE,
    ANDROID_WHEEL,
    ANDROID_ICONIFIED,
    ANDROID_DEICONIFIED,
  };

struct android_any_event
{
  enum android_event_type type;
  android_window window;
};

enum android_modifier_mask
  {
    ANDROID_SHIFT_MASK	 = 193,
    ANDROID_CONTROL_MASK = 4096,
    ANDROID_ALT_MASK	 = 2,
  };

struct android_key_event
{
  enum android_event_type type;
  android_window window;
  android_time time;
  unsigned int state;
  unsigned int keycode;
  unsigned int unicode_char;
};

/* These hard coded values are Android modifier keycodes derived
   through experimentation.  */

#define ANDROID_IS_MODIFIER_KEY(key)					\
  ((key) == 57 || (key) == 58 || (key) == 113 || (key) == 114		\
   || (key) == 119 || (key) == 117 || (key) == 118 || (key) == 78	\
   || (key) == 94 || (key) == 59 || (key) == 60 || (key) == 95		\
   || (key) == 63 || (key) == 115)

struct android_configure_event
{
  enum android_event_type type;
  android_window window;
  android_time time;
  int x, y;
  int width, height;
};

struct android_focus_event
{
  enum android_event_type type;
  android_window window;
  android_time time;
};

struct android_window_action_event
{
  enum android_event_type type;

  /* The window handle.  This can be ANDROID_NONE.  */
  android_window window;

  /* Numerical identifier for this action.  If 0 and WINDOW is set,
     then it means the frame associated with that window has been
     destroyed.  Otherwise, it means Emacs should create a new
     frame.  */
  unsigned int action;
};

struct android_crossing_event
{
  enum android_event_type type;
  android_window window;
  int x, y;
  unsigned long time;
};

struct android_motion_event
{
  enum android_event_type type;
  android_window window;
  int x, y;
  unsigned long time;
};

struct android_button_event
{
  enum android_event_type type;
  android_window window;
  int x, y;
  unsigned long time;
  unsigned int state;
  unsigned int button;
};

struct android_touch_event
{
  /* Type of the event.  */
  enum android_event_type type;

  /* Window associated with the event.  */
  android_window window;

  /* X and Y coordinates of the event.  */
  int x, y;

  /* Time of the event, and the pointer identifier.  */
  unsigned long time;

  /* Index of the pointer being tracked.  */
  unsigned int pointer_id;
};

struct android_wheel_event
{
  /* Type of the event.  */
  enum android_event_type type;

  /* Window associated with the event.  */
  android_window window;

  /* X and Y coordinates of the event.  */
  int x, y;

  /* Time of the event, and the pointer identifier.  */
  unsigned long time;

  /* Modifier state at the time of the event.  */
  int state;

  /* Motion alongside the X and Y axes.  */
  double x_delta, y_delta;
};

struct android_iconify_event
{
  /* Type of the event.  */
  enum android_event_type type;

  /* Window associated with the event.  */
  android_window window;
};

union android_event
{
  enum android_event_type type;
  struct android_any_event xany;
  struct android_key_event xkey;
  struct android_configure_event xconfigure;
  struct android_focus_event xfocus;
  struct android_window_action_event xaction;
  struct android_crossing_event xcrossing;
  struct android_motion_event xmotion;
  struct android_button_event xbutton;

  /* This has no parallel in X, since the X model of having
     monotonically increasing touch IDs can't work on Android.  */
  struct android_touch_event touch;

  /* This has no parallel in X outside the X Input Extension, and
     emulating the input extension interface would be awfully
     complicated.  */
  struct android_wheel_event wheel;

  /* This has no parallel in X because Android doesn't have window
     properties.  */
  struct android_iconify_event iconified;
};

enum
  {
    ANDROID_CURRENT_TIME = 0L,
  };

extern int android_pending (void);
extern void android_next_event (union android_event *);

extern android_window android_create_window (android_window, int,
					     int, int, int,
					     enum android_window_value_mask,
					     struct
					     android_set_window_attributes *);
extern void android_change_window_attributes (android_window,
					      enum android_window_value_mask,
					      struct
					      android_set_window_attributes *);
extern void android_set_window_background (android_window, unsigned long);
extern void android_destroy_window (android_window);
extern void android_reparent_window (android_window, android_window,
				     int, int);
extern void android_set_clip_rectangles (struct android_gc *,
					 int, int,
					 struct android_rectangle *,
					 int);
extern void android_change_gc (struct android_gc *,
			       enum android_gc_value_mask,
			       struct android_gc_values *);

extern void android_clear_window (android_window);
extern void android_map_window (android_window);
extern void android_unmap_window (android_window);
extern void android_resize_window (android_window, unsigned int,
				   unsigned int);
extern void android_move_window (android_window, int, int);
extern void android_swap_buffers (struct android_swap_info *, int);
extern void android_get_gc_values (struct android_gc *,
				   enum android_gc_value_mask,
				   struct android_gc_values *);
extern void android_set_foreground (struct android_gc *,
				    unsigned long);
extern void android_fill_rectangle (android_drawable, struct android_gc *,
				    int, int, unsigned int, unsigned int);
extern android_pixmap android_create_pixmap_from_bitmap_data (char *,
							      unsigned int,
							      unsigned int,
							      unsigned long,
							      unsigned long,
							      unsigned int);
extern void android_set_clip_mask (struct android_gc *, android_pixmap);
extern void android_set_fill_style (struct android_gc *,
				    enum android_fill_style);
extern void android_copy_area (android_drawable, android_drawable,
			       struct android_gc *, int, int,
			       unsigned int, unsigned int, int, int);
extern void android_free_pixmap (android_drawable);

extern void android_set_background (struct android_gc *, unsigned long);
extern void android_fill_polygon (android_drawable, struct android_gc *,
				  struct android_point *, int,
				  enum android_shape,
				  enum android_coord_mode);
extern void android_draw_rectangle (android_drawable, struct android_gc *,
				    int, int, unsigned int, unsigned int);
extern void android_draw_point (android_window, struct android_gc *,
				int, int);
extern void android_draw_line (android_window, struct android_gc *,
			       int, int, int, int);
extern android_pixmap android_create_pixmap (unsigned int, unsigned int,
					     int);
extern void android_set_ts_origin (struct android_gc *, int, int);
extern void android_clear_area (android_window, int, int, unsigned int,
				unsigned int);
extern android_pixmap android_create_bitmap_from_data (char *, unsigned int,
						       unsigned int);

extern void android_bell (void);
extern void android_set_input_focus (android_window, unsigned long);
extern void android_raise_window (android_window);
extern void android_lower_window (android_window);
extern int android_query_tree (android_window, android_window *,
			       android_window *, android_window **,
			       unsigned int *);
extern void android_get_geometry (android_window, android_window *,
				  int *, int *, unsigned int *,
				  unsigned int *, unsigned int *);
extern void android_move_resize_window (android_window, int, int,
					unsigned int, unsigned int);
extern void android_map_raised (android_window);
extern void android_translate_coordinates (android_window, int,
					   int, int *, int *);

#endif



/* Image support.  Keep the API as similar to XImage as possible.  To
   avoid leaving a huge mess of "#ifndef ANDROID_STUBIFY" in image.c,
   stubs should be defined for all functions.  */

enum android_image_format
  {
    ANDROID_Z_PIXMAP,
  };

struct android_image
{
  int width, height;
  enum android_image_format format;
  char *data;
  int depth;
  int bytes_per_line;
  int bits_per_pixel;
};

extern struct android_image *android_create_image (unsigned int,
						   enum android_image_format,
						   char *, unsigned int,
						   unsigned int);
extern void android_destroy_image (struct android_image *);

extern void android_put_pixel (struct android_image *, int, int,
			       unsigned long);
extern unsigned long android_get_pixel (struct android_image *, int, int);
extern struct android_image *android_get_image (android_drawable,
						enum android_image_format);
extern void android_put_image (android_pixmap, struct android_image *);



/* X emulation stuff also needed while building stubs.  */

extern struct android_gc *android_create_gc (enum android_gc_value_mask,
					     struct android_gc_values *);
extern void android_free_gc (struct android_gc *);

#endif /* _ANDROID_GUI_H_ */
