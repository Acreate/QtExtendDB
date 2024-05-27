#include <QApplication>
#include <DB/DBTools.h>
#include <DB/dbInterface/i_depositoryInterface/I_Depository.h>
#include <DB/dbInterface/i_resultInfo/I_ResultInfo.h>
#include <DB/dbInterface/I_DB.h>
#include <qdatetime.h>
#include <QDir>
#include <qfileinfo.h>

#include "../auto_generate_files/macro/cmake_to_c_cpp_header_env.h"

int main( int argc, char *argv[ ] ) {
	QApplication app( argc, argv );
	QString sqlitePath( u8"test.db" );
	auto dbInterface = cylDB::DBTools::linkDB( Cache_Path_Dir );
	if( !dbInterface->link( ) ) {
		qDebug( ) << u8"无法链接到目标";
		return 0;
	}
	auto depositoryShared = dbInterface->openDepository( sqlitePath );
	if( depositoryShared ) {
		qDebug( ) << u8"打开 : " << depositoryShared->getDBName( ) << u8" 成功";
	} else {
		qDebug( ) << u8"打开 : " << dbInterface->getLink( ) << QDir::separator << sqlitePath << u8" 失败";
	}
	auto depositorys = dbInterface->getDepositorys( );
	if( depositorys.size( ) != 0 ) {
		qDebug( ) << u8"找到数据库:";
		for( auto &depository : depositorys ) {
			QString dbName = depository->getDBName( );
			if( dbName.isEmpty( ) ) {
				qDebug( ) << u8"\t数据库不存在";
			} else
				qDebug( ) << '\t' << dbName.toStdString( ).c_str( );
		}
	}
	qDebug( ) << u8"===========================================";
	{
		QString path( u8"%1%2%3" );
		path = path.arg( Cache_Path_Dir ).arg( Cmake_Project_Name ).arg( u8".db" );
		QFileInfo sigDb( path );
		path = sigDb.absoluteFilePath( );
		if( sigDb.exists( ) ) {
			qDebug( ) << u8" -> 删除已经存在的库\t-" << path;
			bool remove = QFile::remove( path );
			if( !remove )
				qDebug( ) << u8" -> 删除已经存在的库\t-失败";
		}

		qDebug( ) << u8" -> 尝试创建一个有利的数据库\t2";
		auto depository = dbInterface->openAbsoluteFilePathDepository( path );
		if( depository ) {
			if( depository->isOpen( ) ) {
				qDebug( ) << '\t' << depository->getDBName( ).toStdString( ).c_str( ) << u8" 尝试成功";
				QString newTabName = u8"url";
				bool tab = depository->createTab( newTabName
					, { { "one", "TEXT" }, { "two", "TEXT" } }
					, "key"
					, "INTEGER" );
				qDebug( ) << u8"创建 (" << newTabName << ") : " << tab;
				if( tab ) {
					qDebug( ) << u8"检查 (" << newTabName << ") : " << depository->hasTab( newTabName );
					auto resultInfoShared = depository->getTabInfo( newTabName );
					if( resultInfoShared ) {
						resultInfoShared->resetColIndex( );
						do {
							auto currentRows = resultInfoShared->getCurrentRows( );
							for( auto &var : *currentRows )
								qDebug( ) << u8"=>\t" << *var;
							qDebug( ) << u8"-------------";
						} while( resultInfoShared->nextCol( ) );
					}
				} else
					qDebug( ) << u8"创建 (" << newTabName << ") : 失败";
			} else
				qDebug( ) << u8"数据库 (" << path << ") 错误 : " << depository->getLastError( );
			depository->close( );
		} else {
			qDebug( ) << '\t' << path << u8" 尝试失败";
		}
	}
	qDebug( ) << "\n\n\n\n=============\n" << "int main( int argc, char *argv[ ] ) ";
	qDebug( ) << "return app.exec( );";
	return 0;
}
