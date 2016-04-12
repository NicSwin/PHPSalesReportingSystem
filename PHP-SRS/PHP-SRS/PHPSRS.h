#pragma once

#include <string>

/* MySQL Connector/C++ specific headers */
#include <mysql_connector/cppconn/driver.h>
#include <mysql_connector/cppconn/connection.h>
#include <mysql_connector/cppconn/statement.h>
#include <mysql_connector/cppconn/prepared_statement.h>
#include <mysql_connector/cppconn/resultset.h>
#include <mysql_connector/cppconn/metadata.h>
#include <mysql_connector/cppconn/resultset_metadata.h>
#include <mysql_connector/cppconn/exception.h>
#include <mysql_connector/cppconn/warning.h>

class PhpSrs
{
public:
	PhpSrs();
	~PhpSrs();

	int run(std::string url, std::string user, std::string password, 
		std::string database);

private:
	bool running = true;

	sql::Driver *driver = nullptr;
	sql::Connection *con = nullptr;
	sql::Statement *stmt = nullptr;
	sql::ResultSet *res = nullptr;
	sql::PreparedStatement *prep_stmt = nullptr;

	std::string termninalInput = "";

	std::string getInput();
	void requestEnter();

	void connect(std::string url, std::string user, std::string password,
		std::string database);
	void menu();

	void viewProducts();
	void addProduct();
	void viewSales();
	void viewWeeklySales();
	void viewIndividualSales();
	void recordSale();

	std::string retrieveDataAndPrint(sql::ResultSet * rs);


};

