/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef cmMakefileExecutableTargetGenerator_h
#define cmMakefileExecutableTargetGenerator_h

#include "cmMakefileTargetGenerator.h"

class cmMakefileExecutableTargetGenerator: public cmMakefileTargetGenerator
{
public:
  cmMakefileExecutableTargetGenerator(cmGeneratorTarget* target);
  virtual ~cmMakefileExecutableTargetGenerator();

  /* the main entry point for this class. Writes the Makefiles associated
     with this target */
  virtual void WriteRuleFiles();

protected:
  virtual void WriteExecutableRule(bool relink);
};

#endif
