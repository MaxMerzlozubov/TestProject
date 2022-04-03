#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdexcept>
#include <string>
#include <unordered_map>

// Функтор для вычисления значения хэш-функции std::pair<T, T>
struct pair_hash {
	unsigned hash_combine(unsigned lhs, unsigned rhs) const {
		lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
		return lhs;
	}
	template <typename T>
	unsigned operator()(const std::pair<T, T>& x) const {
		return hash_combine(std::hash<T>()(x.first), std::hash<T>()(x.second));
	}
};

// Вспомогательный класс для восстановления местоположения устройства
template<typename T> struct DataWithIdx {
	T data;													// element
	const unsigned carId;									// element.carId
	const unsigned deviceId;								// element.deviceId

	DataWithIdx(T data_, std::pair<unsigned, unsigned> id) :
		data(data_), carId(id.first), deviceId(id.second)
	{}
};

template<typename T> class SubscriptHelper;

template<typename T> class Container {
	std::unordered_map<std::pair<unsigned, unsigned>, DataWithIdx<T>, pair_hash> data;

public:
	SubscriptHelper<T> operator[](unsigned carId) {
		return SubscriptHelper<T>(this, carId);
	}
	DataWithIdx<T>& at(unsigned carId, unsigned deviceId) {
		auto search = data.find(std::pair<unsigned, unsigned>(carId, deviceId));
		if (search != data.end())
			return search->second;
		std::string exceptionMsg = "Non-existent carId = ";
		exceptionMsg.append(std::to_string(carId));
		exceptionMsg.append(" & deviceId = ");
		exceptionMsg.append(std::to_string(deviceId));
		throw std::out_of_range(exceptionMsg);
	}
	bool insert(T in, unsigned carId, unsigned deviceId) {
		auto res = data.insert({ std::pair<unsigned, unsigned>(carId, deviceId),
			DataWithIdx<T>(in,std::pair<unsigned, unsigned>(carId, deviceId)) });
		return res.second;
	}
};

// Вспомогательный класс для реализации обращения к элементам
// в требуемом виде container[carId][deviceId]
template<typename T> class SubscriptHelper
{
	Container<T>* container;
	int carId;

public:
	DataWithIdx<T>& operator[](unsigned deviceId) {
		return container->at(carId, deviceId);
	}

private:
	friend Container<T>;
	SubscriptHelper(Container<T>* main, int carId_) : container(main), carId(carId_) {}
};

#endif
