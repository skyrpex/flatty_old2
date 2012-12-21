#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class TimeWidget;
}
class Anim;
class JointModel;
class JointDelegate;
class JointProxyModel;
class QTreeView;
class QSpinBox;
class AnimsWidget;

class TimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimeWidget(AnimsWidget *animsWidget, JointModel *model, int stretchFactor, QWidget *parent = 0);

signals:
    void currentFrameChanged(int frame);

public slots:
    void setCurrentAnim(Anim *anim);

private slots:
    void openEditor(const QModelIndex &index);
    void onExpanded(const QModelIndex &proxyIndex);
    void onCollapsed(const QModelIndex &proxyIndex);
    void resetEditor();
//    void updateAnimFrameCount();
//    void setAnimFps(int fps);
    void showFrameCountDialog();
    void showFpsDialog();

private:
    void updateFrameCountButton();
    void updateFpsButton();

    Ui::TimeWidget *ui;
    AnimsWidget *m_animsWidget;
    JointModel *m_model;
    JointDelegate *m_delegate;
    JointProxyModel *m_leftProxy;
    JointProxyModel *m_rightProxy;
    QModelIndex m_openEditorIndex;
    Anim *m_currentAnim;
};

#endif // TIMEWIDGET_H
