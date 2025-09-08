#pragma once

#include <assert.h>

#include <string>
#include <vector>

/// @code
/// prependable bytes表示数据包的字节数
/// readerIndex 应用程序从readerIndex指向的位置开始读缓冲区

/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode

/// @brief 为了非阻塞IO定做的vector<char> 
class Buffer {
 public:
  static const size_t KCheapPprepend = 8;   // 数据包长度8字节
  static const size_t KInitailSize = 1024;  // 缓冲区初始大小

  explicit Buffer(size_t initialSize = KInitailSize)
      : buffer_(KCheapPprepend + initialSize),
        readerIndex_(KCheapPprepend),
        writerIndex_(KCheapPprepend) {}

  // 待读取的数据大小
  const size_t readableBytes() const { return writerIndex_ - readerIndex_; }

  // 可写的空闲空间大小
  const size_t writeableBytes() const { return buffer_.size() - writerIndex_; }

  // 预留空间的大小
  const size_t prependableBytes() const { return readerIndex_; }

  /// @brief 缓冲区中可读取的起始位置
  /// @return 
  const char* peek() const { return begin() + readerIndex_; }

  /// @brief 在读取buffer中可读取的数据后，更新readerIndex_
  /// @param len 
  void retrieve(size_t len) {
    if (len < readableBytes()) {
      readerIndex_ += len;
    } else {
      readerIndex_ = KCheapPprepend;
      writerIndex_ = KCheapPprepend;
    }
  }

  std::string retrieveAsString(size_t len) {
    assert(len <= readableBytes());
    std::string result(peek(), len);
    retrieve(len);
    return result;
  }

  std::string retrieveAllAsString() {
    return retrieveAsString(readableBytes());
  }

  /// @brief 向writeable区间添加数据
  /// @param data 
  /// @param len 
  void append(const char* data, size_t len) {
    if (writeableBytes() < len) {
      makeSpace(len);
    }
    std::copy(data, data + len, beginWrite());
    writerIndex_ += len;
  }

  char* beginWrite() { return begin() + writerIndex_; }

  const char* beginWrite() const { return begin() + writerIndex_; }

  /// @brief 从fd读取数据，存放到writeIndex_地址，返回实际读取的数据大小
  ///        优化read函数
  /// @param fd 
  /// @param saveErrno 
  /// @return 
  ssize_t readFd(int fd, int* saveErrno);
  ssize_t writeFd(int fd, int* saveErrno);

 private:
  /// @brief 返回Buffer底层的数据首地址
  /// @return
  char* begin() { return &*buffer_.begin(); }

  const char* begin() const { return &*buffer_.begin(); }

  /// @brief 
  /// @param len 
  void makeSpace(size_t len) {
    if (writeableBytes() + prependableBytes() < len + KCheapPprepend) {
      buffer_.resize(writerIndex_ + len);
    } else {
      auto readable = readableBytes();

      std::copy(begin() + readerIndex_, begin() + writerIndex_,
                begin() + KCheapPprepend);
      readerIndex_ = KCheapPprepend;
      writerIndex_ = readerIndex_ + readable;
    }
  }

  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;
};