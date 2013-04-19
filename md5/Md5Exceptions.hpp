#ifndef MD5EXCEPTIONS
#define MD5EXCEPTIONS

#include <exception>

class md5Exception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Md5 Exception occured";
  }
};

class md5JointException: public md5Exception
{
  virtual const char* what() const throw()
  {
    return "Md5 Exception occured relating to joints";
  }
};

class md5LoadMeshException: public md5Exception
{
  virtual const char* what() const throw()
  {
    return "Md5 Exception occured while reading mesh";
  }
};

class md5DrawMeshException: public md5Exception
{
  virtual const char* what() const throw()
  {
    return "Md5 Exception occured while reading mesh";
  }
};

class md5LoadAnimException: public md5Exception
{
  virtual const char* what() const throw()
  {
    return "Md5 Exception occured while reading animation";
  }
};

class md5LoadFrameException: public md5LoadAnimException
{
  virtual const char* what() const throw()
  {
    return "Md5 Exception occured while reading frame";
  }
};

#endif