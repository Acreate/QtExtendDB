#include "SQliteDepository.h"

#include <qfileinfo.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMap>
inline std::shared_ptr< QSqlDatabase > make_QSqlDatabase( ) {
	std::shared_ptr< QSqlDatabase > result;
	if( QSqlDatabase::contains( "qt_sql_default_connection" ) )
		result = std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::database( "qt_sql_default_connection" ) ), []( QSqlDatabase *p ) {
			if( p->isOpen( ) )
				p->close( );
			delete p;
		} );
	else
		result = std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE" ) ), []( QSqlDatabase *p ) {
			if( p->isOpen( ) )
				p->close( );
			delete p;
		} );
	return result;
}
cylDB::SQliteDepository::SQliteDepository( const QString &link_path, const QString &user, const QString &password ) {

	QFileInfo fileInfo( link_path );
	linkPath = fileInfo.absoluteFilePath( );
	this->user = user;
	this->password = password;


	if( fileInfo.exists( ) && fileInfo.isFile( ) ) {
		database = make_QSqlDatabase( );
		database->setDatabaseName( linkPath );
	} else
		database = nullptr;
}
cylDB::SQliteDepository::SQliteDepository( const QString &link_path ) {
	QFileInfo fileInfo( link_path );
	if( fileInfo.exists( ) && fileInfo.isFile( ) ) {
		linkPath = fileInfo.absoluteFilePath( );
		database = make_QSqlDatabase( );
		database->setDatabaseName( linkPath );
	}
}
cylDB::SQliteDepository::~SQliteDepository( ) {
}
cylDB::SQliteDepository::SQliteDepository( const std::shared_ptr< QSqlDatabase > &database, const QString &db_path ) {
	this->database = database;
	linkPath = db_path;
}
QString cylDB::SQliteDepository::getDBName( ) {
	if( database )
		return linkPath;
	return "";
}
bool cylDB::SQliteDepository::hasTab( const QString &tab_name ) {
	return false;
}
bool cylDB::SQliteDepository::createTab( const QString &tab_name ) {
	return false;
}
QMap< QVariant, QVariant > cylDB::SQliteDepository::getTabInfo( const QString &tab_name ) {
	return QMap< QVariant, QVariant >( );
}
bool cylDB::SQliteDepository::addItem( const QString &tab_name, const QString &item_name ) {
	return false;
}
bool cylDB::SQliteDepository::removeItem( const QString &tab_name, const QString &item_name ) {
	return false;
}
bool cylDB::SQliteDepository::appendData( const QString &tab_name, QVector< QVariant > variants ) {
	return false;
}
bool cylDB::SQliteDepository::updateData( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) {
	return false;
}
bool cylDB::SQliteDepository::removeData( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) {
	return false;
}
void cylDB::SQliteDepository::setUserInfo( const QString &user, const QString &password ) {

}
bool cylDB::SQliteDepository::isOpen( ) {
	if( database )
		return database->isOpen( );
	return false;
}
bool cylDB::SQliteDepository::open( ) {
	if( database ) {
		if( database->open( ) )
			return true;
		return database->open( user, password );
	}
	return false;
}
bool cylDB::SQliteDepository::open( const QString &user, const QString &password ) {
	if( database )
		if( database->open( user, password ) ) {
			this->user = user;
			this->password = password;
			return true;
		}
	return false;
}
bool cylDB::SQliteDepository::close( ) {
	if( database ) {
		database->close( );
		return true;
	}
	return false;
}
cylDB::Depository_Shared cylDB::SQliteDepository::createDepository( const QString &db_path ) {
	QFileInfo fileInfo( db_path );
	if( fileInfo.exists( ) && fileInfo.isFile( ) )
		return std::make_shared< SQliteDepository >( db_path );

	auto linkDatabase = make_QSqlDatabase( );;
	linkDatabase->setDatabaseName( db_path );
	if( !linkDatabase->isOpen( ) && !linkDatabase->open( ) )
		return nullptr;
	return std::shared_ptr< SQliteDepository >( new SQliteDepository( linkDatabase, db_path ) );
}
