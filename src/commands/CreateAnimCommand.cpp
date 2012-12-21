#include "CreateAnimCommand.h"
#include "model/AnimModel.h"
#include "model/Anim.h"
#include "widgets/AnimsWidget.h"
#include "Application.h"
#include <QDebug>

const char * const CommandText = "Create Animation";

CreateAnimCommand::CreateAnimCommand(AnimModel *model, Anim *anim, AnimsWidget *widget) :
    m_widget(widget),
    m_model(model),
    m_anim(anim)
{
    Q_ASSERT(model);
    Q_ASSERT(anim);
    setText(qApp->tr(CommandText));
}

void CreateAnimCommand::undo()
{
    int index = m_model->anims().indexOf(m_anim);

    Q_ASSERT(index != -1);
    m_model->takeAnim(index);
}

void CreateAnimCommand::redo()
{
    m_model->addAnim(m_anim);
    m_widget->setCurrentAnim(m_anim);
}
