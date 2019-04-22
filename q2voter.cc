//q2voter.cc

#include "q2voter.h"
#include "q2print.h"

Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ) :
	id(id), nvotes(nvotes), voteTallier(voteTallier), printer(printer) {}

void Voter::main() {
	//cout << "seed = " << mprng.get_seed() << endl;
	for (unsigned int i = 0; i < nvotes; ++i) {
		yield( mprng( 19 ) );

		//start
		printer.print(id, Voter::Start);

		yield( mprng( 4 ) );

		//vote
		TallyVotes::Ballot myVote = cast();
		printer.print(id, Voter::Vote, myVote);
		TallyVotes::Tour myTour = voteTallier.vote(id, myVote);
		if (myTour == 'f') {
			printer.print(id, Voter::Failed);
			break;
		}

		yield( mprng( 4 ) );

		//finish
		printer.print(id, Voter::Finished, myTour);
	}
	//printer.print(id, Voter::Barging);
	voteTallier.done();
	//printer.print(id, Voter::Barging);
	//printer.print(id, Voter::Barging);
}