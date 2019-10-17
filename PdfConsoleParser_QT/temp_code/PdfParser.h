#pragma once
/*====================================================*\
PdfParser.h - ������������ ���� ���������� ������ PdfParser

����� �������� �� ������� pdf ����������.
������� ���������� � 2 �����: 
   1. ������������ �������������� pdf ����� � html �����
   2. ������� ���������� html ������
\*====================================================*/

#include "../Document/Document.h"
#include <QObject>

struct Attribute
{
   QString name, value;
};

struct Tag
{
   Tag(){};

   QString tagName, all, body;
   std::vector< Attribute> attrs;

   void print()
   {
      QString res;

      res += "{\n   tagName: \""  + tagName  + "\""
          +  ",\n   all: \""  + all  + "\""
          +  ",\n   body: \"" + body + "\""
          +  ",\n   attrs: [";

      for (auto& attr : attrs)
      {
         res += "\"{ name: \"" + attr.name + "\", value: \"" + attr.value + "\"}, ";
      }

      res += "]\n}";

      qDebug() << res.toStdString().c_str();
   }
};

class PdfParser : public QObject
{
   Q_OBJECT
public:
   friend class Document;

   PdfParser() {};

   bool parse(const std::string& pathToPdf);
public:
    using StylesMap = std::map<int, TextStyle>;
protected:
    bool parseTag (const QString& tagName, const QString& htmlStr, int& offset, Tag& tag);

    bool parseBody(const QString& htmlStr, const StylesMap& stylesMap);
    bool parseHead(const QString& htmlStr,       StylesMap& stylesMap);

   bool parseAllHtmlFiles(const std::string& pathToDir);
   //bool parseHtml(const std::string& pathToHtml);
   bool parseHtml(const QString& pathToHtml);

private:
   Document document;

public:
   std::string print() { return document.print(); }
};