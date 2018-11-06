
# VST3 Folder (following the specification)
function(smtg_get_default_vst3_path)
    if(WIN)
        string(FIND ${CMAKE_GENERATOR} Win64 win64Found)
        if(${win64Found} EQUAL -1)
            if(EXISTS "$ENV{PROGRAMFILES} (x86)")
                set(DEFAULT_VST3_FOLDER "$ENV{PROGRAMFILES} (x86)\\Common Files\\VST3" PARENT_SCOPE)
            else()
                set(DEFAULT_VST3_FOLDER "$ENV{PROGRAMFILES}\\Common Files\\VST3" PARENT_SCOPE)
            endif()
        else()
            set(DEFAULT_VST3_FOLDER "$ENV{CommonProgramW6432}\\VST3" PARENT_SCOPE)
        endif()
    elseif(MAC)
        set(DEFAULT_VST3_FOLDER "$ENV{HOME}/Library/Audio/Plug-ins/VST3" PARENT_SCOPE)
    elseif(LINUX)  	
        set(DEFAULT_VST3_FOLDER "$ENV{HOME}/.vst3" PARENT_SCOPE)
    endif()
endfunction()
