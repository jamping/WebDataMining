////////////////////////////////// Macros ///////////////////////////

#ifndef __IPENUM_H__
#define __IPENUM_H__



class CIPEnum
{
public:
//constructors / Destructors
  CIPEnum();
  ~CIPEnum();

//methods
  BOOL Enumerate();

protected:
  virtual BOOL EnumCallbackFunction(LPTSTR strHost, int nAdapter, const in_addr& address) = 0;
  BOOL m_bWinsockInitialied;
};




#endif //__IPENUM_H__