#include "text_op_execute.h"

#include "text_duktape.h"

int text_execute_exec(bContext *C, wmOperator *op) {
  Text *text = NULL;
  // #########################
  text = CTX_data_edit_text(C);
  //int resolution = RNA_enum_get(op->ptr, "resolution");
  printf("text_execute_exec!!!\n");
  return OPERATOR_FINISHED;
  //switch (resolution) {
  //  case RESOLVE_RELOAD:
  //    return text_reload_exec(C, op);
  //  case RESOLVE_SAVE:
  //    return text_save_exec(C, op);
  //  case RESOLVE_MAKE_INTERNAL:
  //    return text_make_internal_exec(C, op);
  //  case RESOLVE_IGNORE:
  //    BKE_text_file_modified_ignore(text);
  //    return OPERATOR_FINISHED;
  //}
  //
  //return OPERATOR_CANCELLED;
}

int text_execute_invoke(bContext *C, wmOperator *op, const wmEvent *UNUSED(event)) {
  Text       *text = NULL;
  uiPopupMenu *pup = NULL;
  uiLayout *layout = NULL;
  char *buf        = NULL;
  // #########################
  text = CTX_data_edit_text(C);
  // Either eval selection or eval everything
  if (txt_has_sel(text)) {
    buf = txt_sel_to_buf(text, NULL);
  } else {
    buf = txt_to_buf(text, NULL);
  }
  quickjs_eval(buf);
  //printf("text_execute_invoke\n");
  //printf("Selection: \"%s\"", sel);
  MEM_freeN(buf);
  // Update editor window
  WM_event_add_notifier(C, NC_TEXT | NA_EDITED, text);
  return OPERATOR_FINISHED;

#if 0
  switch (BKE_text_file_modified_check(text)) {
    case 1:
      if (text->flags & TXT_ISDIRTY) {
        /* Modified locally and externally, ah. offer more possibilities. */
        pup = UI_popup_menu_begin(
            C, IFACE_("File Modified Outside and Inside Blender"), ICON_NONE);
        layout = UI_popup_menu_layout(pup);
        uiItemEnumO_ptr(layout,
                        op->type,
                        IFACE_("Reload from disk (ignore local changes)"),
                        0,
                        "resolution",
                        RESOLVE_RELOAD);
        uiItemEnumO_ptr(layout,
                        op->type,
                        IFACE_("Save to disk (ignore outside changes)"),
                        0,
                        "resolution",
                        RESOLVE_SAVE);
        uiItemEnumO_ptr(layout,
                        op->type,
                        IFACE_("Make text internal (separate copy)"),
                        0,
                        "resolution",
                        RESOLVE_MAKE_INTERNAL);
        UI_popup_menu_end(C, pup);
      }
      else {
        pup = UI_popup_menu_begin(C, IFACE_("File Modified Outside Blender"), ICON_NONE);
        layout = UI_popup_menu_layout(pup);
        uiItemEnumO_ptr(
            layout, op->type, IFACE_("Reload from disk"), 0, "resolution", RESOLVE_RELOAD);
        uiItemEnumO_ptr(layout,
                        op->type,
                        IFACE_("Make text internal (separate copy)"),
                        0,
                        "resolution",
                        RESOLVE_MAKE_INTERNAL);
        uiItemEnumO_ptr(layout, op->type, IFACE_("Ignore"), 0, "resolution", RESOLVE_IGNORE);
        UI_popup_menu_end(C, pup);
      }
      break;
    case 2:
      pup = UI_popup_menu_begin(C, IFACE_("File Deleted Outside Blender"), ICON_NONE);
      layout = UI_popup_menu_layout(pup);
      uiItemEnumO_ptr(
          layout, op->type, IFACE_("Make text internal"), 0, "resolution", RESOLVE_MAKE_INTERNAL);
      uiItemEnumO_ptr(layout, op->type, IFACE_("Recreate file"), 0, "resolution", RESOLVE_SAVE);
      UI_popup_menu_end(C, pup);
      break;
  }
  return OPERATOR_INTERFACE;
#endif
}

bool text_execute_poll(bContext *C) {
  Text *text = NULL;
  bool hasText = false;
  // #########################
  text = CTX_data_edit_text(C);
  //printf("text_execute_poll text=%p\n", text);
  if (text) {
    hasText = true;
  }
  return hasText;
  //if (!text_edit_poll(C)) {
  //  return false;
  //}
  //return ((text->filepath != NULL) && !(text->flags & TXT_ISMEM));
}

void TEXT_OT_execute_everything_or_selection(wmOperatorType *ot) {
  // Identifiers
  ot->name        = "Execute Everything Or Selection";
  ot->idname      = "TEXT_OT_execute_everything_or_selected";
  ot->description = "Execute everything or Selection Description";

  // API callbacks
  ot->exec   = text_execute_exec;
  ot->invoke = text_execute_invoke;
  ot->poll   = text_execute_poll;

  ///* properties */
  //RNA_def_enum(ot->srna,
  //             "resolution",
  //             resolution_items,
  //             RESOLVE_IGNORE,
  //             "Resolution",
  //             "How to solve conflict due to differences in internal and external text");
}
