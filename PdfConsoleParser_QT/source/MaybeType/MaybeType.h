#pragma once
/*====================================================*\
MaybeType.h - Заголовочный файл шаблонного класса MaybeType
\*====================================================*/

#include "../stdafx.h"

namespace std {
   template <typename T>
   string to_string(T param)
   {
      string str = "";
      stringstream ss;
      ss << (static_cast<int>(param));
      getline(ss, str);
      return str;
   }
}

template<typename _type>
class MaybeType
{
   template<typename> friend class MaybeType;
public:
   MaybeType()          : m_isValid(false)            {};
   MaybeType(_type _val): m_isValid(true), m_value(_val){};
   
   template<typename srcType> 
   MaybeType(const MaybeType<srcType>& _src) :
       m_value   (_src.m_value)
      ,m_isValid (_src.m_isValid)
    {};

   const std::shared_ptr<_type> operator() ()
   {
      return std::make_shared<_type>(m_value);
   }
   void operator() (const _type& _val)
   {
      m_value = _val;
      m_isValid = true;
   }

   const bool isValid() const { return m_isValid; }
   
   void unset() { m_isValid = false; }

   void        print_cout() { std::cout << "m_value: " << m_value << ",\t m_isValid: " << ( m_isValid ? "true" : "false" ) << std::endl; }
   std::string print()      { std::string res = (m_isValid ? std::to_string(m_value) : "UNDEFINED") + "\n"; return res; };
private:
   _type m_value;
   bool  m_isValid;
};