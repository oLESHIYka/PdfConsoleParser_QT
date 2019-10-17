/*====================================================*\
PdfParser.cpp - Файл реализации класса PdfParser
\*====================================================*/

//=========================================
#include "stdafx.h"
#include "PdfParser.h"
//-----------------------------------------
#include <fstream>
//#include <boost/filesystem>

#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QFile>
#include <QRegExp>
#include <QDir>
#include <QStringList>
#include <QDateTime>
//=========================================

//==========================================
bool PdfParser::parseTag (const QString& tagName, const QString& htmlStr, int& offset, Tag& tag)
//==========================================
{
   const QString regStr = "<" + tagName + "\\s*([^>]*)>([\\s\\S]*)<\\/" + tagName + ">";
   QRegExp reg(regStr);
   reg.setMinimal(true);

   if ((offset = reg.indexIn(htmlStr, offset)) != -1)
   {
      tag.tagName  = tagName;
      tag.all      = reg.cap(0);
      tag.body     = reg.cap(2);
      
      QString attrs = reg.cap(1);
      if (attrs.size())
      {
         QRegExp attrReg("(\\w+)=\"([^\"]*)\"");
         attrReg.setMinimal(true);
         int pos = 0;
         
         while ((pos = attrReg.indexIn(attrs, pos)) != -1)
         {
            //Attribute attr;
            //attr.name  = attrReg.cap(1);
            //attr.value = attrReg.cap(2);

            //tag.attrs.push_back(attr);

            tag.attrs.insert(attrReg.cap(1), attrReg.cap(2));

            pos += attrReg.matchedLength();
         }
      }

      offset += reg.matchedLength();
   }

   return offset != -1;
}

//==========================================
// Чтение данным из конфигурационного файла
//    1. Запоминание пути к XPdf
//    2. Запоминание пути к верменной папке 
//    3. Создание папки внутри временной, в которой будут результаты работы XPdf для каждого pdf-файла
bool PdfParser::readConfig()
//==========================================
{
   bool res;
   QFile cfgFile("cfg/path.cfg");

   if (res = cfgFile.open(QIODevice::ReadOnly | QIODevice::Text))
   {
      QByteArray line;
      QRegExp    newLine("\\n$");

      // Чтение путей:
      m_pathToXPdf    = cfgFile.readLine(); m_pathToXPdf   .replace(newLine, "");
      m_pathToTempDir = cfgFile.readLine(); m_pathToTempDir.replace(newLine, "");

      // Проверка валидности путей
      if (!QFile::exists(m_pathToXPdf))
      {
         qWarning() << "Wrong path to XPdf!";
         return false;
      }

      QDir tempDir(m_pathToTempDir);
      
      if (!tempDir.exists())
      {
         qWarning() << "Wrong path to temp directory!";
         return false;
      }

      // Создание папки внутри временной
      QString curTimestr = QDateTime::currentDateTime().toString("yyyy_MM_dd__hh_mm_ss");

      m_pathToTempDir += QString("/") + curTimestr;

      tempDir.mkdir(curTimestr);
   }
   else
   {
      qWarning() << "Cannot open configuration file!";
   }

   return res;
}

//==========================================
// Метода принимает путь к pdf
// с помощью XPdf получает папку html-файлов и передаёт путь к ней дальше
//bool PdfParser::parse(const std::string& pathToPdf)
bool PdfParser::parse(const QString& pathToArticleDir)
//==========================================
{
   bool res = false;

   //res = parseAllHtmlFiles(pathToPdf); // <- temp: путь к папке с html

   //-----------------------------------------------------------------
   res = readConfig();

   if (!res)
   {
      return false;
   }

   QDir articleDir(pathToArticleDir);
   
   QStringList filters; filters << "*.pdf";

   QFileInfoList articleDirList = articleDir.entryInfoList(filters, QDir::Files);

   for (int i = 0; i < articleDirList.size(); ++i)
   {
      QFileInfo pdfFile = articleDirList.at(i);
      Issue issue(pdfFile.fileName());

      qDebug() << "\nParsing article " << i + 1 << " of " << articleDirList.size() << " (" << pdfFile.fileName() << ")...\n";

      res = parsePdf(pdfFile.absoluteFilePath(), i, issue);

      document.push_back(issue);
   }

   qDebug() << "\n";

   //-----------------------------------------------------------------
   debugPrint();
   //-----------------------------------------------------------------

   return res;
}

//==========================================
bool PdfParser::parsePdf(const QString& pathToPdf, const int& issueNum, Issue& issue)
//==========================================
{
   bool res = false;

   const QString pathToResDir = m_pathToTempDir + QString("/%1").arg(issueNum);

   const QString command = QString("\"") + m_pathToXPdf + QString("\" ") + QString("-q ") // -q уберет вывод информации на консоль
                         + pathToPdf + QString(" ") + pathToResDir;

   system(command.toStdString().c_str());

   res = parseAllHtmlFiles(pathToResDir, issue);

   return res;
}

//==========================================
bool PdfParser::parseAllHtmlFiles(const QString& pathToDir, Issue& issue)
//==========================================
{
   bool res = true;

   QDir dir(pathToDir);

   QStringList filters; filters << "page*.html";

   QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

   for (auto& file : files)
   {
      qDebug() << file.absoluteFilePath();

      QRegExp fileNumberReg("page(\\d+)\\.html");

      if (fileNumberReg.indexIn(file.fileName()) != -1)
      {
         int num = fileNumberReg.cap(1).toInt();

         Page page;
         
         if (res &= parseHtml(file.absoluteFilePath(), page))
         {
            issue.emplace(num, page);
         }
      }
   }

   if (res && issue.empty())
   {
      qWarning() << "CANNOT FIND HTML FILES!";
   }

   return res;
}

//==========================================
bool PdfParser::parseHtml(const QString& pathToHtml, Page& page)
//==========================================
{
   bool res = true;

   QFile      file(pathToHtml);
   QByteArray data;
   QString    htmlStr;

   if (!file.open(QIODevice::ReadOnly))
   {
      qWarning() << "Cannot open file: \"" << pathToHtml << "\"";
      return false;
   }

   data = file.readAll();
   htmlStr = QString::fromUtf8(data);

   Tag head, body;
   StylesMap stylesMap;
   int offset = 0;

   if (res &= parseTag("head", htmlStr, offset, head))
   {
      res &= parseHead(head.body, stylesMap); // заполняем styleMap
   }

   if (res &= parseTag("body", htmlStr, offset, body))
   {
      res &= parseBody(body.body, stylesMap, page); // Заполняем данные о текущей странице

      page.findRunningTitles();
   }

   return res;
}

//==========================================
bool PdfParser::parseHead(const QString& htmlStr, StylesMap& stylesMap)
//==========================================
{
   bool res;

   Tag style;

   int offset = 0;
   if (res = parseTag("style", htmlStr, offset, style))
   {
      QString styleRulesStr = style.body;

      QRegExp styleRuleReg("\\#(f\\d+)[^\\{]*\\{([^\\}]*)\\}");
      styleRuleReg.setMinimal(true);

      offset = 0;
      while ((offset = styleRuleReg.indexIn(styleRulesStr, offset)) != -1)
      {
         QString styleRuleId  = styleRuleReg.cap(1);
         QString styleRule    = styleRuleReg.cap(2);

         TextStyle curStyle;
         curStyle.parse(styleRule.toStdString());

         stylesMap.emplace(styleRuleId, curStyle);

         offset += styleRuleReg.matchedLength();
      }
   }

   return res;
}

//==========================================
// TO DO:
// 1. Реализовать проверку языка!!
bool PdfParser::parseBody(const QString& htmlStr, const StylesMap& stylesMap, Page& page)
//==========================================
{
   bool res = false;

   Tag div;
   int offset = 0;
   while (parseTag("div", htmlStr, offset, div))
   {
      TextStyle divStyle;
      auto styleRuleIt = div.attrs.find("style");
      if (styleRuleIt != div.attrs.end())
      {
         divStyle.parse(styleRuleIt.value().toStdString());
      }

      Tag span;
      int spanOffset = 0;
      while (parseTag("span", div.body, spanOffset, span))
      {
         TextBlock block;

         block.setText(span.body);

         block.style.concat(divStyle);

         auto spanIdAttr = span.attrs.find("id");
         if (spanIdAttr != span.attrs.end())
         {
            auto styleId = stylesMap.find(spanIdAttr.value());
            if (styleId != stylesMap.end())
            {
               block.style.concat(styleId->second);
            }
         }

         auto spanStyleAttr = span.attrs.find("style");
         if (spanStyleAttr != span.attrs.end())
         {
            TextStyle spanStyle;
            spanStyle.parse(spanStyleAttr.value().toStdString());

            block.style.concat(spanStyle);
         }

         page.push_back(block);
         res = true;
      }
   }

   return res;
}


//==========================================
void PdfParser::debugPrint() const
//==========================================
{
   QFile debugFile(m_pathToTempDir + QString("/debugFile.txt"));
   if (!debugFile.open(QFile::WriteOnly | QFile::Text))
   {
      qWarning() << "Cannot create debugFile!";
   }

   QTextStream res(&debugFile);
   const QString indent = "   ";
   res << "Document:\n[\n";
   for (auto& issue : document)
   {
      QString indent1 = indent;
      res << indent1 << "{\n";
      for (int i = 1; i <= issue.size(); ++i)
      {
         QString indent2 = indent1 + indent;
         res << indent2 << "(page: " << i << "):\n" << indent2 << "[\n";
         
         int blockId = 0;
         for (auto block : issue.at(i))
         {
            QString indent3 = indent2 + indent;
            res << indent3 << "(blockId: " << blockId++ << "):\n" << indent3 << "{\n";

            // TextBlock:
            {
               QString indent4 = indent3 + indent;

               res << indent4 << "text: " << block.text << "\n"
                   << indent4 << "language: " << ( block.language == TextBlock::LANGUAGE::ENG ? "ENG" : ( block.language == TextBlock::LANGUAGE::RUS ? "RUS" : "OTHER" ) ) << "\n"
                   << indent4 << "pageArea: " << ( block.pageArea == TextBlock::PAGE_AREA::TOP_RUNNING_TITLE ? "TOP_RUNNING_TITLE" : ( block.pageArea == TextBlock::PAGE_AREA::BODY ? "BODY" : "BOTTOM_RUNNING_TITLE" ) ) << "\n"
                   << indent4 << "register: " << ( block.m_register == TextBlock::REGISTER::LOWER ? "LOWER" : ( block.m_register == TextBlock::REGISTER::UPPER ? "UPPER" : "OTHER" ) ) << "\n"
                   << indent4 << "style:\n"<< indent4 <<"{\n";
               
               // TextStyle
               {
                  QString indent5 = indent4 + indent;
                  TextStyle& style = block.style;

                  std::string fontFamilyStr;    style.get_fontFamilyStr   (*style.fontFamily()   , fontFamilyStr);
                  std::string fontWeightStr;    style.get_fontWeightStr   (*style.fontWeight()   , fontWeightStr);
                  std::string fontStyleStr;     style.get_fontStyleStr    (*style.fontStyle()    , fontStyleStr);
                  std::string verticalAlignStr; style.get_verticalAlignStr(*style.verticalAlign(), verticalAlignStr);

                  res << indent5 << "left: "          << style.left.print().c_str()     
                      << indent5 << "top: "           << style.top.print().c_str()      
                      << indent5 << "fontSize: "      << style.fontSize.print().c_str() 
                      << indent5 << "fontFamily: "    << fontFamilyStr.c_str()          << "\n"
                      << indent5 << "fontWeight: "    << fontWeightStr.c_str()          << "\n"
                      << indent5 << "fontStyle: "     << fontStyleStr.c_str()           << "\n"
                      << indent5 << "verticalAlign: " << verticalAlignStr.c_str()       << "\n"
                      << indent5 << "color:\n" << indent5 << "{\n";
                  // Color:
                  {
                     QString indent6 = indent5 + indent;
                     res << indent6 << "red: "   << style.color.red.print().c_str()  
                         << indent6 << "green: " << style.color.green.print().c_str()
                         << indent6 << "blue: "  << style.color.blue.print().c_str() 
                         << indent6 << "alpha: " << style.color.alpha.print().c_str();
                  }
                  res << indent5 << "}\n";
               }
               res << indent4 << "}\n";
            }
            res << indent3 << "}\n";
         }
         res << indent2 << "]\n";
      }
      res << indent1 << "}\n";
   }
   res << "]";
   
   
   debugFile.close();
   qDebug() << "debugFile.txt CREATED!";
}