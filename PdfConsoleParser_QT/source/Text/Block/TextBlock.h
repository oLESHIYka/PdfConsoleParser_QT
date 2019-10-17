#pragma once
/*====================================================*\
TextBlock.h - Заголовочный файл класса TextBlock

Класс описывает блок текста
\*====================================================*/

#include "../Style/TextStyle.h"
#include <QString>

class TextBlock
{
public:
   enum class LANGUAGE
   {
      RUS = 0,
      ENG    , 
      OTHER
   };

   enum class REGISTER
   {
      LOWER = 0,
      UPPER,
      OTHER
   };

   enum class PAGE_AREA
   {
      TOP_RUNNING_TITLE = 0,
      BODY,
      BOTTOM_RUNNING_TITLE
   };

   TextBlock();

   TextStyle   style;
   QString     text;
   LANGUAGE    language;
   PAGE_AREA   pageArea;
   REGISTER    m_register; // ACHTUNG!! говнокод!!

   void setText(const QString& _text);

   uint32 length() { return text.size(); }

   std::string print()
   {
      std::stringstream ss;

      ss << "{\n"
         << style.print()
         << "\t text: \"" << text.toStdString() << "\"\n}\n";

      return ss.str();
   }
};