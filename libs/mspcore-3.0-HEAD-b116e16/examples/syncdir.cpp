#include <algorithm>
#include <msp/core/application.h>
#include <msp/core/getopt.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/io/file.h>
#include <msp/io/print.h>
#include <msp/strings/format.h>

using namespace std;
using namespace Msp;

class SyncDir: public RegisteredApplication<SyncDir>
{
private:
	FS::Path source;
	FS::Path destination;

public:
	SyncDir(int, char **);

	int main();
private:
	void sync_directory(const FS::Path &, const FS::Path &);
	void copy_file(const FS::Path &, const FS::Path &);
};

SyncDir::SyncDir(int argc, char **argv)
{
	if(argc<3)
		throw usage_error("Missing arguments", format("Usage: %s <source> <destination>", argv[0]));

	source = argv[1];
	destination = argv[2];
}

int SyncDir::main()
{
	sync_directory(source, destination);
	return 0;
}

void SyncDir::sync_directory(const FS::Path &src, const FS::Path &dest)
{
	IO::print("Syncing %s to %s\n", src, dest);

	FS::Stat st = FS::stat(dest);
	if(!st)
		FS::mkpath(dest, 0755);
	else if(!st.is_directory())
	{
		FS::unlink(dest);
		FS::mkdir(dest, 0755);
	}

	vector<string> src_files = FS::list_files(src);
	for(const string &fn: src_files)
	{
		FS::Stat ss = FS::stat(src/fn);
		if(ss.is_directory())
			sync_directory(src/fn, dest/fn);
		else
		{
			FS::Stat ds = FS::stat(dest/fn);
			if(!ds || ds.get_size()!=ss.get_size())
				copy_file(src/fn, dest/fn);
		}
	}

	for(const string &fn: FS::list_files(dest))
		if(find(src_files.begin(), src_files.end(), fn)==src_files.end())
		{
			IO::print("Removing obsolete %s\n", dest/fn);
			if(FS::is_dir(dest/fn))
				FS::rmpath(dest/fn);
			else
				FS::unlink(dest/fn);
		}
}

void SyncDir::copy_file(const FS::Path &src, const FS::Path &dest)
{
	IO::File in(src.str(), IO::M_READ);
	IO::File out(dest.str(), IO::M_WRITE);
	char buf[16384];
	while(!in.eof())
	{
		unsigned len = in.read(buf, sizeof(buf));
		out.write(buf, len);
	}
}
