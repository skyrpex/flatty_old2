#include "Application.h"

Application::Application(int argc, char **argv) :
    QApplication(argc, argv),
    m_undoStack(new QUndoStack(this))
{
    setOrganizationName("VGAFIB");
    setApplicationName("Flatty");
    setApplicationVersion("0.0");
}

QUndoStack *Application::undoStack()
{
    return m_undoStack;
}
