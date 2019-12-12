TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    server.cpp \
    client.cpp
#create server program has feature:
#   receive data from client and save
#   send data to client when receive request
#

HEADERS += \
    server.h \
    client.h
