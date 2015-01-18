/*
 * fileIO.cc
 *
 *  Created on: Nov 17, 2012
 *      Author: leo
 */

#include "fileIO.hh"
//#define SHOW_ALL_INFO // for debugging, show detailed information

int loadhistory(string file) {
	//int main() { // for debugging
	string line;
	struct tm* counttime = new tm();
	ifstream myfile(file.c_str());
	/* temp strings for common use in a game*/
	string game_id;
	string game_max_buyin;
	string game_date;
	string table_id;
	string button_pos;
	string phase;
	// if database exists, delete it
	if (access("Database.sqlite", 0) == 0)
		if (system("rm Database.sqlite"))
			cout << "WARNING: previous database is not removed successfully"
					<< endl;

	Database *db;
	db = new Database("Database.sqlite");
	string command =
			"CREATE TABLE a (game_date DATETIME, game_id INTEGER, table_id TEXT, action TEXT, phase TEXT, cards TEXT, player TEXT, money REAL, position TEXT);";
	db->query((char*) command.c_str());

	if (myfile.is_open()) {
		while (myfile.good()) {
			getline(myfile, line);

			/*
			 * Parse for GAME related
			 */
			/* parse game id*/
			if (line.substr(0, line.find(' ')) == "#Game") {
				game_id.clear();
				game_id = line.substr(line.find(':') + 2, 11);
#ifdef SHOW_ALL_INFO
				cout << "game id:" << line.substr(line.find(':') + 2, 11)
				<< endl;
#endif
				continue;
			}
			/* parse maximum buy-in and date*/
			if (line.substr(0, 1) == "$") {
#ifdef SHOW_ALL_INFO
				cout << "maximum buy-in:" << line.substr(1, line.find(' ') - 1)
				<< " date:"
				<< line.substr(line.find('-') + 2,
						line.size() - line.find('-') - 2) << endl;
#endif
				game_max_buyin = line.substr(1, line.find(' ') - 1);
				string date = line.substr(line.find('-') + 2,
						line.size() - line.find('-') - 2 - 5 - 4);
				string zone = line.substr(line.size() - 8, 3);
				string year = line.substr(line.size() - 4, 4);

				strptime((char *) date.c_str(), "%A, %B %d, %H:%M:%S",
						counttime);
				// Note: time zone have not parsed, to be solved later
				strptime((char *) year.c_str(), "%Y", counttime);
				// store the date in SQlite readable format
				char buf[20];
				strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", counttime);
				game_date = buf;
				continue;
			}
			/* parse table id*/
			if (line.substr(0, line.find(' ')) == "Table") {
				if (line.find("(No DP)") != string::npos) {
#ifdef SHOW_ALL_INFO
					cout << "table id:"
					<< line.substr(sizeof("Table Table "),
							line.find(" (") - sizeof("Table Table "))
					<< endl;
#endif
					table_id.clear();
					table_id = line.substr(sizeof("Table Table "),
							line.find(" (") - sizeof("Table Table "));
					continue;
				} else if (line.find("(Real Money)") != string::npos) {
#ifdef SHOW_ALL_INFO
					cout << "table name:"
					<< line.substr(line.find(' ') + 1,
							line.find(" (") - line.find(' ') - 1)
					<< endl;
#endif
					table_id.clear();
					table_id = line.substr(line.find(' ') + 1,
							line.find(" (") - line.find(' ') - 1);
					continue;
				}
			}
			/* parse button & players of seats*/
			if (line.substr(0, line.find(' ')) == "Seat") {
				if (line.find("button") != string::npos) {
#ifdef SHOW_ALL_INFO
					cout << "button player No.:"
					<< line.substr(line.find(' ') + 1,
							line.find(" is") - line.find(' ') - 1)
					<< endl;
#endif
					button_pos = line.substr(line.find(' ') + 1,
							line.find(" is") - line.find(' ') - 1);

					continue;
				} else {
#ifdef SHOW_ALL_INFO
					cout << "seat:"
					<< line.substr(line.find(' '),
							line.find(':') - line.find(' '))
					<< " player:"
					<< line.substr(line.find(':') + 1,
							line.find(" (") - line.find(':') - 1)
					<< " initial money:"
					<< line.substr(line.find('$') + 1,
							line.find(" USD") - line.find('$') - 1)
					<< endl;
#endif
					string player = line.substr(line.find(':') + 2,
							line.find(" (") - line.find(':') - 2);
					string money = line.substr(line.find('$') + 1,
							line.find(" USD") - line.find('$') - 1);
					string seat = line.substr(line.find(' ') + 1,
							line.find(':') - line.find(' ') - 1);

					if (seat == button_pos) {
						string command = "INSERT INTO a VALUES('" + game_date
								+ "', " + game_id + ", '" + table_id
								+ "', 'none', '" + phase + "', 'none', '"
								+ player + "', " + money + ", 'dealer');";

						db->query((char *) command.c_str());
					}

					else {
						/* send sql query*/
						string command = "INSERT INTO a VALUES('" + game_date
								+ "', " + game_id + ", '" + table_id
								+ "', 'none', '" + phase + "', 'none', '"
								+ player + "', " + money + ", 'none');";
						db->query((char *) command.c_str());
					}
					continue;
				}
			}
			/* parse number of players in this game*/
			if (line.substr(0, line.find(' ')) == "Total") {
#ifdef SHOW_ALL_INFO
				cout << "number of players:"

				<< line.substr(line.find(':') + 2,
						line.find('/') - line.find(':') - 2)
				<< " max num:"
				<< line.substr(line.find('/') + 1,
						line.size() - line.find('/') - 1) << endl;
#endif
				continue;
			}
			/*
			 * Parse for USER SPECIFIC information
			 */
			/* parse dealt information*/
			if (line.substr(0, line.find(' ')) == "Dealt") {
#ifdef SHOW_ALL_INFO
				cout << "dealt info:"

				<< line.substr(line.find("to ") + 3,
						line.find(" [") - line.find("to ") - 3)
				<< " card:"
				<< line.substr(line.find("[  ") + 3,
						line.find(" ]") - line.find("[  ") - 3) << endl;
#endif
				string dealt_player = line.substr(line.find("to ") + 3,
						line.find(" [") - line.find("to ") - 3);
				string card = line.substr(line.find("[  ") + 3,
						line.find(" ]") - line.find("[  ") - 3);
				/* convert hand cards to more readable format*/
				int num1 = 0;
				int num2 = 0;
				string number1 = card.substr(0, 1);
				if (number1 == "A")
					num1 = 14;
				else if (number1 == "K")
					num1 = 13;
				else if (number1 == "Q")
					num1 = 12;
				else if (number1 == "J")
					num1 = 11;
				else if (number1 == "T")
					num1 = 10;
				else
					num1 = atoi(number1.c_str());

				string number2 = card.substr(3, 1);
				if (number2 == "A")
					num2 = 14;
				else if (number2 == "K")
					num2 = 13;
				else if (number2 == "Q")
					num2 = 12;
				else if (number2 == "J")
					num2 = 11;
				else if (number2 == "T")
					num2 = 10;
				else
					num2 = atoi(number2.c_str());
				string suit1 = card.substr(1, 1);
				string suit2 = card.substr(4, 1);
				string add;
				if (suit1 == suit2)
					add = "s"; // s for same suits
				else
					add = "o"; // o for opposite suits
				string card_debug;
				if (num1 > num2)
					card_debug = number1 + number2 + add;
				else
					card_debug = number2 + number1 + add;
				/* send sql query*/
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'dealt', '" + phase
						+ "', '" + card_debug + "', '" + dealt_player
						+ "', 0, 'none');";
				db->query((char *) command.c_str());
				continue;
			}

			/*
			 * Parse for GAME ACTION
			 */
			/* parse who raise*/
			if (line.substr(line.find(' ') + 1, sizeof("raises") - 1)
					== "raises") {
#ifdef SHOW_ALL_INFO
				cout << "who raises:" << line.substr(0, line.find(" "))

				<< " how much:"
				<< line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string money = line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1);
				/* send sql query*/
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'raise', '" + phase
						+ "', 'none', '" + player + "', -" + money
						+ ", 'none');";
				db->query((char *) command.c_str());
				continue;
			}
			/* parse who folds*/
			if (line.substr(line.find(' ') + 1, sizeof("folds") - 1)
					== "folds") {
#ifdef SHOW_ALL_INFO
				cout << "who folds:" << line.substr(0, line.find(" ")) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				/* send sql query*/
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'fold', '" + phase
						+ "', 'none', '" + player + "', 0, 'none');";
				db->query((char *) command.c_str());
				continue;
			}
			/* parse who calls*/
			if (line.substr(line.find(' ') + 1, sizeof("calls") - 1)
					== "calls") {
#ifdef SHOW_ALL_INFO
				cout << "who calls:" << line.substr(0, line.find(" "))
				<< " how much:"
				<< line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string money = line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1);
				/* send sql query*/
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'call', '" + phase
						+ "', 'none', '" + player + "', -" + money
						+ ", 'none');";
				db->query((char *) command.c_str());
				continue;
			}
			/* parse who posts*/
			if (line.substr(line.find(' ') + 1, sizeof("posts") - 1)
					== "posts") {
				if (line.find("small blind") != string::npos) {
#ifdef SHOW_ALL_INFO
					cout << "who posts small blind:"

					<< line.substr(0, line.find(" ")) << "how much:"
					<< line.substr(line.find('$') + 1,
							line.find(" USD") - line.find('$') - 1)
					<< endl;
#endif
					string player = line.substr(0, line.find(" "));
					string money = line.substr(line.find('$') + 1,
							line.find(" USD") - line.find('$') - 1);
					/* send sql query*/
					string command = "INSERT INTO a VALUES('" + game_date
							+ "', " + game_id + ", '" + table_id
							+ "', 'call', '" + phase + "', 'none', '" + player
							+ "', -" + money + ", 'small blind');";
					db->query((char *) command.c_str());
					continue;
				} else if (line.find("big blind [") != string::npos) {
#ifdef SHOW_ALL_INFO
					cout << "who posts big blind:"
					<< line.substr(0, line.find(" ")) << "how much:"
					<< line.substr(line.find('$') + 1,
							line.find(" USD") - line.find('$') - 1)
					<< endl;
#endif
					string player = line.substr(0, line.find(" "));
					string money = line.substr(line.find('$') + 1,
							line.find(" USD") - line.find('$') - 1);
					/* send sql query*/
					string command = "INSERT INTO a VALUES('" + game_date
							+ "', " + game_id + ", '" + table_id
							+ "', 'call', '" + phase + "', 'none', '" + player
							+ "', -" + money + ", 'big blind');";
					db->query((char *) command.c_str());
					continue;
				}
				//parse big blind and dead, strange.. but anyway insert to the table
				else if (line.find("big blind + dead") != string::npos) {
#ifdef SHOW_ALL_INFO
					cout << "who posts big blind:"
					<< line.substr(0, line.find(" ")) << "how much:"
					<< line.substr(line.find('$') + 1,
							line.find(" USD") - line.find('$') - 1)
					<< endl;
#endif
					string player = line.substr(0, line.find(" "));
					string money = line.substr(line.find('[') + 1,
							line.find(" $") - line.find('[') - 1);
					string money_debug = money.substr(0, money.find(',')) + '.'
							+ money.substr(money.find(',') + 1, money.length());
					/* send sql query*/
					string command = "INSERT INTO a VALUES('" + game_date
							+ "', " + game_id + ", '" + table_id
							+ "', 'call', '" + phase + "', 'none', '" + player
							+ "', -" + money_debug + ", 'big blind dead');";
					db->query((char *) command.c_str());
					continue;
				}
			}
			/* parse who bets*/
			if (line.substr(line.find(' ') + 1, sizeof("bets") - 1) == "bets") {
#ifdef SHOW_ALL_INFO
				cout << "who bets:" << line.substr(0, line.find(" "))

				<< " how much:"
				<< line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string money = line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1);
				/* send sql query*/
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'bet', '" + phase
						+ "', 'none', '" + player + "', -" + money
						+ ", 'none');";
				db->query((char *) command.c_str());
				continue;
			}
			/* parse who checks*/
			if (line.substr(line.find(' ') + 1, sizeof("checks") - 1)
					== "checks") {
#ifdef SHOW_ALL_INFO
				cout << "who checks:" << line.substr(0, line.find(" ")) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'check', '" + phase
						+ "', 'none', '" + player + "', 0, 'none');";

				db->query((char *) command.c_str());
				continue;
			}

			/* parse who win*/
			if (line.substr(line.find(' ') + 1, sizeof("wins") - 1) == "wins") {
#ifdef SHOW_ALL_INFO
				cout << "who wins:" << line.substr(0, line.find(" "))

				<< " how much:"
				<< line.substr(line.find('$') + 1, line.find(" USD") - line.find('$') - 1) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string money = line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1);
				/* send sql query*/
				if (line.find("from the main pot") != string::npos) {
#ifdef SHOW_ALL_INFO
					cout << "how to win:"

					<< line.substr(line.find("with") + 5,
							line.size() - line.find("with") - 6)
					<< endl;
#endif
					string command = "INSERT INTO a VALUES('" + game_date
							+ "', " + game_id + ", '" + table_id
							+ "', 'winsd', '" + phase + "', 'none', '" + player
							+ "', " + money + ", 'none');";
					db->query((char *) command.c_str());

				}

				else {
					string command = "INSERT INTO a VALUES('" + game_date
							+ "', " + game_id + ", '" + table_id + "', 'win', '"
							+ phase + "', 'none', '" + player + "', " + money
							+ ", 'none');";
					db->query((char *) command.c_str());
				}
				continue;
			}
			/* parse who does not show cards*/
			if (line.substr(line.find(' ') + 1,
					sizeof("does not show cards") - 1)
					== "does not show cards") {
#ifdef SHOW_ALL_INFO
				cout << "who does not show cards:"
				<< line.substr(0, line.find(" ")) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'not show', '"
						+ phase + "', 'none', '" + player + "', 0, 'none');";
				db->query((char *) command.c_str());

				continue;
			}
			/* parse who shows what cards*/
			if (line.substr(line.find(' ') + 1, sizeof("shows") - 1)
					== "shows") {
#ifdef SHOW_ALL_INFO
				cout << "who shows cards:" << line.substr(0, line.find(" "))
				<< " card:"
				<< line.substr(line.find("[ ") + 2,
						line.find(" ]") - line.find("[ ") - 2)
				<< " what combination:"
				<< line.substr(line.find(']') + 1,
						line.size() - line.find(']') - 2) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string card = line.substr(line.find("[ ") + 2,
						line.find(" ]") - line.find("[ ") - 2);
				int num1 = 0;
				int num2 = 0;
				string number1 = card.substr(0, 1);
				if (number1 == "A")
					num1 = 14;
				else if (number1 == "K")
					num1 = 13;
				else if (number1 == "Q")
					num1 = 12;
				else if (number1 == "J")
					num1 = 11;
				else if (number1 == "T")
					num1 = 10;
				else
					num1 = atoi(number1.c_str());

				string number2 = card.substr(4, 1);
				if (number2 == "A")
					num2 = 14;
				else if (number2 == "K")
					num2 = 13;
				else if (number2 == "Q")
					num2 = 12;
				else if (number2 == "J")
					num2 = 11;
				else if (number2 == "T")
					num2 = 10;
				else
					num2 = atoi(number2.c_str());
				string suit1 = card.substr(1, 1);
				string suit2 = card.substr(5, 1);
				string add;
				if (suit1 == suit2)
					add = "s"; // s for same suits
				else
					add = "o"; // o for opposite suits
				string card_debug;
				// order the number from big to small
				if (num1 > num2)
					card_debug = number1 + number2 + add;
				else
					card_debug = number2 + number1 + add;
				/* send sql query*/
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'show', '" + phase
						+ "', '" + card_debug + "', '" + player
						+ "', 0, 'none');";
				db->query((char *) command.c_str());
				continue;
			}
			/* parse all-in*/
			if (line.find("all-In") != string::npos) {
#ifdef SHOW_ALL_INFO
				cout << "who is all-in:" << line.substr(0, line.find(" "))
				<< " how much:"
				<< line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string money = line.substr(line.find('$') + 1,
						line.find(" USD") - line.find('$') - 1);
				/* send sql query*/
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'all-in', '" + phase
						+ "', 'none', '" + player + "', -" + money
						+ ", 'none');";
				db->query((char *) command.c_str());
				continue;
			}

			/* parse who does not show */
			// Note: do not have to be the Player_6, strange...  but finally agreed to add this to database
			if (line.substr(line.find(' ') + 1, sizeof("doesn't show") - 1)
					== "doesn't show") {
#ifdef SHOW_ALL_INFO
				cout << "who does not shows cards:"
				<< line.substr(0, line.find(" ")) << " card:"
				<< line.substr(line.find("[ ") + 2,
						line.find(" ]") - line.find("[ ") - 2) << endl;
#endif
				string player = line.substr(0, line.find(" "));
				string card = line.substr(line.find("[ ") + 2,
						line.find(" ]") - line.find("[ ") - 2);
				int num1 = 0;
				int num2 = 0;
				string number1 = card.substr(0, 1);
				if (number1 == "A")
					num1 = 14;
				else if (number1 == "K")
					num1 = 13;
				else if (number1 == "Q")
					num1 = 12;
				else if (number1 == "J")
					num1 = 11;
				else if (number1 == "T")
					num1 = 10;
				else
					num1 = atoi(number1.c_str());

				string number2 = card.substr(4, 1);
				if (number2 == "A")
					num2 = 14;
				else if (number2 == "K")
					num2 = 13;
				else if (number2 == "Q")
					num2 = 12;
				else if (number2 == "J")
					num2 = 11;
				else if (number2 == "T")
					num2 = 10;
				else
					num2 = atoi(number2.c_str());
				string suit1 = card.substr(1, 1);
				string suit2 = card.substr(5, 1);
				string add;
				if (suit1 == suit2)
					add = "s";
				else
					add = "o";
				//
				string card_debug;
				if (num1 > num2)
					card_debug = number1 + number2 + add;
				else
					card_debug = number2 + number1 + add;
				/* send sql query*/
				string command = "INSERT INTO a VALUES('" + game_date + "', "
						+ game_id + ", '" + table_id + "', 'not show', '"
						+ phase + "', '" + card_debug + "', '" + player
						+ "', 0, 'none');";
				db->query((char *) command.c_str());
				continue;
			}

			/*
			 * Parse for ROUND info
			 */
			/* parse down cards :: not useful*/
			if (line.find("** Dealing down cards **") != string::npos) {
#ifdef SHOW_ALL_INFO
				cout << "dealing down cards" << endl;
#endif
				phase = "preflop";
				continue;
			}
			/* parse flop*/
			if (line.find("** Dealing Flop **") != string::npos) {
#ifdef SHOW_ALL_INFO
				cout << "flop:"
				<< line.substr(line.find("[ ") + 2,
						line.find(" ]") - line.find("[ ") - 2) << endl;
#endif
				phase = "flop";
				continue;
			}
			/* parse turn*/
			if (line.find("** Dealing Turn **") != string::npos) {
#ifdef SHOW_ALL_INFO
				cout << "turn:"
				<< line.substr(line.find("[ ") + 2,
						line.find(" ]") - line.find("[ ") - 2) << endl;
#endif
				phase = "turn";
				continue;
			}
			/* parse river*/
			if (line.find("** Dealing River **") != string::npos) {
#ifdef SHOW_ALL_INFO
				cout << "river:"
				<< line.substr(line.find("[ ") + 2,
						line.find(" ]") - line.find("[ ") - 2) << endl;
#endif
				phase = "river";
				continue;
			}

			/*
			 * parse NOT USEFUL information to avoid warning
			 */
			/* parse who has joined the table*/
			if (line.substr(line.find(' ') + 1,
					sizeof("has joined the table") - 1)
					== "has joined the table") {
#ifdef SHOW_ALL_INFO
				cout << "who has joined the table:"
				<< line.substr(0, line.find(" ")) << endl;
#endif
				continue;
			}
			/* parse and skip hand history game id*/
			if (line.find("***** Hand History for Game") != string::npos) {
				//do nothing, to avoid warning
				phase = "init";
				continue;
			}
			/* skip game start information*/
			if (line.find("Game #") != string::npos) {
				//do nothing, to avoid warning
				continue;
			}
			/* skip time bank notification*/
			if (line.find("time bank") != string::npos) {
				//do nothing, to avoid warning
				continue;
			}
			/* skip game player left information*/
			if (line.find("has left the table.") != string::npos) {
				//do nothing, to avoid warning
				continue;
			}
			/* skip game player sitting out information*/
			if (line.find("is sitting out") != string::npos) {
				//do nothing, to avoid warning
				continue;
			}
			/* skip return*/
			if (line.size() < 3) {
				//do nothing, to avoid warning
				continue;
			}
			/* skip did not response*/
			if (line.find("did not respond in time") != string::npos) {
				//do nothing, to avoid warning
				continue;
			}
			/* skip disconnection warning*/
			if (line.find("onnect") != string::npos) {
				//do nothing, to avoid warning
				continue;
			}
			/* skip table closed warning*/
			if (line.find("Table Closed") != string::npos) {
				//do nothing, to avoid warning
				continue;
			}
			/* skip not avaliable warning*/
			if (line.find("the email address you have registered with us")
					!= string::npos) {
				//do nothing, to avoid warning
				continue;
			}
			cout << "NOT PARSED WARNING: " << line << endl;
		}

		/* print the whole database table when parsing finished*/
		/*
		string command =
				"SELECT game_date, game_id, table_id, action, phase, cards, player, money, position FROM a;";
		vector<vector<string> > result = db->query((char *) command.c_str());

		for (vector<vector<string> >::iterator it = result.begin();
				it < result.end(); ++it) {
			vector<string> row = *it;
			cout << row.at(0) << " " << row.at(1) << " " << row.at(2) << " "
					<< row.at(3) << " " << row.at(4) << " " << row.at(5) << " "
					<< row.at(6) << " " << row.at(7) << " " << row.at(8)
					<< endl;
		}
		*/
		/* notify the database loaded*/
		cout << "database loaded successfully" << endl;
		myfile.close();

	}

	else
		cout << "Unable to open file";

	return 0;
}

