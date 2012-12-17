#include "EditAnimCommand.h"
#include "model/Anim.h"
#include "Application.h"

const char * const CommandText = "Edit Animation";

EditAnimCommand::EditAnimCommand(Anim *anim, QString name, int frameCount, int fps)
    : m_anim(anim),
      m_name(name),
      m_frameCount(frameCount),
      m_fps(fps),
      m_oldName(m_anim->name()),
      m_oldFrameCount(m_anim->frameCount()),
      m_oldFps(m_anim->fps())
{
    setText(qApp->tr(CommandText));
}

void EditAnimCommand::undo()
{
    m_anim->setName(m_oldName);
    m_anim->setFrameCount(m_oldFrameCount);
    m_anim->setFps(m_oldFps);
}

void EditAnimCommand::redo()
{
    m_anim->setName(m_name);
    m_anim->setFrameCount(m_frameCount);
    m_anim->setFps(m_fps);
}
