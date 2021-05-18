#ifndef TEXT_OP_EXECUTE
#define TEXT_OP_EXECUTE

#include "BKE_context.h"
#include "WM_types.h"
#include "DNA_text_types.h"
#include "DNA_text_types.h"

#include "MEM_guardedalloc.h"

#include "BLI_blenlib.h"

#include "BKE_context.h"
#include "BKE_global.h"
#include "BKE_lib_id.h"
#include "BKE_screen.h"
#include "BKE_text.h"

#include "ED_screen.h"
#include "ED_space_api.h"

#include "WM_api.h"
#include "WM_types.h"

#include "UI_interface.h"
#include "UI_resources.h"
#include "UI_view2d.h"

#include "RNA_access.h"

int text_execute_exec(bContext *C, wmOperator *op);
int text_execute_invoke(bContext *C, wmOperator *op, const wmEvent *UNUSED(event));
bool text_execute_poll(bContext *C);
void TEXT_OT_execute_everything_or_selection(wmOperatorType *ot);

#endif
