#include <stdexcept>
#include "regmatch.h"

using namespace std;

namespace Msp {

RegMatch::RegMatch(const string &str, const vector<Group> &grps):
	groups(grps)
{
	for(Group &g: groups)
		if(g.match)
		{
			g.length = g.end-g.begin;
			g.str = str.substr(g.begin, g.length);
		}
}

const RegMatch::Group &RegMatch::group(unsigned i) const
{
	if(i>=groups.size())
		throw out_of_range("RegMatch::group");
	return groups[i];
}

} // namespace Msp
