#-------------------------------------------------
#
# Project created by QtCreator 2011-09-08T15:25:41
#
#-------------------------------------------------

QT       += core gui

TARGET = sketchit
TEMPLATE = app


SOURCES += main.cpp \
    mainpage.cpp \
    settingspage.cpp \
    colorcell.cpp \
    colorpicker.cpp \
    paintingarea.cpp \
    opendialog.cpp \
    opendialogmodel.cpp \
    opendialogimagewidget.cpp \
    savedialog.cpp \
    toolpicker.cpp \
    toolwidget.cpp \
    window.cpp \
    linewidthdialog.cpp \
    linewidthmodel.cpp \
    toolpickerdialog.cpp \
    toolpickermodel.cpp \
    aboutpage.cpp \
    colorcellwidget.cpp \
    settings.cpp

HEADERS  += \
    mainpage.h \
    settingspage.h \
    colorcell.h \
    colorpicker.h \
    paintingarea.h \
    opendialog.h \
    opendialogmodel.h \
    opendialogimagewidget.h \
    savedialog.h \
    toolpicker.h \
    toolwidget.h \
    window.h \
    linewidthdialog.h \
    linewidthmodel.h \
    toolpickerdialog.h \
    toolpickermodel.h \
    aboutpage.h \
    colorcellwidget.h \
    ViewHeader.h \
    Singleton.h \
    settings.h \
    VariantAnimation.h

FORMS    +=

CONFIG += meegotouch
CONFIG += meegotouch-boostable

MOBILITY = 

symbian {
    TARGET.UID3 = 0xe8f85d27
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/sketchit/bin
    INSTALLS += target

    icon.path = /usr/share/icons/hicolor/80x80/apps
    icon.files = data/sketchit.png

    desktop.path = /usr/share/applications
    desktop.files = data/sketchit.desktop

    toolbar_icons.path = /opt/sketchit/data
    toolbar_icons.files += data/sketchit_linewidth.png
    toolbar_icons.files += data/sketchit_pannable.png
    toolbar_icons.files += data/sketchit_pannable_selected.png
    toolbar_icons.files += data/sketchit_draw.png

    style.path = /opt/sketchit/data
    style.files = data/sketchit.css data/sketchit128.png data/sketchit-dialog-header.png

    INSTALLS += icon toolbar_icons desktop style

}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

RESOURCES +=































































