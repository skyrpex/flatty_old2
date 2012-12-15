#ifndef CREATEANIMCOMMAND_H
#define CREATEANIMCOMMAND_H

#include <QUndoCommand>

class AnimModel;
class Anim;

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
