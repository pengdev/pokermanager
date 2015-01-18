#ifndef SQLTEST_HH
#define SQLTEST_HH

#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

class Database {
public:
	
    	Database(const char* filename);
    	~Database();
    	bool open(const char* filename);
		vector<vector<string> > query(char* query);
		double count(char* query);
		vector<string> column(char* query);
    	void close();
	
private:
    	sqlite3 *database;
};

#endif

