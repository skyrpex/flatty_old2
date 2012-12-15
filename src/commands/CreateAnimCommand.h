#ifndef CREATEANIMCOMMAND_H
#define CREATEANIMCOMMAND_H

#include <QUndoCommand>
#include <model/AnimModel.h>
#include <model/Anim.h>

class CreateAnimCommand : public QUndoCommand
{
public:
    explicit CreateAnimCommand(AnimModel *model, Anim *anim);

    void undo();
    void redo();

private:
    AnimModel *m_model;
    Anim *m_anim;
};

#endif // CREATEANIMCOMMAND_H
