
#include <QtCore/QCoreApplication>
#include <qdebug.h>

//#include "PdfParser/PdfParser.h"
#include "XmlCreator/XmlCreator.h"

#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QFile>
#include <QRegExp>
#include <QDir>
#include <QStringList>
#include <QProcess>
#include <QDateTime>
#include <QXmlStreamWriter>

//struct Attribute
//{
//   QString name, value;
//};
//
//struct Tag
//{
//   Tag(){};
//
//   QString tag, all, body;
//   std::vector< Attribute> attrs;
//
//   void print()
//   {
//      QString res;
//
//      res += "{\n   tag: \""  + tag  + "\""
//          + ",\n   all: \""   + all  + "\""
//          + ",\n   body: \""  + body + "\""
//          + ",\n   attrs: [";
//
//      for (auto& attr : attrs)
//      {
//         res += "\"{ name: \"" + attr.name + "\", value: \"" + attr.value + "\"}, ";
//      }
//
//      res += "]\n}";
//
//      qDebug() << res.toStdString().c_str();
//   }
//};

//Tag parseTag(const QString& str, const QString& tag, int& offset)
//{
//   Tag res;
//
//   const QString regStr = "<" + tag + "\\s*([^>]*)>([\\s\\S]*)<\\/" + tag + ">";
//   QRegExp reg(regStr);
//   reg.setMinimal(true);
//
//   if ((offset = reg.indexIn(str, offset)) != -1)
//   {
//      res.tag  = tag;
//      res.all  = reg.cap(0);
//      res.body = reg.cap(2);
//      
//      QString attrs = reg.cap(1);
//      if (attrs.size())
//      {
//         QRegExp attrReg("(\\w+)=\"([^\"]*)\"");
//         attrReg.setMinimal(true);
//         int pos = 0;
//         
//         while ((pos = attrReg.indexIn(attrs, pos)) != -1)
//         {
//            Attribute attr;
//            attr.name  = attrReg.cap(1);
//            attr.value = attrReg.cap(2);
//
//            res.attrs.push_back(attr);
//
//            pos += attrReg.matchedLength();
//         }
//      }
//
//      offset += reg.matchedLength();
//   }
//
//   return res;
//}

int main(int argc, char *argv[])
{
   //==============================================================================
   // Main:
    system("chcp 65001");

    QCoreApplication a(argc, argv);

    XmlCreator par;

    std::string issuesPath;
    std::cout << "Enter path to issues directory: ";
    std::cin >> issuesPath;

    par.parse(issuesPath.c_str());
    
    //par.test();
    //==============================================================================

    //==============================================================================
    // read and write files
    /*std::string htmlFileStr,
                pathToHtml = "test.txt";

    QFile file(pathToHtml.c_str());

    QByteArray data;

    if (file.open(QIODevice::ReadOnly))
    {
       data = file.readAll();

       QString fileText(data);

       QString test8bit   = QString::fromLocal8Bit(data), // норм в консоли
               testUtf8   = QString::fromUtf8(data),      // норм в отладчике // норм при записи!
               testLatin1 = QString::fromLatin1(data);

       qDebug() << "text: "             << fileText
                << "\n fromLocal8bit: " << test8bit
                << "\n fromUtf8: "      << testUtf8
                << "\n fromLatin1: "    << testLatin1;

       //qDebug() << "text: " << test8bit; qDebug() << "\n";

       htmlFileStr = test8bit.toStdString();
       std::cout << "htmlFileStr: " << htmlFileStr.c_str() << std::endl;

       QFile output("write_test.txt");
       if (output.open(QIODevice::WriteOnly | QIODevice::Text))
       {
          QTextStream outputStream(&output);
          outputStream.setCodec("UTF-8");
          outputStream << test8bit   << "\n"
                       << testUtf8   << "\n"
                       << testLatin1 << "\n";

          outputStream.setGenerateByteOrderMark(true);
       }
       else
       {
          qWarning() << "Cannot create file";
       }
    }
    else
    {
       qWarning() << "Cannot open file";
    }*/
    //==============================================================================

    //==============================================================================
    // RegExp:
    /*QRegExp rx("(\\d+)");
    QString str = "Offsets: 12 14 99 231 7";
    QStringList list;
    int pos = 0;

    while ((pos = rx.indexIn(str, pos)) != -1) {
       list << rx.cap(1);
       pos += rx.matchedLength();
    }*/
    //==============================================================================
    // RegExp 2:
    /*QString html = "<style type=\"text / css\">\
       .txt{ white - space:nowrap; }\
    #f0{ font - family:sans - serif; font - weight:normal; font - style:normal; }\
    #f1{ font - family:monospace; font - weight:normal; font - style:normal; }\
    #f2{ font - family:serif; font - weight:normal; font - style:italic; }\
    #f3{ font - family:serif; font - weight:bold; font - style:italic; }\
    #f4{ font - family:serif; font - weight:bold; font - style:italic; }\
    #f5{ font - family:serif; font - weight:normal; font - style:italic; }\
    #f6{ font - family:serif; font - weight:bold; font - style:normal; }\
    #f7{ font - family:serif; font - weight:normal; font - style:normal; }\
    #f8{ font - family:serif; font - weight:normal; font - style:normal; }\
    #f9{ font - family:serif; font - weight:normal; font - style:italic; }\
    #f10{ font - family:serif; font - weight:normal; font - style:normal; }\
    #f11{ font - family:serif; font - weight:normal; font - style:italic; }\
    #f12{ font - family:sans - serif; font - weight:normal; font - style:normal; }\
       </style>",
      html2 = "<div class=\"txt\" style=\"position:absolute; left:116px; top:28px;\">\
               	<span id=\"f4\" style=\"font-size:14px;vertical-align:baseline;color:rgba(35,31,31,1);\">ИНФОРМАЦИОННЫЕ СИСТЕМЫ. ИНФОРМАТИКА.</span>\
               </div>\
               <div class=\"txt\" style=\"position:absolute; left:112px; top:45px;\">\
               	<span id=\"f4\" style=\"font-size:14px;vertical-align:baseline;color:rgba(35,31,31,1);\">ПРОБЛЕМЫ ИНФОРМАЦИОННОЙ БЕЗОПАСНОСТИ</span>\
               </div>";

    int offset = 0;
    //auto tag = parseTag(html, "style");
    //tag.print();

    qDebug() << "\n\n";

    while (offset != -1)
    {
       auto tag = parseTag(html2, "div", offset);
       tag.print();
    }

    //QString attrStr = "id=\"f4\" style=\"font-size:14px;vertical-align:baseline;color:rgba(35,31,31,1);\"";

    //QRegExp attrReg("(\\w+)=\"([^\"]*)\"");
    //attrReg.setMinimal(true);
    //int pos = 0;

    //while ((pos = attrReg.indexIn(attrStr, pos)) != -1)
    //{
    //   QString res = attrReg.cap(0);
    //   qDebug() << res;

    //   Attribute attr;
    //   attr.name = attrReg.cap(1);
    //   attr.value = attrReg.cap(2);

    //   pos += attrReg.matchedLength();

    //   //res.attrs.push_back(attr);
    //}*/
    //==============================================================================

    //==============================================================================
    // Read html and parse it
    /*std::string htmlFileStr,
       pathToHtml = "page1_test.html";

    QFile file(pathToHtml.c_str());

    QByteArray data;

    if (file.open(QIODevice::ReadOnly))
    {
       data = file.readAll();

       QString fileText(data);

       QString test8bit   = QString::fromLocal8Bit(data), // норм в консоли
               testUtf8   = QString::fromUtf8(data),      // норм в отладчике // норм при записи!
               testLatin1 = QString::fromLatin1(data);

       qDebug() << "text: " << fileText
                << "\n fromLocal8bit: " << test8bit
                << "\n fromUtf8: " << testUtf8
                << "\n fromLatin1: " << testLatin1;

       //qDebug() << "text: " << test8bit; qDebug() << "\n";

       htmlFileStr = test8bit.toStdString();

       qDebug() << "\n================================\n";

       int offset = 0;

       auto styleTag = parseTag(fileText, "style", offset);
       styleTag.print();

       while (offset != -1)
       {
          auto tag = parseTag(fileText, "div", offset);
          tag.print();
       }
    }
    else
    {
       qWarning() << "Cannot open file \"" << pathToHtml.c_str() << "\"";
    }*/
    //==============================================================================

    //==============================================================================
    // Read all files form dir
    /*QDir dir("F:\\VS\\Diplom\\PdfConsoleParser\\PdfConsoleParser\\test_files\\3-12");
    
    QStringList filters; filters << "page*.html";
    
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

    for (auto& file : files)
    {
       QRegExp fileNumberReg("page(\\d+)\\.html");

       if (fileNumberReg.indexIn(file.fileName()) != -1)
       {
          int num = fileNumberReg.cap(1).toInt();

          qDebug() << file.absoluteFilePath() << "\n\t number: " << num;
       }
    }*/

    //============================================================================== 
    /*QRegExp styleRuleReg("(\\#f\\d+)[^\\{]*\\{([^\\}]*)\\}");
    QString rules = ".txt { white-space:nowrap; }\
       #f0{ font-family:sans-serif; font-weight:normal; font-style:normal; }\
       #f1{ font-family:monospace; font-weight:normal; font-style:normal; }\
       #f2{ font-family:serif; font-weight:normal; font-style:italic; }\
       #f3{ font-family:serif; font-weight:bold; font-style:italic; }\
       #f4{ font-family:serif; font-weight:bold; font-style:italic; }\
       #f5{ font-family:serif; font-weight:normal; font-style:italic; }\
       #f6{ font-family:serif; font-weight:bold; font-style:normal; }\
       #f7{ font-family:serif; font-weight:normal; font-style:normal; }\
       #f8{ font-family:serif; font-weight:normal; font-style:normal; }\
       #f9{ font-family:serif; font-weight:normal; font-style:italic; }\
       #f10{ font-family:serif; font-weight:normal; font-style:normal; }\
       #f11{ font-family:serif; font-weight:normal; font-style:italic; }\
       #f12{ font-family:sans-serif; font-weight:normal; font-style:normal; }";
    int offset = 0;

    while ((offset = styleRuleReg.indexIn(rules, offset)) != -1)
    {
       QString ruleId = styleRuleReg.cap(1);
       QString rule   = styleRuleReg.cap(2);

       offset += styleRuleReg.matchedLength();

       qDebug() << "ruleId: " << ruleId << ", rule: " << rule;
    }*/
    //==============================================================================
    // XPdf test:
    //QString pathToXPdf, pathToTempDir;
    //QStringList arguments;
    //
    //QFile cfgFile("cfg/path.cfg");
    //if (cfgFile.open(QIODevice::ReadOnly | QIODevice::Text))
    //{
    //   QByteArray line;
    //   
    //   pathToXPdf    = cfgFile.readLine();
    //   pathToTempDir = cfgFile.readLine();
    //
    //   QRegExp newLine("\\n$");
    //   pathToXPdf   .replace(newLine, "");
    //   pathToTempDir.replace(newLine, "");
    //
    //   qDebug() << "pathToXPdf:"     << pathToXPdf    
    //            << "\npathToTempDir:"  << pathToTempDir;
    //
    //   if (QFile::exists(pathToXPdf))
    //   {
    //      qDebug() << pathToXPdf << " exists!";
    //   }
    //
    //   QDir tempDir(pathToTempDir);
    //   if (tempDir.exists())
    //   {
    //      qDebug() << pathToTempDir << " exists!";
    //
    //      QDateTime curTime = QDateTime::currentDateTime();
    //      QString curTimestr = curTime.toString("yyyy_MM_dd__hh_mm_ss");
    //
    //      tempDir.mkdir(curTimestr);
    //
    //      //qstring pathtopdf;
    //
    //      //std::cout << "enter path to pdf!\n";
    //      //qfile file;
    //      //file.open(stdin, qiodevice::readonly);
    //      //qtextstream qtin(&file);
    //      //qtin >> pathtopdf;
    //
    //      //qdatetime curtime  = qdatetime::currentdatetime();
    //      //qstring curtimestr = curtime.tostring("yyyy_mm_dd__hh_mm_ss");
    //
    //      //pathtotempdir += qstring("/") + curtimestr;
    //
    //      //qstring command = qstring("\"") + pathtoxpdf + qstring("\"") 
    //      //                + qstring(" ") /*+ qstring("-q")*/ + pathtopdf + qstring(" ") + pathtotempdir;
    //
    //      //system(command.tostdstring().c_str());
    //   }
    //}

    //==============================================================================
    /*QFile file("test.txt");

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
       QByteArray data;
       data = file.readAll();

       QRegExp reg("^[А-я]+");
       //QRegExp reg("^[A-z]+");
       QString str = QString::fromUtf8(data);

       qDebug() << QString::fromLocal8Bit(data);

       if (reg.indexIn(str) != -1)
       {
          qDebug() << "IT WORKS with str!";
       }

       if (reg.indexIn(str.toUtf8()) != -1)
       {
          qDebug() << "IT WORKS Utf8!";
       }

       if (reg.indexIn(str.toLatin1()) != -1)
       {
          qDebug() << "IT WORKS Latin1!";
       }

       if (reg.indexIn(str.toLocal8Bit()) != -1)
       {
          qDebug() << "IT WORKS Local8Bit!";
       }
    }*/
    //==============================================================================
    /*std::vector<int> t{1,2,3};
    for (auto prev = t.end() - 1, cur = prev - 1; prev != t.begin(); --cur, --prev)
    {
       std::cout << *cur << " ";
    }
    std::cout << std::endl;*/
    //==============================================================================
    /*QFile xmlFile("test.xml");
    if (xmlFile.open(QIODevice::WriteOnly))
    {
       QXmlStreamWriter test(&xmlFile);

       test.setAutoFormatting(true);
       test.writeStartDocument("1.0", false);

       test.writeStartElement("journal");

       {
          test.writeStartElement("text");
            test.writeAttribute("lang", "RUS");
            test.writeCharacters(QString::fromLocal8Bit("Тестовая сторока, проверка связи!!!"));
          test.writeEndElement();

          std::vector<int> nums = { 1, 2, 3, 4, 5, 6 };

          test.writeStartElement("nums");
          for (int i = 0; i < nums.size(); ++i)
          {
             test.writeStartElement("id"); 
               test.writeCharacters(QString::number(i));
             test.writeEndElement();

             test.writeStartElement("num"); 
               test.writeCharacters(QString::number(nums[i]));
             test.writeEndElement();
          }
          test.writeEndElement();
       }

       test.writeEndElement();

       test.writeEndDocument();
    }
    xmlFile.close();*/
    //==============================================================================
    //QRegExp lowerReg("^[а-яa-z]+"),
    //        upperReg("^[А-ЯA-Z]+");

    ////QString test = "UDK 621.396.99";

    //QFile file("test.txt");

    //if (file.open(QFile::ReadOnly | QFile::Text))
    //{
    //   QByteArray data;
    //   data = file.readAll();

    //   QString str = QString::fromUtf8(data);

    //   if (str == str.toLower())
    //   {
    //      qDebug() << "LOWER!";
    //   }
    //   else if (str == str.toUpper())
    //   {
    //      qDebug() << "UPPER!";
    //   }
    //   else
    //   {
    //      qDebug() << "OTHER";
    //   }
    //}
    //==============================================================================
    qDebug() << "END OF PROGRAM";
    return a.exec();
}
