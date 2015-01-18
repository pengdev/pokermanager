#ifndef PLAYER_HH
#define PLAYER_HH

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "sqltest.hh"

class Player {
public:
	Player(string name);
	~Player();
	
	int hands_played(string filter);
	vector<double> money_won(string filter);
	double vpip(string filter);
	double pfr(string filter);
	double total_actions(string filter);
	double raise(string filter);
	double bet(string filter);
	double call(string filter);
	double agression_factor();
	double wtsd(string filter);
	double wsd(string filter);
	double threeBet(string filter);
	double cbet(string filter);
	vector<vector<string> > cards();
	int blind(int);
	double vpipB(int);
	double pfrB(int);
	double wsdB(int blind);
	string getName() {
		return name;
	}

private:
	string name;
	int totalWtsd;
	int handsPlayed;
	Database *db;
};

#endif
