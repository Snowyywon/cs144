#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) :   _output(capacity),_capacity(capacity),
                                                                _unassembled_bytes_cnt(0),
                                                                _eof_idx(std::numeric_limits<size_t>::max()),_st_idx(0),_TempStream() {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
// data is unsort string 
// index is string first idx
// eof is or is not end
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // _st_idx表示已经上传到那个下标了
    size_t st = std::max(_st_idx,index);
    /*  这里比较细节，需要考虑，如果分组长度大于可以存储的长度，将被截取
        _eof_idx表示当前最后被上传过的下标
    */
    size_t ed = std::min(index + data.size(),std::min(_st_idx + _capacity - _output.buffer_size(),_eof_idx));
    if(eof) _eof_idx = std::min(index + data.size(),_eof_idx);
    for(size_t i = st,j = st - index; i < ed; i++,j++)
    {
        if(_TempStream.count(i) && _TempStream[i] != data[j])
            throw runtime_error("StreamReassembler::push_substring: Inconsistent substrings!");
        _TempStream[i] = data[j];
    }
    
    string temp;
    while(_st_idx < _eof_idx && _TempStream.count(_st_idx)){
        temp.push_back(_TempStream[_st_idx]);
        _TempStream.erase(_st_idx);
        _st_idx++;
    }
    _output.write(temp);
    if(_st_idx == _eof_idx) _output.end_input();
}

size_t StreamReassembler::unassembled_bytes() const { return _TempStream.size(); }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
