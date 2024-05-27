#include "SQliteDepository.h"

#include <qfileinfo.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMap>
inline std::shared_ptr< QSqlDatabase > make_QSqlDatabase( const QString &connectionName = "qt_sql_default_connection" ) {
	std::shared_ptr< QSqlDatabase > result;
	if( QSqlDatabase::contains( connectionName ) )
		result = std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::database( connectionName ) ), []( QSqlDatabase *p ) {
			if( p->isOpen( ) )
				p->close( );
			delete p;
		} );
	else
		result = std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE", connectionName ) ), []( QSqlDatabase *p ) {
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
		database = make_QSqlDatabase( linkPath );
		database->setDatabaseName( linkPath );
	} else
		database = nullptr;
}
cylDB::SQliteDepository::SQliteDepository( const QString &link_path ) {
	QFileInfo fileInfo( link_path );
	if( fileInfo.exists( ) && fileInfo.isFile( ) ) {
		linkPath = fileInfo.absoluteFilePath( );
		database = make_QSqlDatabase( linkPath );
		database->setDatabaseName( linkPath );
	}
}
cylDB::SQliteDepository::~SQliteDepository( ) {
}
cylDB::SQliteDepository::SQliteDepository( const std::shared_ptr< QSqlDatabase > &database, const QString &db_path ) {
	this->database = database;
	linkPath = db_path;
}
QString cylDB::SQliteDepository::getDBName( ) const {
	if( database )
		return linkPath;
	return "";
}
bool cylDB::SQliteDepository::hasTab( const QString &tab_name ) const {
	return database->tables( ).contains( tab_name );
}
bool cylDB::SQliteDepository::createTab( const QString &tab_name, const QVariantMap &tab_info, const QString &key, const QString &key_type ) const {
	if( !database || tab_info.isEmpty( ) || tab_name.isEmpty( ) )
		return false;
	if( key.isEmpty( ) )
		return createTab( tab_name, tab_info );

	QString createTabCmd( R"(CREATE TABLE IF NOT EXISTS `%1` ( )" );
	createTabCmd = createTabCmd.arg( tab_name );
	auto iterator = tab_info.begin( );
	auto end = tab_info.end( );
	QStringList strList;
	strList.append( createTabCmd );

	createTabCmd = "`%1` %2 PRIMARY KEY AUTOINCREMENT, ";
	strList.append( createTabCmd.arg( key ).arg( key_type ) );

	do {

		strList.append( " `" );
		strList.append( iterator.key( ) );
		strList.append( "` " );
		strList.append( iterator.value( ).toString( ) );
		++iterator;
		if( iterator == end ) {
			strList.append( u8" ); " );
			break;
		}
		strList.append( ", " );
	} while( true );
	QSqlQuery query( *database );
	createTabCmd = strList.join( ' ' );
	return query.exec( createTabCmd );
}
bool cylDB::SQliteDepository::createTab( const QString &tab_name, const QVariantMap &tab_info ) const {
	if( !database || tab_info.isEmpty( ) || tab_name.isEmpty( ) )
		return false;

	QString createTabCmd( R"(CREATE TABLE IF NOT EXISTS `%1` ( )" );
	createTabCmd = createTabCmd.arg( tab_name );
	auto iterator = tab_info.begin( );
	auto end = tab_info.end( );
	QStringList strList;
	strList.append( createTabCmd );
	do {

		strList.append( " `" );
		strList.append( iterator.key( ) );
		strList.append( "` " );
		strList.append( iterator.value( ).toString( ) );

		++iterator;
		if( iterator == end ) {
			strList.append( u8" ); " );
			break;
		}
		strList.append( ", " );
	} while( true );
	QSqlQuery query( *database );
	createTabCmd = strList.join( ' ' );
	return query.exec( createTabCmd );
}
QMap< QVariant, QVariant > cylDB::SQliteDepository::getTabInfo( const QString &tab_name ) const {
	return QMap< QVariant, QVariant >( );
}
bool cylDB::SQliteDepository::addItem( const QString &tab_name, const QString &item_name ) const {
	return false;
}
bool cylDB::SQliteDepository::removeItem( const QString &tab_name, const QString &item_name ) const {
	return false;
}
bool cylDB::SQliteDepository::appendData( const QString &tab_name, QVector< QVariant > variants ) const {
	return false;
}
bool cylDB::SQliteDepository::updateData( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) const {
	return false;
}
bool cylDB::SQliteDepository::removeData( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) const {
	return false;
}
void cylDB::SQliteDepository::setUserInfo( const QString &user, const QString &password ) {

}
bool cylDB::SQliteDepository::isOpen( ) const {
	if( database )
		return database->isOpen( );
	return false;
}
bool cylDB::SQliteDepository::open( ) const {
	if( database ) {
		if( database->isOpen( ) || database->open( ) )
			return true;
		return database->open( user, password );
	}
	return false;
}
bool cylDB::SQliteDepository::open( const QString &user, const QString &password ) {
	if( database )
		if( database->isOpen( ) || database->open( ) || database->open( user, password ) ) {
			this->user = user;
			this->password = password;
			return true;
		}
	return false;
}
bool cylDB::SQliteDepository::close( ) const {
	if( database ) {
		database->close( );
		return true;
	}
	return false;
}
bool cylDB::SQliteDepository::commit( ) const {
	return database->commit( );
}
bool cylDB::SQliteDepository::isOpenError( ) const {
	return database->isOpen( );
}
QSqlError cylDB::SQliteDepository::getLastError( ) const {
	return database->lastError( );
}
cylDB::Depository_Shared cylDB::SQliteDepository::createDepository( const QString &db_path ) {
	QFileInfo fileInfo( db_path );
	if( fileInfo.exists( ) && fileInfo.isFile( ) )
		return std::make_shared< SQliteDepository >( db_path );
	auto dbPath = fileInfo.absoluteFilePath( );
	auto linkDatabase = make_QSqlDatabase( db_path );;
	linkDatabase->setDatabaseName( dbPath );
	bool openStatus = linkDatabase->isOpen( );
	if( openStatus )
		return std::shared_ptr< SQliteDepository >( new SQliteDepository( linkDatabase, dbPath ) );

	openStatus = linkDatabase->open( );
	if( openStatus )
		return std::shared_ptr< SQliteDepository >( new SQliteDepository( linkDatabase, dbPath ) );

	return nullptr;
}
cylDB::Depository_Shared cylDB::SQliteDepository::createDepository( const QString &db_path, const QString &user, const QString &password ) {
	QFileInfo fileInfo( db_path );
	if( fileInfo.exists( ) && fileInfo.isFile( ) )
		return std::make_shared< SQliteDepository >( db_path );
	auto linkDatabase = make_QSqlDatabase( db_path );;
	linkDatabase->setDatabaseName( db_path );
	if( !linkDatabase->isOpen( ) && !linkDatabase->open( ) && !linkDatabase->open( user, password ) )
		return nullptr;
	std::shared_ptr< SQliteDepository > qliteDepository( new SQliteDepository( linkDatabase, db_path ) );
	qliteDepository->user = user;
	qliteDepository->password = password;
	return qliteDepository;
}
