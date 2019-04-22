//q2tallyVotesTASK.cc

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
	vid = id;
	vballot = ballot;
	uc.wait(vid);
	//sleep
	return result;
}

void TallyVotes::done() {
	remain--;
}

void TallyVotes::main() {
	while (true) {
		_Accept ( ~TallyVotes ) {
			break;
		} or _When ( curVoters < group )  _Accept ( vote ) {
			if (remain < group) {
				result = Failed;
				while (!uc.empty()) uc.signalBlock();
				continue;
			}
			curVoters++;
			BallotStat(vballot);
			printer.print(vid, Voter::Block, curVoters);
			if (curVoters == group) {
				CalResult();
				ClearStat();
				printer.print(vid, Voter::Complete);
				while (!uc.empty()) {
					curVoters--;
					unsigned int curid = uc.front();
					printer.print(curid, Voter::Unblock, curVoters);
					uc.signalBlock();
				}
			}
		} or _Accept ( done ) {
			if (remain < group) {
				result = Failed;
				while (!uc.empty()) uc.signalBlock();
			}
		}
	}
}