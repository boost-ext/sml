#include <iostream>
#include <boost/sml.hpp>

namespace sml=boost::sml;

namespace {
	struct release{};
	struct ack{};

	const auto isAckValid = []( const ack& ) { return true; };

	const auto send_ack = [] {};

	class established;
	class fin_wait_1;

	struct tester
	{
		auto operator()() const
		{
		using namespace sml;
		return make_transition_table(
				*state<established> + event<release> = state<fin_wait_1>,
				state<fin_wait_1> + event< ack > [ isAckValid ] = state<established>
			);
		}
	};

}



int main() {
sml::sm< tester > sm;
}
