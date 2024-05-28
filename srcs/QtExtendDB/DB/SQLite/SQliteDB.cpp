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
QString SQliteDB::getLink( ) {
	return linkPath;
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

Depository_Shared SQliteDB::openAbsoluteFilePathDepository( const QString &absoluteFilePath ) {
	if( QFile::exists( absoluteFilePath ) ) {
		auto qliteDepository = std::make_shared< SQliteDepository >( absoluteFilePath );
		// 存在，则直接打开
		if( qliteDepository->isOpen( ) || qliteDepository->open( ) )
			return qliteDepository;
	}
	// 不存在，则创建
	return SQliteDepository::createDepository( absoluteFilePath );
}

Depository_Shared SQliteDB::openAbsoluteFilePathDepository( const QString &absoluteFilePath, const QString &name, const QString &password ) const {
	if( QFile::exists( absoluteFilePath ) ) {
		auto qliteDepository = std::make_shared< SQliteDepository >( absoluteFilePath );
		// 存在，则直接打开
		if( qliteDepository->isOpen( ) || qliteDepository->open( ) || qliteDepository->open( name, password ) ) {
			qliteDepository->setUserInfo( name, password );
			return qliteDepository;
		}
	}
	// 不存在，则创建
	return SQliteDepository::createDepository( absoluteFilePath, name, password );

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
