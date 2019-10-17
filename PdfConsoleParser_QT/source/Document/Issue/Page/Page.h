#pragma once
/*====================================================*\
Page.h - ������������ ���� ������ Page

����� ��������� ��������, ������� �������� ����������� �� ������ ������
\*====================================================*/

#include "../../../Text/Block/TextBlock.h"

#include <algorithm>

class Page : public std::vector<TextBlock>
{
public:
   Page() {};

   void findRunningTitles();

   std::string print() 
   {
      uint32 blockId = 0;
      std::stringstream ss;
      ss << "Page: {\nBlocks: [\n";
      std::for_each(
         this->begin(), 
         this->end(), 
         [this, &blockId, &ss](TextBlock& block){ 
            ss << (blockId ? ", " : "") 
               << "[" << blockId << "]:" 
               << block.print() << "\n";
            ++blockId;
      });
      ss << "]\n";

      return ss.str();
   }
protected:
   void findTopRunningTitle();
   void findBottomRunningTitle();
};