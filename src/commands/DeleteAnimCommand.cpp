#include "DeleteAnimCommand.h"
#include "model/AnimModel.h"
#include "model/Anim.h"
#include "widgets/AnimsWidget.h"
#include "Application.h"
#include <QDebug>

const char * const CommandText = "Delete Animation";

DeleteAnimCommand::DeleteAnimCommand(AnimModel *model, Anim *anim, AnimsWidget *widget) :
    m_widget(widget),
    m_model(model),
    m_anim(anim)
{
    Q_ASSERT(model);
    Q_ASSERT(anim);
    setText(qApp->tr(CommandText));

    m_index = model->anims().indexOf(anim);
}

void DeleteAnimCommand::undo()
{
    m_model->insertAnim(m_index, m_anim);
    m_widget->setCurrentAnim(m_anim);
}

void DeleteAnimCommand::redo()
{
    m_model->takeAnim(m_index);
}
