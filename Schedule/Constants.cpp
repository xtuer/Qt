#include "Constants.h"
#include <QString>

namespace Constants {
const char* MIMETYPE_COURSE             = "application/course";
const char* MIMETYPE_TEACHER            = "application/teacher";
const char* MIMETYPE_SCHEDULE_ITEM      = "application/scheduleItem";

const char* QSS_CLASS                   = "class";
const char* QSS_CLASS_DAY_LABEL         = "DayLabel";
const char* QSS_CLASS_TIME_LABEL        = "TimeLabel";
const char* QSS_CLASS_COURSE            = "Course";
const char* QSS_CLASS_TEACHER           = "Teacher";
const char* QSS_CLASS_SCHEDULE_ITEM_WIDGET         = "ScheduleItem";
const char* QSS_CLASS_SCHEDULE_ITEM_EDITABLE_LABEL = "ScheduleItemEditableLabel";


const char* PROPERTY_ROW                = "row";
const char* PROPERTY_COLUMN             = "column";
const char* PROPERTY_COLUMN_SPAN        = "columnSpan";
const char* PROPERTY_DRAG_FOCUS         = "dragFocus";
const char* PROPERTY_SCHEDULE_ITEM      = "scheduleItem";
const char* PROPERTY_SCHEDULE_ITEM_CONFLICT = "conflict";

const char* FLAG_IMAGE_NAME = "editableFlag";
const char* FLAG_IMAGE_PATH = "images/flag.png";

const int   SCHEDULE_MAGIC_NUMBER = 13930;
}
