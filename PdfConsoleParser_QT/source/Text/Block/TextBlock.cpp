/*====================================================*\
TextBlock.cpp - Файл реализации класса TextBlock
\*====================================================*/

#include "stdafx.h"
#include "TextBlock.h"

#include <QRegExp>

//======================================
TextBlock::TextBlock():
   language(LANGUAGE::OTHER)
,  pageArea(PAGE_AREA::BODY)
//======================================
{}

//======================================
void TextBlock::setText(const QString& _text)
//======================================
{
   text = _text;

   language   = LANGUAGE::OTHER;
   m_register = REGISTER::OTHER;

   if (text.size())
   {
      QRegExp regRUS("^[А-я]+"),
              regENG("^[A-z]+");

      if (regRUS.indexIn(text.toLocal8Bit()) != -1)
      {
         language = LANGUAGE::RUS;
      }
      else if (regENG.indexIn(text) != -1)
      {
         language = LANGUAGE::ENG;
      }

      
      if (text == text.toLower())
      {
         m_register = REGISTER::LOWER;
      }
      else if (text == text.toUpper())
      {
         m_register = REGISTER::UPPER;
      }
   }
}