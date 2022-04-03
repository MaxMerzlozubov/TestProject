#include "Container.h"
#include <cassert>
#include <iostream>
#include <stdexcept>

struct TestStruct1 { int x{}; int y{}; double z{}; };
struct TestStruct2 { std::string text; int a{}; };

int main()
{
	try {
		Container<TestStruct1> container1;
		
		TestStruct1 ins1{ 10,20,30.0 };
		TestStruct1 ins2{ 100,200,300.0 };
		bool res1 = container1.insert(ins1, 1, 2); // OK
		assert(res1 == true);
		bool res2 = container1.insert(ins2, 1, 2); // Fail - индекс уже существует
		assert(res2 == false);
		bool res3 = container1.insert(ins2, 2, 1); // OK
		assert(res3 == true);
		
		// Получение данных по ссылке
		auto& ref1 = container1[1][2];
		// Проверка на равенство добавленных и извлечённых данных
		assert(
			ref1.carId  == 1 &&
			ref1.deviceId == 2 &&
			ref1.data.x == ins1.x &&	// 10   == 10
			ref1.data.y == ins1.y &&	// 20   == 20
			ref1.data.z == ins1.z);		// 30.0 == 30.0

		ref1.data.x = 1;
		ref1.data.y = 2;
		// Проверка на частичное равенство добавленных и изменённых данных
		assert(
			ref1.data.x != ins1.x &&	//  1   != 10
			ref1.data.y != ins1.y &&	//  2   != 20
			ref1.data.z == ins1.z);		// 30.0 == 30.0

		// Получение копии данных
		auto copy1 = container1[2][1];
		copy1.data.x = 1000;
		auto& ref2 = container1[2][1];
		assert(
			copy1.carId    == ref2.carId &&
			copy1.deviceId == ref2.deviceId &&
			copy1.data.x   != ref2.data.x);
		

		Container<TestStruct2> container2;
		TestStruct2 ins3{ "Hello!", 123 };
		TestStruct2 ins4{ "vniias", 6 };

		bool res4 = container2.insert(ins3, 4, 3);
		assert(res4 == true);
		bool res5 = container2.insert(ins4, 4, 3);
		assert(res5 == false);
		bool res6 = container2.insert(ins4, 404, 300);
		assert(res6 == true);
		
		// Получение данных по ссылке
		auto& ref3 = container2[4][3];
		// Проверка на равенство добавленных и извлечённых данных
		assert(
			ref3.carId == 4 &&
			ref3.deviceId == 3 &&
			ref3.data.text == ins3.text &&	// Hello!
			ref3.data.a == ins3.a);			// 123 == 123

		ref3.data.text = "Good bye!";
		ref3.data.a = 321;
		//ref3.carId = 0;					// Ошибка компиляции
		assert(
			ref3.carId == 4 &&
			ref3.deviceId == 3 &&
			ref3.data.text != ins3.text &&	// Good bye! != Hello!
			ref3.data.a != ins3.a);			// 321 != 123

		auto& ref4 = container2[404][300];
		assert(
			ref4.carId == 404 &&
			ref4.deviceId == 300 &&
			ref4.data.text == ins4.text &&	// vniias
			ref4.data.a == ins4.a);			// 6 == 6

		//SubscriptHelper<TestStruct2>(&container2, 100); // Ошибка компиляции

		container2[3][4];	// Выброс исключения
	}
	catch (std::out_of_range& exception) {
		std::cout << exception.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unknown error\n";
	}
	return 0;
}
