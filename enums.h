/*! \file
    \brief Autogenerated enumerations (Umba Enum Gen)
 */

#pragma once


#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flag_ops.h"
#include "marty_cpp/marty_flags.h"

#include <exception>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>



namespace marty{
namespace format{

//#!FormatIndexType
enum class FormatIndexType : std::uint32_t
{
    unknown        = (std::uint32_t)(-1) /*!<  */,
    invalid        = (std::uint32_t)(-1) /*!<  */,
    argId          = 0x00 /*!<  */,
    fillRef        = 0x01 /*!<  */,
    widthRef       = 0x02 /*!<  */,
    precisionRef   = 0x03 /*!<  */,
    filterRef      = 0x04 /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(FormatIndexType)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( FormatIndexType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FormatIndexType::unknown        , "Unknown"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FormatIndexType::argId          , "ArgId"        );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FormatIndexType::fillRef        , "FillRef"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FormatIndexType::filterRef      , "FilterRef"    );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FormatIndexType::precisionRef   , "PrecisionRef" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( FormatIndexType::widthRef       , "WidthRef"     );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( FormatIndexType, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( FormatIndexType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::unknown        , "unknown"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::unknown        , "invalid"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::argId          , "arg-id"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::argId          , "arg_id"        );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::argId          , "argid"         );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::fillRef        , "fill-ref"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::fillRef        , "fill_ref"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::fillRef        , "fillref"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::filterRef      , "filter-ref"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::filterRef      , "filter_ref"    );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::filterRef      , "filterref"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::precisionRef   , "precision-ref" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::precisionRef   , "precision_ref" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::precisionRef   , "precisionref"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::widthRef       , "width-ref"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::widthRef       , "width_ref"     );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( FormatIndexType::widthRef       , "widthref"      );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( FormatIndexType, std::map, 1 )


//#!StdFilterType
enum class StdFilterType : std::uint32_t
{
    unknown    = (std::uint32_t)(-1) /*!<  */,
    invalid    = (std::uint32_t)(-1) /*!<  */,
    none       = 0x00 /*!<  */,
    xml        = 0x01 /*!<  */,
    xmlText    = 0x02 /*!<  */,
    xmlAttr    = 0x03 /*!<  */,
    html       = 0x04 /*!<  */,
    htmlText   = 0x05 /*!<  */,
    htmlAttr   = 0x06 /*!<  */,
    sql        = 0x07 /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(StdFilterType)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( StdFilterType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::unknown    , "Unknown"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::htmlAttr   , "HtmlAttr" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::none       , "None"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::xml        , "Xml"      );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::xmlAttr    , "XmlAttr"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::xmlText    , "XmlText"  );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::html       , "Html"     );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::htmlText   , "HtmlText" );
    MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( StdFilterType::sql        , "Sql"      );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( StdFilterType, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( StdFilterType, std::map, 1 )
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::unknown    , "unknown"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::unknown    , "invalid"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::htmlAttr   , "html-attr" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::htmlAttr   , "html_attr" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::htmlAttr   , "htmlattr"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::none       , "none"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::xml        , "xml"       );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::xmlAttr    , "xml-attr"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::xmlAttr    , "xml_attr"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::xmlAttr    , "xmlattr"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::xmlText    , "xml-text"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::xmlText    , "xml_text"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::xmlText    , "xmltext"   );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::html       , "html"      );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::htmlText   , "html-text" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::htmlText   , "html_text" );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::htmlText   , "htmltext"  );
    MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( StdFilterType::sql        , "sql"       );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( StdFilterType, std::map, 1 )


//#!FormattingOptionsFlags
enum class FormattingOptionsFlags : std::uint32_t
{
    unknown                   = (std::uint32_t)(-1) /*!<  */,
    invalid                   = (std::uint32_t)(-1) /*!<  */,
    none                      = 0x00 /*!<  */,
    argIdTaken                = 0x01 /*!< fieldWidth value is set */,
    fieldWidthTaken           = 0x02 /*!< fieldWidth value is set */,
    fieldWidthIndirect        = 0x04 /*!< Indirect field width - fieldWidth member contains index of argument with actual fieldWidth value */,
    signPlus                  = 0x08 /*!< Mutually exclusive with signMinus & signSpace */,
    signMinus                 = 0x10 /*!< Mutually exclusive with signPlus & signSpace */,
    signSpace                 = 0x20 /*!< Mutually exclusive with signMinus & signPlus */,
    signZ                     = 0x40 /*!<  */,
    signAlterForm             = 0x80 /*!<  */,
    signZero                  = 0x100 /*!<  */,
    fillingTaken              = 0x200 /*!<  */,
    fillingIndirect           = 0x400 /*!<  */,
    localeFormatting          = 0x800 /*!<  */,
    precisionTaken            = 0x1000 /*!<  */,
    precisionIndirect         = 0x2000 /*!<  */,
    grouppingTaken            = 0x4000 /*!<  */,
    caseInvert                = 0x8000 /*!<  */,
    fieldWidthIndirectTaken   = fieldWidthTaken | fieldWidthIndirect /*!<  */,
    precisionIndirectTaken    = precisionTaken | precisionIndirect /*!<  */,
    fillingIndirectTaken      = fillingTaken | fillingIndirect /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_FLAGS(FormattingOptionsFlags)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( FormattingOptionsFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::unknown                  , "Unknown"                 );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::none                     , "None"                    );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthIndirect       , "FieldWidthIndirect"      );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::caseInvert               , "CaseInvert"              );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::argIdTaken               , "ArgIdTaken"              );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::signMinus                , "SignMinus"               );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::signPlus                 , "SignPlus"                );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::localeFormatting         , "LocaleFormatting"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthTaken          , "FieldWidthTaken"         );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::signAlterForm            , "SignAlterForm"           );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::signZ                    , "SignZ"                   );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::signSpace                , "SignSpace"               );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::signZero                 , "SignZero"                );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::fillingTaken             , "FillingTaken"            );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::fillingIndirect          , "FillingIndirect"         );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::precisionTaken           , "PrecisionTaken"          );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::precisionIndirect        , "PrecisionIndirect"       );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::grouppingTaken           , "GrouppingTaken"          );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthIndirectTaken  , "FieldWidthIndirectTaken" );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::precisionIndirectTaken   , "PrecisionIndirectTaken"  );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingOptionsFlags::fillingIndirectTaken     , "FillingIndirectTaken"    );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( FormattingOptionsFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( FormattingOptionsFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::unknown                  , "unknown"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::unknown                  , "invalid"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::none                     , "none"                       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthIndirect       , "field-width-indirect"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthIndirect       , "field_width_indirect"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthIndirect       , "fieldwidthindirect"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::caseInvert               , "case-invert"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::caseInvert               , "case_invert"                );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::caseInvert               , "caseinvert"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::argIdTaken               , "arg-id-taken"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::argIdTaken               , "argidtaken"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::argIdTaken               , "arg_id_taken"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signMinus                , "sign-minus"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signMinus                , "sign_minus"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signMinus                , "signminus"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signPlus                 , "sign-plus"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signPlus                 , "sign_plus"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signPlus                 , "signplus"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::localeFormatting         , "locale-formatting"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::localeFormatting         , "locale_formatting"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::localeFormatting         , "localeformatting"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthTaken          , "field-width-taken"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthTaken          , "field_width_taken"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthTaken          , "fieldwidthtaken"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signAlterForm            , "sign-alter-form"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signAlterForm            , "signalterform"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signAlterForm            , "sign_alter_form"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signZ                    , "sign-z"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signZ                    , "sign_z"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signZ                    , "signz"                      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signSpace                , "sign-space"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signSpace                , "sign_space"                 );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signSpace                , "signspace"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signZero                 , "sign-zero"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signZero                 , "sign_zero"                  );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::signZero                 , "signzero"                   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingTaken             , "filling-taken"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingTaken             , "filling_taken"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingTaken             , "fillingtaken"               );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingIndirect          , "filling-indirect"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingIndirect          , "filling_indirect"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingIndirect          , "fillingindirect"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionTaken           , "precision-taken"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionTaken           , "precision_taken"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionTaken           , "precisiontaken"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionIndirect        , "precision-indirect"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionIndirect        , "precision_indirect"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionIndirect        , "precisionindirect"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::grouppingTaken           , "groupping-taken"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::grouppingTaken           , "groupping_taken"            );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::grouppingTaken           , "grouppingtaken"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthIndirectTaken  , "field-width-indirect-taken" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthIndirectTaken  , "field_width_indirect_taken" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fieldWidthIndirectTaken  , "fieldwidthindirecttaken"    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionIndirectTaken   , "precision-indirect-taken"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionIndirectTaken   , "precision_indirect_taken"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::precisionIndirectTaken   , "precisionindirecttaken"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingIndirectTaken     , "filling-indirect-taken"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingIndirectTaken     , "filling_indirect_taken"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingOptionsFlags::fillingIndirectTaken     , "fillingindirecttaken"       );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( FormattingOptionsFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(FormattingOptionsFlags, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(FormattingOptionsFlags, std::set)


//#!FormattingFlags
enum class FormattingFlags : std::uint32_t
{
    unknown                         = (std::uint32_t)(-1) /*!<  */,
    invalid                         = (std::uint32_t)(-1) /*!<  */,
    none                            = 0x00 /*!<  */,
    ignoreFormatStringErrors        = 0x01 /*!< Ignore errors in format string */,
    ignoreArgumentErrors            = 0x02 /*!< Ignore errors while searching for arguments */,
    ignoreFormattingErrors          = 0x04 /*!< Ignore errors while formatting values */,
    ignoreFillIndirectErrors        = 0x08 /*!< Ignore errors while searching for arguments for fill char */,
    ignoreWidthIndirectErrors       = 0x10 /*!< Ignore errors while searching for arguments for width */,
    ignorePrecisionIndirectErrors   = 0x20 /*!< Ignore errors while searching for arguments for precision */,
    ignoreConversionErrors          = 0x40 /*!< Ignore formatting type specifications and allow automatic type conversion */,
    considerZeroWidthSpaces         = 0x80 /*!< Учитывать пробелы нулевой ширины */,
    considerCombiningChars          = 0x100 /*!< Учитывать комбинированные символы */,
    fractionalGroupping             = 0x200 /*!< Группировка цифр также и в дробной части числа, если разделитель разрядов задан для целой части числа */,
    ignoreOptionsIndirectErrors     = ignoreFillIndirectErrors | ignoreWidthIndirectErrors | ignorePrecisionIndirectErrors /*!<  */,
    ignoreErrors                    = ignoreFormatStringErrors | ignoreArgumentErrors | ignoreFormattingErrors | ignoreOptionsIndirectErrors | ignoreConversionErrors /*!<  */,
    considerUnicodeFeatures         = considerZeroWidthSpaces | considerCombiningChars /*!<  */,
    all                             = ignoreErrors | considerUnicodeFeatures | fractionalGroupping /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_FLAGS(FormattingFlags)

MARTY_CPP_ENUM_FLAGS_SERIALIZE_BEGIN( FormattingFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::unknown                         , "Unknown"                       );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignoreFillIndirectErrors        , "IgnoreFillIndirectErrors"      );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::none                            , "None"                          );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignoreFormatStringErrors        , "IgnoreFormatStringErrors"      );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignoreArgumentErrors            , "IgnoreArgumentErrors"          );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignoreFormattingErrors          , "IgnoreFormattingErrors"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignorePrecisionIndirectErrors   , "IgnorePrecisionIndirectErrors" );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignoreWidthIndirectErrors       , "IgnoreWidthIndirectErrors"     );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignoreConversionErrors          , "IgnoreConversionErrors"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::considerZeroWidthSpaces         , "ConsiderZeroWidthSpaces"       );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::considerCombiningChars          , "ConsiderCombiningChars"        );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::fractionalGroupping             , "FractionalGroupping"           );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignoreOptionsIndirectErrors     , "IgnoreOptionsIndirectErrors"   );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::ignoreErrors                    , "IgnoreErrors"                  );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::considerUnicodeFeatures         , "ConsiderUnicodeFeatures"       );
    MARTY_CPP_ENUM_FLAGS_SERIALIZE_ITEM( FormattingFlags::all                             , "All"                           );
MARTY_CPP_ENUM_FLAGS_SERIALIZE_END( FormattingFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_BEGIN( FormattingFlags, std::map, 1 )
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::unknown                         , "unknown"                          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::unknown                         , "invalid"                          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFillIndirectErrors        , "ignore-fill-indirect-errors"      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFillIndirectErrors        , "ignore_fill_indirect_errors"      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFillIndirectErrors        , "ignorefillindirecterrors"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::none                            , "none"                             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFormatStringErrors        , "ignore-format-string-errors"      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFormatStringErrors        , "ignore_format_string_errors"      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFormatStringErrors        , "ignoreformatstringerrors"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreArgumentErrors            , "ignore-argument-errors"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreArgumentErrors            , "ignoreargumenterrors"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreArgumentErrors            , "ignore_argument_errors"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFormattingErrors          , "ignore-formatting-errors"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFormattingErrors          , "ignore_formatting_errors"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreFormattingErrors          , "ignoreformattingerrors"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignorePrecisionIndirectErrors   , "ignore-precision-indirect-errors" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignorePrecisionIndirectErrors   , "ignore_precision_indirect_errors" );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignorePrecisionIndirectErrors   , "ignoreprecisionindirecterrors"    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreWidthIndirectErrors       , "ignore-width-indirect-errors"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreWidthIndirectErrors       , "ignore_width_indirect_errors"     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreWidthIndirectErrors       , "ignorewidthindirecterrors"        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreConversionErrors          , "ignore-conversion-errors"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreConversionErrors          , "ignore_conversion_errors"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreConversionErrors          , "ignoreconversionerrors"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerZeroWidthSpaces         , "consider-zero-width-spaces"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerZeroWidthSpaces         , "consider_zero_width_spaces"       );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerZeroWidthSpaces         , "considerzerowidthspaces"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerCombiningChars          , "consider-combining-chars"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerCombiningChars          , "considercombiningchars"           );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerCombiningChars          , "consider_combining_chars"         );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::fractionalGroupping             , "fractional-groupping"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::fractionalGroupping             , "fractional_groupping"             );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::fractionalGroupping             , "fractionalgroupping"              );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreOptionsIndirectErrors     , "ignore-options-indirect-errors"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreOptionsIndirectErrors     , "ignore_options_indirect_errors"   );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreOptionsIndirectErrors     , "ignoreoptionsindirecterrors"      );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreErrors                    , "ignore-errors"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreErrors                    , "ignore_errors"                    );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::ignoreErrors                    , "ignoreerrors"                     );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerUnicodeFeatures         , "consider-unicode-features"        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerUnicodeFeatures         , "consider_unicode_features"        );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::considerUnicodeFeatures         , "considerunicodefeatures"          );
    MARTY_CPP_ENUM_FLAGS_DESERIALIZE_ITEM( FormattingFlags::all                             , "all"                              );
MARTY_CPP_ENUM_FLAGS_DESERIALIZE_END( FormattingFlags, std::map, 1 )

MARTY_CPP_ENUM_FLAGS_SERIALIZE_SET(FormattingFlags, std::set)

MARTY_CPP_ENUM_FLAGS_DESERIALIZE_SET(FormattingFlags, std::set)

} // namespace format
} // namespace marty

