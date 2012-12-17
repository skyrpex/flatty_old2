#include "AnimFpsEditCommand.h"
#include "model/Anim.h"
#include "Application.h"
#include <QDebug>

const char * const CommandText = "Edit Animation Fps";

AnimFpsEditCommand::AnimFpsEditCommand(Anim *anim, int fps)
    : m_anim(anim),
      m_fps(fps),
      m_oldFps(m_anim->fps())
{
    setText(qApp->tr(CommandText));
}

void AnimFpsEditCommand::undo()
{
    m_anim->setFps(m_oldFps);
}

void AnimFpsEditCommand::redo()
{
    m_anim->setFps(m_fps);
}

bool AnimFpsEditCommand::mergeWith(const QUndoCommand *command)
{
    const AnimFpsEditCommand *other = dynamic_cast<const AnimFpsEditCommand*>(command);
    if(other == NULL)
        return false;

    m_fps = other->m_fps;
    return true;
}

int AnimFpsEditCommand::id() const
{
    return Id;
}
