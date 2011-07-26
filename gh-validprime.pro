TEMPLATE=app
TGT="ValidPrime"

CONFIG += gh gui

RESOURCES += resources/help.qrc

HEADERS +=\
	ghLinReg.h	\
	vpstore.h	\
	window.h	\
	myApp.h

SOURCES +=\
	ghLinReg.cpp	\
	vpstore.cpp	\
	window.cpp	\
	myApp.cpp	\
	_main.cpp

include( $$(GH_HOME)/ghbase.pri )
