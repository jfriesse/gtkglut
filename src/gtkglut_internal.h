#ifndef __GTKGLUT_INTERNAL_H__
#define __GTKGLUT_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkgl.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdk.h>

#define  __GTKGLUT_MAJOR_VERSION 0
#define  __GTKGLUT_MINOR_VERSION 3
#define  __GTKGLUT_MICRO_VERSION 7

#define __GTKGLUT_WINDOW_INIT_WIDTH  300
#define __GTKGLUT_WINDOW_INIT_HEIGHT 300
#define __GTKGLUT_WINDOW_INIT_X       -1
#define __GTKGLUT_WINDOW_INIT_Y       -1

#define __GTKGLUT_DIRECT_RENDER_NOT_SET  0
#define __GTKGLUT_DIRECT_RENDER_INDIRECT 1
#define __GTKGLUT_DIRECT_RENDER_DIRECT   2

#define __GTKGLUT_INIT_DISPLAY_MODE    GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH

#define __GTKGLUT_DISABLED_CONST (~0)
#define __GTKGLUT_UNDEFINED (~0)

#define __GTKGLUT_MAX_MOUSE_BUTTONS 5

#define __GTKGLUT_MAX_MENU_DEPTH 64

typedef struct __gtkglut_geometry_struct {
	int x, y, width, height;
} __gtkglut_geometry_struct;

typedef struct __gtkglut_cursors_cache_struct {
	int gdk_id;
	GdkCursor *cursor;
} __gtkglut_cursors_cache_struct;

typedef struct __gtkglut_refset_item {
	gpointer ref;
	int value;
} __gtkglut_refset_item;

typedef struct __gtkglut_context_struct {
	int is_inicialized;
	__gtkglut_geometry_struct window_init_geometry;
	int window_init_iconic;
	int gl_debug;
	int direct_render;
	unsigned int display_mode;
	GTimer *elapsed_time_timer;
	void (*callback_idle) (void);
	GdkGLConfig *actual_gl_config;
	GPtrArray *window_array;
	GPtrArray *menu_array;
	GdkGLDrawable *current_gl_drawable;
	GdkGLContext *current_gl_context;
	int current_window;
	int current_menu;
	__gtkglut_cursors_cache_struct *cursors_cache;
	GdkCursor *cursor_none;
	guint keyboard_modifiers;
	void (*menu_status_func) (int status, int x, int y);
	void (*menu_state_func) (int status);
	int action_on_window_close;
	int no_active_toplevel_windows;
	int current_active_menu_window_id;
} __gtkglut_context_struct;

typedef struct __gtkglut_timer_callback_struct {
	int value;
	void (*func) (int value);
} __gtkglut_timer_callback_struct;

typedef struct __gtkglut_signal_struct {
	/*
	 * Event handler for g_signal_connect
	 */
	gpointer gtk_event_handler;
	/*
	 * Function called from gtk_event_handler
	 */
	gpointer glut_callback_handler;
	/*
	 * Signal id, for deregistering
	 */
	int signal_id;
	/*
	 * if event is added
	 */
	gboolean event_added;
} __gtkglut_signal_struct;

typedef struct __gtkglut_window_struct {
	GtkWidget *window;
	GtkWidget *drawing_area;
	GtkWidget *event_box;
	GtkWidget *fixed;
	int parent_window;
	gboolean first_displayed;
	gboolean fullscreen;
	GSList *child_list;
	gboolean window_visible;
	int window_status;
	int current_cursor;
	gboolean damaged;
	gboolean iconified;
	void (*callback_display) (void);
	void (*callback_reshape) (int width, int height);
	__gtkglut_signal_struct signal_visibility;
	__gtkglut_signal_struct signal_status;
	__gtkglut_signal_struct signal_keyboard;
	__gtkglut_signal_struct signal_keyboard_special;
	__gtkglut_signal_struct signal_keyboard_up;
	__gtkglut_signal_struct signal_keyboard_special_up;
	__gtkglut_signal_struct signal_mouse_press;
	__gtkglut_signal_struct signal_mouse_release;
	__gtkglut_signal_struct signal_mouse_wheel;
	__gtkglut_signal_struct signal_motion;
	__gtkglut_signal_struct signal_passive_motion;
	__gtkglut_signal_struct signal_entry;
	void (*callback_close) (void);
	int mouse_button_menu[__GTKGLUT_MAX_MOUSE_BUTTONS];
	void *user_data;
} __gtkglut_window_struct;

typedef struct __gtkglut_menu_struct {
	GtkWidget *cached_gtk_menu;
	void (*func) (int value);
	GList *child_items;
	gboolean in_use;
	int popuped_from_window_id;
	gboolean need_refresh;
	GList *parent_menus;
	void *user_data;
} __gtkglut_menu_struct;

typedef struct __gtkglut_menu_item_struct {
	char *label;
	int parent_menu_id;
	int submenu_id;
	int value;
} __gtkglut_menu_item_struct;

/*
 * Freeglut font structure
 */

/*
 * The bitmap font structure
 */
typedef struct __gtkglut_tagSFG_Font __gtkglut_SFG_Font;

struct __gtkglut_tagSFG_Font {
	/*
	 * The source font name
	 */
	char *Name;
	/*
	 * Number of chars in font
	 */
	int Quantity;
	/*
	 * Height of the characters
	 */
	int Height;
	/*
	 * The characters mapping
	 */
	const GLubyte **Characters;
	/*
	 * Relative origin of the character
	 */
	float xorig, yorig;
};

/*
 * The stroke font structures
 */

typedef struct __gtkglut_tagSFG_StrokeVertex __gtkglut_SFG_StrokeVertex;

struct __gtkglut_tagSFG_StrokeVertex {
	GLfloat X, Y;
};

typedef struct __gtkglut_tagSFG_StrokeStrip __gtkglut_SFG_StrokeStrip;

struct __gtkglut_tagSFG_StrokeStrip {
	int Number;
	const __gtkglut_SFG_StrokeVertex *Vertices;
};

typedef struct __gtkglut_tagSFG_StrokeChar __gtkglut_SFG_StrokeChar;

struct __gtkglut_tagSFG_StrokeChar {
	GLfloat Right;
	int Number;
	const __gtkglut_SFG_StrokeStrip *Strips;
};

typedef struct __gtkglut_tagSFG_StrokeFont __gtkglut_SFG_StrokeFont;

struct __gtkglut_tagSFG_StrokeFont {
	/*
	 * The source font name
	 */
	char *Name;
	/*
	 * Number of chars in font
	 */
	int Quantity;
	/*
	 * Height of the characters
	 */
	GLfloat Height;
	/*
	 * The characters mapping
	 */
	const __gtkglut_SFG_StrokeChar **Characters;
};

/*
 * Refset ADS
 */
extern GList *__gtkglut_refset_ref(GList * list, gpointer ref);
extern GList *__gtkglut_refset_unref(GList * list, gpointer ref);
extern GList *__gtkglut_refset_destroy(GList * list);

/*
 * Actual gtkglut context
 */
extern __gtkglut_context_struct *__gtkglut_context;

/*
 * Error if application has low memory. This function ends with exit(1), so
 * you never return from these.
 */
extern void __gtkglut_lowmem(void);
extern void __gtkglut_error(char *func_name, char *fmt, ...);
extern void __gtkglut_warning(char *func_name, char *fmt, ...);

extern void __gtkglut_context_init(void);

/*
 * Test, if library was inicialized. If not, show error and quit
 */
extern void __gtkglut_test_inicialization(char *func_name);

/*
 * Init GdkGLConfig with values from init_display_mode
 */
extern GdkGLConfig *__gtkglut_init_gl_config(void);

/*
 * Test if there was any glerror and show error message
 */
extern void __gtkglut_gl_debug();

/*
 * Get glut window indetified by window_id
 */
extern __gtkglut_window_struct *__gtkglut_get_window_by_id(int window_id);

/*
 * Get active window. If active window is not real window (for example after destroy window),
 * write warning and return NULL.
 */
extern __gtkglut_window_struct *__gtkglut_get_active_window_with_warning(char *func_name);

/*
 * Show warning for unimplemented function
 */
extern void __gtkglut_unimplemented_warning(char *func_name);

/*
 * Get glut menu indentified by menu_id
 */
extern __gtkglut_menu_struct *__gtkglut_get_menu_by_id(int menu_id);

/*
 * For use in gtkglut_menu
 */
extern void __gtkglut_register_mouse_func(__gtkglut_window_struct * glut_window);
extern void __gtkglut_unregister_mouse_func(__gtkglut_window_struct * glut_window,
    int menu_button);

/*
 * Call menu status/state function from window with id window_id and menu with id menu_id
 */
extern void __gtkglut_call_menu_status_func(int status, int x, int y, int window_id,
    int menu_id);

/*
 * Set menu and all his submenus in_use
 */
extern void __gtkglut_set_menu_in_use(__gtkglut_menu_struct * glut_menu, gboolean in_use);

/*
 * Get active menu. If active menu is not real window (for example after destroy window),
 * write warning and return NULL.
 */
extern __gtkglut_menu_struct *__gtkglut_get_active_menu_with_warning(char *func_name);

/*
 * Regenerate menu
 */
extern GtkWidget *__gtkglut_regenerate_menu(int menu_id, int depth);

#ifdef __cplusplus
}
#endif
#endif /*__GTKGLUT_INTERNAL_H__*/
