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
	_outputWidget = new QWidget( this );
	_outputLayout = new QGridLayout;
	_outputWidget->setLayout( _outputLayout );

	_controlWidget->addTab( _paramWidget, "Parameters" );
	_controlWidget->addTab( _outputWidget, "Output" );
	_dockWidget->setWidget( _controlWidget );
        addDockWidget( Qt::LeftDockWidgetArea, _dockWidget );


	// now in ghMainWindow
	//	createParamView();
	configureParams();
	layoutParamParams( _paramLayout );
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
	resetDefaultsAction()->setToolTip( "Set all parameters to default settings" );

	ActionView::Action( this, "Help" )->setShortcut( QKeySequence( QKeySequence::HelpContents ) );
}
void	Window::centralGridClicked( const QVariant& row, const QVariant& column )
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
	param( "Load" )->setEnabled( false );
	param( "Check" )->setEnabled( false );
	param( "Run" )->setEnabled( false );
	param( "Save" )->setEnabled( false );
	switch( state ) {
		case	VP::Load:
			emit( statusMessage( "Load" ) );
			param( "Load" )->setEnabled( true );
			enableParams( _myApp->loadParams() );
			disableParams( _myApp->checkParams() );
			disableParams( _myApp->runParams() );
			disableParams( _myApp->saveParams() );
			break;
		case	VP::Check:
			postload();
			emit( statusMessage( "Check" ) );
			param( "Check" )->setEnabled( true );
			disableParams( _myApp->loadParams() );
			enableParams( _myApp->checkParams() );
			disableParams( _myApp->runParams() );
			disableParams( _myApp->saveParams() );
			break;
		case	VP::PostCheck:
			emit( statusMessage( "PostCheck" ) );
			if( !postcheck() ) {
				setState( VP::Check );
			//} else {
			//	setState( VP::Run );
			}
			break;
		case	VP::Run:
			emit( statusMessage( "Run" ) );
			param( "Run" )->setEnabled( true );
			disableParams( _myApp->loadParams() );
			disableParams( _myApp->checkParams() );
			enableParams( _myApp->runParams() );
			disableParams( _myApp->saveParams() );
			break;
		case	VP::Save:
			postrun();
			emit( statusMessage( "Save" ) );
			_controlWidget->setCurrentIndex( 1 );
			param( "Save" )->setEnabled( true );
			disableParams( _myApp->loadParams() );
			disableParams( _myApp->checkParams() );
			disableParams( _myApp->runParams() );
			enableParams( _myApp->saveParams() );
			break;
		default:
			emit( statusMessage( "Done" ) );
			param( "Load" )->setEnabled( true );
			param( "Save" )->setEnabled( true );
			enableParams( _myApp->loadParams() );
			disableParams( _myApp->checkParams() );
			disableParams( _myApp->runParams() );
			enableParams( _myApp->saveParams() );
			break;
	}
	_state = state;
}
void	Window::getUserConc()
{
	if( _myApp->gDnaReady() ) {
		_myApp->setParamValue( "gDnaConc",
	 	VariantListDialog::VariantList( this,
	 	_myApp->gDnaConcLabels(),
	 	V( _myApp->gDnaConcValues() ) ) );
	}
}

void	Window::postload()
{
	ChoiceView::SetChoices( this, "VpaCol", _myApp->inputCols() );
	ChoiceView::SetChoices( this, "IgnoreCols", _myApp->inputCols() );
	ChoiceView::SetChoices( this, "gDnaRows", _myApp->inputRows() );
	ChoiceView::SetChoices( this, "IgnoreRows", _myApp->inputRows() );

	toggleGrid( VP::Input );
}
bool	Window::postcheck()
{
	bool	passed = _myApp->errorState();
	QString	dialogMesg = _myApp->checkMessage();

	QDialog	*d = new QDialog( this );
	QGridLayout *lay = new QGridLayout;
	d->setLayout( lay );

	QLabel *lbl = new QLabel( dialogMesg, this );
	QPushButton *dbtn;
	QPushButton *cbtn;

	cbtn = new QPushButton( "Close", this );
	connect( cbtn, SIGNAL( clicked() ), d, SLOT( reject() ) );
	lay->addWidget( lbl, 0, 0, 1, -1 );
	if( passed ) {
		dbtn = new QPushButton( "Run", this );
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
	_gridToggle->setText( "Toggle Grid" );
	QMenu	*gridMenu = new QMenu( "Show Grid", this );
	gridMenu->addAction( ActionView::Action( this, "ToggleInput" ) );
	gridMenu->addAction( ActionView::Action( this, "ToggleCqRNA" ) );
	gridMenu->addAction( ActionView::Action( this, "ToggleCqDNA" ) );
	gridMenu->addAction( ActionView::Action( this, "TogglePctDNA" ) );
	gridMenu->addAction( ActionView::Action( this, "ToggleScore" ) );
	_gridToggle->setMenu( gridMenu );

	_gridDisplayed = new QLabel( "Current: ", this );

	_actionToolBar = addToolBar( "Actions" );
	_actionToolBar->setObjectName( "ActionToolBar" );
	_actionToolBar->addAction( ActionView::Action( this, "Load" ) );
	_actionToolBar->addWidget( new QLabel( "->", this ) );
	_actionToolBar->addAction( ActionView::Action( this, "Check" ) );
	_actionToolBar->addWidget( new QLabel( "->", this ) );
	_actionToolBar->addAction( ActionView::Action( this, "Run" ) );
	_actionToolBar->addWidget( new QLabel( "->", this ) );
	_actionToolBar->addAction( ActionView::Action( this, "Save" ) );
	
	_actionToolBar->addSeparator();
	_actionToolBar->addAction( ActionView::Action( this, "Reload" ) );
	_actionToolBar->addSeparator();
	_actionToolBar->addAction( ActionView::Action( this, "Recheck" ) );
	_actionToolBar->addSeparator();
	_actionToolBar->addWidget( _gridToggle );
	_actionToolBar->addSeparator();
	_actionToolBar->addWidget( _gridDisplayed );
	_actionToolBar->addSeparator();
	_actionToolBar->addAction( ActionView::Action( this, "Help" ) );

	_actionToolBar->addAction( resetDefaultsAction() );
}
void	Window::setupMenu()
{
	_viewMenu = menuBar()->addMenu( "View" );
	_viewMenu->addAction( ActionView::Action( this, "ViewCqRNA" ) );
	_viewMenu->addAction( ActionView::Action( this, "ViewCqDNA" ) );
	_viewMenu->addAction( ActionView::Action( this, "ViewPctDNA" ) );
	_viewMenu->addAction( ActionView::Action( this, "ViewScore" ) );
	_viewMenu->addAction( ActionView::Action( this, "ViewHeatMap" ) );
}
void	Window::configureParams()
{
	//	ACTIONS
	//
	ActionView::AddListener( this, "Load", _myApp, SLOT( load() ) );
	ActionView::AddListener( this, "Check", _myApp, SLOT( check() ) );
	ActionView::AddListener( this, "Run", _myApp, SLOT( run() ) );
	ActionView::AddListener( this, "Save", _myApp, SLOT( save() ) );
	ActionView::AddListener( this, "Reload", _myApp, SLOT( reload() ) );
	ActionView::AddListener( this, "Recheck", _myApp, SLOT( recheck() ) );

	ActionView::AddListener( this, "ToggleInput", this, SLOT( toggleInput() ) );
	ActionView::AddListener( this, "ToggleCqRNA", this, SLOT( toggleCqRna() ) );
	ActionView::AddListener( this, "ToggleCqDNA", this, SLOT( toggleCqDna() ) );
	ActionView::AddListener( this, "TogglePctDNA", this, SLOT( togglePctDna() ) );
	ActionView::AddListener( this, "ToggleScore", this, SLOT( toggleScore() ) );

	ActionView::AddListener( this, "ViewCqRNA", this, SLOT( viewCqRna() ) );
	ActionView::AddListener( this, "ViewCqDNA", this, SLOT( viewCqDna() ) );
	ActionView::AddListener( this, "ViewPctDNA", this, SLOT( viewPctDna() ) );
	ActionView::AddListener( this, "ViewScore", this, SLOT( viewScore() ) );
	ActionView::AddListener( this, "ViewHeatMap", this, SLOT( viewHeatMap() ) );

	ActionView::AddListener( this, "gDnaConc", this, SLOT( getUserConc() ) );

	ActionView::AddListener( this, "AutoNameOutput", _myApp, SLOT( autoNameOutput() ) );

	ActionView::AddListener( this, "Help", this, SLOT( showHelp() ) );
	//	CHOICES
	//
	ChoiceView::SetChoices( this, "InputFormat",
	  _myApp->inputFormatChoice() );

	//	FILES
	//
        FileView::Configure( this, "InputFile", FileView::Open,
         "Select your text input file", "*.txt" );

        FileView::Configure( this, "OutputFolder", FileView::Dir,
         "Select a folder for all output files", "" );

        FileView::Configure( this, "FileAllInOne", FileView::Save,
         "Select a file to save all results", "" );
	param( "FileAllInOne" )->setCheckable( true );

        FileView::Configure( this, "FileCqRNA", FileView::Save,
         "Select a file to save CqRNA values", "" );
	param( "FileCqRNA" )->setCheckable( true );

        FileView::Configure( this, "FileCqDNA", FileView::Save,
         "Select a file to save CqDNA values", "" );
	param( "FileCqDNA" )->setCheckable( true );

        FileView::Configure( this, "FilePctDNA", FileView::Save,
         "Select a file to save %-DNA values", "" );
	param( "FilePctDNA" )->setCheckable( true );

        FileView::Configure( this, "FileVPScore", FileView::Save,
         "Select a file to save VPScore values", "" );
	param( "FileVPScore" )->setCheckable( true );

        FileView::Configure( this, "FileSummary", FileView::Save,
         "Select a file to save GOI-Summary", "" );
	param( "FileSummary" )->setCheckable( true );

        FileView::Configure( this, "FileReport", FileView::Save,
         "Select a file to save Log-Report", "" );
	param( "FileReport" )->setCheckable( true );
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

	QLabel		*lbl = new QLabel( this );

	lbl->setText( _helpText );
	lbl->setWordWrap( true );
	lay->addWidget( lbl, 0, 0 );
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
	ColorMap	cmap;

	cmap.addStep( 0.0001, param( "heatmap/coloraplus" )->value().value<QColor>() );
	cmap.addStep( D( param( "GradeA" )->value() ),
	 param( "heatmap/colora" )->value().value<QColor>() );
	cmap.addStep( D( param( "GradeB" )->value() ),
	 param( "heatmap/colorb" )->value().value<QColor>() );
	cmap.addStep( D( param( "GradeC" )->value() ),
	 param( "heatmap/colorc" )->value().value<QColor>() );
/*
	cmap.addStep( D( param( "GradeF" )->value() ),
	 param( "heatmap/colorf" )->value().value<QColor>() );
*/

	HeatMap::Display( _myApp->header( VP::PctDna ), _myApp->verticalHeader( VP::PctDna ),
		_myApp->data( VP::PctDna ), "Heat Map of %-DNA", 0, &cmap );
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
	_myApp->verticalHeader( role ), _myApp->data( role ) );
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
	 groupBox( "Load", _myApp->loadParams() ), row++, 0 );
	layout->addWidget(
	 groupBox( "Check", _myApp->checkParams() ), row++, 0 );
	layout->addWidget(
	 groupBox( "Run", _myApp->runParams() ), row++, 0 );
}
void	Window::layoutOutputParams( QGridLayout* layout )
{
	int	row = 0;
	layout->addWidget(
	 groupBox( "Result Files", _myApp->saveParams() ), row++, 0 );
}
}	//	GH namespace
