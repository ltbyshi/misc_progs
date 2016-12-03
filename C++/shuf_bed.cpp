#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <sstream>
using namespace std;
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <limits.h>

typedef uint64_t GRType;
void init_seed()
{
    srand(time(0));
}

uint64_t rand64()
{
    uint64_t r = 
        ((((uint64_t)rand() & 0xFFFFULL) <<  0) & 0x000000000000FFFFULL) |
        ((((uint64_t)rand() & 0xFFFFULL) << 16) & 0x00000000FFFF0000ULL) |
        ((((uint64_t)rand() & 0xFFFFULL) << 32) & 0x0000FFFF00000000ULL) |
        ((((uint64_t)rand() & 0xFFFFULL) << 48) & 0xFFFF000000000000ULL);
    return r;
}

long string_to_long(const std::string& s)
{
    long val;
    const char *str = s.c_str();
    char *endptr;
    val = strtol(str, &endptr, 10);
    // out of range
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0))
    {
        val = -1;
    }
    // no digits found in the string
    if (endptr == str)
        val = -1;
    return val;
}

std::vector<std::string> str_split(const std::string& s, char delim = ' ')
{
    vector<string> result;
    size_t i = 0;
    while(i < s.size())
    {
        size_t begin = i;
        while((s[i] != delim) && (i < s.size()))
            i ++;
        if(i == begin)
            break;
        else
            result.push_back(s.substr(begin, i - begin));
        if(s[i] == delim)
            i ++;
    }
    return result;
}

struct GenomeInterval
{
    unsigned int chrom;
    GRType start;
    GRType end;
    GRType length;
    char strand; // 1 for + strand, -1 for - strand
};


int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cerr << "Generate random sites within a given bed file (at least 3 columns)" << endl;
        cerr << "Usage: " << argv[0] << " site_length count" << endl;
        exit(1);
    }
    GRType site_length = atoi(argv[1]);
    GRType count = atoi(argv[2]);
    bool strandness = false;

    map<string, unsigned int> chrom_map;
    vector<string> chrom_names;

    vector<GRType> cum_lengths; // interval lengths
    cum_lengths.push_back(0);
    GRType total_length;
    istream& in = cin;

    vector<GenomeInterval> intervals;
    // read input bed file
    int lineno = 0;
    while(!in.eof())
    {
        string line;
        string chrom;
        GenomeInterval interval;
        getline(in, line);
        lineno ++;

        if(line.size() == 0)
            continue;
        
        vector<string> fields = str_split(line, '\t');
        //cout << fields.size() << " fields: ";
        //for(size_t i = 0; i < fields.size(); i ++)
        //    cout << fields[i] << ",";
        //cout << endl;
        if(fields.size() < 3)
        {
            cerr << "Error: less than 3 columns at line " << lineno << endl;
            exit(1);
        }
        chrom = fields[0];
        interval.start = string_to_long(fields[1]);
        if(interval.start < 0 )
        {
            cerr << "Error: invalid start position at line " << lineno << endl;
            exit(1);
        }
        interval.end = string_to_long(fields[2]);
        if(interval.end < 0)
        {
            cerr << " Error: invalid end position at line " << lineno << endl;
            exit(1);
        }
        if(fields.size() >= 6)
        {
            strandness = true;
            if(fields[5][0] == '+')
                interval.strand = '+';
            else if(fields[5][0] == '-')
                interval.strand = '-';
            else
            {
                cerr << "Error: Column 6 is not +/- at line " << lineno << endl;
                exit(1);
            }
        }
        
        map<string, unsigned int>::iterator it = chrom_map.find(chrom);
        if(it == chrom_map.end())
        {
            chrom_map[chrom] = chrom_names.size();
            interval.chrom = chrom_names.size();
            chrom_names.push_back(chrom);
        }
        else
        {
            interval.chrom = it->second;
        }
        interval.length = interval.end - interval.start;
        intervals.push_back(interval);
        cum_lengths.push_back(interval.length + cum_lengths.back());
    }
    total_length = cum_lengths.back();
    // print intervals
    /* 
    cerr << "num\tchrom\tlength\tcum_length" << endl;;
    for(size_t i = 0; i < intervals.size(); i ++)
    {
        GenomeInterval& interval = intervals[i];
        cerr << i 
             << '\t' << chrom_names[interval.chrom] 
             << '\t' << interval.length
             << '\t' << cum_lengths[i + 1] << endl;
    }
    cerr << endl;
    */
    // generate random sites
    init_seed();
    GRType iseq = 0;
    while((iseq < count) || (count == 0))
    {
        // select an interval randomly weighted by interval length
        size_t i_interval;
        bool valid = false;
        while(!valid)
        {
            GRType r = rand64() % total_length;
            vector<GRType>::iterator it = lower_bound(cum_lengths.begin(), cum_lengths.end(), r);
            if(it == cum_lengths.end())
            {
                cerr << "r larger than total_length " << endl;
                i_interval = intervals.size() - 1;
            }
            else
                i_interval = (it - cum_lengths.begin()) - 1;
            if(intervals[i_interval].length >= site_length)
                valid = true;
        }
        // select a site in that interval
        GenomeInterval& interval = intervals[i_interval];
        GRType site_start = (rand64() % (interval.length - site_length + 1)) + interval.start;
        cout << chrom_names[interval.chrom] 
             << '\t' << site_start
             << '\t' << site_start + site_length;
        if(strandness)
        {
            cout << "\t.\t0\t" << interval.strand;
        }
        cout << endl;
        iseq ++;
    }

    return 0;
}
