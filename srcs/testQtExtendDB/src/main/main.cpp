#include <QApplication>
#include <DB/DBTools.h>
#include <DB/dbInterface/i_depositoryInterface/I_Depository.h>
#include <DB/dbInterface/i_resultInfo/I_ResultInfo.h>
#include <DB/dbInterface/i_tabInfo/I_TabInfo.h>
#include <DB/dbInterface/I_DB.h>
#include <qdatetime.h>
#include <QDir>
#include <qfileinfo.h>

#include "../auto_generate_files/macro/cmake_to_c_cpp_header_env.h"

void outDebug( cylDB::IResultInfo_Shared &result_info_shared ) {
	if( result_info_shared ) {
		result_info_shared->resetColIndex( );
		auto sharedPtrs = result_info_shared->getCurrentRows( );
		size_t count = 1;
		if( sharedPtrs->size( ) != 0 ) {
			qDebug( ) << "∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵∵";
			QStringList buffMsg;
			do {
				for( auto &row : *sharedPtrs )
					buffMsg.append( row->toString( ) );
				qDebug( ) << count << '\t' << buffMsg.join( ", " ).toStdString( ).c_str( );
				bool cond = !result_info_shared->nextCol( );
				if( cond )
					break;
				sharedPtrs = result_info_shared->getCurrentRows( );
				if( sharedPtrs->size( ) == 0 )
					break;
				buffMsg.clear( );
				++count;
			} while( true );
			qDebug( ) << "∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴∴";
			return;
		}
	}
	qDebug( ) << "\t" << "result_info_shared 值为 nullptr";

}

void dbremoveTabTest( cylDB::Depository_Shared &db_shared ) {
	qDebug( ) << u8"\n\n===============->	删除表测试 -dbremoveTabTest( cylDB::Depository_Shared &db_shared )";
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
	qDebug( ) << u8"\n----------------------\n";
	auto newTabName2 = u8"tab2";
	tab = db_shared->createTab( newTabName2
		, { { "one", "TEXT" }, { "two", "TEXT" } }
		, "key"
		, "INTEGER" );
	qDebug( ) << u8"创建 (" << newTabName2 << ") : " << tab;
	if( tab ) {
		qDebug( ) << u8"创建 (" << newTabName2 << ") : 成功";
		qDebug( ) << u8"增加表测试 : ";
		auto resultInfo = db_shared->getAllTab( );
		outDebug( resultInfo );
	} else
		qDebug( ) << u8"创建 (" << newTabName2 << ") : 失败";

	qDebug( ) << u8"\n----------------------\n";
	tab = db_shared->removeTab( newTabName );
	qDebug( ) << u8"删除 (" << newTabName << ") : " << tab;
	if( tab ) {
		qDebug( ) << u8"删除 (" << newTabName << ") : 成功";
		qDebug( ) << u8"删除表测试 : ";
		auto resultInfo = db_shared->getAllTab( );
		outDebug( resultInfo );
	} else
		qDebug( ) << u8"删除 (" << newTabName << ") : 失败";

	qDebug( ) << u8"\n\n测试完毕	->==================================\n";
}

void dbTabTest( cylDB::Depository_Shared &db_shared ) {
	qDebug( ) << u8"\n\n===============->	表测试 -dbTabTest( cylDB::Depository_Shared &db_shared )";
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
	qDebug( ) << u8"\n----------------------\n";
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
	qDebug( ) << u8"\n\n测试完毕	->==================================\n";
}

void dbItemTest( cylDB::Depository_Shared &db_shared ) {
	qDebug( ) << u8"\n\n===============->	项测试 -dbItemTest( cylDB::Depository_Shared &db_shared )\n";
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

		qDebug( ) << u8"\n\n----------------------\n";
		addItem = db_shared->addItem(
			newTabName
			, { "one", "two", "three", "age" }
			, { R"("1234 中")", R"("444 文")", R"("中文 2 ")", R"("99")" } );
		qDebug( ) << u8"\t增加项 2 : " << addItem;
		auto where = "age = 99";
		resultInfo = db_shared->findItems( newTabName, where );
		outDebug( resultInfo );
		qDebug( ) << u8"\n\n----------------------\n";
		qDebug( ) << u8"删除项测试 : ";
		qDebug( ) << u8"\t获取所有成员 : ";
		resultInfo = db_shared->findItems( newTabName );
		outDebug( resultInfo );
		qDebug( ) << u8"\t删除指定成员 : " << where;
		auto subItem = db_shared->removeItem( newTabName, where );
		if( subItem ) {
			qDebug( ) << u8"\t删除成功 2 : " << addItem;
			qDebug( ) << u8"\t获取所有成员 : ";
			resultInfo = db_shared->findItems( newTabName );
			outDebug( resultInfo );
		} else
			qDebug( ) << u8"\t删除失败 2 : " << addItem;

		qDebug( ) << u8"\n\n----------------------\n";

		qDebug( ) << u8"更新项测试 : ";
		qDebug( ) << u8"\t获取所有成员 : ";
		resultInfo = db_shared->findItems( newTabName );
		outDebug( resultInfo );
		where = "age = 22";
		qDebug( ) << u8"\t更新指定成员 : " << where;
		auto updateItem = db_shared->updateItem( newTabName, { { "two", R"("更新了")" } }, where );
		if( updateItem ) {

			qDebug( ) << u8"\t更新成功 3 : " << addItem;
			qDebug( ) << u8"\t获取所有成员 : ";
			resultInfo = db_shared->findItems( newTabName );
			outDebug( resultInfo );
		} else
			qDebug( ) << u8"\t更新失败 3 : " << addItem;
		qDebug( ) << u8"\t更新所有成员 : ";
		updateItem = db_shared->updateItem( newTabName, { { "two", R"("更新了2")" }, { "one", R"("更新了2")" } } );
		if( updateItem ) {
			qDebug( ) << u8"\t更新成功 3 : " << addItem;
			qDebug( ) << u8"\t获取所有成员 : ";
			resultInfo = db_shared->findItems( newTabName );
			outDebug( resultInfo );
		} else
			qDebug( ) << u8"\t更新失败 3 : " << addItem;

		qDebug( ) << u8"\t删除所有成员 : ";
		subItem = db_shared->removeItem( newTabName );
		if( subItem ) {
			qDebug( ) << u8"\t删除成功 2 : " << subItem;
			qDebug( ) << u8"\t获取所有成员 : ";
			resultInfo = db_shared->findItems( newTabName );
			outDebug( resultInfo );
		} else
			qDebug( ) << u8"\t删除失败 2 : " << addItem;
	} else
		qDebug( ) << u8"创建 (" << newTabName << ") : 失败";
	qDebug( ) << u8"\n\n测试完毕	->==================================";
}

void dbreConverTabTest( const cylDB::Depository_Shared &db_depository_shared ) {
	qDebug( ) << u8"\n\n===============->	表转换对象 -dbreConverTabTest( cylDB::Depository_Shared &db_shared )\n";
	auto tabName = QDateTime::currentDateTime( ).toString( "yyyy_MM_dd-hh:mm:ss" );
	auto isCtreate = db_depository_shared->createTab( tabName, { { "type", "TEXT" } }, "key", "INTEGER" );
	if( isCtreate ) {
		qDebug( ) << u8"创建成功 :" << tabName.toStdString( ).c_str( );
		auto resultInfoShared = db_depository_shared->getAllTab( );
		if( resultInfoShared ) {
			outDebug( resultInfoShared );
			auto tabInfo = db_depository_shared->converTab( tabName );
			if( tabInfo ) {
				qDebug( ) << "============\t输出标头名称信息  ===  单个";
				auto vector = tabInfo->getIndexs( );
				for( auto index : vector )
					qDebug( ) << "\t" << index << " : " << tabInfo->getTitleIndexName( index )->toString( ).toStdString( ).c_str( );
				qDebug( ) << "============\t输出标头名称结束  ===  单个";
			}


			db_depository_shared->createTab( "tabName 1", { { "type", "TEXT" } }, "key", "INTEGER" );
			db_depository_shared->createTab( "tabName 2", { { "type", "TEXT" } }, "key", "INTEGER" );
			db_depository_shared->createTab( "tabName 3", { { "type", "TEXT" } }, "key", "INTEGER" );
			db_depository_shared->createTab( "tabName 4", { { "type", "TEXT" } }, "key", "INTEGER" );
			auto tabInfos = db_depository_shared->converAllTab( );
			if( tabInfos ) {
				qDebug( ) << "============\t输出标头名称信息  ===  多个";
				for( auto &tabSPtr : *tabInfos ) {
					auto vector = tabSPtr->getIndexs( );
					for( auto index : vector )
						qDebug( ) << "\t[" << tabSPtr->getName( ) << "." << index << "]: " << tabInfo->getTitleIndexName( index )->toString( ).toStdString( ).c_str( );
					qDebug( ) << "----------------";
				}
				qDebug( ) << "============\t输出标头名称结束  ===  多个";
			}

			qDebug( ) << "============\t输出支持类型  ===";
			auto supportType = tabInfo->getSupportType( );
			auto iterator = supportType.begin( );
			auto end = supportType.end( );
			for( ; iterator != end; ++iterator )
				qDebug( ) << "\t( " << iterator.key( ).toStdString( ).c_str( ) << " : " << iterator.value( ) << ") ";

			qDebug( ) << "============\t输出支持类型结束  ===";
		} else
			qDebug( ) << u8"没有任何表";
	} else
		qDebug( ) << u8"创建失败 :" << tabName.toStdString( ).c_str( );
	qDebug( ) << u8"\n\n测试完毕	->==================================";
}
void dbItemTest_db( cylDB::DB_Shared dbInterface ) {
	QString sqlitePath = "dbItemTest.db";
	cylDB::Depository_Shared depositoryShared = dbInterface->openDepository( sqlitePath );
	if( depositoryShared ) {
		qDebug( ) << u8"打开 : " << depositoryShared->getDBName( ) << u8" 成功";
		dbItemTest( depositoryShared );
	} else {
		qDebug( ) << u8"打开 : " << dbInterface->getLink( ) << QDir::separator << sqlitePath << u8" 失败";
	}
}
void dbTabTest_db( cylDB::DB_Shared dbInterface ) {
	QString sqlitePath = "dbTabTest.db";
	cylDB::Depository_Shared depositoryShared = dbInterface->openDepository( sqlitePath );
	if( depositoryShared ) {
		qDebug( ) << u8"打开 : " << depositoryShared->getDBName( ) << u8" 成功";
		dbreConverTabTest( depositoryShared );
	} else {
		qDebug( ) << u8"打开 : " << dbInterface->getLink( ) << QDir::separator << sqlitePath << u8" 失败";
	}
}
void dbreConverTabTest_db( cylDB::DB_Shared dbInterface ) {
	QString sqlitePath = "dbreConverTabTest.db";
	cylDB::Depository_Shared depositoryShared = dbInterface->openDepository( sqlitePath );
	if( depositoryShared ) {
		qDebug( ) << u8"打开 : " << depositoryShared->getDBName( ) << u8" 成功";
		dbreConverTabTest( depositoryShared );
	} else {
		qDebug( ) << u8"打开 : " << dbInterface->getLink( ) << QDir::separator << sqlitePath << u8" 失败";
	}
}
void dbremoveTabTest_db( cylDB::DB_Shared dbInterface ) {
	QString sqlitePath = u8"dbremoveTabTest.db";
	cylDB::Depository_Shared depositoryShared = dbInterface->openDepository( sqlitePath );
	if( depositoryShared ) {
		qDebug( ) << u8"打开 : " << depositoryShared->getDBName( ) << u8" 成功";
		dbremoveTabTest( depositoryShared );
	} else {
		qDebug( ) << u8"打开 : " << dbInterface->getLink( ) << QDir::separator << sqlitePath << u8" 失败";
	}
}
#include <typeinfo>
int main( int argc, char *argv[ ] ) {
	QApplication app( argc, argv );
	const auto &typeInfo = typeid( int );

	QDir dir( Cache_Path_Dir );
	auto entryInfoList = dir.entryInfoList( QDir::Files );
	for( auto &fileInfo : entryInfoList ) {
		QString absoluteFilePath = fileInfo.absoluteFilePath( );
		bool remove = QFile::remove( absoluteFilePath );
		qDebug( ) << u8"删除文件(" << absoluteFilePath << "):" << ( remove ? u8"成功" : u8"失败" );
	}
	qDebug( ) << u8"=================== 进程开始测试 ========================";

	auto dbInterface = cylDB::DBTools::linkDB( Cache_Path_Dir );
	if( !dbInterface->link( ) ) {
		qDebug( ) << u8"无法链接到目标";
		return 0;
	}

	//dbremoveTabTest_db( dbInterface );
	//qDebug( ) << u8"===========================================";
	dbreConverTabTest_db( dbInterface );
	//qDebug( ) << u8"===========================================";
	//dbTabTest_db( dbInterface );
	//qDebug( ) << u8"===========================================";
	//dbItemTest_db( dbInterface );

	qDebug( ) << u8"=================== 进程结束测试 ========================";
	qDebug( ) << "\n\n\n\n=============\n" << "int main( int argc, char *argv[ ] ) ";
	qDebug( ) << "return app.exec( );";
	return 0;
}
