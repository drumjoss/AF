/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiParameterControl.h"

//==============================================================================
/**
*/
class MidiParameterControlEditor :  public AudioProcessorEditor
{
public:
	static const int max_parameter_per_processor = 1000;
	static const int max_mapping_per_instance = 8;

	MidiParameterControlEditor(MidiParameterControl&);
    ~MidiParameterControlEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	void showPopupMenu(int ID);

	bool isMidiCCTextEditorOk(int);

	class TextEditorWithPopUpMenuForParameter : public TextEditor
	{
	public:	

		TextEditorWithPopUpMenuForParameter (const String& name):
			TextEditor(name)
		{}

		void SetParameter(MidiParameterControlEditor *newParameter)
		{
			parameter = newParameter;
		}

		void SetID(int newID)
		{
			ID = newID;
		}

		void mouseDown(const MouseEvent&)
		{
			if((parameter != nullptr)    &&
			   (parameter->isMidiCCTextEditorOk(ID)))
				parameter->showPopupMenu(ID);
		}

	protected:
		MidiParameterControlEditor * parameter;
		int ID;
	};
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	MidiParameterControl& processor;

	//==============================================================================
	std::unique_ptr<TextEditorWithPopUpMenuForParameter>textEditorParams[max_mapping_per_instance];
	std::unique_ptr<TextEditor> textEditorCcs[max_mapping_per_instance];
	std::unique_ptr<PopupMenu> menu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiParameterControlEditor)
};
