#pragma once
#include "CoreDataBase.h"
#include "Core/Utilty.h"
#include <tuple>
#include <type_traits>
		

namespace Timey {

	//	Some compile time utilities for working with strings and passing strings as template parameters.
	//	Modified from the source code of Conor Williams.

	// MIT License

	// Copyright (c) 2020 Conor Williams

	// Permission is hereby granted, free of charge, to any person obtaining a copy
	// of this software and associated documentation files (the "Software"), to deal
	// in the Software without restriction, including without limitation the rights
	// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	// copies of the Software, and to permit persons to whom the Software is
	// furnished to do so, subject to the following conditions:

	// The above copyright notice and this permission notice shall be included in
	// all copies or substantial portions of the Software.

	// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	// SOFTWARE.

	#include <cstdint>

	// Provides utilities for working with compile time strings as types and passing
	// string literals as template parameters. See the examples for details.

	namespace meta {

		template <char... args>
		struct string {
			static constexpr char c_str[sizeof...(args) + 1] = { args..., '\0' };
			static constexpr std::size_t len() { return sizeof...(args); }
			constexpr operator char const* () const { return c_str; }
		};

		template <char... lhs, char... rhs>
		constexpr auto operator+(string<lhs...> a, string<rhs...> b) {
			return string<lhs..., rhs...>{};
		}

		template <char... args>
		constexpr int compare(string<args...>, string<args...>) {
			return 0;
		}

		template <char... lhs, char... rhs>
		constexpr int compare(string<lhs...> a, string<rhs...> b) {
			if constexpr (sizeof...(lhs) < sizeof...(rhs)) {
				return -1;
			}
			if constexpr (sizeof...(lhs) > sizeof...(rhs)) {
				return 1;
			}
			for (std::size_t i = 0; i < sizeof...(lhs); ++i) {
				if (a.c_str[i] < b.c_str[i]) {
					return -1;
				}
				if (a.c_str[i] > b.c_str[i]) {
					return 1;
				}
			}
		}
	

		// Struct to wrap a template non-type parameter string literal
		template <std::size_t N>
		struct wrap {
			static constexpr std::size_t size() { return N - 1; }

			constexpr wrap(char const* s) {
				for (std::size_t i = 0; i < N - 1; ++i) {
					buf[i] = s[i];
				}
			}

			template <char... args>
			constexpr wrap(string<args...> s) {
				for (std::size_t i = 0; i < N - 1; ++i) {
					buf[i] = s[i];
				}
			}

			char buf[N - 1]{};  // miss null
		};

		template <unsigned N>
		wrap(char const (&)[N])->wrap<N>;

		template <char... args>
		wrap(string<args...>)->wrap<sizeof...(args) + 1>;

		namespace impl {

			template <auto S>
			struct wrap_type {
				static constexpr auto c_str = S;
			};

			template <typename S, std::size_t N, char... args>
			struct explode : explode<S, N - 1, S::c_str.buf[N - 1], args...> {};

			template <typename S, char... args>
			struct explode<S, 0, args...> {
				using type = string<args...>;
			};

			constexpr std::intmax_t abs_val(std::intmax_t x) { return x < 0 ? -x : x; }

			constexpr std::intmax_t num_digits(std::intmax_t x) {
				return x < 0 ? 1 + num_digits(-x) : x < 10 ? 1 : 1 + num_digits(x / 10);
			}

			template <std::intmax_t size, std::intmax_t x, char... args>
			struct int_to_str_impl
				: int_to_str_impl<size - 1, x / 10, '0' + abs_val(x) % 10, args...> {};

			template <std::intmax_t x, char... args>
			struct int_to_str_impl<2, x, args...> {
				using type =
					string < x<0 ? '-' : '0' + x / 10, '0' + abs_val(x) % 10, args...>;
			};

			template <std::intmax_t x, char... args>
			struct int_to_str_impl<1, x, args...> {
				using type = string<'0' + x, args...>;
			};

			template<typename, typename>
			struct str_cat;

			template<char ... lhs, char ... rhs>
			struct str_cat<string<lhs ...>, string<rhs ...>> {
				using type = string<lhs..., rhs...>;
			};


			template<typename fst, typename ... strs>
			struct mstr_cat {
				using type = typename str_cat<fst, typename mstr_cat<strs...>::type>::type;
			};

			template<typename str>
			struct mstr_cat<str> {
				using type = str;
			};


		}  // namespace impls

		template <auto wrapper>
		using unwrap_t = typename impl::explode<impl::wrap_type<wrapper>, wrapper.size()>::type;

		template <auto wrapper>
		inline constexpr string unwrap_v = unwrap_t<wrapper>{};

		template <wrap wrapper>
		using stom_t = unwrap_t<wrapper>;

		template <wrap wrapper>
		inline constexpr string stom_v = unwrap_t<wrapper>{};

		template <std::intmax_t x>
		using itom_t = typename impl::int_to_str_impl<impl::num_digits(x), x>::type;

		template <std::intmax_t x>
		inline constexpr string itom_v = itom_t<x>{};

		template <wrap ... wrappers>
		using strcat_t = typename impl::mstr_cat<unwrap_t<wrappers>...>::type;


	}

	struct ConflictCause {
		struct Undefined {};
		struct RollBack  {};
		struct Abort {};
		struct Fail {};
		struct Ignore {};
		struct Replace {};
	};

	template <typename Ky>
	struct ResolveKyCause;

	template<>
	struct ResolveKyCause<ConflictCause::Undefined> {
		static constexpr meta::string value = meta::stom_v<" ">;
	};

	template<>
	struct ResolveKyCause<ConflictCause::Abort> {
		static constexpr meta::string value = meta::stom_v<"ON CONFLICT ABORT">;
	};

	template<>
	struct ResolveKyCause<ConflictCause::RollBack> {
		static constexpr meta::string value = meta::stom_v<"ON CONFLICT ROLLBACK">;
	};

	template<>
	struct ResolveKyCause<ConflictCause::Fail> {
		static constexpr meta::string value = meta::stom_v<"ON CONFLICT FAIL">;
	};

	template<>
	struct ResolveKyCause<ConflictCause::Ignore> {
		static constexpr meta::string value = meta::stom_v<"ON CONFLICT IGNORE">;
	};

	template<>
	struct ResolveKyCause<ConflictCause::Replace> {
		static constexpr meta::string value = meta::stom_v<"ON CONFLICT REPLACE">;
	};


	struct ConstraintType {
		struct Custom {};
		struct CheckEpxr {};
		struct NotNull {};
		struct PrimaryKey {};
		struct Unique {};
	};


	template<meta::wrap str, typename trait>
	struct ColBaseConstraint {

		using cons_trait = trait;
		static constexpr meta::string value = meta::unwrap_v<str>;
	};

	struct Uniqe  : ColBaseConstraint<"UNIQUE", ConstraintType::Unique> {};
	struct NotNull : ColBaseConstraint<"NOT NULL", ConstraintType::NotNull> {};

	template<meta::wrap str>
	struct Custom : ColBaseConstraint<str, ConstraintType::Custom> {};

	template<meta::wrap expr>
	struct CheckExpr : ColBaseConstraint<expr, ConstraintType::CheckEpxr> {};

	template<typename Ky>
	struct PrimaryKey : ColBaseConstraint<" ", ConstraintType::PrimaryKey>{
		using KyCause = Ky;
	};



	struct ColumnType {
		struct Integer {};
		struct Real {};
		struct Text {};
		struct Blob {};
		struct Neumeric{};
	};


	template<meta::wrap colname, typename ColType, typename ... Constraints>
	struct ColumnStmt {

	private:
		static constexpr meta::string padding = meta::stom_v<" ">;
		static constexpr meta::string header = padding + meta::unwrap_v<colname>;

		template<typename>
		struct getTypeVal;

		template<>
		struct getTypeVal<ColumnType::Integer> {
			static constexpr meta::string value = padding + meta::stom_v<"INT">;
		};

		template<>
		struct getTypeVal<ColumnType::Text> {
			static constexpr meta::string value =  padding + meta::stom_v<"TEXT">;
		};

		template<>
		struct getTypeVal<ColumnType::Blob> {
			static constexpr meta::string value = padding + meta::stom_v<"BLOB">;
		};

		template<>
		struct getTypeVal<ColumnType::Real> {
			static constexpr meta::string value = padding + meta::stom_v<"NUMERIC">;
		};

		template<typename T>
		struct resolveConstraints {

			template<typename trait>
			struct resolve {

				resolve() {
					// Type check here.
				};

				static constexpr meta::string value = T::value;
			};

			template<>
			struct resolve<ConstraintType::CheckEpxr>
			{
				static constexpr meta::string value = meta::stom_v<"CHECK ( "> + T::value + meta::stom_v<" ) ">;
			};


			template<>
			struct resolve<ConstraintType::PrimaryKey>
			{
				static constexpr meta::string header = meta::stom_v<"PRIMARY KEY ASC ">;
				static constexpr meta::string trailer = meta::stom_v<" AUTOINCREMENT">;

				static constexpr meta::string value = header + ResolveKyCause<T::KyCause>::value + trailer;
			};

			static constexpr meta::string value = resolve<T::cons_trait>::value;

		};

		



		template<typename Fst, typename ... Rest>
		struct getConstraintVal {
			static constexpr meta::string value = padding + resolveConstraints<Fst>::value + getConstraintVal<Rest...>::value;
		};

		template<typename Fst>
		struct getConstraintVal<Fst> {
			static constexpr meta::string value = padding + resolveConstraints<Fst>::value;
		};

	public:
		static constexpr meta::string stmt = header + getTypeVal<ColType>::value
			+ getConstraintVal<Constraints...>::value;

		static constexpr meta::string name = meta::unwrap_v<colname>;
		static constexpr meta::string coltype = getTypeVal<ColType>::value;
	
	};


	template<typename ... ColStmts>
	struct ColNames {

	private:
		static constexpr meta::string padding = meta::stom_v<" ">;
		static constexpr meta::string delimiter = meta::stom_v<",">;


		template<typename fst, typename ... rest>
		struct catNames {
			static constexpr meta::string value = padding + fst::name + delimiter + catNames<rest...>::value;
		};

		template<typename fst>
		struct catNames<fst> {
			static constexpr meta::string value = padding + fst::name;
		};

	public:
		static constexpr meta::string value = catNames<ColStmts...>::value;
		static constexpr meta::string value_wp = meta::stom_v<"( ">  + value + meta::stom_t<" )">;


	};


	template<typename Cols>
	struct PriKey {

	private:

		static constexpr meta::string padding = meta::stom_v<" ">;
		static constexpr meta::string delimiter = meta::stom_v<",">;
		static constexpr meta::string header = meta::stom_v<" PRIMARY KEY ">;
		static constexpr meta::string trailer = meta::stom_v<" ON CONFLICT FAIL">;
		// Other conflict causes are not implelmented.



	public:

		static constexpr meta::string keys = Cols::value_wp;
		static constexpr meta::string stmt = header + Cols::value_wp + trailer;
	};

	struct ForeignKeyCause {
		struct Undefined {};
		struct SetNull {};
		struct Restrict {};
		struct Cascade {};
	};
	

	template<meta::wrap TbName, typename UpdtCause, typename DelCause, typename Cols, typename FgnCols>
	struct FgnKey {

	private:

		static constexpr meta::string padding = meta::stom_v<" ">;
		static constexpr meta::string delimiter = meta::stom_v<",">;
		static constexpr meta::string header = meta::stom_v<" FOREIGN KEY ">;


		template<typename T>
		struct resolveDelCaue;

		template<>
		struct resolveDelCaue<ForeignKeyCause::Undefined>
		{
			static constexpr meta::string value = meta::stom_v<" ">;
		};

		template<>
		struct resolveDelCaue<ForeignKeyCause::Cascade>
		{
			static constexpr meta::string value = meta::stom_v<" ON DELETE CASCADE ">;
		};


		template<>
		struct resolveDelCaue<ForeignKeyCause::Restrict>
		{
			static constexpr meta::string value = meta::stom_v<" ON DELETE RESTRICT ">;
		};


		template<>
		struct resolveDelCaue<ForeignKeyCause::SetNull>
		{
			static constexpr meta::string value = meta::stom_v<" ON DELETE SET NULL ">;
		};


		template<typename T>
		struct resolveUpdtCaue;

		template<>
		struct resolveUpdtCaue<ForeignKeyCause::Undefined>
		{
			static constexpr meta::string value = meta::stom_v<" ">;
		};

		template<>
		struct resolveUpdtCaue<ForeignKeyCause::Cascade>
		{
			static constexpr meta::string value = meta::stom_v<" ON UPDATE CASCADE ">;
		};


		template<>
		struct resolveUpdtCaue<ForeignKeyCause::Restrict>
		{
			static constexpr meta::string value = meta::stom_v<" ON UPDATE RESTRICT ">;
		};


		template<>
		struct resolveUpdtCaue<ForeignKeyCause::SetNull>
		{
			static constexpr meta::string value = meta::stom_v<" ON UPDATE SET NULL ">;
		};


	public:

		static constexpr meta::string stmt = header + Cols::value_wp + meta::stom_v<" REFERENCES "> + meta::unwrap_t<TbName> + FgnCols::value_wp + resolveDelCaue<DelCause> + resolveUpdtCaue<UpdtCause>;

	};

	template<typename ... FgnKey>
	struct ForeignKeys {

	private:
		static constexpr meta::string padding = meta::stom_v<" ">;
		static constexpr meta::string delimiter = meta::stom_v<",\n">;


		template<typename fst, typename ... rest>
		struct catKeys {
			static constexpr meta::string value = padding + fst::stmt + delimiter + catKeys<rest...>::value;
		};

		template<typename fst>
		struct catKeys<fst> {
			static constexpr meta::string value = padding + fst::stmt;
		};

	public:
		static constexpr meta::string stmt = catKeys<FgnKey...>::value;

	};
	


	template<meta::wrap tbname, typename PriKey, typename FgnKeys, typename ... Cols>
	struct TableStmt {

	private:
		static constexpr meta::string padding = meta::stom_v<" ">;
		static constexpr meta::string delimiter = meta::stom_v<",\n">;
		static constexpr meta::string header = meta::stom_v<"CREATE TABLE "> + meta::unwrap_v<tbname> +meta::stom_v<" (\n">;
		static constexpr meta::string trailer = meta::stom_v < ");">;


		template<typename fst, typename ... rest>
		struct catColumns{
			static constexpr meta::string value = padding + fst::stmt + delimiter + catColumns<rest...>::value;
		};

		template<typename fst>
		struct catColumns<fst>{
			static constexpr meta::string value = padding + fst::stmt;
		};

		template<typename T>
		struct getFgnKey {
			static constexpr meta::string value = delimiter + T::stmt;
		};

		template<>
		struct getFgnKey<void> {
			static constexpr meta::string value = meta::stom_v<" ">;
		};

		template<typename T>
		struct getPriKey {
			static constexpr meta::string value = delimiter + T::stmt;
		};

		template<>
		struct getPriKey<void> {
			static constexpr meta::string value = meta::stom_v<" ">;
		};



	public:

		static constexpr meta::string name = meta::unwrap_v<tbname>;
		static constexpr meta::string stmt = header + catColumns<Cols...>::value + getPriKey<PriKey>::value + getFgnKey<FgnKeys>::value + trailer;

		using primaryKey = PriKey;
		using foreignKeys = FgnKeys;

		using name_t = meta::unwrap_t<tbname>;
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

	void print_res();
};
