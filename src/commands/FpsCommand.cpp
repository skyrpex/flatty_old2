#include "FpsCommand.h"
#include "model/Anim.h"
#include "Application.h"
#include "widgets/AnimsWidget.h"
#include "model/AnimModel.h"
#include <QDebug>

const char * const CommandText = "Edit Animation Fps";

FpsCommand::FpsCommand(Anim *anim, int fps, AnimsWidget *widget)
    : m_widget(widget),
      m_anim(anim),
      m_fps(fps),
      m_oldFps(m_anim->fps())
{
    setText(qApp->tr(CommandText));
}

void FpsCommand::undo()
{
    m_anim->setFps(m_oldFps);
    m_widget->setCurrentAnim(m_anim);
}

void FpsCommand::redo()
{
    m_anim->setFps(m_fps);
    m_widget->setCurrentAnim(m_anim);
}
