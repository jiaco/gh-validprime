#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: CliApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	_inputFormatChoice
		<< VP::FLUID
		<< VP::SIMPLE
		<< VP::STEPONE
		<< VP::CUSTOM;

	//
	//	LOAD
	//
	addParam( "_load/demofile", ParamModel::File,
	 "displayName=Demo Input;"
	 "defaultValue=:sample1.txt;"
	 );
	addParam( "_load/demoformat", ParamModel::Choice,
	 "diplayName=Demo Format;"
	 "defaultValue=" % VP::SIMPLE % ";"
	 );
	addParam( "load/demo1", ParamModel::Action,
	 "displayName=Small Demo;"
	 );
	addParam( "load/demo2", ParamModel::Action,
	 "displayName=BioMark Demo;"
	 );
	addParam( "load/file", ParamModel::File,
	 "displayName=Input File;"
	 "toolTip=Text file with PCR data;"
	 "whatsThis=Input file must be text and formatted"
	 " according to the specification detailed in the Help;"
	 "dialogType=open;"
	 "caption=Select a text file containing input;"
	 "filter=*.txt;"
	);
	addParam( "load/format", ParamModel::Choice,
	 "displayName=Format;"
	 "defaultValue=" % VP::SIMPLE % ";"
	 "toolTip=Specify the format of the input file;"
	 "whatsThis=For a description of the possible input formats"
	 " please see the Help;"
	);
	addParam( "load/autoparse", ParamModel::Boolean,
	 "displayName=Auto find VPA/gDNA;"
	 "defaultValue=false;"
	 "toolTip=Select this if 1 column contains 'VPA' and rows contain 'gDNA';"
	 "whatsThis=The program can find the VPA column if only one column label contains the text 'VAP and can find the gDNA rows(s) provided the text gDNA is present in each appropriate row;"
	);
	
	addParam( "load/configurestepone", ParamModel::Action,
	 "displayName=Configure StepOne;" );

	//
	//	CHECK
	//
	addParam( "check/validate", ParamModel::Boolean,
	 "defaultValue=true;"
	 "displayName=" % tr( "Validate Assays? (Is a gDna dilution series present?)" )  % ";"
	 "toolTip=Only check this if you have 4 or more gDna samples;"
	 "whatsThis=In order to best correct for gDna...more text soon!;"
	);

	addParam( "check/lod", ParamModel::Edit,
	 "defaultValue=45.0;"
	 "displayName=Cq cutoff;"
	 "toolTip=Cutoff whereby Cq values will be flagged LOWSIGNAL;"
	 "whatsThis=High Cq values can be filtered by setting this value."
	 " The default value of 45.0 serves to disable this parameter.;"
	 );

	addParam( "check/failflag", ParamModel::Edit,
	 "defaultValue=FAIL;"
	 "displayName=User-flag for EXPFAIL;" 
	 "toolTip=User defined string that will mark a cell as EXPFAIL;"
	 "whatsThis=If for any reason certain cells should be treated as EXPFAIL,"
	 " replaces those cells with a specific string (for example 'FAIL')"
	 " and provide that string for this parameter;"
	);

	addParam( "check/noampflag", ParamModel::Edit,
	 "defaultValue=NOAMP;"
	 "displayName=Flag for NOAMP;"
	 "toolTip=User defined string that will mark a cell as NOAMP;"
	 "whatsThis=If for any reason certain cells should be treated as NOAMP,"
	 " replaces those cells with a specific string (for example 'NOAMP')"
	 " and provide that string for this parameter;"
	 );

	addParam( "check/vpacol", ParamModel::Choice | ParamModel::Selector,
	 "displayName=" % tr( "Validprime Assay" ) % ";"
	 "toolTip=Click Browse to select the column for the VPA;"
	 "whatsThis=Once input has been loaded, the Browse button will popup"
	 " a selector widget to enable you to select the VPA column from all"
	 " column headings found in the input file;"
	 );

	addParam( "check/gdnarows", ParamModel::Choice | ParamModel::Selector,
	 "displayName=" % tr( "gDNA Sample(s)s" )  % ";"
	 "toolTip=Click Browse to select the rows containing gDNA;"
	 "whatsThis=Once input has been loaded, the Browse button will popup"
	 " a selector widget to enable you to select gDNA row(s) from all"
	 " row headings found in the input file;"
	);

	addParam( "check/parseconc", ParamModel::Boolean,
	 "defaultValue=true;"
	 "displayName=Parse gDNA concentrations from row labels?;"
	 "toolTip=Should the program look in the row strings for numeric concentrations?;"
	 "whatsThis=If you label your gDNA rows with numeric values for each gDNA concentration"
	 " then the program can automatically parse them. Check the values with the button below"
	 " to be sure they were parsed correctly;"
	);

	addParam( "check/gdnaconc", ParamModel::Action,
	 "displayName=Check/Set [gDNA];"
	 "toolTip=Use this button to check and/or set gDNA concentration values;"
	 "whatsThis=Tiggers a popup widget to check/set gDNA concentrations.<br>"
	 " If the program parsed gDNA concentrations from the row labels,"
	 " then you can check those values in the popup widget."
	 " If your row labels do not contain numeric values, you can enter them in the widget.;"
	 );

	addParam( "hidden/gdnaconc", ParamModel::Choice, "" );

	addParam( "check/ignorecols", ParamModel::Choice | ParamModel::Selector,
	"displayName=Ignore Assays;"
	"toolTip=Select any columns that should be ignored;"
	 );

	addParam( "check/ignorerows", ParamModel::Choice | ParamModel::Selector,
	"displayName=Ignore Samples;"
	"toolTip=Select any rows that should be ignored;"
	 );

	addParam( "check/minsdcount", ParamModel::Edit,
	"defaultValue=4;"
	"displayName=Min-Count for SD;"
	"toolTip=Set the minimum number of values required for SD calculations;"
	"whatsThis=Standard Deviations will only be calculated provided there are"
	" at least a minimum number of values (default=4);"
	 );

	addParam( "check/highallsdcutoff", ParamModel::Edit,
	"defaultValue=0.3;"
	"displayName=High All-SD Cutoff;"
	"toolTip=Set threshold for High confidence using all-values-SD;"
	"whatsThis=The Standard Deviation will be compared against this value"
	" in order to determine whether or not the XXX is given High-Confidence;"
	 );

	addParam( "check/performloo", ParamModel::Boolean,
	"defaultValue=false;"
	"displayName=Perform LOO;"
	"toolTip=;"
	"whatsThis=;"
	 );

	addParam( "check/highloosdcutoff", ParamModel::Edit,
	 "defaultValue=0.2;"
	 "displayName=High LOO-SD Cutoff;"
	 "tooltip=;"
	 "whatsThis=;"
	 );

	//
	//	RUN
	//
	addParam( "run/gradea", "3",
	 ParamModel::Edit,
	 tr( "Grade A max" ),
	 "tooltip=;"
	 "whatsThis=;"
	 );
	addParam( "run/gradeb", "25",
	 ParamModel::Edit,
	 tr( "Grade B max" ),
	 "tooltip=;"
	 "whatsThis=;"
	 );
	addParam( "run/gradec", "60",
	 ParamModel::Edit,
	 tr( "Grade C max" ),
	 "tooltip=;"
	 "whatsThis=;"
	 );
	addParam( "run/correcta", ParamModel::Boolean,
	 "defaultValue=false;"
	 "displayName=Correct A Samples? (not recommended);"
	 "tooltip=;"
	 "whatsThis=;"
	 );

	//
	//	SAVE
	//
	addParam( "save/folder", ParamModel::File,
	 "displayName=Output Folder;" 
	 "dialogType=dir;"
	 "caption=Select a folder for all output files;"
	 );
	addParam( "save/autoname", ParamModel::Action,
	 "displayName=Auto-Name;"
	 "toolTip=Use the output folder plus the input file name to auto-create output file names;"
	 "whatsThis=Using the file name of the input file and the output folder, the program"
	 " will automatically generate all file names for the various output files;"
	 );

	addParam( "save/allinone", ParamModel::File,
	 "displayName=All-in-One File;"
	 "dialogType=save;"
	 "caption=Select an output file for the All-in-One file;"
	 "checkable=true;"
	 );

	addParam( "save/cqrna", ParamModel::File,
	 "displayName=CqRNA File;"
	 "dialogType=save;"
	 "caption=Select an output file for the CqRNA values;"
	 "checkable=true;"
	 );

	addParam( "save/cqdna", ParamModel::File,
	 "displayName=CqDNA File;"
	 "dialogType=save;"
	 "caption=Select an output file for the CqDNA values;"
	 "checkable=true;"
	 );

	addParam( "save/pctdna", ParamModel::File,
	 "displayName=PctDNA File;"
	 "dialogType=save;"
	 "caption=Select an output file for the %-DNA values;"
	 "checkable=true;"
	 );

	addParam( "save/vpscore", ParamModel::File,
	 "displayName=VPScore File;"
	 "dialogType=save;"
	 "caption=Select an output file for the VPScores;"
	 "checkable=true;"
	 );

	addParam( "save/summary", ParamModel::File,
	 "displayName=GOI-Summary File;"
	 "dialogType=save;"
	 "caption=Select an output file for the GOI-Summary file;"
	 "checkable=true;"
	 );

	addParam( "save/report", ParamModel::File,
	 "displayName=Report File(Log);"
	 "dialogType=save;"
	 "caption=Select an output file for the Report file;"
	 "checkable=true;"
	 );

	//	ACTIONS
	//
	addParam( "load", ParamModel::Action,
	 "displayName=" % tr( "Load" ) % ";"
	 "toolTip=Load the current input file using current format;"
	 "whatsThis=Load is the first step of the workflow and uses a parser"
	 " select by the Format selector and the specified input file;"
	);
	addParam( "check", "", ParamModel::Action, tr( "Check" ) );
	addParam( "run", "", ParamModel::Action, tr( "Run" ) );
	addParam( "save", "", ParamModel::Action, tr( "Save" ) );
	addParam( "reload", "", ParamModel::Action, tr( "Reload" ) );
	addParam( "recheck", "", ParamModel::Action, tr( "Recheck" ) );
/*
	addParam( "help", "", ParamModel::Action, tr( "Help" ) );
	addParam( "about", "", ParamModel::Action, tr( "About" ) );
*/

	addParam( "toggle/input", "", ParamModel::Action, tr( "Input" ) );
	addParam( "toggle/cqrna", "", ParamModel::Action, tr( "CqRNA" ) );
	addParam( "toggle/cqdna", "", ParamModel::Action, tr( "CqDNA" ) );
	addParam( "toggle/pctdna", "", ParamModel::Action, tr( "%-DNA" ) );
	addParam( "toggle/score", "", ParamModel::Action, tr( "VPScore" ) );

	addParam( "view/cqrna", "", ParamModel::Action, tr( "View CqRNA" ) );
	addParam( "view/cqdna", "", ParamModel::Action, tr( "View CqDNA" ) );
	addParam( "view/pctdna", "", ParamModel::Action, tr( "View %-DNA" ) );
	addParam( "view/score", "", ParamModel::Action, tr( "View VPScore" ) );
	addParam( "view/heatmap", "", ParamModel::Action, tr( "View HeatMap" ) );

	//	HEATMAP
	//
	addParam( "heatmap/coloraplus", "#33ff33",
	 ParamModel::Color, tr( "Color A+" ) );
	addParam( "heatmap/colorastar", "#330033",
	 ParamModel::Color, tr( "Color A*" ) );
	addParam( "heatmap/colora", "#009900",
	 ParamModel::Color, tr( "Color A" ) );
	addParam( "heatmap/colorb", "#ffcc66",
	 ParamModel::Color, tr( "Color B" ) );
	addParam( "heatmap/colorc", "#ff9900",
	 ParamModel::Color, tr( "Color C" ) );
	addParam( "heatmap/colorf", "#ff6600",
	 ParamModel::Color, tr( "Color F" ) );
/*	HIGHDNA IS AN F
	addParam( "heatmap/colorhighdna", "#330000",
	 ParamModel::Color, tr( "Color HIGHDNA" ) );
*/
	addParam( "heatmap/colorhighsd", "#330000",
	 ParamModel::Color, tr( "Color HIGHSD" ) );
	addParam( "heatmap/colorexpfail", "#000000",
	 ParamModel::Color, tr( "Color EXPFAIL" ) );
	addParam( "heatmap/colornoamp", "#000000",
	 ParamModel::Color, tr( "Color NOAMP" ) );
	addParam( "heatmap/coloroverlod", "#000000",
	 ParamModel::Color, tr( "Color LOWSIGNAL" ) );

	//
	//	STEPONE
	//
	addParam( "stepone/col_well", ParamModel::Edit,
	 "displayName=Well Column;"
	 "defaultValue=Well;" );
	addParam( "stepone/col_sample", ParamModel::Edit,
	 "displayName=Sample Column;"
	 "defaultValue=Sample Name;" );
	addParam( "stepone/col_target", ParamModel::Edit,
	 "displayName=Target Column;"
	 "defaultValue=Target Name;" );
	addParam( "stepone/col_ct", ParamModel::Edit,
	 "displayName=Ct Column (#);"
	 "defaultValue=6;" );
	addParam( "stepone/noamp_flag", ParamModel::Edit,
	 "displayName=NOAMP Flag;"
	 "defaultValue=undetermined;" );
/*
	addParam( "heatmap/colorf", "#cc0000",
	 ParamModel::Color, tr( "Color F" ) );
*/

	setRequiredParamNames(
	 QStringList()
	 << "load/file"
	);

	_isGui = !checkArgs();

	loadParamHelp( ":/tooltips.txt" );
}
/*
void	MyApp::loadParamHelp( const QString& path )
{
	Ifp	fp;
	QString	line;
	QString	pname, tooltip, whatsthis;

	pname = tooltip = whatsthis = "";

	if( !fp.open( path ) ) {
		return;
	}
	while( nextLine( fp, line ) ) {
		if( line.isEmpty() && !pname.isEmpty() ) {
			setParamHelp( pname, tooltip, whatsthis );
			pname = tooltip = whatsthis = "";
		} else if( pname.isEmpty() ) {
			int x = line.indexOf( ' ' );
			pname = line.left( x );
			tooltip = line.mid( x + 1 );
		} else if( whatsthis.isEmpty() ) {
			whatsthis = line.trimmed();
		} else {
			whatsthis += QString( " %1" ).arg( line.trimmed() );
		}
	}
	if( !pname.isEmpty() ) {
		setParamHelp( pname, tooltip, whatsthis );
	}
	fp.close();
	return;
}
*/
void	MyApp::prepareHeatMap()
{
	store.preheatmap( this );
}
ColorMap* MyApp::getColorMap() const
{
	return( store.colorMap() );
}
void	MyApp::autoNameOutput()
{
	QString	suffix = "txt";

	QString	baseDir = S( param( "save/folder" )->value() );
	if( baseDir.isEmpty() ) {
		emit( emitError( Warning( "Output Folder not set!" ) ) );
		return;
	}
	QString	inputName = BaseName( S( param( "load/file" )->value() ) );
	if( inputName.isEmpty() ) {
		// TODO WARN changed this for 0.9.5 demo version
		inputName = "demo";
		//emit( emitError( Warning( "Input File not set!" ) ) );
		//return;
	}
	foreach( QString f, paramNames( "save" ) ) {
		if( f == "save/folder" || f == "save/autoname" ) {
			continue;
		}
		QString mid = f.mid( 5 );
		param( f )->setValue( 
		 QString( "%1/%2_%3.%4" )
		 .arg( baseDir ).arg( inputName ).arg( mid ).arg( suffix ) );
	}
}
QString	MyApp::getCalcInfo( const QString& row, const QString& column,
	 const VP::State& state ) const
{
	return( store.getCalcInfo( row, column, state ) );
}
QStringList	MyApp::inputFormatChoice() const
{
	return( _inputFormatChoice );
}
bool	MyApp::gDnaReady() {

	//if( store.gDnaConcLabels().size() == 0 ) {
		if( !store.precheck( this ) ) {
			setError( store.error() );
			store.clearError();
			emit( emitError( error() ) );
			return( false );
		}
	//}
	return( true );
}
void	MyApp::gDnaFromUser( const QVariant& values )
{
	// here want to sort based on the input concentrations
	// and update _gDnaRows and _gDnaConcValues
	setParamValue( "hidden/gdnaconc", values );
	store.fillConcMap( this );
}
QStringList	MyApp::gDnaConcLabels() const
{
	return( store.gDnaConcLabels() );
}
QList<QVariant>	MyApp::gDnaConcValues() const
{
	return( store.gDnaConcValues() );
}
	//
	///
	/*
QStringList	MyApp::loadParams() const
{
	return( _loadParams );
}
QStringList	MyApp::checkParams() const
{
	return( _checkParams );
}
QStringList	MyApp::runParams() const
{
	return( _runParams );
}
QStringList	MyApp::saveParams() const
{
	return( _saveParams );
}
*/

QStringList	MyApp::header( const VP::DataRole& role ) const
{
	return( store.header( role ) );
}
QStringList	MyApp::verticalHeader( const VP::DataRole& role ) const
{
	return( store.verticalHeader( role ) );
}
QList<Row>	MyApp::data( const VP::DataRole& role ) const
{
	return( store.data( role ) );
}


QStringList	MyApp::inputRows() const
{
	return( store.inputRows() );
}
QStringList	MyApp::outputRows() const
{
	return( store.outputRows() );
}
QStringList	MyApp::inputCols() const
{
	return( store.inputCols() );
}
QStringList	MyApp::inputHeader() const
{
	return( store.inputHeader() );
}
QStringList	MyApp::outputCols() const
{
	return( store.outputCols() );
}
QList<Row>	MyApp::inputData() const
{
	return( store.inputData() );
}
QString	MyApp::checkMessage() const
{
	return( store.checkMessage() );
}
	//
	//	CONTROLLER METHODS
	//
void	MyApp::doit()
{
	load();
	check();
	run();
}
bool	MyApp::loadDemo()
{
	bool	rv = true;
	clearError();

	if( !store.load( APP_S( "_load/demofile" ),
	 APP_S( "_load/demoformat" ) ) ) {
		setError( store.error() );
		store.clearError();
		if( errorType() == Error::Crit ) {
			rv = false;
		}
		emit( emitError( error() ) );
	}
	if( rv ) {
		setParamValue( "check/validate", true );
		setParamValue( "check/vpacol", "mVPA1" );
		setParamValue( "check/gdnarows",
		V( QStringList()
			<< "gDNA 2400 (#10)"
			<< "gDNA 800 (#9)"
			<< "gDNA 200 (#8)"
			<< "gDNA 50 (#7)"
		 ) );
		emit( setState( VP::Check ) );
	}
	return( true );
}
bool	MyApp::loadDemo1()
{
	setParamValue( "load/file", ":/demo1.txt" );
	setParamValue( "load/format", VP::SIMPLE );
	setParamValue( "load/autoparse", true );
	return( load() );
}
bool	MyApp::loadDemo2()
{
	setParamValue( "load/file", ":/demo2.txt" );
	setParamValue( "load/format", VP::FLUID );
	setParamValue( "load/autoparse", true );
	return( load() );
}
bool	MyApp::load()
{
	bool	rv = true;
	clearError();

	if( !store.preload( this ) || !store.load() ) {
		setError( store.error() );
		store.clearError();
		if( errorType() == Error::Crit ) {
			rv = false;
		}
		emit( emitError( error() ) );
	}
	if( rv ) {
		if( APP_B( "load/autoparse" ) ) {
			QString vpaCol = "";
			QStringList gdnaRow;
			foreach( QString s, store.inputCols() ) {
				if( !s.contains( "VPA",
				 Qt::CaseInsensitive ) ) {
					continue;
				}
				if( !vpaCol.isEmpty() ) {
					setError( Warning( "Multiple columns contain 'VPA' text" ) );
					break;
				} else {
					vpaCol = s;
				}
			}
			foreach( QString s, store.inputRows() ) {
				if( s.contains( "gDNA",
				 Qt::CaseInsensitive ) ) {
					gdnaRow << s;
				}
			}
			if( vpaCol.isEmpty() ) {
				setError( Warning( "No VPA column found" ) );
				return( false );
			}
			if( gdnaRow.isEmpty() ) {
				setError( Warning( "No gDNA row(s) found" ) );
				return( false );
			}
			setParamValue( "check/vpacol", vpaCol );
			setParamValue( "check/gdnarows", gdnaRow );
		}
		emit( setState( VP::Check ) );
	}
	return( true );
}
bool	MyApp::check()
{
	clearError();
	bool	passed = true;
	QString	dialogMesg = "";

	if( !store.precheck( this ) || !store.check() ) {
		setError( store.error() );
		store.clearError();
		emit( emitError( error() ) );
		return( false );
	}
	if( errorType() == Error::Crit ) {
		passed = false;
		dialogMesg = getCheckCriticalMesg();
	} else if( errorType() == Error::Warn ) {
		passed = true;
		dialogMesg = getCheckWarningMesg();
		store.clearError();
		clearError();
	} 
	emit( setState( VP::PostCheck ) );
	return( passed );
}
bool	MyApp::run()
{
	clearError();

	if( !store.prerun( this ) || !store.run() ) {
		setError( store.error() );
		store.clearError();
		emit( emitError( error() ) );
		return( false );
	}
	emit( setState( VP::Save ) );
	return( true );
}
bool	MyApp::saveData( const QString& path, VP::DataRole& role )
{
	Ofp	fp;

	if( fp.open( path ) ) {
		writeData( fp, role );
		fp.close();
		return( true );
	} else {
		setError( fp.error() );
		emit( emitError( error() ) );
	}
	return( false );
}
	//	TODO get rid of the bool thing and separate out the show header
	// and make calls to writeHeader before writeData in save
	// then make a single line just above grid with the
	// content type for allinone
void	MyApp::writeData( QTextStream& fp, VP::DataRole& role, const bool& includeHeader )
{
	QStringList	hdr, vhdr;
	QList<Row>	d;

	hdr = header( role );
	vhdr = verticalHeader( role );
	d = data( role );

	if( includeHeader ) {
		store.outputFileHeader( fp );
	}
	if( role == VP::InOut ) {
		// TODO WARN this was a change made with HL in office
		fp << "SECTION\t" << "CqNA (Input)" << endl;
	} else {
		fp << "SECTION\t" << RoleString( role ) << endl;
	}

	fp << "Sample\t" << hdr.join( "\t" ) << endl;
	for( int i = 0; i < vhdr.size(); ++i ) {
		fp << vhdr.at( i );
		for( int j = 0; j < hdr.size(); ++j ) {
			fp << "\t"
			 << S( d.at( i )[ hdr.at( j ) ] );
		}
		fp << endl;
	}
}
bool	MyApp::save()
{
	VP::DataRole	role;

	foreach( QString f, paramNames( "save" ) ) {
		if( f == "save/folder" || f == "save/autoname" ) {
			continue;
		}
		QString path = S( param( f )->value() );
		if( path.isEmpty() ) {
			continue;
		}
		if( param( f )->isChecked() ) {
			if( f.contains( "allinone" ) ) {
				Ofp	fp;
				if( !fp.open( path ) ) {
					setError( fp.error() );
					emit( emitError( error() ) );
					return( false );
				}
				role = VP::InOut;
				writeData( fp, role, true );
				fp << endl;
				fp << "SECTION\t" << "Goi Summary Summary" << endl;
				store.showGoiSummarySummary( fp );
				fp << endl;
				fp << "SECTION\t" << "Goi Summary" << endl;
				store.showGoiSummaryTransposed( fp );
				fp << endl;
				role = VP::CqRna;
				writeData( fp, role, false );
				fp << endl;
				role = VP::CqDna;
				writeData( fp, role, false );
				fp << endl;
				role = VP::PctDna;
				writeData( fp, role, false );
				fp << endl;
				role = VP::Score;
				writeData( fp, role, false );
				fp << endl;
				fp.close();
				continue;
			} else if( f.contains( "cqrna" ) ) {
				role = VP::CqRna;
			} else if( f.contains( "cqdna" ) ) {
				role = VP::CqDna;
			} else if( f.contains( "pctdna" ) ) {
				role = VP::PctDna;
			} else if( f.contains( "score" ) ) {
				role = VP::Score;
			} else if( f.contains( "summary" ) ) {
				store.saveSummary( path );
				continue;
			} else if( f.contains( "report" ) ) {
				store.saveReport( path );
				continue;
			}
			saveData( path, role );
		}
	}
	emit( setState( VP::Done ) );
	return( true );
}
void	MyApp::reload()
{
	emit( setState( VP::Load ) );
}
void	MyApp::recheck()
{
	emit( setState( VP::Check ) );
}



QString	MyApp::getCheckCriticalMesg()
{
	QString rv = QString( "Critical: %1" ).arg( errorMessage() );
	return( rv );
}
QString	MyApp::getCheckWarningMesg()
{
	QString rv = QString( "Warning: %1" ).arg( errorMessage() );
	return( rv );
}
}	//	GH namespace
