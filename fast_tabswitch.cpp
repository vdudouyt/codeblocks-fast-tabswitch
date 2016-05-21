#include <sdk.h> // Code::Blocks SDK
#include <configurationpanel.h>
#include <wx/wxscintilla.h>
#include "fast_tabswitch.h"
#include "editor_hooks.h"
#include "editormanager.h"
#include "cbauibook.h"

// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<fast_tabswitch> reg(_T("fast_tabswitch"));
}



// constructor
fast_tabswitch::fast_tabswitch()
{
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(_T("fast_tabswitch.zip")))
    {
        NotifyMissingFile(_T("fast_tabswitch.zip"));
    }
}

// destructor
fast_tabswitch::~fast_tabswitch()
{
}

void fast_tabswitch::OnAttach()
{
    // do whatever initialization you need for your plugin
    // NOTE: after this function, the inherited member variable
    // m_IsAttached will be TRUE...
    // You should check for it in other functions, because if it
    // is FALSE, it means that the application did *not* "load"
    // (see: does not need) this plugin...
    EditorHooks::HookFunctorBase* myhook = new EditorHooks::HookFunctor<fast_tabswitch>(this, &fast_tabswitch::EditorEventHook);
    m_EditorHookId = EditorHooks::RegisterHook(myhook);
}

void fast_tabswitch::OnRelease(bool appShutDown)
{
    EditorHooks::UnregisterHook(m_EditorHookId, true);
}

void fast_tabswitch::EditorEventHook(cbEditor* editor, wxScintillaEvent& event)
{
    #define IN_BOUNDS(num, min, max) ((num) >= (min) && (num) <= (max))

    if(event.GetEventType() == wxEVT_SCI_KEY
         && event.GetModifiers() & wxSCI_SCMOD_ALT
         && IN_BOUNDS(event.GetKey(), 0x30, 0x39))
    {
        EditorManager *em = Manager::Get()->GetEditorManager();
        cbAuiNotebook *notebook = em->GetNotebook();

        int key = event.GetKey();
        int tab_id = key == 0x30 ? 9 : (key - 0x31);

        if(notebook->GetSelection() != tab_id)
        {
            notebook->SetSelection(tab_id);
        }
    }
    else
    {
        event.Skip();
    }
}
