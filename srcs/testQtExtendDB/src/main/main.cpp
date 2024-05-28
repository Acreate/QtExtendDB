#include <QApplication>
#include <DB/DBTools.h>
#include <DB/dbInterface/i_depositoryInterface/I_Depository.h>
#include <DB/dbInterface/i_resultInfo/I_ResultInfo.h>
#include <DB/dbInterface/I_DB.h>
#include <qdatetime.h>
#include <QDir>
#include <qfileinfo.h>

#include "../auto_generate_files/macro/cmake_to_c_cpp_header_env.h"

void outDebug( cylDB::IResultInfo_Shared &result_info_shared ) {
	if( result_info_shared ) {
		result_info_shared->resetColIndex( );
		do {
			auto sharedPtrs = result_info_shared->getCurrentRows( );
			for( auto &row : *sharedPtrs )
				qDebug( ) << "\t" << *row;
		} while( result_info_shared->nextCol( ) );
	} else {
		qDebug( ) << "\t" << "result_info_shared 值为 nullptr";
	}
}

void dbTabTest( cylDB::Depository_Shared &db_shared ) {
	qDebug( ) << u8"===============->	表测试 -dbTabTest( cylDB::Depository_Shared &db_shared )";
	QString newTabName = u8"tab1";
	bool tab = db_shared->createTab( newTabName
		, { { "one", "TEXT" }, { "two", "TEXT" } } );
	qDebug( ) << u8"创建 (" << newTabName << ") : " << tab;
	if( tab ) {
		qDebug( ) << u8"创建 (" << newTabName << ") : 成功";
		qDebug( ) << u8"增加表测试 : ";
		auto resultInfo = db_shared->getAllTab( );
		outDebug( resultInfo );
	} else
		qDebug( ) << u8"创建 (" << newTabName << ") : 失败";
	qDebug( ) << u8"----------------------";
	newTabName = u8"tab2";
	tab = db_shared->createTab( newTabName
		, { { "one", "TEXT" }, { "two", "TEXT" } }
		, "key"
		, "INTEGER" );
	qDebug( ) << u8"创建 (" << newTabName << ") : " << tab;
	if( tab ) {
		qDebug( ) << u8"创建 (" << newTabName << ") : 成功";
		qDebug( ) << u8"增加表测试 : ";
		auto resultInfo = db_shared->getAllTab( );
		outDebug( resultInfo );
	} else
		qDebug( ) << u8"创建 (" << newTabName << ") : 失败";
	qDebug( ) << u8"测试完毕	->==================================";
}

void dbItemTest( cylDB::Depository_Shared &db_shared ) {
	qDebug( ) << u8"===============->	项测试 -dbItemTest( cylDB::Depository_Shared &db_shared )";
	QString newTabName = u8"testItems";
	bool tab = db_shared->createTab( newTabName
		, { { "one", "TEXT" }, { "two", "TEXT" }, { "three", "TEXT" }, { "age", "INTEGER" } } );
	qDebug( ) << u8"创建 (" << newTabName << ") : " << tab;

	if( tab ) {
		qDebug( ) << u8"增加项测试 : ";
		auto addItem = db_shared->addItem(
			newTabName
			, { "one", "two", "three", "age" }
			, { R"("1234")", R"("444")", R"("中文")", R"("22")" } );
		qDebug( ) << u8"\t增加项 1 : " << addItem;
		auto resultInfo = db_shared->findItems( newTabName );
		outDebug( resultInfo );

		qDebug( ) << u8"----------------------";
		addItem = db_shared->addItem(
			newTabName
			, { "one", "two", "three", "age" }
			, { R"("1234 中")", R"("444 文")", R"("中文 2 ")", R"("99")" } );
		qDebug( ) << u8"\t增加项 2 : " << addItem;
		resultInfo = db_shared->findItems( newTabName, "age = 99" );
		outDebug( resultInfo );
	} else
		qDebug( ) << u8"创建 (" << newTabName << ") : 失败";
	qDebug( ) << u8"测试完毕	->==================================";
}

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
		if( sigDb.exists( ) )
			qDebug( ) << u8" -> 尝试打开 " << path;
		else
			qDebug( ) << u8" -> 尝试创建一个有利的数据库\t2";
		auto depository = dbInterface->openAbsoluteFilePathDepository( path );
		if( depository ) {
			if( depository->isOpen( ) ) {
				qDebug( ) << '\t' << depository->getDBName( ).toStdString( ).c_str( ) << u8" 尝试成功";
				dbTabTest( depository );
				dbItemTest( depository );
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
