#-------------------------------------------------
#
# Project created by QtCreator 2018-07-30T15:52:37
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PMan
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogs/dialogproject.cpp \
    dbconnector.cpp \
    models/project.cpp \
    basemodel.cpp \
    mdi/mdiproject.cpp \
    dialogs/dialogtask.cpp \
    models/task.cpp \
    widgets/rtfeditor.cpp \
    models/note.cpp \
    widgets/calendar.cpp \
    mdi/mdijournal.cpp \
    dialogs/dialogevent.cpp \
    models/event.cpp \
    widgets/projectwidgetitem.cpp \
    dialogs/dialogminitasks.cpp \
    dialogabout.cpp

HEADERS  += mainwindow.h \
    dialogs/dialogproject.h \
    dbconnector.h \
    models/project.h \
    basemodel.h \
    mdi/mdiproject.h \
    dialogs/dialogtask.h \
    models/task.h \
    constants.h \
    widgets/rtfeditor.h \
    models/note.h \
    widgets/calendar.h \
    mdi/mdijournal.h \
    dialogs/dialogevent.h \
    models/event.h \
    widgets/projectwidgetitem.h \
    dialogs/dialogminitasks.h \
    dialogabout.h

FORMS    += mainwindow.ui \
    dialogs/dialogproject.ui \
    mdi/mdiproject.ui \
    dialogs/dialogtask.ui \
    widgets/rtfeditor.ui \
    widgets/calendar.ui \
    mdi/mdijournal.ui \
    dialogs/dialogevent.ui \
    widgets/projectwidgetitem.ui \
    dialogs/dialogminitasks.ui \
    dialogabout.ui

RESOURCES += \
    icons.qrc \
    models.qrc

OTHER_FILES += \
    model.sql

win32:RC_ICONS += icons/pman.ico
