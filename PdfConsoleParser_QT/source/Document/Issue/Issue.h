#pragma once
/*====================================================*\
Issue.h - Заголовочный файл класса Issue

Класс описывает статью, которая является контейнером из страниц
\*====================================================*/

#include "Page/Page.h"
#include <memory>

using uint = unsigned int;

class Issue : public std::unordered_map<uint, Page> 
{
public:
   Issue() {};
   Issue(const QString& _fileName) : m_fileName(_fileName) {};

   std::shared_ptr<QString> getFileName() { return std::make_shared<QString>(m_fileName); };

private:
   QString m_fileName;
public:
   std::string print()
   {
      std::stringstream ss;

      ss << "Issue: {\n Pages: [\n";
      std::for_each(
         this->begin(),
         this->end(),
         [this, &ss](std::pair<const uint, Page> page) {
         uint32 pageId = page.first;
         ss << (pageId > 0 ? ", " : "")
            << "[" << pageId << "]:"
            << page.second.print() << "\n";
         ++pageId;
      });
      ss << "]\n";

      return ss.str();
   }
};