/*====================================================*\
Page.cpp - Файл реализации класса Page
\*====================================================*/

#include "stdafx.h"
#include "Page.h"

#include <QDebug>
#include <cstdlib>

const unsigned int c_maxLineIntrval = 5;

//======================================
void Page::findRunningTitles()
//======================================
{
   if (size() < 2)
   {
      qWarning() << "Too few blocks in page!";
      return;
   }

   findTopRunningTitle();

   findBottomRunningTitle();
}

//======================================
void Page::findTopRunningTitle()
//======================================
{
   auto prevBlock = begin(),
        curBlock  = prevBlock + 1;

   prevBlock->pageArea = TextBlock::PAGE_AREA::TOP_RUNNING_TITLE;

   for (; curBlock != end(); ++prevBlock, ++curBlock)
   {
      int curBlockTop       = *curBlock->style.top(),
          prevBlockFontSize = *prevBlock->style.fontSize(),
          prevBlockTop      = *prevBlock->style.top();

      if ( (std::abs(curBlockTop - prevBlockTop) < c_maxLineIntrval) || ( curBlockTop - ( prevBlockTop + prevBlockFontSize ) < c_maxLineIntrval ) )
      {
         curBlock->pageArea = TextBlock::PAGE_AREA::TOP_RUNNING_TITLE;
      }
      else
      {
         break;
      }
   }
}

//======================================
void Page::findBottomRunningTitle()
//======================================
{
   auto prevBlock = end() - 1,
        curBlock  = prevBlock - 1;

   prevBlock->pageArea = TextBlock::PAGE_AREA::BOTTOM_RUNNING_TITLE;

   for (; prevBlock != begin(); --prevBlock, --curBlock)
   {
      int curBlockTop      = *curBlock->style.top(),
          curBlockFontSize = *curBlock->style.fontSize(),
          prevBlockTop     = *prevBlock->style.top();

      if ( (std::abs(curBlockTop - prevBlockTop) < c_maxLineIntrval) || ( prevBlockTop - ( curBlockTop + curBlockFontSize ) < c_maxLineIntrval ) )
      {
         curBlock->pageArea = TextBlock::PAGE_AREA::BOTTOM_RUNNING_TITLE;
      }
      else
      {
         break;
      }
   }
}