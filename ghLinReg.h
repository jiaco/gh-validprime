#ifndef	GH_LINREG_H
#define	GH_LINREG_H	1
#include <math.h>
#include <QtCore>
#include "ghMath.h"

namespace	GH
{
class	LinReg
{
public:
	LinReg();
	void	init();
	void	compute( const QList<double>& xpts, const QList<double>& ypts );
	double	yForX( const double& x ) const;
	double	xForY( const double& y ) const;

	double	slope() const;
	int	n() const;
	double	r2() const;
	double	e() const;

	void		setIndexes( const QList<int>& indexes );
	QList<int>	indexes() const;

	QList<double>	_xpts, _ypts;
	double	sum_x, sum_y, sum_xy, sum_x2, sumx_2, sum_y2, sumy_2;
	double	_slope, yIntercept, r;
	double	_n;

	QString	fullDebug() const;
	QList<int>	_indexes;
	QString	flag;
};
}	//	GH namespace
#endif	//	GH_LINREG_H
