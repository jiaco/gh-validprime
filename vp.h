#ifndef	VP_H
#define	VP_H	1
#include <GhCore.h>
#include "ghLinReg.h"
using namespace GH;

const	QString	COLUMN_NID = "NID";
const	QString	COLUMN_SAMPLE = "SAMPLE";

namespace	VP
{
	const QString	VERSION = "v.0.9.5";

	const double	CHECK0_PCTFAIL_CRIT = 0.9;
	const double	CHECK0_PCTFAIL_WARN = 0.5;
	const QString	CRIT_MESG_ERRORCELLS =
	 "Critical: %1%% of input cells not properly parsed";
	const QString	CHECK0_CRIT_MESG = "Critical: %1%% of input cells are Over-LOD";
	const QString	CHECK0_WARN_MESG = "Warning: %1%% of input cells are Over-LOD";

	const QString	CHECK1_CRIT_MESG = "Critical: all gDna-VPA data is flagged, cannot proceed";
	const QString	CHECK1_WARN_MESG = "Warning: %1 out of %2 gDna points removed due to flags in VPA column";

	const double	CHECK2_PCTFAIL_CRIT = 0.9;
	const double	CHECK2_PCTFAIL_WARN = 0.5;

	const int	CHECK3_MINFORSD = 3;

	const double	FLAG = 999;

	const QString	FLAG_ERROR = "Error";
	const QString	FLAG_CALC = "NoFlag";
	const QString	FLAG_NADA = "NoFlag";
	const QString	FLAG_AA3 = "A+++";
	const QString	FLAG_AA2 = "A++";
	const QString	FLAG_AA = "A+";
	const QString	FLAG_ASTAR = "A*";
	const QString	FLAG_HIGHDNA = "HIGHDNA";
	const QString	FLAG_HIGHSD = "HIGHSD";
	const QString	FLAG_NOAMP = "NOAMP";
	const QString	FLAG_OVERLOD = "OVERLOD";
	const QString	FLAG_EXPFAIL = "EXPFAIL";
	const QString	FLAG_ND = "ND";

	// not actual flags but the visible grade string
	const QString	FLAG_A = "A";
	const QString	FLAG_B = "B";
	const QString	FLAG_C = "C";
	const QString	FLAG_F = "F";

	enum	GoiFlag	{
		CALC, APLUS3, APLUS2, APLUS //, LOWCONF
	};
	enum	Flag	{
		NADA, AA3, AA2, AA, ASTAR, HIGHSD,
		 //HIGHDNA, HIGHSD,
		 NOAMP, OVERLOD, EXPFAIL, ND, ERROR
	};
	enum	State	{
		Load, Check, PostCheck, Run, Save, Done
	};
		// the InOut DataRole is for AllInOne
		//
	enum	DataRole {
		Input, Working, InOut, 
		CqRna, CqDna, PctDna, Score, CalcNa
	};
	enum	Confidence {
		High, Low, NotUsed
		//High, Med, Low, NotUsed
	};
	const QString	FLUID = "Biomark";
	const QString	SIMPLE = "Spreadsheet";
	const QString	STEPONE = "Abi-StepOne";
	const QString	CUSTOM = "Custom";

inline	QVariant	VD( const double& value )
{
	if( value == VP::FLAG ) {
		return( V( "NA" ) );
	} else {
		return( V( value ) );
	}
}
inline	QString	GoiFlagString( const GoiFlag& flag )
{
	QString rv = FLAG_ERROR;

	switch( flag ) {
		case	CALC:
			rv = FLAG_CALC;
			break;
		case	APLUS3:
			rv = FLAG_AA3;
			break;
		case	APLUS2:
			rv = FLAG_AA2;
			break;
		case	APLUS:
			rv = FLAG_AA;
			break;
/*
		case	LOWCONF:
			rv = FLAG_HIGHSD;
			break;
*/
	}
	return( rv );
}
inline	QString	FlagString( const Flag& flag )
{
	QString	rv = FLAG_ERROR;

	switch( flag ) {
		case	NADA:
			rv = FLAG_NADA;
			break;
		case	AA3:
			rv = FLAG_AA3;
			break;
		case	AA2:
			rv = FLAG_AA2;
			break;
		case	AA:
			rv = FLAG_AA;
			break;
		case	ASTAR:
			rv = FLAG_ASTAR;
			break;
/*
		case	HIGHDNA:
			rv = FLAG_HIGHDNA;
			break;
*/
		case	HIGHSD:
			rv = FLAG_HIGHSD;
			break;
		case	NOAMP:
			rv = FLAG_NOAMP;
			break;
		case	OVERLOD:
			rv = FLAG_OVERLOD;
			break;
		case	EXPFAIL:
			rv = FLAG_EXPFAIL;
			break;
		case	ND:
			rv = FLAG_ND;
			break;
		default:	// ERROR
			break;
	}
	return( rv );
}
inline	QString	ConfidenceString( const Confidence& confidence )
{
	switch( confidence ) {
		case	High:
			return( "High" );
			break;
/*
		case	Med:
			return( "Med" );
			break;
*/
		case	Low:
			return( "Low" );
			break;
		default:
			return( "-" );
			break;
	}
	return( QString() );
}
inline	QString	StateString( const State& state )
{
	QString	rv;

	switch( state ) {
		case	Load:
			rv = "Load";
			break;
		case	Check:
			rv = "Check";
			break;
		case	PostCheck:
			rv = "PostCheck";
			break;
		case	Run:
			rv = "Run";
			break;
		case	Save:
			rv = "Save";
			break;
		case	Done:
			rv = "Done";
			break;
	}
	return( rv );
}
inline	QString	RoleString( const DataRole& role )
{
	QString	rv;

	switch( role ) {
		case	Input:
			rv = "Input";
			break;
		case	Working:
			rv = "Working";
			break;
		case	InOut:
			rv = "InOut";
			break;
		case	CqRna:
			rv = "CqRNA";
			break;
		case	CqDna:
			rv = "CqDNA";
			break;
		case	PctDna:
			rv = "%-DNA";
			break;
		case	Score:
			rv = "VPScore";
			break;
		case	CalcNa:
			rv = "Calc-CqNA";
			break;
	}
	return( rv );
}
};	// VP namespace

inline	double	CqNa( const double& cqRna, const double& cqDna )
{
	double rv;
	if( cqRna == VP::FLAG || cqDna == VP::FLAG ) {
		return( VP::FLAG );
	}
	rv = -1 * log2( pow( 2, ( -1 * cqRna ) ) + pow( 2, ( -1 * cqDna ) ) );

	return( rv );
}

inline	double	PctDna( const double& cqNa, const double& cqDna )
{
	double	rv;

	// 2^(-cqDNA) / 2^(-cqNA)

	rv = pow( 2, -1 * cqDna ) / pow( 2, -1 * cqNa );
	return( rv );
}
inline	double	Kubi( const double& cqNa, const double& cqDna )
{
	// returns cqRna
	// if cqNA > cqDna then k0 even
	//
	double 	rv;

	// -log2( 2^-cqNA - 2^-cqDNA )
	// -log2( 2^-( cqNA - cqDNA ) )

	if( cqNa >= cqDna ) {
		rv = 0;
	} else {
		rv =
		 -1 * ( log2( pow( 2, -1 * cqNa ) - pow( 2, -1 * cqDna ) ) );
	}
	return( rv );
}
inline bool	IsFluidigmNoamp( const QVariant& value )
{
	QString s = value.toString();
	if( s.isEmpty() || s == "999" ) {
		return( true );
	}
	return( false );
}
#endif	//	VP_H
