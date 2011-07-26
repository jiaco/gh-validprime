#include "ghLinReg.h"

namespace	GH
{

	LinReg::LinReg()
{
	init();
}

QString	LinReg::fullDebug() const
{
	QString	rv;

	rv = QString( "LinReg n = %1, slope = %2, r2 = %3, eff = %4\n" )
                .arg( n() ).arg( slope() )
                .arg( r2() ).arg( e() );

	for( int i = 0; i < _xpts.size(); ++i ) {
                rv.append( QString( "%1 : %2 %3\n" )
                 .arg( i ).arg( _xpts.at( i ) )
                 .arg( _ypts.at( i ) ) );
        }
	return( rv );

}
void	LinReg::setIndexes( const QList<int>& indexes )
{
	_indexes = indexes;
}
QList<int>	LinReg::indexes() const
{
	return( _indexes );
}
void	LinReg::init()
{
	sum_x = sum_y = sum_xy = sum_x2 = sum_y2 = 0;
	sumx_2 = sumy_2 = 0;
	_slope = 0;
	yIntercept = 0;
	r = 0;
	_n = 0;
	_xpts.clear();
	_ypts.clear();
	flag = "NA";
}
double	LinReg::slope() const
{
	return( _slope );
}
int	LinReg::n() const
{
	return( _n );
}
double	LinReg::r2() const
{
	return( r );
}
double	LinReg::e() const
{
	double rv = 0;
	if( _slope != 0 ) {
		rv = ( 100 * ( ( pow( 10, -1 / _slope ) ) - 1 ) );
	}
	return( rv );
}
double	LinReg::yForX( const double& x ) const
{
	double y = ( _slope * x ) + yIntercept;
	return( y );
}
double	LinReg::xForY( const double& y ) const
{
	double x = ( y - yIntercept ) / _slope;
	return( x );
}
void	LinReg::compute( const QList<double>& xpts, const QList<double>& ypts )
{
	init();
	if( xpts.size() != ypts.size() ) {
		return;
	}
// DANGER MyApp::FLAG_FLAG is not accessible in this function
// hardwired the 655 number to select valid points
	for( int i = 0; i < xpts.size(); ++i ) {
		if( ypts.at( i ) < 655 ) {
			_xpts << xpts.at( i );
			_ypts << ypts.at( i );
		}
	}
	_n = _xpts.size();

	if( _n < 2 ) {
		return;
	}

	sum_x = Sum<double>( _xpts );
	sum_y = Sum<double>( _ypts );
	sum_xy = 0;
	for( int i = 0; i < _n; ++i ) {
		sum_xy += ( _xpts.at( i ) * _ypts.at( i ) );
	}
	sumx_2 = sum_x * sum_x;
	sumy_2 = sum_y * sum_y;
	sum_x2 = SumSquares<double>( _xpts );
	sum_y2 = SumSquares<double>( _ypts );

	_slope = ( ( _n * sum_xy ) - ( sum_x * sum_y ) ) /
		( ( _n * sum_x2 ) - sumx_2 );
	yIntercept = ( sum_y - _slope * sum_x ) / _n;
	double dr = sqrt( ( ( _n * sum_x2 ) - sumx_2 ) * ( ( _n * sum_y2 ) - sumy_2 ) );
	r = ( ( _n * sum_xy ) - sum_x * sum_y ) / dr;
	r *= r;
}

}	//	GH namespace
