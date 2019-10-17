#pragma once
/*====================================================*\
PdfParser.h - Заголовочный файл шаблонного класса PdfParser

Класс отвечает за парсинг pdf документов.
Парсинг происходит в 2 этапа: 
   1. Постраничное преобразование pdf файла в html файлы
   2. Парсинг полученных html файлов
\*====================================================*/

#include "../Document/Document.h"
#include <QObject>
#include <QMap>

//struct Attribute
//{
//   QString name, value;
//};

//using Attribute = std::unordered_map < QString, QString > ;
using Attribute = QMap < QString, QString >;

struct Tag
{
   Tag(){};

   QString tagName, all, body;
   Attribute attrs;

   void print()
   {
      QString res;

      res += "{\n   tagName: \"" + tagName + "\""
          +  ",\n   all: \""     + all     + "\""
          +  ",\n   body: \""    + body    + "\""
          +  ",\n   attrs: [";

      for (auto attr = attrs.begin(); attr != attrs.end(); ++attr)
      {
         res += "\"{ name: \"" + attr.key() + "\", value: \"" + attr.value() + "\"}, ";
      }

      res += "]\n}";

      //qDebug() << res.toStdString().c_str();
   }
};

class PdfParser : public QObject
{
   Q_OBJECT
public:
   //friend class Document;

   PdfParser() {};

   virtual bool parse(const QString& pathToArticleDir);
public:
   using StylesMap = std::map<QString, TextStyle>;
protected:
   bool parseTag(const QString& tagName, const QString& htmlStr, int& offset, Tag& tag);
   bool readConfig();

   bool parseBody(const QString& htmlStr, const StylesMap& stylesMap, Page& page);
   bool parseHead(const QString& htmlStr,       StylesMap& stylesMap);

   bool parsePdf         (const QString& pathToPdf, const int& issueNum, Issue& issue);   
   bool parseAllHtmlFiles(const QString& pathToDir, Issue& issue);
   bool parseHtml        (const QString& pathToHtml, Page& page);

private:
   //QTextStream print() const;
   void debugPrint() const;
protected:
   Document document;

   QString m_pathToXPdf,
           m_pathToTempDir;

public:
   //std::string print() { return document.print(); }
};