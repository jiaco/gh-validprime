TEMPLATE=app
TGT="ValidPrime"

CONFIG += gh gui

RESOURCES +=\
	resources/help.qrc	\
	resources/demo.qrc	\

HEADERS +=\
	ghLinReg.h	\
	ghDilSeries.h	\
	vpstore.h	\
	window.h	\
	myApp.h

SOURCES +=\
	ghLinReg.cpp	\
	ghDilSeries.cpp	\
	vpstore.cpp	\
	window.cpp	\
	myApp.cpp	\
	_main.cpp

include( $$(GH_HOME)/ghbase.pri )
