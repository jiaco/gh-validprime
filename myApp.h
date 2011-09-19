#ifndef	MYAPP_H
#define	MYAPP_H	1
#include <GhCore.h>
#include <GhGui.h>
#include "vpstore.h"

namespace	GH
{


class	MyApp	:	public	CliApp
{
	Q_OBJECT

public:
	MyApp( int argc, char** argv );

	QStringList	inputFormatChoice() const;

	QStringList	header( const VP::DataRole& role ) const;
	QStringList	verticalHeader( const VP::DataRole& role ) const;
	QList<Row>	data( const VP::DataRole& role ) const;

	QStringList	inputRows() const;
	QStringList	outputRows() const;
	QStringList	inputHeader() const;
	QStringList	inputCols() const;
	QStringList	outputCols() const;
	QList<Row>	inputData() const;

	QString		checkMessage() const;

	bool		gDnaReady();
	QStringList	gDnaConcLabels() const;
	QList<QVariant>	gDnaConcValues() const;
	void		gDnaFromUser( const QVariant& values );

	QString	getCheckCriticalMesg();
	QString	getCheckWarningMesg();

	void	prepareHeatMap();
	ColorMap*	getColorMap() const;

	QString	getCalcInfo( const QString& row, const QString& column,
		 const VP::State& state = VP::Load ) const;

public slots:
	void	doit();
	bool	load();
	bool	loadDemo();
	bool	check();
	bool	run();
	bool	save();
	void	reload();
	void	recheck();	
	void	autoNameOutput();
signals:
	void	setState( const VP::State& state );
	void	postload();
	void	postcheck( const bool& passed, const QString& dialogMesg );
	void	postrun();
private:
	bool	saveData( const QString& path, VP::DataRole& role );
	void	writeData( QTextStream& fp, VP::DataRole& role,
		 const bool& includeHeader = true );

	Ofp	cout, cerr;
	VPStore	store;
	QStringList	_inputFormatChoice;
};

}	//	GH namespace
#endif	//	MYAPP_H
