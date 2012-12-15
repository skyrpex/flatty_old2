#ifndef DELETEANIMCOMMAND_H
#define DELETEANIMCOMMAND_H

#include <QUndoCommand>

class AnimModel;
class Anim;

class DeleteAnimCommand : public QUndoCommand
{
public:
    DeleteAnimCommand(AnimModel *model, Anim *anim);

    void undo();
    void redo();

private:
    AnimModel *m_model;
    Anim *m_anim;
};

#endif // DELETEANIMCOMMAND_H
