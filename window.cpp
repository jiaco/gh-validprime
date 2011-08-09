#include "window.h"

namespace	GH
{
	Window::Window( MyApp* app )
	: MainWindow( app )
{
	_myApp = app;
	setWindowTitle( "ValidPrime" );
	//
	//	centralWidget is the Input Grid
	//
	_gridWidget = new GridWidget( this );
	_gridWidget->tableView()->setSortingEnabled( false );
	connect( _gridWidget, SIGNAL( clicked(QVariant,QVariant) ),
	 this, SLOT( centralGridClicked(QVariant,QVariant) ) );
	setCentralWidget( _gridWidget );
	//
	//	all params go in a controlWidget (docked)
	//
	_dockWidget = new QDockWidget( "Controls", this );
	_dockWidget->setObjectName( "ParamDock" );

	_controlWidget = new QTabWidget( this );
	_paramWidget = new QWidget( this );
	_paramLayout = new QGridLayout;
	_paramWidget->setLayout( _paramLayout );
	_heatmapWidget = new QWidget( this );
	_heatmapLayout = new QGridLayout;
	_heatmapWidget->setLayout( _heatmapLayout );
	_outputWidget = new QWidget( this );
	_outputLayout = new QGridLayout;
	_outputWidget->setLayout( _outputLayout );

	_controlWidget->addTab( _paramWidget, "Parameters" );
	_controlWidget->addTab( _heatmapWidget, "Heatmap" );
	_controlWidget->addTab( _outputWidget, "Output" );
	_dockWidget->setWidget( _controlWidget );
        addDockWidget( Qt::LeftDockWidgetArea, _dockWidget );

	// now in GH::MainWindow::createParamView();
	configureParams();
	layoutParamParams( _paramLayout );
	layoutHeatmapParams( _heatmapLayout );
	layoutOutputParams( _outputLayout );

	setupActionToolBar();
	setupMenu();

	connect( this, SIGNAL( statusMessage(QString,int) ),
	 statusBar(), SLOT( showMessage(QString,int) ) );
	setState( VP::Load );

	connect( _myApp, SIGNAL( setState( VP::State ) ),
	 this, SLOT( setState( VP::State ) ) );

	// from GH::MainWindow

	resetDefaultsAction()->setShortcut( QKeySequence( tr( "Ctrl+D" ) ) );
	resetDefaultsAction()->setToolTip(
	 "Set all parameters to default settings" );

	// PNAME
	ActionView::Action( this, "help" )->setShortcut(
	 QKeySequence( QKeySequence::HelpContents ) );
}
void	Window::centralGridClicked(
	 const QVariant& row, const QVariant& column )
{
	emit(
	 statusMessage(
		 _myApp->getCalcInfo( S( row ), S( column ), _state ) ) );
}
void	Window::enableParams( const QStringList& names )
{
	foreach( QString p, names ) {
		param( p )->setEnabled( true );
	}
}
void	Window::disableParams( const QStringList& names )
{
	foreach( QString p, names ) {
		param( p )->setEnabled( false );
	}
}
void	Window::setState( const VP::State& state )
{
	// PNAME
	param( "load" )->setEnabled( false );
	param( "check" )->setEnabled( false );
	param( "run" )->setEnabled( false );
	param( "save" )->setEnabled( false );

	emit( statusMessage( StateString( state ) ) );

	switch( state ) {
		case	VP::Load:
			param( "load" )->setEnabled( true );
			enableParams( _myApp->paramNames( "load" ) );
			disableParams( _myApp->paramNames( "check" ) );
			disableParams( _myApp->paramNames( "run" ) );
			disableParams( _myApp->paramNames( "save" ) );
			break;
		case	VP::Check:
			postload();
			param( "check" )->setEnabled( true );
			disableParams( _myApp->paramNames( "load" ) );
			enableParams( _myApp->paramNames( "check" ) );
			enableParams( _myApp->paramNames( "run" ) );
			disableParams( _myApp->paramNames( "save" ) );
			break;
		case	VP::PostCheck:
// TODO figure out what happens when dialog is closed without buttons
			if( !postcheck() ) {
				setState( VP::Check );
			}
			break;
		case	VP::Run:
			param( "run" )->setEnabled( true );
			disableParams( _myApp->paramNames( "load" ) );
			disableParams( _myApp->paramNames( "check" ) );
			enableParams( _myApp->paramNames( "run" ) );
			disableParams( _myApp->paramNames( "save" ) );
			break;
		case	VP::Save:
			postrun();
			_controlWidget->setCurrentIndex( 1 );
			param( "save" )->setEnabled( true );
			disableParams( _myApp->paramNames( "load" ) );
			disableParams( _myApp->paramNames( "check" ) );
			disableParams( _myApp->paramNames( "run" ) );
			enableParams( _myApp->paramNames( "save" ) );
			break;
		case	VP::Done:
			param( "load" )->setEnabled( true );
			param( "save" )->setEnabled( true );
			enableParams( _myApp->paramNames( "load" ) );
			disableParams( _myApp->paramNames( "check" ) );
			disableParams( _myApp->paramNames( "run" ) );
			enableParams( _myApp->paramNames( "save" ) );
			break;
	}
	_state = state;
}
void	Window::getUserConc()
{
	if( _myApp->gDnaReady() ) {
		_myApp->setParamValue( "check/gdnaconc",
	 	VariantListDialog::VariantList( this,
	 	_myApp->gDnaConcLabels(),
	 	V( _myApp->gDnaConcValues() ) ) );
	}
}

void	Window::postload()
{
	ChoiceView::SetChoices( this, "check/vpacol", _myApp->inputCols() );
	ChoiceView::SetChoices( this, "check/ignorecols", _myApp->inputCols() );
	ChoiceView::SetChoices( this, "check/gdnarows", _myApp->inputRows() );
	ChoiceView::SetChoices( this, "check/ignorerows", _myApp->inputRows() );

	toggleGrid( VP::Input );
}
bool	Window::postcheck()
{
	bool	passed = _myApp->errorState();
	QString	dialogMesg = _myApp->checkMessage();

	QDialog	*d = new QDialog( this );
	QGridLayout *lay = new QGridLayout;
	d->setLayout( lay );

	QScrollArea *sa = new QScrollArea( this );
	QLabel *lbl = new QLabel( dialogMesg, this );
	QPushButton *dbtn;
	QPushButton *cbtn;

	sa->setWidget( lbl );
	cbtn = new QPushButton( tr( "Close" ), this );
	connect( cbtn, SIGNAL( clicked() ), d, SLOT( reject() ) );
	lay->addWidget( sa, 0, 0, 1, -1 );
	if( passed ) {
		dbtn = new QPushButton( tr( "Run" ), this );
		dbtn->setDefault( true );
		connect( dbtn, SIGNAL( clicked() ), d, SLOT( accept() ) );
		connect( dbtn, SIGNAL( clicked() ), d, SLOT( close() ) );
		lay->addWidget( dbtn, 1, 0 );
	} else {
		cbtn->setDefault( true );
	}
	lay->addWidget( cbtn, 1, 1 );

	toggleGrid( VP::Working );

	int dres = d->exec();
	if( dres == QDialog::Accepted ) {
		_myApp->run();
	} else {
		passed = false;
	}
	return( passed );
}
void	Window::postrun()
{
	toggleGrid( VP::CqRna );
}
void	Window::setupActionToolBar()
{
	_gridToggle = new QToolButton( this );
	_gridToggle->setToolButtonStyle( Qt::ToolButtonTextOnly );
	_gridToggle->setPopupMode( QToolButton::MenuButtonPopup );
	_gridToggle->setText( tr( "Toggle Grid" ) );
	QMenu	*gridMenu = new QMenu( tr( "Show Grid" ), this );
	gridMenu->addAction( ActionView::Action( this, "toggle/input" ) );
	gridMenu->addAction( ActionView::Action( this, "toggle/cqrna" ) );
	gridMenu->addAction( ActionView::Action( this, "toggle/cqdna" ) );
	gridMenu->addAction( ActionView::Action( this, "toggle/pctdna" ) );
	gridMenu->addAction( ActionView::Action( this, "toggle/score" ) );
	_gridToggle->setMenu( gridMenu );

	_gridDisplayed = new QLabel( tr( "Current: " ), this );

	_actionToolBar = addToolBar( tr( "Actions" ) );
	_actionToolBar->setObjectName( "ActionToolBar" );
	_actionToolBar->addAction( ActionView::Action( this, "load" ) );
	_actionToolBar->addWidget( new QLabel( "->", this ) );
	_actionToolBar->addAction( ActionView::Action( this, "check" ) );
	_actionToolBar->addWidget( new QLabel( "->", this ) );
	_actionToolBar->addAction( ActionView::Action( this, "run" ) );
	_actionToolBar->addWidget( new QLabel( "->", this ) );
	_actionToolBar->addAction( ActionView::Action( this, "save" ) );
	
	_actionToolBar->addSeparator();
	_actionToolBar->addAction( ActionView::Action( this, "reload" ) );
	_actionToolBar->addSeparator();
	_actionToolBar->addAction( ActionView::Action( this, "recheck" ) );
	_actionToolBar->addSeparator();
	_actionToolBar->addWidget( _gridToggle );
	_actionToolBar->addSeparator();
	_actionToolBar->addWidget( _gridDisplayed );
	_actionToolBar->addSeparator();
	_actionToolBar->addAction( ActionView::Action( this, "help" ) );

	_actionToolBar->addAction( resetDefaultsAction() );
}
void	Window::setupMenu()
{
	_viewMenu = menuBar()->addMenu( tr( "View" ) );
	_viewMenu->addAction( ActionView::Action( this, "view/cqrna" ) );
	_viewMenu->addAction( ActionView::Action( this, "view/cqdna" ) );
	_viewMenu->addAction( ActionView::Action( this, "view/pctdna" ) );
	_viewMenu->addAction( ActionView::Action( this, "view/score" ) );
	_viewMenu->addAction( ActionView::Action( this, "view/heatmap" ) );
}
void	Window::configureParams()
{
	//	ACTIONS
	//
	ActionView::AddListener( this, "load", _myApp, SLOT( load() ) );
	ActionView::AddListener( this, "check", _myApp, SLOT( check() ) );
	ActionView::AddListener( this, "run", _myApp, SLOT( run() ) );
	ActionView::AddListener( this, "save", _myApp, SLOT( save() ) );
	ActionView::AddListener( this, "reload", _myApp, SLOT( reload() ) );
	ActionView::AddListener( this, "recheck", _myApp, SLOT( recheck() ) );

	ActionView::AddListener( this, "toggle/input", this, SLOT( toggleInput() ) );
	ActionView::AddListener( this, "toggle/cqrna", this, SLOT( toggleCqRna() ) );
	ActionView::AddListener( this, "toggle/cqdna", this, SLOT( toggleCqDna() ) );
	ActionView::AddListener( this, "toggle/pctdna", this, SLOT( togglePctDna() ) );
	ActionView::AddListener( this, "toggle/score", this, SLOT( toggleScore() ) );

	ActionView::AddListener( this, "view/cqrna", this, SLOT( viewCqRna() ) );
	ActionView::AddListener( this, "view/cqdna", this, SLOT( viewCqDna() ) );
	ActionView::AddListener( this, "view/pctdna", this, SLOT( viewPctDna() ) );
	ActionView::AddListener( this, "view/score", this, SLOT( viewScore() ) );
	ActionView::AddListener( this, "view/heatmap", this, SLOT( viewHeatMap() ) );

	ActionView::AddListener( this, "check/gdnaconc", this, SLOT( getUserConc() ) );

	ActionView::AddListener( this, "save/autoname", _myApp, SLOT( autoNameOutput() ) );

	ActionView::AddListener( this, "help", this, SLOT( showHelp() ) );
	//	CHOICES
	//
	ChoiceView::SetChoices( this, "load/format",
	  _myApp->inputFormatChoice() );

	//	FILES
	//
        FileView::Configure( this, "load/file", FileView::Open,
         "Select your text input file", "*.txt" );

        FileView::Configure( this, "save/folder", FileView::Dir,
         "Select a folder for all output files", "" );

        FileView::Configure( this, "save/allinone", FileView::Save,
         "Select a file to save all results", "" );
	param( "save/allinone" )->setCheckable( true );

        FileView::Configure( this, "save/cqrna", FileView::Save,
         "Select a file to save CqRNA values", "" );
	param( "save/cqrna" )->setCheckable( true );

        FileView::Configure( this, "save/cqdna", FileView::Save,
         "Select a file to save CqDNA values", "" );
	param( "save/cqdna" )->setCheckable( true );

        FileView::Configure( this, "save/pctdna", FileView::Save,
         "Select a file to save %-DNA values", "" );
	param( "save/pctdna" )->setCheckable( true );

        FileView::Configure( this, "save/vpscore", FileView::Save,
         "Select a file to save VPScore values", "" );
	param( "save/vpscore" )->setCheckable( true );

        FileView::Configure( this, "save/summary", FileView::Save,
         "Select a file to save GOI-Summary", "" );
	param( "save/summary" )->setCheckable( true );

        FileView::Configure( this, "save/report", FileView::Save,
         "Select a file to save Log-Report", "" );
	param( "save/report" )->setCheckable( true );
}
void	Window::showHelp()
{
	if( _help ) {
		_help->show();
		return;
	}
	if( _helpText.isEmpty() ) {
		_helpText = Ifp::load( ":help.html" );
	}
	_help = new QWidget( 0, Qt::Window );

	QGridLayout *lay = new QGridLayout;
	QScrollArea	*sa = new QScrollArea;

	QLabel		*lbl = new QLabel( this );

	lbl->setText( _helpText );
	lbl->setWordWrap( true );
	sa->setWidget( lbl );
	lay->addWidget( sa, 0, 0 );
	_help->setLayout( lay );

	_help->setObjectName( "HelpWidget" );
	_help->show();
}
void	Window::viewHeatMap()
{
	switch( _state ) {
		case	VP::Load:
		case	VP::Check:
		//case	VP::Run:
			return;
			break;
		default:
			break;
	}
/*
	ColorMap	cmap;

	cmap.addStep( 0.0001, param( "heatmap/coloraplus" )->value().value<QColor>() );
	cmap.addStep( D( param( "run/gradea" )->value() ),
	 param( "heatmap/colora" )->value().value<QColor>() );
	cmap.addStep( D( param( "run/gradeb" )->value() ),
	 param( "heatmap/colorb" )->value().value<QColor>() );
	cmap.addStep( D( param( "run/gradec" )->value() ),
	 param( "heatmap/colorc" )->value().value<QColor>() );
	cmap.addStep( D( param( "GradeF" )->value() ),
	 param( "heatmap/colorf" )->value().value<QColor>() );
*/

	_myApp->prepareHeatMap();

	HeatMap::Display( _myApp->header( VP::Score ),
	 _myApp->verticalHeader( VP::Score ), _myApp->data( VP::Score ),
	 "Heat Map of %-DNA", _myApp->getColorMap(), 0 );
}
void	Window::viewCqRna()
{
	viewGrid( VP::CqRna );
}
void	Window::viewCqDna()
{
	viewGrid( VP::CqDna );
}
void	Window::viewPctDna()
{
	viewGrid( VP::PctDna );
}
void	Window::viewScore()
{
	viewGrid( VP::Score );
}
void	Window::toggleInput()
{
	toggleGrid( VP::Input );
}
void	Window::toggleCqRna()
{
	toggleGrid( VP::CqRna );
}
void	Window::toggleCqDna()
{
	toggleGrid( VP::CqDna );
}
void	Window::togglePctDna()
{
	toggleGrid( VP::PctDna );
}
void	Window::toggleScore()
{
	toggleGrid( VP::Score );
}
void	Window::viewGrid( const VP::DataRole& role )
{
	if( _state == VP::Load || _state == VP::Check ) {
		showMessage( Warning( "Program must be run first" ) );
		return;
	}
	GridWidget::ShowTable( _myApp->header( role ),
	_myApp->verticalHeader( role ), _myApp->data( role ), RoleString( role ) );
}
void	Window::toggleGrid( const VP::DataRole& role )
{
	_gridDisplayed->setText( QString( "Current: %1" ).arg( RoleString( role ) ) );
	_gridWidget->setData( _myApp->header( role ),
	_myApp->verticalHeader( role ), _myApp->data( role ) );
}
QGroupBox*	Window::groupBox( const QString& title,
		 const QStringList& paramNames )
{
	QGroupBox	*rv = new QGroupBox( title, this );
	QGridLayout	*lay = new QGridLayout;
	rv->setLayout( lay );
	pui( lay, paramNames );
	return( rv );
}
void	Window::layoutParamParams( QGridLayout* layout )
{
	int	row = 0;

	layout->addWidget(
	 groupBox( "Load", _myApp->paramNames( "load" ) ), row++, 0 );
	layout->addWidget(
	 groupBox( "Check", _myApp->paramNames( "check" ) ), row++, 0 );
	layout->addWidget(
	 groupBox( "Run", _myApp->paramNames( "run" ) ), row++, 0 );
}
void	Window::layoutHeatmapParams( QGridLayout* layout )
{
	int	row = 0;

	layout->addWidget(
	 groupBox( "Display Settings", _myApp->paramNames( "heatmap" ) ), row++, 0 );
}
void	Window::layoutOutputParams( QGridLayout* layout )
{
	int	row = 0;
	layout->addWidget(
	 groupBox( "Result Files", _myApp->paramNames( "save" ) ), row++, 0 );
}
}	//	GH namespace
