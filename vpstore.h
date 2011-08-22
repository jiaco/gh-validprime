#ifndef	VPSTORE_H
#define	VPSTORE_H	1
#include <GhCore.h>
#include <GhGui.h>
#include "vp.h"
using namespace GH;

/* SVS
	foreach point on the SVS

		get cqDna
			calculate pctDNA with cqNA and cqDNA
			if( < F )
				collect cqRNA
				but if cqRNA is K0 still skip;
				 and collect GOOD cqDNAs

		now we have a set of cqRNAs
				
		if this set is EMPTY, ie all cqDNA has F %
			flag cqRNA as Flag::HIGHDNA
		if there is 1, there is no mean nor sd
			-> CANNOT FLAG, NEVER FLAG
		if there is >= 2 get mean and sd
			if( sd > 0.25 param: sdmeancqrna )
				flag cqRNA as Flag::HIGHSD
		
*/
class	TempConc
{
public:
	TempConc( const QString& rowLabel );
static	double	SnarfConc( const QString& label );

	QString	label;
	double	value;
};
bool	TempConcSort( const TempConc& e1, const TempConc& e2 );

class	GoiSummary
{
public:
		GoiSummary();
	void	show( QTextStream& fp );
	bool	isFlagged() const;
static	void	ShowHeader( QTextStream& fp );

	LinReg	linReg;
	int	nAA3, nAA2, nAA, nASTAR,
		 nA, nB, nC, nF,
		 nHIGHDNA, nHIGHSD, nK0, nND;

	QString		goi;
	VP::GoiFlag	_flag;
	VP::Confidence	confidence;
	double		allSd, looSd;	// looSd can be ND
	QString		gDnaOutlier;	// the string of the gDna (@minAt)
	QList<int>	gDnaIndexes;	// specific for this GOI

	// A+ and highsd (lowconf) are flag states across entire GOI
	// counts are only needed for a* a b c f highdna nd
	// but an A+ goi will have a series of 0 for all other counts.

	// TODO have collection of counts of samples
	//	 with various grades (A+, A*, A, B, C, F, ND)
};
class	CalcReport
{
public:
	CalcReport();
	QString	goi, sample, mesg;
	int	cqDnaCount, cqDnaCount2, cqRnaCount;
	double	cqInput, cqDna, pctDna, cqDna2, cqRna, cqRnaSd;

static	void	ShowHeader( QTextStream& fp );
	void	show( QTextStream& fp );
};
class	VPCell
{
public:
	VPCell();

	void	setInput( const QVariant& value );
	void	setInputFlagged( const bool& tf = true );
	void	setFlag( const VP::Flag& flag );
	void	setCqNA( const double& value );
	void	setCqDNA( const double& value );
	void	setPctDNA( const double& value );
	void	setGrade( const QString& value );
	void	setCqRNA( const double& value );
	void	confirmCqNA();

	QVariant	data( const VP::DataRole& role ) const;

	QVariant	input() const;
	bool		inputFlagged() const;
	VP::Flag	flag() const;
	QString		flagString() const;
	double	cqNA() const;
	double	cqDNA() const;
	double	pctDNA() const;
	double	cqRNA() const;
	QString	grade() const;

	QVariant	_input;
	VP::Flag	_flag;
	bool		_inputFlagged;
	double		_cqNA;
	double		_cqDNA;
	double		_pctDNA;
	double		_cqRNA;
	QString		_grade;
};

	// TODO	 make sure that no Cq is reported as ZERO as it is in the init() state
	// as it is totally false and on the wrong end of the number line.

class	VPStore	:	public	Errorprone
{
public:
	bool	debugOn;

	LinReg	gDnaLR, gDnaVpaLR;

	void	outputFileHeader( QTextStream& fp  );
	void	showGoiSummarySummary( QTextStream& fp );
	void	showGoiSummaryTransposed( QTextStream& fp );

		VPStore();
		~VPStore();

	bool	preload( CliApp* app );
	bool	load();
	bool	precheck( CliApp* app );
	bool	check();
	bool	prerun( CliApp* app );
	bool	run();
	bool	preheatmap( CliApp* app );

	bool	parseFluidigm( const QStringList& lines );
	bool	parseStepone( const QStringList& lines );
	bool	parseSpreadsheet( const QStringList& lines );
	void	debugLoad( QTextStream& fp );
	void	debugCheck( QTextStream& fp );
	void	debugRun( QTextStream& fp );
	void	dump( QTextStream& fp );

	LinReg		getLinReg( const QList<int>& rows,
			 const int& cidx ) const;

	QStringList	inputRows() const;
	QStringList	outputRows() const;
	QStringList	inputCols() const;
	QStringList	outputCols() const;

	QStringList	header( const VP::DataRole& role ) const;
	QStringList	verticalHeader( const VP::DataRole& role ) const;
	QList<Row>	data( const VP::DataRole& role ) const;

	QStringList	inputHeader() const;
	QStringList	workingHeader() const;
	QStringList	inoutHeader() const;
	QStringList	outputHeader() const;

	QStringList	inputVerticalHeader() const;
	QStringList	workingVerticalHeader() const;
	QStringList	inoutVerticalHeader() const;
	QStringList	outputVerticalHeader() const;

	QList<Row>	inputData() const; 
	QList<Row>	workingData() const; 
	QList<Row>	inoutData() const; 
	QList<Row>	outputData( const VP::DataRole& role ) const; 

	QStringList	gDnaConcLabels() const;
	QList<QVariant>	gDnaConcValues() const;
	ColorMap	*colorMap() const;
	
	QString	vpScore( const int& ridx, const int& cidx ) const;

	QString	checkMessage() const;
	QString	getCalcInfo( const QString& row, const QString& column,
		 const VP::State& state = VP::Load ) const;

	bool	saveSummary( const QString& path );
	bool	saveReport( const QString& path );
private:
	// internal methods
	//
	void	deleteData();
	bool	newData();
	bool	check_0();
	bool	check_1();
	bool	check_2();
	bool	check_3();
	double	getSd( const QList<int>& rowIndexes, const int& colIndex ) const;
	double	getSdLoo( const QList<int>& rowIndexes,
		 int *minAt, const int& colIndex );
	double	getSdLoo( const QList<int>& rowIndexes, const double& minSd,
		 int *minAt, const int& colIndex );

	bool	calc( const QString& goi, const QString& sample );
	void	summarizeGoi( GoiSummary* goiSummary );


	int	rowIndex( const QString& label ) const;
	int	colIndex( const QString& label ) const;
	QString	rowString( const int& index ) const;
	QString	colString( const int& index ) const;


	QString	uniqueRowLabel( const Row& row ) const;
	void	setInput( const QString& rowname, const QString& colname,
		 const QVariant& value );

	void	setInput( const QString& rowname, const QString& colname,
		 const double& value, const VP::Flag& flag );
	VP::Flag getFlag( const QString& rowname, const QString& colname );


	// parameters from App
	//
	//	load
	//
	QString	_inputFile;
	QString	_inputFormat;
	//
	//	check
	//
	bool	_validateAssay;
	double		_LOD;
	QString		_failFlag, _noampFlag;
	QString		_vpaCol;
	int		_vpaColIndex;
	QStringList	_gDnaRows;
	QList<double>	_gDnaConcentrations;
	QList<QVariant>	_gDnaConcValues;
	QList<int>	_gDnaRowIndexes;
	QList<int>	_gDnaVpaRowIndexes;
	QStringList	_ignoreRows, _ignoreCols;
	double		_maxDeltaSd;


	int		_minSdCount;
	double		_highAllSdCutoff, _highLooSdCutoff;
	bool		_performLoo;

/*
	int		_minCountSdNa;
	double		_cutoffSdNa;
	double		_cutHighAll, _cutHighLoo,
			_cutMedAll, _cutMedLoo;
*/

	QStringList	_validSamples, _astarSamples, _failSamples;

	//
	//	run
	//
	double		_gradeA, _gradeB, _gradeC;

	// store data
	//
	QStringList	_inputRows, _inputCols;
	QStringList	_outputRows, _outputCols;
	VPCell		**_data;
	int		_dataRowSize, _dataColSize;

	QString		_checkMessage;
	QMap<QString,GoiSummary>	_goiSummary;
	QList<CalcReport>		_calcReport;

	// stuff not really used
	//
	QStringList	_inputFileHeader;

	ColorMap		*_cmap;
};
#endif	//	VPSTORE_H
