#include <memory>

namespace pycxx
{
	template <typename T>
	struct indirect: public std::unique_ptr<T>
	{
		template <typename... Arg>
		indirect(Arg&&... arg): std::unique_ptr<T>{std::make_unique<T>(std::forward<Arg>(arg)...)} {}

		indirect(const indirect& o): std::unique_ptr<T>(o ? std::make_unique<T>(*o) : nullptr) {}
		indirect(indirect&& o): std::unique_ptr<T>(std::move(o)) {}

		auto& operator=(const indirect& o) {std::unique_ptr<T>::operator=(o ? std::make_unique<T>(*o) : nullptr); return *this;}
		auto& operator=(indirect&& o) {std::unique_ptr<T>::operator=(std::move(o)); return *this;}

		auto& operator=(const T& o) {std::unique_ptr<T>::operator=(std::make_unique<T>(o)); return *this;}
		auto& operator=(T&& o) {std::unique_ptr<T>::operator=(std::make_unique<T>(std::move(o))); return *this;}
	};

	template <typename T>
	inline bool operator==(const indirect<T>& a, const indirect<T>& b) { return *a == *b; }
	template <typename T>
	inline bool operator!=(const indirect<T>& a, const indirect<T>& b) { return !(a == b); }
}
