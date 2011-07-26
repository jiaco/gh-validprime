#include "vpstore.h"

using namespace GH;

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

GoiSummary::GoiSummary()
{
	goi = "ERROR";
	gDnaIndexes.clear();
	_flag = VP::CALC;
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
 	 ",nA*,nA,nB,nC,nF,nHIGHDNA,nK0,nND"
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
	 << "\t" << nHIGHDNA
	 << "\t" << nK0
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
	fp << "nHIGHDNA";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nHIGHDNA;
	}
	fp << endl;
	fp << "nK0";
	foreach( QString goi, _outputCols ) {
		fp << "\t" << _goiSummary[ goi ].nK0;
	}
	fp << endl;
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
			case	VP::LOWCONF:
				highsd << goi;
				break;
			default:
				break;
		}
		switch( _goiSummary[ goi ].confidence ) {
			case	VP::High:
				high << goi;
				break;
			case	VP::Med:
				med << goi;
				break;
			case	VP::Low:
				if( _goiSummary[ goi ]._flag != VP::LOWCONF ) {
					low << goi;
				}
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
	fp << "MedConfidence\t" << med.size() << "\t" << med.join( "," ) << endl;
	fp << "LowConfidence\t" << low.size() << "\t" << low.join( "," ) << endl;
}
	CalcReport::CalcReport()
{
	goi = sample = mesg = "";
	cqDnaCount = cqDnaCount2 = cqRnaCount = 0;
	cqInput = cqDna = pctDna = cqDna2 = cqRna = cqRnaSd = cqRna1 = 0.0;
}
void	CalcReport::ShowHeader( QTextStream& fp )
{
	QStringList hdr = QString(
		"GOI,Sample,Note"
		",CqInput"
		",CqDNA Count,CqDNA,PctDNA"
		",CqDNA Count-2,CqDNA-2,CqRNA Count,CqRNA,CqRNA-SD,CqRNA1"
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
	 << "\t" << cqDnaCount2
	 << "\t" << cqDna2
	 << "\t" << cqRnaCount
	 << "\t" << cqRna
	 << "\t" << cqRnaSd
	 << "\t" << cqRna1
	<< endl;
}

	VPCell::VPCell()
{
	_flag = VP::NADA;
	_inputFlagged = false;
	_input = QVariant();
	_cqNA = _cqDNA = _cqRNA = _cqRNA1 = _pctDNA = VP::FLAG;
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
	// have to override the flagging here to get the actual input
	if( role == VP::Input  || role == VP::InOut) {
		return( input() );
	}
	switch( _flag ) {
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
				case	VP::CqRna1:
					return( cqRNA1() );
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
				case	VP::CqRna1:
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
				case	VP::CqRna1:
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
	return( QVariant() );
}
QVariant	VPCell::input() const
{
	return( _input );
}
void	VPCell::setInput( const QVariant& value )
{
	_input = value;
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

	if( rv > 100 ) {
		rv = 100;
	}
	return( rv );
}
double	VPCell::cqRNA() const
{
	return( _cqRNA );
}
double	VPCell::cqRNA1() const
{
	return( _cqRNA1 );
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
void	VPCell::setCqRNA( const double& value )
{
	_cqRNA = value;
}
void	VPCell::setCqRNA1( const double& value )
{
	_cqRNA1 = value;
}

	VPStore::VPStore()
{
	_data = 0;
}
	VPStore::~VPStore()
{
	deleteData();
}
	// TODO
	// NEED TO SEE IF WE ALREADY HAVE DATA AND WHAT TO DO
	// TO GET RID OF IT

bool	VPStore::preload( CliApp* app )
{
	_inputFile = S( app->param( "InputFile" )->value() );
	_inputFormat = S( app->param( "InputFormat" )->value() );
	return( true );
}
bool	VPStore::precheck( CliApp* app )
{
	_LOD = D( app->param( "LOD" )->value() );
	_failFlag = S( app->param( "FailFlag" )->value() );
	_noampFlag = S( app->param( "NoampFlag" )->value() );

	_vpaCol = S( app->param( "VpaCol" )->value() );
	_gDnaRows = SL( app->param( "gDnaRows" )->value() );
	if( _gDnaRows.at( 0 ).contains( "," ) ) {
		_gDnaRows = _gDnaRows.at( 0 ).split( "," );
	}
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

	QList<TempConc>	tlist;
	for( int i = 0; i < _gDnaRows.size(); ++i ) {
		tlist << TempConc( _gDnaRows.at( i ) );
	}
	qSort( tlist.begin(), tlist.end(), TempConcSort );
	_gDnaConcentrations.clear();
	_gDnaConcValues.clear();
	for( int i = 0; i < tlist.size(); ++i ) {
		_gDnaRows[ i ] = tlist.at( i ).label;
		_gDnaConcentrations << tlist.at( i ).value;
		_gDnaConcValues << V( tlist.at( i ).value );
	}
	
	_ignoreCols = SL( app->param( "IgnoreCols" )->value() );
	_ignoreRows = SL( app->param( "IgnoreRows" )->value() );

// TODO
// second _cutoff for looSd
	_minCountSdNa = I( app->param( "MinCountSdNa" )->value() );
	_cutoffSdNa = D( app->param( "CutoffSdNa" )->value() );

	_cutHighAll = D( app->param( "CutHighAll" )->value() );
	_cutHighLoo = D( app->param( "CutHighLoo" )->value() );
	_cutMedAll = D( app->param( "CutMedAll" )->value() );
	_cutMedLoo = D( app->param( "CutMedLoo" )->value() );
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
	// TODO
	// in check, get some error messages out about missing params
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
LinReg	VPStore::getLinReg( const QList<int>& rows, const int& cidx ) const
{
	LinReg		rv;
	QList<double>	x, y;

	for( int i = 0; i < rows.size(); ++i ) {
		//int	ridx = _gDnaRowIndexes.at( i );
		int	ridx = rows.at( i );
		if( _data[ ridx ][ cidx ].inputFlagged() ) {
			continue;
		}
		x << log10( _gDnaConcentrations.at( i ) );
		y << _data[ ridx ][ cidx ].input().toDouble();
	}
	rv.setIndexes( rows );
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
		QString( "%1 cells input, %2 cells over-LOD and %3 cells caused error\n" )
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
	_checkMessage.append(
	 QString( "LinReg all-gDna slope %1, Eff %2, R2 %3\n" )
	  .arg( gDnaLR.slope() ).arg( gDnaLR.e() ).arg( gDnaLR.r2() )
	);
	_checkMessage.append(
	 QString( "LinReg vpa-gDna slope %1, Eff %2, R2 %3\n" )
	  .arg( gDnaVpaLR.slope() ).arg( gDnaVpaLR.e() ).arg(  gDnaVpaLR.r2() )
	);
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
	QString( "Set %1 cells to LOD + 1\n" ).arg( nFiction )
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
	int	nMed = 0;
	int	nLow = 0;

	double	APP_WEIGHT_NADA = -1;
	double	APP_WEIGHT_EXPFAIL = 0;
	double	APP_WEIGHT_OVERLOD = 1;
	double	APP_WEIGHT_NOAMP = 2;

	double	APP_CUTOFF_APLUS3 = 8;
	double	APP_CUTOFF_APLUS2 = 5;
	double	APP_CUTOFF_APLUS  = 2;

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
					score += APP_WEIGHT_EXPFAIL;
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
	//	IMPORTANT NOTE: _gDnaForGoi now actually has row indexes
	//
	// Now get SD for deltaNA (VPA versus GOI)
	//
	foreach( QString goi, _goiSummary.keys() ) {
		//
		// SKIP THE A+, A++, and A+++ cases
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
		
		if( _goiSummary[ goi ].gDnaIndexes.size() < _minCountSdNa ) {
			_goiSummary[ goi ].confidence = VP::Low;
			_goiSummary[ goi ]._flag = VP::LOWCONF;
			++nLow;
			_checkMessage.append(
			 QString( "%1 has ZERO confidence %2 out of %3 gDna Indexes\n" )
			 .arg( goi ).arg( _goiSummary[ goi ].gDnaIndexes.size() )
			 .arg( _gDnaVpaRowIndexes.size() ) );
			continue;
		}
		allSd = getSd( _goiSummary[ goi ].gDnaIndexes, cidx );
// TODO looSd only calculated if n >= 3
// set a default, flag, unused value
//
			// TODO Hardwired 4 for now, should it be _minCountSdNa + 1?
			//
		if( _goiSummary[ goi ].gDnaIndexes.size() >= 4 ) {
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
		_goiSummary[ goi ].gDnaOutlier = _inputRows.at(
			_goiSummary[ goi ].gDnaIndexes[ 
			   _goiSummary[ goi ].gDnaIndexes.indexOf( minAt )
			]
		);

		if( allSd <= _cutHighAll || ( minAt != UINT && looSd <= _cutHighLoo ) ) {
			_goiSummary[ goi ].confidence = VP::High;
		} else if( allSd <= _cutMedAll || ( minAt != UINT && looSd <= _cutMedLoo ) ) {
			_goiSummary[ goi ].confidence = VP::Med;
		}
	//	Low-Confidence will be a GOI_global flag and no calculations
	//	will be made down the entire GOI-column.
	//	TODO, not true anymore, want different behavior for A, versus B C F

// TODO
// need a second _cutoff for Loo-Sd
//
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
	_checkMessage.append( QString(
"Out of %1 output columns, %2 will be used for calculations (%3 High, %4 Med, %5 Low) \n" )
	.arg( _outputCols.size() ).arg( _goiSummary.size() )
	.arg( nHigh ).arg( nMed ).arg( nLow )
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
	//	PERCENT NOW STORED AS PERCENT
	_gradeA = D( app->param( "GradeA" )->value() );
	if( _gradeA < 1 ) {
		_gradeA *= 100;
	}
	_gradeB = D( app->param( "GradeB" )->value() );
	if( _gradeB < 1 ) {
		_gradeB *= 100;
	}
	_gradeC = D( app->param( "GradeC" )->value() );
	if( _gradeC < 1 ) {
		_gradeC *= 100;
	}
	return( true );
}
bool	VPStore::calc( const QString& goi, const QString& sample )
{
	bool	rv = false;
	double	cqRna;
	double	pctCalc;
	double	cqNa;
	QList<double>	cqDnaTab, cqRnaTab, pctTab, useable_cqDnaTab;

	CalcReport	calcReport;

	_calcReport.clear();

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
		//	 Henrik chooses the method :PctDna( cqNa, Mean<double>( cqDnaTab ) 
		//
		//	previous:
		//	foreach() {
		//		double	pctDna = PctDna( cqNa, cqDna );
		//		pctTab << pctDna;
		//	}
		//	pctCalc = Mean<double>( pctTab );
	}
	pctCalc = PctDna( cqNa, Mean<double>( cqDnaTab ) ) * 100;
	rv = true;
	//	PERCENT NOW STORED BY DEFAULT
	_data[ridx][cidx].setPctDNA( pctCalc );

	calcReport.cqDnaCount = cqDnaTab.size();
	calcReport.cqDna = Mean<double>( cqDnaTab );
	calcReport.pctDna = _data[ridx][cidx].pctDNA();

// TODO
// pretty sure this is what is bugging Henrik right now
// if LOWCONF does not get checked before HIGHDNA and it is likely that 
// most LOWCONF are HIGHDNA too.

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
		//	CqRna1	
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
		calcReport.cqRna1 = Kubi( cqNa, Mean<double>( useable_cqDnaTab ) );
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
			_data[ ridx ][ cidx ].setCqRNA1(
			  Kubi( cqNa, Mean<double>( useable_cqDnaTab ) ) );
		}
	}
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
			continue;
		}
		if( _goiSummary[ goi ].isFlagged() ) {
			switch( _goiSummary[ goi ]._flag ) {
				case	VP::APLUS3:
					foreach( QString sample, _outputRows ) {
						int	ridx = rowIndex( sample );
						_data[ ridx ][ cidx ].setFlag( VP::AA3 );
						_data[ ridx ][ cidx ].setCqRNA(
					 	_data[ ridx ][ cidx ].input().toDouble() );
					}
					break;
				case	VP::APLUS2:
					foreach( QString sample, _outputRows ) {
						int	ridx = rowIndex( sample );
						_data[ ridx ][ cidx ].setFlag( VP::AA2 );
						_data[ ridx ][ cidx ].setCqRNA(
					 	_data[ ridx ][ cidx ].input().toDouble() );
					}
					break;
				case	VP::APLUS:
					foreach( QString sample, _outputRows ) {
						int	ridx = rowIndex( sample );
						_data[ ridx ][ cidx ].setFlag( VP::AA );
						_data[ ridx ][ cidx ].setCqRNA(
					 	_data[ ridx ][ cidx ].input().toDouble() );
					}
					break;
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
				default:
					break;
			}
		} else {
			foreach( QString sample, _outputRows ) {
				int	ridx = rowIndex( sample );
				if( _failSamples.contains( sample ) ) {
					_data[ ridx ][ cidx ].setFlag( VP::ND );
				} else if( _astarSamples.contains( sample ) ) {
					_data[ ridx ][ cidx ].setFlag( VP::ASTAR );
				} else {
					calc( goi, sample );
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
	//HERE TODO
		grades << vpScore( ridx, cidx );
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
	goiSummary->nHIGHDNA = grades.count( VP::FLAG_HIGHDNA );
	goiSummary->nK0 = grades.count( VP::FLAG_K0 );
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

	if( index > 0 && index < _inputRows.size() ) {
		rv = _inputRows.at( index );
	}
	return( rv );
}
QString	VPStore::colString( const int& index ) const
{
	QString	rv;

	if( index > 0 && index < _inputCols.size() ) {
		rv = _inputCols.at( index );
	}
	return( rv );
}
QString	VPStore::uniqueRowLabel( const Row& row ) const
{
	return( QString( "_%1_%2" ).arg( S( row[ COLUMN_NID ] ) )
		.arg( S( row[ COLUMN_SAMPLE ] ) ) ); 
}
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

bool	VPStore::load()
{
	//
	// common code for all input formats
	//
	_inputRows.clear();
	_inputCols.clear();
	QString	buf = Ifp::load( _inputFile );
	if( buf.size() == 0 || buf == USTR ) {
		setCritical( "Empty or invalid input file" );
		return( false );
	}
	QStringList	lines = buf.split( "\n", QString::KeepEmptyParts );
	for( int i = 0; i < lines.size(); ++i ) {
		QString s = lines.at( i ).trimmed();
		if( s.size() == 0 ) {
			lines[ i ] = "";
		}
	}
	if( _inputFormat == VP::FLUID ) {
		return( parseFluidigm( lines ) );
	} else if( _inputFormat == VP::SIMPLE ) {
		return( parseSpreadsheet( lines ) );
	}
	return( false );
}
bool	VPStore::parseFluidigm( const QStringList& lines )
{
	int     lnum = 0;
        QStringList	tokens;
        QString		rlabel;
	int		ridx;
        Row     row;
        int     dataStart = UINT;
        int     qualityStart = UINT;

        for( ; lnum < lines.size() && !lines.at( lnum ).isEmpty(); ++lnum ) {
                _inputFileHeader << lines.at( lnum );
        }
        for( ; lnum < lines.size() && lines.at( lnum ).isEmpty(); ++lnum ) {
        }
        ++lnum;
        if( lines.at( lnum ).endsWith( "Ct" ) ) {
                ++lnum;
        }
        if( lnum >= lines.size() ) {
                return( false );
        }
        ++lnum;
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
                        qualityStart = lnum + 3;
                        break;
                }
                row.split( lines.at( lnum ) );
                rlabel = uniqueRowLabel( row );
                if( !_inputRows.contains( rlabel ) ) {
                        _inputRows << rlabel;
                }
        }
	if( !newData() ) {
		return( false );
	}
        for( lnum = dataStart; lnum < dataStart + _inputRows.size(); ++lnum ) {
                row.split( lines.at( lnum ) );
                rlabel = uniqueRowLabel( row );
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
        for( ; lnum < qualityStart + _inputRows.size(); ++ lnum ) {
                row.split( lines.at( lnum ) );
                rlabel = uniqueRowLabel( row );
		ridx = rowIndex( rlabel );
                for( int j = 0; j < _inputCols.size(); ++j ) {
			QString	clabel = _inputCols.at( j );
                        if( S( row[ clabel ] ) == "Fail" &&
			 getFlag( rlabel, clabel ) != VP::NOAMP ) {
				_data[ ridx ][ j ].setFlag( VP::EXPFAIL );
				_data[ ridx ][ j ].setInputFlagged();
			}
                }
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
                rlabel = QString( "_%1_%2" ).arg( lnum ).arg( S( row[ tokens.at( 0 ) ] ) );
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
                rlabel = QString( "_%1_%2" ).arg( lnum ).arg( S( row[ tokens.at( 0 ) ] ) );
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
		case	VP::CqRna1:
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
			if( role == VP::Score ) {
				t.insert( _inputCols.at( cidx ), vpScore( ridx, cidx ) );
			} else {
				t.insert( _inputCols.at( cidx ), _data[ ridx ][ cidx ].data( role ) );
			}
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
			rv = _data[ ridx ][ cidx ].flagString();
			break;
		case	VP::K0:
		case	VP::HIGHDNA:
		case	VP::NOAMP:
		case	VP::OVERLOD:
		case	VP::EXPFAIL:
		case	VP::ND:
		case	VP::ERROR:
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
	fp << "gDNA concentrations:";
	foreach( double c, _gDnaConcentrations ) {
		fp << " " << c;
	}
	fp << endl;
	fp << "LinReg all-gDNA: slope " << gDnaLR.slope() << "; Eff " << gDnaLR.e() << "; R2 " << gDnaLR.r2() << endl;
	fp << "LinReg VPA-gDNA: slope " << gDnaVpaLR.slope() << "; Eff " << gDnaVpaLR.e() << "; R2 " << gDnaVpaLR.r2() << endl;

	fp << "LOD: " << _LOD << endl;
	fp << "DeltaCq Min Count: " << _minCountSdNa << endl;
	fp << "DeltaCq SD Cutoff: " << _cutoffSdNa << endl;
	// PERCENT NOW STORED AS PERCENT
	fp << "Grade A: " << _gradeA << endl;
	fp << "Grade B: " << _gradeB << endl;
	fp << "Grade C: " << _gradeC << endl;
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
