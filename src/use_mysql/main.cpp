#include <mysql_connection.h>
#include <mysql_driver.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

int main(int argc, char* argv[]) {
	::sql::mysql::MySQL_Driver* driver = nullptr;
	sql::Connection* con = nullptr;

	driver = ::sql::mysql::get_driver_instance();

	try {
		con = driver->connect("tcp://127.0.0.1:3306", "root", "1234");
		std::cout << con->isValid() << std::endl;
		delete con;
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;
		std::cout << "MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << std::endl;
	}

	return 0;
}
