#include "PHPSRS.h"

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "root"
#define PASSWORD "bird"
#define DATABASE "php-srs"

#define NUMOFFSET 100
#define COLNAME 200

int main(int argc, const char *argv[]) 
{
	/* initiate url, user, password and database variables */
	const std::string url(argc >= 2 ? argv[1] : DBHOST);
	const std::string user(argc >= 3 ? argv[2] : USER);
	const std::string password(argc >= 4 ? argv[3] : PASSWORD);
	const std::string database(argc >= 5 ? argv[4] : DATABASE);

	PhpSrs phpSrs;
	int returnVal = phpSrs.run(url, user, password, database);
	
	//Error?
	if (returnVal == EXIT_FAILURE) {
		std::cout << "Press enter to exit." << std::endl;
		std::cin.get();
	}

	return returnVal;
}