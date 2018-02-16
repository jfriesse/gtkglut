/*!
 * \file  gtkglut_menu.c
 * \brief Pop-up menu creation and handling.
 */

/*
 * Pop-up menu creation and handling.
 *
 * Copyright (c) 2008-2018 Jan Friesse. All Rights Reserved.
 * Written by Jan Friesse, <jfriesse@gmail.comt>
 * Creation date: Sun Mar 16 2008
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Jan Friesse shall not be
 * used in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Jan Friesse.
 */

#include "GL/gtkglut.h"
#include "gtkglut_internal.h"

/*
 * Append glut_menu to context menu array
 */
void __gtkglut_menu_array_append(__gtkglut_menu_struct * glut_menu)
{

	if (!__gtkglut_context->menu_array) {
		__gtkglut_context->menu_array = g_ptr_array_new();

		if (!__gtkglut_context->menu_array)
			__gtkglut_lowmem();

		/*
		 * Because GLUT start numbering from 1, we do this same way
		 */
		g_ptr_array_add(__gtkglut_context->menu_array, NULL);
	}

	g_ptr_array_add(__gtkglut_context->menu_array, glut_menu);
}

/*
 * Return gtkglut_menu by id from context menu array
 */
__gtkglut_menu_struct *__gtkglut_get_menu_by_id(int menu_id)
{

	if (menu_id < 0 || !__gtkglut_context->menu_array
	    || (guint) menu_id >= __gtkglut_context->menu_array->len) {
		return NULL;
	}

	return (__gtkglut_menu_struct *) g_ptr_array_index(__gtkglut_context->menu_array, menu_id);
}

/*!
 * \defgroup menus Menu Management
 *
 * GtkGLUT provides an implementation of GLUT-like cascading
 * text menus.  The menus are basically association lists:
 * A string of text is associated either with an integer
 * or with a submenu.  If with a submenu, then dragging the
 * mouse over the submenu's name brings up the submenu.
 * Submenu items can in turn have items with menus-or-integers.
 * This allows you to nest menus to any desired depth.
 *
 * Actual action generally occurs when an integer-bound
 * item is selected.  The integer can be any integer of
 * the application's choosing, provided that it fits into
 * a variable of type \a int.  Enumerated values, array indices,
 * and OpenGL and GtkGLUT ids are common choices.  The item
 * selection is handed to a menu callback that you specify via
 * glutCreateMenu().  (Each menu can have its own callback,
 * or you can use one callback for all menus, or something inbetween,
 * according to your application's needs snf your own preferences.)
 *
 * The original GLUT documentation disclaimed GLUT menus as
 * being an attempt at ``a full-featured user interface''.
 * Certainly, the GLUT menus are limited, but they are
 * functional.  At present, they are the only officially supported
 * mechanism for creating practical menus.  (Using subwindows in your
 * own application window is possible, but the possibility of having
 * your menus occluded by other applications' windows is a bit
 * irritating.)
 */

/*!
 * \brief    Get the current menu ID.
 * \ingroup  menus
 *
 * Returns the integer ID of the current menu.
 *
 * \see glutSetMenu()
 */
int glutGetMenu(void)
{
	int res;
	__gtkglut_menu_struct *glut_menu;

	__gtkglut_test_inicialization("glutGetMenu");

	glut_menu = __gtkglut_get_menu_by_id(__gtkglut_context->current_menu);

	res = (glut_menu == NULL) ? 0 : __gtkglut_context->current_menu;

	return res;
}

/*
 * Get active gtkglut_menu. If no menu is active, show warning and return NULL
 */
__gtkglut_menu_struct *__gtkglut_get_active_menu_with_warning(char *func_name)
{
	int menu_id;
	__gtkglut_menu_struct *glut_menu;

	menu_id = glutGetMenu();

	glut_menu = __gtkglut_get_menu_by_id(menu_id);

	if (!glut_menu) {
		__gtkglut_warning(func_name, "attempted on bogus menu %d!", menu_id);
	}

	return glut_menu;
}

/*!
 * \brief    Set the current menu ID.
 * \ingroup  menus
 * \param    menu    Selected menu
 *
 * Set the *current menu* to the specified \a menuID.
 *
 * \see glutGetMenu()
 */
void glutSetMenu(int menu)
{
	__gtkglut_menu_struct *glut_menu;

	__gtkglut_test_inicialization("glutSetMenu");

	if (menu < 1 || (guint) menu >= __gtkglut_context->menu_array->len) {
		__gtkglut_warning("glutSetMenu", "Attempted on bogus menu %d!", menu);
		return;
	}

	__gtkglut_context->current_menu = menu;

	glut_menu = __gtkglut_get_menu_by_id(menu);

	if (!glut_menu) {
		__gtkglut_warning("glutSetMenu", "You are trying set menu to nonexistent menu %d!",
				  menu);

		return;
	}
}

/*
 * Set in_use flag for gtkglut_menu and all submenus
 */
void __gtkglut_set_menu_in_use(__gtkglut_menu_struct * glut_menu, gboolean in_use)
{
	GList *elem;
	__gtkglut_menu_item_struct *menu_item;
	__gtkglut_menu_struct *glut_submenu;

	if (glut_menu->in_use != in_use) {
		/*
		 * Set in use
		 */
		glut_menu->in_use = in_use;

		/*
		 * Traverse all childs data
		 */
		elem = glut_menu->child_items;

		while (elem) {
			/*
			 * Test if child is submenu
			 */
			menu_item = (__gtkglut_menu_item_struct *) (elem->data);

			if (menu_item->submenu_id) {
				/*
				 * Menu have submenu
				 */

				glut_submenu = __gtkglut_get_menu_by_id(menu_item->submenu_id);

				if (!glut_submenu) {
					__gtkglut_warning("__gtkglut_set_menu_in_use",
					    "attempt on bogus submenu %d",
					    menu_item->submenu_id);
				} else {
					__gtkglut_set_menu_in_use(glut_submenu, in_use);
				}
			}

			elem = g_list_next(elem);
		}
	}
}

/*
 * GTK+ deactivate handler for menu
 */
void __gtkglut_callback_menu_deactivate_handler(GtkMenuShell * menushell, gpointer data)
{
	int menu_id;
	int window_id;
	__gtkglut_menu_struct *glut_menu;
	__gtkglut_window_struct *glut_window;
	int mouse_x, mouse_y;

	menu_id = GPOINTER_TO_INT(data);

	glut_menu = __gtkglut_get_menu_by_id(menu_id);

	if (!glut_menu) {
		__gtkglut_warning("__gtkglut_callback_menu_deactivate_handler",
				  "called deactivate menu from nonexisting menu %d", menu_id);
	} else {
		window_id = glut_menu->popuped_from_window_id;

		if (!window_id) {
			/*This means, menu is submenu */
		} else {
			glut_window = __gtkglut_get_window_by_id(window_id);

			if (!glut_window) {
				__gtkglut_warning("__gtkglut_callback_menu_deactivate_handler",
						  "called deactivate menu on nonexisting window %d",
						  window_id);
			} else {
				gdk_window_get_pointer(glut_window->drawing_area->window, &mouse_x,
						       &mouse_y, NULL);

				__gtkglut_call_menu_status_func(GLUT_MENU_NOT_IN_USE, mouse_x,
								mouse_y, window_id, menu_id);

				__gtkglut_set_menu_in_use(glut_menu, FALSE);

				glut_menu->popuped_from_window_id = 0;
			}
		}
	}

}

/*!
 * \brief    Create a new menu.
 * \ingroup  menus
 * \param    func    Client hook for the new menu.
 *
 * Create a menu with a callback bound to \a callback, and
 * return the menu's integer id.
 *
 * When the user makes a selection from this menu,
 * \a callback is invoked with the parameter \a value,
 * which comes from the (\a label, \a value) pair that
 * is defined with glutAddMenuEntry().
 *
 * \see glutAddMenuEntry(), glutAddSubMenu(), glutDestroyMenu()
 */
int glutCreateMenu(void (*func) (int value))
{
	int menu_id;
	__gtkglut_menu_struct *glut_menu;

	__gtkglut_test_inicialization("glutCreateMenu");

	if (!func) {
		__gtkglut_error("glutCreateMenu", "can't register empty (NULL) callback for menu!");

		return -1;
	}

	glut_menu = (__gtkglut_menu_struct *) malloc(sizeof(__gtkglut_menu_struct));
	if (!glut_menu)
		__gtkglut_lowmem();

	glut_menu->func = func;
	glut_menu->cached_gtk_menu = NULL;
	glut_menu->child_items = NULL;
	glut_menu->in_use = FALSE;
	glut_menu->popuped_from_window_id = 0;
	glut_menu->need_refresh = TRUE;
	glut_menu->parent_menus = NULL;
	glut_menu->user_data = NULL;

	__gtkglut_menu_array_append(glut_menu);

	menu_id = __gtkglut_context->menu_array->len - 1;

	glutSetMenu(menu_id);

	return menu_id;
}

/*!
 * \brief    Destroy a menu.
 * \ingroup  menus
 * \param    menu    The menu to destroy.
 *
 * Destroys a given menu object by its ID.
 *
 * \see glutCreateMenu()
 */
void glutDestroyMenu(int menu)
{
	__gtkglut_menu_struct *glut_menu;
	GtkWidget *widget_to_destroy;
	GList *elem;

	__gtkglut_test_inicialization("glutDestroyMenu");

	glut_menu = __gtkglut_get_menu_by_id(menu);

	if (glut_menu == NULL) {
		__gtkglut_warning("glutDestroyMenu", "trying destroy nonexistent menu %d", menu);
		return;
	}

	if (glut_menu->in_use) {
		__gtkglut_warning("glutDestroyMenu", "trying destroy menu %d which is in use",
				  menu);
		return;
	}

	if (glut_menu) {
		widget_to_destroy = glut_menu->cached_gtk_menu;

		/*
		 * Destroy widget
		 */
		if (widget_to_destroy) {
			gtk_widget_destroy(widget_to_destroy);
		}

		/*
		 * Destroy all childs data
		 */
		elem = glut_menu->child_items;

		while (elem) {
			/*
			 * Destroy child
			 */
			free(elem->data);
			elem = g_list_next(elem);
		}

		/*
		 * All childs data are destroyed, destroy list
		 */
		g_list_free(glut_menu->child_items);

		/*
		 * Unlink from array, destroy data structure
		 */
		g_ptr_array_index(__gtkglut_context->menu_array, menu) = NULL;

		if (glut_menu->parent_menus)
			__gtkglut_refset_destroy(glut_menu->parent_menus);

		free(glut_menu);

		if (menu == glutGetMenu()) {
			__gtkglut_context->current_menu = 0;
		}
	}
}

/*
 * GTK callback for activate menu item
 */
void __gtkglut_callback_menu_activate_handler(GtkMenuItem * menu_item, gpointer data)
{
	__gtkglut_menu_item_struct *menu_item_data;
	__gtkglut_menu_struct *glut_menu;
	void (*callback_func) (int value);

	menu_item_data = (__gtkglut_menu_item_struct *) data;

	if (!menu_item_data->submenu_id) {
		/*
		 * Item is not submenu item
		 */
		glut_menu = __gtkglut_get_menu_by_id(menu_item_data->parent_menu_id);

		if (!glut_menu) {
			__gtkglut_warning("__gtkglut_callback_menu_activate_handler",
					  "function callback on nonexisting menu %d",
					  menu_item_data->parent_menu_id);
		} else {
			callback_func = glut_menu->func;

			if (!callback_func) {
				__gtkglut_warning("__gtkglut_callback_menu_activate_handler",
						  "callback for nonexisting (NULL) function on menu %d",
						  menu_item_data->parent_menu_id);
			} else {
				glutSetMenu(menu_item_data->parent_menu_id);
				glutSetWindow(__gtkglut_context->current_active_menu_window_id);
				callback_func(menu_item_data->value);

				__gtkglut_context->current_active_menu_window_id = -1;
			}
		}
	}
}

/*
 * Compatibility with Win32 GLUT version, where & is mnemonic prefix.
 * Convert this character to _ (which is GTK mnemonic prefix) and
 * character _ convert to __.
 */
char *__gtkglut_convert_name_with_mnemonic(char *name)
{
	char *res;
	unsigned int i;
	char *resp;

	/*
	 * We replace _ to __ -> in worst case, we need 2*strlen(name)+1 chars
	 */
	res = (char *)malloc(strlen(name) * 2 + 1);
	if (!res)
		__gtkglut_lowmem();

	resp = res;

	for (i = 0; i < strlen(name); i++) {
		if (name[i] == '_') {
			*resp = '_';
			resp++;
			*resp = '_';
			resp++;
		} else if (name[i] == '&') {
			*resp = '_';
			resp++;
		} else {
			*resp = name[i];
			resp++;
		}
	}

	*resp = '\0';

	return res;
}

/*
 * Set need_refresh for all parent menus of current menu
 */
void __gtkglut_menu_notify_parent_menus(__gtkglut_menu_struct * menu, int depth)
{
	GList *elem;
	__gtkglut_refset_item *item;
	__gtkglut_menu_struct *parent_menu;

	elem = menu->parent_menus;

	if (depth > __GTKGLUT_MAX_MENU_DEPTH) {
		__gtkglut_warning("__gtkglut_menu_notify_parent_menus",
				  "menu exceed maximal menu depth!");

		return;
	}

	while (elem) {
		item = (__gtkglut_refset_item *) elem->data;

		parent_menu = (__gtkglut_menu_struct *) item->ref;

		parent_menu->need_refresh = TRUE;

		__gtkglut_menu_notify_parent_menus(parent_menu, depth + 1);

		elem = g_list_next(elem);
	}
}

/*
 * Add menu entry. Used for glutAddMenuEntry and glutAddSubMenu
 */
__gtkglut_menu_item_struct *__gtkglut_add_menu_entry(char *func_name, char *name, int value,
						     int submenu_id)
{
	char *label;
	__gtkglut_menu_struct *glut_menu;
	__gtkglut_menu_struct *glut_sub_menu;
	__gtkglut_menu_item_struct *menu_item_data;

	__gtkglut_test_inicialization(func_name);

	glut_sub_menu = NULL;
	menu_item_data = NULL;

	glut_menu = __gtkglut_get_active_menu_with_warning(func_name);

	if (glut_menu) {
		if (glut_menu->in_use) {
			__gtkglut_warning(func_name, "trying change menu %d which is in use",
					  glutGetMenu());
			return NULL;
		}

		if (submenu_id) {
			glut_sub_menu = __gtkglut_get_menu_by_id(submenu_id);

			if (!glut_sub_menu) {
				__gtkglut_warning(func_name, "Attempted to add bogus menu %d",
						  submenu_id);

				return NULL;
			}
		}

		label = __gtkglut_convert_name_with_mnemonic(name);

		if (!label)
			__gtkglut_lowmem();

		menu_item_data =
		    (__gtkglut_menu_item_struct *) malloc(sizeof(__gtkglut_menu_item_struct));

		if (!menu_item_data)
			__gtkglut_lowmem();

		menu_item_data->label = label;
		menu_item_data->parent_menu_id = glutGetMenu();
		menu_item_data->value = value;
		menu_item_data->submenu_id = submenu_id;

		if (submenu_id) {
			/*
			 * We add submenu -> we want be notified, if submenu changes
			 */
			glut_sub_menu->parent_menus =
			    __gtkglut_refset_ref(glut_sub_menu->parent_menus, glut_menu);
		}

		glut_menu->child_items = g_list_append(glut_menu->child_items, menu_item_data);
		glut_menu->need_refresh = TRUE;
		__gtkglut_menu_notify_parent_menus(glut_menu, 0);
	}

	return menu_item_data;
}

/*
 * Change menu entry. Used for glutChangeToMenuEntry and  glutChangeToSubMenu
 */
void __gtkglut_change_menu_entry(char *func_name, int entry, char *name, int value, int submenu_id)
{
	__gtkglut_menu_struct *glut_menu;
	__gtkglut_menu_struct *glut_sub_menu, *glut_old_sub_menu;
	__gtkglut_menu_item_struct *menu_item_data;
	char *label;

	__gtkglut_test_inicialization(func_name);

	glut_sub_menu = NULL;

	glut_menu = __gtkglut_get_active_menu_with_warning(func_name);

	if (glut_menu) {
		if (glut_menu->in_use) {
			__gtkglut_warning(func_name, "trying change menu %d which is in use",
					  glutGetMenu());
			return;
		}

		/*
		 * Get item
		 */
		if (entry < 1 || entry > (gint) g_list_length(glut_menu->child_items)) {
			__gtkglut_warning(func_name, "Attempt on bogus item entry %d", entry);
		} else {
			if (submenu_id) {
				/*
				 * Test menu validity
				 */
				glut_sub_menu = __gtkglut_get_menu_by_id(submenu_id);

				if (!glut_sub_menu) {
					__gtkglut_warning(func_name,
							  "Attempt to change link to bogus menu %d",
							  submenu_id);

					return;
				}
			}

			label = __gtkglut_convert_name_with_mnemonic(name);

			if (!label)
				__gtkglut_lowmem();

			menu_item_data =
			    (__gtkglut_menu_item_struct *) g_list_nth_data(glut_menu->child_items,
									   entry - 1);

			if (menu_item_data->label)
				free(menu_item_data->label);

			menu_item_data->label = label;
			menu_item_data->value = value;

			if (menu_item_data->submenu_id) {
				/*
				 * We had registered old version -> remove
				 */
				glut_old_sub_menu =
				    __gtkglut_get_menu_by_id(menu_item_data->submenu_id);

				if (glut_old_sub_menu) {
					/*
					 * old glut sub menu exists -> unref our menu
					 */
					glut_old_sub_menu->parent_menus =
					    __gtkglut_refset_unref(glut_old_sub_menu->parent_menus,
								   glut_menu);
				}
			}

			menu_item_data->submenu_id = submenu_id;

			if (submenu_id) {
				/*
				 * We add submenu -> we want be notified, if submenu changes
				 */
				glut_sub_menu->parent_menus =
				    __gtkglut_refset_ref(glut_sub_menu->parent_menus, glut_menu);
			}

			glut_menu->need_refresh = TRUE;
			__gtkglut_menu_notify_parent_menus(glut_menu, 0);
		}
	}
}

/*!
 * \brief    Append an item to the current menu.
 * \ingroup  menus
 * \param    name    Menu item text
 * \param    value    Menu item callback value
 *
 * Inserts a given (\a label, \a value) pair
 * into the current menu.  \a label is the text displayed
 * in the menu; \a value is the identifier received
 * by the callback when the item is selected.
 *
 * The new entry is added to the end of the menu.
 *
 * \see glutAddSubMenu(), glutChangeToMenuEntry(), glutChangeToSubMenu(),
 *      glutRemoveMenuItem()
 */
void glutAddMenuEntry(char *name, int value)
{

	__gtkglut_add_menu_entry("glutAddMenuEntry", name, value, 0);
}

/*!
 * \brief    Append a submenu to the current menu.
 * \ingroup  menus
 * \param    name     Submenu item text
 * \param    menu Submenu identifier
 *
 * Attaches an existing menu as a submenu of the current menu.
 * \a label is the text used for the item in the menu.
 * \a subMenuID is the identifier of an existing menu to be
 * appended as a submenu.
 *
 * The submenu is added to the end of the menu.
 *
 * \see glutCreateMenu(), glutAddMenuEntry()
 */
void glutAddSubMenu(char *name, int menu)
{

	__gtkglut_add_menu_entry("glutAddSubMenu", name, 0, menu);
}

/*!
 * \brief    Replace a menu entry with an item.
 * \ingroup  menus
 * \param    entry    Integer position down the list
 * \param    name   Menu item text
 * \param    value   Menu item callback value
 *
 * Walks the list of the menu items and replaces
 * the numbered \a item in the list with the
 * given definition.  Except that it replaces a
 * pre-existing \a item, this function is much like
 * glutAddMenuEntry().
 *
 * \see glutAddMenuEntry(), glutAddSubMenu(),
 *      glutChangeToSubMenu(), glutRemoveMenuItem()
 */
void glutChangeToMenuEntry(int entry, char *name, int value)
{

	__gtkglut_change_menu_entry("glutChangeToMenuEntry", entry, name, value, 0);
}

/*!
 * \brief    Replace a menu entry with a submenu.
 * \ingroup  menus
 * \param    entry      Integer position down the list
 * \param    name      Submenu item text
 * \param    menu      Submenu identifier
 *
 * Walks the list of the menu items and replaces
 * the numbered \a item in the list with the
 * given submenu.
 *
 * \see glutAddMenuEntry(), glutAddSubMenu(),
 *      glutChangeToMenuEntry(), glutRemoveMenuItem()
 */
void glutChangeToSubMenu(int entry, char *name, int menu)
{

	__gtkglut_change_menu_entry("glutChangeToMenuEntry", entry, name, 0, menu);
}

/*!
 * \brief    Attach the current menu to the current window.
 * \ingroup  menus
 * \param    button    Mouse button to bind to
 *
 * Associates the \a button with the current menu
 * in the current window.
 *
 * \see glutCreateMenu(), glutSetMenu(), glutDetachMenu(),
 */
void glutAttachMenu(int button)
{
	__gtkglut_window_struct *glut_window;
	__gtkglut_menu_struct *glut_menu;

	__gtkglut_test_inicialization("glutAttachMenu");

	if (button < 0 || button >= __GTKGLUT_MAX_MOUSE_BUTTONS) {
		__gtkglut_warning("glutAttachMenu", "bad mouse button %d", button);

		return;
	}

	glut_window = __gtkglut_get_active_window_with_warning("glutAttachMenu");
	glut_menu = __gtkglut_get_active_menu_with_warning("glutAttachMenu");

	if (glut_window && glut_menu) {
		if (glut_menu->in_use) {
			__gtkglut_warning("glutAttachMenu", "trying attach menu %d which is in use",
					  glutGetMenu());
			return;
		}

		__gtkglut_register_mouse_func(glut_window);

		glut_window->mouse_button_menu[button] = glutGetMenu();
	}
}

/*!
 * \brief    Detach menu from the current window.
 * \ingroup  menus
 * \param    button    Mouse button to unbind from.
 *
 * If the given \a button has a menu bound to it,
 * this breaks the assocation.
 *
 * \see glutCreateMenu, glutDestroyMenu(), glutAttachMenu()
 */
void glutDetachMenu(int button)
{
	__gtkglut_window_struct *glut_window;
	__gtkglut_menu_struct *glut_menu;

	__gtkglut_test_inicialization("glutDetachMenu");

	if (button < 0 || button >= __GTKGLUT_MAX_MOUSE_BUTTONS) {
		__gtkglut_warning("glutDetachMenu", "bad mouse button %d", button);

		return;
	}

	glut_window = __gtkglut_get_active_window_with_warning("glutDetachMenu");

	if (glut_window) {
		glut_menu = __gtkglut_get_menu_by_id(glut_window->mouse_button_menu[button]);

		if (!glut_menu) {
			__gtkglut_warning("glutDetachMenu",
					  "trying to detach menu from button %d which is in not used",
					  button);
			return;
		}

		if (glut_menu->in_use) {
			__gtkglut_warning("glutDetachMenu", "trying attach menu %d which is in use",
					  glut_window->mouse_button_menu[button]);
			return;
		}

		/*
		 * Real detach
		 */
		__gtkglut_unregister_mouse_func(glut_window, button);

		glut_window->mouse_button_menu[button] = 0;
	}
}

/*
 * Call asociaten menu_state and menu_status functions
 */
void __gtkglut_call_menu_status_func(int status, int x, int y, int window_id, int menu_id)
{
	if (__gtkglut_context->menu_status_func) {
		glutSetWindow(window_id);
		glutSetMenu(menu_id);
		__gtkglut_context->menu_status_func(status, x, y);
	}

	if (__gtkglut_context->menu_state_func) {
		glutSetWindow(window_id);
		glutSetMenu(menu_id);
		__gtkglut_context->menu_state_func(status);
	}
}

/*!
 * \brief    Remove a given menu item.
 * \ingroup  menus
 * \param    entry    Integer position down the list
 *
 * Walks the list of the menu items and deletes
 * the numbered \a item in the list.
 *
 * \see glutAddMenuEntry(), glutAddSubMenu(),
 *      glutChangeToMenuEntry(), glutChangeToSubMenu()
 */
void glutRemoveMenuItem(int entry)
{
	__gtkglut_menu_struct *glut_menu;
	__gtkglut_menu_struct *glut_old_sub_menu;
	__gtkglut_menu_item_struct *menu_item_data;

	__gtkglut_test_inicialization("glutRemoveMenuItem");

	glut_menu = __gtkglut_get_active_menu_with_warning("glutRemoveMenuItem");

	if (glut_menu) {
		if (glut_menu->in_use) {
			__gtkglut_warning("glutRemoveMenuItem",
					  "trying change menu %d which is in use", glutGetMenu());
			return;
		}

		/*
		 * Get item
		 */
		if (entry < 1 || entry > (gint) g_list_length(glut_menu->child_items)) {
			__gtkglut_warning("glutRemoveMenuItem", "Attempt on bogus item entry %d",
					  entry);
		} else {
			menu_item_data =
			    (__gtkglut_menu_item_struct *) g_list_nth_data(glut_menu->child_items,
									   entry - 1);

			/*
			 * Remove from list
			 */
			glut_menu->child_items =
			    g_list_remove(glut_menu->child_items, menu_item_data);

			/*
			 * Free menu item data
			 */
			if (menu_item_data->label)
				free(menu_item_data->label);

			if (menu_item_data->submenu_id) {
				/*
				 * We had registered old version -> remove
				 */
				glut_old_sub_menu =
				    __gtkglut_get_menu_by_id(menu_item_data->submenu_id);

				if (glut_old_sub_menu) {
					/*
					 * old glut sub menu exists -> unref our menu
					 */
					glut_old_sub_menu->parent_menus =
					    __gtkglut_refset_unref(glut_old_sub_menu->parent_menus,
								   glut_menu);
				}
			}

			free(menu_item_data);

			glut_menu->need_refresh = TRUE;
			__gtkglut_menu_notify_parent_menus(glut_menu, 0);
		}
	}

}

/*!
 * \brief    Store menu user data pointer
 * \ingroup  menus
 * \param    data    Client pointer data
 *
 * glutSetMenuData() associates an arbitrary user
 * data pointer, \a data, with the *current menu*.
 * GtkGLUT does not interpret this pointer in any way, but
 * merely stores it for you in the menu structure.
 *
 * \see glutGetMenuData()
 */
void glutSetMenuData(void *data)
{
	__gtkglut_menu_struct *glut_menu;

	__gtkglut_test_inicialization("glutSetMenuData");

	glut_menu = __gtkglut_get_active_menu_with_warning("glutSetMenuData");

	if (glut_menu) {
		glut_menu->user_data = data;
	}
}

/*!
 * \brief    Retrieve menu user data pointer
 * \ingroup  menus
 *
 * Retrieve a previously stored user
 * data pointer from the current menu.
 *
 * \see      glutSetMenuData()
 */
void *glutGetMenuData(void)
{
	__gtkglut_menu_struct *glut_menu;
	void *res;

	res = NULL;

	__gtkglut_test_inicialization("glutGetMenuData");

	glut_menu = __gtkglut_get_active_menu_with_warning("glutGetMenuData");

	if (glut_menu) {
		res = glut_menu->user_data;
	}

	return res;
}

/*
 * Debug print of menu structure
 */
void __gtkglut_menu_debug_print(int menu)
{
	__gtkglut_menu_struct *glut_menu;
	GList *elem;
	__gtkglut_menu_item_struct *menu_item_data;
	int i;

	__gtkglut_test_inicialization("__gtkglut_menu_debug_print");

	glut_menu = __gtkglut_get_menu_by_id(menu);

	if (glut_menu == NULL) {
		__gtkglut_warning("__gtkglut_menu_debug_print",
				  "trying destroy nonexistent menu %d", menu);
		return;
	}

	if (glut_menu->in_use) {
		__gtkglut_warning("__gtkglut_menu_debug_print",
				  "trying destroy menu %d which is in use", menu);
		return;
	}

	if (glut_menu) {
		printf("*Menu debug print: [id:%d,cached:%p,use:%d,needref:%d]\n", menu,
		       glut_menu->cached_gtk_menu, glut_menu->in_use, glut_menu->need_refresh);

		/*
		 * Destroy all childs data
		 */
		elem = glut_menu->child_items;

		i = 1;
		while (elem) {
			/*
			 * Destroy child
			 */
			menu_item_data = (__gtkglut_menu_item_struct *) elem->data;
			printf("Submenu item %d [label:%s,par_m_id:%d,val:%d,subm_id:%d]\n",
			       i, menu_item_data->label, menu_item_data->parent_menu_id,
			       menu_item_data->value, menu_item_data->submenu_id);

			elem = g_list_next(elem);
			i++;
		}
	}
}

/*
 * Return valid GtkWidget, which is valid menu construed (or returned from cache) of actual
 * state of menu menu_id
 */
GtkWidget *__gtkglut_regenerate_menu(int menu_id, int depth)
{
	__gtkglut_menu_struct *glut_menu;
	__gtkglut_menu_item_struct *glut_menu_item;
	GtkWidget *gtk_menu;
	GtkWidget *gtk_sub_menu;
	GtkWidget *gtk_menu_item;
	GList *elem;

	if (depth > __GTKGLUT_MAX_MENU_DEPTH) {
		__gtkglut_warning("__gtkglut_regenerate_menu", "Menu is too deep (>%d)",
				  __GTKGLUT_MAX_MENU_DEPTH);
	}

	glut_menu = __gtkglut_get_menu_by_id(menu_id);

	if (!glut_menu) {
		__gtkglut_warning("__gtkglut_regenerate_menu", "Attempt on bogus menu %d", menu_id);

		return NULL;
	}

	if (depth == 0) {
		if (!glut_menu->need_refresh && glut_menu->cached_gtk_menu)
			return glut_menu->cached_gtk_menu;

		/*
		 * Try destroy old menu
		 */
		if (glut_menu->cached_gtk_menu) {
			gtk_widget_destroy(glut_menu->cached_gtk_menu);
		}
	}

	gtk_menu = gtk_menu_new();

	if (depth == 0) {
		g_signal_connect(G_OBJECT(gtk_menu), "deactivate",
				 G_CALLBACK(__gtkglut_callback_menu_deactivate_handler),
				 GINT_TO_POINTER(menu_id));
	}

	/*
	 * Add all childs
	 */
	elem = glut_menu->child_items;

	while (elem) {
		glut_menu_item = (__gtkglut_menu_item_struct *) elem->data;

		gtk_menu_item = gtk_menu_item_new_with_mnemonic(glut_menu_item->label);
		gtk_menu_shell_append(GTK_MENU_SHELL(gtk_menu), gtk_menu_item);

		if (glut_menu_item->submenu_id) {
			gtk_sub_menu =
			    __gtkglut_regenerate_menu(glut_menu_item->submenu_id, depth + 1);

			if (gtk_sub_menu) {
				gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_menu_item),
							  gtk_sub_menu);
			} else {
				__gtkglut_warning("__gtkglut_regenerate_menu",
						  "Attempt on bogus submenu %d",
						  glut_menu_item->submenu_id);
			}
		} else {
			g_signal_connect(G_OBJECT(gtk_menu_item), "activate",
					 G_CALLBACK(__gtkglut_callback_menu_activate_handler),
					 glut_menu_item);
		}
		gtk_widget_show(gtk_menu_item);

		elem = g_list_next(elem);
	}

	if (depth == 0) {
		glut_menu->need_refresh = FALSE;
		glut_menu->cached_gtk_menu = gtk_menu;
	}

	return gtk_menu;
}
