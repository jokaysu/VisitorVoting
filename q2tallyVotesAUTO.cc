//q2tallyVotesAUTO.cc

#include "q2tallyVotes.h"
#include "q2voter.h"
#include "q2print.h"

void TallyVotes::BallotStat(Ballot ballot) {
	stats.picture += ballot.picture;
	stats.statue += ballot.statue;
	stats.giftshop += ballot.picture;
}

void TallyVotes::ClearStat() {
	stats.picture = stats.statue = stats.giftshop = 0;
}

void TallyVotes::CalResult() {
	if (stats.statue > stats.picture && stats.statue > stats.giftshop) {
		result = Statue;
	} else if (stats.picture > stats.giftshop) {
		result = Picture;
	} else {
		result = GiftShop;
	}
	completed = true;
}

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
	completed(false), voters(voters), group(group), printer(printer),
	remain(voters), curVoters(0) {
		ClearStat();
	}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
	if (remain < group) {
		result = Failed;
		completed = true;
		RETURN(result);
	}

	//count
	curVoters++;
	BallotStat(ballot);
	if (curVoters < group) {
		WAITUNTIL(completed,
				  printer.print(id, Voter::Block, curVoters)
				  , );
		//sleep
		if (remain < group && result == Failed) {
			RETURN(result);
		}
		curVoters--;
		printer.print(id, Voter::Unblock, curVoters);
	} else {
		printer.print(id, Voter::Complete);
		CalResult();
		ClearStat();
		curVoters--;
	}
	if (curVoters == 0) completed = false;
	RETURN(result);
}

void TallyVotes::done() {
	remain--;
}