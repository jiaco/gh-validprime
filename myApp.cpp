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

	//	LOAD
	_loadParams << "InputFile" << "InputFormat" << "GridFont";

	addParam( "GridFont", "",
	 ParamModel::Font,
	 tr( "Table Font" ) );

	addParam( "InputFile", "",
	 ParamModel::File,
	 tr( "Input File"  ) );
	addParam( "InputFormat", VP::FLUID,
	 ParamModel::Choice,
	 tr( "Input Format" ) );

	//	CHECK
	_checkParams
		<< "LOD" << "FailFlag" << "NoampFlag"
		<< "VpaCol" << "gDnaRows" << "gDnaConc"
		<< "IgnoreRows" << "IgnoreCols"
		<< "MinCountSdNa" << "CutoffSdNa";

	addParam( "LOD", "40.0",
	 ParamModel::Edit,
	 tr( "Detection Limit (LOD)" ) );

	addParam( "FailFlag", "FAIL",
	 ParamModel::Edit,
	 tr( "Flag for EXPFAIL" ) );

	addParam( "NoampFlag", "NOAMP",
	 ParamModel::Edit,
	 tr( "Flag for NOAMP" ) );

	addParam( "VpaCol", "m1qB_2",
	 ParamModel::Choice | ParamModel::Selector,
		tr( "Validprime Assay" ) );
	addParam( "gDnaRows", debugRows,
	 ParamModel::Choice | ParamModel::Selector,
		tr( "gDNA Samples" ) );
	addParam( "gDnaConc", "", ParamModel::Action,
		tr( "Check/Set [gDNA]" ) );
/*
	addParam( "gDnaRows", "_76_gDNA 5000",
	 ParamModel::Choice | ParamModel::Selector,
		tr( "gDNA Samples" ) );
*/
	addParam( "IgnoreRows", "",
	 ParamModel::Choice | ParamModel::Selector,
		tr( "Ignore Samples" ) );
	addParam( "IgnoreCols", "",
	 ParamModel::Choice | ParamModel::Selector,
		tr( "Ignore Assays" ) );

	//	TODO are these params used? or tobedeleted?
	//
	addParam( "MinCountSdNa", "3", 
	 ParamModel::Edit, "Delta-Cq Min-Count" );
	addParam( "CutoffSdNa", "0.3", 
	 ParamModel::Edit, "Delta-Cq SD-Cutoff" );
	addParam( "CutHighAll", "0.3",
	 ParamModel::Edit, "High All-SD Limit" );
	addParam( "CutHighLoo", "0.2",
	 ParamModel::Edit, "High LOO-SD Limit" );
	addParam( "CutMedAll", "0.45",
	 ParamModel::Edit, "Medium All-SD Limit" );
	addParam( "CutMedLoo", "0.3",
	 ParamModel::Edit, "Medium LOO-SD Limit" );

	//	RUN
	_runParams << "GradeA" << "GradeB"
		<< "GradeC";
		//<< "MinCountSdRna" << "CutoffSdRna";

	// PERCENT NOW STORED AS PERCENT
	addParam( "GradeA", "3",
	 ParamModel::Edit,
	 tr( "Grade A max" ) );
	addParam( "GradeB", "25",
	 ParamModel::Edit,
	 tr( "Grade B max" ) );
	addParam( "GradeC", "50",
	 ParamModel::Edit,
	 tr( "Grade C max" ) );

/*
	addParam( "MinCountSdRna", "3", 
	 ParamModel::Edit, "Delta-CqRNA Min-Count" );
	addParam( "CutoffSdRna", "0.4", 
	 ParamModel::Edit, "Delta-CqRNA SD-Cutoff" );
*/

	//	SAVE
	_saveParams << "OutputFolder" << "AutoNameOutput"
	<< "FileAllInOne" << "FileCqRNA"
	<< "FileCqDNA" << "FilePctDNA" << "FileVPScore"
	<< "FileSummary" << "FileReport";

	addParam( "OutputFolder", "",
	 ParamModel::File, tr( "Output Folder"  ) );
	addParam( "AutoNameOutput", "",
	 ParamModel::Action, tr( "Auto-Name" ) );

	addParam( "FileAllInOne", "",
	 ParamModel::File, tr( "All-in-One File"  ) );

	addParam( "FileCqRNA", "",
	 ParamModel::File, tr( "CqRNA File"  ) );
	addParam( "FileCqDNA", "",
	 ParamModel::File, tr( "CqDNA File"  ) );
	addParam( "FilePctDNA", "",
	 ParamModel::File, tr( "PctDNA File"  ) );
	addParam( "FileVPScore", "",
	 ParamModel::File, tr( "VPScore File"  ) );
	addParam( "FileSummary", "",
	 ParamModel::File, tr( "GOI-Summary File"  ) );
	addParam( "FileReport", "",
	 ParamModel::File, tr( "Report File(Log)"  ) );

	//	ACTIONS
	//
	addParam( "Load", "", ParamModel::Action, tr( "Load" ) );
	addParam( "Check", "", ParamModel::Action, tr( "Check" ) );
	addParam( "Run", "", ParamModel::Action, tr( "Run" ) );
	addParam( "Save", "", ParamModel::Action, tr( "Save" ) );
	addParam( "Reload", "", ParamModel::Action, tr( "Reload" ) );
	addParam( "Recheck", "", ParamModel::Action, tr( "Recheck" ) );
	addParam( "Help", "", ParamModel::Action, tr( "Help" ) );

	addParam( "ToggleInput", "", ParamModel::Action, tr( "Input" ) );
	addParam( "ToggleCqRNA", "", ParamModel::Action, tr( "CqRNA" ) );
	addParam( "ToggleCqDNA", "", ParamModel::Action, tr( "CqDNA" ) );
	addParam( "TogglePctDNA", "", ParamModel::Action, tr( "%-DNA" ) );
	addParam( "ToggleScore", "", ParamModel::Action, tr( "VPScore" ) );

	addParam( "ViewCqRNA", "", ParamModel::Action, tr( "View CqRNA" ) );
	addParam( "ViewCqDNA", "", ParamModel::Action, tr( "View CqDNA" ) );
	addParam( "ViewPctDNA", "", ParamModel::Action, tr( "View %-DNA" ) );
	addParam( "ViewScore", "", ParamModel::Action, tr( "View VPScore" ) );
	addParam( "ViewHeatMap", "", ParamModel::Action, tr( "View HeatMap" ) );

	//	HEATMAP
	//
	addParam( "heatmap/coloraplus", "#33ff33",
	 ParamModel::Color, tr( "Color A+" ) );
	addParam( "heatmap/colora", "#009900",
	 ParamModel::Color, tr( "Color A" ) );
	addParam( "heatmap/colorb", "#ffcc66",
	 ParamModel::Color, tr( "Color B" ) );
	addParam( "heatmap/colorc", "#ff6600",
	 ParamModel::Color, tr( "Color C" ) );
/*
	addParam( "heatmap/colorf", "#cc0000",
	 ParamModel::Color, tr( "Color F" ) );
*/

	setRequiredParamNames(
	 QStringList()
	 << "InputFile"
	);

	_isGui = !checkArgs();
}
void	MyApp::autoNameOutput()
{
	QString	suffix = "txt";

	QString	baseDir = S( param( "OutputFolder" )->value() );
	if( baseDir.isEmpty() ) {
		emit( emitError( Warning( "Output Folder not set!" ) ) );
		return;
	}
	QString	inputName = BaseName( S( param( "InputFile" )->value() ) );
	if( inputName.isEmpty() ) {
		emit( emitError( Warning( "Input File not set!" ) ) );
		return;
	}
	foreach( QString f, saveParams() ) {
		if( !f.startsWith( "File" ) ) {
			continue;
		}
		QString mid = f.mid( 4 );
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
	//
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
	clearError();

	if( !store.preload( this ) || !store.load() ) {
		setError( store.error() );
		store.clearError();
		emit( emitError( error() ) );
		return( false );
	}
	emit( setState( VP::Check ) );
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

	foreach( QString f, saveParams() ) {
		if( !f.startsWith( "File" ) ) {
			continue;
		}
		QString path = S( param( f )->value() );
		if( path.isEmpty() ) {
			continue;
		}
		if( param( f )->isChecked() ) {
			if( f.contains( "AllInOne" ) ) {
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
				role = VP::CqRna1;
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
			} else if( f.contains( "CqRNA" ) ) {
				role = VP::CqRna;
			} else if( f.contains( "CqDNA" ) ) {
				role = VP::CqDna;
			} else if( f.contains( "PctDNA" ) ) {
				role = VP::PctDna;
			} else if( f.contains( "Score" ) ) {
				role = VP::Score;
			} else if( f.contains( "Summary" ) ) {
				store.saveSummary( path );
				continue;
			} else if( f.contains( "Report" ) ) {
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
