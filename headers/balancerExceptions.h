#ifndef __BALANCEREXCEPTIONS_H__
#define __BALANCEREXCEPTIONS_H__

#include <exception>
#include <string>

class configReaderException : public std::exception {
private:
  std::string m_error;

public:
  configReaderException(const std::string &error) : m_error(error){

                                                    };
  virtual const char *what() const noexcept {
    return m_error.c_str();
  }
};

#endif //__BALANCEREXCEPTIONS_H__
