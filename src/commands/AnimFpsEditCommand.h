#ifndef ANIMFPSEDITCOMMAND_H
#define ANIMFPSEDITCOMMAND_H

#include <QUndoCommand>

class Anim;

class AnimFpsEditCommand : public QUndoCommand
{
public:
    enum {
        Id = 0
    };
    AnimFpsEditCommand(Anim *anim, int fps);

    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *command);
    int id() const;

private:
    Anim *m_anim;
    int m_fps;
    int m_oldFps;
};

#endif // ANIMFPSEDITCOMMAND_H
