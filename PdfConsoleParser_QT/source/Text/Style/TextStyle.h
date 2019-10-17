#pragma once
/*====================================================*\
TextType.h - Заголовочный файл класса TextType

Класс описывает стили блока текста
\*====================================================*/

#include "../../MaybeType/MaybeType.h"

#include <unordered_map>
#include <functional>

#define ENUMERABLE_STYLE_FIELD_DECLARATION(enumType, fieldName, ...)         \
public:                                                                                \
   MaybeType<enumType> fieldName;                                                      \
private:                                                                               \
   std::unordered_map<std::string, enumType> fieldName##Map;  /*__VA_ARGS__;*/         \
   void set_##fieldName##(const std::string& value);                                   \
public:                                                                                \
   bool get_##fieldName##Enum(enumType##& _enum, const std::string& _str)              \
   {                                                                                   \
      bool res = false;                                                                \
      auto resEnum = fieldName##Map.find(_str);                                        \
      if (res = (resEnum != fieldName##Map.end()))                                     \
      {                                                                                \
         _enum = resEnum->second;                                                      \
      }                                                                                \
      return res;                                                                      \
   };                                                                                  \
   bool get_##fieldName##Str(const enumType _enum, std::string &_str)                  \
   {                                                                                   \
      bool res = false;                                                                \
                                                                                       \
      for (auto pair = fieldName##Map.begin(); pair != fieldName##Map.end(); ++pair)   \
      {                                                                                \
         if (pair->second == _enum)                                                    \
         {                                                                             \
            _str = pair->first;                                                        \
            res = true;                                                                \
         }                                                                             \
      }                                                                               \
                                                                                       \
      return res;                                                                      \
   }                                                                                   

#define STYLE_FIELD_DECLARATION(fieldType, fieldName) \
public:                                               \
   fieldType fieldName;                               \
private:                                              \
   void set_##fieldName##(const std::string& value);                                   

using uint32 = unsigned int;


struct RGB_color
{
   MaybeType<uint32>  red
                    , green
                    , blue
                    , alpha;
};

class TextStyle
{
   template<typename> friend class MaybeType;

   using SetterFunction = void(TextStyle::*)(const std::string&);

   std::unordered_map<std::string, SetterFunction> settersMap;

public:
   TextStyle();
   ~TextStyle() {};

   enum class FAMILY
   {
        SERIF = 0
      , SANS_SERIF
      , CURSIVE
      , FANTASY
      , MONOSPACE
   };
   enum class WEIGHT
   {
        BOLD = 0
      , BOLDER
      , LIGHTER
      , NORMAL
      , ONE_HUNDRED     = 100
      , TWO_HUNDRED     = 200
      , THREE_HUNDRED   = 300
      , FOUR_HUNDRED    = 400
      , FIVE_HUNDRED    = 500
      , SIX_HUNDRED     = 600
      , SEVEN_HUNDRED   = 700
      , EIGHT_HUNDRED   = 800
      , NINE_HUNDRED    = 900
   };
   enum class STYLE
   {
        NORMAL = 0
      , ITALIC
      , OBLIQUE
   };
   enum class VERTICAL_ALIGN
   {
        BASELINE = 0
      , BOTTOM
      , MIDDLE
      , SUB
      , SUPER
      , TEXT_BOTTOM
      , TEXT_TOP
      , TOP
   };
   
   STYLE_FIELD_DECLARATION(MaybeType<uint32>, left    )
   STYLE_FIELD_DECLARATION(MaybeType<uint32>, top     )
   STYLE_FIELD_DECLARATION(MaybeType<uint32>, fontSize)
   STYLE_FIELD_DECLARATION(RGB_color        , color   )

   ENUMERABLE_STYLE_FIELD_DECLARATION(FAMILY, fontFamily,
      {
         { "serif"      , FAMILY::SERIF     },
         { "sans-serif" , FAMILY::SANS_SERIF},
         { "cursive"    , FAMILY::CURSIVE   },
         { "fantasy"    , FAMILY::FANTASY   },
         { "monospace"  , FAMILY::MONOSPACE }
      }
   )



   ENUMERABLE_STYLE_FIELD_DECLARATION(WEIGHT, fontWeight,
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
   )

   ENUMERABLE_STYLE_FIELD_DECLARATION(STYLE, fontStyle,
      {
         { "normal"  , STYLE::NORMAL  },
         { "italic"  , STYLE::ITALIC  },
         { "oblique" , STYLE::OBLIQUE }
      }
   )

   ENUMERABLE_STYLE_FIELD_DECLARATION(VERTICAL_ALIGN, verticalAlign,
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
   )


   void concat(const TextStyle& _style);
private:
   template<typename type>
   void compareFields(MaybeType<type>& _old, const MaybeType<type>& _new);
public:
   bool parse(std::string& rule);

public:
   std::string print();
};