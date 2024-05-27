#include "SQliteDepository.h"
#include "../sqliteResult/SQLiteResult.h"
#include <qfileinfo.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlResult>
#include <QSqlQuery>
#include <QMap>

using namespace cylDB;

inline std::shared_ptr< QSqlDatabase > make_QSqlDatabase( const QString &connectionName = "qt_sql_default_connection" ) {
	std::shared_ptr< QSqlDatabase > result;
	if( QSqlDatabase::contains( connectionName ) )
		result = std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::database( connectionName ) )
			, []( QSqlDatabase *p ) {
				if( p->isOpen( ) )
					p->close( );
				delete p;
			} );
	else
		result = std::shared_ptr< QSqlDatabase >( new QSqlDatabase( QSqlDatabase::addDatabase( "QSQLITE", connectionName ) )
			, []( QSqlDatabase *p ) {
				if( p->isOpen( ) )
					p->close( );
				delete p;
			} );
	return result;
}

inline IResultInfo_Shared get_QSqlQuery_result( QSqlQuery &q_sql_result ) {
	bool next = q_sql_result.next( );
	if( next ) {
		SQLiteResult *sqLiteResult = new SQLiteResult;
		do {
			for( int index = 0; true; ++index ) {
				auto value = q_sql_result.value( index );
				if( !value.isValid( ) )
					break;
				sqLiteResult->appendRwo( value );
			}
			sqLiteResult->newCol( );
			next = q_sql_result.next( );
		} while( next );
		return std::shared_ptr< I_ResultInfo >( sqLiteResult );
	}
	return nullptr;
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

	createTabCmd.append( '`' );
	createTabCmd.append( key );
	createTabCmd.append( '`' );
	createTabCmd.append( key_type );
	createTabCmd.append( "  PRIMARY KEY AUTOINCREMENT, " );
	do {
		createTabCmd.append( " `" );
		createTabCmd.append( iterator.key( ) );
		createTabCmd.append( "` " );
		createTabCmd.append( iterator.value( ).toString( ) );
		++iterator;
		if( iterator == end ) {
			createTabCmd.append( u8" ); " );
			break;
		}
		createTabCmd.append( ", " );
	} while( true );
	QSqlQuery query( *database );
	return query.exec( createTabCmd );
}
bool cylDB::SQliteDepository::createTab( const QString &tab_name, const QVariantMap &tab_info ) const {
	if( !database || tab_info.isEmpty( ) || tab_name.isEmpty( ) )
		return false;

	QString createTabCmd( R"(CREATE TABLE IF NOT EXISTS `%1` ( )" );
	createTabCmd = createTabCmd.arg( tab_name );
	auto iterator = tab_info.begin( );
	auto end = tab_info.end( );
	do {

		createTabCmd.append( " `" );
		createTabCmd.append( iterator.key( ) );
		createTabCmd.append( "` " );
		createTabCmd.append( iterator.value( ).toString( ) );

		++iterator;
		if( iterator == end ) {
			createTabCmd.append( u8" ); " );
			break;
		}
		createTabCmd.append( ", " );
	} while( true );
	QSqlQuery query( *database );
	return query.exec( createTabCmd );
}
IResultInfo_Shared cylDB::SQliteDepository::getTabInfo( const QString &tab_name ) const {
	if( database ) {
		QString cmd = R"( PRAGMA table_info( `%1` ); )";
		cmd = cmd.arg( tab_name );
		QSqlQuery query( *database );
		if( query.exec( cmd ) )
			return get_QSqlQuery_result( query );
	}
	return nullptr;
}
IResultInfo_Shared SQliteDepository::getAllTab( ) const {
	if( database ) {
		QSqlQuery query( *database );
		if( query.exec( R"(SELECT * FROM sqlite_master ;)" ) )
			return get_QSqlQuery_result( query );
	}
	return nullptr;
}
bool SQliteDepository::addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value ) const {
	if( database ) {
		if( item_name.size( ) == 0 || item_value.size( ) == 0 )
			return false;

		QString cmd = R"(INSERT INTO )";
		cmd.append( tab_name ).append( "( " );
		auto iterator = item_name.begin( );
		auto end = item_name.end( );
		do {
			cmd.append( *iterator );
			++iterator;
			if( iterator == end )
				break;
			cmd.append( ", " );
		} while( true );
		cmd.append( " ) VALUES ( " );
		iterator = item_value.begin( );
		end = item_value.end( );
		do {
			cmd.append( *iterator );
			++iterator;
			if( iterator == end )
				break;
			cmd.append( " , " );
		} while( true );

		cmd.append( " )" );
		QSqlQuery query( *database );
		qDebug( ) << cmd;
		bool exec = query.exec( cmd );
		if( !exec )
			qDebug( ) << database->lastError( );
		return exec;
	}

	return false;
}
bool SQliteDepository::addItem( const QString &tab_name, const QStringList &item_name, const QStringList &item_value, const QString &where ) const {
	if( database ) {
		if( item_name.size( ) == 0 || item_value.size( ) == 0 )
			return false;
		if( where.isEmpty( ) )
			return addItem( tab_name, item_name, item_value );

		QString cmd = R"(INSERT INTO `)";
		cmd.append( tab_name ).append( "` ( `" );
		auto iterator = tab_name.begin( );
		auto end = tab_name.end( );
		do {
			cmd.append( "`" );
			cmd.append( *iterator );
			cmd.append( "` " );
			++iterator;
			if( iterator == end )
				break;
			cmd.append( ", `" );
		} while( true );
		cmd.append( ") VALUES (" );
		auto item_valueiterator = item_value.begin( );
		auto item_valueend = item_value.end( );
		do {
			cmd.append( "`" );
			cmd.append( *item_valueiterator );
			cmd.append( "` " );
			++item_valueiterator;
			if( item_valueiterator == item_valueend )
				break;
			cmd.append( ", `" );
		} while( true );
		cmd.append( " ) WHERE" );
		cmd.append( where );
		cmd.append( ";" );
		QSqlQuery query( *database );
		return query.exec( cmd );
	}
	return false;
}

bool cylDB::SQliteDepository::removeItem( const QString &tab_name, const QString &item_name ) const {
	return false;
}
bool SQliteDepository::updateItem( const QString &tab_name, QMap< QVariant, QVariant > var_map_s ) const {
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
