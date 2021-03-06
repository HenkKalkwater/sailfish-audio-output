# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-audio-output


CONFIG += sailfishapp\
        libpulse

LIBS += -lpulse

SOURCES += src/audio-output.cpp \
    src/pamainloopthread.cpp \
    src/portmodel.cpp \
    src/sinkmodel.cpp \
    src/sink.cpp \
    src/port.cpp

DISTFILES += \
    qml/cover/CoverPage.qml \
    rpm/harbour-audio-output.changes.run \
    rpm/harbour-audio-output.spec \
    translations/*.ts \
    harbour-audio-output.desktop \
    qml/pages/SelectPage.qml \
    rpm/harbour-audio-output.changes \
    rpm/harbour-audio-output.yaml \
    qml/harbour-audio-output.qml \
    qml/pages/MainPage.qml

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += \
            translations/harbour-aduio-output.ts \
            translations/harbour-audio-output-nl.ts \
            translations/harbour-audio-output-fr.ts \
            translations/harbour-audio-output-zh_CN.ts

HEADERS += \
    src/audio-output.h \
    src/pamainloopthread.h \
    src/portmodel.h \
    src/sinkmodel.h \
    src/sink.h \
    src/port.h \
    src/pamainloopthread.h
