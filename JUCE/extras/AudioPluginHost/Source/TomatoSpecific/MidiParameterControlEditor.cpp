/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include "MidiParameterControl.h"
#include "MidiParameterControlEditor.h"

#include <stdlib.h> 

//==============================================================================
MidiParameterControlEditor::MidiParameterControlEditor (MidiParameterControl& p)
    : AudioProcessorEditor (&p), processor (p)
{
	for (int i = 0; i < max_mapping_per_instance; ++i)
	{
		textEditorCcs[i].reset(new TextEditor("MidiCC"));
		addAndMakeVisible(textEditorCcs[i].get());
		textEditorCcs[i]->setMultiLine(false);
		textEditorCcs[i]->setReturnKeyStartsNewLine(false);
		textEditorCcs[i]->setReadOnly(false);
		textEditorCcs[i]->setScrollbarsShown(true);
		textEditorCcs[i]->setCaretVisible(true);
		textEditorCcs[i]->setPopupMenuEnabled(true);
		textEditorCcs[i]->setText(String());
		textEditorCcs[i]->setTextToShowWhenEmpty(String("Enter MidiCC Number"), getLookAndFeel().findColour(TextEditor::highlightColourId));
		textEditorCcs[i]->setInputRestrictions(3, "1234567890");

		textEditorCcs[i]->setBounds(24, 24 * (2 * i + 1), 200, 24);

		textEditorParams[i].reset(new TextEditorWithPopUpMenuForParameter("Parameter"));
		textEditorParams[i]->SetParameter(this);
		textEditorParams[i]->SetID(i);
		textEditorParams[i]->setReadOnly(true);
		addAndMakeVisible(textEditorParams[i].get());
		textEditorParams[i]->setMultiLine(false);
		textEditorParams[i]->setReturnKeyStartsNewLine(false);
		textEditorParams[i]->setReadOnly(false);
		textEditorParams[i]->setScrollbarsShown(true);
		textEditorParams[i]->setCaretVisible(true);
		textEditorParams[i]->setPopupMenuEnabled(true);
		textEditorParams[i]->setText(String());
		textEditorParams[i]->setTextToShowWhenEmpty(String("Parameter"), getLookAndFeel().findColour(TextEditor::highlightColourId));
		textEditorParams[i]->setCaretVisible(false);

		textEditorParams[i]->setBounds(248, 24 * (2 * i + 1), 200, 24);
	}

	// Restoring
	auto map = processor.getParameterMappings();
	if (map.size() > 0)
	{
		int nbRestoredParameter = 0;
		for (std::map<int, MidiParameterControl::ControllableParameter*>::iterator it = map.begin(); it != map.end(); ++it)
		{
			textEditorCcs[nbRestoredParameter]->setText(*(new String(it->first)));
			textEditorCcs[nbRestoredParameter]->setReadOnly(true);

			AudioProcessor &targetProcessor = it->second->processorParameterOwner;
			textEditorParams[nbRestoredParameter]->setText(targetProcessor.getName() + " : " + targetProcessor.getParameterName(it->second->parameterIndex));
			++nbRestoredParameter; // should not be > max_mapping_per_instance
		}

	}
	else
	{
		// Noooothing to restore
	}

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (472, 500);
}

MidiParameterControlEditor::~MidiParameterControlEditor()
{
	for (int i = 0; i < max_mapping_per_instance; ++i)
	{
		textEditorCcs[i] = nullptr;
		textEditorParams[i] = nullptr;
	}

	processor.editorBeingDeleted(this);
}

//==============================================================================
void MidiParameterControlEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MidiParameterControlEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void MidiParameterControlEditor::showPopupMenu(int ID)
{
	// Well this huge function parses the graph to fetch all plugins parameters, then adds every parameter with a particular Item ID
	menu.reset(new PopupMenu);

	auto *graphToParse = processor.GetAudioProcessorGraph();

	// Parse graph
	for (int i = 0; i < graphToParse->getNumNodes(); ++i)
	{
		auto *processorToParse = graphToParse->getNode(i)->getProcessor();
		
		if ((processorToParse != nullptr) &&
			(processorToParse->getName() != processor.getName())) // Well do not process yourself uh
		{
			std::unique_ptr<PopupMenu> SubMenu;
			SubMenu.reset(new PopupMenu);

			for (int j = 0; j < processorToParse->getNumParameters(); ++j)
			{
				// Item ID is: thousands (max_parameter_per_processor) for processor ID + 0-999 for Parameter ID
				// Start at 1 because 0 is when nothing has been done 
				SubMenu->addItem((i * max_parameter_per_processor) + j + 1, processorToParse->getParameterName(j));
			}

			menu->addSubMenu(processorToParse->getName(), *SubMenu, true);
		}

	}

	menu->showMenuAsync({}, ModalCallbackFunction::create
	([this, ID](int ProcessorParameterProduct) {
		if (ProcessorParameterProduct > 0)
		{
			auto *graphToParse = processor.GetAudioProcessorGraph();
			// Calculate Processor
			const int processorIndex = ProcessorParameterProduct / max_parameter_per_processor;
			auto *targetProcessor = graphToParse->getNode(processorIndex)->getProcessor();
			//Calculate Parameter index
			int parameterIndex = ProcessorParameterProduct - (processorIndex * max_parameter_per_processor) - 1;

			processor.addMapping(textEditorCcs[ID]->getText().getIntValue(), *targetProcessor, parameterIndex);

			textEditorCcs[ID]->setReadOnly(true);
			textEditorParams[ID]->setText(targetProcessor->getName() + " : " + targetProcessor->getParameterName(parameterIndex));
		}
	}));
}

bool MidiParameterControlEditor::isMidiCCTextEditorOk(int ID)
{
	return !(textEditorCcs[ID]->isEmpty());
}
