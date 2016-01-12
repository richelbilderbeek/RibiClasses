INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/array_view_lite.h \
    $$PWD/fail_fast_lite.h \
    $$PWD/gsl_lite.h \
    $$PWD/string_view_lite.h

DEFINES += GSL_ARRAY_VIEW_H GSL_STRING_VIEW_H
