/*====================================================*\
XmlCreator.cpp - Файл реализации класса XmlCreator
\*====================================================*/

//=========================================
#include "XmlCreator.h"

#include <QFile>
#include <QDebug>
//=========================================

//==========================================
bool XmlCreator::parse(const QString& pathToArticleDir)
//==========================================
{
   bool res = false;

   m_pathToArticleDir = pathToArticleDir;

   if (res = __super::parse(pathToArticleDir))
   {
      for (auto& issue : document)
      {
         issueHandling(issue);
      }

      generateXml();
   }

   return res;
}

//==========================================
void XmlCreator::issueHandling(const Issue& issue)
//==========================================
{
   BlockPtr udk;
   Blocks topRunningTitle,
          bottomRunningTitle,
          headerRUS,
          headerENG,
          text,
          references;

   auto getBlockPtr = [](TextBlock& block) -> BlockPtr { return std::make_shared<TextBlock>(block); };

   enum BODY_PART
   {
      TITLE_RUS = 0,
      TITLE_ENG,
      TEXT,
      REFERENCES_RUS,
      REFERENCES_ENG,
      END
   };

   BODY_PART curPart = TITLE_RUS;

   auto changeCurPart = [&curPart](TextBlock& block)
   {
      switch (curPart)
      {
         case TITLE_RUS:
         {
            if (block.language == TextBlock::LANGUAGE::ENG)
            {
               curPart = TITLE_ENG;
            }

            break;
         }
         case TITLE_ENG:
         {
            if (block.language == TextBlock::LANGUAGE::RUS)
            {
               curPart = TEXT;
            }

            break;
         }
         case TEXT:
         {
            if (block.text == QString::fromLocal8Bit("Литература:") &&
               *block.style.fontWeight() == TextStyle::WEIGHT::BOLD
            )
            {
               curPart = REFERENCES_RUS;
            }

            break;
         }
         case REFERENCES_RUS:
         {
            if (block.text == "References:" &&
               *block.style.fontWeight() == TextStyle::WEIGHT::BOLD
            )
            {
               curPart = REFERENCES_ENG;
            }

            break;
         }
         case REFERENCES_ENG:
         {
            if (block.language == TextBlock::LANGUAGE::RUS)
            {
               curPart = END;
            }

            break;
         }
      }
   };

   const std::unordered_map<BODY_PART, Blocks&> blocksMap =
   {
      { TITLE_RUS      , headerRUS },
      { TITLE_ENG      , headerENG },
      { TEXT           , text      },
      { REFERENCES_RUS , references },
      { REFERENCES_ENG , references },
   };

   int  curPageNum = 1;
   auto curPage = issue.find(curPageNum);

   bool isUdk = true;

   for (auto block : curPage->second)
   {
      if (block.pageArea == TextBlock::PAGE_AREA::TOP_RUNNING_TITLE)
      {
         topRunningTitle.push_back(getBlockPtr(block));
      }
      else if (block.pageArea == TextBlock::PAGE_AREA::BOTTOM_RUNNING_TITLE)
      {
         bottomRunningTitle.push_back(getBlockPtr(block));
      }
      else
      {
         if (isUdk)
         {
            isUdk = false;
            udk = getBlockPtr(block);

            continue;
         }

         changeCurPart(block);

         if (curPart != END)
         {
            blocksMap.at(curPart).push_back(getBlockPtr(block));
         }
         else
         {
            break;
         }
      }
   }

   ++curPageNum;
   for (; curPageNum <= issue.size(); ++curPageNum)
   {
      curPage = issue.find(curPageNum);

      for (auto block : curPage->second)
      {
         if (block.pageArea != TextBlock::PAGE_AREA::BODY)
         {
            continue;
         }

         changeCurPart(block);

         if (curPart != END)
         {
            blocksMap.at(curPart).push_back(getBlockPtr(block));
         }
         else
         {
            break;
         }
         
      }
   }

   m_articles.resize(m_articles.size() + 1);

   udkHandling               (udk);
   topRunningTitleHandling   (topRunningTitle   );
   bottomRunningTitleHandling(bottomRunningTitle);
   headerRUSHandling         (headerRUS         );
   headerENGHandling         (headerENG         );
   textHandling              (text              );
   referencesHandling        (references        );

   if (m_articles.back().m_startPage != -1)
   {
      m_articles.back().m_endPage = m_articles.back().m_startPage + issue.size() - 1;

      m_articles.back().m_pages = QString::number(m_articles.back().m_startPage) + QString("-") + QString::number(m_articles.back().m_endPage);
   }
}

//==========================================
void XmlCreator::udkHandling(const BlockPtr block)
//==========================================
{
   QRegExp udkReg("УДК\\s(.*)");

   if (udkReg.indexIn(block->text.toLocal8Bit()) != -1)
   {
      m_articles.back().m_udk = udkReg.cap(1);
   }
}

//==========================================
void XmlCreator::topRunningTitleHandling(const Blocks& blocks)
//==========================================
{
   QString res;

   for (auto block : blocks)
   {
      res += (res.size() > 0 ? QString(" ") : QString("")) + block->text;
   }

   if ( !m_articles.back().m_section.size() )
   {
      m_articles.back().m_section = res;
   }
}

//==========================================
void XmlCreator::bottomRunningTitleHandling(Blocks& blocks)
//==========================================
{
   // Получение номера страницы
   int startPage = -1;
   if ( ( startPage = blocks.front()->text.toInt() ) != 0 )
   {
      blocks.erase(blocks.begin());
   }
   else if ( ( startPage = blocks.back()->text.toInt() ) != 0)
   {
      blocks.erase(blocks.end() - 1);
   }

   m_articles.back().m_startPage = startPage;

   // Получение названия журнала
   if (  *blocks.front()->style.fontStyle() == TextStyle::STYLE::ITALIC 
      && *blocks.front()->style.fontWeight() == TextStyle::WEIGHT::NORMAL
      && !m_title.size()
   )
   {
      m_title = blocks.front()->text;
      blocks.erase(blocks.begin());
   }

   // Получение строки вида ГГГГ Том Ч №Ч
   QString journalInfo = "";

   for (auto block : blocks)
   {
      journalInfo += block->text;
   }

   // Извлекаем год, номер тома и номер выпуска из полученной строки
   QRegExp infoReg("(\\d{4})\\sТом\\s(\\d)\\s№\\s(\\d)");

   if (infoReg.indexIn(journalInfo.toLocal8Bit()) != -1)
   {
      if (!m_date.size())
      {
         m_dateUni = infoReg.cap(1);
      }

      if (!m_volume.size())
      {
         m_volume = infoReg.cap(2);
      }

      if (!m_number.size())
      {
         m_number = infoReg.cap(3);
      }
   }
}

//==========================================
void XmlCreator::headerHandling(const Blocks& blocks, TextBlock::LANGUAGE lang)
//==========================================
{
   enum HEADER_PART
   {
      TITLE                = 0,
      AUTHORS              ,
      WORKPLACE_AND_EMAIL  ,
      ABSTRACT             ,
      KEY_WORD_TITLE       ,
      KEY_WORDS
   };

   HEADER_PART curPart = TITLE;

   auto changeCurPart = [lang, &curPart](TextBlock& block)
   {
      switch (curPart)
      {
         case TITLE:
         {
            if (block.m_register != TextBlock::REGISTER::UPPER)
            {
               curPart = AUTHORS;
            }

            break;
         }
         case AUTHORS:
         {
            if (*block.style.fontStyle() == TextStyle::STYLE::ITALIC)
            {
               curPart = WORKPLACE_AND_EMAIL;
            }

            break;
         }

         case WORKPLACE_AND_EMAIL:
         {
            if (*block.style.fontStyle() == TextStyle::STYLE::NORMAL)
            {
               curPart = ABSTRACT;
            }

            break;
         }
         case ABSTRACT:
         {
            if (  *block.style.fontStyle() == TextStyle::STYLE::ITALIC
               && block.text == ( lang == TextBlock::LANGUAGE::RUS ? QString::fromLocal8Bit("Ключевые слова: ") : "Keywords: " )
               )
            {
               curPart = KEY_WORD_TITLE;
            }

            break;
         }
         case KEY_WORD_TITLE:
         {
            curPart = KEY_WORDS;

            break;
         }

         case KEY_WORDS:
         {
            break;
         }
      }
   };

   auto setField = [lang](QString& value, QString& fieldRUS, QString& fieldENG)
   {
      if (lang == TextBlock::LANGUAGE::RUS)
      {
         fieldRUS = value;
      }
      else
      {
         fieldENG = value;
      }
   };

   Blocks title            ,
          authors          ,
          workplaceAndEmail,
          abstracts        ,
          keyWords         ;

   std::unordered_map<HEADER_PART, Blocks&> blocksMap =
   {
      { TITLE              , title             },
      { AUTHORS            , authors           },
      { WORKPLACE_AND_EMAIL, workplaceAndEmail },
      { ABSTRACT           , abstracts         },
      { KEY_WORDS          , keyWords          }
   };

   for (auto block : blocks)
   {
      changeCurPart(*block);

      if (curPart != KEY_WORD_TITLE)
      {
         blocksMap.at(curPart).push_back(block);
      }
   }

   // Title
   QString titleStr = "";
   for (auto block : title)
   {
      titleStr += (titleStr.size() > 0 ? QString(" ") : QString("")) + block->text;
   }
   setField(titleStr, m_articles.back().m_artTitleRUS, m_articles.back().m_artTitleENG);

   // Authors:
   for (int i = 0, othersCount = 0; i < authors.size(); ++i)
   {
      auto block = authors[i];
      
      if (block->language == lang && lang == TextBlock::LANGUAGE::RUS)
      {
         m_articles.back().m_authors.resize(i + 1);
      }
      else if (block->language == TextBlock::LANGUAGE::OTHER)
      {
         ++othersCount;
         continue;
      }

      auto curAuthor = ( lang == TextBlock::LANGUAGE::RUS ? ( m_articles.back().m_authors.end() - 1 ) : ( m_articles.back().m_authors.begin() + i - othersCount ) );

      if (block->language == TextBlock::LANGUAGE::OTHER && block->text == "@")
      {
         curAuthor->isMainAuthor = true;
      }

      QRegExp authorReg("(.+)\\.\\s(.+)");
      if (authorReg.indexIn(block->text.toLocal8Bit()) != -1)
      {
         QString initials = QString::fromUtf8(authorReg.cap(1).toStdString().c_str()),
                 surname  = QString::fromUtf8(authorReg.cap(2).toStdString().c_str());

         setField(initials, curAuthor->m_initialsRUS, curAuthor->m_initialsENG);
         setField(surname , curAuthor->m_surnameRUS , curAuthor->m_surnameENG );
      }
   }

   // workplaceAndEmail:
   // TO BE MADE...

   // Abstract:


   // KeyWords:
}

//==========================================
void XmlCreator::headerRUSHandling(const Blocks& blocks)
//==========================================
{
   headerHandling(blocks, TextBlock::LANGUAGE::RUS);
}

//==========================================
void XmlCreator::headerENGHandling(const Blocks& blocks)
//==========================================
{
   headerHandling(blocks, TextBlock::LANGUAGE::ENG);
}

//==========================================
void XmlCreator::textHandling(const Blocks& blocks)
//==========================================
{
   QString res;

   for (auto block : blocks)
   {
      res += (res.size() > 0 ? QString(" ") : QString("")) + block->text;
   }

   if (!m_articles.back().m_text.size())
   {
      m_articles.back().m_text = res;
   }
}

//==========================================
void XmlCreator::referencesHandling(const Blocks& blocks)
//==========================================
{
   for (auto block : blocks)
   {

   }
}

//==========================================
void XmlCreator::generateXml() const
//==========================================
{
   //QString pathToXml = "XmlCreator_test.xml"; // temp: Тут будет генерация имени, исходя из введённых данных!
   QString pathToXml = m_pathToArticleDir + QString("\\")
                     /*+ m_issn + QString("_")*/ 
                     + m_dateUni + QString("_")
                     + m_volume + QString("_") 
                     + m_number + QString("_unicode.xml");
   
   QFile   xmlFile(pathToXml);
   
   if (xmlFile.open(QIODevice::WriteOnly))
   {
      qDebug() << "Creating xml file: " << pathToXml;

      QXmlStreamWriter res(&xmlFile);

      res.setAutoFormatting(true);
      res.writeStartDocument("1.0", false);

      res.writeStartElement("journal");

      createOperCardTag(res);

      res.writeTextElement("titleid", m_titleid);
      res.writeTextElement("issn"   , m_issn);
      res.writeTextElement("eissn"  , m_eissn);

      res.writeStartElement("journalInfo"); 
         res.writeAttribute("lang", "RUS");
         res.writeTextElement("title", m_title);
      res.writeEndElement();

      createIssueTag(res);

      res.writeEndElement(); // journal

      res.writeEndDocument();

      qDebug() << "xml file created!";
   }
   else
   {
      qDebug() << "Cannot create file: " << pathToXml;
   }

   xmlFile.close();
}

//==========================================
void XmlCreator::createOperCardTag(QXmlStreamWriter& res) const
//==========================================
{
   res.writeStartElement("operCard");


   res.writeTextElement ("operator"    , m_operator);
   res.writeTextElement ("pid"         , m_pid);
   res.writeTextElement ("date"        , m_date);
   res.writeTextElement ("cntArticle"  , m_cntArticle);
   res.writeEmptyElement("cntNode");    
   res.writeTextElement ("cs"          , "0");

   res.writeEndElement(); // operCard
}

//==========================================
void XmlCreator::createIssueTag(QXmlStreamWriter& res) const
//==========================================
{
   res.writeStartElement("issue");

   res.writeTextElement("volume"    , m_volume);
   res.writeTextElement("number"    , m_number);
   res.writeTextElement("altNumber" , m_altNumber);
   res.writeTextElement("part"      , m_part);
   res.writeTextElement("dateUni"   , m_dateUni);
   res.writeTextElement("issTitle"  , m_issTitle);
   res.writeTextElement("pages"     , m_pages);

   createArticlesTag(res);

   res.writeEndElement(); // issue
}

//==========================================
void XmlCreator::createArticlesTag(QXmlStreamWriter& res) const
//==========================================
{
   res.writeStartElement("articles");

   for (auto& article : m_articles)
   {
      createArticleTag(res, article);
   }

   res.writeEndElement(); // articles
}

//==========================================
void XmlCreator::createArticleTag(QXmlStreamWriter& res, const Article& article) const
//==========================================
{
   res.writeStartElement("section");

      res.writeStartElement("secTitle"); 
         res.writeAttribute ("lang", "RUS"); 
         res.writeCharacters(article.m_section); 
      res.writeEndElement(); // secTitle

   res.writeEndElement(); // section


   res.writeStartElement("article");

      res.writeTextElement("pages"  , article.m_pages  );
      res.writeTextElement("artType", article.m_artType);

      res.writeStartElement("authors");
      for (int i = 0; i < article.m_authors.size(); ++i)
      {
         createAuthorTag(res, article.m_authors[i], i + 1);
      }
      res.writeEndElement(); // authors

      res.writeStartElement("artTitles");

         res.writeStartElement("artTitle");
            res.writeAttribute ("lang", "RUS"); 
            res.writeCharacters(article.m_artTitleRUS);
         res.writeEndElement(); // artTitle

         res.writeStartElement("artTitle");
            res.writeAttribute ("lang", "ENG"); 
            res.writeCharacters(article.m_artTitleENG);
         res.writeEndElement(); // artTitle;

      res.writeEndElement(); // artTitles

      res.writeStartElement("abstracts");

         res.writeStartElement("abstract");
            res.writeAttribute ("lang", "RUS"); 
            res.writeCharacters(article.m_abstractRUS);
         res.writeEndElement(); // abstract;

         res.writeStartElement("abstract");
            res.writeAttribute ("lang", "ENG"); 
            res.writeCharacters(article.m_abstractENG);
         res.writeEndElement(); // abstract;

      res.writeEndElement(); // abstracts

      res.writeStartElement("text");
         res.writeAttribute ("lang", "RUS");
         res.writeCharacters(article.m_text);
      res.writeEndElement(); // text

      res.writeStartElement("codes"); res.writeTextElement("udk", article.m_udk); res.writeEndElement();

      res.writeStartElement("keywords"); 
         res.writeStartElement("kwdGroup"); res.writeAttribute("lang", "ANY");
         for (auto& keyword : article.m_keywords)
         {
            res.writeTextElement("keyword", keyword);
         }
         res.writeEndElement(); // kwdGroup
      res.writeEndElement(); // keywords

      res.writeStartElement("references");
      for (auto& reference : article.m_references)
      {
         res.writeTextElement("reference", reference);
      }
      res.writeEndElement(); // references

      res.writeStartElement("files"); res.writeTextElement("file", m_file); res.writeEndElement(); 

   res.writeEndElement(); // article
}

//==========================================
void XmlCreator::createAuthorTag(QXmlStreamWriter& res, const Author& author, const int num) const
//==========================================
{
   res.writeStartElement("author");

   QString numStr = ( num < 100 ? QString("0") : QString("") ) + ( num < 10 ? QString("0") : QString("") ) + QString::number(num);

   res.writeAttribute("num", numStr);

   res.writeStartElement("individInfo"); res.writeAttribute("lang", "RUS");

      res.writeTextElement("surname"   , author.m_surnameRUS); 
      res.writeTextElement("initials"  , author.m_initialsRUS);
      res.writeTextElement("orgName"   , author.m_orgNameRUS); 
   
      if (author.m_email.size() > 0) res.writeTextElement("email", author.m_email);

   res.writeEndElement(); // individInfo

   res.writeStartElement("individInfo"); res.writeAttribute("lang", "ENG");

      res.writeTextElement("surname"   , author.m_surnameENG); 
      res.writeTextElement("initials"  , author.m_initialsENG);
      res.writeTextElement("orgName"   , author.m_orgNameENG); 

   res.writeEndElement(); // individInfo

   res.writeEndElement(); // author
}



//==========================================
void XmlCreator::test()
//==========================================
{
   m_articles.resize(1);

   auto article = m_articles.begin();
   article->m_authors.resize(2);

   article->m_keywords.push_back(QString::fromLocal8Bit("ключевое слово"));
   article->m_references.push_back(QString::fromLocal8Bit("1. Умная книжка"));

   generateXml();
}