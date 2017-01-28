#include "react/Domain.h"
#include "react/Signal.h"

REACTIVE_DOMAIN(D, react::sequential)

enum PropertyType {
	Read,
	Write
};

/// <summary>
/// Base Property
/// </summary>
template<class T, PropertyType pType = Write>
class Property
{
	friend std::ostream& operator<<(std::ostream& os, const Property<T, pType>& obj);
};

template <typename T, PropertyType pType>
std::ostream& operator<<(std::ostream& os, const Property<T, pType>& obj)
{
	os << obj.value;
	return os;
}

/// <summary>
/// Read only property specialization
/// </summary>
template<class T>
class Property<T, Read>
{
	using func = std::function<void(const T &)>;
	// if readonly use Signal else use VarSignal
	using Reactive = react::Signal<D, T>;
public:
	Property() : value(T()) {}

	/// <summary>
	/// read only CTOR
	/// </summary>
	/// <param name="value"></param>
	/// <param name="get_func"></param>
	Property(const T &value, func get_func = nullptr)
	{
		this->value = value;
		this->get_func = get_func;
	}

	/// <summary>
	/// read only CTOR
	/// </summary>
	/// <param name="get_func"></param>
	Property(func get_func = nullptr) : Property(T(), get_func) {}

private:
	T value;
	Reactive reactive;

	func get_func = nullptr;

	friend std::ostream& operator<<(std::ostream& os, const Property<T, Write>& obj);
};

/// <summary>
/// Read-write property specialization
/// </summary>
template<class T>
class Property<T, Write>
{
	using func = std::function<void(const T &)>;
	// if readonly use Signal else use VarSignal
	using Reactive = react::VarSignal<D, T>;
public:
	Property() : value(T()) {}

	/// <summary>
	/// read-write CTOR
	/// </summary>
	/// <param name="value"></param>
	/// <param name="set_func"></param>
	Property(const T &value, func set_func = nullptr)
	{
		this->value = value;
		this->set_func = set_func;
	}

	/// <summary>
	/// read-write CTOR
	/// </summary>
	/// <param name="set_func"></param>
	Property(func set_func) : Property(T(), nullptr, set_func) {}

	/// <summary>
	/// read-write CTOR
	/// </summary>
	/// <param name="value"></param>
	/// <param name="get_func"></param>
	/// <param name="set_func"></param>
	Property(const T &value, func get_func, func set_func)
	{
		this->value = value;
		this->get_func = get_func;
		this->set_func = set_func;
	}

	/// <summary>
	/// propert = value
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	T& operator=(const T &value) {
		if (set_func)
			set_func(value);
		return this->value;
	}

private:
	T value;
	Reactive reactive;

	func get_func = nullptr;
	func set_func = nullptr;

	friend std::ostream& operator<<(std::ostream& os, const Property<T, Read>& obj);
};


int main(int argc, char* argv[])
{

	return 0;
}
