#include "Constants.h"
#include <QString>

namespace Constants {
const char* MIME_TYPE_COURSE            = "application/course";
const char* MIME_TYPE_TEACHER           = "application/teacher";
const char* MIME_TYPE_SCHEDULE_ITEM     = "application/scheduleItem";

const char* QSS_CLASS                   = "class";
const char* QSS_CLASS_DAY_LABEL         = "DayLabel";
const char* QSS_CLASS_TIME_LABEL        = "TimeLabel";
const char* QSS_CLASS_REST_LABEL        = "RestLabel";
const char* QSS_CLASS_SCHEDULE_ITEM_WIDGET         = "ScheduleItemWidget";
const char* QSS_CLASS_SCHEDULE_ITEM_EDITABLE_LABEL = "ScheduleItemEditableLabel";

const char* PROPERTY_ROW                = "row";
const char* PROPERTY_COLUMN             = "column";
const char* PROPERTY_DRAG_FOCUS         = "dragFocus";
const char* PROPERTY_SCHEDULE_ITEM_CONFLICT = "conflict";

const int   SCHEDULE_MAGIC_NUMBER = 13930;
}
