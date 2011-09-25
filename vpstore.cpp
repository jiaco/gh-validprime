#include "vpstore.h"

using namespace GH;

/*
TempConc::TempConc( const QString& rowLabel )
{
	label = rowLabel;
	value = TempConc::SnarfConc( label );
}
double	TempConc::SnarfConc( const QString& label )
{
	QString s = label;
	QString	t = "";
	if( s.startsWith( '_' ) ) {
		s = s.mid( s.indexOf( '_', 1 ) );
	}
	foreach( QChar ch, s ) {
		if( ch.isDigit() ) {
			t.append( ch );
		} else if( ch == '-' || ch == '.' ) {
			t.append( ch );
		} else if( ch == ',' ) {
			t.append( '.' );
		}
	}
	return( t.toDouble() );
}
bool	TempConcSort( const TempConc& e1, const TempConc& e2 )
{
	return( e1.value < e2.value );
}
*/
bool	DilSeriesSortV( const DilutionPoint& e1, const DilutionPoint& e2 )
{
	return( e1.value() < e2.value() );
}

GoiSummary::GoiSummary()
{
	goi = "ERROR";
	gDnaIndexes.clear();
	_flag = VP::CALC;
	// changing confidence to high versus low
	confidence = VP::NotUsed;
	allSd = looSd = 0.0;
	gDnaOutlier = "ND";
}
void	GoiSummary::ShowHeader( QTextStream& fp )
{
	QStringList	hdr;
	hdr = QString(
	 "GOI,gDNA Index Count,Flag,Confidence,All Sd,LOO Sd,gDNA Outlier"
	 ",Slope,Eff,R2"
	// ",nA+++,nA++,nA+,HIGHSD"
 	 ",nA*,nA,nB,nC,nF,nND"
	 ).split( "," );

	fp << hdr.join( "\t" ) << endl;
}
bool	GoiSummary::isFlagged() const
{
	if( _flag == VP::CALC ) {
		return( false );
	}
	return( true );
}
void	GoiSummary::show( QTextStream& fp )
{
	fp << goi
	 << "\t" << gDnaIndexes.size()
	 << "\t" << GoiFlagString( _flag )
	 << "\t" << ConfidenceString( confidence )
	 << "\t" << allSd
	 << "\t" << looSd
	 << "\t" << gDnaOutlier
	 << "\t" << linReg.slope()
	 << "\t" << linReg.e()
	 << "\t" << linReg.r2()
/*
	 << "\t" << nAA3
	 << "\t" << nAA2
	 << "\t" << nAA
	 << "\t" << nHIGHSD
*/
	 << "\t" << nASTAR
	 << "\t" << nA
	 << "\t" << nB
	 << "\t" << nC
	 << "\t" << nF
	// << "\t" << nHIGHDNA
	 << "\t" << nND
	 << endl;
}
void	VPStore::showGoiSummaryTransposed( QTextStream& fp )
{
	fp << "Sample";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << goi;
	}
	fp << endl;

	fp << "gDnaIndexCount";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].gDnaIndexes.size();
	}
	fp << endl;

	fp << "GoiFlag";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << GoiFlagString( _goiSummary[ goi ]._flag );
	}
	fp << endl;

	fp << "Confidence";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << ConfidenceString( _goiSummary[ goi ].confidence );
	}
	fp << endl;
	fp << "allSd";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].allSd;
	}
	fp << endl;
	fp << "looSd";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].looSd;
	}
	fp << endl;
	fp << "gDnaOutlier";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].gDnaOutlier;
	}
	fp << endl;
	fp << "slope";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].linReg.slope();
	}
	fp << endl;
	fp << "Eff";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].linReg.e();
	}
	fp << endl;
	fp << "R2";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].linReg.r2();
	}
	fp << endl;
	fp << "nA*";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nASTAR;
	}
	fp << endl;
	fp << "nA";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nA;
	}
	fp << endl;
	fp << "nB";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nB;
	}
	fp << endl;
	fp << "nC";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nC;
	}
	fp << endl;
	fp << "nF";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nF;
	}
	fp << endl;
/*
	fp << "nHIGHDNA";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nHIGHDNA;
	}
	fp << endl;
*/
	fp << "nND";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nND;
	}
	fp << endl;
}
void	VPStore::showGoiSummarySummary( QTextStream& fp )
{
	QStringList	aplus3, aplus2, aplus, highsd;
	QStringList	low, med, high;

	foreach( QString goi, _goiSummary.keys() ) {
		switch( _goiSummary[ goi ]._flag ) {
			case	VP::APLUS3:
				aplus3 << goi;
				break;
			case	VP::APLUS2:
				aplus2 << goi;
				break;
			case	VP::APLUS:
				aplus << goi;
				break;
/*
			case	VP::LOWCONF:
				highsd << goi;
				break;
*/
			default:
				break;
		}
		switch( _goiSummary[ goi ].confidence ) {
			case	VP::High:
				high << goi;
				break;
/*
			case	VP::Med:
				med << goi;
				break;
*/
			case	VP::Low:
				//if( _goiSummary[ goi ]._flag != VP::LOWCONF ) {
					low << goi;
			//	}
				break;
			default:
				break;
		}
	}
	fp << "GOI-Total\t" << _goiSummary.keys().size() << endl;
	fp << "A+++\t" << aplus3.size() << "\t" << aplus3.join( "," ) << endl;
	fp << "A++\t" << aplus2.size() << "\t" << aplus2.join( "," ) << endl;
	fp << "A+\t" << aplus.size() << "\t" << aplus.join( "," ) << endl;
	fp << "HighSD\t" << highsd.size() << "\t" << highsd.join( "," ) << endl;
	fp << "HighConfidence\t" << high.size() << "\t" << high.join( "," ) << endl;
	//fp << "MedConfidence\t" << med.size() << "\t" << med.join( "," ) << endl;
	fp << "LowConfidence\t" << low.size() << "\t" << low.join( "," ) << endl;
}
	CalcReport::CalcReport()
{
	goi = sample = mesg = "";
	cqDnaCount = 0;
	//cqDnaCount = cqDnaCount2 = cqRnaCount = 0;
	//cqInput = cqDna = pctDna = cqDna2 = cqRna = cqRnaSd = 0.0;
	cqInput = cqDna = pctDna = cqRna = 0.0;
}
void	CalcReport::ShowHeader( QTextStream& fp )
{
	QStringList hdr = QString(
		"GOI,Sample,Note"
		",CqInput"
		",CqDNA Count,CqDNA,PctDNA"
		",CqRNA"
	).split( "," );
	fp << hdr.join( "\t" ) << endl;
}
void	CalcReport::show( QTextStream& fp )
{
	fp << goi
	 << "\t" << sample
	 << "\t" << mesg
	 << "\t" << cqInput
	 << "\t" << cqDnaCount
	 << "\t" << cqDna
	 << "\t" << pctDna
	 << "\t" << cqRna
	<< endl;
}

	VPCell::VPCell()
{
	_flag = VP::NADA;
	_inputFlagged = false;
	_input = QVariant();
	_cqNA = _cqDNA = _cqRNA = _pctDNA = VP::FLAG;
	_grade = "";
}
VP::Flag	VPCell::flag() const
{
	return( _flag );
}
QString	VPCell::flagString() const
{
	return( VP::FlagString( _flag ) );
}
QVariant	VPCell::data( const VP::DataRole& role ) const
{
	switch( role ) {
		case	VP::Input:
		case	VP::InOut:
			return( input() );
			break;
		case	VP::Working:
			switch( _flag ) {
				case	VP::NADA:
					return( input() );
					break;
				default:
					return( V( flagString() ) );
					break;
			}
			break;
		case	VP::CqRna:
			switch( _flag ) {
				case	VP::EXPFAIL:
				case	VP::NOAMP:
				case	VP::OVERLOD:
				case	VP::HIGHSD:
				case	VP::ND:
					return( V( flagString() ) );
					break;
				case	VP::AA3:
				case	VP::AA2:
				case	VP::AA:
				case	VP::ASTAR:
					return( cqNA() );
					break;
				default:
					if( _grade == VP::FLAG_F ) {
						return( V( VP::FLAG_HIGHDNA ) );
					} else {
if( cqRNA() == VP::FLAG ) {
	qDebug() << "999 has " << flagString();
}
						return( cqRNA() );
					}
/*
 had to remove this for the new correctA option
					if( _grade == VP::FLAG_A ) {
						return( cqNA() );
						break;
					} else if( _grade == VP::FLAG_F ) {
						return( V( VP::FLAG_HIGHDNA ) );
					} else {
						return( cqRNA() );
					}
*/
					break;
			}
			break;
		case	VP::CqDna:
			switch( _flag ) {
				case	VP::AA3:
				case	VP::AA2:
				case	VP::AA:
				case	VP::ASTAR:
					return( V( "NA" ) );
					break;
				case	VP::EXPFAIL:
				case	VP::NOAMP:
				case	VP::OVERLOD:
				case	VP::ND:
					return( V( flagString() ) );
					break;
				default:
					return( VP::VD( cqDNA() ) );
					break;
			}
			break;
		case	VP::PctDna:
			switch( _flag ) {
				case	VP::AA3:
				case	VP::AA2:
				case	VP::AA:
				case	VP::ASTAR:
					return( V( "NA" ) );
					break;
				case	VP::EXPFAIL:
				case	VP::NOAMP:
				case	VP::OVERLOD:
				case	VP::ND:
					return( V( flagString() ) );
					break;
				default:
					return( VP::VD( pctDNA() ) );
					break;
			}
			break;
		case	VP::Score:
			switch( _flag ) {
				case	VP::EXPFAIL:
				case	VP::NOAMP:
				case	VP::OVERLOD:
				case	VP::ND:
					return( V( flagString() ) );
					break;
				default:
					return( V( _grade ) );
					break;
			}
			break;
		// WHAT ABOUT CASE VP::CalcNa:

		default:
			break;
	}
/*
	// have to override the flagging here to get the actual input
	if( role == VP::Input  || role == VP::InOut) {
		return( input() );
	}
	switch( _flag ) {
	// TODO add this case:
	//	case	VP::FLAG_AA3:
	//	case	VP::FLAG_AA2:
	//	case	VP::FLAG_AA:
	//	role is CqRNA want CqNA
	//	        CqDNA want flagstring
	//		%DNA want flagstring
	//		break;
		case	VP::NADA:
			switch( role ) {
				case	VP::Input:
				case	VP::Working:
				case	VP::InOut:
					return( input() );
					break;
				case	VP::CqRna:
					return( cqRNA() );
					break;
				case	VP::CqDna:
					return( cqDNA() );
					break;
				case	VP::PctDna:
					return( pctDNA() );
					break;
				case	VP::CalcNa:
					return( cqNA() );
					break;
				case	VP::Score:
					return( QVariant() );
					break;
			}
			break;
		case	VP::HIGHSD:
			switch( role ) {
				case	VP::Input:
				case	VP::Working:
				case	VP::InOut:
					return( input() );
					break;
				case	VP::CqDna:
					return( cqDNA() );
					break;
				case	VP::PctDna:
					return( pctDNA() );
					break;
				case	VP::CalcNa:
					return( cqNA() );
					break;
				case	VP::Score:
					return( QVariant() );
					break;
				case	VP::CqRna:
					return( V( flagString() ) );
					break;
			}
			break;
		case	VP::HIGHDNA:
			switch( role ) {
				case	VP::Input:
				case	VP::Working:
				case	VP::InOut:
					return( input() );
					break;
					// HIGHDNA FLAG STILL SHOWS PCTDNA
				case	VP::PctDna:
					return( pctDNA() );
					break;
				case	VP::CqRna:
				case	VP::CqDna:
				case	VP::CalcNa:
					return( V( flagString() ) );
					break;
				case	VP::Score:
					return( QVariant() );
					break;
			}
			break;
		default:
			return( V( flagString() ) );
			break;
	}
*/
	return( QVariant() );
}
QVariant	VPCell::input() const
{
	return( _input );
}
void	VPCell::setInput( const QVariant& value )
{
	_input = value;
/*
	if( _input == VP::FLAG ) {
qDebug() << "HERE WE ALSO CATCH IT";
		setInputFlagged( true );
		setFlag( VP::EXPFAIL );
	}
*/
}
bool	VPCell::inputFlagged() const
{
	return( _inputFlagged );
}
void	VPCell::setInputFlagged( const bool& tf )
{
	_inputFlagged = tf;
}
double	VPCell::cqNA() const
{
	return( _cqNA );
}
double	VPCell::cqDNA() const
{
	return( _cqDNA );
}
double	VPCell::pctDNA() const
{
	double rv = _pctDNA;

	if( rv == VP::FLAG ) {	// 999 or default, never set state
		rv = 0;
	} else if( rv > 100 ) {
		rv = 100;
	}
	return( rv );
}
QString	VPCell::grade() const
{
	return( _grade );
}
double	VPCell::cqRNA() const
{
/*
	if( _cqRNA == 999 ) {
		qDebug() << "Caught it";
	}
*/
	return( _cqRNA );
}
void	VPCell::setFlag( const VP::Flag& flag )
{
	_flag = flag;
}
void	VPCell::confirmCqNA()
{
	setCqNA( CqNa( _cqRNA, _cqDNA ) );
}
void	VPCell::setCqNA( const double& value )
{
	_cqNA = value;
}
void	VPCell::setCqDNA( const double& value )
{
	_cqDNA = value;
}
void	VPCell::setPctDNA( const double& value )
{
	_pctDNA = value;
}
void	VPCell::setGrade( const QString& value )
{
	_grade = value;
}
void	VPCell::setCqRNA( const double& value )
{
	_cqRNA = value;
}

	VPStore::VPStore()
{
	_data = 0;
	_cmap = new ColorMap();
}
	VPStore::~VPStore()
{
	deleteData();
}
	// TODO - but likely finished, one more check cannot hurt
	// NEED TO SEE IF WE ALREADY HAVE DATA AND WHAT TO DO
	// TO GET RID OF IT

bool	VPStore::preload( CliApp* app )
{
	Q_UNUSED( app );

	_inputFile = APP_S( "load/file" );
	_inputFormat = APP_S( "load/format" );
	//_inputFile = S( app->param( "load/file" )->value() );
	//_inputFormat = S( app->param( "load/format" )->value() );
	return( true );
}
	//	precheck is also called by gDnaReady()
	// 	so need to make sure it is harmless for all
	//	things not related to gDNA concentration series
	//
bool	VPStore::precheck( CliApp* app )
{
	_parseConc = B( app->param( "check/parseconc" )->value() );

	_validateAssay = B( app->param( "check/validate" )->value() );

	_LOD = D( app->param( "check/lod" )->value() );
	_failFlag = S( app->param( "check/failflag" )->value() );
	_noampFlag = S( app->param( "check/noampflag" )->value() );

	_vpaCol = S( app->param( "check/vpacol" )->value() );
	_gDnaRows = SL( app->param( "check/gdnarows" )->value() );

	_ignoreCols = SL( app->param( "check/ignorecols" )->value() );
	_ignoreRows = SL( app->param( "check/ignorerows" )->value() );

	_minSdCount = I( app->param( "check/minsdcount" )->value() );
	_highAllSdCutoff = D( app->param( "check/highallsdcutoff" )->value() );

	_performLoo = B( app->param( "check/performloo" )->value() );
	_highLooSdCutoff = D( app->param( "check/highloosdcutoff" )->value() );

	_gDnaConcValues = app->param( "hidden/gdnaconc" )->value().toList();
	/* gDnaRows is complicated...
		filled here from the app and then reordered below
		there is also a companion set of values which are
		the actual concentrations
			- either snarfed from the labels
			- or entered via the GUI
		so when reording takes place, those values also need
		to be reordered and accessible for getLinReg
	 */
	// here we need to save gDnaRows for return value
	//	from gDnaConcLabels
	// and will also fill in QList<double> gDnaConcentrations
	// and QList<QVariant> gDnaConcValues
	// these are used vi myApp to make VariantListDialog
	// with the ActionParam::gDnaConc
	//
	//	In order to sort from low-high concentration
	//	need a temporary storage type
	//
	//	Dont need to worry about _gDnaRowIndexes here
	//	as that will be handled next in check();
	if( _gDnaRows.at( 0 ).contains( "," ) ) {
		_gDnaRows = _gDnaRows.at( 0 ).split( "," );
	}

	if( _parseConc ) {
		DilSeries	dilSeries;
		for( int i = 0; i < _gDnaRows.size(); ++i ) {
			dilSeries << DilutionPoint( _gDnaRows.at( i ) );
		}
		qSort( dilSeries.begin(), dilSeries.end(), DilSeriesSortV );
		_gDnaConcValues.clear();
		for( int i = 0; i < dilSeries.size(); ++i ) {
			_gDnaRows[ i ] = dilSeries.at( i ).label();
			_gDnaConcValues << V( dilSeries.at( i ).value() );
		}
		app->setParamValue( "check/gdnarows", _gDnaRows );
	} else if( _gDnaConcValues.size() != _gDnaRows.size() ) {
		_gDnaConcValues.clear();
/*
		for( int i = 0; i < _gDnaRows.size(); ++i ) {
			dilSeries <<
			 DilutionPoint( _gDnaRows.at( i ), D( clist.at( i ) ) );
			_gDnaConcValues << clist.at( i );
		}
	} else {
		for( int i = 0; i < _gDnaRows.size(); ++i ) {
			dilSeries << DilutionPoint( _gDnaRows.at( i ) );
		}
*/
	}
	app->setParamValue( "hidden/gdnaconc", _gDnaConcValues );
	fillConcMap( app );
	return( true );
}
bool	VPStore::fillConcMap( CliApp *app )
{
	_gDnaConcMap.clear();
	// get back to the app for the latest variables
	_gDnaRows = SL( app->param( "check/gdnarows" )->value() );
	if( _gDnaRows.at( 0 ).contains( "," ) ) {
		_gDnaRows = _gDnaRows.at( 0 ).split( "," );
	}
	_gDnaConcValues = app->param( "hidden/gdnaconc" )->value().toList();
	if( _gDnaRows.size() != _gDnaConcValues.size() ) {
		return( false );
	}
	for( int i = 0; i < _gDnaRows.size(); ++i ) {
		_gDnaConcMap.insert( _gDnaRows.at( i ),
		 log10( _gDnaConcValues.at( i ).toDouble() ) );
	}
	return( true );
}
QStringList	VPStore::gDnaConcLabels() const
{
	return( _gDnaRows );
}
QList<QVariant>	VPStore::gDnaConcValues() const
{
	return( _gDnaConcValues );
}
QString	VPStore::checkMessage() const
{
	return( _checkMessage );
}
	// this is currently displayed in statusBar
QString	VPStore::getCalcInfo( const QString& row, const QString& column,
	 const VP::State& state ) const
{
	QString	rv;
	QString	flag, inputString;
	double	inputValue;
	bool	inputOk;

	int	fw = 6;
	char	f = 'f';
	int	pr = 2;

	int	ridx = rowIndex( row );
	int	cidx = colIndex( column );

	if( ridx == UINT || cidx == UINT ) {
		return( QString() );
	}
	inputValue = _data[ ridx ][ cidx ].input().toDouble( &inputOk );
	if( !inputOk ) {
		inputString = " Input=(NA)";
	} else {
		inputString = QString( " Input=(%1)" ).arg( inputValue, fw, f, pr );
	}
	flag = _data[ ridx ][ cidx ].flagString();

	rv = QString( "%1 :: " ).arg( StateString( state ) );
	switch( state ) {
		case	VP::Load:
			if( _data[ ridx ][ cidx ].flag() != VP::NADA ) {
				rv.append( _data[ridx][cidx].flagString() );
			}
			break;
		case	VP::Check:
		case	VP::PostCheck:
			rv.append( inputString );
			if( _data[ ridx ][ cidx ].flag() != VP::NADA ) {
				rv.append( _data[ridx][cidx].flagString() );
			}
			break;
		case	VP::Run:
		case	VP::Save:
		case	VP::Done:
			rv.append( inputString );
			if( _data[ ridx ][ cidx ].flag() != VP::NADA ) {
				rv.append( _data[ridx][cidx].flagString() );
			}
			rv.append( QString( " CqRNA=(%1)" )
			 .arg( _data[ridx][cidx].cqRNA(), fw, f, pr ) );
			rv.append( QString( " CqDNA=(%1)" )
			 .arg( _data[ridx][cidx].cqDNA(), fw, f, pr ) );
			rv.append( QString( " %DNA=(%1)" )
			 .arg( _data[ridx][cidx].pctDNA(), fw, f, pr ) );
			rv.append( QString( " CqNA=(%1)" )
			 .arg( _data[ridx][cidx].cqNA(), fw, f, pr ) );
			break;
	}
	rv.append( QString( " Col: %1, Row: %2" )
	 .arg( column ).arg( row ) );

	return( rv );
}
bool	VPStore::check()
{
	_checkMessage.clear();
	if( _vpaCol.isEmpty() || !_inputCols.contains( _vpaCol ) ) {
		setCritical( "VPA not found in input columns" );
		return( false );
	}
	_vpaColIndex = colIndex( _vpaCol );
	if( _gDnaRows.size() == 0 ) {
		setCritical( "gDNA(s) not specified for input rows" );
		return( false );
	}
	//	NOTE: gDnaRows is sorted (by conc) in precheck()
	//
	_gDnaRowIndexes.clear();
	foreach( QString s, _gDnaRows ) {
		if( rowIndex( s ) == UINT ) {
			setCritical( QString( "%1 not found in input rows" )
			 .arg( s ) );
			return( false );
		}
		_gDnaRowIndexes << rowIndex( s );
	}
	if( _gDnaRowIndexes.size() == 0 ) {
		setCritical( "gDNA(s) not found for input rows" );
		return( false );
	}
	if( _validateAssay && _gDnaRowIndexes.size() < 3 ) {
		setCritical( "Unable to validate assays with < 3 gDna samples" );
		return( false );
	}
	_outputCols = _inputCols;
	_outputRows = _inputRows;
	foreach( QString s, _ignoreCols ) {
		_outputCols.removeOne( s );
	}
	// get the vpa col out of the analysis
	//
	_outputCols.removeOne( _vpaCol );

	foreach( QString s, _ignoreRows ) {
		_outputRows.removeOne( s );
	}
	// get the gDNA rows out of the analysis
	//
	foreach( QString s, _gDnaRows ) {
		_outputRows.removeOne( s );
	}

	// First run check_0 which will flag all cells OVERLOD
	//
	//	 this will only fail is %overlod > 90
	//	 warn if > 50%
	//
	if( !check_0() ) {
		return( false );
	}
	// Now run check_1 gDna-VPA
	//
	if( !check_1() ) {
		return( false );
	}
	// Then run check_2 Samples(...)-VPA
	//
	if( !check_2() ) {
		return( false );
	}
	// then run check_3 gDna-GOIs(...) //
	if( !check_3() ) {
		return( false );
	}
	// myApp is repsonsible for preparing cli and gui dumps of text
	// to explain to the user a summary of what check found
	// and what will happen when run is pressed
	//
	// if the checks return critical somewhere, then run state
	// should not become active
	//
	return( true );
}
LinReg	VPStore::getLinReg( const QList<int>& rowIndexes,
	 //const QList<double>& concValues,
	 const int& cidx ) const
{
	LinReg		rv;
	QList<double>	x, y;

/*	if( rowIndexes.size() != concValues.size() ) {
		return( rv );
	}
*/
	for( int i = 0; i < rowIndexes.size(); ++i ) {
		int ridx = rowIndexes.at( i );
		if( _data[ ridx ][ cidx ].inputFlagged() ) {
			continue;
		}
		x << _gDnaConcMap[ rowString( ridx ) ];
		//x << log10( concValues.at( i ) );
		y << _data[ ridx ][ cidx ].input().toDouble();
	}

/*


	for( int i = 0; i < rows.size(); ++i ) {
		//int	ridx = _gDnaRowIndexes.at( i );
		int	ridx = rows.at( i );
		if( _data[ ridx ][ cidx ].inputFlagged() ) {
			continue;
		}
	// this was a bug, we were accessing something that may not
	// directly correspond to the input.
	// get the string for the input row and use that to access
	// the index position of the input to access conc array
	//
		QString s = rowString( ridx );
		int idx = _gDnaRows.indexOf( s );

		x << log10( _gDnaConcentrations.at( idx ) );
		y << _data[ ridx ][ cidx ].input().toDouble();
	}
*/
	rv.setIndexes( rowIndexes );
	rv.compute( x, y );
	return( rv );
}
bool	VPStore::check_0()
{
	double	cellsError, cellsTotal, cellsValid, cellsOverlod;
	double	pctOverlod = 0.0;

	_checkMessage.clear();
	cellsError = cellsTotal = cellsValid = cellsOverlod = 0;
	//
	// scan all data points:
	//	if inputFlagged then do not process
	//	otherwise flag using user defined EXFAIL and NOAMP flags
	//	and mark OVERLOD where over user defined LOD
	//
	for( int i = 0; i < _inputRows.size(); ++i ) {
		for( int j = 0; j < _inputCols.size(); ++j ) {
			if( _data[ i ][ j ].inputFlagged() ) {
				continue;
			}
			QString s = S( _data[ i ][ j ].input() );
			++cellsTotal;
			if( !_failFlag.isEmpty() && s == _failFlag ) {
				_data[ i ][ j ].setFlag( VP::EXPFAIL );
			} else if( !_noampFlag.isEmpty() && s == _noampFlag ) {
				_data[ i ][ j ].setFlag( VP::NOAMP );
			} else {
				bool ok = false;
				double d = s.toDouble( &ok );
				if( !ok ) {
					_data[ i ][ j ].setFlag( VP::ERROR );
					++cellsError;
					continue;
				}
				if( d >= _LOD ) {
					_data[ i ][ j ].setFlag( VP::OVERLOD );
					++cellsOverlod;
				} else {
					_data[ i ][ j ].setFlag( VP::NADA );
					_data[ i ][ j ].setCqNA( d );
				}
				++cellsValid;
			}
		}
	}
	_checkMessage.append( 
		QString( "%1 cells input, %2 cells above Cq cutoff and %3 cells caused error\n" )
		.arg( cellsTotal ).arg( cellsOverlod ).arg( cellsError )
	);
	if( cellsError > 0 ) {
		setCritical( QString( VP::CRIT_MESG_ERRORCELLS ).arg( cellsError ) );
		return( false );
	}
	pctOverlod = cellsOverlod / cellsValid;
	_checkMessage.append(
		QString( "%1% of valid cells are over-LOD (%2)\n" ).arg( 100 * pctOverlod ).arg( _LOD )
	);

	if( pctOverlod > VP::CHECK0_PCTFAIL_CRIT ) {
		setCritical( QString( VP::CHECK0_CRIT_MESG ).arg( pctOverlod ) );
		return( false );
	} else if( pctOverlod > VP::CHECK0_PCTFAIL_WARN ) {
		setWarning( QString( VP::CHECK0_WARN_MESG ).arg( pctOverlod ) );
	}
	return( true );
}
	//
	//	CHECK_1
	//
	// Fix column on VPA and iterate over all gDna
	//
	/*
		gDnaVpa-tab size == 0
			POPUP CRITICAL SAYING WE CANNOT CONTINUE CHECK YOUR LOD
		gDnaVpa-tab  size != gDna-series size and gDna-series size > 1
			POPUP WARNING X gDNAS removed, CAN PROCEED WITH single
		and gDna-series size == 1 -> ok passed this check
	 */
bool	VPStore::check_1()
{
	_gDnaVpaRowIndexes.clear();
	foreach( int ridx, _gDnaRowIndexes ) {
		if( _data[ ridx ][ _vpaColIndex ].flag() == VP::NADA ) {
			_gDnaVpaRowIndexes << ridx;
		}
	}

	if( _gDnaVpaRowIndexes.size() == 0 ) {
		setCritical( VP::CHECK1_CRIT_MESG );
		return( false );
	}
	if( _gDnaRowIndexes.size() > 1 &&
	 _gDnaVpaRowIndexes.size() != _gDnaRowIndexes.size() ) {
		setWarning( QString( VP::CHECK1_WARN_MESG )
		  .arg( _gDnaRowIndexes.size() - _gDnaVpaRowIndexes.size() )
		  .arg( _gDnaVpaRowIndexes.size() ) );
		// but we continue to the run state
	}
	gDnaLR = getLinReg( _gDnaRowIndexes, _vpaColIndex );
	gDnaVpaLR = getLinReg( _gDnaVpaRowIndexes, _vpaColIndex );

	_checkMessage.append(
	 QString( "%1 gDna rows input, %2 gDna rows unflagged for VPA\n" )
	  .arg( _gDnaRowIndexes.size() ).arg( _gDnaVpaRowIndexes.size() )
	);
	if( _gDnaRowIndexes.size() >= 3 ) {
		_checkMessage.append(
	 	 QString( "LinReg all-gDna slope %1, Eff %2, R2 %3\n" )
	  	  .arg( gDnaLR.slope() ).arg( gDnaLR.e() ).arg( gDnaLR.r2() )
		);
	}
	if( _gDnaVpaRowIndexes.size() >= 3 &&
	  _gDnaVpaRowIndexes.size() != _gDnaRowIndexes.size() ) {
		_checkMessage.append(
		 QString( "LinReg vpa-gDna slope %1, Eff %2, R2 %3\n" )
		  .arg( gDnaVpaLR.slope() ).arg( gDnaVpaLR.e() )
		  .arg(  gDnaVpaLR.r2() )
		);
	}
	return( true );
}
	//
	//	CHECK_2
	//
	// Fix column on VPA and iterate over all Samples
	//
	/*
		If VpaSample is OVERLOD or NOAMP then either
			VPA is shit
			That sample does not amplify gDNA...(should check against result from CHECK_1
				for example if the VPA on gDNA worked, then there is really not any gDNA in the sample
				 and if the VPA also failed on the gDNA then it is likely the VPA is shit
			=> OUTPUT: A* astar
				CqRNA = CqNA
				CqDNA = 0
				pct = 0
	
		If VpaSample is EXPFAIL then we cannot do anything for that Row...
			=> OUTPUT:
				CqRNA/DNA/pct = UNDEF
		Otherwise we have a Ct for VpaSample and if CHECK_1 and CHECK_2 pass,
		 proceed to CHECK_3 for each GOI
	 */
bool	VPStore::check_2()
{
	int	nFiction = 0;

	_validSamples.clear();
	_astarSamples.clear();
	_failSamples.clear();
	for( int r = 0; r < _outputRows.size(); ++r ) {
		QString rlabel = _outputRows.at( r );
		int	ridx = rowIndex( rlabel );

		VP::Flag	flag = _data[ ridx ][ _vpaColIndex ].flag();

		if( flag == VP::EXPFAIL ) {
			_failSamples << rlabel;
		} else if( flag == VP::NOAMP ) {
			_astarSamples << rlabel;
		} else {
			if( flag == VP::OVERLOD ) {
				// TODO, if we do this, but do not unset the flag
				// will the data actually ever get used? No...
				// set cqNA to LOD + 1.0
				//
				_data[ ridx ][ _vpaColIndex ].setCqNA( _LOD + 1 );
				++nFiction;
			}
			_validSamples << rlabel;
		}
	}
	double	pctFail = _outputRows.size() - _validSamples.size();
	pctFail /= _outputRows.size();

	_checkMessage.append(
	QString( "Set %1 cells to Cq-cutoff + 1\n" ).arg( nFiction )
	);
	_checkMessage.append(
	QString( "%1% of rows failed check_2 (%2 / %3 valid)\n" )
	.arg( pctFail * 100 ).arg( _validSamples.size() ).arg( _outputRows.size() )
	);
	// 90% for pctFail crit in vp.h file
	// 50% for pctFail warn in vp.h file
	//
	if( pctFail > VP::CHECK2_PCTFAIL_CRIT ) {
		setCritical( QString( "%1%% of samples were flagged in VPA column" ).arg( pctFail ) );
		return( false );
	} else if( pctFail > VP::CHECK2_PCTFAIL_WARN ) {
		setWarning( QString( "%1%% of Samples are not being using for calculations" )
		 .arg( pctFail ) );
	}
	return( true );
}
	//
	//	CHECK_3
	//
	//	Iterate over GOIs and pre-check each (set) gDNAs
	//
bool	VPStore::check_3()
{
	//	double	cqDna = sampleVpa + ( gDnaGoi - gDnaVpa );
	//
	// here we iterate over GOI and check the flag state of
	//	each deltaTerm ( gDnaGoi - gDnaVpa )
	//	when no flagging exists on the deltaTerm components
	//	we need to get the SD over the series to find outliers
	//	
	//	Store this is a QMap<QString,QList<int>>
	//	 which is GOI -> list of indexed in gDNA that are acceptable
	//
	//	WATCH OUT FOR A+, this is where we will find it
	// and will need a MAP GOI -> indexes as flagging is GIO-dependent now

	int	nHigh = 0;
	//int	nMed = 0;
	int	nLow = 0;

	double	APP_WEIGHT_NADA = -1;
	double	APP_WEIGHT_EXPFAIL = 0;
	double	APP_WEIGHT_OVERLOD = 1;
	double	APP_WEIGHT_NOAMP = 2;

	double	APP_CUTOFF_APLUS3 = 8;
	double	APP_CUTOFF_APLUS2 = 5;
	double	APP_CUTOFF_APLUS  = 2;

	// if _validateAssay == false then a lot of this does not get done

	_goiSummary.clear();
	foreach( QString goi, _outputCols ) {
		GoiSummary	summary;
		summary.goi = goi;
		summary._flag = VP::CALC;
		summary.gDnaIndexes.clear();

		int	cidx = colIndex( goi );
		QList<int>	validIndexes;

		double	score = 0;
		for( int i = 0; i < _gDnaRowIndexes.size(); ++i ) {
			int ridx = _gDnaRowIndexes.at( i );
			switch( _data[ ridx ][ cidx ].flag() ) {
				case	VP::EXPFAIL:
// CHANGED ON July 28 2011
// Henrik wanted it
					if( _data[ ridx ][ cidx ].cqNA() > _LOD ) {
						score += APP_WEIGHT_OVERLOD;
					} else {
						score += APP_WEIGHT_EXPFAIL;
					}
					break;
				case	VP::OVERLOD:
					score += APP_WEIGHT_OVERLOD;
					break;
				case	VP::NOAMP:
					score += APP_WEIGHT_NOAMP;
					break;
				default:
					score += APP_WEIGHT_NADA;
					break;
			}
		}
		if( score >= APP_CUTOFF_APLUS3 ) {
			summary._flag = VP::APLUS3;
		} else if( score >= APP_CUTOFF_APLUS2 ) {
			summary._flag = VP::APLUS2;
		} else if( score >= APP_CUTOFF_APLUS ) {
			summary._flag = VP::APLUS;
		}
		//
		//	A+ means ALL gDna points for a GOI are NOAMP
		//
		//	Cq -1
		//	EXPFAIL 0
		//	OVERLOD	1
		//	NOAMP	2
		//
		//	sum these weights, test /n or /sqrt(n) but arrive at score
		//	A+++ >= 8
		//	A++ >= 5	
		//	A= >= 2
		/*
		int	nNoamp = 0;
		int	nTotal = _gDnaRowIndexes.size();
		for( int i = 0; i < nTotal; ++i ) {
			int ridx = _gDnaRowIndexes.at( i );
			if( _data[ ridx ][ cidx ].flag() == VP::NOAMP ) {
				++nNoamp;
			}
		}
		double	pctNoamp = nNoamp;
		pctNoamp /= nTotal;

		if( pctNoamp >= 0.9 ) {		// > 90% NOAMP
			if( nTotal >= 4 ) {
				summary._flag = VP::APLUS3;
			} else if( nTotal == 3 ) {
				summary._flag = VP::APLUS2;
			} else if( nTotal >= 1 ) {
				summary._flag = VP::APLUS;
			}
		} else if( pctNoamp >= 0.6 && nTotal == 3 ) { 
			summary._flag = VP::APLUS;
		} else if( pctNoamp >= 0.5 && nTotal == 4 ) { 
			summary._flag = VP::APLUS2;
		} else if( pctNoamp >= 0.4 && nTotal == 4 ) { 
			summary._flag = VP::APLUS;
		}
		*/
		//
		//	Collect gDna indexes for this GOI
		//	if validIndexes ends empty, will see later...
		//
		if( summary._flag == VP::CALC ) {
			for( int i = 0; i < _gDnaVpaRowIndexes.size(); ++i ) {
				int ridx = _gDnaVpaRowIndexes.at( i );
				if( _data[ ridx ][ cidx ].flag() == VP::NADA ) {
					summary.gDnaIndexes << ridx;
				}
			}
		}
		_goiSummary.insert( goi, summary );
	}
	if( _validateAssay == false ) {
		return( true );
	}
	//	IMPORTANT NOTE: _gDnaForGoi now actually has row indexes
	//
	// Now get SD for deltaNA (VPA versus GOI)
	//
	foreach( QString goi, _goiSummary.keys() ) {
		//
		// SKIP THE A+, A++, and A+++ cases
		//
		// default confidence is NotUsed (A+++/A++/A+)
		//
		switch( _goiSummary[ goi ]._flag ) {
			case	VP::APLUS3:
			case	VP::APLUS2:
			case	VP::APLUS:
				continue;
				break;
			default:
				break;
		}
		// allSd requires a certain number of gDna Indexes
		// and looSd required 4 indexes
		//
		int	cidx = colIndex( goi );
		double	allSd = 0;	
		double	looSd = 0;
		int	minAt = UINT;
		
		if( _goiSummary[ goi ].gDnaIndexes.size() < _minSdCount ) {
			_goiSummary[ goi ].confidence = VP::Low;
			//_goiSummary[ goi ]._flag = VP::LOWCONF;
			++nLow;
			_checkMessage.append(
			 QString( "%1 has ZERO confidence %2 out of %3 gDna Indexes\n" )
			 .arg( goi ).arg( _goiSummary[ goi ].gDnaIndexes.size() )
			 .arg( _gDnaVpaRowIndexes.size() ) );
			continue;
		}
		allSd = getSd( _goiSummary[ goi ].gDnaIndexes, cidx );
		if( _performLoo &&
		 _goiSummary[ goi ].gDnaIndexes.size() >= _minSdCount + 1 ) {
			looSd = getSdLoo( _goiSummary[ goi ].gDnaIndexes, &minAt, cidx );
		}
		_goiSummary[ goi ].allSd = allSd;
		_goiSummary[ goi ].looSd = looSd;
		// for a GOI with 4 gDnaIndexes:
		//	Looking for minAt  2  is it:   "_77_gDNA 2500" 
		//	Check gDna Indexes for  "txnip"  size =  4 
		//	0   0 "_75_gDNA 10000" 
		//	1   1 "_76_gDNA 5000" 
		//	2   2 "_77_gDNA 2500" 
		//	3   3 "_78_gDNA 500" 
		//
		// no idea how this go so complex but this seems to be correct
		if( minAt != UINT ) {
			_goiSummary[ goi ].gDnaOutlier = _inputRows.at(
		  	 _goiSummary[ goi ].gDnaIndexes[ 
			   _goiSummary[ goi ].gDnaIndexes.indexOf( minAt )
			]
			);
		}

		if( allSd <= _highAllSdCutoff ||
		 ( minAt != UINT && looSd <= _highLooSdCutoff ) ) {
			_goiSummary[ goi ].confidence = VP::High;
			++nHigh;
		} else {
			_goiSummary[ goi ].confidence = VP::Low;
			//_goiSummary[ goi ]._flag = VP::LOWCONF;
			++nLow;
		}
		if( minAt != UINT && looSd < allSd ) {
			_goiSummary[ goi ].gDnaIndexes.removeOne( minAt );
		}
		_goiSummary[goi].linReg =
		 getLinReg( _goiSummary[ goi ].gDnaIndexes, cidx );
	}
/* NO MORE MED CONFIDENCE
		if( allSd <= _cutHighAll || ( minAt != UINT && looSd <= _cutHighLoo ) ) {
			_goiSummary[ goi ].confidence = VP::High;
		} else if( allSd <= _cutMedAll || ( minAt != UINT && looSd <= _cutMedLoo ) ) {
			_goiSummary[ goi ].confidence = VP::Med;
		}
*/

/*
		if( allSd <= _cutoffSdNa && looSd <= _cutoffSdNa ) {
			_goiSummary[ goi ].confidence = VP::High;
			// THINKING...FUTURE
			// cut = 0.4
			// allSd = 0.39
			// looSd1 = 0.29 looSd2 = 0.1
			//
			// formula ( cut - loo ) / ( cut - all )
			//
			//	eventually may take the loo even in High-Confidence
			//	 if the separation is very great
			//
			++nHigh;
		} else if( looSd <= _cutoffSdNa && minAt != UINT ) {
			//	remove minAt and use these indexes for GOI
			//OLD _gDnaForGoi[ goi ].removeOne( minAt );
			_goiSummary[ goi ].gDnaIndexes.removeOne( minAt );
			_goiSummary[ goi ].confidence = VP::Med;
			++nMed;
		} else {
			_goiSummary[ goi ].confidence = VP::Low;
			_goiSummary[ goi ]._flag = VP::LOWCONF;
			if( minAt != UINT && looSd < allSd ) {
				_goiSummary[ goi ].gDnaIndexes.removeOne( minAt );
			}
			++nLow;
		}
		_goiSummary[goi].linReg = getLinReg( _goiSummary[goi].gDnaIndexes, cidx );
	}
*/
	_checkMessage.append( QString(
"Out of %1 output columns, %2 will be used for calculations (%3 High, %4 Low) \n" )
	.arg( _outputCols.size() ).arg( _goiSummary.size() )
	.arg( nHigh ).arg( nLow )
	);
	return( true );
} 
double	VPStore::getSd( const QList<int>& rowIndexes, const int& colIndex ) const
{
	QList<double>	tab;

	foreach( int rowIndex, rowIndexes ) {
		tab << qAbs( _data[ rowIndex ][ colIndex ].cqNA() -
			_data[ rowIndex ][ _vpaColIndex ].cqNA() );
	}
	return( SD<double>( tab ) );
}
double	VPStore::getSdLoo( const QList<int>& rowIndexes, int *minAt, const int& colIndex )
{
	double	min = 0;

	for( int i = 0; i < rowIndexes.size(); ++i ) {
		int loo = rowIndexes.at( i );
		QList<int> subset;
		foreach( int rowIndex, rowIndexes ) {
			if( rowIndex == loo ) {
				continue;
			}
			subset << rowIndex;
		}
		double sd = getSd( subset, colIndex );
		if( i == 0 || sd < min ) {
			min = sd;
			*minAt = loo;
		}
	}
	return( min );
}
double	VPStore::getSdLoo( const QList<int>& rowIndexes, const double& minSd,
	 int *minAt, const int& colIndex )
{
	double	min = minSd;

	foreach( int loo, rowIndexes ) {
		QList<int> subset;
		foreach( int rowIndex, rowIndexes ) {
			if( rowIndex == loo ) {
				continue;
			}
			subset << rowIndex;
		}
		double sd = getSd( subset, colIndex );
		if( sd < min ) {
			min = sd;
			*minAt = loo;
		}
	}
	return( min );
}
bool	VPStore::prerun( CliApp* app )
{
	_correctA = B( app->param( "run/correcta" )->value() );

	_gradeA = D( app->param( "run/gradea" )->value() );
	if( _gradeA < 1 ) {
		_gradeA *= 100;
	}
	_gradeB = D( app->param( "run/gradeb" )->value() );
	if( _gradeB < 1 ) {
		_gradeB *= 100;
	}
	_gradeC = D( app->param( "run/gradec" )->value() );
	if( _gradeC < 1 ) {
		_gradeC *= 100;
	}
	return( true );
}
bool	VPStore::preheatmap( CliApp *app )
{
	_cmap->clear();
	_cmap->insert( VP::FLAG_AA3, CLR( app->param( "heatmap/coloraplus" )->value() ) );
	_cmap->insert( VP::FLAG_AA2, CLR( app->param( "heatmap/coloraplus" )->value() ) );
	_cmap->insert( VP::FLAG_AA, CLR( app->param( "heatmap/coloraplus" )->value() ) );
	_cmap->insert( VP::FLAG_ASTAR, CLR( app->param( "heatmap/colorastar" )->value() ) );
	//_cmap->insert( VP::FLAG_HIGHDNA, CLR( app->param( "heatmap/colorhighdna" )->value() ) );
	_cmap->insert( VP::FLAG_HIGHSD, CLR( app->param( "heatmap/colorhighsd" )->value() ) );
	_cmap->insert( VP::FLAG_NOAMP, CLR( app->param( "heatmap/colornoamp" )->value() ) );
	_cmap->insert( VP::FLAG_OVERLOD, CLR( app->param( "heatmap/coloroverlod" )->value() ) );
	_cmap->insert( VP::FLAG_EXPFAIL, CLR( app->param( "heatmap/colorexpfail" )->value() ) );
	_cmap->insert( VP::FLAG_A, CLR( app->param( "heatmap/colora" )->value() ) );
	_cmap->insert( VP::FLAG_B, CLR( app->param( "heatmap/colorb" )->value() ) );
	_cmap->insert( VP::FLAG_C, CLR( app->param( "heatmap/colorc" )->value() ) );
	_cmap->insert( VP::FLAG_F, CLR( app->param( "heatmap/colorf" )->value() ) );

	return( true );
}
ColorMap*	VPStore::colorMap() const
{
	return( _cmap );
}
bool	VPStore::calc( const QString& goi, const QString& sample )
{
	bool	rv = false;
	double	cqNa, cqDna, pctDna, cqRna;
	QList<double>	cqDnaTab;

	CalcReport	calcReport;

	calcReport.goi = goi;
	calcReport.sample = sample;
	calcReport.mesg = "CALC";

	int	ridx = rowIndex( sample );
	int	cidx = colIndex( goi );
	if( ridx == UINT || cidx == UINT ) {
		return( false );
	}
	if( !_goiSummary.contains( goi ) ) {
		// maybe this should have a different flag?
		// basically there was a fail in check_3 no gDnas for the GOI
qDebug() << "ND SET VIA NO GOI IN SUMMARY " << goi;
		_data[ ridx ][ cidx ].setFlag( VP::ND );
		calcReport.mesg = "GoiSummary missing this GOI";

		goto CALC_DONE;
	}
	switch( _data[ ridx ][ cidx ].flag() ) {
		case	VP::EXPFAIL:
		case	VP::NOAMP:
		case	VP::OVERLOD:
// TODO, BUG was that OVERLOD was never reported (over-written with ND)
// but I need to check that commenting this out does not hurt other things?
//
			//_data[ ridx ][ cidx ].setFlag( VP::ND );
			calcReport.mesg = "Cell Flagged " +
				 _data[ridx][cidx].flagString();
			goto CALC_DONE;
			break;
		default:
			break;
	}


	cqNa = _data[ ridx ][ cidx ].cqNA();
	calcReport.cqInput = cqNa;

	foreach( int gDnaRowIndex, _goiSummary[ goi ].gDnaIndexes ) {
		double	gDnaGoi = _data[ gDnaRowIndex ][ cidx ].cqNA();
		double	sampleVpa = _data[ ridx ][ _vpaColIndex ].cqNA();
		double	gDnaVpa = _data[ gDnaRowIndex ][ _vpaColIndex ].cqNA();

		double	cqDna = sampleVpa + ( gDnaGoi - gDnaVpa );
		cqDnaTab << cqDna;
		//
		//	4th July 2011 :
		//	 Henrik chooses the method:
		//	   PctDna( cqNa, Mean<double>( cqDnaTab ) 
		//
		//	previous:
		//	foreach() {
		//		double	pctDna = PctDna( cqNa, cqDna );
		//		pctTab << pctDna;
		//	}
		//	pctCalc = Mean<double>( pctTab );
	}
	rv = true;
	cqDna = Mean<double>( cqDnaTab );
	pctDna = PctDna( cqNa, cqDna ) * 100;

	//pctCalc = PctDna( cqNa, Mean<double>( cqDnaTab ) ) * 100;
	//	PERCENT NOW STORED BY DEFAULT
	//	GRADE IS SET SIMULTANEOUSLY
	//
	_data[ridx][cidx].setPctDNA( pctDna );
	_data[ridx][cidx].setCqDNA( cqDna );
	_data[ridx][cidx].setGrade( vpScore( ridx, cidx ) );

	calcReport.cqDnaCount = cqDnaTab.size();
	calcReport.cqDna = cqDna;
	calcReport.pctDna = pctDna;

	// First test has to be for grade A
	//
	if( _correctA  == false && 
	 _data[ ridx ][ cidx ].grade() == VP::FLAG_A ) {
		calcReport.mesg = "NOCALC";
if( cqNa == 999 ) {
	qDebug() << "HERE WE HAVE A 999 going into CqRNA";
}
		_data[ ridx ][ cidx ].setCqRNA( cqNa );
		// GRADE A WILL OPTIONALLY OUTPUT CqNA
		goto CALC_DONE;
	}
	// Second test has to be for grade F
	//
	if( _data[ ridx ][ cidx ].grade() == VP::FLAG_F ) {
		calcReport.mesg = "NOCALC";
		// GRADE F will output HIGHDNA
		goto CALC_DONE;
	}
	//if( _goiSummary[ goi ]._flag == VP::LOWCONF ) {
	if( _goiSummary[ goi ].confidence == VP::Low ) {
		calcReport.mesg = "NOCALC";
		// WE ARE ONLY HERE FOR GRADE B AND C
		_data[ ridx ][ cidx ].setFlag( VP::HIGHSD );
		goto CALC_DONE;
	}
	// ONLY GET HERE IF GRADE B or C and HIGHCONF
	//
	/* other method
	foreach( double cqDna, cqDnaTab ) {
		cqRna = Kubi( cqNa, cqDna );
		if( cqRna > 0 ) {
			cqRnaTab << cqRna;
			useable_cqDnaTab << cqDna;
		}
	}*/
	cqRna = Kubi( cqNa, cqDna );
	if( cqRna < 0 ) {
		qDebug() << "K0 IS STILL POSSIBLE!";
	}

	calcReport.mesg = "CALC";
	calcReport.cqRna = cqRna;
if( cqRna == 999 ) {
	qDebug() << "HERE WE HAVE A 999 going into CqRNA";
}
	_data[ ridx ][ cidx ].setCqRNA( cqRna );
/* TODO REMOVE THESE THINGS FROM REPORT
	calcReport.cqRnaCount = cqRnaTab.size();
	calcReport.cqRnaSd = SD<double>( cqRnaTab );
	calcReport.cqDnaCount2 = useable_cqDnaTab.size();
	calcReport.cqDna2 = Mean<double>( useable_cqDnaTab );
*/
/*
	if( cqRnaTab.size() == 0 ) {
		//
		// TODO make this a setError condition
		// to make sure if it happens during testing
		_data[ ridx ][ cidx ].setFlag( VP::K0 );
		calcReport.mesg = "K0";
		goto CALC_DONE;
	} else if( cqRnaTab.size() == 1 ) {
		_data[ ridx ][ cidx ].setCqDNA( useable_cqDnaTab.at( 0 ) );
		_data[ ridx ][ cidx ].setCqRNA( cqRnaTab.at( 0 ) );
		goto CALC_DONE;
	} else {
		_data[ ridx ][ cidx ].setCqDNA(
		  Mean<double>( useable_cqDnaTab ) );
		_data[ ridx ][ cidx ].setCqRNA(
		  Mean<double>( cqRnaTab ) );
	}
*/
	/*grade = vpScore( ridx, cidx );
	
	_data[ ridx ][ cidx ].setCqDNA( Mean<double>( cqDnaTab ) );
	if( grade == VP::FLAG_A ) {
		// CqRNA = INPUT
		//_data[ ridx ][ cidx ].setCqRNA( cqNa );
		// cqNA() is output, no need to store the value
		goto CALC_DONE;
	} else if( grade == VP::FLAG_F ) {
		// HIGHDNA is an output only in CqRNA flag, no need to do anything here
		// CqRNA = ND
		//_data[ ridx ][ cidx ].setFlag( VP::HIGHDNA );
		calcReport.mesg = "HIGHDNA";
		goto CALC_DONE;
	} else if( _goiSummary[ goi ]._flag == VP::LOWCONF ) {
		// GoiSummary constructor sets _flag to VP::CALC
		// so if _validateAssay == false this will not happen
		// HIGHSD
		// CqRNA = ND
		_data[ ridx ][ cidx ].setFlag( VP::HIGHSD );
		goto CALC_DONE;
	}
	*/
	/* CqRNA = calculated value
	{	// THIS SHOULD ONLY HAPPEN IF NOT GOTO CALC_DONE

		foreach( double cqDna, cqDnaTab ) {
			cqRna = Kubi( cqNa, cqDna );
			if( cqRna > 0 ) {
				cqRnaTab << cqRna;
				useable_cqDnaTab << cqDna;
			}
		}
		calcReport.cqRnaCount = cqRnaTab.size();
		calcReport.cqRna = Mean<double>( cqRnaTab );
		calcReport.cqRnaSd = SD<double>( cqRnaTab );
		calcReport.cqDnaCount2 = useable_cqDnaTab.size();
		calcReport.cqDna2 = Mean<double>( useable_cqDnaTab );

		if( cqRnaTab.size() == 0 ) {
		//
		// TODO make this a setError condition
		// to make sure if it happens during testing
			_data[ ridx ][ cidx ].setFlag( VP::K0 );
			calcReport.mesg = "K0";
			goto CALC_DONE;
		} else if( cqRnaTab.size() == 1 ) {
			_data[ ridx ][ cidx ].setCqDNA( useable_cqDnaTab.at( 0 ) );
			_data[ ridx ][ cidx ].setCqRNA( cqRnaTab.at( 0 ) );
			goto CALC_DONE;
		} else {
			_data[ ridx ][ cidx ].setCqDNA(
			  Mean<double>( useable_cqDnaTab ) );
			_data[ ridx ][ cidx ].setCqRNA(
			  Mean<double>( cqRnaTab ) );
		}
	}	// END BAD WHITESPACE
	*/
/*
	if( pctCalc >= _gradeC ) {
		// this is an F condition
		//
		_data[ ridx ][ cidx ].setFlag( VP::HIGHDNA );
		_data[ ridx ][ cidx ].setCqDNA( Mean<double>( cqDnaTab ) );
		calcReport.mesg = "HIGHDNA";
		goto CALC_DONE;
	} else if( _goiSummary[ goi ]._flag == VP::LOWCONF ) {
		_data[ ridx ][ cidx ].setFlag( VP::HIGHSD );
		_data[ ridx ][ cidx ].setCqDNA( Mean<double>( cqDnaTab ) );
		// SKIP THE REPORT FOR HIGHSD
		goto CALC_DONE;
	} else {
		//
		// TODO there is the alternate calcs:
		//
		//	 CqRna
		//0	 Mean<double>( cqRnaTab );
		//	-> here we have 2 SD, one for CqRna and CqDna
		//
		//1	 Kubi( cqNa, Mean<double>( cqDnaTab ) )
		// 	-> no SD CqRna but we do have the SD for cqDna
		//
		// send this into Cell and output in allinone
		//
		// need to find out which one is "better"
		//
		foreach( double cqDna, cqDnaTab ) {
			cqRna = Kubi( cqNa, cqDna );
			if( cqRna > 0 ) {
				cqRnaTab << cqRna;
				useable_cqDnaTab << cqDna;
			}
		}
		calcReport.cqRnaCount = cqRnaTab.size();
		calcReport.cqRna = Mean<double>( cqRnaTab );
		calcReport.cqRnaSd = SD<double>( cqRnaTab );
		calcReport.cqDnaCount2 = useable_cqDnaTab.size();
		calcReport.cqDna2 = Mean<double>( useable_cqDnaTab );

		if( cqRnaTab.size() == 0 ) {
		//
		// TODO make this a setError condition
		// to make sure if it happens during testing
			_data[ ridx ][ cidx ].setFlag( VP::K0 );
			calcReport.mesg = "K0";
			goto CALC_DONE;
		} else if( cqRnaTab.size() == 1 ) {
			_data[ ridx ][ cidx ].setCqDNA( useable_cqDnaTab.at( 0 ) );
			_data[ ridx ][ cidx ].setCqRNA( cqRnaTab.at( 0 ) );
			goto CALC_DONE;
		} else {
			_data[ ridx ][ cidx ].setCqDNA(
			  Mean<double>( useable_cqDnaTab ) );
			_data[ ridx ][ cidx ].setCqRNA(
			  Mean<double>( cqRnaTab ) );
		}
	}
*/
CALC_DONE:;
	if( calcReport.mesg == "CALC" ) {
		_data[ ridx ][ cidx ].confirmCqNA();
	}
	_calcReport << calcReport;

	return( rv );
}
bool	VPStore::run()
{
// TODO
//	BE READY TO HAVE RUN HIT AGAIN, NEED TO UNDO PREVIOUS RUN FLAGS!
//
//	IDEALLY ALSO NEED GUI LOGIC SO IF A CHECK PARAM IS CHANGED,
//	 CHECK HAS TO BE RUN AGAIN
//

	_calcReport.clear();

	foreach( QString goi, _outputCols ) {
		// first figure out if this goi is aplus
		int cidx = colIndex( goi );
		// if the goi is not in the goiSummary then what is it?
		if( !_goiSummary.contains( goi ) ) {
			// NOT SURE WHAT THIS CASE IS...DOES IT HAPPEN?
			foreach( QString sample, _outputRows ) {
				int	ridx = rowIndex( sample );
				_data[ ridx ][ cidx ].setFlag( VP::ERROR );
			}
qDebug() << "run() continues as " << goi << " is not in summary";
			continue;
		}
		if( _goiSummary[ goi ].isFlagged() ) {
if( goi == "36B4" ) {
	qDebug() << goi << " is flagged";
}
			switch( _goiSummary[ goi ]._flag ) {
				case	VP::APLUS3:
					foreach( QString sample, _outputRows ) {
						int	ridx = rowIndex( sample );
						if( !_data[ ridx ][ cidx ].inputFlagged() ) {
							_data[ ridx ][ cidx ].setFlag( VP::AA3 );
							_data[ ridx ][ cidx ].setGrade( VP::FLAG_AA3 );
						}
			/* THIS WAS HAPPENING EVERYWHERE
						_data[ ridx ][ cidx ].setCqRNA(
					 	_data[ ridx ][ cidx ].input().toDouble() );
			*/
					}
					break;
				case	VP::APLUS2:
					foreach( QString sample, _outputRows ) {
						int	ridx = rowIndex( sample );
						if( !_data[ ridx ][ cidx ].inputFlagged() ) {
							_data[ ridx ][ cidx ].setFlag( VP::AA2 );
							_data[ ridx ][ cidx ].setGrade( VP::FLAG_AA2 );
						}
			/* THIS WAS HAPPENING EVERYWHERE

						_data[ ridx ][ cidx ].setCqRNA(
					 	_data[ ridx ][ cidx ].input().toDouble() );
			*/
					}
					break;
				case	VP::APLUS:
					foreach( QString sample, _outputRows ) {
						int	ridx = rowIndex( sample );
						if( !_data[ ridx ][ cidx ].inputFlagged() ) {
							_data[ ridx ][ cidx ].setFlag( VP::AA );
							_data[ ridx ][ cidx ].setGrade( VP::FLAG_AA );
						}
			/* THIS WAS HAPPENING EVERYWHERE
						_data[ ridx ][ cidx ].setCqRNA(
					 	_data[ ridx ][ cidx ].input().toDouble() );
			*/
					}
					break;
/*
				case	VP::LOWCONF:
					foreach( QString sample, _outputRows ) {
						int	ridx = rowIndex( sample );
						if( _failSamples.contains( sample ) ) {
							_data[ ridx ][ cidx ].setFlag( VP::ND );
						} else if( _astarSamples.contains( sample ) ) {
							_data[ ridx ][ cidx ].setFlag( VP::ASTAR );
						} else {
							calc( goi, sample );
						}
						//_data[ ridx ][ cidx ].setFlag( VP::HIGHSD );
					}
					break;
*/
				default:
					break;
			}
		} else {
			// GoiSummary._flag == VP::CALC : the default condition
			foreach( QString sample, _outputRows ) {
				int	ridx = rowIndex( sample );
				if( !_data[ ridx ][ cidx ].inputFlagged() ) {
					if( _failSamples.contains( sample ) ) {
if( goi == "36B4" ) {
	qDebug() << "FAIL Have " << ridx << " for " << sample;
}
						_data[ ridx ][ cidx ].setFlag( VP::ND );
					} else if( _astarSamples.contains( sample ) ) {
						_data[ ridx ][ cidx ].setFlag( VP::ASTAR );
						_data[ ridx ][ cidx ].setGrade( VP::FLAG_ASTAR );
					} else {
						calc( goi, sample );
					}
				} else {
if( goi == "36B4" ) {
	qDebug() << " UGH Have " << ridx << " for " << sample;
				}
}
			}
		}
		summarizeGoi( &_goiSummary[ goi ] );
	}
	return( true );
}
void	VPStore::summarizeGoi( GoiSummary *goiSummary )
{
	QStringList	grades;
	int	cidx = colIndex( goiSummary->goi );
	foreach( QString sample, _outputRows ) {
		int ridx = rowIndex( sample );
		grades << _data[ ridx ][ cidx ].grade();
	//HERE TODO
		//grades << vpScore( ridx, cidx );
	}
	// these cases are GOI-global, so not worth doing
	//
	goiSummary->nAA3 = grades.count( VP::FLAG_AA3 );
	goiSummary->nAA2 = grades.count( VP::FLAG_AA2 );
	goiSummary->nAA = grades.count( VP::FLAG_AA );
	goiSummary->nHIGHSD = grades.count( VP::FLAG_HIGHSD );

	goiSummary->nASTAR = grades.count( VP::FLAG_ASTAR );
	goiSummary->nA = grades.count( VP::FLAG_A );
	goiSummary->nB = grades.count( VP::FLAG_B );
	goiSummary->nC = grades.count( VP::FLAG_C );
	goiSummary->nF = grades.count( VP::FLAG_F );
	//goiSummary->nHIGHDNA = grades.count( VP::FLAG_HIGHDNA );
	goiSummary->nND = grades.count( VP::FLAG_ND );
}

	//
	//	load methods
	//
void	VPStore::deleteData()
{
	if( _data == 0 ) {
		return;
	}
/*
	THIS WAS SEQ-FAULTING
	TODO FIGURE OUT WHY
	
*/
	for( int i = 0; i < _dataRowSize; ++i ) {
		//if( _data[ i ] ) {
			delete[] _data[ i ];
		//}
	}
	delete[] _data;
	_dataRowSize = _dataColSize = 0;
	_data = 0;
	return;
}
bool	VPStore::newData()
{
	if( _inputRows.size() == 0 || _inputCols.size() == 0 ) {
		setCritical( "VPStore::newData() called with invalid input" );
		return( false );
	}
	if( _data != 0 ) {
		deleteData();
	}
	_dataRowSize = _inputRows.size();
	_dataColSize = _inputCols.size();
	if( ( _data = new VPCell*[ _dataRowSize ] ) == 0 ) {
		setCritical( "VPStore::newData() out of memory" );
		return( false );
	}
	for( int i = 0; i < _dataRowSize; ++i ) {
		_data[ i ] = new VPCell[ _dataColSize ];
	}
	return( true );
}


int	VPStore::rowIndex( const QString& label ) const
{
	return( _inputRows.indexOf( label ) );
}
int	VPStore::colIndex( const QString& label ) const
{
	return( _inputCols.indexOf( label ) );
}
QString	VPStore::rowString( const int& index ) const
{
	QString	rv;

	if( index >= 0 && index < _inputRows.size() ) {
		rv = _inputRows.at( index );
	}
	return( rv );
}
QString	VPStore::colString( const int& index ) const
{
	QString	rv;

	if( index >= 0 && index < _inputCols.size() ) {
		rv = _inputCols.at( index );
	}
	return( rv );
}
/*
QString	VPStore::uniqueRowLabel( const Row& row ) const
{
	//return( QString( "%2_Row_%1" ).arg( S( row[ COLUMN_NID ] ) )
	return( QString( "_%1_%2" ).arg( S( row[ COLUMN_NID ] ) )
		.arg( S( row[ COLUMN_SAMPLE ] ) ) ); 
}
*/
VP::Flag VPStore::getFlag( const QString& rowname, const QString& colname )
{
	int	r = rowIndex( rowname );
	int	c = colIndex( colname );
	if( r != UINT && c != UINT ) {
		return( _data[ r ][ c ].flag() );
	}
	return( VP::ERROR );
}
	// FAIRLY SURE THESE ARE DEPRECATED AND CAN BE REMOVED
	//
void	VPStore::setInput( const QString& rowname, const QString& colname,
		const QVariant& value )
{
	int	r = rowIndex( rowname );
	int	c = colIndex( colname );
	if( r != UINT && c != UINT ) {
		_data[ r ][ c ].setInput( value );
	}
}
void	VPStore::setInput( const QString& rowname, const QString& colname,
	const double& value, const VP::Flag& flag )
{
	int	r = rowIndex( rowname );
	int	c = colIndex( colname );
	if( r != UINT && c != UINT ) {
		_data[ r ][ c ].setCqNA( value );
		_data[ r ][ c ].setFlag( flag );
	}
}
bool	VPStore::load( const QString& file, const QString& format )
{
	_inputFile = file;
	_inputFormat = format;
	return( load() );
}
bool	VPStore::load()
{
	bool	rv = false;
	//
	// common code for all input formats
	//
	_inputRows.clear();
	_inputCols.clear();
	QString	buf = Ifp::load( _inputFile );
	if( buf.size() == 0 || buf == USTR ) {
		setCritical( "Empty or invalid input file" );
		return( rv );
	}
	QStringList	lines = buf.split( "\n", QString::KeepEmptyParts );
	for( int i = 0; i < lines.size(); ++i ) {
		QString s = lines.at( i ).trimmed();
		if( s.size() == 0 ) {
			lines[ i ] = "";
		}
	}
	if( _inputFormat == VP::FLUID ) {
		rv = parseFluidigm( lines );
	} else if( _inputFormat == VP::STEPONE ) {
		rv = parseStepone( lines );
	} else if( _inputFormat == VP::CUSTOM ) {
		// need those pesky parameters
		//rv = parseSpreadsheet( lines );
	} else if( _inputFormat == VP::SIMPLE ) {
		rv = parseSpreadsheet( lines );
	}
	return( rv );
}
bool	VPStore::parseFluidigm( const QStringList& lines )
{
	int     lnum = 0;
        QStringList	tokens;
        QString		rlabel, clabel;
	int		ridx, cidx;
        Row     row;
        int     dataStart = UINT;
        int     qualityStart = UINT;
	QString	save1, save2;
	int	nWarn = 0;

        for( ; lnum < lines.size() && !lines.at( lnum ).isEmpty(); ++lnum ) {
                _inputFileHeader << lines.at( lnum );
        }
        for( ; lnum < lines.size() && lines.at( lnum ).isEmpty(); ++lnum ) {
        }
        ++lnum;
        if( lnum < lines.size() && lines.at( lnum ).endsWith( "Ct" ) ) {
                ++lnum;
        }
        if( lnum >= lines.size() - 1 ) {
		setCritical( "Biomark parser failed to distinguish header and data in input" );
                return( false );
        }
	save1 = lines.at( lnum );
        ++lnum;
	save2 = lines.at( lnum );
        tokens = lines.at( lnum ).split( "\t", QString::SkipEmptyParts );
        _inputCols = tokens;
        row.setSeparator( "\t" );
        // add in the empty cols to use the Lexer
        tokens = QStringList() << COLUMN_NID << COLUMN_SAMPLE << tokens;
        row.attachHeader( tokens );
        ++lnum;
        dataStart = lnum;
        qualityStart = UINT;
        for( ; lnum < lines.size(); ++lnum ) {
                if( lines.at( lnum ).startsWith( "Quality" ) ) {
			lnum++;
			if( lnum >= lines.size()
			 && lines.at( lnum ) != save1 ) {
				break;
			}
			lnum++;
			if( lnum >= lines.size()
			 && lines.at( lnum ) != save2 ) {
				break;
			}
			++lnum;
                        qualityStart = lnum;
                        break;
                }
                row.split( lines.at( lnum ) );
		rlabel = VP::UniqueLabel( S( row[ COLUMN_SAMPLE ] ), I( row[ COLUMN_NID ] ) );
                //rlabel = uniqueRowLabel( row );
                if( !_inputRows.contains( rlabel ) ) {
                        _inputRows << rlabel;
                }
        }
	if( !newData() ) {
		setCritical(
		 QString( "Biomark parse error: newData() failed." ) );
		return( false );
	}
        for( lnum = dataStart; lnum < lines.size() &&
	 lnum < dataStart + _inputRows.size(); ++lnum ) {
                row.split( lines.at( lnum ) );
		rlabel = VP::UniqueLabel( S( row[ COLUMN_SAMPLE ] ), I( row[ COLUMN_NID ] ) );
                //rlabel = uniqueRowLabel( row );
		ridx = rowIndex( rlabel );
                for( int j = 0; j < _inputCols.size(); ++j ) {
			QString	clabel = _inputCols.at( j );
			_data[ ridx ][ j ].setInput( row[ _inputCols.at( j ) ] );
			if( IsFluidigmNoamp( _data[ ridx ][ j ].input() ) ) {
				_data[ ridx ][ j ].setFlag( VP::NOAMP );
				_data[ ridx ][ j ].setInputFlagged();
			}
                }
        }
        if( qualityStart == UINT ) {
                return( true );
        }
	lnum = qualityStart;
        for( ; lnum < lines.size() &&
	 lnum < qualityStart + _inputRows.size(); ++lnum ) {
                row.split( lines.at( lnum ) );
		rlabel = VP::UniqueLabel( S( row[ COLUMN_SAMPLE ] ), I( row[ COLUMN_NID ] ) );
                //rlabel = uniqueRowLabel( row );
		ridx = rowIndex( rlabel );
		if( ridx == UINT ) {
			++nWarn;
			continue;
		}
                for( int j = 0; j < _inputCols.size(); ++j ) {
			if( j >= _inputCols.size() ) {
				setCritical( "Error: input file has a format error" );
				return( false );
			}
			clabel = _inputCols.at( j );
			cidx = colIndex( clabel );
			if( cidx == UINT ) {
				++nWarn;
				continue;
			}
                        if( S( row[ clabel ] ) == "Fail" &&
			 _data[ ridx ][ cidx ].flag() != VP::NOAMP ) {
			 //getFlag( rlabel, clabel ) != VP::NOAMP ) {
				_data[ ridx ][ j ].setFlag( VP::EXPFAIL );
				_data[ ridx ][ j ].setInputFlagged();
			}
                }
        }
	if( nWarn > 0 ) {
		setWarning( "BioMark parser had warnings: check inconsistencies in assay/sample labels" );
		return( false );
	}
	return( true );
}
bool	VPStore::parseStepone( const QStringList& lines )
{
	int	CT_POS = APP_I( "stepone/col_ct" );
	QString	COL_CT = "Ct";
	QString	COL_WELL = APP_S( "stepone/col_well" );
	QString	COL_SAMPLE = APP_S( "stepone/col_sample" );
	QString	COL_TARGET = APP_S( "stepone/col_target" );
	QString	SO_NOAMP = APP_S( "stepone/noamp_flag" ).toLower();

	int	lnum = 0;
	QStringList	tokens;
	Row		row;
	int	dataStart = UINT;

	for( ; lnum < lines.size() && !lines.at( lnum ).isEmpty(); ++lnum ) {
		;	// used to skip(could capture) the header
	}
	for( ; lnum < lines.size() && lines.at( lnum ).isEmpty(); ++lnum ) {
		;	// skip empty lines after header
	}
	if( lnum >= lines.size() ) {
		setCritical(
		 QString( "StepOne parse error: "
		 "file ended after %1 lines of header" )
		 .arg( lnum ) );
		return( false );
	}
	tokens = lines.at( lnum ).split( "\t" );
	row.setSeparator( "\t" );
	if( tokens.size() < CT_POS ) {
		setCritical(
		 QString( "StepOne parse error: "
		 "header line too short %1 tokens found" )
		 .arg( tokens.size() ) );
		return( false );
	}
	tokens[ CT_POS ] = COL_CT;
	if( !tokens.contains( COL_WELL ) ) {
		setCritical(
		 QString( "StepOne parse error: "
		 "header line missing '%1' Well token" )
		 .arg( COL_WELL ) );
		return( false );
	}
	if( !tokens.contains( COL_SAMPLE ) ) {
		setCritical(
		 QString( "StepOne parse error: "
		 "header line missing '%1' Sample token" )
		 .arg( COL_SAMPLE ) );
		return( false );
	}
	if( !tokens.contains( COL_TARGET ) ) {
		setCritical(
		 QString( "StepOne parse error: "
		 "header line missing '%1' Target token" )
		 .arg( COL_TARGET ) );
		return( false );
	}
	row.attachHeader( tokens );
/*
	if( !tokens.contains( COL_TARGET ) ) {
		for( int i = 0; i < tokens.size(); ++i ) {
			if( tokens.at( i ).startsWith( "Target" ) ) {
				TARGET_COL = tokens.at( i );
				break;
			}
		}
	}
	if( !tokens.contains( TARGET_COL ) ) {
		// TODO set error here
		return( false );
	}
*/
	++lnum;
	dataStart = lnum;
	int	rowSize = 0;
	int	colSize = 0;
	for( ; lnum < lines.size(); ++lnum ) {
		row.split( lines.at( lnum ) );
		QString	well = S( row[ COL_WELL ] );
		QChar	colCh = well.at( 0 );
		// let both be 1-based
		int	colInt = well.mid( 1 ).toInt();
		int	rowInt = ( colCh.toAscii() - 'A' ) + 1;

		if( rowInt > rowSize ) {
			rowSize = rowInt;
		}
		if( colInt > colSize ) {
			colSize = colInt;
		}
		if( !_inputRows.contains( S( row[ COL_SAMPLE ] ) ) ) {
			_inputRows << S( row[ COL_SAMPLE ] );
		}
		if( !_inputCols.contains( S( row[ COL_TARGET ] ) ) ) {
			_inputCols << S( row[ COL_TARGET ] );
		}
	}
	colSize -= 1;
	rowSize -= 1;
	if( colSize > _inputCols.size() ) {
		for( int x = 0; x < ( colSize - _inputCols.size() ); ++x ) {
			_inputCols << QString( "UnusedTarget%1" ).arg( x + 1 );
		}
	}
	if( rowSize > _inputRows.size() ) {
		for( int x = 0; x < ( rowSize - _inputRows.size() ); ++x ) {
			_inputRows << QString( "UnusedSample%1" ).arg( x + 1 );
		}
	}
	if( !newData() ) {
		setCritical(
		 QString( "StepOne parse error: newData() failed." ) );
		return( false );
	}
	for( lnum = dataStart; lnum < lines.size(); ++lnum ) {
		row.split( lines.at( lnum ) );
		int ridx = rowIndex( S( row[ COL_SAMPLE ] ) );
		int cidx = colIndex( S( row[ COL_TARGET ] ) );
		if( S( row[ COL_CT ] ).isEmpty() ) {
			_data[ ridx ][ cidx ].setFlag( VP::EXPFAIL );
			_data[ ridx ][ cidx ].setInputFlagged();
			continue;
		}
		if( S( row[ COL_CT ] ).toLower() == SO_NOAMP ) {
			_data[ ridx ][ cidx ].setFlag( VP::NOAMP );
			_data[ ridx ][ cidx ].setInputFlagged();
			continue;
		}
		_data[ ridx ][ cidx ].setInput( D( row[ COL_CT ] ) );
	}
	return( true );
}
bool	VPStore::parseSpreadsheet( const QStringList& lines )
{
        // TODO
        // inputHeader can be read using a rSkip and
        // columns can be skipped by inserting dummy cols in the header
        int     lnum = 0;
        QStringList     tokens;
        QString rlabel;
        Row     row;
        int     dataStart = UINT;

        //tokens = lines.at( lnum ).split( "\t", QString::SkipEmptyParts );
        tokens = lines.at( lnum ).split( "\t" );
        if( tokens.at( 0 ).isEmpty() ) {
                tokens[ 0 ] = "ID";
        }
        for( int i = 1; i < tokens.size(); ++i ) {
                _inputCols << tokens.at( i );
        }
        row.setSeparator( "\t" );
        row.attachHeader( tokens );
        ++lnum;
        dataStart = lnum;

        for( ; lnum < lines.size(); ++lnum ) {
                if( lines.at( lnum ).isEmpty() ) {
                        break;
                }
                row.split( lines.at( lnum ) );
		rlabel = VP::UniqueLabel( S( row[ tokens.at( 0 ) ] ), lnum );
   //rlabel = QString( "_%1_%2" ).arg( lnum ).arg( S( row[ tokens.at( 0 ) ] ) );
                if( !_inputRows.contains( rlabel ) ) {
                        _inputRows << rlabel;
                }
        }
	if( !newData() ) {
		return( false );
	}
        // _cols cannot have ID at col-0 to make a C-style double array
        for( lnum = dataStart; lnum < dataStart + _inputRows.size(); ++lnum ) {
                row.split( lines.at( lnum ) );
		rlabel = VP::UniqueLabel( S( row[ tokens.at( 0 ) ] ), lnum );
   //rlabel = QString( "_%1_%2" ).arg( lnum ).arg( S( row[ tokens.at( 0 ) ] ) );
		int	ridx = rowIndex( rlabel );
                for( int j = 0; j < _inputCols.size(); ++j ) {
			if( IsFluidigmNoamp( row[ _inputCols.at( j ) ] ) ) {
				_data[ ridx ][ j ].setFlag( VP::NOAMP );
                                continue;
                        }
			_data[ ridx ][ j ].setInput( row[ _inputCols.at( j ) ] );
                }
        }
	return( true );
}
	//
	//	DEBUG METHODS
	//
void	VPStore::debugLoad( QTextStream& fp )
{
	fp << "STORE: " << _inputFile << " ( " << _inputFormat << " )" << endl;
	fp << "DATA: " << _inputRows.size() << " by " << _inputCols.size() << endl;
	for( int i = 0; i < _inputRows.size(); ++i ) {
		for( int j = 0; j < _inputCols.size(); ++j ) {
			fp << i << "\t" << j << "\t"
			<< _inputRows.at( i ) << "\t"
			<< _inputCols.at( j ) << "\t"
			<< _data[ i ][ j ].cqNA() << "\t"
			<< _data[ i ][ j ].flagString()
			<< endl;
		}
	}
}
void	VPStore::debugCheck( QTextStream& fp )
{
	fp << "CHECK" << endl;
}
void	VPStore::dump( QTextStream& fp )
{
	for( int i = 0; i < _outputRows.size(); ++i ) {
		fp << _outputRows.at( i ) << "\t";
		for( int j = 0; j < _outputCols.size(); ++j ) {
		}
	}
}
void	VPStore::debugRun( QTextStream& fp )
{
	QString	hdrStr = "Row,Col,RowName,ColName,INPUT,FLAG,CqDNA,CqRNA,CqNA-CALC";
	QStringList	hdr = hdrStr.split( "," );

	fp << hdr.join( "\t" ) << endl;

	for( int i = 0; i < _outputRows.size(); ++i ) {
		for( int j = 0; j < _outputCols.size(); ++j ) {
			double	test = -1;
			if( _data[ i ][ j ].flag() == VP::NADA ) {
				test = CqNa( _data[i][j].cqRNA(), _data[i][j].cqDNA() );
			}
			fp << i << "\t" << j << "\t"
			<< _outputRows.at( i ) << "\t"
			<< _outputCols.at( j ) << "\t"
			<< _data[ i ][ j ].cqNA() << "\t"
			<< _data[ i ][ j ].flagString() << "\t"
			<< _data[ i ][ j ].cqDNA() << "\t"
			<< _data[ i ][ j ].cqRNA()
			 << "\t" << test
			<< endl;
		}
	}
}
	//
	//	THESE ACCESSOR METHODS ARE CONST AND FIXED
	//
QStringList	VPStore::inputRows() const
{
	return( _inputRows );
}
QStringList	VPStore::outputRows() const
{
	return( _outputRows );
}
QStringList	VPStore::inputCols() const
{
	return( _inputCols );
}
QStringList	VPStore::outputCols() const
{
	return( _outputCols );
}
	//
	//	Access internal data types for each DataRole
	//
QStringList	VPStore::header( const VP::DataRole& role ) const
{
	QStringList	rv;

	switch( role ) {
		case	VP::Input:
			rv = inputHeader();
			break;
		case	VP::Working:
			rv = workingHeader();
			break;
		case	VP::InOut:
			rv = inoutHeader();
			break;
		default:
			rv = outputHeader();
			break;
	}
	return( rv );
}
QStringList	VPStore::verticalHeader( const VP::DataRole& role ) const
{
	QStringList	rv;

	switch( role ) {
		case	VP::Input:
			rv = inputVerticalHeader();
			break;
		case	VP::Working:
			rv = workingVerticalHeader();
			break;
		case	VP::InOut:
			rv = inoutVerticalHeader();
			break;
		default:
			rv = outputVerticalHeader();
			break;
	}
	return( rv );
}
QList<Row>	VPStore::data( const VP::DataRole& role ) const
{
	QList<Row>	rv;

	switch( role ) {
		case	VP::Input:
			rv = inputData();
			break;
		case	VP::Working:
			rv = workingData();
			break;
		case	VP::InOut:
			rv = inoutData();
			break;
		case	VP::CqRna:
		case	VP::CqDna:
		case	VP::PctDna:
		case	VP::Score:
			rv = outputData( role );
			break;
		default:
			break;
	}
	return( rv );
}
	//
	// Internal header (COLS) methods
	//
QStringList	VPStore::inputHeader() const
{
	return( QStringList() << _inputCols );
}
QStringList	VPStore::workingHeader() const
{
	return( QStringList() << _vpaCol << _outputCols );
}
QStringList	VPStore::inoutHeader() const
{
	return( QStringList() << _outputCols << _vpaCol );
}
QStringList	VPStore::outputHeader() const
{
	return( QStringList() << _outputCols );
}
	//
	// Internal vertical header (ROWS) methods
	//
QStringList	VPStore::inputVerticalHeader() const
{
	return( QStringList() << _inputRows );
}
QStringList	VPStore::workingVerticalHeader() const
{
	QStringList	rv;
	foreach( int ridx, _gDnaRowIndexes ) {
		rv << _inputRows.at( ridx );
	}
	rv << _outputRows;
	return( rv );
}
QStringList	VPStore::inoutVerticalHeader() const
{
	return( QStringList() << _outputRows );
}
QStringList	VPStore::outputVerticalHeader() const
{
	return( QStringList() << _outputRows );
}
	//
	// Internal data access methods for VPStore
	//
	//	only input can directly use headers
	//	 the rest need to translate each string back to an original input index
	//
QList<Row>	VPStore::inputData() const
{
	QList<Row>	rv;
	QStringList	hdr = inputHeader();
	QStringList	vhdr = inputVerticalHeader();

	for( int i = 0; i < vhdr.size(); ++i ) {
		Row	t;
		for( int j = 0; j < hdr.size(); ++j ) {
			t.insert( hdr.at( j ), _data[ i ][ j ].data( VP::Input ) );
		}
		rv << t;
	}
	return( rv );
}
QList<Row>	VPStore::workingData() const
{
	QList<Row>	rv;
	QStringList	hdr = workingHeader();
	QStringList	vhdr = workingVerticalHeader();

	for( int i = 0; i < vhdr.size(); ++i ) {
		Row	t;
		int	ridx = rowIndex( vhdr.at( i ) );
		for( int j = 0; j < hdr.size(); ++j ) {
			int	cidx = colIndex( hdr.at( j ) );
			t.insert( hdr.at( j ), _data[ ridx ][ cidx ].data( VP::Input ) );
		}
		rv << t;
	}
/*
	foreach( int ridx, _gDnaRowIndexes ) {
		Row t;
		t.insert( COLUMN_SAMPLE, _inputRows.at( ridx ) );
		t.insert( _inputCols.at( _vpaColIndex ), _data[ ridx ][ _vpaColIndex ].input() );
		for( int j = 0; j < _outputCols.size(); ++j ) {
			int cidx = colIndex( _outputCols.at( j ) );
			t.insert( _inputCols.at( cidx ), _data[ ridx ][ cidx ].data( VP::Working ) );
		}
		rv << t;
	}
	for( int i = 0; i < _outputRows.size(); ++i ) {
		Row t;
		int	ridx = rowIndex( _outputRows.at( i ) );
		t.insert( COLUMN_SAMPLE, _inputRows.at( ridx ) );
		t.insert( _inputCols.at( _vpaColIndex ), _data[ ridx ][ _vpaColIndex ].input() );
		for( int j = 0; j < _outputCols.size(); ++j ) {
			int cidx = colIndex( _outputCols.at( j ) );
			t.insert( _inputCols.at( cidx ), _data[ ridx ][ cidx ].data( VP::Working ) );
		}
		rv << t;
	}
*/
	return( rv );
}
QList<Row>	VPStore::inoutData() const
{
	QList<Row>	rv;
	QStringList	hdr = inoutHeader();
	QStringList	vhdr = inoutVerticalHeader();

	for( int i = 0; i < vhdr.size(); ++i ) {
		Row	t;
		int	ridx = rowIndex( vhdr.at( i ) );
		for( int j = 0; j < hdr.size(); ++j ) {
			int	cidx = colIndex( hdr.at( j ) );
			t.insert( hdr.at( j ), _data[ ridx ][ cidx ].data( VP::Input ) );
		}
		rv << t;
	}
	return( rv );
}
	// THIS ONE CANNOT BE TOTALLY GENERIC AS THE SCORE IS NOT IN VPCELL
QList<Row>	VPStore::outputData( const VP::DataRole& role ) const
{
	QList<Row>	rv;
	QStringList	hdr = outputHeader();
	QStringList	vhdr = outputVerticalHeader();
	
	for( int i = 0; i < vhdr.size(); ++i ) {
		Row t;
		int	ridx = rowIndex( vhdr.at( i ) );
		t.insert( COLUMN_SAMPLE, _inputRows.at( ridx ) );
		for( int j = 0; j < hdr.size(); ++j ) {
			int cidx = colIndex( hdr.at( j ) );
			t.insert( _inputCols.at( cidx ), _data[ ridx ][ cidx ].data( role ) );
/* TODO check this change
			if( role == VP::Score ) {
				t.insert( _inputCols.at( cidx ), vpScore( ridx, cidx ) );
			} else {
				t.insert( _inputCols.at( cidx ), _data[ ridx ][ cidx ].data( role ) );
			}
*/
		}
		rv << t;
	}
	return( rv );
}
QString		VPStore::vpScore( const int& ridx, const int& cidx ) const
{
	QString	rv;
	double	pct;

	switch( _data[ ridx ][ cidx ].flag() ) {
		case	VP::NADA:
		case	VP::HIGHSD:
			//	PERCENT NOW STORED AS PERCENT
			pct = _data[ ridx ][ cidx ].pctDNA();
			if( pct == 0 ) {
qDebug() << "BEWARE A+err is possible";
				rv = "A+err";
			} else if( pct < _gradeA ) {
				rv = VP::FLAG_A;
			} else if( pct < _gradeB ) {
				rv = VP::FLAG_B;
			} else if( pct < _gradeC ) {
				rv = VP::FLAG_C;
			} else {
				rv = VP::FLAG_F;
			}
			break;
		case	VP::AA3:
		case	VP::AA2:
		case	VP::AA:
		case	VP::ASTAR:
		case	VP::NOAMP:
		case	VP::OVERLOD:
		case	VP::EXPFAIL:
		case	VP::ND:
		case	VP::ERROR:
			// this never happens anymore
			rv = _data[ ridx ][ cidx ].flagString();
			break;
	}
	return( rv );
}
void	VPStore::outputFileHeader( QTextStream& fp )
{
	fp << "ValidPrime Version: " << VP::VERSION << endl;
	//fp << "This file contains: " << fileName << endl;
	fp << "Input file: " << _inputFile << endl;
	fp << "Input format: " << _inputFormat << endl;
	fp << "Date/Time: " << QDateTime::currentDateTime().toString( "MMM dd yyyy hh:mm" ) << endl;

	fp << "VPAssay: " << _vpaCol << endl;
	fp << "Valid gDNA rows:";
	foreach( int ridx, _gDnaVpaRowIndexes ) {
		fp << " " << _inputRows.at( ridx );
	}
	fp << endl;
/* TODO MAKE THIS WORK AGAIN?

	fp << "gDNA concentrations:";
	foreach( double c, _gDnaConcentrations ) {
		fp << " " << c;
	}
	fp << endl;
*/
	fp << "LinReg all-gDNA: slope " << gDnaLR.slope()
	 << "; Eff " << gDnaLR.e()
	 << "; R2 " << gDnaLR.r2() << endl;

	if( gDnaVpaLR.slope() != 0 && gDnaVpaLR.slope() != gDnaLR.slope() ) {
		fp << "LinReg VPA-gDNA: slope " << gDnaVpaLR.slope()
		 << "; Eff " << gDnaVpaLR.e()
		 << "; R2 " << gDnaVpaLR.r2() << endl;
	} else {
		fp << "LinReg VPA-gDNA: same as all-gDNA" << endl;
	}

	fp << "ValidateAssays: ";
	if( _validateAssay ) {
		fp << "true";
	} else {
		fp << "false";
	}
	fp << endl;

	fp << "LOO calculation performed: ";
	if( _performLoo ) {
		fp << "true";
	} else {
		fp << "false";
	}
	fp << endl;

	fp << "Cq-cutoff: " << _LOD << endl;
	fp << "DeltaCq Min Count: " << _minSdCount << endl;
	fp << "DeltaCq AllSD Cutoff: " << _highAllSdCutoff << endl;
	if( _performLoo ) {
		fp << "DeltaCq LooSD Cutoff: " << _highLooSdCutoff << endl;
	} else {
		fp << "DeltaCq LooSD Cutoff: " << "not performed"  << endl;
	}
	// PERCENT NOW STORED AS PERCENT
	fp << "Grade A: " << _gradeA << endl;
	fp << "Grade B: " << _gradeB << endl;
	fp << "Grade C: " << _gradeC << endl;

	fp << "Correction performed on A-Samples: ";
	if( _correctA ) {
		fp << "true";
	} else {
		fp << "false";
	}
	fp << endl;

	fp << endl;
}
bool	VPStore::saveReport( const QString& path )
{
	Ofp	fp;
	if( !fp.open( path ) ) {
		return( false );
	}
	fp << "VP Report" << endl;
	outputFileHeader( fp );
	CalcReport::ShowHeader( fp );

	fp << endl;
	foreach( CalcReport x, _calcReport ) {
		x.show( fp );
	}
	fp.close();
	return( true );
}
bool	VPStore::saveSummary( const QString& path )
{
	Ofp	fp;

	if( _goiSummary.size() == 0 ) {
		return( false );
	}
	if( !fp.open( path ) ) {
		return( false );
	}
	fp << "GOI Summary" << endl;
	outputFileHeader( fp );
	GoiSummary::ShowHeader( fp );
	foreach( QString goi, _goiSummary.keys() ) {
		_goiSummary[ goi ].show( fp );
	}
	fp.close();
	return( true );
}
