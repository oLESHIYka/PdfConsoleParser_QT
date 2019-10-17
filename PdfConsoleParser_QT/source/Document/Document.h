#pragma once
/*====================================================*\
Document.h - Заголовочный файл класса Document

Класс описывает документ, который является контейнером из статей
По сути это Выпуск журнала
\*====================================================*/

#include "Issue/Issue.h"

using Document = std::vector<Issue>;

//class Document : public std::vector<Issue>
//{
//public:
//   Document() {};
//
//   
//};