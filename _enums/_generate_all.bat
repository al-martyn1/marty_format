@set UINT8=--underlaying-type=std::uint8_t
@set UINT16=--underlaying-type=std::uint16_t
@set UINT32=--underlaying-type=std::uint32_t
@set INT=--underlaying-type=int

@set HEX2=--hex-width=2
@set HEX4=--hex-width=4
@set HEX6=--hex-width=6
@set HEX8=--hex-width=8

@rem set EXTRA=--enum-flags=extra
@set FLAGENUM_EXTRA=--enum-flags=serialize-set,deserialize-set

@set FLAGS=--enum-flags=flags
@set DECL=--enum-flags=type-decl
@set CLS=--enum-flags=enum-class
@set DECLCLS=--enum-flags=type-decl,enum-class
@set FMTHEX=--enum-flags=fmt-hex
@set SERIALIZE=--enum-flags=serialize,deserialize,lowercase

@set TPL_OVERRIDE=--override-template-parameter=EnumNameFormat:$(ENAMNAME)
@set GEN_OPTS=--enum-values-style=CamelStyle

@set VALUES_CAMEL=--enum-values-style=CamelStyle
@set SERIALIZE_CAMEL=--serialize-style=CamelStyle
@set VALUES_PASCAL=--enum-values-style=PascalStyle
@set SERIALIZE_PASCAL=--serialize-style=PascalStyle
@set SERIALIZE_HYPHEN=--serialize-style=HyphenStyle

@set TR=--tr-lang=en-US,ru-RU --tr-template-language=en-US --tr-template-output=./tr-en-US.json
@set MD=--md-output-path=../md_/_enums

@set SNIPPETOPTIONS_GEN_FLAGS=--enum-flags=0 --enum-flags=type-decl,serialize,deserialize,lowercase,enum-class,fmt-hex %VALUES_CAMEL% %SERIALIZE_PASCAL% --enum-serialize-style=All

@set FLAGS=--enum-flags=flags

@call %~dp0\tr.bat

umba-enum-gen %TR% %MD% %GEN_OPTS% %HEX2% %TPL_OVERRIDE% %SNIPPETOPTIONS_GEN_FLAGS%              ^
    %UINT32% %HEX2% -E=FormatIndexType          -F=@FormatIndexType.txt                ^
    %UINT32% %HEX2% -E=StdFilterType            -F=@StdFilterType.txt                  ^
    %UINT32% %HEX2% -E=NumeralSystem            -F=@NumeralSystem.txt                  ^
    %UINT32% %HEX2% -E=LocaleInfoType           -F=@LocaleInfoType.txt                 ^
    %UINT32% %HEX2% -E=LocaleInfoValueType      -F=@LocaleInfoValueType.txt            ^
    %UINT32% %HEX2% -E=LocaleInfoSeparatorType  -F=@LocaleInfoSeparatorType.txt        ^
    %UINT32% %HEX2% -E=LocaleInfoDigitsType     -F=@LocaleInfoDigitsType.txt           ^
    %UINT32% %HEX2% -E=PositiveNumbersMode      -F=@PositiveNumbersMode.txt            ^
    %FLAGS% %FLAGENUM_EXTRA%                                                           ^
    %UINT32% %HEX2% -E=FormattingOptionsFlags   -F=@FormattingOptionsFlags.txt         ^
    %UINT32% %HEX2% -E=FormattingFlags          -F=@FormattingFlags.txt                ^
..\enums.h

