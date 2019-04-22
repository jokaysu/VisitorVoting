//q2tallyVotes.h

#ifndef Q2TALLYVOTES_H
#define Q2TALLYVOTES_H

_Monitor Printer;

#if defined( EXT )                           // external scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
#elif defined( INT )                         // internal scheduling monitor solution
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition uc;
#elif defined( INTB )                        // internal scheduling monitor solution with barging
// includes for this kind of vote-tallier
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition bench;                        // only one condition variable (you may change the variable name)
    void wait();                             // barging version of wait
    void signalAll();                        // unblock all waiting tasks
#elif defined( AUTO )                        // automatic-signal monitor solution
// includes for this kind of vote-tallier
#include "AutomaticSignal.h"
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    AUTOMATIC_SIGNAL;
    bool completed;
#elif defined( TASK )                        // internal/external scheduling task solution
_Task TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition uc;
    unsigned int vid;
  protected:
  	void main();
#else
    #error unsupported voter type
#endif
    // common declarations
  public:
    struct Ballot { unsigned int picture, statue, giftshop; } vballot;
    enum Tour { Picture = 'p', Statue = 's', GiftShop = 'g', Failed = 'f' };
  private:
    unsigned int voters;
    unsigned int group;
    Printer & printer;

    unsigned int remain;
    unsigned int curVoters;
    Ballot stats;
    Tour result;

    void BallotStat(Ballot ballot);
    void ClearStat();
    void CalResult();
  public:                            // common interface
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    Tour vote( unsigned int id, Ballot ballot );
    void done();
};

#endif