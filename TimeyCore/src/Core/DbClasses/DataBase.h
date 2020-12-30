#pragma once
#include "CoreDataBase.h"
#include "Utilty.h"
#include <tuple>
#include <type_traits>
		

namespace Timey {


	template<int...I> using is = std::integer_sequence<int, I...>;
	template<int N>   using make_is = std::make_integer_sequence<int, N>;

	constexpr auto size(const char* s) { int i = 0; while (*s != 0) { ++i; ++s; } return i; }

	template<const char*, typename, const char*, typename>
	struct concat_impl;

	template<const char* S1, int... I1, const char* S2, int... I2>
	struct concat_impl<S1, is<I1...>, S2, is<I2...>> {
		static constexpr const char value[]
		{
			S1[I1]..., S2[I2]..., 0
		};
	};

	template<const char* S1, const char* S2>
	constexpr auto concat{
		concat_impl<S1, make_is<sizeof(S1)>, S2, make_is<sizeof(S2)>>::value
	};


/* calculate absolute value */
	constexpr int abs_val(int x)
	{
		return x < 0 ? -x : x;
	}

	/* calculate number of digits needed, including minus sign */
	constexpr int num_digits(int x)
	{
		return x < 0 ? 1 + num_digits(-x) : x < 10 ? 1 : 1 + num_digits(x / 10);
	}

	/* metaprogramming string type: each different string is a unique type */
	template<char... args>
	struct metastring {
		const char data[sizeof... (args)] = { args... };
	};

	/* recursive number-printing template, general case (for three or more digits) */
	template<int size, int x, char... args>
	struct numeric_builder {
		typedef typename numeric_builder<size - 1, x / 10, '0' + abs_val(x) % 10, args...>::type type;
	};

	/* special case for two digits; minus sign is handled here */
	template<int x, char... args>
	struct numeric_builder<2, x, args...> {
		typedef metastring < x < 0 ? '-' : '0' + x / 10, '0' + abs_val(x) % 10, args...> type;
	};

	/* special case for one digit (positive numbers only) */
	template<int x, char... args>
	struct numeric_builder<1, x, args...> {
		typedef metastring<'0' + x, args...> type;
	};

	/* convenience wrapper for numeric_builder */
	template<int x>
	class numeric_string
	{
	private:
		/* generate a unique string type representing this number */
		typedef typename numeric_builder<num_digits(x), x, '\0'>::type type;

		/* declare a static string of that type (instantiated later at file scope) */
		static constexpr type value{};

	public:
		/* returns a pointer to the instantiated string */
		static constexpr const char* get()
		{
			return value.data;
		}
	};

	/* instantiate numeric_string::value as needed for different numbers */
	template<int x>
	constexpr typename numeric_string<x>::type numeric_string<x>::value;

	/* SAMPLE USAGE */


	/* exponentiate a number, just for fun */
	static constexpr int exponent(int x, int e)
	{
		return e ? x * exponent(x, e - 1) : 1;
	}

	/* test a few sample numbers */
	static constexpr const char* five = numeric_string<5>::get();
	static constexpr const char* one_ten = numeric_string<110>::get();
	static constexpr const char* minus_thirty = numeric_string<-30>::get();

	/* works for any constant integer, including constexpr calculations */
	static constexpr const char* eight_cubed = numeric_string<exponent(8, 3)>::get();



	struct ColConstraint {
		struct Costum {
			constexpr Costum(const char* constraint)
				:value(constraint)
			{};
			const char* value;
		};

		struct Unique { static constexpr const char* value = " UNIQUE ";};
		struct NotNull { static constexpr const char* value = " NOT NULL "; };

		template <typename T>
		struct Range {
		
			constexpr Range(const T lower, const T upper)
				:Upper(upper), Lower(lower)
			{
				static_assert(std::is_arithmetic_v<T>, "Must be a arithmetic type.");
			}
			const T Upper;
			const T Lower;

		};

	
	};

	using UniqueCol = ColConstraint::Unique;
	using NotNullCol = ColConstraint::NotNull;
	using CostumCol = ColConstraint::Costum;
	using ColRange = ColConstraint::Range;


	struct ColumnType {
		struct Integer {};
		struct Real {};
		struct Text {};
		struct Blob {};
		struct Neumeric{};
	};


	template<typename ColType>
	struct Column {
		template<typename ...Args>
		constexpr Column(const std::string name, Args&&...args) 
			:Name(name)
		{
			Value = getColumnValue(name, std::forward<Args>(args)...);
		}
		
	private:

		constexpr const std::string getTypeVal() {

			if constexpr (std::is_same_v<ColType, ColumnType::Integer>) {
				return " INT ";
			}
			else if constexpr (std::is_same_v<ColType, ColumnType::Text>) {
				return " TEXT ";
			}
			else if constexpr (std::is_same_v<ColType, ColumnType::Blob>) {
				return " BLOB ";
			}
			else if constexpr (std::is_same_v<ColType, ColumnType::Real>) {
				return " REAL ";
			}
			else if constexpr (std::is_same_v<ColType, ColumnType::Neumeric>) {
				return " NUMERIC ";
			}
			else {
				static_assert(false, "Undefined Column type. ");
			};

			return " ";
		};


		template<class First>
		constexpr const std::string getConstraintsVal(First&& fs) {
			/**
			if constexpr (std::is_same_v<First, ColConstraint::Unique>)
			{
				return fs.value;
			}
			else if constexpr (std::is_same_v<First, ColConstraint::NotNull>)
			{
				return fs.value;
			}
			else if constexpr (std::is_same_v<First, ColConstraint::Costum>) {

				return fs.value;
			}
			else {
				return " ";
			};
			**/
		
		};

		template <class First, class ... Args>
		constexpr const std::string getConstraintsVal(First&& fs, Args&& ... args) {
			return getConstraintsVal<First>(std::forward<First>(fs)) + getConstraintsVal(std::forward<Args>(args)...);
			
		};

		template<typename ... Args>
		constexpr const std::string getColumnValue(const std::string name, Args&&...args) {
			return (name + getTypeVal() + getConstraintsVal(std::forward<Args>(args)...) );
		};

		

		std::string Name;
		std::string Value;

	};
	


	template<typename T>
	class DataBase {

		DataBase(const std::string& filepath) {
			m_sqliteWrapper = CreateScope<CoreDataBase>(filepath);
		};

		int InsertRow(const T& row) { return 0; };
		int DeleteRow(uint32_t Id) { return 0; };
		Ref<T> FetchRow(uint32_t Id) { return nullptr; };
		RefList<T> FindRows(const T& rowdata) { return nullptr; };
		Ref<T> UpdateRow(uint32_t Id, const T& rowdata) { return nullptr; };


		template<typename U>
		Ref<U> getComponent(uint32_t rowID) { return nullptr; };

		template<typename U>
		Ref<U> getComponent(const T& row) { return nullptr; };

		template<typename U>
		DataBase<U>* getComponentDb() { return nullptr; };

	

	private:
		Scope<CoreDataBase> m_sqliteWrapper;


	};


};
