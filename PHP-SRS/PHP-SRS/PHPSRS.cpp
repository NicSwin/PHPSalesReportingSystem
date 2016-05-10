#include "PHPSRS.h"

#include <iostream>

#include <bprinter\table_printer.h>
#include <ostream>
#include <vector>

#define VERBOSE true

PhpSrs::PhpSrs()
{
}

PhpSrs::~PhpSrs()
{
	delete con;
}

int PhpSrs::run(std::string url, std::string user, std::string password,
	std::string database)
{
	try {
		// Check connection
		connect(url, user, password, database);		

		// Go to menu
		menu();
	} 
	catch (sql::SQLException &e) {
		std::cout << "ERROR: SQLException in " << __FILE__;
		std::cout << " (" << __func__ << ") on line " << __LINE__ << std::endl;
		std::cout << "ERROR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << ")" << std::endl;

		return EXIT_FAILURE;
	}
	catch (std::runtime_error &e) {
		std::cout << "ERROR: runtime_error in " << __FILE__;
		std::cout << " (" << __func__ << ") on line " << __LINE__ << std::endl;
		std::cout << "ERROR: " << e.what() << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

std::string PhpSrs::getInput()
{
	std::string line;
	std::getline(std::cin, line);

	// To Lower case 
	// std::transform(line.begin(), line.end(), line.begin(), ::tolower);

	return line;
}

void PhpSrs::requestEnter()
{
	std::cout << "Press enter to continue." << std::endl;
	std::cin.get();
}

void PhpSrs::connect(std::string url, std::string user, std::string password,
	std::string database)
{
	driver = get_driver_instance();

	// create a database connection using the Driver
	con = driver->connect(url, user, password);

	// select appropriate database schema
	con->setSchema(database);
}

void PhpSrs::menu()
{
	while (running)	{
		std::cout << std::endl << "PHP-SRS Ver 0.1:" << std::endl <<
			"------------------------------" << std::endl <<
			"1: View product stock." << std::endl <<
			"2: Add new product." << std::endl <<
			"3: View sale transactions." << std::endl <<
			"4: View individual sales." << std::endl <<
			"5: Record sale." << std::endl <<
			"6: View last week's sales report." << std::endl <<
			"------------------------------" << std::endl <<
			"9: Quit" << std::endl <<
			"> ";

		termninalInput = getInput();

		if (termninalInput == "1") {
			viewProducts();
		}
		else if (termninalInput == "2") {
			addProduct();
		}
		else if (termninalInput == "3") {
			viewSales();
		}
		else if (termninalInput == "4") {
			viewIndividualSales();
		}
		else if (termninalInput == "5") {
			recordSale();
		}
		else if (termninalInput == "6") {
			viewWeeklySales();
		}
		else if (termninalInput == "9") {
			running = false;
		}
	}
}

void PhpSrs::viewProducts()
{
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT * FROM products");

	retrieveDataAndPrint(res);
	requestEnter();
}

void PhpSrs::addProduct()
{
	bool continueing = true;

	while (continueing) {
		std::string productName = "";
		std::string productPrice = "";
		std::string productStock = "";

		std::cout << "New product name: > ";
		productName = getInput();
		std::cout << "New product price: > ";
		productPrice = getInput();
		std::cout << "New product initial stock: > ";
		productStock = getInput();

		std::cout << "Name: " << productName << " Price: " << productPrice <<
			" Stock: " << productStock << std::endl;
		std::cout << "Confirm? y/n: " << std::endl << "> ";

		std::string yes = getInput();
		if (yes == "y") {
			prep_stmt = con->prepareStatement(
				"INSERT INTO Products (Name, Price, Stock) VALUES (?, ?, ?)");
			prep_stmt->setString(1, productName);
			prep_stmt->setString(2, productPrice);
			prep_stmt->setString(3, productStock);
			prep_stmt->executeUpdate();

			delete prep_stmt;

			continueing = false;
		}
		else {
			continueing = false;
		}
	}

	requestEnter();
}

void PhpSrs::viewSales()
{
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT * FROM sales_record");

	retrieveDataAndPrint(res);
	requestEnter();
}

void PhpSrs::viewIndividualSales()
{
	stmt = con->createStatement();
	res = stmt->executeQuery("SELECT ids.SaleID, p.Name, ids.Quanity FROM individual_sale ids INNER JOIN products p ON ids.ProductID = p.ProductID;");

	retrieveDataAndPrint(res);
	requestEnter();
}

void PhpSrs::viewWeeklySales()
{
	stmt = con->createStatement();
	
	res = stmt->executeQuery("SELECT * FROM sales_record WHERE DateTime >= curdate() - INTERVAL DAYOFWEEK(curdate())+6 DAY AND DateTime < curdate() - INTERVAL DAYOFWEEK(curdate())-1 DAY");

	//res = stmt->executeQuery("SELECT * FROM sales_record WHERE DateTime BETWEEN date_sub(now(), INTERVAL 1 WEEK) AND now()");

	retrieveDataAndPrint(res);
	requestEnter();
}

void PhpSrs::recordSale()
{
	std::vector<std::pair<std::string, std::string>> itemList;

	bool addingItems = true;
	while (addingItems) {
		std::string productID = "";
		std::string productQuanity = "";

		std::cout << "ID of product being purchased: > ";
		productID = getInput();
		std::cout << "Quanity of this product being purchased: > ";
		productQuanity = getInput();

		itemList.emplace_back(productID, productQuanity);
		
		std::cout << "Is this the last item in this transaction? y/n : > ";

		std::string yes = getInput();
		if (yes == "y" ) {
			addingItems = false;
		}
		else if (yes == "n") {

		}
	}

	for (auto item : itemList) {
		std::cout << "Name: " << item.first << " Amount: " << item.second <<
			std::endl;
	}

	std::cout << "Transaction price: > ";
	std::string transactionPrice = getInput();

	std::cout << "Confirm? y/n: " << std::endl << "> ";
	std::string yes = getInput();
	if (yes == "y") {
		// 1: Add sales record
		prep_stmt = con->prepareStatement(
			"INSERT INTO sales_record (SalePrice) VALUES (?);");
		prep_stmt->setString(1, transactionPrice);
		prep_stmt->executeQuery();	
		delete prep_stmt;

		// 2: Get ID of transaction (have to query due to being auto generated)
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT LAST_INSERT_ID();");
		res->next();
		std::string transactionID = res->getString(1);

		// 3: Add record of each item being sold
		for (auto item : itemList) {
			prep_stmt = con->prepareStatement(
				"INSERT INTO individual_sale (SaleID, ProductID, Quanity) VALUES (?, ?, ?)");
			prep_stmt->setString(1, transactionID);
			prep_stmt->setString(2, item.first); // First = ID
			prep_stmt->setString(3, item.second); // Second = quanity
			prep_stmt->executeUpdate();
		}

		// 4: Reduce stock of each item my quanity of being sold
		for (auto item : itemList) {
			prep_stmt = con->prepareStatement(
				"UPDATE products SET stock = stock - ? WHERE ProductID = ?;");
			prep_stmt->setString(1, item.second); // Second = quanity
			prep_stmt->setString(2, item.first); // First = ID
			prep_stmt->executeUpdate();
		}
	}
	else {
		
	}
}

std::string PhpSrs::retrieveDataAndPrint(sql::ResultSet * rs)
{
	// Comma seperated values table
	std::ostringstream csvTable;	
	// Fancy table
	bprinter::TablePrinter tp(&std::cout);

	// Need meta data to get column names
	sql::ResultSetMetaData *res_meta;
	res_meta = rs->getMetaData();

	unsigned int columnCount = res_meta->getColumnCount();

	// Row of column names
	for (int i = 1; i != columnCount+1; ++i) {
		std::string columnName = res_meta->getColumnName(i);
		csvTable << columnName;
		
		// Do not add comma after last column
		if (i != columnCount) {
			csvTable << ",";
		}

		// Get fancy table column names + length
		if (VERBOSE) {
			tp.AddColumn(columnName, 
				std::max(columnName.size(), res_meta->getColumnDisplaySize(i)));
		}
	}
	csvTable << std::endl;

	if (VERBOSE) {
		tp.PrintHeader();
	}

	// Row contents
	while (rs->next()) {
		for (int i = 1; i != columnCount + 1; ++i) {		
			std::string value = rs->getString(i);
			csvTable << value;

			// Do not add comma after last column
			if (i != columnCount) {
				csvTable << ",";
			}

			if (VERBOSE) {
				tp << value;
			}
		}
		
		csvTable << std::endl;
	}

	if (VERBOSE) {
		tp.PrintFooter();
	}

	return csvTable.str();
}
