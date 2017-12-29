/*!
    \file  gtkglut_refset.c
    \brief GtkGLUT refset ADS.
*/

/* 
 * GtkGLUT refset ADS.
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
 *
 */

#include "GL/gtkglut.h"
#include "gtkglut_internal.h"

/*Special ADS for menu. We must be notified, if some menu changes, and we must
  allow user to attach menu more time to other menu -> we need reference counting*/

/*Make reference in refset list to ref pointer*/
GList *__gtkglut_refset_ref(GList *list,gpointer ref) {
  GList *elem;
  __gtkglut_refset_item *item;
  gboolean found;
  
  found=FALSE;
  elem=list;

  while (elem && !found) {
    item=(__gtkglut_refset_item*)elem->data;

    if (item->ref==ref) {
      found=TRUE;
    }
    
    elem=g_list_next(elem);
  }

  if (found) {
    item->value++;
  } else {
    item=(__gtkglut_refset_item*)malloc(sizeof(__gtkglut_refset_item));

    if (!item) __gtkglut_lowmem();

    item->ref=ref;
    item->value=1;

    list=g_list_append(list,item);
  }

  return list;
}

/*Dec reference in refset list to ref pointer. If reference count is 0 -> remove item from list*/
GList *__gtkglut_refset_unref(GList *list,gpointer ref) {
  GList *elem;
  __gtkglut_refset_item *item;
  gboolean found;
  
  found=FALSE;
  elem=list;

  while (elem && !found) {
    item=(__gtkglut_refset_item*)elem->data;

    if (item->ref==ref) {
      found=TRUE;
    }
    
    elem=g_list_next(elem);
  }

  if (found) {
    item->value--;

    if (item->value<=0) {
      list=g_list_remove(list,item);
      free(item);
    }
  }
 
  return list;
}

/*Destroy refset ADS*/
GList *__gtkglut_refset_destroy(GList *list) {
  GList *elem;
  __gtkglut_refset_item *item;
  
  elem=list;

  while (elem) {
    item=(__gtkglut_refset_item*)elem->data;

    free(item);
    
    elem=g_list_next(elem);
  }

  /*All childs data are destroyed, destroy list*/
  g_list_free(list);
  
  return NULL;
}

