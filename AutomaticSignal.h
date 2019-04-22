//AutomaticSignal.h

#define AUTOMATIC_SIGNAL uCondition uc;

#define WAITUNTIL( pred, before, after ) \
	if ( !pred ) { \
		while ( !uc.empty() ) uc.signal(); \
		before; \
		do { \
			uc.wait(); \
		} while ( !pred ) \
		after; \
	}
#define RETURN( expr... ) \
	while ( !uc.empty() ) uc.signal(); \
	return expr;
