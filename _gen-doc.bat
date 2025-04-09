@set UMBA_MDPP_EXTRA_OPTIONS=
@rem "--batch-generate-page-index" "--batch-page-index-file=doc/Index.md"

@set BRIEF_SCAN_PATHS="--scan=%~dp0"
@rem "--scan=%~dp0\../src" "--scan=%~dp0\../_src" "--scan=%~dp0\../examples"
@rem set BRIEF_EXTRA_OPTS_TXT=--scan-notes "--notes-output-path=%~dp0\doc\_md"
@set BRIEF_EXTRA_OPTS_MD=--scan-notes "--notes-output-path=%~dp0\doc\_md"

@rem call "%~dp0.bat\gen-doc.bat"

@set EXAMPLES_INCLUDE="--add-examples-path=%~dp0\." "--add-examples-path=%~dp0\md_"
@rem "--add-examples-path=%~dp0" "--add-examples-path=%~dp0\md_"


@set EXCLUDE_DIRS=--batch-exclude-dir=_libs,libs,_lib,lib,tests,test,rc,_generators,conf,_distr_conf,doxy,src,_src,.msvc2019,boost,icons
@set COMMON=--overwrite --copy-images
@rem 
@set GRAPHVIZ_OPTIONS=
@rem "--graphviz-output-path=%~dp0\..\doc\.generated_images"
@rem
@rem set TEMP_OPTS=--graphviz-keep-temp-dot-files
@set OPTIONS= %TEMP_OPTS% %COMMON% %VARS% %EXCLUDE_DIRS% %GRAPHVIZ_OPTIONS% %UMBA_MDPP_EXTRA_OPTIONS% %EXAMPLES_INCLUDE% "--batch-scan-recurse=%~dp0\."
umba-md-pp %OPTIONS%

@umba-md-pp --help > umba-md-pp-help.txt 2>&1
