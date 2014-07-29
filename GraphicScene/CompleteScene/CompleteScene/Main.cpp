#include <Qt\qapplication.h>
#include <QtGui\qwidget.h>
#include <Qt\qdebug.h>
#include "GLWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	GLWindow widget;
	widget.show();
	return app.exec();
}