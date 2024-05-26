#include <QApplication>
int main( int argc, char *argv[ ] ) {
	QApplication app( argc, argv );
	
	qDebug( ) << "\n\n\n\n=============\n" << "int main( int argc, char *argv[ ] ) ";
	qDebug( ) << "return app.exec( );";
	return app.exec( );
}
