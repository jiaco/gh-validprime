
vp1=function( na, dna )
{
	cat( "NA  = ", na, "\n" );
	cat( "DNA = ", dna, "\n" );

	rqna = 2^-na;
	rqdna = 2^-dna;

	rqrna = rqna - rqdna;
	cqrna = -1 * log2( rqrna );

	cat( "RQ  NA = ", rqna, "\n" );
	cat( "RQ DNA = ", rqdna, "\n" );
	cat( "RQ RNA = ", rqrna, "\n" );
	cat( "CQ RNA = ", cqrna, "\n" );
	

	cqrna;
}
vp2=function( na, dna )
{
	cat( "NA  = ", na, "\n" );
	cat( "DNA = ", dna, "\n" );


	dcq = na - dna;
	rq_dcq = 2 ^ -dcq;
	cqrna = -log2( rq_dcq );

	cat( "DELTA CQ = ", dcq, "\n" );
	cat( "RQ DCQ = ", rq_dcq, "\n" );
	cat( "CQ RNA = ", cqrna, "\n" );
	

	cqrna;
}
