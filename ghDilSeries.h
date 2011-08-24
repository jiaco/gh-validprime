#ifndef	GH_DILSERIES_H
#define	GH_DILSERIES_H
#include "GhCore.h"

namespace	GH
{

class	DilutionPoint
{
public:
	DilutionPoint( const QString& label );
	DilutionPoint( const QString& label, const double& value );

static double	SnarfConc( const QString& label );
	QString	label() const;
	double	value() const;

private:
	QString	_label;
	double	_value;
};

typedef	QList<DilutionPoint>	DilSeries;

bool	DilSeriesSort( const DilutionPoint& e1, const DilutionPoint& e2 );

}	//	GH namespace
#endif	//	GH_DILSERIES_H
