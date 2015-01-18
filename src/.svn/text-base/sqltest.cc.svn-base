#include "sqltest.hh"
#include <iostream>

Database::Database(const char* filename)
{
    database = NULL;
    open(filename);
}

Database::~Database()
{
}

bool Database::open(const char* filename)
{
    if(sqlite3_open(filename, &database) == SQLITE_OK){
		sqlite3_exec(database, "PRAGMA synchronous = OFF;", 0, 0, 0);
		return true;
	}

    return false;
}

double Database::count(char* query)
{
	sqlite3_stmt *statement;
	double value = 0;

	if(sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;
		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
				value = sqlite3_column_double(statement, 0);
			else
				break;
		}

		sqlite3_finalize(statement);
	}

	string error = sqlite3_errmsg(database);
 
	if(error != "not an error") cout << query << " " << error << endl;

	return value;
}

vector<string> Database::column(char* query)
{
	sqlite3_stmt *statement;
	vector<string> column;

	if(sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK)
	{
		int result = 0;

		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
				column.push_back((char*)sqlite3_column_text(statement, 0));
			else
				break;
		}

		sqlite3_finalize(statement);

	}

	string error = sqlite3_errmsg(database);

	if(error != "not an error") cout << query << " " << error << endl;

	return column;
}

vector<vector<string> > Database::query(char* query)
{
	sqlite3_stmt *statement;
    	vector<vector<string> > results;

    	if(sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK)
	{
        	int cols = sqlite3_column_count(statement);
        	int result = 0;

        	while(true)
       	 	{
            		result = sqlite3_step(statement);

            		if(result == SQLITE_ROW)
            		{
                		vector<string> values;

                		for(int col = 0; col < cols; col++)
                	       		values.push_back((char*)sqlite3_column_text(statement, col));
                
                		results.push_back(values);
            		}

            		else
                		break;
        	}

        	sqlite3_finalize(statement);
    	}

    	string error = sqlite3_errmsg(database);

    	if(error != "not an error") cout << query << " " << error << endl;

	return results;
}

void Database::close()
{
	sqlite3_close(database);
}
