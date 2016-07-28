# codeAdapt_IDE

The plan is to fork Code::Blocks SVN 4907.
And, add or update some code modules to Code::Blocks 16.01 release.

Planned or taken steps below.

## 1. Change modules
###a. Remove Modules
###b. Git Operations
####i.   Create src/.gitattributes file
####ii.  Normalize line-feeds on Windows
####iii. Add src/.gitattributes file
####iv.  Add src/.gitignore file
###c. Add Modules

## 2. Add Documentation
###a. Add README.md
###b. Add Branches.md

## 3. Fix Portablity Issues
###a. Remove Windows stuff from Unix CB projects
###b. Add missing compiler option "-std=c++11" to non Windows projects.
###c. Add missing compiler option "-std=gnu++11" to Windows projects.
###d. Add WX_VERSION=28
###e. Change library prefix from "wxmsw28" to "wxmsw$(WX_VERSION)"
###f. Add WX_COMPILER="gcc" and WX_TOOLKIT="msw"
###g. Change "gcc_dll" to "$(WX_COMPILER)_dll"
###h. Move wxmsw$(WX_VERSION)$(WX_SUFFIX) to target level

## 4. Change to using lib, lib28, or lib30 folders
###a. Add src/.gitignore
###b. Edit the old .gitignore
###c. Change Linux CB Projects to have ".a" files in lib?? folders
###d. Change Windows CB Projects to have ".a" files in lib?? folders

## 5. Change Projects to NOT use update batch or script files
###a. Change Linux CB Projects
###b. Change Windows CB Projects

## 6. Fix PCH Issues
###a. Fix PCH Issues on Linux when using Projects
###b. Fix PCH Issues on Windows

## 7. Source code fixes needed to build
###a. Remove headers "wx/wxscintilla.h" and "editorcolourset.h" from PCH
###b. Fix SDK code
###c. Fix Core project
###d. Fix Contrib project

## 8. Edit sdk_common.h
###a. Remove "other base files" headers
###b. Fix SDK code
###c. Fix Core project
###d. Fix Contrib project

## 9. Change simple source code issues related to branding
###a. Add SDK header "branding.h"
###b. Add use of STANDARD_DATA_PATH like used in Em::Blocks
###c. Add use of APP_NAME partly like used in Em::Blocks
