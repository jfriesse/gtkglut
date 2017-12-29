/*!
    \file  gtkglut_window.c
    \brief Window management methods.
*/

/* 
 * Window management methods.
 *
 * Copyright (c) 2008 Jan Friesse. All Rights Reserved.
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

/*Create GTK drawing area with OpenGL context asociated*/
GtkWidget *__gtkglut_create_drawing_area(char *func_name) {
  GtkWidget *drawing_area;
  gboolean direct_render;
  gboolean res;                 
  gboolean is_rgba;
  
  if (__gtkglut_context->actual_gl_config==NULL) {
    __gtkglut_error(func_name,"No appropriate OpenGL-capable visual found.");
  }

  drawing_area=gtk_drawing_area_new();

  direct_render=(__gtkglut_context->direct_render==__GTKGLUT_DIRECT_RENDER_INDIRECT)?FALSE:TRUE;

  is_rgba = gdk_gl_config_is_rgba (__gtkglut_context->actual_gl_config);

  res=gtk_widget_set_gl_capability(drawing_area,__gtkglut_context->actual_gl_config,
      NULL,direct_render,is_rgba ? GDK_GL_RGBA_TYPE : GDK_GL_COLOR_INDEX_TYPE);

  if (!res) {
    /*Maybe there was no direct render capable visual, try it with indirect*/
    res=gtk_widget_set_gl_capability(drawing_area,__gtkglut_context->actual_gl_config,
        NULL,FALSE,is_rgba ? GDK_GL_RGBA_TYPE : GDK_GL_COLOR_INDEX_TYPE);

    if (!res) {
      __gtkglut_error(func_name,"Can't create OpenGL visual.");
    }
  }

  /*Drawing area can get focus*/
  GTK_WIDGET_SET_FLAGS(drawing_area, GTK_CAN_FOCUS);  
  
  gtk_widget_add_events(drawing_area, GDK_ENTER_NOTIFY_MASK);

  return drawing_area;
}

/*Test direct rendering, and show message if user want direct rendering and we must fallback to indirect*/
void __gtkglut_test_direct_rendering(char *func_name,GtkWidget *drawing_area) {
  gboolean is_direct;
  gboolean direct_render;

  direct_render=(__gtkglut_context->direct_render==__GTKGLUT_DIRECT_RENDER_INDIRECT)?FALSE:TRUE;
  is_direct=gdk_gl_context_is_direct (gtk_widget_get_gl_context(drawing_area));

  if (!is_direct && __gtkglut_context->direct_render==__GTKGLUT_DIRECT_RENDER_NOT_SET) {
    __gtkglut_warning(func_name,"Can't create direct OpenGL visual! Decreased to slower indirect rendering.");
    __gtkglut_context->direct_render=__GTKGLUT_DIRECT_RENDER_INDIRECT;
  }
  
  if (!is_direct && __gtkglut_context->direct_render==__GTKGLUT_DIRECT_RENDER_DIRECT) {
    __gtkglut_error(func_name,"Can't create direct OpenGL visual!");
  }
}

__gtkglut_window_struct *__gtkglut_get_window_by_id(int window_id) {
  if (window_id<0 || !__gtkglut_context->window_array || (guint)window_id>=__gtkglut_context->window_array->len) {
/*    __gtkglut_error("__gtkglut_get_window_by_id","Internal error. Access window %d, which doesn't exists!",window_id);*/
    return NULL;
  }
  
  return (__gtkglut_window_struct*)g_ptr_array_index(__gtkglut_context->window_array,window_id);
} 

/*! \defgroup window Window Management

    There are two kinds of windows available using GtkGLUT:
        - Top-level windows 
        - Subwindows

    Each window has a unique identifier and OpenGL rendering context.

    This section describes the GtkGLUT API interface for creating,
    managing and closing windows.  The section \ref windowcallback
    describes the callback handlers that applications can provide to
    customise the behaviour of each window.  

    The desired position and size of a window is specified by
    glutInitWindowPosition() and glutInitWindowSize().  
    The display mode is specified by glutInitDisplayMode() or
    glutInitDisplayString()  
    including RGBA color, double buffering,
     depth buffering and stencil buffering. 
    
    Once created with glutCreateWindow() windows can be controlled using:

        - glutShowWindow() show the window
        - glutHideWindow() hide the window
        - glutIconifyWindow() iconify/minimize the window
        - glutReshapeWindow() resize the window
        - glutPositionWindow() reposition the window
        - glutFullScreen() make the window cover the entire screen
        - glutPushWindow() lower the current window in z
        - glutPopWindow() raise the current window in z
        - glutPostRedisplay() schedule a window redraw
        - glutSetCursor() set the window cursor
        - glutWarpPointer() position the window cursor
        - glutSwapBuffers() swap back buffer to the front
        
    A window is closed by calling glutDestroyWindow().

    An application may open multiple top-level windows, each with optional
    subwindows.  The <i>current</i> window is usually managed by the GtkGLUT
    event loop, but also be explicitly controlled:
    
        - glutSetWindow() set the current window
        - glutGetWindow() get the current window

    GtkGLUT also provides the ability for application data to be
    associated with each window:

        - glutGetWindowData() get the application data
        - glutSetWindowData() set the application data

    \note One important incompatibility with GLUT is, that show/hide/... operations
          are imadiately executed, there is no queue like in original GLUT!
*/

/*!
    \brief    Select the <i>current window</i>
    \ingroup  window
    \param    win       Window identifier

    Sets the <i>current window</i> to \a ID.

    All OpenGL rendering goes to the <i>current window</i>.
    Many GtkGLUT functions also implicitly use the
    <i>current window</i>.

    Many GtkGLUT callback operations are tied to a window.
    When your callback is invoked, GtkGLUT will set that
    particular window to be the <i>current window</i>.
    However, some callbacks---such as that registered via
    glutIdleFunc()---do not have associated windows.  If
    a callback is not associated to a particular window,
    then when GtkGLUT invokes that callback
    you should <b>always</b> use glutSetWindow() to
    select the appropriate window before doing any
    OpenGL rendering or doing any GtkGLUT window-related
    operations.

    There may be cases when you can get away with assuming
    that the <i>current window</i> is unchanged since some
    prior time, but GtkGLUT has considerable liberaty with
    respect to when it invokes your functions.  Also, your
    program may add more windows or more operations on other
    windows as you develop it.

    Lastly, this is a convenient way to select among
    multiple windows for drawing without actually waiting
    for that window's display callback.  Simply set the
    <i>current window</i> and draw immediately.  This is
    not always advisable, but may be practical.

    It is an error to set the <i>current window</i> to
    a non-existant window (e.g., one that you have closed).
    A warning will be printed on \a stderr if you
    try to do so, and the <i>current window</i> should be
    unchanged.

    \see glutGetWindow()
*/
void glutSetWindow(int win) {
  GdkGLContext *gl_context;
  GdkGLDrawable *gl_drawable;
  __gtkglut_window_struct *glut_window;
  GtkWidget *drawing_area;
  
  __gtkglut_test_inicialization("glutSetWindow");
  
  if (win<1 || (guint)win>=__gtkglut_context->window_array->len) {
    __gtkglut_warning("glutSetWindow","Attempted on bogus window %d!",win);
    return ;
  }

  __gtkglut_context->current_window=win;

  glut_window=__gtkglut_get_window_by_id(win);

  if (!glut_window) {
    __gtkglut_warning("glutSetWindow","You are trying set window to nonexistent window %d!",win);
    
    return ;
  }

  drawing_area=glut_window->drawing_area;
    
  if (__gtkglut_context->current_gl_drawable) {
    gdk_gl_drawable_gl_end(__gtkglut_context->current_gl_drawable);
  }
    
  gl_context=gtk_widget_get_gl_context(drawing_area);
  gl_drawable=gtk_widget_get_gl_drawable(drawing_area);

  if (!gdk_gl_drawable_gl_begin (gl_drawable, gl_context)) {
    __gtkglut_warning("glutSetWindow","Can't begin draw on GL %d!",win);
    return ;
  }

  __gtkglut_context->current_gl_drawable=gl_drawable;
  __gtkglut_context->current_gl_context=gl_context;
}

/*!
    \brief    Return the current window identifier, 0 if undefined
    \ingroup  window

    glutGetWindow() returns the <i>window id</i> of the
    <i>current window</i>.  This is useful, e.g., if you have
    a generic function that is used with several windows and
    it needs to temporarily change to another window.
    (There is no window stack for you to use with pushes and
    pops.  Do not be confused by glutPushWindow() and glutPopWindow();
    those pushes and pops are <b>not</b> stack-related!)

    One cause for the function to return 0 is if you have
    called glutDestroyWindow() on the <i>current window</i> and have
    done nothing to set a new window as current.

    \see glutSetWindow()
*/
int glutGetWindow(void) {
  int res;
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutGetWindow");

  glut_window=__gtkglut_get_window_by_id(__gtkglut_context->current_window);

  res=(glut_window==NULL)?0:__gtkglut_context->current_window;

  return res;
}

/*Destroy gtkglut window with id win, and all childs. GTK window is not destroyed!*/
void __gtkglut_destroy_glut_window(int win) {
  GSList  *elem;
  __gtkglut_window_struct *glut_window;

  glut_window=__gtkglut_get_window_by_id(win);

  if (glut_window==NULL) {
    __gtkglut_error("internal error","trying destroy nonexistent window %d",win);
  }
  
  if (glutGetWindow()==win) {
    /*if we destroy window in use -> change gl context,drawable*/
    __gtkglut_context->current_window=0;
    __gtkglut_context->current_gl_drawable=NULL;
    __gtkglut_context->current_gl_context=NULL;
  }

  g_ptr_array_index(__gtkglut_context->window_array,win)=NULL;

  if (glut_window->child_list) {
    elem=glut_window->child_list;
  
    while (elem) {
      /*Destroy child*/
      __gtkglut_destroy_glut_window(GPOINTER_TO_INT(elem->data));
      elem=g_slist_next(elem);
    }

    /*All childs are destroyed*/
    g_slist_free(glut_window->child_list);
  }

  free(glut_window);
}

/*Destroy gtkglut window (by calling __gtkglut_destroy_glut_window) and GTK+ window*/
void __gtkglut_destroy_window(char *func_name,int win) {
  __gtkglut_window_struct *glut_window;
  GtkWidget *widget_to_destroy;
  __gtkglut_window_struct *parent_glut_window;
  
  __gtkglut_test_inicialization(func_name);

  glut_window=__gtkglut_get_window_by_id(win);

  if (glut_window==NULL) {
    __gtkglut_warning(func_name,"trying destroy nonexistent window %d",win);
    return;
  }
  
  if (glut_window) {
    if (glut_window->parent_window<0) {
      widget_to_destroy=glut_window->window;
      __gtkglut_context->no_active_toplevel_windows--;

      if (glut_window->callback_close) {
        glutSetWindow(win);
        glut_window->callback_close();
      }
    } else {
      widget_to_destroy=glut_window->event_box;
    }

    if (glut_window->parent_window>0) {
      /*This is subwindow -> remove from parent window list*/
      parent_glut_window=__gtkglut_get_window_by_id(glut_window->parent_window);

      if (!parent_glut_window) {
        __gtkglut_warning("__gtkglut_destroy_glut_window","attempt on bogus window %d",glut_window->parent_window);
      } else {
        /*remove us from parent's child_list*/
        parent_glut_window->child_list=g_slist_remove(parent_glut_window->child_list,GINT_TO_POINTER(win));
      }
    }

    __gtkglut_destroy_glut_window(win);

    gtk_widget_destroy (widget_to_destroy);
  }
}

/*Destroy all glut windows and GTK+ windows*/
void __gtkglut_destroy_all_windows(void) {
  unsigned int i;
  __gtkglut_window_struct *glut_window;
  
  __gtkglut_test_inicialization("__gtkglut_destroy_all_windows");

  for (i=0;i<__gtkglut_context->window_array->len;i++) {
    glut_window=__gtkglut_get_window_by_id(i);

    if (glut_window) {
      __gtkglut_destroy_window("__gtkglut_destroy_all_windows",i);
    }
  }
}

/*!
    \brief    Destroy a window and associated subwindows
    \ingroup  window
    \param    win   Window identifier

    After this function is invoked, the only further event
    that may be delivered for your window is the one for its
    destruction.  All other events should be discarded.

    Once a window has been destroyed, further attempts to
    use the window named by \a win are undefined.  GtkGLUT generally
    tries to be sensible, and should not recycle the dead
    \a win, but you should treat a destroyed window much
    like a pointer to deallocated memory and try not to use it.

    \see      glutCreateWindow()
*/
void glutDestroyWindow(int win) {
  __gtkglut_destroy_window("glutDestroyWindow",win);

  if (__gtkglut_context->no_active_toplevel_windows<1) {
    /*User destroy all windows*/
    if (__gtkglut_context->action_on_window_close==GLUT_ACTION_EXIT) {
      /*Should be next line, but breaks test8*/
      /*exit(0);*/
      
    } else {
      gtk_main_quit();
    }
  }
}

/*Return active gtkglut window. If no window is active, show warning and return NULL*/
__gtkglut_window_struct *__gtkglut_get_active_window_with_warning(char *func_name) {
  int window_id;
  __gtkglut_window_struct *glut_window;

  window_id=glutGetWindow();

  glut_window=__gtkglut_get_window_by_id(window_id);

  if (!glut_window) {
    __gtkglut_warning(func_name,"attempted on bogus window %d!",window_id);
  }

  return glut_window;
}

/*Append window glut_window to global windows array*/
void __gtkglut_window_array_append(__gtkglut_window_struct *glut_window) {
  if (!__gtkglut_context->window_array) {
    __gtkglut_context->window_array=g_ptr_array_new();

    if (!__gtkglut_context->window_array)
      __gtkglut_lowmem();

    /*Because GLUT start numbering from 1, we do this same way*/
    g_ptr_array_add(__gtkglut_context->window_array,NULL);
  }

  g_ptr_array_add(__gtkglut_context->window_array,glut_window);
}

/*GTK+ event called, when size of eventbox widget is changed*/
void  __gtkglut_eventbox_size_allocate_event(GtkWidget *widget,GtkAllocation *allocation, gpointer data) {
  int width; 
  int height;
  int window_id;
  __gtkglut_window_struct *glut_window;
  
  width=allocation->width;
  height=allocation->height;
  window_id=GPOINTER_TO_INT(data);
  glut_window=__gtkglut_get_window_by_id(window_id);

  gtk_widget_set_size_request(glut_window->drawing_area,width,height);
}

/*Call reshape function associated with window on reshape, or default handler glViewport(0,0,width,height)
  if no function is associated*/
void __gtkglut_callback_reshape_call(__gtkglut_window_struct *glut_window,int width,int height) {
  glut_window->damaged=TRUE;

  if (glut_window->callback_reshape) {
    glut_window->callback_reshape(width,height);
  } else {
    glViewport(0,0,width,height);
  }

  __gtkglut_gl_debug();
}

/*GTK+ expose handler for drawingarea*/
static gboolean __gtkglut_callback_expose_handler(GtkWidget *widget,GdkEventExpose *event,gpointer data) {
  int width; 
  int height;
  int window_id;
  __gtkglut_window_struct *glut_window;

  window_id=GPOINTER_TO_INT(data);
  
  glut_window=__gtkglut_get_window_by_id(window_id);
  
  if (!glut_window->callback_display) {
    __gtkglut_error("glut display callback","You don't set display callback for window %d",window_id);
  }
  
  if (!glut_window->first_displayed) {
    glut_window->first_displayed=TRUE;

    width=widget->allocation.width;
    height=widget->allocation.height;
    
    glutSetWindow(window_id);
      
    __gtkglut_callback_reshape_call(glut_window,width,height);
  }
  
  glutSetWindow(window_id);

  glut_window->callback_display();
  
  __gtkglut_gl_debug();

  glut_window->damaged=FALSE;
  
  return TRUE;
} 

/*Handler for reshape event, if drawingarea size is changed*/
void  __gtkglut_callback_reshape_handler(GtkWidget *widget,GtkAllocation *allocation, gpointer data) {
  int width; 
  int height;
  int window_id;
  __gtkglut_window_struct *glut_window;
  
  width=allocation->width;
  height=allocation->height;
  window_id=GPOINTER_TO_INT(data);
  glut_window=__gtkglut_get_window_by_id(window_id);
  
  glutSetWindow(window_id);

  __gtkglut_callback_reshape_call(glut_window,width,height);
}

/*Change window visibility status and call asociated function*/
void __gtkglut_change_window_status(int window_id,gboolean visible,int status,gboolean recurse) {
  gboolean call_visibility_callback;
  gboolean call_status_callback;
  int callback_value;
  __gtkglut_window_struct *glut_window;
  GSList *elem;

  call_visibility_callback=call_status_callback=FALSE;
  
  glut_window=__gtkglut_get_window_by_id(window_id);

  if (!glut_window) {
    __gtkglut_warning("__gtkglut_change_window_visibility","Internal error. Attempt on bogus window %d",window_id); 
  } else {
    if (visible) {
      if (!glut_window->window_visible || glut_window->window_visible==__GTKGLUT_UNDEFINED)
        call_visibility_callback=TRUE;
    
      glut_window->window_visible=TRUE;
      callback_value=GLUT_VISIBLE;
    } else {
      if (glut_window->window_visible)
        call_visibility_callback=TRUE;

      glut_window->window_visible=FALSE;
      callback_value=GLUT_NOT_VISIBLE;
    }

    if (glut_window->window_status!=status && status!=__GTKGLUT_UNDEFINED) {
      glut_window->window_status=status;
      callback_value=status;
      call_status_callback=TRUE;
    }
    
    if (glut_window->signal_visibility.glut_callback_handler && call_visibility_callback) {
      glutSetWindow(window_id);

      ((void (*)(int state))glut_window->signal_visibility.glut_callback_handler)(callback_value);

      __gtkglut_gl_debug();
    }

    if (glut_window->signal_status.glut_callback_handler && call_status_callback) {
      glutSetWindow(window_id);

      ((void (*)(int state))glut_window->signal_status.glut_callback_handler)(callback_value);

      __gtkglut_gl_debug();
    }

    if (recurse && glut_window->child_list) {
      elem=glut_window->child_list;
      while (elem) {
        /*Change visibility of child*/
        __gtkglut_change_window_status(GPOINTER_TO_INT(elem->data),visible,status,recurse);
        elem=g_slist_next(elem);
      }
    }

  }
}

/*GTK+ event handler for toplevel window, if state for window changes*/
gboolean __gtkglut_window_state_handler(GtkWidget *widget, GdkEventWindowState *event, gpointer data)  {
  int window_id;
  __gtkglut_window_struct *glut_window;
  
  window_id=GPOINTER_TO_INT(data);

  if (event->changed_mask & GDK_WINDOW_STATE_WITHDRAWN) {
    __gtkglut_change_window_status(window_id,!(event->new_window_state& GDK_WINDOW_STATE_WITHDRAWN),
        (event->new_window_state& GDK_WINDOW_STATE_WITHDRAWN)?GLUT_HIDDEN:GLUT_FULLY_RETAINED,FALSE);
  }

  if (event->changed_mask & GDK_WINDOW_STATE_ICONIFIED) {
    __gtkglut_change_window_status(window_id,!(event->new_window_state& GDK_WINDOW_STATE_ICONIFIED),
        (event->new_window_state& GDK_WINDOW_STATE_ICONIFIED)?GLUT_HIDDEN:__GTKGLUT_UNDEFINED,TRUE);

    glut_window=__gtkglut_get_window_by_id(window_id);

    if (!glut_window) {
      __gtkglut_warning("__gtkglut_window_state_handler","Internal error. Attempt on bogus window %d",window_id); 
    } else {
      glut_window->iconified=(event->new_window_state& GDK_WINDOW_STATE_ICONIFIED)?TRUE:FALSE;
    }
  
  }
  return TRUE;
}

/*GTK+ visibility handler. Called when visibility of window is changed*/
gboolean __gtkglut_callback_visibility_notify_handler(GtkWidget *widget,GdkEventVisibility *event,gpointer data) {
  int window_id;
  int status;
  
  window_id=GPOINTER_TO_INT(data);
  
  status=GLUT_FULLY_COVERED;

  switch (event->state) {
    case GDK_VISIBILITY_FULLY_OBSCURED: status=GLUT_FULLY_COVERED;break;  
    case GDK_VISIBILITY_PARTIAL: status=GLUT_PARTIALLY_RETAINED; break;
    case GDK_VISIBILITY_UNOBSCURED: status=GLUT_FULLY_RETAINED;break; 
  }
  
  __gtkglut_change_window_status(window_id,!(event->state& GDK_VISIBILITY_FULLY_OBSCURED),status,FALSE);
  
  return TRUE;
}

/*GTK+ handler for toplevel close event (user click close button, mostly "x")*/
gboolean __gtkglut_toplevel_window_delete_event(GtkWidget *widget,GdkEvent  *event, gpointer data) {
  int window_id;
  __gtkglut_window_struct *glut_window;
  
  window_id=GPOINTER_TO_INT(data);

  glut_window=__gtkglut_get_window_by_id(window_id);

  if (glut_window) {
    /*Handler invoked by user (click on close button?), action depends on choosen action_on_window_close */

    switch (__gtkglut_context->action_on_window_close) {
      case GLUT_ACTION_EXIT:
        __gtkglut_destroy_all_windows();

        exit(0);
      break;

      case GLUT_ACTION_GLUTMAINLOOP_RETURNS:
        __gtkglut_destroy_window("__gtkglut_toplevel_window_delete_event",window_id);
        
        gtk_main_quit();
      break;

      case GLUT_ACTION_CONTINUE_EXECUTION:
        __gtkglut_destroy_window("__gtkglut_toplevel_window_delete_event",window_id);

        if (__gtkglut_context->no_active_toplevel_windows<1) {
          gtk_main_quit();
        }
      break;
    }
  }

  return TRUE;
}

/*GTK+ key handler*/
gboolean __gtkglut_callback_key_handler(GtkWidget *widget,GdkEventKey *event,gpointer data) {
  int window_id;
  void (*special_func)(int key, int x, int y);
  void (*keyboard_func)(unsigned char key,int x, int y);
  __gtkglut_window_struct *glut_window;
  gint mouse_x,mouse_y;
  int special_key;
  /*GTK event->string handles almost everything but ... there is problem with Tab, Delete and Backspace key.
    This variable hold 0 if user didn't press this keys, otherwise code for Tab, Delete or Backspace.*/
  char tbd_key; 
    
  window_id=GPOINTER_TO_INT(data);

  glut_window=__gtkglut_get_window_by_id(window_id);

  if (!glut_window) {
    __gtkglut_warning("__gtkglut_callback_keypress_handler","Internal error. Attempt on bogus window %d",window_id); 
  } else {
    if (event->type==GDK_KEY_PRESS) {
      special_func=((void (*)(int key, int x, int y))glut_window->signal_keyboard_special.glut_callback_handler);
      keyboard_func=((void (*)(unsigned char key,int x, int y))glut_window->signal_keyboard.glut_callback_handler);
    } else {
      special_func=((void (*)(int key, int x, int y))glut_window->signal_keyboard_special_up.glut_callback_handler);
      keyboard_func=((void (*)(unsigned char key,int x, int y))glut_window->signal_keyboard_up.glut_callback_handler);
    }

    if (keyboard_func || special_func) {
      gdk_window_get_pointer (glut_window->drawing_area->window,&mouse_x,&mouse_y,NULL);

      tbd_key=0;
      
      if (event->length<=0) {
        /*Handle tbd key*/
        switch (event->keyval) {
          case GDK_Delete: case GDK_KP_Delete: tbd_key=127;break;
          case GDK_Tab: case GDK_KP_Tab: tbd_key=9;break;
          case GDK_BackSpace: tbd_key=8;break;
        }
      }
      
      /*We use deprecated code, but ... this is only possibility how to do this*/
      if (event->length>0 || tbd_key) {
        if (keyboard_func) {
          __gtkglut_context->keyboard_modifiers=event->state;
          glutSetWindow(window_id);

          if (tbd_key)
            keyboard_func(tbd_key,mouse_x,mouse_y);
          else
            keyboard_func(event->string[0],mouse_x,mouse_y);
          
          __gtkglut_context->keyboard_modifiers=__GTKGLUT_UNDEFINED;
        }
      } else {
        /*Special code*/
        special_key=-1;

        switch (event->keyval) {
          case GDK_F1:case GDK_KP_F1: special_key=GLUT_KEY_F1;break;
          case GDK_F2:case GDK_KP_F2: special_key=GLUT_KEY_F2;break;
          case GDK_F3:case GDK_KP_F3: special_key=GLUT_KEY_F3;break;
          case GDK_F4:case GDK_KP_F4: special_key=GLUT_KEY_F4;break;
          case GDK_F5: special_key=GLUT_KEY_F5;break;
          case GDK_F6: special_key=GLUT_KEY_F6;break;
          case GDK_F7: special_key=GLUT_KEY_F7;break;
          case GDK_F8: special_key=GLUT_KEY_F8;break;
          case GDK_F9: special_key=GLUT_KEY_F9;break;
          case GDK_F10: special_key=GLUT_KEY_F10;break;
          case GDK_F11: special_key=GLUT_KEY_F11;break;
          case GDK_F12: special_key=GLUT_KEY_F12;break;
            
          case GDK_Left:case GDK_KP_Left: special_key=GLUT_KEY_LEFT;break;
          case GDK_Right:case GDK_KP_Right: special_key=GLUT_KEY_RIGHT;break;
          case GDK_Up:case GDK_KP_Up: special_key=GLUT_KEY_UP;break;
          case GDK_Down:case GDK_KP_Down: special_key=GLUT_KEY_DOWN;break;

          case GDK_Prior:case GDK_KP_Prior: special_key=GLUT_KEY_PAGE_UP;break;
          case GDK_Next:case GDK_KP_Next: special_key=GLUT_KEY_PAGE_DOWN;break;
          case GDK_Home:case GDK_KP_Home: special_key=GLUT_KEY_HOME;break;
          case GDK_End:case GDK_KP_End: special_key=GLUT_KEY_END;break;
          case GDK_Insert:case GDK_KP_Insert: special_key=GLUT_KEY_INSERT;break;
        }

        if (special_func && special_key!=-1) {
          __gtkglut_context->keyboard_modifiers=event->state;
          glutSetWindow(window_id);
          special_func(special_key,mouse_x,mouse_y);
          __gtkglut_context->keyboard_modifiers=__GTKGLUT_UNDEFINED;
        }
      }
    }
  }
  
  __gtkglut_gl_debug();

  return TRUE;
}

/*GTK+ mouse handler*/
gboolean __gtkglut_callback_mouse_press_handler(GtkWidget *widget,GdkEventButton *event,gpointer data) {
  int window_id;
  int menu_id;
  __gtkglut_window_struct *glut_window;
  void (*callback_func)(int button, int state,int x, int y);
  int button;
  int state;
   __gtkglut_menu_struct *glut_menu;
  GtkWidget *gtk_menu;
  
  window_id=GPOINTER_TO_INT(data);

  glut_window=__gtkglut_get_window_by_id(window_id);

  if (!glut_window) {
    __gtkglut_warning("__gtkglut_callback_mouse_press","Internal error. Attempt on bogus window %d",window_id); 
  } else {
    button=__GTKGLUT_UNDEFINED;
      
    switch (event->button) {
      case 1: button=GLUT_LEFT_BUTTON;break;
      case 2: button=GLUT_MIDDLE_BUTTON;break;
      case 3: button=GLUT_RIGHT_BUTTON;break;
      case 4: button=GLUT_FOURTH_BUTTON;break;
      case 5: button=GLUT_FIFTH_BUTTON;break;
    }

    state=__GTKGLUT_UNDEFINED;
      
    switch (event->type) {
      case GDK_BUTTON_PRESS: state=GLUT_DOWN;break;
      case GDK_BUTTON_RELEASE: state=GLUT_UP;break;
      default: state=__GTKGLUT_UNDEFINED;break;
    }

    if (button!=__GTKGLUT_UNDEFINED && state!=__GTKGLUT_UNDEFINED) {
      menu_id=glut_window->mouse_button_menu[button];
      
      /*Test if window have registered menu*/
      if (menu_id) {
        /*We have menu for this button*/
        glut_menu=__gtkglut_get_menu_by_id(menu_id);

        if (!glut_menu) {
          __gtkglut_warning("__gtkglut_callback_mouse_press_handler","you have registered undefined menu on button %d",button);
        } else {
          /*Set menu in use and call callback*/
          __gtkglut_set_menu_in_use(glut_menu,TRUE);

          glut_menu->popuped_from_window_id=window_id;
          
          __gtkglut_context->current_active_menu_window_id=window_id;
          
          __gtkglut_call_menu_status_func(GLUT_MENU_IN_USE,(int)event->x,(int)event->y,window_id,menu_id);
          /*Show glut menu*/
          gtk_menu=__gtkglut_regenerate_menu(menu_id,0);

          if (gtk_menu)
            gtk_menu_popup (GTK_MENU(gtk_menu), NULL, NULL, NULL, NULL,event->button, event->time);
        }                 
      } else {
        /*Test if user have callback*/
        if (glut_window->signal_mouse_press.glut_callback_handler) {
          callback_func=(void (*)(int button, int state,int x, int y))glut_window->signal_mouse_press.glut_callback_handler;

          /*Call callback*/
          __gtkglut_context->keyboard_modifiers=event->state;
          glutSetWindow(window_id);
          callback_func(button,state,(int)event->x,(int)event->y);
          __gtkglut_context->keyboard_modifiers=__GTKGLUT_UNDEFINED;
        }
      }
    }
  }

  __gtkglut_gl_debug();

  return TRUE;
}

/*GTK+ mouse wheel handler*/
gboolean __gtkglut_callback_mouse_wheel_handler(GtkWidget *widget,GdkEventScroll *event,gpointer data) {
  int window_id;
  __gtkglut_window_struct *glut_window;
  void (*callback_func)(int wheel, int direction, int x, int y);
  int wheel;
  int direction;

  wheel=0;direction=GLUT_WHEEL_UP;

  window_id=GPOINTER_TO_INT(data);

  glut_window=__gtkglut_get_window_by_id(window_id);

  if (!glut_window) {
    __gtkglut_warning("__gtkglut_callback_mouse_wheel","Internal error. Attempt on bogus window %d",window_id); 
  } else {
    if (glut_window->signal_mouse_wheel.glut_callback_handler) {
      callback_func=(void (*)(int wheel, int direction, int x, int y))glut_window->signal_mouse_wheel.glut_callback_handler;

      switch (event->direction) {
        case GDK_SCROLL_UP:wheel=0;direction=GLUT_WHEEL_UP;break;
        case GDK_SCROLL_DOWN:wheel=0;direction=GLUT_WHEEL_DOWN;break;
        case GDK_SCROLL_LEFT:wheel=1;direction=GLUT_WHEEL_UP;break;
        case GDK_SCROLL_RIGHT:wheel=1;direction=GLUT_WHEEL_DOWN;break;
      }
      
        __gtkglut_context->keyboard_modifiers=event->state;
        glutSetWindow(window_id);
        callback_func(wheel,direction,(int)event->x,(int)event->y);
        __gtkglut_context->keyboard_modifiers=__GTKGLUT_UNDEFINED;
    }
  }

  __gtkglut_gl_debug();

  return TRUE;
}

/*GTK+ mouse motion handler*/
gboolean __gtkglut_callback_motion_handler(GtkWidget *widget,GdkEventMotion *event,gpointer data) {
  int window_id;
  __gtkglut_window_struct *glut_window;
  void (*callback_func)(int x,int y);

  window_id=GPOINTER_TO_INT(data);

  glut_window=__gtkglut_get_window_by_id(window_id);

  if (!glut_window) {
    __gtkglut_warning("__gtkglut_callback_motion","Internal error. Attempt on bogus window %d",window_id); 
  } else {
    /*Call active or passive callback*/
    if (event->state&GDK_BUTTON1_MASK || event->state&GDK_BUTTON2_MASK || event->state&GDK_BUTTON3_MASK ||
        event->state&GDK_BUTTON4_MASK || event->state&GDK_BUTTON5_MASK) {
      callback_func=(void (*)(int x,int y))glut_window->signal_motion.glut_callback_handler;
    } else {
      callback_func=(void (*)(int x,int y))glut_window->signal_passive_motion.glut_callback_handler;
    }

    if (callback_func) {
      __gtkglut_context->keyboard_modifiers=event->state;
      glutSetWindow(window_id);
      callback_func((int)event->x,(int)event->y);
      __gtkglut_context->keyboard_modifiers=__GTKGLUT_UNDEFINED;
    }
  }
  
  __gtkglut_gl_debug();

  return TRUE;  
}

/*GTK+ drawing_area mouse enter/leave handler*/
gboolean __gtkglut_callback_enter_leave_handler(GtkWidget *widget,GdkEventCrossing *event,gpointer data) {
  int window_id;
  __gtkglut_window_struct *glut_window;
  void (*callback_func)(int state);
  int state;
  
  window_id=GPOINTER_TO_INT(data);

  glut_window=__gtkglut_get_window_by_id(window_id);

  /*Grab focus*/
  gtk_widget_grab_focus(widget);

  if (!glut_window) {
    __gtkglut_warning("__gtkglut_callback_enter_leave","Internal error. Attempt on bogus window %d",window_id); 
  } else {
    /*Call active or passive callback*/
    if (event->type==GDK_ENTER_NOTIFY)
      state=GLUT_ENTERED;
    else
      state=GLUT_LEFT;

    callback_func=(void (*)(int state))glut_window->signal_entry.glut_callback_handler;

    if (callback_func) {
      glutSetWindow(window_id);
      callback_func(state);
    }
  }
  
  __gtkglut_gl_debug();

  return TRUE;  
}

/*Create window. Universal function for creating toplevel (parent<0) and sub windows*/
int __gtkglut_create_window(char *func_name,int parent,__gtkglut_geometry_struct geom,char *win_name) {
  int window_id;
  __gtkglut_window_struct *glut_window;
  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *drawing_area;
  GtkWidget *event_box;
  __gtkglut_window_struct *parent_glut_window;
  int i;
  
  window=NULL;
  parent_glut_window=NULL;

  glut_window=(__gtkglut_window_struct*)malloc(sizeof(__gtkglut_window_struct));
  if (!glut_window) 
    __gtkglut_lowmem();

  if (parent<0) {
    /*Toplevel*/
    /*Create window*/
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), win_name);

    /*Set right policy -> we can shrink window*/
    gtk_window_set_policy (GTK_WINDOW (window),TRUE,TRUE,FALSE);

    if (__gtkglut_context->window_init_iconic) {
      /*User wan't create window iconified*/
      gtk_window_iconify(GTK_WINDOW(window));
    }

    if (geom.x>=0 && geom.y>=0) {
      /*User want initial window position*/
      gtk_window_move(GTK_WINDOW(window),geom.x,geom.y);
    }

    /*Set window size*/
    gtk_window_resize(GTK_WINDOW(window),geom.width,geom.height);
  } else {
    /*Subwindow. Get parent of this window*/
    parent_glut_window=__gtkglut_get_window_by_id(parent);

    if (parent_glut_window==NULL) {
      /*Should never hapend, but sure is sure*/
      __gtkglut_error("glutCreateSubWindow","You are trying create subwindow of nonexistent window %d!",parent);
    }
  }
  
  /*Create eventbox*/
  event_box=gtk_event_box_new();
  gtk_widget_set_size_request(event_box,geom.width,geom.height);
  
  /*Create fixed*/
  fixed=gtk_fixed_new();

  /*Create drawing area*/
  drawing_area= __gtkglut_create_drawing_area(func_name);

  gtk_widget_set_size_request(drawing_area,geom.width,geom.height);

  /*Put items to right place*/
  gtk_fixed_put(GTK_FIXED(fixed),drawing_area,0,0);
  gtk_container_add(GTK_CONTAINER(event_box),fixed);

  if (parent<0) {
    /*Toplevel*/
    gtk_container_add(GTK_CONTAINER(window),event_box);
  } else {
    gtk_fixed_put(GTK_FIXED(parent_glut_window->fixed),event_box,geom.x,geom.y);
  }

  /*Show all widgets*/
  gtk_widget_show(drawing_area);
  gtk_widget_show(fixed);
  gtk_widget_show(event_box);
  
  if (parent<0) {
    /*Toplevel*/
    gtk_widget_show(window);
  }

  __gtkglut_test_direct_rendering(func_name,drawing_area);

  /*Fill glut_window struct*/
  glut_window->window=window;
  glut_window->event_box=event_box;
  glut_window->parent_window=parent;
  glut_window->drawing_area=drawing_area;
  glut_window->fixed=fixed;
  glut_window->first_displayed=FALSE;
  glut_window->callback_display=NULL;
  glut_window->callback_reshape=NULL;
  glut_window->fullscreen=FALSE;
  glut_window->child_list=NULL;
  glut_window->current_cursor=GLUT_CURSOR_INHERIT;
  glut_window->damaged=TRUE;
  glut_window->iconified=(parent<0 && __gtkglut_context->window_init_iconic);
  glut_window->user_data=NULL;

/*  glut_window->window_visible=__GTKGLUT_UNDEFINED;
  glut_window->window_status=__GTKGLUT_UNDEFINED;*/
  glut_window->window_visible=FALSE;
  glut_window->window_status=GLUT_HIDDEN;

  glut_window->signal_visibility.gtk_event_handler=(void*)__gtkglut_callback_visibility_notify_handler;
  glut_window->signal_visibility.glut_callback_handler=NULL;
  glut_window->signal_visibility.event_added=FALSE;
  
  glut_window->signal_status.gtk_event_handler=(void*)__gtkglut_callback_visibility_notify_handler;
  glut_window->signal_status.glut_callback_handler=NULL;
  glut_window->signal_status.event_added=FALSE;

  glut_window->signal_keyboard.gtk_event_handler=(void*)__gtkglut_callback_key_handler;
  glut_window->signal_keyboard.glut_callback_handler=NULL;
  glut_window->signal_keyboard.event_added=FALSE;
  
  glut_window->signal_keyboard_special.gtk_event_handler=(void*)__gtkglut_callback_key_handler;
  glut_window->signal_keyboard_special.glut_callback_handler=NULL;
  glut_window->signal_keyboard_special.event_added=FALSE;
  
  glut_window->signal_keyboard_up.gtk_event_handler=(void*)__gtkglut_callback_key_handler;
  glut_window->signal_keyboard_up.glut_callback_handler=NULL;
  glut_window->signal_keyboard_up.event_added=FALSE;

  glut_window->signal_keyboard_special_up.gtk_event_handler=(void*)__gtkglut_callback_key_handler;
  glut_window->signal_keyboard_special_up.glut_callback_handler=NULL;
  glut_window->signal_keyboard_special_up.event_added=FALSE;

  glut_window->signal_mouse_press.gtk_event_handler=(void*)__gtkglut_callback_mouse_press_handler;
  glut_window->signal_mouse_press.glut_callback_handler=NULL;
  glut_window->signal_mouse_press.event_added=FALSE;

  glut_window->signal_mouse_release.gtk_event_handler=(void*)__gtkglut_callback_mouse_press_handler;
  glut_window->signal_mouse_release.glut_callback_handler=NULL;
  glut_window->signal_mouse_release.event_added=FALSE;
  
  glut_window->signal_mouse_wheel.gtk_event_handler=(void*)__gtkglut_callback_mouse_wheel_handler;
  glut_window->signal_mouse_wheel.glut_callback_handler=NULL;
  glut_window->signal_mouse_wheel.event_added=FALSE;

  glut_window->signal_motion.gtk_event_handler=(void*)__gtkglut_callback_motion_handler;
  glut_window->signal_motion.glut_callback_handler=NULL;
  glut_window->signal_motion.event_added=FALSE;
  
  glut_window->signal_passive_motion.gtk_event_handler=(void*)__gtkglut_callback_motion_handler;
  glut_window->signal_passive_motion.glut_callback_handler=NULL;
  glut_window->signal_passive_motion.event_added=FALSE;

  glut_window->signal_entry.gtk_event_handler=(void*)__gtkglut_callback_enter_leave_handler;
  glut_window->signal_entry.glut_callback_handler=NULL;
  glut_window->signal_entry.event_added=FALSE;

  glut_window->callback_close=NULL;
  
  for (i=0;i<__GTKGLUT_MAX_MOUSE_BUTTONS;i++)
    glut_window->mouse_button_menu[i]=0;

  __gtkglut_window_array_append(glut_window);

  window_id=__gtkglut_context->window_array->len-1;

  if (parent>0) {
    /*Add us to parent's child_list*/
    parent_glut_window->child_list=g_slist_append(parent_glut_window->child_list,GINT_TO_POINTER(window_id));
  }

  /*Register clabbacks*/
  if (parent<0) {
    g_signal_connect (G_OBJECT (window), "delete_event",
                      G_CALLBACK (__gtkglut_toplevel_window_delete_event), GINT_TO_POINTER(window_id));

    g_signal_connect (G_OBJECT (window), "window-state-event",
                    G_CALLBACK (__gtkglut_window_state_handler), GINT_TO_POINTER(window_id));
  }
  
  g_signal_connect(G_OBJECT (event_box),"size_allocate",
      G_CALLBACK(__gtkglut_eventbox_size_allocate_event),GINT_TO_POINTER(window_id));

  g_signal_connect(G_OBJECT (drawing_area),"size_allocate",
      G_CALLBACK(__gtkglut_callback_reshape_handler),GINT_TO_POINTER(window_id));

  g_signal_connect (G_OBJECT (drawing_area), "expose_event",
                    G_CALLBACK (__gtkglut_callback_expose_handler), GINT_TO_POINTER(window_id));

  g_signal_connect (G_OBJECT (drawing_area), "enter-notify-event",
                    G_CALLBACK (__gtkglut_callback_enter_leave_handler), GINT_TO_POINTER(window_id));

  
  glutSetWindow(window_id);

  return window_id;
}

/*!
    \brief    Create a new top-level window
    \ingroup  window
    \param    name      Title for created window

    This function sends a request for a window to be constructed.
    GtkGLUT immediately constructs a data structure to track further
    events with the window, on the theory that eventually the window
    manager will get back to us with a real window.  This allows us
    to begin registering callbacks immediately.

    In fact, you <b>must</b> register a display callback via
    glutDisplayFunc() before you enter glutMainLoop().

    For onscreen windows, you should not depend upon the window
    concretely existing or being visibile until you are told
    that it exists and is visible via a registered callback.

    The return value is an \a int.  It should be positive for
    valid windows. If failure occurred for some reason
    error is printer to stderr and program exits. The integer is your
    <i>window id</i>.  Old GLUT promises that these integers
    are ``small''; we do not reuse old <i>id</i>s, but do
    produce them sequentially.

    The title can be changed using glutSetWindowTitle().

    \see glutDestroyWindow(), glutCreateSubWindow(), glutSetWindowTitle()
*/
int glutCreateWindow(char *name) {
  __gtkglut_geometry_struct geom;
  int window_id;

  __gtkglut_test_inicialization("glutCreateWindow");

  geom=__gtkglut_context->window_init_geometry;

  window_id=__gtkglut_create_window("glutCreateWindow",-1,geom,name);

  __gtkglut_context->no_active_toplevel_windows++;
    
  return window_id;
}

/*!
    \brief    Create a subwindow
    \ingroup  window
    \param    win        Parent window identifier
    \param    x          Horizontal position of subwindow
    \param    y          Vertical position of subwindow
    \param    width          Width of subwindow
    \param    height          Height of subwindow

    In almost every regard that is important to you, a subwindow is like
    a top-level window.  It has a window id; it has its own set of
    event callbacks; you can render to it; you are notified of its
    creation; ...

    A subwindow lives inside of some other window (possibly a top-level
    window, possibly another subwindow).  Because of this, it generally
    only interacts with other windows of your own creation, hence
    it is not subjected to a window manager.  This is the primary
    source for its differences from a top-level window:

    - There are no borders or decorations.
    - There is no title bar, hence no title.
    - Requests tend to be acted on a little more directly,
      without interference from a window manager.
    - The subwindow inherits the display mode of its parent.

    Like a top-level window, you <b>must</b> register a display
    callback function if you wish to use glutMainloop().

    Subwindows can be very useful for partitioning a window into
    GUI elements: They have their own input callbacks, so you
    don't have to figure out which window an event is for.
    Graphics are clipped to the boundaries of your subwindows,
    so you do not need to worry much about where your drawing
    goes.  Because windows and subwindows work almost identically
    from the perspective of a GLUT program, it is relatively easy
    to move a cluster of related controls into a separate top-level
    window---or, conversely, embed what was a top-level window
    inside of another window.  GtkGLUT can also report
    some basic statistics about your (sub)window, relieving you
    of the duty of tracking all of that information for yourself.

    \see glutCreateWindow(), glutDestroyWindow()
*/
int glutCreateSubWindow(int win,int x, int y, int width, int height) {
  __gtkglut_geometry_struct geom;
  __gtkglut_window_struct *parent_glut_window;
  int window_id;

  __gtkglut_test_inicialization("glutCreateSubWindow");
  
  parent_glut_window=__gtkglut_get_window_by_id(win);

  if (!parent_glut_window) {
    __gtkglut_error("glutCreateSubWindow","You are trying create subwindow of nonexistent window %d!",win);
  }

  if (width<1 || height<1) {
    __gtkglut_error("glutCreateSubWindow","non-positive width or height not allowed");
  }

  geom.x=x;
  geom.y=y;
  geom.width=width;
  geom.height=height;

  window_id=__gtkglut_create_window("glutCreateSubWindow",win,geom,NULL);
  
  return window_id;
}

/*!
    \brief    Swaps the buffers for the current window.
    \ingroup  window

              This function signals to GtkGLUT that you are done drawing
              to the <i>current window</i> for now.  If your window is
              double-buffered (\a GLUT_DOUBLE param to glutInitDisplayMode()),
              then GtkGLUT will swap the front buffer with the back buffer.

    \note     Function call glFlush automaticly. For single buffer this is only
              one effect, so you can use always this function.
    \see      glutPostRedisplay(), glutPostOverlayRedisplay(),
              glutPostWindowRedisplay(), glutPostWindowOverlayRedisplay(),
              glutInitDisplaymode()
*/
void glutSwapBuffers(void) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutSwapBuffers");

  glut_window=__gtkglut_get_active_window_with_warning("glutSwapBuffers");

  if (glut_window) {
    /*Default is call glFlush before swap*/
    glFlush();

    if (gdk_gl_drawable_is_double_buffered(__gtkglut_context->current_gl_drawable)) {
      gdk_gl_drawable_swap_buffers(__gtkglut_context->current_gl_drawable);
    }
  }
}

/*Create bitmap for empty cursor*/
void __gtkglut_create_cursor_none(void) {
  gchar bits[] = { 0 };
  GdkColor color = { 0, 0, 0, 0 };
  GdkPixmap *pixmap;
  GdkCursor *cursor;

  pixmap = gdk_bitmap_create_from_data(NULL, bits, 1, 1);
  cursor = gdk_cursor_new_from_pixmap(pixmap, pixmap, &color, &color, 0, 0);
  
  __gtkglut_context->cursor_none=cursor;
}

/*!
    \brief    Set the cursor image to be used for the current window
    \ingroup  window
    \param    cursor   Name of desired cursor.

              For the <i>current window</i>, sets the mouse-cursor to
              one of a set of predefined images.  The GLUT symbolic constant
              IDs are:

               - \a GLUT_CURSOR_RIGHT_ARROW
               - \a GLUT_CURSOR_LEFT_ARROW
               - \a GLUT_CURSOR_INFO
               - \a GLUT_CURSOR_DESTROY
               - \a GLUT_CURSOR_HELP
               - \a GLUT_CURSOR_CYCLE
               - \a GLUT_CURSOR_SPRAY
               - \a GLUT_CURSOR_WAIT
               - \a GLUT_CURSOR_TEXT
               - \a GLUT_CURSOR_CROSSHAIR
               - \a GLUT_CURSOR_UP_DOWN
               - \a GLUT_CURSOR_LEFT_RIGHT
               - \a GLUT_CURSOR_TOP_SIDE
               - \a GLUT_CURSOR_BOTTOM_SIDE
               - \a GLUT_CURSOR_LEFT_SIDE
               - \a GLUT_CURSOR_RIGHT_SIDE
               - \a GLUT_CURSOR_TOP_LEFT_CORNER
               - \a GLUT_CURSOR_TOP_RIGHT_CORNER
               - \a GLUT_CURSOR_BOTTOM_RIGHT_CORNER
               - \a GLUT_CURSOR_BOTTOM_LEFT_CORNER

              Additionally, there are the following special cases:

              \a GLUT_CURSOR_FULL_CROSSHAIR This cursor, where supported,
              draws a crosshair the full width and height of the display.
              It may be mapped by GtkGLUT to the \a GLUT_CURSOR_CROSSHAIR,
              however.

              \a GLUT_CURSOR_NONE Turn the mouse cursor invisibile.

              \a GLUT_CURSOR_INHERIT Take the cursor that the parent
              window provides.
    \note     GtkGLUT always converts \a FULL_CROSSHAIR
              to \a CROSSHAIR.
              This is acceptable, but if a host system supports a fullscreen
              crosshair, it would be nice to support that.
    \note     Out of range \a cursorID values generate warnings.
    \note     In Win32 thanks to GTK+ we have full range of cursors (this is big change from original GLUT)
*/


void glutSetCursor(int cursor) {
  __gtkglut_window_struct *glut_window;
  GtkWidget *drawing_area;
  GdkCursor *gcursor;
  int cursor_id;
  
  __gtkglut_test_inicialization("glutSetCursor");

  glut_window=__gtkglut_get_active_window_with_warning("glutSetCursor");

  if (glut_window) {
/*    drawing_area=glut_window->drawing_area;*/
    drawing_area=glut_window->event_box;

    /*Default is inherit*/
    gcursor=NULL;

    /*We don't have fullscreen cursor*/
    if (cursor==GLUT_CURSOR_FULL_CROSSHAIR) {
      cursor_id=GLUT_CURSOR_CROSSHAIR;
    } else {
      cursor_id=cursor;
    }
  
    if (cursor_id>=GLUT_CURSOR_RIGHT_ARROW && cursor_id<=GLUT_CURSOR_BOTTOM_LEFT_CORNER) {
      /*Get cursor from cache*/
      if (!__gtkglut_context->cursors_cache[cursor_id].cursor) {
        __gtkglut_context->cursors_cache[cursor_id].cursor=gdk_cursor_new((GdkCursorType)__gtkglut_context->cursors_cache[cursor_id].gdk_id);
      }
    
      gcursor=__gtkglut_context->cursors_cache[cursor_id].cursor;
    }
  
    if (cursor_id==GLUT_CURSOR_NONE) {
      /*Get cursor from cache*/
      if (!__gtkglut_context->cursor_none) {
        __gtkglut_create_cursor_none();
      }
    
      gcursor=__gtkglut_context->cursor_none;
    }
  
    if (!gcursor && cursor_id!=GLUT_CURSOR_INHERIT) {
      __gtkglut_warning("glutSetCursor","You are trying set undefined cursor %d, GLUT_CURSOR_INHERIT will be used!",cursor);
    } else {
      glut_window->current_cursor=cursor;
    }
  
    gdk_window_set_cursor(drawing_area->window,gcursor);
  }
}

/*!
    \brief    Lower the current window to the bottom of the stack
    \ingroup  window

    This function requests that the <i>current window</i> be ``pushed''
    to the back.

    A window can be in front of or behind other windows, as determined
    by the z-order from front to back.  Top-level GtkGLUT windows
    can be placed at the front or back of the z-order by means of
    the glutPopWindow() and glutPushWindow() API functions.

    A z-order also applies to the subwindows of a top-level window.
    While the z-order of top-level windows can usually be
    adjusted by the user, subwindow z-order is controlled entirely
    by the application.

    There may not be an immediate effect to this function.  Wait for
    the glutWindowStatusFunc() callback to tell you about whatever
    obscured/visible status your window achieves.

    \note    The z-order of top-level windows is ultimately managed by
             the windowing system.  Therefore, a push or pop request
             by an GtkGLUT application may not necessarily succeed
             or take immediate effect.
    \see     glutPopWindow(), glutWindowStatusFunc()
*/
void glutPushWindow(void) {
  __gtkglut_window_struct *glut_window;
  __gtkglut_window_struct *glut_parent_window;
  
  __gtkglut_test_inicialization("glutPushWindow");

  glut_window=__gtkglut_get_active_window_with_warning("glutPushWindow");
  
  if (glut_window) {
    if (glut_window->parent_window<0) {
      gdk_window_lower(glut_window->window->window);
    } else {
      gdk_window_lower(glut_window->event_box->window);
      /*There may be problem, that if we lower too much, window is unvisible. Solve this by lower parent window
        drawing_area*/
      glut_parent_window=__gtkglut_get_window_by_id(glut_window->parent_window);
      gdk_window_lower(glut_parent_window->drawing_area->window);
    }
  }
}


/*!
    \brief    Raise the current window to the top of the stack
    \ingroup  window

    Request that the <i>current window</i> be ``popped'' to the top.

    A window can be in front of or behind other windows, as determined
    by the z-order from front to back.  Top-level GtkGLUT windows
    can be placed at the front or back of the z-order by means of
    the glutPopWindow() and glutPushWindow() API functions.

    A z-order also applies to the subwindows of a top-level window.
    While the z-order of top-level windows can usually be
    adjusted by the user, subwindow z-order is controlled entirely
    by the application.

    \see     glutCreateWindow(), glutDisplayFunc(), glutPushWindow(),
             glutWindowStatusFunc()
*/
void glutPopWindow(void) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutPopWindow");

  glut_window=__gtkglut_get_active_window_with_warning("glutPopWindow");

  if (glut_window) {
    if (glut_window->parent_window<0) {
      gdk_window_raise(glut_window->window->window);
    } else {
      gdk_window_raise(glut_window->event_box->window);
    }
  }
}

/*Internal function, which send reguest for redisplay window win, used by
  glutPostRedisplay and glutPostWindowRedisplay.*/
void __gtkglut_post_window_redisplay(char *func_name,int win) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization(func_name);

  glut_window=__gtkglut_get_window_by_id(win);  

  if (!glut_window) {
    __gtkglut_warning(func_name,"attempted on bogus window %d!",win);
  } else {
    gdk_window_invalidate_rect (glut_window->drawing_area->window, &glut_window->drawing_area->allocation, FALSE);
  }
}

/*!
    \brief    Mark the current window as needing a redisplay.
    \ingroup  window

              Whenever circumstances indicate that your window is
              in need of being redisplayed, you may call glutPostRedisplay()
              to tell GtkGLUT that you want to redraw your graphics.
              Multiple calls to this function may be coalesced by GtkGLUT
              to avoid excessive invocation of your drawing support.

              The ultimate effect of this function is to call your
              Display callback for the <i>current window</i>.

    \see      glutPostWindowRedisplay(), glutPostOverlayRedisplay(),
              glutPostWindowOverlayRedisplay(), glutSwapBuffers(),
              glutDisplayFunc()
*/
void glutPostRedisplay(void) {
  __gtkglut_post_window_redisplay("glutPostRedisplay",glutGetWindow());
}

/*!
    \brief    Mark an indicated window as needing a redisplay.
    \ingroup  window
    \param    win    The GtkGLUT window id to be affected.

              Similar to glutPostRedisplay(), except that instead
              of affecting the <i>current window</i>, this function
              affects an arbitrary window, indicated by the
              \a win parameter.

    \see      glutPostRedisplay(), glutPostOverlayRedisplay(),
              glutPostWindowOverlayRedisplay(), glutSwapBuffers(),
              glutDisplayFunc(), glutCreateWindow(), glutCreateSubWindow()
*/
void glutPostWindowRedisplay(int win) {
  __gtkglut_post_window_redisplay("glutPostWindowRedisplay",win);
}

/*Switch-off fullscreen mode (if active)*/
void __gtkglut_disable_fullscreen(void) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("__gtkglut_disable_fullscreen");

  glut_window=__gtkglut_get_active_window_with_warning("__gtkglut_disable_fullscreen");

  if (glut_window) {
    if (glut_window->parent_window<0) {
      if (glut_window->fullscreen) {
        gtk_window_unfullscreen(GTK_WINDOW(glut_window->window));
        glut_window->fullscreen=FALSE;
      }
    } else {
      __gtkglut_warning("__gtkglut_disable_fullscreen","can't call this function on subwindow %d",glutGetWindow());
    }
  }
}

/*!
    \brief    Resize the current window to cover the entire screen
    \ingroup  window

    The glutFullScreen() function resizes the window to cover the
    entire screen and hide window decorations such as title bars
    and icons.

    \note    The desktop resolution is not affected by a call
             to glutReshapeWindow() or glutFullScreen().
    \note    The size of windows is ultimately determined by the
             windowing system.  Therefore, a fullscreen request
             by an GtkGLUT application may not necessarily succeed
             or take immediate effect.
    \note    Not applicable to subwindows.
    \see     glutInit(), glutInitWindowPosition(), glutInitWindowSize(),
             glutGet(), glutPositionWindow() and glutReshapeWindow()
*/
void glutFullScreen(void) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutFullScreen");

  glut_window=__gtkglut_get_active_window_with_warning("glutFullScreen");

  if (glut_window) {
    if (glut_window->parent_window<0) {
      gtk_window_fullscreen(GTK_WINDOW(glut_window->window));
      glut_window->fullscreen=TRUE;
    } else {
      __gtkglut_warning("glutFullScreen","can't call this function on subwindow %d",glutGetWindow());
    }
  }
}

/*!
    \brief    Request to change the position of the current window
    \ingroup  window
    \param    x       Requested horizontal position of the current window
    \param    y       Requested vertical position of the current window

    The glutPositionWindow() function requests that the window system
    position a top-level or subwindow
    relative to the top-left corner.  Subwindows are typically
    resized and repositioned in response to window resize events.

    \note     The position of top-level windows is ultimately determined
              by the windowing system.  Therefore, a position request
              by an GtkGLUT application may not necessarily succeed.
    \note     May not take immediate effect; wait for the callback.
    \see      glutInit(), glutInitWindowPosition(), glutReshapeFunc(), and
              glutCreateSubWindow()
*/
void glutPositionWindow(int x, int y) {
  __gtkglut_window_struct *glut_window,*glut_parent_window;

  __gtkglut_test_inicialization("glutPositionWindow");

  glut_window=__gtkglut_get_active_window_with_warning("glutPositionWindow");

  if (glut_window) {
    if (glut_window->parent_window<0) {
      __gtkglut_disable_fullscreen();
      gtk_window_move(GTK_WINDOW(glut_window->window),x,y);
    } else {
      glut_parent_window=__gtkglut_get_window_by_id(glut_window->parent_window);
      gtk_fixed_move(GTK_FIXED(glut_parent_window->fixed),glut_window->event_box,x,y);
    }
  }  
}

/*!
    \brief    Change the size of the current window
    \ingroup  window
    \param    width    Requested window width
    \param    height   Requested window height

    The glutReshapeWindow() function adjusts the width and height of
    the <i>current window</i>, if it is an onscreen
    top-level or subwindow.  Subwindows are typically
    resized and repositioned in response to window resize events.

    The window system may delay or even alter your request.

    If you try to make a subwindow smaller than its parent, the
    parent will not grow to accomodate the child.

    \see      glutInit(), glutInitWindowSize(), glutReshapeFunc() and
              glutCreateSubWindow()
    \todo     Add support for offscreen windows.
*/
void glutReshapeWindow(int width, int height) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutReshapeWindow");

  glut_window=__gtkglut_get_active_window_with_warning("glutReshapeWindow");

  if (width<1 || height<1) {
    __gtkglut_warning("glutReshapeWindow","non-positive width or height not allowed for window %d",glutGetWindow());

    return ;
  }
  
  if (glut_window) {
    if (glut_window->parent_window<0) {
      __gtkglut_disable_fullscreen();
      gtk_window_resize(GTK_WINDOW(glut_window->window),width,height);
    } else {
      gtk_widget_set_size_request(glut_window->event_box,width,height);
    }
  }  
}

/*!
    \brief    Request that the <i>current window</i> be visible
    \ingroup  window

    glutShowWindow() requests that the window system make
    the <i>current window</i> visible.

    When a window is created, it is normally visible.

    Changes to window visibility status are notified via
    the glutWindowStatusFunc() callback.

    \see      glutHideWindow(), glutPopWindow(), glutPushWindow(),
              glutWindowStatusFunc()
*/
void glutShowWindow(void) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutShowWindow");

  glut_window=__gtkglut_get_active_window_with_warning("glutShowWindow");

  if (glut_window) {
    if (glut_window->parent_window<0) {
      if (glut_window->iconified)
        gtk_window_deiconify(GTK_WINDOW(glut_window->window));

      gtk_widget_show(glut_window->window);
    } else {
      gtk_widget_show(glut_window->event_box);
    }
  }  
  
}

/*!
    \brief    Hide the current window
    \ingroup  window

    \see      glutShowWindow()
*/
void glutHideWindow(void) {
  __gtkglut_window_struct *glut_window;
  int old_window_id;
  
  __gtkglut_test_inicialization("glutHideWindow");

  glut_window=__gtkglut_get_active_window_with_warning("glutHideWindow");

  if (glut_window) {
    old_window_id=glutGetWindow();
    __gtkglut_change_window_status(glutGetWindow(),FALSE,GLUT_HIDDEN,TRUE);
    glutSetWindow(old_window_id);
    
    if (glut_window->parent_window<0) {
      gtk_widget_hide(glut_window->window);
    } else {
      gtk_widget_hide(glut_window->event_box);
    }
  }  
}

/*!
    \brief    Iconify the current window
    \ingroup  window

    Most window systems have some kind of ``minimized'' or ``iconified''
    state for windows.  All systems currently supported by GtkGLUT
    do so.  The exact meaning of iconification is somewhat
    system-dependant, but this makes a request of the window system
    to place the window into this state.

    Graphic output is usually suspended in this form.
    User input may be partially or wholly suspended.

    If and when your window is iconified by the window system,
    it may be uniconified at any time by the system.  This usually
    happens at the request of a user.  Because of this, you should
    not use this function to hide a window.  Rather, it is to help
    unclutter the user's display, and is more or less consensual with
    the user.  Use glutHideWindow() if you want to hide the window
    entirely.

    \note     Applies only to onscreen, top-level windows.

    \note     Not guaranteed to have any effect; effect may be
              arbitrarily delayed.
    \note     There is no callback that specifically tells you
              when (or if) your window is iconified.

    \see      glutSetIconTitle(), glutHideWindow(), and glutShowWindow()
*/
void glutIconifyWindow(void) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutIconifyWindow");

  glut_window=__gtkglut_get_active_window_with_warning("glutIconifyWindow");

  if (glut_window) {
    if (glut_window->parent_window<0) {
      gtk_window_iconify(GTK_WINDOW(glut_window->window));
    } else {
      __gtkglut_warning("glutIconifyWindow","can't call this function on subwindow %d",glutGetWindow());
    }
  }
}

/*!
    \brief    Change the title of the current window
    \ingroup  window
    \param    name    New window title

    glutSetWindowTitle() requests that the window system
    change the title of the window.

    Normally a window system displays a title for every
    top-level window in the system.  The initial title is
    set when you call glutCreateWindow().  By means of this
    function you can set the titles for your top-level
    GtkGLUT windows.

    Some window systems do not provide titles for
    windows, in which case this function may have no
    useful effect.

    Because the effect may be delayed or lost, you
    should not count on the effect of this function.
    However, it can be a nice touch to use the window
    title bar for a one-line status bar in some cases.
    Use discretion.

    If you just want one title for the window over the window's
    entire life, you should set it when you open the window
    with glutCreateWindow().

    \note     Only for managed, onscreen, top-level windows.
    \note     Not all window systems display titles.
    \note     May be ignored or delayed by window manager.

    \see glutCreateWindow(), glutSetIconTitle()
*/
void glutSetWindowTitle(char *name) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutSetWindowTitle");

  glut_window=__gtkglut_get_active_window_with_warning("glutSetWindowTitle");

  if (glut_window) {
    if (glut_window->parent_window<0) {
      gtk_window_set_title(GTK_WINDOW(glut_window->window),name);
    } else {
      __gtkglut_warning("glutSetWindowTitle","can't call this function on subwindow %d",glutGetWindow());
    }
  }
}

/*!
    \brief    Change the iconified title of the current window
    \ingroup  window
    \param    name    New window title
    \note     Effect is system-dependant.

    Requests that the window system change the title of the
    icon (or whatever) that is displayed when the
    <i>current window</i> is in iconified mode.

    As discussed under glutIconifyWindow(), most window systems allow
    a window to be placed in some kind of minimized, or iconified,
    state.  In that state, the normal interior of the window is
    likely to be obscured, and the only clue about the window
    contents may be the window title.

    \see  glutSetWindowTitle(), glutIconifyWindow()

*/
void glutSetIconTitle(char *name) {
  __gtkglut_window_struct *glut_window;

  __gtkglut_test_inicialization("glutSetIconTitle");

  glut_window=__gtkglut_get_active_window_with_warning("glutSetIconTitle");

  if (glut_window) {
    if (glut_window->parent_window<0) {
      gdk_window_set_icon_name(glut_window->window->window,name);
    } else {
      __gtkglut_warning("glutSetIconTitle","can't call this function on subwindow %d",glutGetWindow());
    }
  }
}

/*!
    \brief    Moves the mouse pointer to given window coordinates.
    \ingroup  window
    \param    x        Window X coord for mouse.
    \param    y        Window Y coord for mouse.

              glutWarpPointer() moves the mouse pointer to window-relative
              coordinates given by \a x and \a y.

    \note     \a x and \a y are relative to current window.
    \note     Warping means moving, just as if the user had manually
              moved the mouse.  This can generate mouse-motion callbacks.
              If your callback then moves the pointer again, you may
              end up in an endless loop.  There is some discussion about
              changing this, but at present this is just a caveat for
              you, the user, to be aware of.

*/
void glutWarpPointer(int x, int y) {
  __gtkglut_window_struct *glut_window;
  GdkDisplay *display;
  GdkScreen *screen;
  gint wx,wy;
  
  __gtkglut_test_inicialization("glutWarpPointer");

  glut_window=__gtkglut_get_active_window_with_warning("glutWarpPointer");

  if (glut_window) {
    display=gtk_widget_get_display(glut_window->drawing_area);
    screen=gtk_widget_get_screen(glut_window->drawing_area);
    
    gdk_window_get_root_origin(glut_window->drawing_area->window,&wx,&wy);
    
    wx+=x;
    wy+=y;
    
#if GTK_MAJOR_VERSION>=2 && GTK_MINOR_VERSION>=8
    gdk_display_warp_pointer(display,screen, wx,wy);
#else
  __gtkglut_warning("glutWarpPointer","you need GDK >=2.8 for this function (warp to %d %d)",wx,wy);
#endif
      
  }
}

/*!
    \brief    Set the user data for the current window
    \ingroup  window
    \param    data    Arbitrary client-supplied pointer.

    This associates an arbitrary \a void* value with the
    <i>current window</i>.  This is especially useful
    in client-side callbacks that service many windows, if
    the client needs to know more about the window than
    GtkGLUT normally will provide.

    \see      glutGetWindowData()
*/
void glutSetWindowData(void *data) {
  __gtkglut_window_struct *glut_window;
  
  __gtkglut_test_inicialization("glutSetWindowData");

  glut_window=__gtkglut_get_active_window_with_warning("glutSetWindowData");

  if (glut_window) {
    glut_window->user_data=data;
  }
} 

/*!
    \brief    Get the user data for the current window
    \ingroup  window

    This function will return whatever \a void* value is
    associated with the <i>current window</i> via glutSetWindowData().
    This is \a NULL if you did not associate a pointer with your window.
    This can be useful in a situation where you have a single
    callback function performing services for many windows.
    You <b>could</b> keep track of the <i>window id</i>s in a
    global list and search for the <i>current window</i> in
    that list.  But this is quicker than searching a data
    structure, and allows you to avoid the use of globals for this.

    \see      glutSetWindowData()

*/
void *glutGetWindowData(void) {
  __gtkglut_window_struct *glut_window;
  void *res;
  
  __gtkglut_test_inicialization("glutGetWindowData");

  res=NULL;
  
  glut_window=__gtkglut_get_active_window_with_warning("glutGetWindowData");

  if (glut_window) {
    res=glut_window->user_data;
  }

  return res;
}
