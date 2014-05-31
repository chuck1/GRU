#ifndef NEBULA_UTIL_ADDRESS_HH
#define NEBULA_UTIL_ADDRESS_HH

namespace Neb {
	namespace Util {
		class AddressBase {
			public:
				virtual ~AddressBase() {}
		};

		template <class T> class Address {
			public:
				typedef Neb::Util::index_type __index_type;

				std::vector<__index_type>	vec_;
				
				std::shared_ptr<AddressBase>	parent_address_;
		};
	}
}

#endif
