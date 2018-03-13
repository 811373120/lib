#include "./protocol/test.pb.h"
#include <iostream>
#include <fstream>


int main()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	// 设置数据， 并序列化到文件
	Test::Person person;
	person.set_id(123);
	person.set_name("abc");
	person.set_email("abc@163.com");

	std::fstream out("person.pb", std::ios::out | std::ios::binary | std::ios::trunc);
	person.SerializeToOstream(&out);
	out.close();

	// 从文件中读取数据， 并且反序列化
	Test::Person person1;
	std::fstream in("person.pb", std::ios::in | std::ios::binary);
	if (!person1.ParseFromIstream(&in)) {
		std::cerr << "Failed to parse person.pb." << std::endl;
		exit(1);
	}

	std::cout << "ID: " << person1.id() << std::endl;
	std::cout << "name: " << person1.name() << std::endl;
	if (person1.has_email()) {
		std::cout << "e-mail: " << person1.email() << std::endl;
	}

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	getchar();

	return 0;
}