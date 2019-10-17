#pragma once
/*====================================================*\
XmlCreator.h - Заголовочный файл шаблонного класса XmlCreator

Данный класс предназначен для распознавание разпарсенных данных
и генерации xml-документа
\*====================================================*/

#include "../PdfParser/PdfParser.h"

#include <QXmlStreamWriter>

struct Author
{
   QString m_surnameRUS  = "",
           m_surnameENG  = "",
           m_initialsRUS = "",
           m_initialsENG = "",
           m_orgNameRUS  = QString::fromLocal8Bit("Московский технологический университет"),
           m_orgNameENG  = "Moscow Technological University",
           m_email       = "";
   bool isMainAuthor     = false;
};

struct Article
{
   QString m_section    = "",
           m_pages      = "",
           m_artType    = "RAR",
           m_artTitleRUS= "",
           m_artTitleENG= "",
           m_abstractRUS= "",
           m_abstractENG= "",
           m_text       = "",
           m_udk        = "";

   int     m_startPage  ,
           m_endPage    ;

   std::vector<Author>  m_authors;

   std::vector<QString> m_keywords,
                        m_references;
};

class XmlCreator : public PdfParser
{
   Q_OBJECT
public:
    XmlCreator(){};
   ~XmlCreator(){};

   virtual bool parse(const QString& pathToArticleDir) override;

   void generateXml() const;


   void test();


   using BlockPtr = std::shared_ptr < TextBlock > ;
   using Blocks   = std::vector < BlockPtr > ;

protected:
   void issueHandling(const Issue& issue);

   void udkHandling               (const BlockPtr block);
   void topRunningTitleHandling   (const Blocks& blocks);
   void bottomRunningTitleHandling(      Blocks& blocks);
   void headerHandling            (const Blocks& blocks, TextBlock::LANGUAGE lang);
   void headerRUSHandling         (const Blocks& blocks);
   void headerENGHandling         (const Blocks& blocks);
   void textHandling              (const Blocks& blocks);
   void referencesHandling        (const Blocks& blocks);

   void createOperCardTag  (QXmlStreamWriter& res)                                        const;
   void createIssueTag     (QXmlStreamWriter& res)                                        const;
   void createArticlesTag  (QXmlStreamWriter& res)                                        const;
   void createArticleTag   (QXmlStreamWriter& res, const Article& article)                const;
   void createAuthorTag    (QXmlStreamWriter& res, const Author& author  , const int num) const;
private: 
   QString m_pathToArticleDir;

   QString m_operator   = "Articulus_2687",
           m_pid        = "",
           m_date       = "",
           m_cntArticle = "",
           m_titleid    = "58030",
           m_issn       = "",
           m_eissn      = "2500-316X",
           m_title      = "",//QString::fromLocal8Bit("Российский технологический журнал"),
           m_volume     = "",
           m_number     = "",
           m_altNumber  = "",
           m_part       = "",
           m_dateUni    = "",
           m_issTitle   = "",
           m_pages      = "",
           m_file       = "";

   std::vector<Article> m_articles;
};