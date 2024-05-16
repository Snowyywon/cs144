#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) :  _buffer(capacity + 1) , _capacity(capacity) , _begin_idx(0),_end_idx(capacity),
                                                _written_cnt(0) , _read_cnt(0),_input_end(false){}

// write data to buffer
size_t ByteStream::write(const string &data) {
    size_t _real_size = std::min(data.size(),remaining_capacity()); 
    for(size_t i = 0; i < _real_size; i++){
        _end_idx = (_end_idx + 1) % (_capacity + 1);
        _buffer[_end_idx] = data[i];
    }
    _written_cnt += _real_size;
    return _real_size;
}

// copied len char to buffer 
//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    std::string temp;
    size_t _real_len = std::min(len,buffer_size());
    for(size_t i = 0; i < _real_len; i++){
        temp.push_back(_buffer[(_begin_idx + i)%(_capacity + 1)]);
    }
    return temp;
}

// pop len char to buffer
//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    size_t _real_len = std::min(len,buffer_size());
    _begin_idx = (_begin_idx + _real_len)%(_capacity + 1); 
    // ??,???pop?_read_cnt+
    _read_cnt += _real_len;
}


// read len char to buffer
//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    size_t _real_len = std::min(len,buffer_size());
    std::string temp = peek_output(_real_len);
    pop_output(_real_len);
    return temp;
}

// end input
void ByteStream::end_input() { _input_end = true; }
// input status
bool ByteStream::input_ended() const { return _input_end; }
//buff user size
size_t ByteStream::buffer_size() const { return (_end_idx - _begin_idx + 1 + _capacity + 1)%(_capacity +1); }
// buffer is or if not empty
bool ByteStream::buffer_empty() const { return buffer_size() == 0; }
// byteStream EOF status
bool ByteStream::eof() const { return _input_end && buffer_empty(); }
// written char size
size_t ByteStream::bytes_written() const { return _written_cnt; }
// read char size
size_t ByteStream::bytes_read() const { return _read_cnt; }
// remaining capacity
size_t ByteStream::remaining_capacity() const { return _capacity - buffer_size(); }

