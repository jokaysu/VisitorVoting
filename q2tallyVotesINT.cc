//q2tallyVotesINT.cc

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
}

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) :
	voters(voters), group(group), printer(printer),
	remain(voters), curVoters(0) {
		ClearStat();
	}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
	if (remain < group) {
		while (!uc.empty()) uc.signal();
		result = Failed;
		return Failed;
	}

	//count
	curVoters++;
	BallotStat(ballot);
	if (curVoters < group) {
		printer.print(id, Voter::Block, curVoters);
		uc.wait();
		//sleep
		if (remain < group && result == Failed) {
			return Failed;
		}
		curVoters--;
		printer.print(id, Voter::Unblock, curVoters);
	} else {
		printer.print(id, Voter::Complete);
		CalResult();
		ClearStat();
		while (!uc.empty()) uc.signal();
		curVoters--;
	}
	return result;
}

void TallyVotes::done() {
	remain--;
}