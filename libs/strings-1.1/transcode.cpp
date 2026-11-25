/* $Id$ */
#include <iostream>
#include <string>
#include <msp/strings/codec.h>

using namespace std;
using namespace Msp;

int main(int argc, char **argv)
{
	if(argc<3)
	{
		cerr<<"Usage: "<<argv[0]<<" <from-enc> <to-enc>\n";
		return 1;
	}

	Codecs::Codec *from=Codecs::create_codec(argv[1]);
	Codecs::Codec *to=Codecs::create_codec(argv[2]);

	Codecs::Decoder *from_dec=from->create_decoder(Codecs::TRANSLITERATE);
	Codecs::Encoder *to_enc=to->create_encoder(Codecs::TRANSLITERATE);

	string line;
	while(getline(cin, line))
	{
		line+='\n';
		Codecs::ustring ustr;
		from_dec->decode(line, ustr);
		string result;
		to_enc->encode(ustr, result);
		cout<<result;
	}

	string result;
	to_enc->sync(result);
	cout<<result;

	delete from_dec;
	delete to_enc;
	delete from;
	delete to;

	return 0;
}
