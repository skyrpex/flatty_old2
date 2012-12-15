#include "CreateAnimCommand.h"
#include "model/AnimModel.h"
#include "model/Anim.h"
#include <QDebug>

CreateAnimCommand::CreateAnimCommand(AnimModel *model, Anim *anim) :
    m_model(model),
    m_anim(anim)
{
    Q_ASSERT(model);
    Q_ASSERT(anim);
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
}
