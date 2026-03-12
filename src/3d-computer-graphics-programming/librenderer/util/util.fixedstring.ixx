export module librenderer:util.fixedstring;
import std;

export namespace util
{
	template<typename T>
	struct always_false : std::false_type {};

	template<std::size_t N, typename TChar = char>
	struct fixed_string
	{
		TChar Buffer[N]{};
		constexpr fixed_string(const TChar(&str)[N])
		{
			std::copy_n(str, N, Buffer);
		}
		constexpr auto size(this auto&& self)					noexcept { return N-1; }
		constexpr auto c_str(this const fixed_string& self)		noexcept { return self.Buffer; }
		constexpr auto data(this const fixed_string& self)		noexcept { return self.Buffer; }
		constexpr auto begin(this const fixed_string& self)		noexcept { return self.Buffer; }
		constexpr auto end(this const fixed_string& self)		noexcept { return self.Buffer + N - 1; }
		constexpr auto ToString(this const fixed_string& self)	noexcept { return std::basic_string<TChar>{ self.Buffer }; }
		constexpr auto ToView(this const fixed_string& self)	noexcept { return std::basic_string_view<TChar>{ self.Buffer }; }

		template<std::size_t M>
		constexpr auto operator==(this const fixed_string& self, const TChar(&str)[M]) noexcept -> bool
		{
			if constexpr (N != M)
				return false;
			else
				return std::equal(self.begin(), self.end(), str, str + N - 1);
		}

		template<std::size_t M>
		constexpr auto operator==(this const fixed_string& self, const fixed_string<M, TChar>& other) noexcept -> bool
		{
			if constexpr (N != M)
				return false;
			else
				return std::equal(self.begin(), self.end(), other.begin(), other.end());
		}
	};
	template<std::size_t N>
	fixed_string(const char(&str)[N]) -> fixed_string<N, char>;
	template<std::size_t N>
	fixed_string(const wchar_t(&str)[N]) -> fixed_string<N, wchar_t>;

	static_assert(fixed_string("Hello") == "Hello");
	static_assert(fixed_string("Hello") == fixed_string("Hello"));
	static_assert(fixed_string("Hello") != "World");
	static_assert(fixed_string("Hello") != fixed_string("Hello!"));

	template<typename T>
	struct is_fixed_string_t : std::false_type {};
	template<std::size_t N, typename TChar>
	struct is_fixed_string_t<fixed_string<N, TChar>> : std::true_type {};
	template<typename T>
	concept is_fixed_string = is_fixed_string_t<T>::value;
}
