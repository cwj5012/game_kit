#include <iostream>
#include <string>
#include <windows.h>

#include "msg.pb.h"

int main(int argc, char* argv[]) {
	Person john;
	john.set_id(1234);
	john.set_name("John Doe");
	john.set_email("jdoe@example.com");
	std::cout << john.DebugString() << std::endl;

	Person john_copy;
	john_copy.set_id(john.id());
	john_copy.set_name(john.name());
	john_copy.set_email(john.email());
	std::cout << john_copy.DebugString() << std::endl;


	//john.SerializeToString();

	Sleep(3000);
	return 0;
}
