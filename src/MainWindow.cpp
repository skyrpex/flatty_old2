#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "model/JointModel.h"
#include "model/Joint.h"
#include "widgets/AnimsWidget.h"
#include "widgets/TimeWidget.h"
#include "widgets/GraphicsWidget.h"
#include "Application.h"
#include <QGraphicsScene>
#include <QSplitter>
#include <QVBoxLayout>

const int StrecthFactor = 12;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_model(new JointModel(this))
{
    ui->setupUi(this);
    ui->menuEdit->addAction(qApp->undoStack()->createUndoAction(this));
    ui->menuEdit->addAction(qApp->undoStack()->createRedoAction(this));

    new Joint("Rack", m_model->root());

    AnimsWidget *animsWidget = new AnimsWidget(m_model->animModel());
    TimeWidget *timeWidget = new TimeWidget(animsWidget, m_model, StrecthFactor);
    GraphicsWidget *graphicsWidget = new GraphicsWidget(m_model);

    connect(animsWidget, SIGNAL(currentAnimChanged(int)), timeWidget, SLOT(setCurrentAnim(int)));
    connect(animsWidget, SIGNAL(currentAnimChanged(int)), graphicsWidget, SLOT(setCurrentAnim(int)));
    connect(timeWidget, SIGNAL(currentFrameChanged(int)), graphicsWidget, SLOT(setCurrentFrame(int)));

    QSplitter *horizontalSplitter = new QSplitter(this);
    horizontalSplitter->addWidget(animsWidget);
    horizontalSplitter->addWidget(graphicsWidget);
    horizontalSplitter->setStretchFactor(1, StrecthFactor);
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, this);
    verticalSplitter->addWidget(horizontalSplitter);
    verticalSplitter->addWidget(timeWidget);
    verticalSplitter->setStretchFactor(0, StrecthFactor);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(verticalSplitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}
