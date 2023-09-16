
/******************************************************************************
* MODULE     : editor.cpp
* DESCRIPTION: routines for the editor
* COPYRIGHT  : (C) 1999  Joris van der Hoeven
*******************************************************************************
* This software falls under the GNU general public license version 3 or later.
* It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
* in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
******************************************************************************/

#include "edit_main.hpp"
#include "tm_buffer.hpp"
#include "tm_file.hpp"
#include "sys_utils.hpp"
#include "printer.hpp"
#include "convert.hpp"
#include "connect.hpp"
#include "typesetter.hpp"
#include "drd_std.hpp"
#include "message.hpp"
#include <setjmp.h>
#include "image_files.hpp"
#include "iterator.hpp"
#include "tree_observer.hpp"
#include "observers.hpp"

#ifdef USE_PLUGIN_PDF
#include "Pdf/pdf_hummus_make_attachment.hpp"
#endif

#ifdef EXPERIMENTAL
#include "../../Style/Memorizer/clean_copy.hpp"
#endif

#ifdef USE_GS
#include "Ghostscript/gs_utilities.hpp"
#endif

#ifdef QTTEXMACS
#include "Qt/qt_gui.hpp"
#include "Qt/qt_utilities.hpp"
#endif

/******************************************************************************
* Constructors and destructor
******************************************************************************/

editor_rep::editor_rep ():
  simple_widget_rep (), cvw (NULL), mvw (NULL),
  drd (std_drd), et (the_et), rp () {
  cout << "TeXmacs] warning, this virtual constructor should never be called\n";
}

editor_rep::editor_rep (server_rep* sv2, tm_buffer buf2):
  simple_widget_rep (), sv (sv2), cvw (NULL), mvw (NULL), buf (buf2),
  drd (buf->buf->title, std_drd), et (the_et), rp (buf2->rp) {}

bool
editor_rep::is_current_editor () {
  editor ed= get_current_editor ();
  return ed.rep == (editor_rep*) this;
}

edit_main_rep::edit_main_rep (server_rep* sv, tm_buffer buf):
  editor_rep (sv, buf), props (UNKNOWN), ed_obs (edit_observer (this))
{
#ifdef EXPERIMENTAL
  cct= copy (subtree (et, rp));
  copy_ip (subtree (et, rp), cct);
#endif
  attach_observer (subtree (et, rp), ed_obs);
  notify_change (THE_TREE);
  tp= correct_cursor (et, rp * 0);
}

edit_main_rep::~edit_main_rep () {
  detach_observer (subtree (et, rp), ed_obs);
#ifdef EXPERIMENTAL
  mem= memorizer ();
#endif
}

editor
new_editor (server_rep* sv, tm_buffer buf) {
  return tm_new<edit_main_rep> (sv, buf);
}

template<> void
tm_delete<editor_rep> (editor_rep* ptr) {
  void *mem= ptr->derived_this ();
  ptr -> ~editor_rep ();
  fast_delete (mem);
}

/******************************************************************************
* Properties
******************************************************************************/

void
edit_main_rep::set_property (scheme_tree what, scheme_tree val) {
  props (what)= val;
}

void
edit_main_rep::set_bool_property (string what, bool val) {
  props (what)= (val? string ("true"): string ("false"));
}

void
edit_main_rep::set_int_property (string what, int val) {
  props (what)= as_tree (val);
}

void
edit_main_rep::set_string_property (string what, string val) {
  props (what)= val;
}

scheme_tree
edit_main_rep::get_property (scheme_tree what) {
  return props [what];
}

bool
edit_main_rep::get_bool_property (string what) {
  return as_bool (props [what]);
}

int
edit_main_rep::get_int_property (string what) {
  return as_int (props [what]);
}

string
edit_main_rep::get_string_property (string what) {
  return as_string (props [what]);
}

/******************************************************************************
* Global routines
******************************************************************************/

void
edit_main_rep::clear_buffer () {
  assign (rp, tree (DOCUMENT, tree ("")));
}

void
edit_main_rep::new_window () {
}

void
edit_main_rep::clone_window () {
}

void
edit_main_rep::tex_buffer () {
}

url
edit_main_rep::get_name () {
  return buf->buf->name;
}

void
edit_main_rep::focus_on_this_editor () {
  focus_on_editor (this);
}

void
edit_main_rep::notify_page_change () {
  if (is_attached (this)) send_invalidate_all (this);
}

string
edit_main_rep::get_metadata (string kind) {
  string var= "global-" * kind;
  string val= cork_to_utf8 (get_init_string (var));
  if (val != "") return val;
  val= cork_to_utf8 (search_metadata (subtree (et, rp), kind));
  if (val != "") return val;
  if (kind == "title") return as_string (tail (get_name ()));
  return "";
}

/******************************************************************************
* Printing
******************************************************************************/

string printing_dpi ("600");
string printing_on ("a4");

bool
use_pdf () {
#ifdef USE_PLUGIN_PDF
  return get_preference ("native pdf", "on") == "on";
#else
  return false;
#endif
}

bool
use_ps () {
#ifdef USE_PLUGIN_PDF
  return get_preference ("native postscript", "on") == "on";
#else
  return true;
#endif
}

int
edit_main_rep::nr_pages () {
  string medium = env->get_string (PAGE_MEDIUM);
  if (medium == "paper") return N (eb[0]);
  typeset_prepare ();
  env->write (PAGE_MEDIUM, "paper");
  box the_box= typeset_as_document (env, subtree (et, rp), reverse (rp));
  env->write (PAGE_MEDIUM, medium);
  return N (the_box[0]);
}

void
edit_main_rep::print_doc (url name, bool conform, int first, int last) {
  bool ps  = (suffix (name) == "ps");
  bool pdf = (suffix (name) == "pdf");
  url  orig= resolve (name, "");

#ifdef USE_GS
  if (!use_pdf () && pdf)
    name= url_temp (".ps");
  if (!use_ps () && ps)
    name= url_temp (".pdf");
#endif
  
  string medium = env->get_string (PAGE_MEDIUM);
  if (conform && (medium != "paper")) conform= false;
    // FIXME: better command for conform printing

  typeset_preamble ();
    // FIXME: when printing several files via aux buffers,
    // it seems that the style can be corrupted.  Why?
  
  // Set environment variables for printing

  typeset_prepare ();
  env->write (DPI, printing_dpi);
  env->write (PAGE_SHOW_HF, "true");
  env->write (PAGE_SCREEN_MARGIN, "false");
  env->write (PAGE_BORDER, "none");
  if (is_func (env->read (BG_COLOR), PATTERN))
    env->write (BG_COLOR, env->exec (env->read (BG_COLOR)));
  if (!conform) {
    env->write (PAGE_MEDIUM, "paper");
    env->write (PAGE_PRINTED, "true");
  }

  // Typeset pages for printing

  box the_box= typeset_as_document (env, subtree (et, rp), reverse (rp));

  // Determine parameters for printer

  string page_type = env->page_real_type;
  double w         = env->page_real_width;
  double h         = env->page_real_height;
  double cm        = env->as_real_length (string ("1cm"));
  bool   landsc    = env->page_landscape;
  int    dpi       = as_int (printing_dpi);
  int    start     = max (0, first-1);
  int    end       = min (N(the_box[0]), last);
  int    pages     = end-start;
  if (conform) {
    page_type= "user";
    SI bw= the_box[0][0]->w();
    SI bh= the_box[0][0]->h();
    string bws= as_string (bw) * "tmpt";
    string bhs= as_string (bh) * "tmpt";
    w= env->as_length (bws);
    h= env->as_length (bhs);
  }
  
  // Print pages
  renderer ren= printer (name, dpi, pages, page_type, landsc, w/cm, h/cm);
  
  if (ren->is_started ()) {
    int i;
    ren->set_metadata ("title", get_metadata ("title"));
    ren->set_metadata ("author", get_metadata ("author"));
    ren->set_metadata ("subject", get_metadata ("subject"));
    for (i=start; i<end; i++) {
      tree bg= env->read (BG_COLOR);
      ren->set_background (bg);
      if (bg != "white" && bg != "#ffffff")
        ren->clear_pattern (0, (SI) -h, (SI) w, 0);

      rectangles rs;
      the_box[0]->sx(i)= 0;
      the_box[0]->sy(i)= 0;
      the_box[0][i]->redraw (ren, path (0), rs);
      if (i<end-1) ren->next_page ();
    }
  }
  tm_delete (ren);

#ifdef USE_GS
  if (!use_pdf () && pdf) {
    gs_to_pdf (name, orig, landsc, h/cm, w/cm);
    ::remove (name);
  }
  if (!use_ps () && ps) {
    gs_to_ps (name, orig, landsc, h/cm, w/cm);
    ::remove (name);
  }
  if (ps || pdf)
    if (get_preference ("texmacs->pdf:check", "off") == "on") {
      //system_wait ("Checking exported file for correctness", "please wait");
      // FIXME: the wait message often causes a crash, currently
      gs_check (orig);
    }
#endif
}
static hashset<string> internal_styles;

static void
declare_style (url u) {
  if (is_or (u)) {
    declare_style (u[1]);
    declare_style (u[2]);
  }
  else if (is_concat (u)) {
    string dir= upcase_first (as_string (u[1]));
    if (dir == "CVS" || dir == ".svn");
    else declare_style (u[2]);
  }
  else if (is_atomic (u)) {
    string s= as_string (u);
    if (ends (s, ".ts") && !starts (s, "source")) {
      internal_styles->insert (s(0,N(s)-3));
      if (starts (s, "old-"))
        internal_styles->insert (s(4,N(s)-3));
      if (starts (s, "old2-"))
        internal_styles->insert (s(5,N(s)-3));
    }
  }
}

static bool
is_internal_style (string style) {
  return true;

  if (N (internal_styles) == 0) {
    url sty_u= descendance ("$TEXMACS_STYLE_ROOT");
    declare_style (sty_u);
  }
  return internal_styles->contains (style);
}

void
edit_main_rep::print_to_file (url name, string first, string last) {
  print_doc (name, false, as_int (first), as_int (last));

#ifdef USE_PLUGIN_PDF
  if ((suffix (name) == "pdf")) {
    if (as_bool (call ("get-boolean-preference",
                       "gui:export PDF with tm attachment"))) {
      if (!attach_doc_to_exported_pdf (name)) {
        debug_convert << "fail : attach_doc_to_exported_pdf" << LF;
      }
    }
  }
#endif
  set_message ("Done printing", "print to file");
}

#ifdef USE_PLUGIN_PDF
bool
edit_main_rep::attach_doc_to_exported_pdf (url pdf_name) {
  // copy the current buffer to a new buffer
  string dir ("$TEXMACS_HOME_PATH/texts/scratch/");
  string name_= basename (pdf_name) * string (".tm");
  url    tmp_save_path (dir, name_);

  url new_u= make_new_buffer ();
  rename_buffer (new_u, tmp_save_path);
  new_u    = tmp_save_path;
  url cur_u= get_current_buffer ();
  call ("buffer-copy", cur_u, new_u);
  buffer_save (new_u);

  // Re-read the saved file to update the buffer, otherwise there is no style in
  // the export tree
  tree new_t= import_tree (new_u, "texmacs");
  set_buffer_tree (new_u, new_t);
  new_t= get_buffer_tree (new_u);

  // dfs search all style and link
  list<tree> st (new_t);
  list<url>  tm_and_linked_file (new_u);
  while (N (st) != 0) {
    auto la= last_item (st);
    st     = suppress_last (st);
    for (int i= 0; i < arity (la); i++) {
      if (is_compound (la[i])) {
        string label= get_label (la[i]);
        if (label == "image" || label == "include") {
          if (is_atomic (la[i][0])) {
            url pre_url= url (la[i][0]->label);
            if (!exists (pre_url)) {
              pre_url= head (cur_u) * pre_url;
              if (!exists (pre_url)) {
                debug_convert << pre_url << "do not exist" << LF;
              }
            }
            tm_and_linked_file= tm_and_linked_file * pre_url;
            string name       = as_string (tail (pre_url));
            la[i][0]->label   = string (name);
          }
        }
        else if (label == "style") {
          if (is_func (la[i][0], TUPLE)) {
            for (int j= 0; j < N (la[i][0]); j++) {
              string style_name= get_label (la[i][0][j]);
              if (!is_internal_style (style_name)) {
                url style_url= url (style_name);
                style_url    = glue (style_url, ".ts");
                if (!exists (style_url)) {
                  style_url= head (cur_u) * style_url;
                  if (!exists (style_url)) {
                    debug_convert << style_url << "do not exist" << LF;
                  }
                }
                tm_and_linked_file= tm_and_linked_file * style_url;
                string name       = basename (style_url);
                la[i][0][j]->label= name;
              }
            }
          }
          else {
            if (!is_atomic (la[i][0])) {
              debug_convert << get_label (la[i][0]) << "is not atomic tree" << LF;
            }
            string style_name= get_label (la[i][0]);
            if (!is_internal_style (style_name)) {
              url style_url= url (style_name);
              style_url    = glue (style_url, ".ts");
              if (!exists (style_url)) {
                style_url= head (cur_u) * style_url;
                if (!exists (style_url)) {
                    debug_convert << style_url << "do not exist" << LF;
                }
              }
              tm_and_linked_file= tm_and_linked_file * style_url;
              string name= basename (style_url);
              la[i][0]->label= name;
            }
          }
        }
        else st= st * la[i];
      }
    }
  }

  set_buffer_tree (new_u, new_t);
  buffer_save (new_u);
  new_t= get_buffer_tree (new_u);

  if (!pdf_hummus_make_attachments (pdf_name, tm_and_linked_file, pdf_name)) {
    debug_convert << "fail : pdf_hummus_make_attachments" << LF;
    return false;
  }
  return true;
}
#endif

void
edit_main_rep::print_buffer (string first, string last) {
  url target;
#if defined(OS_MINGW) || defined(OS_WIN)
  target= use_pdf ()? url_temp (".pdf"): url_temp (".ps");
#else
  target= url_temp (".ps");
#endif
  print_doc (target, false, as_int (first), as_int (last));
  system (get_printing_cmd (), target);  // Send the document to the printer
  set_message ("Done printing", "print buffer");
  ::remove (target);
}

#ifdef THISISTHEPREVIOUSCODE_IJUSTLEFTITHEREINCASE
void
edit_main_rep::print_buffer (string first, string last) {
  // in Qt this is the main entry point to the printing subsystem.
  // the other routines (print_to_file, ...) are overriden since all fine tuning 
  // is made here via the Qt print dialog
  bool to_file, landscape;
  url name = url_none();
  string printer;
  string paper_type;
  if (qt_print (to_file, landscape, printer, name, first, last, paper_type)) {
      if (!to_file) name = url_temp (".ps");
      print_doc (name, false, as_int (first), as_int (last));
      if (!to_file) {
        string cmd = printing_cmd * " -P" * printer;
        system (cmd, name);  
        ::remove (name);
      }
  }
}
#endif

void
edit_main_rep::export_ps (url name, string first, string last) {
  print_doc (name, true, as_int (first), as_int (last));
}

array<int>
edit_main_rep::print_snippet (url name, tree t, bool conserve_preamble) {
  tree buft= subtree (et, rp);
  if (conserve_preamble)
    if (is_document (buft) && is_compound (buft[0], "hide-preamble"))
      t= tree (SURROUND, buft[0], "", t);

  string s= suffix (name);
  bool bitmap=
    (s == "png" || s == "jpg" || s == "jpeg" || s == "tif" || s == "tiff");
#ifndef QTTEXMACS
  bitmap= false;
#endif
  bool ps= (s == "ps" || s == "eps");
  if (use_pdf ()) ps= (ps || s == "pdf");

  typeset_prepare ();
  int dpi= as_int (printing_dpi);
  tree old_dpi= env->read (DPI);
  tree old_info_flag= env->read (INFO_FLAG);
  env->write (DPI, printing_dpi);
  if (is_compound (old_info_flag) || !ends (old_info_flag->label, "paper"))
    env->write (INFO_FLAG, "none");
  env->style_init_env ();
  env->update ();
  box b= typeset_as_box (env, t, path ());
  env->write (DPI, old_dpi);
  env->write (INFO_FLAG, old_info_flag);
  env->style_init_env ();
  env->update ();
  
  if (b->x4 - b->x3 >= 5*PIXEL && b->y4 - b->y3 >= 5*PIXEL) {
    if (bitmap) make_raster_image (name, b, 5.0);
    else if (ps) make_eps (name, b, dpi);
    else {
      url temp= url_temp (use_pdf ()? ".pdf": ".eps");
      make_eps (temp, b, dpi);
      ::remove (name);
      if (!call_scm_converter (temp, name)) {
        call_imagemagick_convert (temp, name);
        if (!exists (name))
          convert_error << "could not convert snippet " << temp
                        << " into :" << name << "\n";
      }
      ::remove (temp);
    }
  }
  array<int> a;
  a << b->x3 << b->y3 << b->x4 << b->y4 << b->x1 << b->y1 << b->x2 << b->y2;
  a << env->get_int (FONT_BASE_SIZE) << dpi;
  return a;
}

bool
edit_main_rep::graphics_file_to_clipboard (url name) {
#ifdef QTTEXMACS
  the_gui->put_graphics_on_clipboard (name);
  return true;
#else 
  return false;
#endif
}

/******************************************************************************
* Evaluation of expressions
******************************************************************************/

void
edit_main_rep::footer_eval (string s) {
  // s= unslash (s); // FIXME: dirty fix; should not be necessary
  s= tm_decode (s);
  string r= object_to_string (eval (s));
  set_message (verbatim (r), "evaluate expression");
}

tree
edit_main_rep::the_line () {
  path p= search_parent_upwards (DOCUMENT);
  return copy (subtree (et, p));
}

tree
edit_main_rep::the_root () {
  return et;
}

tree
edit_main_rep::the_buffer () {
  return subtree (et, rp);
}

bool
edit_main_rep::test_subtree (path p) {
  return has_subtree (et, p);
}

tree
edit_main_rep::the_subtree (path p) {
  return subtree (et, p);
}

path
edit_main_rep::the_buffer_path () {
  return copy (rp);
}

path
edit_main_rep::the_path () {
  return copy (tp);
}

path
edit_main_rep::the_shifted_path () {
  return shift (et, tp, 1);
}

/******************************************************************************
* Miscellaneous
******************************************************************************/

void
edit_main_rep::show_tree () {
  stretched_print (et, true);
  // cout << et << "\n";
}

void
edit_main_rep::show_env () {
  cout << env << "\n";
}

void
edit_main_rep::show_path () {
  cout << tp << "\n";
}

void
edit_main_rep::show_cursor () {
  cout << "Principal cursor: "
       << cu->ox << ", " << cu->oy << " [" << cu->delta << "], "
       << cu->y1 << " : " << cu->y2 << ", " << cu->slope << "\n";
  cout << "Ghost cursor    : "
       << mv->ox << ", " << mv->oy << " [" << mv->delta << "], "
       << mv->y1 << " : " << mv->y2 << ", " << mv->slope << "\n";
}

void
edit_main_rep::show_selection () {
  selection sel; selection_get (sel);
  cout << "physical  selection: " << cur_sel << "\n";
  cout << "logical   selection: " << sel->start << " --- " << sel->end << "\n";
}

void
edit_main_rep::show_meminfo () {
  mem_info ();
}

void
edit_main_rep::edit_special () {
}
