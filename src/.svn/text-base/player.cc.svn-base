#include "player.hh"
#include <stdlib.h>

Player::Player(string n) 
{
	name = n;
	db = new Database("Database.sqlite");
}

Player::~Player() { }

int Player::hands_played(string filter) 
{
	int result = 0;
	
	if(filter.empty())
	{	
		string hands_query = "SELECT DISTINCT game_id FROM a WHERE player= \"" + name + "\";";
		vector<string> gameIds = db->column((char*)hands_query.c_str());

		for(vector<string>::iterator iter = gameIds.begin(); iter != gameIds.end(); iter++)
		{
			string game_id = *iter;
			
			string actionsQuery = "SELECT count(action) FROM a WHERE player= \"" + name + "\" AND game_id=" + game_id +";";
				if(db->count((char*)actionsQuery.c_str()) > 1)
					result++;		
		}
	}

	if(!filter.empty())
	{	
		string hands_query = "SELECT DISTINCT game_id FROM a WHERE player= \"" + name + "\" AND " + filter +";";
		vector<string> gameIds = db->column((char*)hands_query.c_str());

		for(vector<string>::iterator iter = gameIds.begin(); iter != gameIds.end(); iter++)
		{
			string game_id = *iter;

			string actionsQuery = "SELECT count(action) FROM a WHERE player= \"" + name + "\" AND game_id=" + game_id +";";
				if(db->count((char*)actionsQuery.c_str()) > 1)
					result++;
		}
	}
	
	handsPlayed = result;
	
	return result;	
}

vector<double> Player::money_won(string filter) 
{
	vector<double> res;
	res.push_back(0);
	string resultQuery;

		if(filter.empty())
		resultQuery = "SELECT game_id, action, player, money FROM a;";
	else
		resultQuery = "SELECT game_id, action, player, money FROM a WHERE " + filter + ";";
			
	vector<vector<string> > result = db->query((char*)resultQuery.c_str());

	string temp_id = "start";
	double money = 0;
	for (vector<vector<string> >::iterator it = result.begin();
		 it < result.end(); ++it) {
		vector<string> row = *it;
        if (row.at(2) != name)
            continue;
        if (row.at(1) != "none") {
            money += atof(row.at(3).c_str());
            if (row.at(0) != temp_id && temp_id != "start") {
                res.push_back(money);
            }

            temp_id = row.at(0);
        }
        else
            continue;


    }
    res.push_back(money);
	
	return res;
}

double Player::vpip(string filter) 
{
	string vpip_query;
	
	if(filter.empty())
		vpip_query = "SELECT CAST(count(DISTINCT game_id) AS FLOAT) FROM a WHERE player=\"" + name + "\" AND phase=\"preflop\" AND (action=\"call\" OR action=\"raise\" OR action=\"all-in\");";
	else
		vpip_query = "SELECT CAST(count(DISTINCT game_id) AS FLOAT) FROM a WHERE player=\"" + name + "\" AND phase=\"preflop\" AND (action=\"call\" OR action=\"raise\" OR action=\"all-in\") AND " + filter + ";";
	
	double result = db->count((char*) vpip_query.c_str()) / handsPlayed * 100;

	return result;
}

double Player::pfr(string filter) 
{
	string pfr_query;
	
	if(filter.empty())
		pfr_query = "SELECT CAST(count(DISTINCT game_id) AS FLOAT) FROM a WHERE player=\"" + name + "\" AND phase=\"preflop\" AND (action=\"raise\" OR action=\"all-in\");";
	else
		pfr_query = "SELECT CAST(count(DISTINCT game_id) AS FLOAT) FROM a WHERE player=\"" + name + "\" AND phase=\"preflop\" AND (action=\"raise\" OR action=\"all-in\") AND " + filter + ";";
	
	double result = db->count((char*) pfr_query.c_str()) / handsPlayed * 100;

	return result;
}

double Player::total_actions(string filter)
{
	string total_actions_query;
	
	if(filter.empty())
		total_actions_query = "SELECT count (*) FROM a WHERE player=\"" + name + "\" AND (action=\"check\" OR action=\"bet\" OR action=\"raise\" OR action=\"call\" OR action=\"fold\" OR action=\"all-in\") AND phase!=\"init\";";
	else
		total_actions_query = "SELECT count (*) FROM a WHERE player=\"" + name + "\" AND (action=\"check\" OR action=\"bet\" OR action=\"raise\" OR action=\"call\" OR action=\"fold\" OR action=\"all-in\") AND phase!=\"init\" AND " + filter + ";";
	
	double result = db->count((char*) total_actions_query.c_str());

	return result;
}

double Player::raise(string filter) 
{
	string raise_query;
	
	if(filter.empty())
		raise_query = "SELECT count(*) FROM a WHERE player= \"" + name + "\" AND action=\"raise\" AND phase!=\"init\";";	
	else
		raise_query = "SELECT count(*) FROM a WHERE player= \"" + name + "\" AND action=\"raise\" AND phase!=\"init\" AND " + filter + ";";
	
	double result = db->count((char*) raise_query.c_str()) / total_actions("") * 100;

	return result;
}

double Player::bet(string filter) 
{
	string bet_query;
	
	if(filter.empty())
		bet_query = "SELECT count(*) FROM a WHERE player= \"" + name + "\" AND (action=\"bet\" OR (action=\"all-in\" AND phase=\"river\")) AND phase!=\"init\";";
	else
		bet_query = "SELECT count(*) FROM a WHERE player= \"" + name + "\" AND (action=\"bet\" OR action=\"all-in\") AND phase!=\"init\" AND " + filter +";";
	
	double result = db->count((char*) bet_query.c_str()) / total_actions("") * 100;

	return result;
}

double Player::call(string filter) 
{
	string call_query;
	
	if(filter.empty())
		call_query = "SELECT count(*) FROM a WHERE player= \"" + name + "\" AND action=\"call\" AND phase!=\"init\";";
	else
		call_query = "SELECT count(*) FROM a WHERE player= \"" + name + "\" AND action=\"call\" AND phase!=\"init\" AND " + filter + ";";
	
	double result = db->count((char*) call_query.c_str()) / total_actions("") * 100;

	return result;
}

double Player::agression_factor() 
{
	return (raise("") + bet("")) / call("");
}

double Player::wtsd(string filter) 
{
	string totalWtsdQuery;
	
	/* Went to showdown, % = Total Times Went to Showdown / Total Times Saw The Flop * 100 */

	/* Total Times Went to Showdown = Times was on river - times folded on river + "all-in" times excluding all-in on river */
	if(filter.empty())
		totalWtsdQuery = 
			"SELECT(SELECT count(DISTINCT game_id) FROM a WHERE phase=\"river\" AND player=\"" 
								+ name 
								+ "\") - (SELECT count(DISTINCT game_id) FROM a WHERE phase=\"river\" AND player=\"" 
								+ name
								+ "\" and action=\"fold\") - (SELECT count(DISTINCT game_id) FROM a WHERE phase=\"river\" AND player=\"" 
								+ name 
								+ "\" and action=\"not show\" AND cards=\"none\");";
	else
		totalWtsdQuery =
			"SELECT(SELECT count(DISTINCT game_id) FROM a WHERE phase=\"river\" AND player=\""
								+ name
								+ "\" AND " + filter + ") - (SELECT count(DISTINCT game_id) FROM a WHERE phase=\"river\" AND player=\""
								+ name
								+ "\" and action=\"fold\" AND " + filter +") - (SELECT count(DISTINCT game_id) FROM a WHERE phase=\"river\" AND player=\""
								+ name
								+ "\" and action=\"not show\" AND cards=\"none\" AND " + filter + ");";

	totalWtsd = db->count((char*) totalWtsdQuery.c_str());

	/* Total times saw the flop */
	string sawFlopQuery;

	if(filter.empty())
		sawFlopQuery = "SELECT count(DISTINCT game_id) FROM a WHERE player= \"" + name + "\" AND (phase=\"flop\" OR phase=\"turn\" OR phase=\"river\");";
	else
		sawFlopQuery = "SELECT count(DISTINCT game_id) FROM a WHERE player= \"" + name + "\" AND (phase=\"flop\" OR phase=\"turn\" OR phase=\"river\") AND " + filter +";";
	
	double sawFlop = db->count((char*) sawFlopQuery.c_str());

	if(sawFlop == 0)
		return 0;
	
	return totalWtsd / sawFlop * 100;
}

double Player::wsd(string filter) 
{
	string wsdQuery;
	
	/* Won at showdown, % = Total Times Won Money at Showdown / Total Times Went to Showdown  * 100 )*/
	if(filter.empty())
		wsdQuery = "SELECT count(*) FROM a WHERE player=\"" + name + "\" AND phase=\"river\" AND action=\"winsd\";";
	else
		wsdQuery = "SELECT count(*) FROM a WHERE player=\"" + name + "\" AND phase=\"river\" AND action=\"winsd\" AND " + filter +";";

	return db->count((char*) wsdQuery.c_str()) / totalWtsd * 100;
}

double Player::threeBet(string filter) 
{
	int threeBetOpp = 0;
	string handsPlayedQuery, threeBetQuery;

	/* Opportunities to 3bet */
	if(filter.empty())
		handsPlayedQuery = "SELECT DISTINCT game_id FROM a WHERE player=\"" + name + "\";";
	else
		handsPlayedQuery = "SELECT DISTINCT game_id FROM a WHERE player=\"" + name + "\" AND " + filter +";";
	
	vector<string> handsPlayed = db->column((char*) handsPlayedQuery.c_str());

	for (vector<string>::iterator it = handsPlayed.begin(); it != handsPlayed.end(); it++) 
	{
		string game_id = (*it);

		string othersRaiseQuery = "SELECT * FROM a WHERE game_id=" + game_id + " AND phase=\"preflop\" AND action=\"raise\";";
		vector<vector<string> > othersRaise = db->query((char*) othersRaiseQuery.c_str());

		for (vector<vector<string> >::iterator it = othersRaise.begin(); it != othersRaise.end(); it++) {
			vector<string> row = *it;

			if (row.at(6) != name)
				break;

			else {
				threeBetOpp++;
				break;
			}
		}
	}

	/* Real 3bet */
	if(filter.empty())
		threeBetQuery =
			"SELECT * FROM a WHERE phase=\"preflop\" AND action=\"raise\" group by game_id, player, action, money order by game_id, money;";
	else
		threeBetQuery =
			"SELECT * FROM a WHERE phase=\"preflop\" AND action=\"raise\" AND " + filter + " group by game_id, player, action, money order by game_id, money;";
	
	vector<vector<string> > threeBet = db->query((char *) threeBetQuery.c_str());

	string last_game_id;
	int cnt = 0, threeBets = 0;

	for (vector<vector<string> >::iterator it = threeBet.begin(); it != threeBet.end(); it++) {
		vector<string> row = *it;

		if (row.at(1) == last_game_id) {
			cnt++;

			if (cnt == 1)
				if (name == row.at(6))
					threeBets++;
		} else {
			last_game_id = row.at(1);
			cnt = 0;
		}
	}

	if (threeBetOpp)
		return (double) threeBets / (double) threeBetOpp * 100;

	return 0;
}

double Player::cbet(string filter) 
{
	int betFlop = 0, oppToContBetFlop = 0;

	string raisePreflopQuery;
	
	if(filter.empty())
		raisePreflopQuery = "SELECT DISTINCT game_id FROM a WHERE player=\"" + name + "\" AND action=\"raise\" AND phase=\"preflop\";";
	else
		raisePreflopQuery = "SELECT DISTINCT game_id FROM a WHERE player=\"" + name + "\" AND action=\"raise\" AND phase=\"preflop\" AND " + filter +";";
	
	vector<string> raisePreflop = db->column((char*) raisePreflopQuery.c_str());

	/* Opportunities to continuation bet flop */
	for (vector<string>::iterator it = raisePreflop.begin(); it != raisePreflop.end(); it++) 
	{
		string game_id = (*it);

		string betFlopOppQuery = "SELECT count(DISTINCT game_id) FROM a WHERE game_id=" + game_id + " AND player=\"" + name + "\" AND phase=\"flop\";";
		oppToContBetFlop += db->count((char*) betFlopOppQuery.c_str());

		string othersBetFlopQuery = "SELECT * FROM a WHERE game_id=" + game_id + " AND phase=\"flop\";";
		vector<vector<string> > othersBetFlop = db->query((char*) othersBetFlopQuery.c_str());

		if (!othersBetFlop.empty()) 
		{
			vector<vector<string> >::iterator it = othersBetFlop.begin();
			vector<string> row = *it;

			if (row.at(3) == "bet" && row.at(6) != name)
				oppToContBetFlop--;
		}
	}

	/* Real continuation bet flop */
	for (vector<string>::iterator it = raisePreflop.begin(); it != raisePreflop.end(); it++) 
	{
		string game_id = (*it);

		string actionsPreflopQuery = "SELECT action FROM a WHERE game_id=" + game_id + " AND player=\"" + name + "\" AND phase=\"preflop\";";
		vector<string> actionsPreflop = db->column((char*) actionsPreflopQuery.c_str());

		if (actionsPreflop.back() == "call") 
		{
			it = raisePreflop.erase(it);
			it--;
		}

	}

	for (vector<string>::iterator it = raisePreflop.begin(); it != raisePreflop.end(); it++) 
	{
		string game_id = (*it);

		string betFlopQuery = "SELECT count(DISTINCT game_id) FROM a WHERE game_id=" + game_id + " AND player=\"" + name + "\" AND phase=\"flop\" AND action=\"bet\";";
		betFlop += db->count((char*) betFlopQuery.c_str());
	}

	return (double) betFlop / (double) oppToContBetFlop * 100;
}

vector<vector<string> > Player::cards()
{
	vector<vector<string> > cards;
	vector<vector<string> > result;
	ostringstream ostr;
	int cnt = 0;
	double percentage;
	
	string cardsQuery = "SELECT count(DISTINCT game_id), cards FROM a WHERE player=\"" + name + "\" AND cards!=\"none\" group by cards;";
	cards = db->query((char*)cardsQuery.c_str());
	
	for(vector<vector<string> >::iterator it = cards.begin(); it < cards.end(); ++it)
	{
		vector<string> row = *it;
		
		string gamesIdQuery = "SELECT DISTINCT game_id FROM a WHERE cards=\"" + row.at(1) + "\" AND player=\"" + name + "\";";
		vector<string> gamesId = db->column((char*)gamesIdQuery.c_str());
		
		for(vector<string>::iterator iter = gamesId.begin(); iter != gamesId.end(); iter++)
		{
			string game_id = *iter;
			string winQuery = "SELECT count(*) FROM a WHERE game_id=" + game_id +" AND player=\"" + name + "\" AND (action=\"win\" or action=\"winsd\");";
			
			if(db->count((char*)winQuery.c_str()))
				cnt++;
		}
		
		percentage = cnt / atof(row.at(0).c_str()) * 100;
		ostr << percentage;		
		row.push_back(ostr.str());
		result.push_back(row);								
		ostr.str("");
		cnt = 0;
	}
	
	return result;
}

int Player::blind(int type)
{
	string blindQuery;
	
	if(type == 0)
		blindQuery = "SELECT count(DISTINCT game_id) FROM A WHERE player=\"" + name + "\" and position=\"small blind\";";	
	if(type == 1)
		blindQuery = "SELECT count(DISTINCT game_id) FROM A WHERE player=\"" + name + "\" and position=\"big blind\";"; 
	
	return db->count((char*)blindQuery.c_str());
}

double Player::vpipB(int blind)
{	
	double vpipCnt = 0.0;
	string blindQuery;
	vector<string> blinds;

	if(blind == 0)
	{
		string blindQuery = "SELECT DISTINCT game_id FROM A WHERE player=\"" + name + "\" and position=\"small blind\";";
		blinds = db->column((char*)blindQuery.c_str());
			
		for(vector<string>::iterator iter = blinds.begin(); iter != blinds.end(); iter++)
		{
			string filter = "game_id=" + *iter + "";
			if(vpip(filter) != 0)
				vpipCnt++;
		}

	}
	
	if(blind == 1)
	{
		string blindQuery = "SELECT DISTINCT game_id FROM A WHERE player=\"" + name + "\" and position=\"big blind\";";
		blinds = db->column((char*)blindQuery.c_str());

		for(vector<string>::iterator iter = blinds.begin(); iter != blinds.end(); iter++)
		{
			string filter = "game_id=" + *iter + "";
			if(vpip(filter) != 0)
				vpipCnt++;
		}
	}
	
	return vpipCnt / blinds.size() * 100;
}

double Player::pfrB(int blind)
{
	double pfrCnt = 0.0;
	string blindQuery;
	vector<string> blinds;
	
	if(blind == 0)
	{
		blindQuery = "SELECT DISTINCT game_id FROM A WHERE player=\"" + name + "\" and position=\"small blind\";";
		blinds = db->column((char*)blindQuery.c_str());
		
		for(vector<string>::iterator iter = blinds.begin(); iter != blinds.end(); iter++)
		{
			string filter = "game_id=" + *iter + "";
			if(pfr(filter) != 0)
				pfrCnt++;
		}
	}
	
	if(blind == 1)
	{
		blindQuery = "SELECT DISTINCT game_id FROM A WHERE player=\"" + name + "\" and position=\"big blind\";";
		blinds = db->column((char*)blindQuery.c_str());
	
		for(vector<string>::iterator iter = blinds.begin(); iter != blinds.end(); iter++)
		{
			string filter = "game_id=" + *iter + "";
			if(pfr(filter) != 0)
				pfrCnt++;
		}
	}
	
	return pfrCnt / blinds.size() * 100;
}

double Player::wsdB(int blind)
{
	double wtsdCnt = 0.0, wsdCnt = 0.0;

	if(blind == 1)
	{
		string bigBlindQuery = "SELECT DISTINCT game_id FROM A WHERE player=\"" + name + "\" and position=\"big blind\";";
		vector<string> bigBlind = db->column((char*)bigBlindQuery.c_str());

		for(vector<string>::iterator iter = bigBlind.begin(); iter != bigBlind.end(); iter++)
		{
			string filter = "game_id=" + *iter + "";
	
			string wsdQuery = "SELECT count(*) FROM a WHERE player=\"" + name + "\" AND phase=\"river\" AND action=\"winsd\" AND " + filter +";";
			if(db->count((char*) wsdQuery.c_str()))
				wsdCnt++;
		
			if(wtsd(filter) != 0)
				wtsdCnt++;
		}
	}
	
	if(blind == 0)
	{
		string smallBlindQuery = "SELECT DISTINCT game_id FROM A WHERE player=\"" + name + "\" and position=\"small blind\";";
		vector<string> smallBlind = db->column((char*)smallBlindQuery.c_str());
		
		for(vector<string>::iterator iter = smallBlind.begin(); iter != smallBlind.end(); iter++)
		{
			string filter = "game_id=" + *iter + "";
			
			string wsdQuery = "SELECT count(*) FROM a WHERE player=\"" + name + "\" AND phase=\"river\" AND action=\"winsd\" AND " + filter +";";
			if(db->count((char*) wsdQuery.c_str()))
				wsdCnt++;
			
			if(wtsd(filter) != 0)
				wtsdCnt++;
		}
	}
	
	return wsdCnt / wtsdCnt * 100;
}
