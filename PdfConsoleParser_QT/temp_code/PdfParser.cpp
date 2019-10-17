/*====================================================*\
PdfParser.cpp - ���� ���������� ������ PdfParser
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
            Attribute attr;
            attr.name  = attrReg.cap(1);
            attr.value = attrReg.cap(2);

            tag.attrs.push_back(attr);

            pos += attrReg.matchedLength();
         }
      }

      offset += reg.matchedLength();
   }

   return offset != -1;
}

//==========================================
// ������ ��������� ���� � pdf
// � ������� xpdf �������� ����� html-������ � ������� ���� � ��� ������
bool PdfParser::parse(const std::string& pathToPdf)
//==========================================
{
   bool res = false;

   res = parseAllHtmlFiles(pathToPdf); // <- temp: ���� � ����� � html



   return res;
}

//==========================================
bool PdfParser::parseAllHtmlFiles(const std::string& pathToDir)
//==========================================
{
   bool res = true;

   QDir dir(pathToDir.c_str());

   QStringList filters; filters << "page*.html";

   QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

   for (auto& file : files)
   {
      res &= parseHtml(file.absoluteFilePath());
   }

   return res;
}

//==========================================
bool PdfParser::parseHtml(const QString& pathToHtml)
//==========================================
{
    bool res;

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

    if (res = parseTag("head", htmlStr, offset, head))
    {
        parseHead(head.body, stylesMap); // заполняем styleMap
    }

    if (res &= parseTag("body", htmlStr, offset, body))
    {
        parseBody(body.body, stylesMap); // Заполняем данные о текущей странице
    }

    return res;
}

//==========================================
bool PdfParser::parseHead(const QString& htmlStr,       StylesMap& stylesMap)
//==========================================
{
    bool res;

    Tag style;

    if (res = parseTag("style", htmlStr, 0, style))
    {
        
    }

    return res;
}

/*
//==========================================
// ����� ��������� �� ���� ���� � html-�����
// 1 ���� - 1 �������� ���������
bool PdfParser::parseHtml(const std::string& pathToHtml)
//==========================================
{
   bool res = false;

   std::vector<TextStyle> styleRules;

   //std::ifstream htmlFile(pathToHtml);
   //std::stringstream htmlFileStream;

   //if (!htmlFile.is_open())
   //{
   //   std::cout << "ERROR: Connot open file: \"" << pathToHtml.c_str() << "\"\n";
   //   return res;
   //}

   //document.resize(document.size() + 1); // ��������� �������� � ��������

   //std::string line;
   //while (std::getline(htmlFile, line))
   //{
   //   htmlFileStream << line;
   //}
   //htmlFile.close();

   ////std::cout << htmlFileStream.str().c_str();

   //auto htmlFileStr = htmlFileStream.str();

   
   std::string htmlFileStr;

   QFile file(pathToHtml.c_str());

   QByteArray data;

   if (file.open(QIODevice::ReadOnly))
   {
       data = file.readAll();

       QString fileText(data);

       QString test8bit   = QString::fromLocal8Bit(data), // ���� � �������
               testUtf8   = QString::fromUtf8(data),      // ���� � ���������
               testLatin1 = QString::fromLatin1(data);

       /*qDebug() << "text: " << fileText
               << "\n fromLocal8bit: " << test8bit
               << "\n fromUtf8: "      << testUtf8
               << "\n fromLatin1: "    << testLatin1;* /

       qDebug() << "text: " << test8bit; qDebug() << "\n";

       htmlFileStr = test8bit.toStdString();

   }
   else
   {
       qWarning() << "Cannot open file";
       return res;
   }

   

   std::string::const_iterator htmlFileStr_it(htmlFileStr.begin());
   std::smatch match;

   //----------------��������� head----------------
   std::regex style_reg("<head>((?:[\\s\\S]*?)<style(?:[^>]*?)>([\\s\\S]*?)<\\/style>(?:[\\s\\S]*?))<\\/head>");
   
   if (res = std::regex_search(htmlFileStr, match, style_reg))
   {
      htmlFileStr_it += match.position() + match.length();
      std::string styles = match[2];
      
      std::regex styleRuleReg("\\#f(\\d+)[^\\{]*?\\{([^\\}]*?)\\}");
      
      for (
           std::string::const_iterator styleBegin(styles.begin()); 
           std::regex_search( styleBegin, styles.cend(), match, styleRuleReg );
           styleBegin += match.position() + match.length()
         )
      {         
         std::string styleRule = match[2];

         styleRules.resize(styleRules.size() + 1);
         styleRules.back().parse(styleRule);
      }

      //for (auto rule = styleRules.begin(); rule != styleRules.end(); ++rule)
      //{
      //   std::cout << "Rule [" << rule - styleRules.begin() << "]:\n" << rule->print().c_str();
      //}
   }

   //----------------��������� body----------------
   std::regex blockReg("<div([^>]*?)>([\\s\\S]*?)<\\/div>");
   
   for (
         htmlFileStr_it;
         std::regex_search(htmlFileStr_it, htmlFileStr.cend(), match, blockReg);
         htmlFileStr_it += match.position() + match.length()
      )
   {
      std::string attributes = match[1],
                  blockBody  = match[2];      

      //-------------���� �������� style-------------

      std::regex styleRuleReg("style=\"([^\"]*?)\"");
      std::smatch styleAttr;
      std::string blockStyleRules;

      if (std::regex_search(attributes, styleAttr, styleRuleReg))
      {
         blockStyleRules = styleAttr[1];
      }

      //-------------���� ���� �����-------------

      std::regex spanReg("<span ([^>]*?)>([\\s\\S]*?)<\\/span>");
      std::smatch spanMatch;

      // Span'�� �.�. ��������� � ����� �����. �� �� ����� ���� ���������
      for (
            std::string::const_iterator spanBegin(blockBody.begin());
            std::regex_search(spanBegin, blockBody.cend(), spanMatch, spanReg);
            spanBegin += spanMatch.position() + spanMatch.length()
         )
      {
         std::string spanAttributes = spanMatch[1],
                     spanBody       = spanMatch[2];
         
         document.back().resize(document.back().size() + 1); // ��������� ���� ������ �� ��������� ��������
         
         auto lastBlock = document.back().end() - 1;

         //------------���������� ����� � ����� ������������� div'�------------
         lastBlock->text = spanBody;
         lastBlock->style.parse(blockStyleRules);

         //------------������� id �����------------
         std::regex styleIdReg("id=\"f(\\d+)\"");
         std::smatch styleIdMatch;

         if (std::regex_search(spanAttributes, styleIdMatch, styleIdReg))
         {
            std::string styleId = styleIdMatch[1];
            if (styleId != "")
            {
               auto& requiredStyle = styleRules[atoi(styleId.c_str())];
               lastBlock->style.concat(requiredStyle);
            }
         }

         //------------�������� ����� span'�------------
         std::smatch styleMatch;
         if (std::regex_search(spanAttributes, styleMatch, styleRuleReg))
         {
            std::string spanStyleRules = styleMatch[1];
            lastBlock->style.parse(spanStyleRules);
         }
      }
   }
   

   return res;
}
*/