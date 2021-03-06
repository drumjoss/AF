/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

#include "../../../ProjectSaving/jucer_ProjectExporter.h"
#include "../../../Application/Windows/jucer_TranslationToolWindowComponent.h"
#include "../../../Utility/UI/jucer_JucerTreeViewBase.h"
#include "../../../Wizards/jucer_NewFileWizard.h"
#include "../jucer_ContentViewComponents.h"
#include "../jucer_FileGroupInformationComponent.h"
#include "../jucer_ModulesInformationComponent.h"

struct TreeItemTypes
{
    #include "jucer_ProjectTreeItemBase.h"
    #include "jucer_ExporterTreeItems.h"
    #include "jucer_ModuleTreeItems.h"
    #include "jucer_FileTreeItems.h"
};
