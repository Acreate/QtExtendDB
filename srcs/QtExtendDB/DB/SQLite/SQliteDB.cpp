#include "SQliteDB.h"
#include <QVector>
#include <QDir>
#include "SQliteDepository/SQliteDepository.h"
using namespace cylDB;
SQliteDB::SQliteDB( const QString &link ) {
	linkPath = QFileInfo( link ).absoluteFilePath( );
}
SQliteDB::~SQliteDB( ) {
}
bool SQliteDB::link( ) {
	return isLink( );
}
bool SQliteDB::link( const QString &link ) {
	QFileInfo qFileInfo( link );
	if( qFileInfo.isDir( ) && qFileInfo.exists( ) ) {
		linkPath = qFileInfo.absoluteFilePath( );
		return true;
	}
	return false;
}
bool SQliteDB::isLink( ) {
	QFileInfo qFileInfo( linkPath );
	if( qFileInfo.isDir( ) && qFileInfo.exists( ) )
		return true;
	return false;
}
bool SQliteDB::hasDepository( const QString &db_name ) {
	auto dbPath = linkPath + QDir::separator( ) + db_name;
	SQliteDepository depository( dbPath );
	if( depository.open( ) ) {
		depository.close( );
		return true;
	}
	return false;
}
Depository_Shared SQliteDB::openDepository( const QString &db_name ) {
	auto dbPath = linkPath + QDir::separator( ) + db_name;
	if( QFile::exists( dbPath ) ) // 存在，则直接打开
		return std::make_shared< SQliteDepository >( dbPath );
	// 不存在，则创建
	return SQliteDepository::createDepository( dbPath );
}
QVector< Depository_Shared > SQliteDB::getDepositorys( ) {
	QVector< Depository_Shared > result;
	QDir qDir( linkPath );
	auto entryInfoList = qDir.entryInfoList( QDir::Files );
	for( auto &info : entryInfoList ) {
		QString absoluteFilePath = info.absoluteFilePath( );
		auto depository = std::make_shared< SQliteDepository >( absoluteFilePath );
		if( !depository->isOpen( ) && !depository->open( ) )
			continue;
		result.emplaceBack( depository );
	}
	return result;
}
