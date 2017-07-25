
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDirModel>
#include <rocore/ui/GerberDocumentView.hpp>
#include "About.hpp"
#include "MainWindow.hpp"
#include "rogerber/Gerber.hpp"
#include "rogerber/GerberProcessor.hpp"
#include "rocore/ui/DocumentView.hpp"
#include "rocore/ui/ProjectView.hpp"
#include "rocore/ui/LayerView.hpp"
#include "rocore/graphics/Color.hpp"
#include "util/make_unique.hpp"
#include "GerberOpenGlProcessor.hpp"

MainWindow::MainWindow(QWidget *parent) :
        QiMainWindow("roviewer",parent)
{
	setupMenubar();

    setCentralWidget(mdiArea_);
	doc = new rocore::ui::GerberDocumentView(mdiArea_->viewport());
	mdiArea_->addSubWindow(doc);

	project_ = std::make_shared<rocore::projects::Viewer>("");

	setupProjectView();
	setupLayerView();
	QObject::connect(project_.get(), &rocore::projects::Viewer::NameChanged,
					 this, &MainWindow::projectNameChanged);

	project_->SetName("untitled");

}

void MainWindow::projectNameChanged(QString name) {
    setWindowTitle("roviewer - " + name);
}

void MainWindow::setupProjectView()
{
	QDockWidget *dock = new QDockWidget(tr("Project"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	auto projView = new rocore::ui::ProjectView(project_,dock);

	dock->setWidget(projView);
	addDockWidget(Qt::LeftDockWidgetArea, dock);

	QObject::connect(project_.get(), &rocore::projects::Viewer::NameChanged,
					 projView, &rocore::ui::ProjectView::projectNameChanged);
}

void MainWindow::setupLayerView()
{
	QDockWidget *dock = new QDockWidget(tr("Layers"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	auto projView = new rocore::ui::LayerView(project_,dock);

	dock->setWidget(projView);
	addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::setupMenubar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open gerber"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the project to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);


    QAction *saveAsAct = fileMenu->addAction(tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the project under a new name"));

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    aboutAct->setStatusTip(tr("Show the application's About box"));
}

void MainWindow::open()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QStringList filters;
    filters << "Gerber (*.gbr *.ger *.gtl *.gbl *.gts *.gbs *.gto *.gbo *.gtp *.gbp)"
            << "Any files (*)";

    dialog.setNameFilters(filters);

    if(dialog.exec())
    {
        QString fileName = dialog.selectedFiles()[0];
        if (!fileName.isEmpty())
            LoadFile(fileName);
    }
}

bool MainWindow::save()
{
	return false;
}

bool MainWindow::saveAs()
{
	return false;
}

void MainWindow::about()
{
    About about;
    about.exec();
}

void MainWindow::LoadFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                                     .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
    QString inStr = in.readAll();


	rogerber::Gerber gerber;
	std::string data = inStr.toStdString();
	gerber.Parse(data);


	rocore::graphics::GraphicLayer* layer = new rocore::graphics::GraphicLayer(rocore::graphics::Colors::Red);

	GerberOpenGlProcessor processor(gerber, layer);
	processor.Execute();

	doc->AddLayer(layer);
}

MainWindow::~MainWindow()
{
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
}
