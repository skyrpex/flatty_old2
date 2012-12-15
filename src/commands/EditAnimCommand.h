#ifndef EDITANIMCOMMAND_H
#define EDITANIMCOMMAND_H

#include <QUndoCommand>
#include <QString>

class Anim;

class EditAnimCommand : public QUndoCommand
{
public:
    EditAnimCommand(Anim *anim, QString name, int frameCount, int fps);

    void undo();
    void redo();

private:
    Anim *m_anim;
    QString m_name;
    int m_frameCount;
    int m_fps;
    QString m_oldName;
    int m_oldFrameCount;
    int m_oldFps;
};

#endif // EDITANIMCOMMAND_H
