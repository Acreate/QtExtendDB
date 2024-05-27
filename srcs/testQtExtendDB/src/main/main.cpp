#include <QApplication>
#include <DB/DBTools.h>
#include <DB/dbInterface/depositoryInterface/I_Depository.h>
#include <DB/dbInterface/I_DB.h>
#include <qdatetime.h>

#include "../auto_generate_files/macro/cmake_to_c_cpp_header_env.h"

int main( int argc, char *argv[ ] ) {
	QApplication app( argc, argv );
	QString sqlitePath( u8"test.db" );
	auto dbInterface = cylDB::DBTools::linkDB( Cache_Path_Dir );
	if( !dbInterface->link( ) ) {
		qDebug( ) << u8"无法链接到目标";
		return 0;
	}
	auto depositorys = dbInterface->getDepositorys( );
	if( depositorys.size( ) == 0 ) {
		qDebug( ) << u8"没有数据库 -> 尝试打开一个有利的数据库";
		QString dbName = QDateTime::currentDateTime( ).toString( u8"yyyy_MM_dd hh-mm-ss" ).append( ".db" );
		auto depository = dbInterface->openDepository( dbName );
		if( depository && depository->isOpen( ) )
			qDebug( ) << '\t' << dbName << u8" 打开成功";
		else
			qDebug( ) << '\t' << dbName << u8" 打开失败";
	} else {
		qDebug( ) << u8"找到数据库:";
		for( auto &depository : depositorys ) {
			QString dbName = depository->getDBName( );
			if( dbName.isEmpty( ) ) {
				qDebug( ) << u8"\t数据库不存在";
			} else
				qDebug( ) << '\t' << dbName.toStdString( ).c_str( );
		}
	}
	qDebug( ) << "\n\n\n\n=============\n" << "int main( int argc, char *argv[ ] ) ";
	qDebug( ) << "return app.exec( );";
	return 0;
}
