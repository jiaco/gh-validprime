#ifndef	WINDOW_H
#define	WINDOW_H	1
#include <GhCore.h>
#include <GhGui.h>
#include "myApp.h"

namespace	GH
{

class	Window	:	public	MainWindow
{
	Q_OBJECT

public:
	Window( MyApp* app = 0 );
public slots:
	void	postload();
	bool	postcheck();
	//void	postcheck( const bool& passed, const QString& dialogMesg );
	void	postrun();
	
	void	setState( const VP::State& state );
	void	centralGridClicked( const QVariant& row, const QVariant& column );

	void	getUserConc();

	void	viewCqRna();
	void	viewCqDna();
	void	viewPctDna();
	void	viewScore();
	void	viewHeatMap();

	void	toggleInput();
	void	toggleCqRna();
	void	toggleCqDna();
	void	togglePctDna();
	void	toggleScore();

	void	viewGrid( const VP::DataRole& role );
	void	toggleGrid( const VP::DataRole& role );

	void	showHelp();
signals:
	void	statusMessage( const QString& message, int timeout = 0 );
private:
	void	enableParams( const QStringList& names );
	void	disableParams( const QStringList& names );
	void	configureParams();
	void	layoutParamParams( QGridLayout* layout );
	void	layoutOutputParams( QGridLayout* layout );
	void	setupActionToolBar();
	void	setupMenu();

	QGroupBox*	groupBox( const QString& title,
			 const QStringList& paramNames );

	MyApp	*_myApp;

	QTabWidget	*_controlWidget;
	QWidget		*_paramWidget;
	QGridLayout	*_paramLayout;
	QWidget		*_outputWidget;
	QGridLayout	*_outputLayout;

	GridWidget	*_gridWidget;
	QDockWidget	*_dockWidget;

	QToolBar	*_actionToolBar;
	QToolButton	*_gridToggle;
	QLabel		*_gridDisplayed;
	QMenu		*_viewMenu;
	QStatusBar	*_statusBar;

	VP::State	_state;

	QString		_helpText;
	QWidget		*_help;
};

}	//	GH namespace
#endif	//	WINDOW_H
