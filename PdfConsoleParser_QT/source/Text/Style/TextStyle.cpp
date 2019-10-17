#pragma once
/*====================================================*\
TextType.cpp - Файл реализации класса TextType
\*====================================================*/

#include "stdafx.h"
#include "TextStyle.h"

//======================================
TextStyle::TextStyle() :
fontFamilyMap(
   {
      { "serif"      , FAMILY::SERIF     },
      { "sans-serif" , FAMILY::SANS_SERIF},
      { "cursive"    , FAMILY::CURSIVE   },
      { "fantasy"    , FAMILY::FANTASY   },
      { "monospace"  , FAMILY::MONOSPACE }
   }
),
fontWeightMap(
   {
      { "bold"    , WEIGHT::BOLD          },
      { "bolder"  , WEIGHT::BOLDER        },
      { "lighter" , WEIGHT::LIGHTER       },
      { "normal"  , WEIGHT::NORMAL        },
      { "100"     , WEIGHT::ONE_HUNDRED   },
      { "200"     , WEIGHT::TWO_HUNDRED   },
      { "300"     , WEIGHT::THREE_HUNDRED },
      { "400"     , WEIGHT::FOUR_HUNDRED  },
      { "500"     , WEIGHT::FIVE_HUNDRED  },
      { "600"     , WEIGHT::SIX_HUNDRED   },
      { "700"     , WEIGHT::SEVEN_HUNDRED },
      { "800"     , WEIGHT::EIGHT_HUNDRED },
      { "900"     , WEIGHT::NINE_HUNDRED  }
   }
),
fontStyleMap(
   {
      { "normal"  , STYLE::NORMAL },
      { "italic"  , STYLE::ITALIC },
      { "oblique" , STYLE::OBLIQUE }
   }
),
verticalAlignMap(
   {
      { "baseline"   , VERTICAL_ALIGN::BASELINE    },
      { "bottom"     , VERTICAL_ALIGN::BOTTOM      },
      { "middle"     , VERTICAL_ALIGN::MIDDLE      },
      { "sub"        , VERTICAL_ALIGN::SUB         },
      { "super"      , VERTICAL_ALIGN::SUPER       },
      { "text-bottom", VERTICAL_ALIGN::TEXT_BOTTOM },
      { "text-top"   , VERTICAL_ALIGN::TEXT_TOP    },
      { "top"        , VERTICAL_ALIGN::TOP         }
   }
),
settersMap(
   {
      { "font-family"    , &TextStyle::set_fontFamily    },
      { "font-weight"    , &TextStyle::set_fontWeight    },
      { "font-style"     , &TextStyle::set_fontStyle     },
      { "vertical-align" , &TextStyle::set_verticalAlign },
      { "left"           , &TextStyle::set_left          },
      { "top"            , &TextStyle::set_top           },
      { "font-size"      , &TextStyle::set_fontSize      },
      { "color"          , &TextStyle::set_color         }
   }
)
//======================================
{
   //settersMap.emplace("font-family"    , &TextStyle::set_fontFamily   );
   //settersMap.emplace("font-weight"    , &TextStyle::set_fontWeight   );
   //settersMap.emplace("font-style"     , &TextStyle::set_fontStyle    );
   //settersMap.emplace("vertical-align" , &TextStyle::set_verticalAlign);

   //settersMap.emplace("left"           , &TextStyle::set_left     );
   //settersMap.emplace("top"            , &TextStyle::set_top      );
   //settersMap.emplace("font-size"      , &TextStyle::set_fontSize );
   //settersMap.emplace("color"          , &TextStyle::set_color    );
}

//======================================
void TextStyle::set_fontFamily(const std::string &value)
//======================================
{
   FAMILY enumValue; get_fontFamilyEnum(enumValue, value);
   fontFamily(enumValue);
}

//======================================
void TextStyle::set_fontWeight(const std::string &value)
//======================================
{
   WEIGHT enumValue; get_fontWeightEnum(enumValue, value);
   fontWeight(enumValue);
}

//======================================
void TextStyle::set_fontStyle(const std::string &value)
//======================================
{
   STYLE enumValue; get_fontStyleEnum(enumValue, value);
   fontStyle(enumValue);
}

//======================================
void TextStyle::set_verticalAlign(const std::string &value)
//======================================
{
   VERTICAL_ALIGN enumValue; get_verticalAlignEnum(enumValue, value);
   verticalAlign(enumValue);
}

//======================================
void TextStyle::set_left(const std::string &value)
//======================================
{
   uint32 val = atoi(value.c_str());
   left(val);
}

//======================================
void TextStyle::set_top(const std::string &value)
//======================================
{
   uint32 val = atoi(value.c_str());
   top(val);
}

//======================================
void TextStyle::set_fontSize(const std::string &value)
//======================================
{
   uint32 val = atoi(value.c_str());
   fontSize(val);
}

//======================================
void TextStyle::set_color(const std::string &value)
//======================================
{
   std::regex colors_rgba("rgba\\((\\d+),(\\d+),(\\d+),(\\d+)\\)");
   std::smatch match;

   if (std::regex_search(value, match, colors_rgba))
   {
      std::vector<uint32> rgba_colors(4);
      for (uint32 i = 1; i < 4; ++i)
      {
         rgba_colors[i - 1] = atoi(match[i].str().c_str());
      }

      color.red   = rgba_colors[0];
      color.green = rgba_colors[1];
      color.blue  = rgba_colors[2];
      color.alpha = rgba_colors[3];
   }
}

//======================================
void TextStyle::concat(const TextStyle& _style)
//======================================
{
   compareFields<uint32>         (left          , _style.left           );
   compareFields<uint32>         (top           , _style.top            );
   compareFields<uint32>         (fontSize      , _style.fontSize       );

   compareFields<uint32>         (color.red     , _style.color.red      );
   compareFields<uint32>         (color.green   , _style.color.green    );
   compareFields<uint32>         (color.blue    , _style.color.blue     );
   compareFields<uint32>         (color.alpha   , _style.color.alpha    );

   compareFields<FAMILY>         (fontFamily    , _style.fontFamily     );
   compareFields<WEIGHT>         (fontWeight    , _style.fontWeight     );
   compareFields<STYLE>          (fontStyle     , _style.fontStyle      );
   compareFields<VERTICAL_ALIGN> (verticalAlign , _style.verticalAlign  );
}

//======================================
template<typename type>
void TextStyle::compareFields(MaybeType<type>& _old, const MaybeType<type>& _new)
//======================================
{
   if (_new.isValid())
   {
      _old = _new;
   }
}

//======================================
bool TextStyle::parse(std::string& rule)
//======================================
{
   bool res = true;

   std::regex ruleReg("([\\w\\-]+)\\:([^\\;]+)");
   std::smatch match;

   for (
      std::string::const_iterator ruleBegin(rule.begin());
      std::regex_search(ruleBegin, rule.cend(), match, ruleReg);
      ruleBegin += match.position() + match.length()
      )
   {
      std::string field = match[1],
                  value = match[2];

      auto setter = settersMap.find(field);
      if (setter != settersMap.end())
      {
         (this->*setter->second)(value);
      }

      //if (field == "font-family")
      //{
      //   set_fontFamily(value);
      //}
      //else if (field == "font-weight")
      //{
      //   set_fontWeight(value);
      //}
      //else if (field == "font-style")
      //{
      //   set_fontStyle(value);
      //}
      //else if (field == "left")
      //{
      //   set_left(value);
      //}
      //else if (field == "top")
      //{
      //   set_top(value);
      //}
      //else if (field == "font-size")
      //{
      //   set_fontSize(value);
      //}
      //else if (field == "vertical-align")
      //{
      //   set_verticalAlign(value);
      //}
      //else if (field == "color")
      //{
      //   set_color(value);
      //}
      //else
      //{
      //   res = false;
      //}
   }

   return res;
}

//======================================
std::string TextStyle::print()
//======================================
{
   std::stringstream res_ss;

   std::string fontFamilyStr;    get_fontFamilyStr(*fontFamily(), fontFamilyStr);
   std::string fontWeightStr;    get_fontWeightStr(*fontWeight(), fontWeightStr);
   std::string fontStyleStr;     get_fontStyleStr(*fontStyle(), fontStyleStr);
   std::string verticalAlignStr; get_verticalAlignStr(*verticalAlign(), verticalAlignStr);

   res_ss   << "Style: {\n"
               << "\t left: "       << left.print()
               << "\t top: "        << top.print()
               << "\t fontSize: "   << fontSize.print()
               << "\t color: {\n" 
                  << "\t\t color.red: "   << color.red.print()
                  << "\t\t color.green: " << color.green.print()
                  << "\t\t color.blue: "  << color.blue.print()
                  << "\t\t color.alpha: " << color.alpha.print()
               << "\t}\n"
               << "\t fontFamily: "    << fontFamilyStr     << std::endl
               << "\t fontWeight: "    << fontWeightStr     << std::endl
               << "\t fontStyle: "     << fontStyleStr      << std::endl
               << "\t verticalAlign: " << verticalAlignStr  << std::endl
            << "}\n";

   return res_ss.str();
}