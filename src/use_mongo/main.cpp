#include <cstdint>
#include <iostream>
#include <vector>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>

//using bsoncxx::builder::stream::close_array;
//using bsoncxx::builder::stream::close_document;
//using bsoncxx::builder::stream::document;
//using bsoncxx::builder::stream::finalize;
//using bsoncxx::builder::stream::open_array;
//using bsoncxx::builder::stream::open_document;

const std::string DATABASE_HOST = "localhost";
const std::string DATABASE_PORT = "27017";
const std::string DATABASE_NAME = "mydb";
const std::string COLLECTION_NAME = "test";

int main(int argc, char* argv[]) {
	// Make a Connection
	mongocxx::instance instance{}; // This should be done only once.
	mongocxx::uri uri("mongodb://" + DATABASE_HOST + ":" + DATABASE_PORT);
	mongocxx::client client(uri);

	// Access a Database
	mongocxx::database db = client[DATABASE_NAME];

	// Access a Collection
	mongocxx::collection coll = db[COLLECTION_NAME];

	// Create a Document
	//	{
	//		"name" : "MongoDB",
	//		"type" : "database",
	//		"count" : 1,
	//		"versions" : ["v3.2", "v3.0", "v2.6"],
	//		"info" : {
	//			"x" : 203,
	//			"y" : 102
	//		}
	//	}
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "name" << "MongoDB"
		<< "type" << "database"
		<< "count" << 1
		<< "versions"
		<< bsoncxx::builder::stream::open_array
		<< "v3.2" << "v3.0" << "v2.6"
		<< bsoncxx::builder::stream::close_array
		<< "info" << bsoncxx::builder::stream::open_document
		<< "x" << 203
		<< "y" << 102
		<< bsoncxx::builder::stream::close_document
		<< bsoncxx::builder::stream::finalize;

	bsoncxx::document::view view = doc_value.view();

	bsoncxx::document::element element = view["name"];
	if (element.type() != bsoncxx::type::k_utf8) {
		// Error
	}
	std::string name = element.get_utf8().value.to_string();

	// Insert Documents

	// Insert One Document


	return 0;
}
