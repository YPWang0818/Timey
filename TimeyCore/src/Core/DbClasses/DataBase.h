#pragma once
#include "timey_pch.h"
#include "sqlite3.h"

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
				if(a.c_str[i] < b.c_str[i]) {
					return -1;
				}
				if(a.c_str[i] > b.c_str[i]) {
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

		template<wrap a, wrap b>
		struct compare_t {
			//static constexpr string upper = unwrap_v<a>;
			//static constexpr string lower = unwrap_v<b>;
			//static constexpr bool v= compare(upper, lower);
			using value_t = std::integral_constant<int, compare(unwrap_v<a>, unwrap_v<b>) >;
			static constexpr int value = value_t::value;
		};
	}

	using strP = meta::wrap;

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


	template<strP str, typename Trait>
	struct ColBaseConstraint {
		using cons_trait = Trait;
		static constexpr meta::string value = meta::unwrap_v<str>;
	};

	struct Unique : ColBaseConstraint<"UNIQUE", ConstraintType::Unique> {
	};

	struct NotNull : ColBaseConstraint<"NOT NULL", ConstraintType::NotNull> {
	};

	template<meta::wrap str>
	struct Custom : ColBaseConstraint<str, ConstraintType::Custom> {
	};

	template<meta::wrap expr>
	struct CheckExpr : ColBaseConstraint<expr, ConstraintType::CheckEpxr> { 
	};

	struct PrimaryKey : ColBaseConstraint<" PRIMARY KEY AUTOINCREMENT ",ConstraintType::PrimaryKey>{
	};



	struct ColumnType {
		struct Integer {};
		struct Real {};
		struct Text {};
		struct Blob {};
	};


	template<strP colName, typename ColType, typename ... Constraints>
	struct ColumnStmt {

	private:
		static constexpr meta::string padding = meta::stom_v<" ">;
		static constexpr meta::string header = padding + meta::unwrap_v<colName>;

		template<typename>
		struct getTypeVal;

		template<>
		struct getTypeVal<ColumnType::Integer> {
			static constexpr meta::string value = padding + meta::stom_v<"INTEGER">;
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
			static constexpr meta::string value = padding + meta::stom_v<"REAL">;
		};

		template<typename T>
		struct resolveConstraints {

			template<typename trait>
			struct resolve {
				static constexpr meta::string value = T::value;
				constexpr resolve() { static_assert(false); }
			};

			template<>
			struct resolve<ConstraintType::CheckEpxr>
			{
				static constexpr meta::string value = meta::stom_v<"CHECK ( "> + T::value + meta::stom_v<" ) ">;
			;
			};

			static constexpr meta::string value = resolve<T::cons_trait>::value;

		};

		
		template<typename ... Params>
		struct getConstraintVal;

		template<typename Fst, typename ... Rest>
		struct getConstraintVal<typename Fst, typename Rest...>{
			static constexpr meta::string value = padding + resolveConstraints<Fst>::value + getConstraintVal<Rest...>::value;
		};

		template<typename Fst>
		struct getConstraintVal<Fst> {
			static constexpr meta::string value = padding + resolveConstraints<Fst>::value;
		};

		template<>
		struct getConstraintVal<>{
			static constexpr meta::string value = padding;
		};


	public:
		static constexpr meta::string stmt = header + getTypeVal<ColType>::value
			+ getConstraintVal<Constraints...>::value;

		static constexpr meta::string name = meta::unwrap_v<colName>;
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
		static constexpr meta::string value_wp = meta::stom_v<"( ">  + value + meta::stom_v<" )">;


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
	

	template<strP tbName, typename UpdtCause, typename DelCause, typename Cols, typename FgnCols>
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

		static constexpr meta::string stmt = header + Cols::value_wp + meta::stom_v<" REFERENCES "> + meta::unwrap_v<tbName> + FgnCols::value_wp + resolveDelCaue<DelCause>::value + resolveUpdtCaue<UpdtCause>::value;

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
	


	template<strP tbName, typename PriKey, typename FgnKeys, typename ... Cols>
	struct TableStmt {

	private:
		static constexpr meta::string padding = meta::stom_v<" ">;
		static constexpr meta::string delimiter = meta::stom_v<",\n">;
		static constexpr meta::string header = meta::stom_v<"CREATE TABLE "> + meta::unwrap_v<tbName> +meta::stom_v<" (\n">;
		static constexpr meta::string trailer = meta::stom_v < ");">;


		template<typename fst, typename ... rest>
		struct catColumns{
			static constexpr meta::string value = padding + fst::stmt + delimiter +  catColumns<rest...>::value;
			static constexpr meta::string colNames = padding + fst::name + delimiter + catColumns<rest...>::colNames;
			static constexpr uint32_t count = catColumns<rest...>::count + 1;
		};

		template<typename fst>
		struct catColumns<fst>{
			static constexpr meta::string value = padding + fst::stmt;
			static constexpr meta::string colNames = padding + fst::name;
			static constexpr uint32_t count = 1;
		}; // No column is not handled. 

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

		static constexpr meta::string name = meta::unwrap_v<tbName>;
		static constexpr meta::string stmt = header + catColumns<Cols...>::value + getPriKey<PriKey>::value + getFgnKey<FgnKeys>::value + trailer;
		static constexpr meta::string allKeysNames_wp = meta::stom_v<"( "> +catColumns<Cols...>::colNames + meta::stom_v<") ">;

		using allCols_t = typename ColNames<Cols...>;

		using colCounts_t = std::integral_constant<uint32_t, catColumns<Cols...>::count>;
		using primaryKey = PriKey; // Dosen't consider the case when primary keys are in the column constraint. 
		using foreignKeys = FgnKeys;
		using name_t = meta::unwrap_t<tbName>;
	};


	template<typename Tb>
	struct InsertStmt {
	private:
		static constexpr meta::string header = meta::stom_v<"INSERT INTO  ">;
		static constexpr meta::string trialer = meta::stom_v<" ); ">;

		template<typename T>
		struct getColNameList {
			static constexpr meta::string nameList = T::allKeysNames_wp;
		};


		template<typename T>
		struct getParams;

		template<uint32_t n>
		struct getParams<std::integral_constant<uint32_t, n>>{
			static constexpr meta::string params = getParams<std::integral_constant<uint32_t, n - 1>>::params + meta::stom_v<", ?"> + meta::itom_v<n>;
		};

		template<>
		struct getParams<std::integral_constant<uint32_t, 1>> {
			static constexpr meta::string params = meta::stom_v<" ?1">;
		};

	public:

		static constexpr meta::string stmt = header + Tb::name + getColNameList<Tb>::nameList + meta::stom_v<" VALUES( ">  + getParams<typename Tb::colCounts_t>::params + trialer;


	};

	template<typename Tb>
	struct SelectStmt {
	
	private:
		static constexpr meta::string header = meta::stom_v<"SELECT * FROM ">;
		static constexpr meta::string trailer = meta::stom_v<" WHERE id == ?1;">;


	public:
		static constexpr meta::string stmt = header + Tb::name + trailer;
	};


	template<typename Tb>
	struct DeleteStmt {

	private:
		static constexpr meta::string header = meta::stom_v<"DELETE FROM ">;
		static constexpr meta::string trailer = meta::stom_v<" WHERE id == ?1;">;


	public:
		static constexpr meta::string stmt = header + Tb::name + trailer;

	};


	template<typename Tb>
	struct UpdateStmt {
	
	private:
		static constexpr meta::string header = meta::stom_v<"UPDATE  ">;
		static constexpr meta::string trailer = meta::stom_v<" WHERE id == ?1;">;
		static constexpr meta::string delimiter = meta::stom_v<",\n ">;
	
		template<typename T>
		struct getSetParams {

			template<typename idx, typename U>
			struct loopCols;

			template<uint32_t n, typename fst, typename ... rst>
			struct loopCols<std::integral_constant<uint32_t, n>, ColNames<fst, rst...>> {
				static constexpr meta::string value = fst::name + meta::stom_v<" = ?"> + meta::itom_v<n + 1> + 
					delimiter + loopCols<std::integral_constant<uint32_t, n + 1>, ColNames<rst...>>::value;
			};

			template<uint32_t n, typename fst>
			struct loopCols<std::integral_constant<uint32_t, n>, ColNames<fst>> {
				static constexpr meta::string value = fst::name + meta::stom_v<" = ?"> + meta::itom_v<n + 1>;
			};

			static constexpr meta::string setParams = loopCols<std::integral_constant<uint32_t, 1>, T::allCols_t>::value;
		
		};


	public:
		static constexpr meta::string stmt = header + Tb::name + meta::stom_v<" \n SET "> + getSetParams<Tb>::setParams
			+ meta::stom_v<"\n"> + trailer;

	
	};



	template<strP name, typename T, typename ... Meta>
	struct ComponetInfo {

		ComponetInfo(T* data)
			:Data(data) {
	
		};

		T* Data;

		static constexpr meta::string name_v = meta::unwrap_v<name>;

	private:

		template<typename U>
		struct getType {

			static constexpr bool isInteger = std::numeric_limits<U>::is_integer;
			static constexpr bool isReal = std::is_floating_point_v<U>;
			static constexpr bool isText = std::is_same_v<U, char*> || std::is_same_v<U, std::string>;
			static constexpr bool isBlob = !(isInteger || isReal || isText);

			template<bool b1, bool b2, bool b3, bool b4 >
			struct checkType {
				using colType = void;
			};

			template<>
			struct checkType<true, false, false, false>
			{	using colType = ColumnType::Integer; };

			template<>
			struct checkType<false, true, false, false>
			{ using colType = ColumnType::Real; };


			template<>
			struct checkType<false, false, true, false>
			{ using colType = ColumnType::Text; };


			template<>
			struct checkType<false, false, false, true>
			{ using colType = ColumnType::Blob; };

			using Type = typename checkType<isInteger, isReal, isText, isBlob>::colType;
		};
		
	public:
		using Comp_t = T;
		using Column_t = ColumnStmt<name, typename getType<T>::Type, Meta...>;
	
	};

	template <typename ... CompInfo>
	struct Components {

		Components(CompInfo ... args) 
			:m_components{ std::make_tuple<CompInfo...>(std::forward<CompInfo>(args)...) }
		{ }

		using compsTuple_t = std::tuple<CompInfo...>;


		template<meta::wrap name>
		static constexpr std::size_t findIdx() {
			return getIndex<name>::index;
		};

		template<meta::wrap name>
		using findType = typename std::tuple_element<findIdx<name>(), compsTuple_t>::type::Comp_t;

		template<meta::wrap name>
		inline auto* getData() {
			return std::get<findIdx<name>()>(m_components).Data;
		}
		



	private:

		template<meta::wrap compName>
		struct getIndex {



			template<std::size_t n, typename ... T>
			struct IdxLoop;

			template<std::size_t k, typename fst, typename ... rst>
			struct IdxLoop <k, fst, rst...>{
				using idx_t = std::conditional_t<meta::compare_t<compName, fst::name_v>::value == 0, std::integral_constant<int, k>, typename IdxLoop<k+1, rst...>::idx_t>;

				constexpr static int idx = idx_t::value;
					
			};

			template<std::size_t k, typename T>
			struct IdxLoop<k, T>
			{
				using idx_t = std::conditional_t<meta::compare_t<compName, T::name_v>::value == 0, std::integral_constant<int, k>, std::integral_constant<int, -1>>;

				constexpr static std::size_t idx = idx_t::value;
			};
		
			constexpr static int index = IdxLoop<0, CompInfo...>::idx;
		
		};

	private:
		std::tuple<CompInfo ...> m_components;

	};
	


	class SqliteDb;

	template<strP dbName, typename Comps>
	class DataBase {

	public:


		DataBase(const std::string& filepath) 
		{
			sqliteCoreDb = std::make_unique<SqliteDb>(filepath);
		};

		int InsertRow(const Comps& row) { return 0; };
		int DeleteRow(uint32_t Id) { return 0; };
		Ref<Comps> FetchRow(uint32_t Id) { return nullptr; };
		RefList<Comps> FindRows(const Comps& rowdata) { return nullptr; };
		Ref<Comps> UpdateRow(uint32_t Id, const Comps& rowdata) { return nullptr; };


		template<typename U>
		Ref<U> getComponent(uint32_t rowID) { return nullptr; };

		template<typename U>
		Ref<U> getComponent(const Comps& row) { return nullptr; };


		template<typename T>
		struct getCreateTableStmt {
		};


		template <typename ... CompInfo>
		struct getCreateTableStmt<std::tuple<CompInfo...>>
		{
			using colID = typename ColumnStmt<"id", ColumnType::Integer, NotNull, PrimaryKey>;
			using tbStmt = typename TableStmt<dbName, void, void, colID, typename CompInfo::Column_t...>;

		};

		


		static constexpr meta::string  createTableStmt = getCreateTableStmt<typename Comps::comps_tuple_>::tbStmt::stmt;
		using rawTbStmt_ = typename getCreateTableStmt<typename Comps::comps_tuple_>::tbStmt;

		static constexpr meta::string  insertStmt = InsertStmt<rawTbStmt_>::stmt;
		static constexpr meta::string  deleteStmt = DeleteStmt<rawTbStmt_>::stmt;
		static constexpr meta::string  updateStmt = UpdateStmt<rawTbStmt_>::stmt;
		static constexpr meta::string  selectStmt = SelectStmt<rawTbStmt_>::stmt;

	private:

		std::unique_ptr<SqliteDb> sqliteCoreDb;
	

		
	};

	enum SQLType {
		integer = SQLITE_INTEGER,
		real = SQLITE_FLOAT, // 64-bit.
		text = SQLITE_TEXT,
		blob = SQLITE_BLOB,
		null = SQLITE_NULL,
		undef = 0 // Undefined.

	};


	struct SqliteEntry {

		union EntryData {
			int64_t i; // SQLite Integer Type
			double d; // SQLite Real Type
			char* c; // SQLite Text Type
			void* v; // SQLite BLOB Type
		};

		SQLType type = SQLType::undef;
		std::size_t size = 0;
		EntryData data;

		SqliteEntry() {
			data.v = nullptr;
			size = 0;
			type = SQLType::null;
		};

		template <typename T>
		SqliteEntry(T Data) {

			static constexpr bool isInteger = std::numeric_limits<std::remove_cvref_t<T>>::is_integer;
			static constexpr bool isReal = std::is_floating_point_v<std::remove_cvref_t<T>>;
			static constexpr bool isText = std::is_same_v<std::remove_cvref_t<T>, std::string>;
			// Constr with one parameter only accept (cvref removed) std::string.  

			static_assert(isInteger || isReal || isText);


			if constexpr (isInteger) {
				data.i = (int64_t)Data;
				type = SQLType::integer;
				size = sizeof(int64_t);
			}
			else if constexpr (isReal) {
				data.d = (double)Data;
				type = SQLType::real;
				size = sizeof(double);
			}
			else if constexpr (isText) {

				const char* src = Data.c_str();
				data.c = (char*)malloc(Data.size() + 1);
				strcpy(data.c, src);

				type = SQLType::text;
				size = Data.size();
			}

		};

		template<typename T>
		SqliteEntry(T Data, std::size_t sz) {

			static constexpr bool isText = std::is_same_v<std::remove_cvref_t<T>, char const*> ||
				std::is_same_v<std::remove_cvref_t<T>, char*>;

			static constexpr bool isBlob = std::is_pointer_v<std::remove_cvref_t<T>> && (!isText);

			static_assert(isText || isBlob);


			if constexpr (isText) {

				const char* src = Data;
				data.c = (char*)malloc(sz);
				strcpy(data.c, src);
				type = SQLType::text;
				size = sz;

			}
			else if constexpr (isBlob) {
				const void* src = Data;
				data.v = (void*)malloc(sz);
				memcpy(data.v, src, sz);
				type = SQLType::blob;
				size = sz;

			};

		};

		~SqliteEntry() {

			if (type == SQLType::text || type == SQLType::blob) {
				free((void*)data.c);
			};
		};

		operator int64_t () { return data.i; };
		operator long int() { return data.i; };
		operator int() { return static_cast<int>(data.i); };


		operator double() { return data.d; };
		operator float() { return static_cast<float>(data.d); };

		operator char const* () { return static_cast<char const*>(data.c); };
		operator char* () { return data.c; }

		operator void* () { return data.v; };


	};

	class SQLiteTable;
	using SqliteRow = std::vector<SqliteEntry>;
	
	class SqliteColumn 
	{
	public:
		SqliteColumn() {
			m_wrapper = std::vector<SqliteEntry>();
			m_type = SQLType::undef;
				
		};

		SqliteColumn(std::vector<SqliteEntry> data)
			:m_wrapper(data)
		{
			m_type = SQLType::undef;
		};

		inline void changeType(SQLType tp) { m_type = tp; };
		inline SQLType isType() { return m_type; };

	private:
		std::vector<SqliteEntry> m_wrapper;
		SQLType m_type;

		friend class SqliteTable;
	};

	

	class SqliteTable {

	public:

		inline int getColumnCount() const { return colCount; };
		inline uint32_t getCurrentRow() const { return currentRow; };

		SqliteRow getCurrentRow();
		int nextRow();
		void resetRow();

		SqliteColumn getColumn(std::size_t col);

		SqliteColumn operator [] (std::size_t idx) { return getColumn(idx); };

	private:

		//Instance of this class cannot be created on user end. 
		//It can only be created using the Exec() method in SqliteQuery.

		SqliteTable(sqlite3_stmt* stmt)
			:stmt(stmt) { 
			currentRow = 0;
			colCount = sqlite3_column_count(stmt);
		};

		int64_t getEntryInt(uint32_t col);
		double getEntryReal(uint32_t col);
		const char* getEntryText(uint32_t col, std::size_t& sz);
		const void* getEntryBlob(uint32_t col, std::size_t& sz);

		static SQLType getTypeFromNativeType(int type);

	private:

		sqlite3_stmt* stmt;
		uint32_t currentRow = 0;
	    uint32_t colCount;
	};

	class SqliteQuery {

	public:
		SqliteQuery(const std::string& queryStr)
			:query(queryStr) {};

		~SqliteQuery() {};

		
		int bindDb(SqliteDb& db); // Binding db will make the qurey status become prepared.
		void unbindDb();

		int bindColumnInteger(uint32_t idx, int value);
		int bindColumnReal(uint32_t idx, double value);
		int bindColumnText(uint32_t idx, const char* value);
		int bindColumnNull(uint32_t idx);
		int bindColumnBlob(uint32_t idx, const void* data, uint32_t size);
		int unbindAllColumns();

		int exec();

		inline bool isPrepared() { return prepared; };
		inline SqliteDb* getDbHandle() { return bindedDb; };
	private:
		void bdErrorMsg(int errcode);
	private:
		bool prepared = false;

		SqliteDb* bindedDb = nullptr;
		sqlite3_stmt* stmt = nullptr;
		std::string query;
		uint32_t rowCount = 0;

		friend class SQLiteTable;
	};


	class SqliteDb {

	public:
		SqliteDb(const std::string& filePath)
			:dbPath(filePath)
		{
			initDb(filePath);
		};

		~SqliteDb() {
			closeDb();
		};

		inline sqlite3* getRawDbHanle() { return sqliteHandle; };

	private:
		void initDb(const std::string& filePath);
		void closeDb();

		sqlite3* sqliteHandle;
		std::string dbPath;
	};


	

	void print_res();

};
