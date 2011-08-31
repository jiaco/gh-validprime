#include "ghDilSeries.h"

using namespace	GH;

DilutionPoint::DilutionPoint( const QString& label )
{
	_label = label;
	_value = DilutionPoint::SnarfConc( label );
}
DilutionPoint::DilutionPoint( const QString& label, const double& value )
{
	_label = label;
	_value = value;
}
QString	DilutionPoint::label() const
{
	return( _label );
}
double	DilutionPoint::value() const
{
	return( _value );
}
double	DilutionPoint::SnarfConc( const QString& label )
{
	bool	ok;
	double	rv = -1;	// cannot have negative conc
	QString	s, t;

	//s = label;
	s = VP::DeUniqueLabel( label );
	t = "";
	// in ValidPrime, mulitple rows can have the same label
	// so a numeric id is prefixed to the row label
	//	like : "_75_Label"
	// this needs to be ignored in order to snarf the conc
	//
/*
	if( s.startsWith( '_' ) ) {
		s = s.mid( s.indexOf( '_', 1 ) );
	}
*/
	foreach( QChar ch, s ) {
		if( ch.isDigit() ) {
			t.append( ch );
		} else if( ch == '-' || ch == '.' ) {
			t.append( ch );
		} else if( ch == ',' ) {
			t.append( '.' );
		}
	}
	rv = t.toDouble( &ok );
	if( ok ) {
		return( rv );
	} else {
		return( -1 );
	}
}

bool	DilSeriesSort( const DilutionPoint& e1, const DilutionPoint& e2 )
{
	return( e1.value() < e2.value() );
}
