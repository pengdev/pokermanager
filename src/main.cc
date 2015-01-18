#include <iostream>
#include "sqltest.hh"

using namespace std;

int main(void)
{

	Database *db;

	db = new Database("Database.sqlite");

	db->query("CREATE TABLE a (game_date INTEGER, game_id INTEGER, action TEXT, phase TEXT, card_1 TEXT, card_2 TEXT, money REAL, position TEXT);");
	db->query("INSERT INTO a VALUES(3, 2, 'BET', 'PREFLOP', 'Ah', 'Ad', 3.10, 'Blind');");
	//db->query("INSERT INTO a VALUES(5, 4);");

	vector<vector<string> > result = db->query("SELECT game_date, game_id, action FROM a;");

	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{	
		vector<string> row = *it;
		
		cout << "Values: (Game date: " << row.at(0) << ", Game ID =" << row.at(1) << ", Action: " << row.at(2) << " )" << endl;
	}

	db->close(); 
}
	

