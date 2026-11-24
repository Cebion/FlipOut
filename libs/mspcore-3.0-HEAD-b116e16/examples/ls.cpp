#include <msp/core/algorithm.h>
#include <msp/core/application.h>
#include <msp/core/getopt.h>
#include <msp/fs/dir.h>
#include <msp/fs/stat.h>
#include <msp/fs/utils.h>
#include <msp/io/print.h>
#include <msp/time/datetime.h>

using namespace std;
using namespace Msp;

class Ls: public RegisteredApplication<Ls>
{
private:
	typedef vector<string> Row;

	bool long_format;
	vector<string> args;

public:
	Ls(int, char **);

	virtual int main();
private:
	Row create_row(const string &, const FS::Stat &);
	void print_rows(const vector<Row> &);
};

Ls::Ls(int argc, char **argv):
	long_format(false)
{
	GetOpt getopt;
	getopt.add_option('l', "long", long_format, GetOpt::NO_ARG);
	getopt.add_argument("file", args, GetOpt::OPTIONAL_ARG);
	getopt(argc, argv);

	if(args.empty())
		args.push_back(".");
}

int Ls::main()
{
	vector<Row> file_rows;
	bool first = true;
	for(const string &a: args)
	{
		FS::Path path = a;
		FS::Stat stat = FS::lstat(path);
		if(stat.is_directory())
		{
			vector<Row> rows;
			vector<string> files = FS::list_files(path);
			sort(files);
			for(const string &fn: files)
			{
				if(long_format)
					rows.push_back(create_row(fn, FS::lstat(path/fn)));
				else
				{
					Row r;
					r.push_back(fn);
					rows.push_back(r);
				}
			}

			if(!first)
				IO::print("\n");
			if(args.size()>1)
				IO::print("%s:\n", a);
			print_rows(rows);

			first = false;
		}
		else if(long_format)
			file_rows.push_back(create_row(a, stat));
		else
		{
			Row r;
			r.push_back(a);
			file_rows.push_back(r);
		}
	}

	if(!file_rows.empty())
	{
		if(!first)
			IO::print("\n");
		print_rows(file_rows);
	}

	return 0;
}

Ls::Row Ls::create_row(const string &name, const FS::Stat &st)
{
	Row result;
	if(st.is_regular())
		result.push_back("-");
	else if(st.is_directory())
		result.push_back("D");
	else if(st.is_symlink())
		result.push_back("L");
	else
		result.push_back("?");

	result.push_back(st.get_owner());
	result.push_back(st.get_group());
	result.push_back(format("%d", st.get_size()));
	result.push_back(Time::DateTime(st.get_modify_time()).format("%Y-%m-%d %H:%M:%S"));
	result.push_back(name);

	return result;
}

void Ls::print_rows(const vector<Row> &rows)
{
	vector<unsigned> col_width;

	for(const Row &row: rows)
	{
		if(row.size()>col_width.size())
			col_width.resize(row.size(), 0);
		for(unsigned j=0; j<row.size(); ++j)
			col_width[j] = max<unsigned>(col_width[j], row[j].size());
	}

	for(const Row &row: rows)
	{
		string line;
		for(unsigned j=0; j<row.size(); ++j)
		{
			if(j>0)
				line += ' ';
			unsigned padding = col_width[j]-row[j].size();
			if(j==3)
				line += string(padding, ' ');
			line += row[j];
			if(j!=3)
				line += string(padding, ' ');
		}
		line += '\n';
		IO::print(line);
	}
}
