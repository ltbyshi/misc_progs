#ifndef __DATAFRAME_H__
#define __DATAFRAME_H__
#include <sstream>
#include <string>
#include <vector>
#include "Exception.h"
#include "StringUtils.h"

class DataFrameError: public BasicError
{
public:
    DataFrameError(const std::string& what_arg)
        : BasicError(what_arg, "DataFrameError") {}
};

class SeriesBase
{
public:
    virtual ~SeriesBase() {};
    virtual void parse(size_t i, const std::string& s) = 0;
    virtual void parse(std::vector<std::string>& v) = 0;
};

template <typename Type>
class Series: public SeriesBase
{
public:
    Series(size_t size = 0) : _data(size) {}
    virtual ~Series() {}
    Type operator[](size_t i) { return _data[i]; }
    virtual void parse(size_t i, const std::string& s) {
        std::istringstream is(s);
        is >> _data[i];
    }
    virtual void parse(std::vector<std::string>& v) {
        if(v.size() != _data.size())
            throw DataFrameError("unmatched size when parsing a Series");
        for(size_t i = 0; i < _data.size(); i ++)
        {
            std::istringstream is(v[i]);
            is >> _data[i];
        }
    }
    Type operator[](size_t i) const { return _data[i]; }
    std::vector<Type>& data() { return _data; }
        
private:
    std::vector<Type> _data;
};


class DataFrame
{
public:
    typedef long INT;
    typedef double FLOAT;
    typedef std::string STRING;
    
    DataFrame() : _nrow(0), _ncol(0) {}
    ~DataFrame() {
        for(size_t i = 0; i < _series.size(); i ++)
            delete _series[i];
    }
    void clear() {
        for(size_t i = 0; i < _series.size(); i ++)
            delete _series[i];
        _series.resize(0);
        _nrow = 0;
        _ncol = 0;
        _header.resize(0);
    }
    
    void read_table(std::istream& fin,
                    bool header = true,
                    const std::string& types="",
                    char delimiter='\t')
    {
        clear();
        size_t lineno = 0;
        std::string line;
        std::vector<std::string> values;
        
        if(!std::getline(fin, line))
            throw DataFrameError("no lines in the input");
        lineno ++;
        Split(line, values, delimiter);
        _ncol = values.size();
        _series.resize(_ncol);
        for(size_t i = 0; i < _ncol; i ++)
            _series[i] = new Series<std::string>;
        if(header)
        {
            _header = values;
        }
        else
        {
            for(size_t i = 0; i < _ncol; i ++)
                (*(Series<std::string>*)_series[i]).data().push_back(values[i]);
            _nrow ++;
        }
        while(std::getline(fin, line))
        {
            lineno ++;
            Split(line, values, delimiter);
            if(values.size() != _ncol)
            {
                std::ostringstream os;
                os << "number of columns is not equal to that of the first line at line " << lineno;
                os << ", ncol: " << _ncol << ", fields: " << values.size();
                throw DataFrameError(os.str());
            }
            for(size_t i = 0; i < _ncol; i ++)
                (*(Series<std::string>*)_series[i]).data().push_back(values[i]);
            _nrow ++;
        }

        if(types.size() > 0)
        {
            for(size_t i = 0; i < types.size(); i ++)
            {
                switch(types[i])
                {
                    case 'i':
                    {
                        Series<INT>* parsed = new Series<INT>(_nrow);
                        parsed->parse((*(Series<std::string>*)_series[i]).data());
                        delete _series[i];
                        _series[i] = parsed;
                        break;
                    }
                    case 'f':
                    {
                        Series<FLOAT>* parsed = new Series<FLOAT>(_nrow);
                        parsed->parse((*(Series<std::string>*)_series[i]).data());
                        delete _series[i];
                        _series[i] = parsed;
                        break;
                    }
                    case 's':
                        break;
                    default:
                        throw DataFrameError("Invalid type: " + types[i]);
                }
            }
        }
    }
    
    template <typename Type>
    Type data(size_t r, size_t c) {
        return (*((Series<Type>*)_series[r]))[c];
    }
    
    template <typename Type>
    Series<Type>& icol(size_t i){
        return *((Series<Type>*)_series[i]);
    }
    
    std::vector<std::string>& header() { return _header; }
    size_t ncol() const { return _series.size(); }
    size_t nrow() const { return _nrow; }
protected:
    DataFrame(const DataFrame& rhs);
    DataFrame& operator=(const DataFrame& rhs);
private:
    std::vector<SeriesBase*> _series;
    size_t _nrow;
    size_t _ncol;
    std::vector<std::string> _header;
};

#endif
