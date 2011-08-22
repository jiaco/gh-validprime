#include "myApp.h"

namespace	GH
{
	MyApp::MyApp( int argc, char** argv )
	: CliApp( argc, argv )
{
	cout.open( STDOUT );
	cerr.open( STDERR );

	_inputFormatChoice << VP::FLUID
	 << VP::SIMPLE << VP::STEPONE << VP::CUSTOM;

	QString	debugRow = "_76_gDNA 5000";
	QString	debugRows = "_75_gDNA 10000,_76_gDNA 5000,_77_gDNA 2500,_78_gDNA 500,_79_gDNA 50,_80_gDNA 5";


	// TODO this is not really used, just to test
	addParam( "GridFont", "",
	 ParamModel::Font,
	 tr( "Table Font" ) );

	//
	//	LOAD
	//
	addParam( "load/file", "",
	 ParamModel::File,
	 tr( "Input File"  ) );
	addParam( "load/format", VP::FLUID,
	 ParamModel::Choice,
	 tr( "Input Format" ) );

	//
	//	CHECK
	//
	addParam( "check/validate", "true",
	 ParamModel::Boolean,
	 tr( "Validate Assays? (Is a gDna dilution series present?)" ) );

	addParam( "check/lod", "40.0",
	 ParamModel::Edit,
	 tr( "Detection Limit (LOD)" ) );

	addParam( "check/failflag", "FAIL",
	 ParamModel::Edit,
	 tr( "Flag for EXPFAIL" ) );

	addParam( "check/noampflag", "NOAMP",
	 ParamModel::Edit,
	 tr( "Flag for NOAMP" ) );

	addParam( "check/vpacol", "m1qB_2",
	 ParamModel::Choice | ParamModel::Selector,
		tr( "Validprime Assay" ) );
	addParam( "check/gdnarows", debugRows,
	 ParamModel::Choice | ParamModel::Selector,
		tr( "gDNA Samples" ) );
	addParam( "check/gdnaconc", "", ParamModel::Action,
		tr( "Check/Set [gDNA]" ) );
	addParam( "check/ignorecols", "",
	 ParamModel::Choice | ParamModel::Selector,
		tr( "Ignore Assays" ) );
	addParam( "check/ignorerows", "",
	 ParamModel::Choice | ParamModel::Selector,
		tr( "Ignore Samples" ) );

	//	TODO are these params used? or tobedeleted?
	//

	addParam( "check/minsdcount", "3", 
	 ParamModel::Edit, "Min-Count for SD" );
	addParam( "check/highallsdcutoff", "0.3",
	 ParamModel::Edit, "High All-SD Cutoff" );
	addParam( "check/highloosdcutoff", "0.2",
	 ParamModel::Edit, "High LOO-SD Cutoff" );

	addParam( "check/performloo", "false",
	 ParamModel::Boolean, "Perform LOO" );
/*
	addParam( "CutMedAll", "0.45",
	 ParamModel::Edit, "Medium All-SD Limit" );
	addParam( "CutMedLoo", "0.3",
	 ParamModel::Edit, "Medium LOO-SD Limit" );

	// older check params

	addParam( "MinCountSdNa", "3", 
	 ParamModel::Edit, "Delta-Cq Min-Count" );
	addParam( "CutoffSdNa", "0.3", 
	 ParamModel::Edit, "Delta-Cq SD-Cutoff" );
*/

	//
	//	RUN
	//
	addParam( "run/gradea", "3",
	 ParamModel::Edit,
	 tr( "Grade A max" ) );
	addParam( "run/gradeb", "25",
	 ParamModel::Edit,
	 tr( "Grade B max" ) );
	addParam( "run/gradec", "60",
	 ParamModel::Edit,
	 tr( "Grade C max" ) );

/*
	addParam( "MinCountSdRna", "3", 
	 ParamModel::Edit, "Delta-CqRNA Min-Count" );
	addParam( "CutoffSdRna", "0.4", 
	 ParamModel::Edit, "Delta-CqRNA SD-Cutoff" );
*/

	//	SAVE
/*
	_saveParams << "OutputFolder" << "AutoNameOutput"
	<< "FileAllInOne" << "FileCqRNA"
	<< "FileCqDNA" << "FilePctDNA" << "FileVPScore"
	<< "FileSummary" << "FileReport";
*/

	addParam( "save/folder", "",
	 ParamModel::File, tr( "Output Folder"  ) );
	addParam( "save/autoname", "",
	 ParamModel::Action, tr( "Auto-Name" ) );

	addParam( "save/allinone", "",
	 ParamModel::File, tr( "All-in-One File"  ) );

	addParam( "save/cqrna", "",
	 ParamModel::File, tr( "CqRNA File"  ) );
	addParam( "save/cqdna", "",
	 ParamModel::File, tr( "CqDNA File"  ) );
	addParam( "save/pctdna", "",
	 ParamModel::File, tr( "PctDNA File"  ) );
	addParam( "save/vpscore", "",
	 ParamModel::File, tr( "VPScore File"  ) );
	addParam( "save/summary", "",
	 ParamModel::File, tr( "GOI-Summary File"  ) );
	addParam( "save/report", "",
	 ParamModel::File, tr( "Report File(Log)"  ) );

	//	ACTIONS
	//
	addParam( "load", "", ParamModel::Action, tr( "Load" ) );
	addParam( "check", "", ParamModel::Action, tr( "Check" ) );
	addParam( "run", "", ParamModel::Action, tr( "Run" ) );
	addParam( "save", "", ParamModel::Action, tr( "Save" ) );
	addParam( "reload", "", ParamModel::Action, tr( "Reload" ) );
	addParam( "recheck", "", ParamModel::Action, tr( "Recheck" ) );
	addParam( "help", "", ParamModel::Action, tr( "Help" ) );

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
	addParam( "heatmap/colorhighsd", "#330000",
	 ParamModel::Color, tr( "Color HIGHSD" ) );
	addParam( "heatmap/colorexpfail", "#000000",
	 ParamModel::Color, tr( "Color EXPFAIL" ) );
	addParam( "heatmap/colornoamp", "#000000",
	 ParamModel::Color, tr( "Color NOAMP" ) );
	addParam( "heatmap/coloroverlod", "#000000",
	 ParamModel::Color, tr( "Color OVERLOD" ) );
/*
	addParam( "heatmap/colorf", "#cc0000",
	 ParamModel::Color, tr( "Color F" ) );
*/

	setRequiredParamNames(
	 QStringList()
	 << "load/file"
	);

	_isGui = !checkArgs();
}
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
		emit( emitError( Warning( "Input File not set!" ) ) );
		return;
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

	if( store.gDnaConcLabels().size() == 0 ) {
		if( !store.precheck( this ) ) {
			setError( store.error() );
			store.clearError();
			emit( emitError( error() ) );
			return( false );
		}
	}
	return( true );
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
	fp << "SECTION\t" << RoleString( role ) << endl;

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
					return( false );
				}
				role = VP::InOut;
				writeData( fp, role, true );
				fp << endl;
				fp << "SECTION\t" << "GoiSummary Summary" << endl;
				store.showGoiSummarySummary( fp );
				fp << endl;
				fp << "SECTION\t" << "GoiSummary" << endl;
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
